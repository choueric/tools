#ifndef _T_OMX_H_
#define _T_OMX_H_

#include <string.h>

//#define DEBUG_VERBOSE

#define BUFFER_NUM 20

#define fatal(fmt, args...) {\
  printf("\n*** fatal: " fmt, ##args);\
  exit(1);\
}

#define OMX_INIT_STRUCT(st) { \
  memset ((st), 0, sizeof (*(st))); \
  (st)->nSize = sizeof (*(st)); \
  (st)->nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
  (st)->nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
  (st)->nVersion.s.nRevision = OMX_VERSION_REVISION; \
  (st)->nVersion.s.nStep = OMX_VERSION_STEP; \
}
const char *omx_state_to_string(OMX_STATETYPE state);
const char *omx_error_to_string(OMX_ERRORTYPE err);
const char *omx_command_to_string (OMX_COMMANDTYPE cmd);

struct Comp {
  OMX_STRING name;
  OMX_HANDLETYPE handle;
  OMX_U32 in_port_index;
  OMX_PARAM_PORTDEFINITIONTYPE in_port_def;
  OMX_U32 out_port_index;
  OMX_PARAM_PORTDEFINITIONTYPE out_port_def;
  OMX_CALLBACKTYPE cbs;
  OMX_STATETYPE state;
  OMX_BUFFERHEADERTYPE *in_port_bufs[BUFFER_NUM];
  OMX_U32 in_bufs_num;
  OMX_BUFFERHEADERTYPE *out_port_bufs[BUFFER_NUM];
  OMX_U32 out_bufs_num;

  OMX_U32 fps;
};

OMX_ERRORTYPE GetParameter(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param);
OMX_ERRORTYPE SetParameter(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param);
void GetIndex(OMX_HANDLETYPE handle, OMX_STRING str, OMX_INDEXTYPE *index);

OMX_ERRORTYPE GetConfig(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param);
OMX_ERRORTYPE SetConfig(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param);

void ListAllComps();

/********** component related functions **********/
void CompGetRoles(struct Comp *comp);
void CompSetRole(struct Comp *comp, OMX_STRING role);

/*
 * get handle and store in @comp
 * set callbacks in @comp, set appData to @comp into core.
 */
int CompGetHandle(struct Comp *comp);

/*
 * get state of component and store in @comp
 */
OMX_STATETYPE CompGetState(struct Comp *comp);
void CompSetState(struct Comp *comp, OMX_STATETYPE state);

/*
 * get index of in_port and out_port, and store in @comp
 */
int CompGetPorts(struct Comp *comp);

/*
 * get port definition specified by port index @index
 * and store in @comp
 */
int CompGetPortDefinition(struct Comp *comp, OMX_U32 index,
	OMX_PARAM_PORTDEFINITIONTYPE *port_def);

void PrintPortDef(OMX_PARAM_PORTDEFINITIONTYPE *def);

int CompSetPortDefinition(struct Comp *comp, OMX_PARAM_PORTDEFINITIONTYPE *port_def);

void CompUpdatePortDefinition(struct Comp *comp, OMX_U32 index,
	OMX_PARAM_PORTDEFINITIONTYPE *port_def);

#define ENCODER_CONF_LOCATION   "/etc/enctune.conf"
void CompSetEncConfLocation(struct Comp *comp);

void CompSetQuant(struct Comp *comp, OMX_U32 quant_i_frames,
	OMX_U32 quant_p_frames, OMX_U32 quant_b_frames);

void CompGetSupportedColorformats(struct Comp *comp);
void doCompGetSupportedColorformats(struct Comp *comp);
void CompSetColorFormat(struct Comp *comp, OMX_U32 port_index, OMX_COLOR_FORMATTYPE f);

OMX_U32 CalculateBufferSize(OMX_PARAM_PORTDEFINITIONTYPE *port_def);

/*
 * @in: in port is true, false is out port
 * @n: buffer number
 */
OMX_ERRORTYPE CompAllocBuffer(struct Comp *comp, OMX_BOOL in, OMX_U32 n);
OMX_ERRORTYPE ClientAllocBuffer(struct Comp *comp, OMX_BOOL in, OMX_U32 n);


/* @port_index: in port index */
void CompUseNvMem(struct Comp *comp, OMX_U32 port_index);

/*
 * for in port
 * @rc_mode: NVX_VIDEO_RateControlMode_CBR, NVX_VIDEO_RateControlMode_VBR, 
 * 			 NVX_VIDEO_RateControlMode_VBR2(default)
 */
void CompSetRcMode(struct Comp *comp, OMX_U32 rc_mode);

void CompSetBitrate(struct Comp *comp, OMX_U32 port_index, OMX_U32 bitrate);

void CompGetIntraPeriod(struct Comp *comp, OMX_U32 port_index, OMX_VIDEO_CONFIG_AVCINTRAPERIOD *data);
void CompSetIntraPeriod(struct Comp *comp, OMX_U32 val, OMX_VIDEO_CONFIG_AVCINTRAPERIOD *data);

void PortSetEnable(struct Comp *comp, OMX_U32 port_index, OMX_BOOL enable);
OMX_BOOL PortIsEnabled(struct Comp *comp, OMX_U32 port_index);
void PortFlush(struct Comp *comp, OMX_U32 port_index);

void PrintBufferHead(OMX_BUFFERHEADERTYPE *p);
void PrintPortStatus(struct Comp *comp, int is_in);
#endif
