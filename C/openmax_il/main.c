#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <OMX_Core.h>
#include <OMX_Component.h>

#include <NVOMX_ParserExtensions.h>
#include <NVOMX_ColorFormatExtensions.h>
#include <NVOMX_IndexExtensions.h>

#include "omx.h"
#include "statistic.h"

//#define OUTPUT

#define RAW_VIDEO_FILE "./rawnv12"
#define OUTPUT_FILE "./output.h264"

static int fd = -1;
#ifdef OUTPUT
static int outfd = -1;
#endif

static struct statistic g_stat;

static void read_raw_data(OMX_BUFFERHEADERTYPE *bufhead, int fd)
{
  ssize_t size;
  OMX_U8 *buf;

  buf = bufhead->pBuffer;
  bufhead->nFlags = 0;
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

  //PrintBufferHead(bufhead);
}

static OMX_ERRORTYPE EventHandlerCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_COMMANDTYPE cmd;
  OMX_STATETYPE state;
  OMX_U32 port;
  OMX_ERRORTYPE error;
  OMX_U32 index;
  OMX_U32 flags;

#ifdef DEBUG_VERBOSE
  printf("********** %s() **************\n", __func__);
#endif

  switch (eEvent) {
	case OMX_EventCmdComplete:
	  cmd = (OMX_COMMANDTYPE) nData1;
#ifdef DEBUG_VERBOSE
	  printf("%s: %s(%d) command complete\n", comp->name,
		  omx_command_to_string (cmd), cmd);
#endif
	  switch (cmd) {
		case OMX_CommandStateSet:
		  state = nData2;
#ifdef DEBUG_VERBOSE
		  printf("  %s state change to %s finished\n",
			  comp->name, omx_state_to_string (state));
#endif
		  break;
		case OMX_CommandFlush:
		  port = nData2;
#ifdef DEBUG_VERBOSE
		  printf("  %s port %lu flushed\n", comp->name, port);
#endif
		  break;
		case OMX_CommandPortEnable:
		  port = nData2;
#ifdef DEBUG_VERBOSE
		  printf("  %s port %lu enabled\n", comp->name, port);
#endif
		  break;
		case OMX_CommandPortDisable:
		  port = nData2;
#ifdef DEBUG_VERBOSE
		  printf("  %s port %lu disabled\n", comp->name, port);
#endif
		  break;
		  break;
		default:
		  break;
	  }
	  break;
	case OMX_EventError:
	  /* Yes, this really happens... */
	  if (nData1 == OMX_ErrorNone)
		break;

	  error = nData1;
#ifdef DEBUG_VERBOSE
	  printf("%s got error: %s (0x%08x)\n", comp->name,
		  omx_error_to_string (error), error);
#endif
	  break;
	case OMX_EventPortSettingsChanged:
	  index = nData1;
	  //index = nData2;
#if 0
	  if (index == 0)
		index = 1;
#endif
#ifdef DEBUG_VERBOSE
	  printf("%s settings changed (port index: %ld)\n",
		  comp->name, index);
#endif
	  break;
	case OMX_EventBufferFlag:
	  port = nData1;
	  flags = nData2;
#ifdef DEBUG_VERBOSE
	  printf("%s port %lu got buffer flags 0x%x\n",
		  comp->name, port, (unsigned int)flags);
#endif
	  break;
	case OMX_EventPortFormatDetected:
#ifdef DEBUG_VERBOSE
	  printf("fomat detected event\n");
#endif
	  break;
	default:
#ifdef DEBUG_VERBOSE
	  printf("%s unknown event 0x%08x\n", comp->name,
		  eEvent);
#endif
	  break;
  }
#ifdef DEBUG_VERBOSE
  printf("******************************************************\n");
#endif
  return OMX_ErrorNone;
}

#if 0
static OMX_ERRORTYPE EmptyBufferDoneCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE* pBuffer)
{
  struct Comp *comp = (struct Comp *)pAppData;

  int i;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_U32 count = comp->in_bufs_num;
  OMX_BUFFERHEADERTYPE *bufhead;

  static int number = 0;
  //printf("========== %s() ==============\n", __func__);

  number++;
  if (number != count)
	return OMX_ErrorNone;

  number = 0;
  for (i = 0; i < count; i++) {
	bufhead = comp->in_port_bufs[i];
	read_raw_data(bufhead, fd);
	err = OMX_EmptyThisBuffer(comp->handle, bufhead);
	if (err != OMX_ErrorNone) {
	  fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);
	}
  }

  //printf("======================================================\n");
  return OMX_ErrorNone;
}
#else
static OMX_ERRORTYPE EmptyBufferDoneCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE* pBuffer)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_ERRORTYPE err = OMX_ErrorNone;

  err = OMX_EmptyThisBuffer(comp->handle, pBuffer);
  if (err != OMX_ErrorNone) {
	fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);
  }

  return OMX_ErrorNone;
}
#endif

static OMX_ERRORTYPE FillBufferDoneCb(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE* bufhead)
{
  struct Comp *comp = (struct Comp *)pAppData;
  OMX_ERRORTYPE err = OMX_ErrorNone;

  //printf("%s(): filled len = %lu\n", __func__, bufhead->nFilledLen);
#ifdef OUTPUT
  write(outfd, bufhead->pBuffer + bufhead->nOffset, bufhead->nFilledLen);
#endif

  print_statistic(&g_stat);

  err = OMX_FillThisBuffer(comp->handle, bufhead);
  if (err != OMX_ErrorNone) {
	fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);
  }

  return OMX_ErrorNone;
}

////////////////////////////////////////////////////////////////////////////////

static int init_enc_comp(struct Comp *comp, OMX_U32 fps)
{
  comp->name = "OMX.Nvidia.h264.encoder";
  comp->cbs.EventHandler = EventHandlerCb;
  comp->cbs.EmptyBufferDone = EmptyBufferDoneCb;
  comp->cbs.FillBufferDone = FillBufferDoneCb;
  comp->fps = fps;

  return 0;
}

static void setInPortDefinition(struct Comp *comp)
{
  OMX_U32 width = 1920, height = 1080;
  OMX_PARAM_PORTDEFINITIONTYPE *port_def = &comp->in_port_def;

  CompGetPortDefinition(comp, comp->in_port_index, port_def);

  /* color format: NV12 */
  port_def->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
  port_def->format.video.nFrameWidth = width;
  port_def->format.video.nStride = width;
  port_def->format.video.nFrameHeight = height;
  port_def->format.video.nSliceHeight = height;
  port_def->nBufferSize = CalculateBufferSize(port_def);

  port_def->format.video.xFramerate = (comp->fps << 16);

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

static void popolate_out_port(struct Comp *comp)
{
  OMX_U32 i;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_BUFFERHEADERTYPE *buf;
  OMX_U32 port_index = comp->out_port_index;

  // populate
#ifdef DEBUG_VERBOSE
  printf("---- populate out port ---- \n");
#endif
  for (i = 0; i < comp->out_bufs_num; i++) {
	buf = comp->out_port_bufs[i];
	buf->nFlags = 0;
	err = OMX_FillThisBuffer(comp->handle, buf);
	if (err != OMX_ErrorNone) {
	  fatal("Failed to pass buffer %p to %s port %lu: %s (0x%08x)\n",
		  buf->pBuffer, comp->name, port_index, omx_error_to_string (err), err);
	}
#ifdef DEBUG_VERBOSE
	printf("Passed buffer %p to component %s\n", buf->pBuffer, comp->name);
#endif
  }
  sleep(1);
}

static void enc_reconfigure_output_port(struct Comp *comp) {
  OMX_U32 port_index = comp->out_port_index;

  if (!PortIsEnabled(comp, port_index)) {
	PortSetEnable(comp, port_index, OMX_TRUE);
	sleep(1);
  }

#if 1
  CompAllocBuffer(comp, OMX_FALSE, -1);
#else
  ClientAllocBuffer(comp, OMX_FALSE, -1);
#endif
  sleep(1);
}

static void main_loop(struct Comp *comp)
{
  int i = 0;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_U32 count = comp->in_bufs_num;
  OMX_BUFFERHEADERTYPE *bufhead;

  printf("\nstart main loop\n");

  init_statistic(&g_stat, "filldone", 5000000);

  fd = open(RAW_VIDEO_FILE, O_RDONLY);
  if (fd < 0) {
	printf("open failed: %m\n");
	return;
  }

#ifdef OUTPUT
  outfd = open(OUTPUT_FILE, O_RDWR | O_CREAT | O_TRUNC);
  if (outfd < 0) {
	printf("open output failed: %m\n");
	return;
  }
#endif

  for (i = 0; i < count; i++) {
	bufhead = comp->in_port_bufs[i];
	read_raw_data(bufhead, fd);
	err = OMX_EmptyThisBuffer(comp->handle, bufhead);
	if (err != OMX_ErrorNone) {
	  fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);
	}
  }

  while (1) {
	sleep(1);
  }
#ifdef OUTPUT
  close(outfd);
#endif
  close(fd);
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
  OMX_ERRORTYPE err;
  struct Comp encComp;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;
  OMX_U32 fps = 30;

  if (argc == 2) {
	  fps = atoi(argv[1]);
  }

  init_enc_comp(&encComp, fps);

  if ((err = OMX_Init ()) != OMX_ErrorNone) {
	printf("Failed to initialize core: %d\n", err);
	return -1;
  }
  printf("\n--------------------------------------------\n");
  printf("fps set to %lu\n", fps);

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

  // configure in port
  setInPortDefinition(&encComp);

  // configure out port
  setOutPortDefinition(&encComp);

  CompGetPortDefinition(&encComp, encComp.in_port_index, &encComp.in_port_def);
  CompGetPortDefinition(&encComp, encComp.out_port_index, &encComp.out_port_def);

  CompSetRcMode(&encComp, NVX_VIDEO_RateControlMode_VBR2);
  CompSetBitrate(&encComp, encComp.in_port_index, 4000000);
  CompGetPortDefinition(&encComp, encComp.in_port_index, &port_def);
  PrintPortDef(&port_def);

#if 1
  PortSetEnable(&encComp, encComp.out_port_index, OMX_FALSE);
  sleep(1);
#endif

  // <Idle>
  CompSetState(&encComp, OMX_StateIdle);

  // allocate buffer of in port
#if 1
  CompAllocBuffer(&encComp, OMX_TRUE, -1);
#else
  ClientAllocBuffer(&encComp, OMX_TRUE, -1);
#endif

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

  enc_reconfigure_output_port(&encComp);
  popolate_out_port(&encComp);

  //PrintPortStatus(&encComp, 0);

  main_loop(&encComp);

  OMX_Deinit();

  return 0;
}
