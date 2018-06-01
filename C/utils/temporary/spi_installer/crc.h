#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>
#include <sys/types.h>

uint32_t getCRC32(uint32_t crc, const uint8_t *pbuf, int length, int xorf);
unsigned char getCRC8(unsigned char *ptr, int size);

#endif

