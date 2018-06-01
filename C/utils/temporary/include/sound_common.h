/* ****************************************************************************
 * file:	sound_common.h
 * date:	2014-07-05
 * func:	header file for sound function; only used internal
 * author:	50217022
 * ****************************************************************************/
#ifndef	__SOUND_COMMON_H__
#define	__SOUND_COMMON_H__

#include "SoundAccess.h"

#define	SOUND_NAME_LEN		20

enum sound_type {
	SOUND_TYPE_ECG,
	SOUND_TYPE_PWM_AUDIO,
	SOUND_TYPE_WAV_AUDIO,
	SOUND_TYPE_FRONTBOARD,
};

struct sound_ops {
	// interface ops
	INT (* init)(struct sound_dev *dev, INT32 parameter);
	INT (* close)(struct sound_dev *dev);
	INT (* setup)(struct sound_dev *dev);
	INT (* release)(struct sound_dev *dev, INT32 parameter);
	INT (* play)(struct sound_dev *dev, UINT8 nFileIndex, UINT32 nVolume, INT bRepeat, UINT32 nID);
	INT (* play_file)(struct sound_dev *dev, const CHAR *path);
	INT (* play_buffer)(struct sound_dev *dev, const CHAR *buffer, INT size);
	INT (* set_volume)(struct sound_dev *dev, UINT8 nFileIndex, UINT8 value);
	UINT32 (* get_version)(struct sound_dev *dev);
	BOOL (* is_speaker_exist)(struct sound_dev *dev);
	BOOL (* is_playing)(struct sound_dev *dev, UINT32 nID);
	INT (* is_playingCount)(struct sound_dev *dev, UINT32 nID);
	INT (* stop)(struct sound_dev *dev, INT reset, UINT32 nID);
	INT (* get_play_status)(struct sound_dev *dev);
	INT (* enable_speaker)(struct sound_dev *dev, BOOL enable);
	INT (* enable_record)(struct sound_dev *dev, BOOL enable);
	INT (* recording)(struct sound_dev *dev, CHAR *pBuff, UINT size);
	INT (* get_param)(struct sound_dev *dev, enum SOUND_AUDIO_PARAM param, UINT &value);
	INT (* set_param)(struct sound_dev *dev, enum SOUND_AUDIO_PARAM param, UINT &value);
	INT (* get_agc_gain)(struct sound_dev *dev, UINT8 *lgain, UINT8 *rgain);

	enum sound_type type;
};

struct sound_dev {
	struct sound_ops *ops;
	CHAR name[SOUND_NAME_LEN + 1];
	const CHAR *path;  					// path of dev node or sys node
	CHAR fd;
	enum sound_type type;
};

INT InitSoundAdapters(VOID);

struct sound_dev *SoundSetup(const CHAR *name, const CHAR *path, enum sound_type type);

INT SoundRelease(struct sound_dev *dev);

/************************************************************************************************/
INT SetSoundDev(struct sound_dev *dev);
struct sound_dev *GetSoundDev(VOID);

/************************************************************************************************/
/* pwm ops */
VOID init_sound_ecg_ops(VOID);
struct sound_ops *Get_Sound_Ecg_Ops(VOID);

/* pwm_audio ops */
VOID init_sound_pwm_audio(VOID);
struct sound_ops *Get_Sound_PwmAudio_Ops(VOID);

/* wav ops */
VOID init_sound_wav_audio(VOID);
struct sound_ops *Get_Sound_WavAudio_Ops(VOID);

/* FrontBoard ops */
VOID init_sound_frontboard(VOID);
struct sound_ops *Get_Sound_FrontBoard_Ops(VOID);
#endif
