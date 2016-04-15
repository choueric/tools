#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <OMX_Core.h>
#include <OMX_Component.h>

#include "omx.h"

#define ENCODER_NAME "OMX.XYZ.h264.encoder"

#define RAW_VIDEO_FILE "./rawnv12"
#define OUTPUT_FILE "./output.h264"

static int infd = -1;
static int outfd = -1;

static void read_raw_data(OMX_BUFFERHEADERTYPE *bufhead, int fd)
{
  ssize_t size;
  OMX_U8 *buf;

  buf = bufhead->pBuffer;
  size = read(fd, buf, bufhead->nAllocLen);
  if (size <= 0) {
	lseek(fd, 0, SEEK_SET);
	size = read(fd, buf, bufhead->nAllocLen);
	if (size <= 0)
	  fatal("read size = %d\n", size);
  }
  bufhead->nFilledLen = size;
  bufhead->nOffset = 0;
  bufhead->nTimeStamp = 0;
  bufhead->nFlags = 0;
}

static void write_encoded_data(OMX_BUFFERHEADERTYPE *bufhead, int fd)
{
  //PrintBufferHead(bufhead);
  write(fd, bufhead->pBuffer + bufhead->nOffset, bufhead->nFilledLen);
}

////////////////////////////////////////////////////////////////////////////////

static OMX_ERRORTYPE EventHandlerCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_COMMANDTYPE cmd;
  OMX_STATETYPE state;
  OMX_U32 port;
  OMX_ERRORTYPE err;
  OMX_U32 index;
  OMX_U32 flags;

  printf("********** Event of [%s] **************\n", comp->name);

  switch (eEvent) {
	case OMX_EventCmdComplete:
	  cmd = (OMX_COMMANDTYPE) nData1;
	  printf("%s: cmd %s(%d) complete\n", omx_cmd2str(cmd), cmd);
	  switch (cmd) {
		case OMX_CommandStateSet:
		  state = nData2;
		  printf("    state to %s OK\n", omx_state2str(state));
		  break;
		case OMX_CommandFlush:
		  port = nData2;
		  printf("    port %lu flushed\n", port);
		  break;
		case OMX_CommandPortEnable:
		  port = nData2;
		  printf("    port %lu enabled\n", port);
		  break;
		case OMX_CommandPortDisable:
		  port = nData2;
		  printf("    port %lu disabled\n", port);
		  break;
		default:
		  printf("    unknown cmd\n");
		  break;
	  }
	  break;
	case OMX_EventError:
	  err = nData1;
	  printf("%s got error: %s(0x%08x)\n", comp->name, omx_err2str (err), err);
	  break;
	case OMX_EventPortSettingsChanged:
	  index = nData1;
	  printf("%s settings changed (port index: %ld)\n", comp->name, index);
	  break;
	case OMX_EventBufferFlag:
	  port = nData1;
	  flags = nData2;
	  printf("%s port %lu got buffer flags 0x%x\n", comp->name, port, flags);
	  break;
	case OMX_EventPortFormatDetected:
	  printf("fomat detected event\n");
	  break;
	default:
	  printf("%s unknown event 0x%08x\n", comp->name, eEvent);
	  break;
  }
  printf("******************************************************\n");
  return OMX_ErrorNone;
}

static OMX_ERRORTYPE EmptyBufferDoneCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE* bufhead)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_ERRORTYPE err = OMX_ErrorNone;

  // Lock
  read_raw_data(bufhead, fd);
  err = OMX_EmptyThisBuffer(comp->handle, bufhead);
  if (err != OMX_ErrorNone) {
	  fatal("%s failed :%s (0x%08x)\n", __func__, omx_err2str (err), err);
  }
  // UnLock

  return OMX_ErrorNone;
}

static OMX_ERRORTYPE FillBufferDoneCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE* bufhead)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_ERRORTYPE err = OMX_ErrorNone;

  write_encoded_data(bufhead, outfd);
  err = OMX_FillThisBuffer(comp->handle, bufhead);
  if (err != OMX_ErrorNone) {
	fatal("%s failed :%s (0x%08x)\n", __func__, omx_err2str (err), err);
  }

  return OMX_ErrorNone;
}

////////////////////////////////////////////////////////////////////////////////

static int initEncComp(struct Comp *comp)
{
  comp->name = ENCODER_NAME;
  comp->cbs.EventHandler = EventHandlerCb;
  comp->cbs.EmptyBufferDone = EmptyBufferDoneCb;
  comp->cbs.FillBufferDone = FillBufferDoneCb;

  comp->fps = 30;
  comp->width = 1920;
  comp->height = 1080;
  comp->inFormat = OMX_COLOR_FormatYUV420SemiPlanar;  // NV12

  return 0;
}

static void setInPortDefinition(struct Comp *comp)
{
  OMX_PARAM_PORTDEFINITIONTYPE *port_def = &comp->in_port_def;

  CompGetPortDefinition(comp, comp->in_port_index, port_def);

  port_def->format.video.eColorFormat = comp->inFormat;
  port_def->format.video.nFrameWidth = comp->width;
  port_def->format.video.nStride = comp->width;
  port_def->format.video.nFrameHeight = comp->height;
  port_def->format.video.nSliceHeight = comp->height;
  port_def->format.video.xFramerate = comp->fps;
  port_def->nBufferSize = CalculateBufferSize(port_def);

  CompUpdatePortDefinition(comp, comp->in_port_index, port_def);
  if (!PortIsEnabled(comp, comp->in_port_index)) {
	PortSetEnable(comp, comp->in_port_index, OMX_TRUE);
  }
}

static void setOutPortDefinition(struct Comp *comp)
{
  OMX_PARAM_PORTDEFINITIONTYPE *port_def = &comp->out_port_def;
  OMX_U32 port_index = comp->out_port_index;

  OMX_VIDEO_CONFIG_AVCINTRAPERIOD intraPeriod;
  OMX_U32 val = 0;
  
  CompGetPortDefinition(comp, port_index, port_def);
  port_def->format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
  CompUpdatePortDefinition(comp, comp->out_port_index, port_def);

  // IDR frame intervals
  CompGetIntraPeriod(comp, port_index, &intraPeriod);
  CompSetIntraPeriod(comp, val, &intraPeriod);
}

static void main_loop(struct Comp *comp)
{
  int i = 0;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_U32 count = comp->in_bufs_num;
  OMX_BUFFERHEADERTYPE *bufhead;

  printf("\nstart main loop\n");

  infd = open(RAW_VIDEO_FILE, O_RDONLY);
  if (infd < 0) {
	printf("open failed: %m\n");
	return;
  }

  outfd = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_TRUNC);
  if (outfd < 0) {
	printf("open output failed: %m\n");
	close(infd);
	return;
  }

  for (i = 0; i < count; i++) {
	bufhead = comp->in_port_bufs[i];
	read_raw_data(bufhead, infd);
	err = OMX_EmptyThisBuffer(comp->handle, bufhead);
	if (err != OMX_ErrorNone) {
	  fatal("%s failed :%s (0x%08x)\n", __func__, omx_err2str (err), err);
	}
  }

  while (1) {
	sleep(1);
  }

  close(infd);
  close(outfd);
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
  OMX_ERRORTYPE err;
  struct Comp encComp;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;

  initEncComp(&encComp);

  if ((err = OMX_Init ()) != OMX_ErrorNone) {
	printf("Failed to initialize core: %d\n", err);
	return -1;
  }

  CompGetRoles(&encComp); 
  CompGetHandle(&encComp);
  CompSetRole(&encComp, "video_encoder.avc");

  // <Loaded>
  encComp.state = CompGetState(&encComp); 

  /* get ports */
  CompGetPorts(&encComp);

  CompGetPortDefinition(&encComp, encComp.in_port_index, &encComp.in_port_def);
  CompGetPortDefinition(&encComp, encComp.out_port_index, &encComp.out_port_def);

  CompSetEncConfLocation(&encComp);
  CompGetSupportedColorformats(&encComp);

  setInPortDefinition(&encComp);
  setOutPortDefinition(&encComp);

  CompSetRcMode(&encComp, NVX_VIDEO_RateControlMode_VBR2);
  CompSetBitrate(&encComp, encComp.in_port_index, 4000000);

  CompGetPortDefinition(&encComp, encComp.in_port_index, &port_def);
  PrintPortDef(&port_def);

  // <Idle>
  CompSetState(&encComp, OMX_StateIdle);

  // allocate buffer of in port
  CompAllocBuffer(&encComp, OMX_TRUE, -1);

WaitIdle:
  sleep(1);
  encComp.state = CompGetState(&encComp);
  if (encComp.state != OMX_StateIdle) {
	printf("---------- should be Idle ----------\n");
	goto WaitIdle;
  }

  // <Executing>
  CompSetState(&encComp, OMX_StateExecuting);
WaitExecuting:
  sleep(1);
  encComp.state = CompGetState(&encComp);
  if (encComp.state != OMX_StateExecuting) {
	printf("---------- should be Idle ----------\n");
	goto WaitExecuting;
  }

  CompAllocBuffer(comp, OMX_FALSE, -1);
  for (i = 0; i < comp->out_bufs_num; i++) {
	buf = comp->out_port_bufs[i];
	buf->nFlags = 0;
	err = OMX_FillThisBuffer(comp->handle, buf);
	if (err != OMX_ErrorNone) {
	  fatal("Failed to pass buffer %p to %s port %lu: %s (0x%08x)\n",
		  buf->pBuffer, comp->name, port_index, omx_err2str (err), err);
	}
	printf("Passed buffer %p to component %s\n", buf->pBuffer, comp->name);
  }

  main_loop(&encComp);

  // TODO: free buffer, deinitialize, etc.
  OMX_Deinit();

  return 0;
}
