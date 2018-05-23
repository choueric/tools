#pragma once

#include <stdint.h>

typedef enum {
	HEXDUMP_HEX_1 = 0,
	HEXDUMP_HEX_2,  // like '-x' in hexdump
	HEXDUMP_DEC_1,  //
	HEXDUMP_DEC_2,  // '-d
	HEXDUMP_OCT_1,  // '-b'
	HEXDUMP_OCT_2,  // '-o'
	HEXDUMP_CHAR,   // '-C'
} hexdump_flag_t;

void fhexdump(FILE *fp, uint8_t *buf, size_t nbytes, hexdump_flag_t flag);
void hexdump(uint8_t *buf, size_t nbytes, hexdump_flag_t flag);

// {0x01, 0x23, 0x45, 0x67}  -> "0000,0001 0010,0011 0100,0101 0110,0111"
const char *bin_str(uint8_t *buf, size_t nbytes, char *str, size_t str_size);

// {0x01, 0x23, 0x45, 0x67}  -> "01 23 45 67"
const char *hex_str(uint8_t *buf, size_t nbytes, char *str, size_t str_size);
