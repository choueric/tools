#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "print.h"

static void hexdump_char(FILE *fp, uint8_t *buf, size_t nbytes)
{
	size_t offset;
	int i;
	for (offset = 0; offset < nbytes; ) {
		int len = (offset + 16 > nbytes) ? nbytes - offset : 16;
		uint8_t *data = buf + offset;

		fprintf(fp, "%08x  ", (int)offset);
		for (i = 0; i < len; i++)
			fprintf(fp, "%02x ", data[i]);
		for (i = len; i < 16; i++)
			fprintf(fp, "   ");
		fprintf(fp, "|");
		for (i = 0; i < len; i++)
			fprintf(fp, "%c", isprint(data[i]) ? data[i]: '.');
		fprintf(fp, "|");
		fprintf(fp, "\n");
		offset += len;
	}
	fprintf(fp, "%08x\n", (int)nbytes);
}

void fhexdump(FILE *fp, uint8_t *buf, size_t nbytes, hexdump_flag_t flag)
{
	const char *fmt, *fmt_end;
	int blen = 0, i;
	uint8_t *data;
	bool is_alloc = false;
	size_t offset;

	switch (flag) {
	case HEXDUMP_HEX_1: blen = 1; fmt = "%02x "; fmt_end = "%02x"; break;
	case HEXDUMP_HEX_2: blen = 2; fmt = "%04x "; fmt_end = "%04x"; break;
	case HEXDUMP_DEC_1: blen = 1; fmt = "%03d "; fmt_end = "%03d"; break;
	case HEXDUMP_DEC_2: blen = 2; fmt = "%05d "; fmt_end = "%05d"; break;
	case HEXDUMP_OCT_1: blen = 1; fmt = "%03o "; fmt_end = "%03o"; break;
	case HEXDUMP_OCT_2: blen = 2; fmt = "%06o "; fmt_end = "%06o"; break;
	case HEXDUMP_CHAR: return hexdump_char(fp, buf, nbytes);
	default: return;
	}

	for (offset = 0; offset < nbytes; ) {
		int len;
		data = buf + offset;
		if (offset + 16 > nbytes) {
			len = nbytes - offset;
			if (blen == 2 && (len & 0x1)) {
				data = (uint8_t *)malloc(len+1);
				memcpy(data, buf + offset, len);
				data[len] = 0;
				len++;
				is_alloc = true;
			}
		}
		else {
			len = 16;
		}

		fprintf(fp, "%08x  ", (int)offset);
		if (blen == 1) {
			for (i = 0; i < (len - 1); i++)
				fprintf(fp, fmt, data[i]);
			fprintf(fp, fmt_end, data[len - 1]);
		} else if (blen == 2) {
			uint16_t *data2 = (uint16_t *)data;
			int len2 = len >> 1;
			for (i = 0; i < (len2 - 1); i++)
				fprintf(fp, fmt, data2[i]);
			fprintf(fp, fmt_end, data2[len2 - 1]);
		}
		fprintf(fp, "\n");
		offset += len;
	}
	if (is_alloc)
		free(data);
	fprintf(fp, "%08x\n", (int)nbytes);
}

void hexdump(uint8_t *buf, size_t nbytes, hexdump_flag_t flag)
{
	return fhexdump(stderr, buf, nbytes, flag);
}

static const char bit_rep[16][5] = {
    "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111",
};

static int byte_bin_str(uint8_t byte, char *str, size_t str_size)
{
    return snprintf(str, str_size, "%s,%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

const char *bin_str(uint8_t *buf, size_t nbytes, char *str, size_t str_size)
{
	int i, n;

	assert(NULL != str && 0 != str_size);
	assert(NULL != buf && 0 != nbytes);

	for (i = 0, n = 0; i < (nbytes-1) && n < str_size; i++) {
		n += byte_bin_str(buf[i], str + n, str_size - n);
		str[n++] = ' ';
	}

	if (n < str_size)
		byte_bin_str(buf[i], str + n, str_size - n);

	return str;
}

const char *hex_str(uint8_t *buf, size_t nbytes, char *str, size_t str_size)
{
	int i, n;

	assert(NULL != str && 0 != str_size);
	assert(NULL != buf && 0 != nbytes);

	for (i = 0, n = 0; i < (nbytes-1) && n < str_size; i++) {
		n += snprintf(str + n, str_size - n, "%02x", buf[i]);
		str[n++] = ' ';
	}

	if (n < str_size)
		snprintf(str + n, str_size - n, "%02x", buf[i]);

	return str;
}
