#include "clog.h"

int clog_level = CLOG_ERROR;

void clog_set_level(int level)
{
	clog_level = level;
}
