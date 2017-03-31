#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <OMX_Core.h>
#include <OMX_Component.h>
#include <NVOMX_ParserExtensions.h>
#include <NVOMX_ColorFormatExtensions.h>
#include <NVOMX_IndexExtensions.h>

#include "omx.h"

const char *
omx_index_to_string (OMX_INDEXTYPE index)
{
	switch (index) {
		case OMX_IndexParamPriorityMgmt: return "OMX_IndexParamPriorityMgmt";
		case OMX_IndexParamAudioInit: return "OMX_IndexParamAudioInit";
		case OMX_IndexParamImageInit: return "OMX_IndexParamImageInit";
		case OMX_IndexParamVideoInit: return "OMX_IndexParamVideoInit";
		case OMX_IndexParamOtherInit: return "OMX_IndexParamOtherInit";
		case OMX_IndexParamNumAvailableStreams: return "OMX_IndexParamNumAvailableStreams";
		case OMX_IndexParamActiveStream: return "OMX_IndexParamActiveStream";
		case OMX_IndexParamSuspensionPolicy: return "OMX_IndexParamSuspensionPolicy";
		case OMX_IndexParamComponentSuspended: return "OMX_IndexParamComponentSuspended";
		case OMX_IndexConfigCapturing: return "OMX_IndexConfigCapturing";
		case OMX_IndexConfigCaptureMode: return "OMX_IndexConfigCaptureMode";
		case OMX_IndexAutoPauseAfterCapture: return "OMX_IndexAutoPauseAfterCapture";
		case OMX_IndexParamContentURI: return "OMX_IndexParamContentURI";
		case OMX_IndexParamCustomContentPipe: return "OMX_IndexParamCustomContentPipe";
		case OMX_IndexParamDisableResourceConcealment: return "OMX_IndexParamDisableResourceConcealment";
		case OMX_IndexConfigMetadataItemCount: return "OMX_IndexConfigMetadataItemCount";
		case OMX_IndexConfigContainerNodeCount: return "OMX_IndexConfigContainerNodeCount";
		case OMX_IndexConfigMetadataItem: return "OMX_IndexConfigMetadataItem";
		case OMX_IndexConfigCounterNodeID: return "OMX_IndexConfigCounterNodeID";
		case OMX_IndexParamMetadataFilterType: return "OMX_IndexParamMetadataFilterType";
		case OMX_IndexParamMetadataKeyFilter: return "OMX_IndexParamMetadataKeyFilter";
		case OMX_IndexConfigPriorityMgmt: return "OMX_IndexConfigPriorityMgmt";
		case OMX_IndexParamStandardComponentRole: return "OMX_IndexParamStandardComponentRole";
		case OMX_IndexParamPortDefinition: return "OMX_IndexParamPortDefinition";
		case OMX_IndexParamCompBufferSupplier: return "OMX_IndexParamCompBufferSupplier";
		case OMX_IndexParamAudioPortFormat: return "OMX_IndexParamAudioPortFormat";
		case OMX_IndexParamAudioPcm: return "OMX_IndexParamAudioPcm";
		case OMX_IndexParamAudioAac: return "OMX_IndexParamAudioAac";
		case OMX_IndexParamAudioRa: return "OMX_IndexParamAudioRa";
		case OMX_IndexParamAudioMp3: return "OMX_IndexParamAudioMp3";
		case OMX_IndexParamAudioAdpcm: return "OMX_IndexParamAudioAdpcm";
		case OMX_IndexParamAudioG723: return "OMX_IndexParamAudioG723";
		case OMX_IndexParamAudioG729: return "OMX_IndexParamAudioG729";
		case OMX_IndexParamAudioAmr: return "OMX_IndexParamAudioAmr";
		case OMX_IndexParamAudioWma: return "OMX_IndexParamAudioWma";
		case OMX_IndexParamAudioSbc: return "OMX_IndexParamAudioSbc";
		case OMX_IndexParamAudioMidi: return "OMX_IndexParamAudioMidi";
		case OMX_IndexParamAudioGsm_FR: return "OMX_IndexParamAudioGsm_FR";
		case OMX_IndexParamAudioMidiLoadUserSound: return "OMX_IndexParamAudioMidiLoadUserSound";
		case OMX_IndexParamAudioG726: return "OMX_IndexParamAudioG726";
		case OMX_IndexParamAudioGsm_EFR: return "OMX_IndexParamAudioGsm_EFR";
		case OMX_IndexParamAudioGsm_HR: return "OMX_IndexParamAudioGsm_HR";
		case OMX_IndexParamAudioPdc_FR: return "OMX_IndexParamAudioPdc_FR";
		case OMX_IndexParamAudioPdc_EFR: return "OMX_IndexParamAudioPdc_EFR";
		case OMX_IndexParamAudioPdc_HR: return "OMX_IndexParamAudioPdc_HR";
		case OMX_IndexParamAudioTdma_FR: return "OMX_IndexParamAudioTdma_FR";
		case OMX_IndexParamAudioTdma_EFR: return "OMX_IndexParamAudioTdma_EFR";
		case OMX_IndexParamAudioQcelp8: return "OMX_IndexParamAudioQcelp8";
		case OMX_IndexParamAudioQcelp13: return "OMX_IndexParamAudioQcelp13";
		case OMX_IndexParamAudioEvrc: return "OMX_IndexParamAudioEvrc";
		case OMX_IndexParamAudioSmv: return "OMX_IndexParamAudioSmv";
		case OMX_IndexParamAudioVorbis: return "OMX_IndexParamAudioVorbis";
		case OMX_IndexConfigAudioMidiImmediateEvent: return "OMX_IndexConfigAudioMidiImmediateEvent";
		case OMX_IndexConfigAudioMidiControl: return "OMX_IndexConfigAudioMidiControl";
		case OMX_IndexConfigAudioMidiSoundBankProgram: return "OMX_IndexConfigAudioMidiSoundBankProgram";
		case OMX_IndexConfigAudioMidiStatus: return "OMX_IndexConfigAudioMidiStatus";
		case OMX_IndexConfigAudioMidiMetaEvent: return "OMX_IndexConfigAudioMidiMetaEvent";
		case OMX_IndexConfigAudioMidiMetaEventData: return "OMX_IndexConfigAudioMidiMetaEventData";
		case OMX_IndexConfigAudioVolume: return "OMX_IndexConfigAudioVolume";
		case OMX_IndexConfigAudioBalance: return "OMX_IndexConfigAudioBalance";
		case OMX_IndexConfigAudioChannelMute: return "OMX_IndexConfigAudioChannelMute";
		case OMX_IndexConfigAudioMute: return "OMX_IndexConfigAudioMute";
		case OMX_IndexConfigAudioLoudness: return "OMX_IndexConfigAudioLoudness";
		case OMX_IndexConfigAudioEchoCancelation: return "OMX_IndexConfigAudioEchoCancelation";
		case OMX_IndexConfigAudioNoiseReduction: return "OMX_IndexConfigAudioNoiseReduction";
		case OMX_IndexConfigAudioBass: return "OMX_IndexConfigAudioBass";
		case OMX_IndexConfigAudioTreble: return "OMX_IndexConfigAudioTreble";
		case OMX_IndexConfigAudioStereoWidening: return "OMX_IndexConfigAudioStereoWidening";
		case OMX_IndexConfigAudioChorus: return "OMX_IndexConfigAudioChorus";
		case OMX_IndexConfigAudioEqualizer: return "OMX_IndexConfigAudioEqualizer";
		case OMX_IndexConfigAudioReverberation: return "OMX_IndexConfigAudioReverberation";
		case OMX_IndexConfigAudioChannelVolume: return "OMX_IndexConfigAudioChannelVolume";
		case OMX_IndexParamImagePortFormat: return "OMX_IndexParamImagePortFormat";
		case OMX_IndexParamFlashControl: return "OMX_IndexParamFlashControl";
		case OMX_IndexConfigFocusControl: return "OMX_IndexConfigFocusControl";
		case OMX_IndexParamQFactor: return "OMX_IndexParamQFactor";
		case OMX_IndexParamQuantizationTable: return "OMX_IndexParamQuantizationTable";
		case OMX_IndexParamHuffmanTable: return "OMX_IndexParamHuffmanTable";
		case OMX_IndexConfigFlashControl: return "OMX_IndexConfigFlashControl";
		case OMX_IndexConfigCILThreshold: return "OMX_IndexConfigCILThreshold";
		case OMX_IndexParamVideoPortFormat: return "OMX_IndexParamVideoPortFormat";
		case OMX_IndexParamVideoQuantization: return "OMX_IndexParamVideoQuantization";
		case OMX_IndexParamVideoFastUpdate: return "OMX_IndexParamVideoFastUpdate";
		case OMX_IndexParamVideoBitrate: return "OMX_IndexParamVideoBitrate";
		case OMX_IndexParamVideoMotionVector: return "OMX_IndexParamVideoMotionVector";
		case OMX_IndexParamVideoIntraRefresh: return "OMX_IndexParamVideoIntraRefresh";
		case OMX_IndexParamVideoErrorCorrection: return "OMX_IndexParamVideoErrorCorrection";
		case OMX_IndexParamVideoVBSMC: return "OMX_IndexParamVideoVBSMC";
		case OMX_IndexParamVideoMpeg2: return "OMX_IndexParamVideoMpeg2";
		case OMX_IndexParamVideoMpeg4: return "OMX_IndexParamVideoMpeg4";
		case OMX_IndexParamVideoWmv: return "OMX_IndexParamVideoWmv";
		case OMX_IndexParamVideoRv: return "OMX_IndexParamVideoRv";
		case OMX_IndexParamVideoAvc: return "OMX_IndexParamVideoAvc";
		case OMX_IndexParamVideoH263: return "OMX_IndexParamVideoH263";
		case OMX_IndexParamVideoProfileLevelQuerySupported: return "OMX_IndexParamVideoProfileLevelQuerySupported";
		case OMX_IndexParamVideoProfileLevelCurrent: return "OMX_IndexParamVideoProfileLevelCurrent";
		case OMX_IndexConfigVideoBitrate: return "OMX_IndexConfigVideoBitrate";
		case OMX_IndexConfigVideoFramerate: return "OMX_IndexConfigVideoFramerate";
		case OMX_IndexConfigVideoIntraVOPRefresh: return "OMX_IndexConfigVideoIntraVOPRefresh";
		case OMX_IndexConfigVideoIntraMBRefresh: return "OMX_IndexConfigVideoIntraMBRefresh";
		case OMX_IndexConfigVideoMBErrorReporting: return "OMX_IndexConfigVideoMBErrorReporting";
		case OMX_IndexParamVideoMacroblocksPerFrame: return "OMX_IndexParamVideoMacroblocksPerFrame";
		case OMX_IndexConfigVideoMacroBlockErrorMap: return "OMX_IndexConfigVideoMacroBlockErrorMap";
		case OMX_IndexParamVideoSliceFMO: return "OMX_IndexParamVideoSliceFMO";
		case OMX_IndexConfigVideoAVCIntraPeriod: return "OMX_IndexConfigVideoAVCIntraPeriod";
		case OMX_IndexConfigVideoNalSize: return "OMX_IndexConfigVideoNalSize";
		case OMX_IndexParamCommonDeblocking: return "OMX_IndexParamCommonDeblocking";
		case OMX_IndexParamCommonSensorMode: return "OMX_IndexParamCommonSensorMode";
		case OMX_IndexParamCommonInterleave: return "OMX_IndexParamCommonInterleave";
		case OMX_IndexConfigCommonColorFormatConversion: return "OMX_IndexConfigCommonColorFormatConversion";
		case OMX_IndexConfigCommonScale: return "OMX_IndexConfigCommonScale";
		case OMX_IndexConfigCommonImageFilter: return "OMX_IndexConfigCommonImageFilter";
		case OMX_IndexConfigCommonColorEnhancement: return "OMX_IndexConfigCommonColorEnhancement";
		case OMX_IndexConfigCommonColorKey: return "OMX_IndexConfigCommonColorKey";
		case OMX_IndexConfigCommonColorBlend: return "OMX_IndexConfigCommonColorBlend";
		case OMX_IndexConfigCommonFrameStabilisation: return "OMX_IndexConfigCommonFrameStabilisation";
		case OMX_IndexConfigCommonRotate: return "OMX_IndexConfigCommonRotate";
		case OMX_IndexConfigCommonMirror: return "OMX_IndexConfigCommonMirror";
		case OMX_IndexConfigCommonOutputPosition: return "OMX_IndexConfigCommonOutputPosition";
		case OMX_IndexConfigCommonInputCrop: return "OMX_IndexConfigCommonInputCrop";
		case OMX_IndexConfigCommonOutputCrop: return "OMX_IndexConfigCommonOutputCrop";
		case OMX_IndexConfigCommonDigitalZoom: return "OMX_IndexConfigCommonDigitalZoom";
		case OMX_IndexConfigCommonOpticalZoom: return "OMX_IndexConfigCommonOpticalZoom";
		case OMX_IndexConfigCommonWhiteBalance: return "OMX_IndexConfigCommonWhiteBalance";
		case OMX_IndexConfigCommonExposure: return "OMX_IndexConfigCommonExposure";
		case OMX_IndexConfigCommonContrast: return "OMX_IndexConfigCommonContrast";
		case OMX_IndexConfigCommonBrightness: return "OMX_IndexConfigCommonBrightness";
		case OMX_IndexConfigCommonBacklight: return "OMX_IndexConfigCommonBacklight";
		case OMX_IndexConfigCommonGamma: return "OMX_IndexConfigCommonGamma";
		case OMX_IndexConfigCommonSaturation: return "OMX_IndexConfigCommonSaturation";
		case OMX_IndexConfigCommonLightness: return "OMX_IndexConfigCommonLightness";
		case OMX_IndexConfigCommonExclusionRect: return "OMX_IndexConfigCommonExclusionRect";
		case OMX_IndexConfigCommonDithering: return "OMX_IndexConfigCommonDithering";
		case OMX_IndexConfigCommonPlaneBlend: return "OMX_IndexConfigCommonPlaneBlend";
		case OMX_IndexConfigCommonExposureValue: return "OMX_IndexConfigCommonExposureValue";
		case OMX_IndexConfigCommonOutputSize: return "OMX_IndexConfigCommonOutputSize";
		case OMX_IndexParamCommonExtraQuantData: return "OMX_IndexParamCommonExtraQuantData";
		case OMX_IndexConfigCommonFocusRegion: return "OMX_IndexConfigCommonFocusRegion";
		case OMX_IndexConfigCommonFocusStatus: return "OMX_IndexConfigCommonFocusStatus";
		case OMX_IndexConfigCommonTransitionEffect: return "OMX_IndexConfigCommonTransitionEffect";
		case OMX_IndexParamOtherPortFormat: return "OMX_IndexParamOtherPortFormat";
		case OMX_IndexConfigOtherPower: return "OMX_IndexConfigOtherPower";
		case OMX_IndexConfigOtherStats: return "OMX_IndexConfigOtherStats";
		case OMX_IndexConfigTimeScale: return "OMX_IndexConfigTimeScale";
		case OMX_IndexConfigTimeClockState: return "OMX_IndexConfigTimeClockState";
		case OMX_IndexConfigTimeActiveRefClock: return "OMX_IndexConfigTimeActiveRefClock";
		case OMX_IndexConfigTimeCurrentMediaTime: return "OMX_IndexConfigTimeCurrentMediaTime";
		case OMX_IndexConfigTimeCurrentWallTime: return "OMX_IndexConfigTimeCurrentWallTime";
		case OMX_IndexConfigTimeCurrentAudioReference: return "OMX_IndexConfigTimeCurrentAudioReference";
		case OMX_IndexConfigTimeCurrentVideoReference: return "OMX_IndexConfigTimeCurrentVideoReference";
		case OMX_IndexConfigTimeMediaTimeRequest: return "OMX_IndexConfigTimeMediaTimeRequest";
		case OMX_IndexConfigTimeClientStartTime: return "OMX_IndexConfigTimeClientStartTime";
		case OMX_IndexConfigTimePosition: return "OMX_IndexConfigTimePosition";
		case OMX_IndexConfigTimeSeekMode: return "OMX_IndexConfigTimeSeekMode";
		case OMX_IndexConfigCommitMode: return "OMX_IndexConfigCommitMode";
		case OMX_IndexConfigCommit: return "OMX_IndexConfigCommit";
		case OMX_IndexConfigCallbackRequest: return "OMX_IndexConfigCallbackRequest";
		case OMX_IndexParamMediaContainer: return "OMX_IndexParamMediaContainer";
		case OMX_IndexParamReadOnlyBuffers: return "OMX_IndexParamReadOnlyBuffers";
		default:return "unknow";
	}
}


const char *omx_compress_format_to_string(OMX_VIDEO_CODINGTYPE f)
{
  switch (f) {
	case OMX_VIDEO_CodingUnused:
	  return "N/A";
	case OMX_VIDEO_CodingAutoDetect:
	  return "Autodetection";
	case OMX_VIDEO_CodingMPEG2:
	  return "H.262";
	case OMX_VIDEO_CodingH263:
	  return "H.263";
	case OMX_VIDEO_CodingMPEG4:
	  return "MPEG-4";
	case OMX_VIDEO_CodingWMV:
	  return "WMV";
	case OMX_VIDEO_CodingRV:
	  return "Real Video";
	case OMX_VIDEO_CodingAVC:
	  return "H.264/AVC";
	case OMX_VIDEO_CodingMJPEG:
	  return "Motion JPEG";
	default:
	  return "Others";
  }
}

const char *omx_color_format_to_string(OMX_COLOR_FORMATTYPE f)
{
  switch ((int)f) {
	case OMX_COLOR_FormatUnused:
	  return "Unused";
	case OMX_COLOR_FormatMonochrome:
	  return "Monochrome";
	case OMX_COLOR_Format8bitRGB332:
	  return "8bitRGB332";
	case OMX_COLOR_Format12bitRGB444:
	  return "12bitRGB444";
	case OMX_COLOR_Format16bitARGB4444:
	  return "16bitARGB4444";
	case OMX_COLOR_Format16bitARGB1555:
	  return "16bitARGB1555";
	case OMX_COLOR_Format16bitRGB565:
	  return "16bitRGB565";
	case OMX_COLOR_Format16bitBGR565:
	  return "16bitBGR565";
	case OMX_COLOR_Format18bitRGB666:
	  return "18bitRGB666";
	case OMX_COLOR_Format18bitARGB1665:
	  return "18bitARGB1665";
	case OMX_COLOR_Format19bitARGB1666:
	  return "19bitARGB1666";
	case OMX_COLOR_Format24bitRGB888:
	  return "24bitRGB888";
	case OMX_COLOR_Format24bitBGR888:
	  return "24bitBGR888";
	case OMX_COLOR_Format24bitARGB1887:
	  return "24bitARGB1887";
	case OMX_COLOR_Format25bitARGB1888:
	  return "25bitARGB1888";
	case OMX_COLOR_Format32bitBGRA8888:
	  return "32bitBGRA8888";
	case OMX_COLOR_Format32bitARGB8888:
	  return "32bitARGB8888";
	case OMX_COLOR_FormatYUV411Planar:
	  return "YUV411Planar";
	case OMX_COLOR_FormatYUV411PackedPlanar:
	  return "YUV411PackedPlanar";
	case OMX_COLOR_FormatYUV420Planar:
	  return "YUV420Planar";
	case OMX_COLOR_FormatYUV420PackedPlanar:
	  return "YUV420PackedPlanar";
	case OMX_COLOR_FormatYUV420SemiPlanar:
	  return "YUV420SemiPlanar";
	case OMX_COLOR_FormatYUV422Planar:
	  return "YUV422Planar";
	case OMX_COLOR_FormatYUV422PackedPlanar:
	  return "YUV422PackedPlanar";
	case OMX_COLOR_FormatYUV422SemiPlanar:
	  return "YUV422SemiPlanar";
	case OMX_COLOR_FormatYCbYCr:
	  return "YCbYCr";
	case OMX_COLOR_FormatYCrYCb:
	  return "YCrYCb";
	case OMX_COLOR_FormatCbYCrY:
	  return "CbYCrY";
	case OMX_COLOR_FormatCrYCbY:
	  return "CrYCbY";
	case OMX_COLOR_FormatYUV444Interleaved:
	  return "YUV444Interleaved";
	case OMX_COLOR_FormatRawBayer8bit:
	  return "RawBayer8bit";
	case OMX_COLOR_FormatRawBayer10bit:
	  return "RawBayer10bit";
	case OMX_COLOR_FormatRawBayer8bitcompressed:
	  return "RawBayer8bitcompressed";
	case OMX_COLOR_FormatL2:
	  return "L2";
	case OMX_COLOR_FormatL4:
	  return "L4";
	case OMX_COLOR_FormatL8:
	  return "L8";
	case OMX_COLOR_FormatL16:
	  return "L16";
	case OMX_COLOR_FormatL24:
	  return "L24";
	case OMX_COLOR_FormatL32:
	  return "L32";
	case OMX_COLOR_FormatYUV420PackedSemiPlanar:
	  return "YUV420PackedSemiPlanar";
	case OMX_COLOR_FormatYUV422PackedSemiPlanar:
	  return "YUV422PackedSemiPlanar";
	case OMX_COLOR_Format18BitBGR666:
	  return "18BitBGR666";
	case OMX_COLOR_Format24BitARGB6666:
	  return "24BitARGB6666";
	case OMX_COLOR_Format24BitABGR6666:
	  return "24BitABGR6666";
	case OMX_COLOR_FormatAndroidOpaque:
	  return "AndroidOpaque";
	default:
	  return "N/A";
  }
}

const char *omx_error_to_string(OMX_ERRORTYPE err)
{
  switch (err) {
	case OMX_ErrorNone:
	  return "None";
	case OMX_ErrorInsufficientResources:
	  return "Insufficient resources";
	case OMX_ErrorUndefined:
	  return "Undefined";
	case OMX_ErrorInvalidComponentName:
	  return "Invalid component name";
	case OMX_ErrorComponentNotFound:
	  return "Component not found";
	case OMX_ErrorInvalidComponent:
	  return "Invalid component";
	case OMX_ErrorBadParameter:
	  return "Bad parameter";
	case OMX_ErrorNotImplemented:
	  return "Not implemented";
	case OMX_ErrorUnderflow:
	  return "Underflow";
	case OMX_ErrorOverflow:
	  return "Overflow";
	case OMX_ErrorHardware:
	  return "Hardware";
	case OMX_ErrorInvalidState:
	  return "Invalid state";
	case OMX_ErrorStreamCorrupt:
	  return "Stream corrupt";
	case OMX_ErrorPortsNotCompatible:
	  return "Ports not compatible";
	case OMX_ErrorResourcesLost:
	  return "Resources lost";
	case OMX_ErrorNoMore:
	  return "No more";
	case OMX_ErrorVersionMismatch:
	  return "Version mismatch";
	case OMX_ErrorNotReady:
	  return "Not ready";
	case OMX_ErrorTimeout:
	  return "Timeout";
	case OMX_ErrorSameState:
	  return "Same state";
	case OMX_ErrorResourcesPreempted:
	  return "Resources preempted";
	case OMX_ErrorPortUnresponsiveDuringAllocation:
	  return "Port unresponsive during allocation";
	case OMX_ErrorPortUnresponsiveDuringDeallocation:
	  return "Port unresponsive during deallocation";
	case OMX_ErrorPortUnresponsiveDuringStop:
	  return "Port unresponsive during stop";
	case OMX_ErrorIncorrectStateTransition:
	  return "Incorrect state transition";
	case OMX_ErrorIncorrectStateOperation:
	  return "Incorrect state operation";
	case OMX_ErrorUnsupportedSetting:
	  return "Unsupported setting";
	case OMX_ErrorUnsupportedIndex:
	  return "Unsupported index";
	case OMX_ErrorBadPortIndex:
	  return "Bad port index";
	case OMX_ErrorPortUnpopulated:
	  return "Port unpopulated";
	case OMX_ErrorComponentSuspended:
	  return "Component suspended";
	case OMX_ErrorDynamicResourcesUnavailable:
	  return "Dynamic resources unavailable";
	case OMX_ErrorMbErrorsInFrame:
	  return "Macroblock errors in frame";
	case OMX_ErrorFormatNotDetected:
	  return "Format not detected";
	case OMX_ErrorContentPipeOpenFailed:
	  return "Content pipe open failed";
	case OMX_ErrorContentPipeCreationFailed:
	  return "Content pipe creation failed";
	case OMX_ErrorSeperateTablesUsed:
	  return "Separate tables used";
	case OMX_ErrorTunnelingUnsupported:
	  return "Tunneling unsupported";
	default:
	  if (err >= OMX_ErrorKhronosExtensions
		  && err < OMX_ErrorVendorStartUnused) {
		return "Khronos extension error";
	  } else if (err >= OMX_ErrorVendorStartUnused
		  && err < OMX_ErrorMax) {
		return "Vendor specific error";
	  } else {
		return "Unknown error";
	  }
  }
}

const char *omx_state_to_string(OMX_STATETYPE state)
{
  switch (state) {
	case OMX_StateInvalid:
	  return "Invalid";
	case OMX_StateLoaded:
	  return "Loaded";
	case OMX_StateIdle:
	  return "Idle";
	case OMX_StateExecuting:
	  return "Executing";
	case OMX_StatePause:
	  return "Pause";
	case OMX_StateWaitForResources:
	  return "WaitForResources";
	default:
	  if (state >= OMX_StateKhronosExtensions)
		return "KhronosExtensionState";
	  else if (state >= OMX_StateVendorStartUnused)
		return "CustomVendorState";
	  break;
  }
  return "Unknown state";
}

const char *omx_command_to_string (OMX_COMMANDTYPE cmd)
{
  switch (cmd) {
	case OMX_CommandStateSet:
	  return "SetState";
	case OMX_CommandFlush:
	  return "Flush";
	case OMX_CommandPortDisable:
	  return "DisablePort";
	case OMX_CommandPortEnable:
	  return "EnablePort";
	case OMX_CommandMarkBuffer:
	  return "MarkBuffer";
	default:
	  if (cmd >= OMX_CommandKhronosExtensions)
		return "KhronosExtensionCommand";
	  else if (cmd >= OMX_CommandVendorStartUnused)
		return "VendorExtensionCommand";
	  break;
  }
  return "Unknown command";
}

////////////////////////////////////////////////////////////////////////////////

void do_get_roles(OMX_STRING comp_name)
{
  OMX_U32 i;
  OMX_U32 nroles;
  OMX_U8 **roles;

#ifdef DEBUG_VERBOSE
  printf("Roles of Comp %s:\n", comp_name);
#endif
  if (OMX_GetRolesOfComponent(comp_name, &nroles, NULL) == OMX_ErrorNone 
	  && nroles > 0) {
	roles = malloc(sizeof(char *) * nroles);
	roles[0] = malloc(sizeof(char) * 129 * nroles);
	for (i = 1; i < nroles; i++) {
	  roles[i] = roles[i - 1] + 129;
	}

	if (OMX_GetRolesOfComponent(comp_name, &nroles, roles) == OMX_ErrorNone) {
	  for (i = 0; i < nroles; i++) {
#ifdef DEBUG_VERBOSE
		printf("    Role %d: [%s]\n", (int)i, roles[i]);
#endif
	  }
	}
	free(roles[0]);
	free(roles);
  }
}

void ListAllComps()
{
  OMX_U32 i;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  char comp_name[256];

  i = 0;
  while (err == OMX_ErrorNone) {
	err = OMX_ComponentNameEnum(comp_name, sizeof (comp_name), i);
	if (err == OMX_ErrorNone || err == OMX_ErrorNoMore) {
#ifdef DEBUG_VERBOSE
	  printf("Component %03d: [%s]\n", (int)i, comp_name);
#endif
	  do_get_roles(comp_name);
	}
	i++;
  }
}

OMX_ERRORTYPE GetParameter(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param)
{
  OMX_ERRORTYPE err;

  err = OMX_GetParameter(handle, index, param);
  if (err != OMX_ErrorNone && err != OMX_ErrorNoMore) {
	fatal("%s for index: 0x%x(%s) failed :%s (0x%08x)\n", __func__,
		index, omx_index_to_string(index), omx_error_to_string (err), err);
  }

  return err;
}

OMX_ERRORTYPE SetParameter(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param)
{
  OMX_ERRORTYPE err;

  err = OMX_SetParameter (handle, index, param);
  if (err != OMX_ErrorNone && err != OMX_ErrorNoMore) {
	fatal("%s for index: 0x%x(%s) failed :%s (0x%08x)\n", __func__,
		index, omx_index_to_string(index), omx_error_to_string (err), err);
  }

  return err;
}

OMX_ERRORTYPE GetConfig(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param)
{
  OMX_ERRORTYPE err;

  err = OMX_GetConfig(handle, index, param);
  if (err != OMX_ErrorNone) {
	fatal("%s for index: 0x%x(%s) failed :%s (0x%08x)\n", __func__,
		index, omx_index_to_string(index), omx_error_to_string (err), err);
  }

  return err;
}

OMX_ERRORTYPE SetConfig(OMX_HANDLETYPE handle, OMX_INDEXTYPE index, OMX_PTR param)
{
  OMX_ERRORTYPE err;

  err = OMX_SetConfig(handle, index, param);
  if (err != OMX_ErrorNone) {
	fatal("%s for index: 0x%x(%s) failed :%s (0x%08x)\n", __func__,
		index, omx_index_to_string(index), omx_error_to_string (err), err);
  }

  return err;
}

void GetIndex(OMX_HANDLETYPE handle, OMX_STRING str, OMX_INDEXTYPE *index)
{
  OMX_ERRORTYPE err;

  err = OMX_GetExtensionIndex (handle, str, index);
  if (err != OMX_ErrorNone)
	fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);
}

////////////////////////////////////////////////////////////////////////////////

void CompGetRoles(struct Comp *comp)
{
  do_get_roles(comp->name);
}

void CompSetRole(struct Comp *comp, OMX_STRING role)
{
  OMX_ERRORTYPE err;
  OMX_PARAM_COMPONENTROLETYPE param;

  OMX_INIT_STRUCT (&param);

  strncpy ((char *)param.cRole, role, sizeof(param.cRole));
  err = SetParameter(comp->handle, OMX_IndexParamStandardComponentRole, &param);
  if (err != OMX_ErrorNone)
	fatal("%s failed :%s (0x%08x)\n", __func__, omx_error_to_string (err), err);

#ifdef DEBUG_VERBOSE
  printf("Setting component role to '%s': %s (0x%08x)\n", 
	  role, omx_error_to_string (err), err);
#endif
}

int CompGetHandle(struct Comp *comp)
{
  OMX_HANDLETYPE handle = 0;

  OMX_ERRORTYPE err;
  err = OMX_GetHandle(&handle, comp->name, comp, &comp->cbs);
  if (err != OMX_ErrorNone) {
	fatal("OMX_GetHandle failed\n");
  }
  comp->handle = handle;
  return 0;
}

OMX_STATETYPE CompGetState(struct Comp *comp)
{
  OMX_STATETYPE state;
  OMX_GetState(comp->handle, &state);
#ifdef DEBUG_VERBOSE
  printf("Comp State: %s\n", omx_state_to_string(state));
#endif
  return state;
}

void CompSetState(struct Comp *comp, OMX_STATETYPE state)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  err = OMX_SendCommand(comp->handle, OMX_CommandStateSet, state, NULL);

  if (err != OMX_ErrorNone) {
	fatal("Error setting %s state to %u: %s (0x%08x)", comp->name,
		state, omx_error_to_string (err), err);
  }
#ifdef DEBUG_VERBOSE
  printf("===> state to [%s] <===\n", omx_state_to_string(state));
#endif
}

int CompGetPorts(struct Comp *comp)
{
  OMX_PORT_PARAM_TYPE port;

  OMX_INIT_STRUCT(&port);

  GetParameter(comp->handle, OMX_IndexParamVideoInit, &port);
#ifdef DEBUG_VERBOSE
  printf("port num: %lu, start number = %lu\n", port.nPorts, port.nStartPortNumber);
#endif
  if (port.nPorts != 2) {
	fatal("port number is not 2/%lu\n", port.nPorts);
  }
  comp->in_port_index = port.nStartPortNumber + 0;
  comp->out_port_index = port.nStartPortNumber + 1;

  return 0;
}

void PrintPortDef(OMX_PARAM_PORTDEFINITIONTYPE *def)
{
#ifdef DEBUG_VERBOSE
  printf("\n------------------ port definition --------------------\n");
  printf("index: %lu, dir: %s\n", def->nPortIndex,
	  def->eDir == OMX_DirInput ? "in" : "out");
  printf("acutal buffer num: %lu, min num: %lu, size: %lu\n",
	  def->nBufferCountActual ,def->nBufferCountMin, def->nBufferSize);
  printf("enabled: %d, populated: %d\n", def->bEnabled, def->bPopulated);
  printf("continuous: %d, aligment: %lu\n", def->bBuffersContiguous,
	  def->nBufferAlignment);
  printf("cMIMEType: %s\n", def->format.video.cMIMEType);
  printf("width: %lu, height: %lu\n", def->format.video.nFrameWidth,
	  def->format.video.nFrameHeight);
  printf("stride: %lu, slice height: %lu\n", def->format.video.nStride,
	  def->format.video.nSliceHeight);
  printf("bitrate: %lu, fps: %lu\n", def->format.video.nBitrate,
	  def->format.video.xFramerate);
  printf("compress format: %s, color format: %s\n",
	  omx_compress_format_to_string(def->format.video.eCompressionFormat),
	  omx_color_format_to_string(def->format.video.eColorFormat));
#endif
}

int CompGetPortDefinition(struct Comp *comp, OMX_U32 index,
	OMX_PARAM_PORTDEFINITIONTYPE *port_def)
{
  OMX_INIT_STRUCT(port_def);
  port_def->nPortIndex = index;

  GetParameter(comp->handle, OMX_IndexParamPortDefinition, port_def);
  return 0;
}

int CompSetPortDefinition(struct Comp *comp, OMX_PARAM_PORTDEFINITIONTYPE *port_def)
{
  SetParameter(comp->handle, OMX_IndexParamPortDefinition, port_def);
  return 0;
}

void CompUpdatePortDefinition(struct Comp *comp, OMX_U32 port_index,
	OMX_PARAM_PORTDEFINITIONTYPE *port_def)
{
  OMX_PARAM_PORTDEFINITIONTYPE *comp_port_def;

  if (port_def)
	SetParameter(comp->handle, OMX_IndexParamPortDefinition, port_def);

  if (port_index == comp->in_port_index)
	comp_port_def = &comp->in_port_def;
  else
	comp_port_def = &comp->out_port_def;
  GetParameter(comp->handle, OMX_IndexParamPortDefinition, comp_port_def);
}

void CompSetEncConfLocation(struct Comp *comp)
{
  /* setup encoder config file location */
  OMX_INDEXTYPE eIndex;
  NVX_PARAM_TEMPFILEPATH enc_conf_loc;

  GetIndex(comp->handle, NVX_INDEX_PARAM_TEMPFILEPATH, &eIndex);
  OMX_INIT_STRUCT (&enc_conf_loc);
  enc_conf_loc.pTempPath = ENCODER_CONF_LOCATION;
  SetParameter(comp->handle, eIndex, &enc_conf_loc);
}

void CompSetQuant(struct Comp *comp, OMX_U32 quant_i_frames,
	OMX_U32 quant_p_frames, OMX_U32 quant_b_frames)
{
  OMX_VIDEO_PARAM_QUANTIZATIONTYPE quant_param;

  OMX_INIT_STRUCT (&quant_param);
  quant_param.nPortIndex = comp->out_port_index;
  GetParameter(comp->handle, OMX_IndexParamVideoQuantization, &quant_param);

  quant_param.nQpI = quant_i_frames;
  quant_param.nQpP = quant_p_frames;
  quant_param.nQpB = quant_b_frames;

  SetParameter(comp->handle, OMX_IndexParamVideoQuantization, &quant_param);
}

void CompGetSupportedColorformats(struct Comp *comp)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE param;
  OMX_ERRORTYPE err;
  OMX_U32 old_index;

  OMX_INIT_STRUCT (&param);
  param.nPortIndex = comp->in_port_index;
  param.nIndex = 0;
  param.xFramerate = (comp->fps << 16);
  old_index = -1;

#ifdef DEBUG_VERBOSE
  printf("supported color formats:\n");
#endif
  do {
	err = GetParameter(comp->handle, OMX_IndexParamVideoPortFormat, &param);
	if (old_index == param.nIndex)
	  break;

	if (err == OMX_ErrorNone || err == OMX_ErrorNoMore) {
#ifdef DEBUG_VERBOSE
	  printf("  [%lu] 0x%x %s\n", param.nIndex, param.eColorFormat,
		  omx_color_format_to_string(param.eColorFormat));
#endif
	}
	old_index = param.nIndex++;
  } while (err == OMX_ErrorNone);
}

void doCompGetSupportedColorformats(struct Comp *comp)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE param;
  OMX_ERRORTYPE err;
  OMX_U32 old_index;

  OMX_INIT_STRUCT (&param);
  param.nPortIndex = comp->out_port_index;
  param.nIndex = 0;
  param.xFramerate = comp->fps;
  old_index = -1;

#ifdef DEBUG_VERBOSE
  printf("supported color formats:\n");
#endif
  do {
	err = GetParameter(comp->handle, OMX_IndexParamVideoPortFormat, &param);
	if (old_index == param.nIndex)
	  break;

	if (err == OMX_ErrorNone || err == OMX_ErrorNoMore) {
#ifdef DEBUG_VERBOSE
	  printf("  [%lu] 0x%x %s\n", param.nIndex, param.eColorFormat,
		  omx_color_format_to_string(param.eColorFormat));
#endif
	}
	old_index = param.nIndex++;
  } while (err == OMX_ErrorNone);
}

void CompSetColorFormat(struct Comp *comp, OMX_U32 port_index, OMX_COLOR_FORMATTYPE f)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE param;

  OMX_INIT_STRUCT (&param);
  param.nPortIndex = port_index;
  param.eColorFormat = f;

  SetParameter(comp->handle, OMX_IndexParamVideoPortFormat, &param);
}

OMX_U32 CalculateBufferSize(OMX_PARAM_PORTDEFINITIONTYPE *port_def)
{
  OMX_U32 size = 0;
  switch (port_def->format.video.eColorFormat) {
	case OMX_COLOR_FormatYUV420Planar:
	case OMX_COLOR_FormatYUV420PackedPlanar:
	  size = (port_def->format.video.nStride * port_def->format.video.nFrameHeight) +
		2 * ((port_def->format.video.nStride / 2) *
			((port_def->format.video.nFrameHeight + 1) / 2));
	  break;

	case OMX_COLOR_FormatYUV420SemiPlanar:
	  size =
		(port_def->format.video.nStride * port_def->format.video.nFrameHeight) +
		(port_def->format.video.nStride *
		 ((port_def->format.video.nFrameHeight + 1) / 2));
	  break;
	default:
	  break;
  }
  return size;
}

OMX_ERRORTYPE ClientAllocBuffer(struct Comp *comp, OMX_BOOL in, OMX_U32 n)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_U32 i;
  OMX_U32 port_index;
  OMX_PARAM_PORTDEFINITIONTYPE port_def; 
  OMX_BUFFERHEADERTYPE **omx_buf;
  OMX_U32 *bufs_num;
  OMX_U8 *buf;

  port_index =  in ? comp->in_port_index : comp->out_port_index;
  port_def = in ? comp->in_port_def : comp->out_port_def;
  omx_buf = in ? comp->in_port_bufs : comp->out_port_bufs;
  bufs_num = in ? &comp->in_bufs_num: &comp->out_bufs_num;

  if (n == -1)
	n = port_def.nBufferCountActual;
  *bufs_num = n;

  for (i = 0; i < n; i++) {
	buf = malloc(port_def.nBufferSize);
	err = OMX_UseBuffer(comp->handle, &omx_buf[i], port_index, comp, port_def.nBufferSize, buf);
	if (err != OMX_ErrorNone) {
	  fatal("Failed to allocate buffer for %s port %lu: %s (0x%08x)\n", comp->name,
		  port_index, omx_error_to_string (err), err);
	}
  }

  return err;
}

OMX_ERRORTYPE CompAllocBuffer(struct Comp *comp, OMX_BOOL in, OMX_U32 n)
{
  OMX_U32 i;
  OMX_U32 port_index;
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_PARAM_PORTDEFINITIONTYPE port_def; 
  OMX_BUFFERHEADERTYPE **omx_buf;
  OMX_U32 *bufs_num;

  port_index =  in ? comp->in_port_index : comp->out_port_index;
  port_def = in ? comp->in_port_def : comp->out_port_def;
  omx_buf = in ? comp->in_port_bufs : comp->out_port_bufs;
  bufs_num = in ? &comp->in_bufs_num: &comp->out_bufs_num;

  if (n == -1)
	n = port_def.nBufferCountActual;
  *bufs_num = n;

  for (i = 0; i < n; i++) {
	err = OMX_AllocateBuffer(comp->handle, &omx_buf[i], port_index, (void *)i, 
		port_def.nBufferSize);

	if (err != OMX_ErrorNone) {
	  fatal("Failed to allocate buffer for %s port %lu: %s (0x%08x)\n", comp->name,
		  port_index, omx_error_to_string (err), err);
	  return err;
	}

#ifdef DEBUG_VERBOSE
	printf("%s: allocated buffer %p, out port index: %lu, in port index: %lu\n",
		comp->name, omx_buf[i]->pBuffer, omx_buf[i]->nOutputPortIndex,
		omx_buf[i]->nInputPortIndex);
#endif
  }

  return err;
}

void CompUseNvMem(struct Comp *comp, OMX_U32 port_index)
{
  NVX_PARAM_USENVBUFFER param;
  OMX_INDEXTYPE eIndex;

  GetIndex(comp->handle, NVX_INDEX_CONFIG_USENVBUFFER, &eIndex);

  OMX_INIT_STRUCT (&param);
  param.nPortIndex = port_index;
  param.bUseNvBuffer = OMX_TRUE;
  SetParameter(comp->handle, eIndex, &param);
}

void CompSetRcMode(struct Comp *comp, OMX_U32 rc_mode)
{
  OMX_INDEXTYPE eIndex;
  NVX_PARAM_RATECONTROLMODE oRCMode;

  OMX_INIT_STRUCT (&oRCMode);
  oRCMode.nPortIndex = 0; // in port

  GetIndex(comp->handle, NVX_INDEX_PARAM_RATECONTROLMODE, &eIndex);

  switch (rc_mode) {
	case NVX_VIDEO_RateControlMode_CBR:
	  oRCMode.eRateCtrlMode = NVX_VIDEO_RateControlMode_CBR;
	  break;
	case NVX_VIDEO_RateControlMode_VBR:
	  oRCMode.eRateCtrlMode = NVX_VIDEO_RateControlMode_VBR;
	  break;
	case NVX_VIDEO_RateControlMode_VBR2:
	  oRCMode.eRateCtrlMode = NVX_VIDEO_RateControlMode_VBR2;
	  break;
	default:
	  oRCMode.eRateCtrlMode = NVX_VIDEO_RateControlMode_VBR2;
	  break;
  }

  SetParameter(comp->handle, eIndex, &oRCMode);
}

void CompSetBitrate(struct Comp *comp, OMX_U32 port_index, OMX_U32 bitrate)
{
  OMX_VIDEO_CONFIG_BITRATETYPE oBitrate;
  OMX_ERRORTYPE err;
  OMX_U32 index = OMX_IndexConfigVideoBitrate;

  OMX_INIT_STRUCT (&oBitrate);

  err = OMX_GetConfig(comp->handle, index, &oBitrate);
  if (err != OMX_ErrorNone) {
	printf("%s failed :%s\n", omx_index_to_string(index), omx_error_to_string (err));
  }
#ifdef DEBUG_VERBOSE
  printf("port [%lu] bitrate = %lu\n", oBitrate.nPortIndex, oBitrate.nEncodeBitrate);
#endif

  //oBitrate.nPortIndex = port_index;
  oBitrate.nEncodeBitrate = bitrate;

  SetConfig(comp->handle, index, &oBitrate);
}

void CompGetIntraPeriod(struct Comp *comp, OMX_U32 port_index, OMX_VIDEO_CONFIG_AVCINTRAPERIOD *data)
{
  OMX_INIT_STRUCT(data);
  data->nPortIndex = port_index;

  GetConfig(comp->handle, OMX_IndexConfigVideoAVCIntraPeriod, data);
#ifdef DEBUG_VERBOSE
  printf("  port: %lu, IDR period: %lu, PFrames: %lu\n", data->nPortIndex, data->nIDRPeriod, data->nPFrames);
#endif
}

void CompSetIntraPeriod(struct Comp *comp, OMX_U32 val, OMX_VIDEO_CONFIG_AVCINTRAPERIOD *data)
{
  data->nIDRPeriod = val;
  data->nPFrames = val - 1;
  SetConfig(comp->handle, OMX_IndexConfigVideoAVCIntraPeriod, data);
}

////////////////////////////////////////////////////////////////////////////////

void PortSetEnable(struct Comp *comp, OMX_U32 port_index, OMX_BOOL enable)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
#ifdef DEBUG_VERBOSE
  printf("set port %lu to %d\n", port_index, enable);
#endif
  if (enable)
	err = OMX_SendCommand(comp->handle, OMX_CommandPortEnable,
		port_index, NULL);
  else
	err = OMX_SendCommand(comp->handle, OMX_CommandPortDisable,
		port_index, NULL);

  if (err != OMX_ErrorNone) {
	fatal("Failed to send enable/disable command to %s port %lu: %s (0x%08x)",
		comp->name, port_index, omx_error_to_string (err), err);
  }
}

OMX_BOOL PortIsEnabled(struct Comp *comp, OMX_U32 port_index)
{
  OMX_BOOL enabled;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;
  OMX_INIT_STRUCT (&port_def);
  port_def.nPortIndex = port_index;

  GetParameter(comp->handle, OMX_IndexParamPortDefinition, &port_def);
  enabled = ! !port_def.bEnabled;

#ifdef DEBUG_VERBOSE
  printf("port [%lu] is %s\n", port_index, enabled ? "enabled": "disabled");
#endif

  return enabled;
}

void PortFlush(struct Comp *comp, OMX_U32 port_index)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  err = OMX_SendCommand(comp->handle, OMX_CommandFlush, port_index, NULL);
  if (err != OMX_ErrorNone) {
	fatal("Flush %s port %lu: %s (0x%08x)",
		comp->name, port_index, omx_error_to_string (err), err);
  }
}

void PrintBufferHead(OMX_BUFFERHEADERTYPE *p)
{
#ifdef DEBUG_VERBOSE
  printf("----------- buffer header ---------------\n");
  printf("  InPortIndex = %lu, OutPortIndex = %lu\n",
	  p->nInputPortIndex, p->nOutputPortIndex);
  printf("  pBuffer: %p, AllocLen = %lu, FilledLen = %lu, Offset = %lu\n",
	  p->pBuffer, p->nAllocLen, p->nFilledLen, p->nOffset);
  printf("  nTickCount = %lu, TimeStamp = %lld, Flags = 0x%x\n",
	  p->nTickCount, (OMX_S64)p->nTimeStamp, (unsigned int)p->nFlags);
#endif
}

void PrintPortStatus(struct Comp *comp, int is_in)
{
  OMX_PARAM_PORTDEFINITIONTYPE port_def;

  if (is_in)
	CompGetPortDefinition(comp, comp->in_port_index, &port_def);
  else
	CompGetPortDefinition(comp, comp->out_port_index, &port_def);
  PrintPortDef(&port_def);
}

