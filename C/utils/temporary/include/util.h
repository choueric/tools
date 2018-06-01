#ifndef UTIL_H_
#define UTIL_H_

#include "MM_Types.h"
#include <errno.h>

#define BITMASK(n) (1UL << (n))
#define ARRAY_SIZE(array) (sizeof((array)) / sizeof(*(array)))

/* defined in execute.cpp */
int RemoteExecute(const char *cmd, int timeout = 0);

int GetFileSize(char *name);
int DeleteFile(char *name);
int GetFreeSpace(const char *path);

int dump_buf(FILE *fp, const unsigned char *buf, int len, const char *desc);

// add syslog
#define DRV_ASSERT(exp) \
		do\
		{\
			if (!(exp)) {\
				fprintf(stderr, "DRV_ASSERT: %s %d, errno: %d\n", __FILE__, __LINE__, errno);\
			}\
		} while (0)

// add syslog
#define DRV_ASSERT_PRINT(exp, ...) \
		do\
		{\
			if (!(exp)) {\
				fprintf(stderr, "DRV_ASSERT: %s %d, errno: %d\n", __FILE__, __LINE__, errno);\
				fprintf(stderr, __VA_ARGS__);\
			}\
		} while (0)

#define DRV_LOG(...) \
	do {\
		fprintf(stderr, "DRV_LOG: %s:%d:%s(): ", __FILE__, __LINE__, __func__);\
		fprintf(stderr, __VA_ARGS__);\
		fflush(stderr);\
	} while (0)

#define DRV_ERR(...) \
	do {\
		fprintf(stderr, "DRV_ERR: %s:%d:%s(): ", __FILE__, __LINE__, __func__);\
		fprintf(stderr, __VA_ARGS__);\
		fflush(stderr);\
	} while (0)

// @return: 0, success.
typedef int (* iterator_func_t)(int offset, uint32 len, void *data);

/*
 * iterate from @start in the step of @page_size.
 * call @func to do actual work in each iteration.
 * if @func return < 0, it will return with failure immediately.
 * in each iteration, if the return value of @func is >= 0, iterate will
 * add the return value together, and when iterate ends, return the sum.
 * @return: how many bytes have done from @count, if fails, return value < 0
 */
int page_iterate(int start, int page_size, uint32 count, iterator_func_t func, void *data);

#endif /* UTIL_H_ */
