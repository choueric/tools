#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#include <sys/time.h>

/* need this variable:
   static struct timeval tv1 = {0}, tv2 = {0};
*/

#define GET_TIME(str) do {\
	gettimeofday(&tv2, NULL);\
	printf("--> %s [%ld]\n", str, 1000000 * (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec));\
	tv1 = tv2;\
} while (0)


void debugoutput(char *str, int fd);

void do_edvlog(const char *format, ...);

#endif


