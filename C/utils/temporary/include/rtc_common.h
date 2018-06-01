/* ****************************************************************************
 * file:	rtc_common.h
 * date:	2014-07-05
 * func:	header file for rtc function; only used internal
 * author:	50217022
 * ****************************************************************************/
#ifndef	__RTC_COMMON_H__
#define	__RTC_COMMON_H__

#include "RtcAccess.h"

#define	RTC_NAME_LEN		20

struct rtc_ops {
	// interface ops
	INT (* init)(struct rtc_dev *dev);
	INT (* setup)(struct rtc_dev *dev);
	INT (* release)(struct rtc_dev *dev);
	BOOL (* is_exist)(struct rtc_dev *dev);
	BOOL (* is_need_set)(struct rtc_dev *dev);
	INT (* set_output_freq)(struct rtc_dev *dev, INT32 freq);
	INT (* write_sys_time)(struct rtc_dev *dev, MM_SYSTIME* theTime);
	INT (* read_sys_time)(struct rtc_dev *dev, MM_SYSTIME* theTime);
	enum RTC_TIME_STATUS (* get_init_status)(struct rtc_dev *dev);
	INT (* enable_alarm)(struct rtc_dev *dev, BOOL enable);
	INT (* clear_alarm_flag)(struct rtc_dev *dev);
	INT (* read_alarm_flag)(struct rtc_dev *dev);
	INT (* enable_bbtd)(struct rtc_dev *dev, BOOL enable);
	BOOL (* is_bbtd_enable)(struct rtc_dev *dev);
	INT (* set_sam_rate)(struct rtc_dev *dev, enum RTC_SAM_RATE rate);
	enum RTC_SAM_RATE (* get_sam_rate)(struct rtc_dev *dev);
	INT (* write_alarm_time)(struct rtc_dev *dev, MM_SYSTIME* theTime, enum RTC_ALARM_TIMER_TYPE type);
	INT (* read_alarm_time)(struct rtc_dev *dev, MM_SYSTIME* theTime);
	BOOL (* close_output_clock)(struct rtc_dev *dev);
	enum rtc_type type;
};

struct rtc_dev {
	struct rtc_ops *ops;
	CHAR name[RTC_NAME_LEN + 1];
	const CHAR *path;  					// path of dev node or sys node
	INT fd;
	enum RTC_TIME_STATUS status;
	enum rtc_type type;
};

BOOL InitRtcAdapters(VOID);

struct rtc_dev *RtcSetup(const CHAR *name, const CHAR *path, enum rtc_type type);

BOOL RtcRelease(struct rtc_dev *dev);

/***************************************************************************************/
BOOL SetRtcDev(struct rtc_dev *dev);
struct rtc_dev *GetRtcDev(VOID);

/***************************************************************************************/
/* */
VOID init_rtc_isl1208_ops(VOID);
struct rtc_ops *Get_Rtc_ISL1208_Ops(VOID);

/* */
VOID init_rtc_ds3234_ops(VOID);
struct rtc_ops *Get_Rtc_DS3234_Ops(VOID);

VOID init_rtc_ds1339b_ops();
struct rtc_ops *Get_Rtc_Ds1339b_Ops();

VOID init_rtc_e3800_ops();
struct rtc_ops *Get_Rtc_E3800_Ops();
#endif
