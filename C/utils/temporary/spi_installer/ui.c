#include <stdio.h>
#include <stdarg.h>
#include "ui.h"

#define RED "\E[31m"
#define GREEN "\E[32m"
#define END "\E[0m"

void errlog(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    printf(RED);
    vprintf(format, args);
    va_end(args);
    printf(END);
}


void goodlog(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    printf(GREEN);
    vprintf(format, args);
    va_end(args);
    printf(END);
}

void dump_buf(const unsigned char *buf, int len)
{
    int i = 0;

	printf("--------------------------------");
	for (i = 0; i < len; i++) {
		if (i % 16 == 0)
			printf("\n");
		printf("0x%02x ", buf[i]);
	}
	printf("\n--------------------------------\n");
}
