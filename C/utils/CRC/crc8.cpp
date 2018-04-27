/*
 * crc8.cpp
 *
 *  Created on: 2014-7-9
 *      Author: 50219854
 */
#include "DriverAccess.h"
#include "common.h"

/*
 * CRC8校验多项式：x^8+x^5+x^4+1
 */
const static UCHAR crc_table1[16] =
{
	0x00, 	0x31,	0x62,	0x53,
	0xC4,	0xF5,	0xA6,	0x97,
	0xB9,	0x88,	0xDB,	0xEA,
	0x7D,	0x4C,	0x1F,	0x2E
};

const static UCHAR crc_table2[16] =
{
	0x00,	0x43,	0x86,	0xC5,
	0x3D,	0x7E,	0xBB,	0xF8,
	0x7A,	0x39,	0xFC,	0xBF,
	0x47,	0x04,	0xC1,	0x82
};

/****************************************************************************
***************************************************************************/
uint8 getCRC8(uint8 *ptr, int size)
{
	uint8 crc = 0;
	uint8 data = 0;

	if(NULL == ptr || size == 0)
		return 0;

	while(size-- != 0)
	{
		data = (*ptr ^ crc) & 0xff;
		crc = crc_table1[data & 0x0f] ^ crc_table2[data >> 4];
		ptr ++;
	}
	return crc;
}


