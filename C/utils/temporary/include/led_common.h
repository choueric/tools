/* ****************************************************************************
 * file:	led_common.h
 * date:	2014-09-04
 * func:	header file for led function; only used internal
 * author:	50214332
 * ****************************************************************************/
#ifndef	__LED_COMMON_H__
#define	__LED_COMMON_H__

#include "LedAccess.h"

#define	LED_NAME_LEN		20

enum led_type {
	LED_TYPE_FRONTBOARD,
};

struct led_ops {
	// interface ops
	INT  (* init)(struct led_dev *dev, int id);
	INT  (* release)(struct led_dev *dev, int id);
	BOOL (* switchon)(struct led_dev *dev, unsigned char redBrightness, unsigned char greenBrightness, unsigned char blueBrightness,TwinkleFrequency freq, unsigned char duty_cycle, int id);
	BOOL (* switchoff)(struct led_dev *dev, int id);
	BOOL (* getstatus)(struct led_dev *dev, led_status_t* ledStatusPtr, int id);
};

struct led_dev {
	struct led_ops *ops;
	CHAR name[LED_NAME_LEN + 1];
	const CHAR *path;  					// path of dev node or sys node
	enum led_type type;
};

INT InitLedAdapters(VOID);
VOID init_led_frontboard(VOID);


struct led_dev *LedSetup(const CHAR *name, const CHAR *path, enum led_type type);
struct led_ops *Get_Led_Frontboard_Ops(VOID);

struct led_dev *GetLedDev();

BOOL SetLedDev(struct led_dev *dev);


#endif
