#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "print_time.h"


int fetch_time(struct tm *t)
{
	time_t tt;
	
	if (time(&tt) == ((time_t)-1)) {
		perror("time failed");
		return 1;
	}

	gmtime_r(&tt, t);

	return 0;
}

void time2str(struct tm *t, char *str, int len)
{
    if (len < TIME_LEN) {
        printf("len is short\n");
        return;
    }

    memset(str, 0, len);
    snprintf(str, len, "%04d-%02d-%02d %02d:%02d:%02d\n",
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);
}

void print_timeofday()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	printf("%ld.%ld\n", t.tv_sec, t.tv_usec);
}

