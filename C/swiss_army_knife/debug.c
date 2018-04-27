#include <stdio.h>
#include <sys/time.h>

#include "debug.h"

int dump_buf(FILE *fp, const unsigned char *buf, int len, const char *desc)
{
	struct timeval tv;
	if (desc)
		fprintf(fp, "%s:\n", desc);

	gettimeofday(&tv, NULL);
	fprintf(fp, "++ %ld.%06ld ++ ---------------------------\n", tv.tv_sec, tv.tv_usec);
	for (int i = 0; i < len; i++) {
		fprintf(fp, "0x%02x ", (unsigned char)buf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(fp, "\n");
	}

	fprintf(fp, "\n--------------------------------------------------------\n");

    return len;
}
