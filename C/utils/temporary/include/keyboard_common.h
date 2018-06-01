/*
 * keyboard_common.h
 *
 *  Created on: 2014-7-9
 *      Author: 50215243
 */

#ifndef KEYBOARD_COMMON_H_
#define KEYBOARD_COMMON_H_

#define KEYBOARD_NAME_LEN 15

enum keyboard_type
{
	KEYBOARD_TYPE_AM335X,
	KEYBOARD_TYPE_AM1808,
	KEYBOARD_TYPE_INVALID,
};

struct keyboard_dev
{
	struct keyboard_ops *ops;
	char name[KEYBOARD_NAME_LEN + 1];
	const char *path;  // path of dev node or sys node
	int fd;
	enum keyboard_type type;
};

struct keyboard_ops
{
	INT32 (* init)(struct keyboard_dev* dev);
	INT32 (* release)(struct keyboard_dev* dev);
	INT32 (* read)(struct keyboard_dev* dev, CHAR* buffer, INT32 buffer_size);
	INT32 (* get_status)(struct keyboard_dev* dev, UINT16 index);
};


INT32 GpioKeyboard_Read_Common(struct keyboard_dev* dev, CHAR *buffer, INT32 buffer_size);
INT32 GpioKeyboard_GetStataus_Common(struct keyboard_dev* dev, UINT16 index);

void InitKeyBoardAdapters();
void Init_keyboard_am1808_ops();
void Init_keyboard_am335x_ops();
struct keyboard_ops *Get_keyboard_am335x_ops();
struct keyboard_ops *Get_keyboard_am1808_ops();
struct keyboard_dev *Keyboard_Setup(const char *name, const char *path, enum keyboard_type type);

#endif /* KEYBOARD_COMMON_H_ */
