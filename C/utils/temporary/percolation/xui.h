#ifndef _XUI_H
#define _XUI_H

#include <X11/Xlib.h>

#define VIEW3D

#define RECT_BORDER 1

enum rect_type_t {
	TYPE_BLOCK,
	TYPE_FULL,
	TYPE_EMPTY,
};

#define BLOCK_COLOR "blue"
#define FULL_COLOR "yellow"
#define EMPTY_COLOR "red"

typedef int (CALLBACK_FUNC)(void *);

int XuiInit(int n, int rectw, int border, int is_auto);
int XuiMainLoop(CALLBACK_FUNC *cb, void *data);
void XuiClose();

void XuiDrawRect(int x, int y, enum rect_type_t type);

#endif
