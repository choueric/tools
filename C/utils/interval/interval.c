#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#include "interval.h"

#define EDV_LOG_LEN 128

// append \n
void debugoutput(char *str, int fd)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	char s[100];

	sprintf(s, "%ld.%08ld: %s\n", t.tv_sec, t.tv_usec, str);
	write(fd, s, strlen(s));
	fsync(fd);
}

// don't append \n
void do_edvlog(const char *format, ...)
{
	char str[EDV_LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);
	
	gettimeofday(&tv, NULL);  // do_gettimeofday(&tv);
	snprintf(str, EDV_LOG_LEN, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, EDV_LOG_LEN, format, args);
	
	va_end(args);
    printf("%s", str);
	fflush(stdout);
}

