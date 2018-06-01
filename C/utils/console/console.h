#pragma once

#include <termios.h>

typedef struct {
	struct termios oldt, newt;
} console_t;

int console_enter_raw(console_t *con);
int console_leave_raw(console_t *con);
int console_restore();
