/*
 * screen_common.h
 *
 *  Created on: 2014-7-9
 *      Author: 50215243
 */

#ifndef SCREEN_COMMON_H_
#define SCREEN_COMMON_H_

#define SCREEN_NAME_LEN 15


enum SCREEN_TYPE
{
	SCREEN_TYPE_AM1808,
	SCREEN_TYPE_AM335X,
	SCREEN_TYPE_FRONTBOARD,
	SCREEN_TYPE_INVALID,
};

struct screen_dev {
	struct screen_ops *ops;
	char name[SCREEN_NAME_LEN + 1];
	const char *path;  // path of dev node or sys node
	SCREEN_TYPE type;
};

struct screen_ops
{
	INT (* init)(struct screen_dev* dev, unsigned char nScrnID);
	BOOL (* setval)(struct screen_dev* dev, unsigned char nBrigthness, unsigned char nScrnID);
};


void InitScreenAdapters();
void Init_screen_am1808_ops();
void Init_screen_am335x_ops();
void Init_screen_frontboard_ops();
struct screen_ops *Get_screen_am1808_ops();
struct screen_ops *Get_screen_am335x_ops();
struct screen_ops *Get_screen_frontboard_ops();
struct screen_dev *Screen_Setup(const char *name, const char *path, SCREEN_TYPE type);

struct screen_dev *GetScreenDev();
BOOL SetScreenDev(struct screen_dev *dev);

#endif /* SCREEN_COMMON_H_ */
