#ifndef _DRIVER_ASYN_ACCESS_H_
#define _DRIVER_ASYN_ACCESS_H_
#include "MMOS.h"


typedef enum
{
	//LED
    DriverAccessMessageId_LedInit= 0,
    DriverAccessMessageId_LedRelease,
    DriverAccessMessageId_LedSwitchOn,
    DriverAccessMessageId_LedSwitchOff,
    DriverAccessMessageId_LedGetStatus,
    //SOUND
    DriverAccessMessageId_SoundInit,
    DriverAccessMessageId_SoundRelease,
    DriverAccessMessageId_SoundPlay,
    DriverAccessMessageId_SoundIsPlaying,
    DriverAccessMessageId_SoundIsPlayingCount,
    DriverAccessMessageId_SoundStop,
    //Backlight
    DriverAccessMessageId_BackLightCtl,
    //Powermanager
    DriverAccessMessageId_Reboot,
    DriverAccessMessageId_PowerOff,
    DriverAccessMessageId_PowerInfoRealTime,
    DriverAccessMessageId_PowerInfoAcquire,
    DriverAccessMessageId_PowerOffFlagGet

}DriverAccessMessageId;

BOOL DriverAsynAccessThreadInit();
VOID AddDriverAsynAccessMessage(const COREMSG *pMsg);

#endif //_DRIVER_ASYN_ACCESS_H_
