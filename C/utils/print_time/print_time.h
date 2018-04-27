#ifndef _PRINT_TIME_H_
#define _PRINT_TIME_H_

#include <time.h>
#define TIME_LEN 20

/*
 * store in @t
 */
int fetch_time(struct tm *t);

/*
 * from @t to @str whose length is @len
 */
void time2str(struct tm *t, char *str, int len);

void print_timeofday();
#endif


