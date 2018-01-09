#pragma once

#include <stdio.h>

extern int clog_level;

enum {
	CLOG_NONE,
	CLOG_ERROR,
	CLOG_WARNING,
	CLOG_INFO,
	CLOG_DEBUG,
	CLOG_TRACE,
};

void clog_set_level(int level);

#define error(fmt, args...) do {\
	if (clog_level >= CLOG_ERROR) \
	printf("\033[31m!!! %s %d %s()\033[0m: " fmt "\n", \
			__FILE__, __LINE__, __func__, ##args); } while (0)

#define warn(fmt, args...) do {\
	if (clog_level >= CLOG_WARNING) \
	printf("\033[33m>>> %s %d %s()\033[0m: " fmt "\n", \
			__FILE__, __LINE__, __func__, ##args); } while (0)

#define info(fmt, args...) do {\
	if (clog_level >= CLOG_INFO) \
	printf("\033[34m>>> %s %d %s()\033[0m: " fmt "\n", \
			__FILE__, __LINE__, __func__, ##args); } while (0)

#define debug(fmt, args...) do {\
	if (clog_level >= CLOG_DEBUG) \
	printf("\033[32m>>> %s %d %s()\033[0m: " fmt "\n", \
			__FILE__, __LINE__, __func__, ##args); } while (0)

#define trace(fmt, args...) do {\
	if (clog_level >= CLOG_TRACE) \
	printf("\033[35m>>>\033[0m " fmt "\n", ##args);} while (0)

#define fatal(fmt, args...) {\
	printf("\n=== fatal === %s:%d: " fmt "\n", __FILE__, __LINE__, ##args);\
	exit(1);\
}
