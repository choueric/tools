#include "flash.h"
#include "spidev.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * M25P40
 * VID(JEDEC ID): 0x20; PID: 0x2013
 * RES instruction signature: 12h
 * 256 Byte/page, 256 pages/sector, 8 secotrs/chip
 * page program, 1-256 Bytes
 * sector erase, one sector once
 * bulk earse, whole chip once
 * no page erase.
 * sector 0: 00000h - 0ffffh
 * sector 1: 10000h - 1ffffh
 * sector 2: 20000h - 2ffffh
 * sector 3: 30000h - 3ffffh
 * sector 4: 40000h - 4ffffh
 * sector 5: 50000h - 5ffffh
 * sector 6: 60000h - 6ffffh
 * sector 7: 70000h - 7ffffh
 * SPI Supported Mode:
 * - CPOL=0, CPHA=0
 * - CPOL=1, CPHA=1
 */

#define	OPCODE_WREN			0x06    // write enable
#define	OPCODE_WRDI			0x04    // write disable
#define OPCODE_JEDEC		0x9F	// read jedec id
#define OPCODE_RDID         OPCODE_JEDEC
#define	OPCODE_RDSR			0x05    // read status register
#define	OPCODE_WRSR			0x01    // write status register
#define	OPCODE_READ			0x03    // read data bytes
#define	OPCODE_FAST_READ	0x0B    // read data bytes at higher speed
#define OPCODE_PP			0x02    // page program
#define OPCODE_SE			0xD8    // sector erase
#define OPCODE_BE			0xC7	// bulk earse
#define OPCODE_DP			0xB9	// deep power-down
#define OPCODE_RES			0xAB	// release from deep power-down / read Electornic signature

#define WAIT_TIME 10   // ms

//#define DEBUG

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf
#endif

/**********************************************************/

static int do_write_enable(int fd)
{
	unsigned char op = OPCODE_WREN;

	return spi_write(fd, &op, 1);
}

static int do_write_disable(int fd)
{
	uint8_t op = OPCODE_WRDI;

	return spi_write(fd, &op, 1);
}

static uint8_t do_read_status_reg(int fd)
{
	uint8_t val = 0;
	uint8_t op = OPCODE_RDSR;

	if (spi_write_then_read(fd, &op, 1, &val, 1) < 0)
		return 0;
	else
		return val;
}

static int do_write_status_reg(int fd, uint8_t val)
{
	uint8_t buf[2] = {OPCODE_WRSR, val};
	return spi_write(fd, buf, 2);
}

// whole memroy can be read by a single READ instruction.
// the address will increase automaticlly after data being sent out.
// terminated by driving CS high.
static int do_read_data(int fd, int addr, uint8_t *buf, int len)
{
	uint8_t wbuf[4] = {OPCODE_READ, (addr & 0xff0000) >> 16, 
		(addr & 0xff00) >> 8, addr & 0xff};

	return spi_write_then_read(fd, wbuf, 4, buf, len);
}

// need dummy byte after address
static int do_fast_read(int fd, int addr, uint8_t *buf, int len)
{
	uint8_t wbuf[5] = {OPCODE_FAST_READ, (addr & 0xff0000) >> 16, 
		(addr & 0xff00) >> 8, addr & 0xff, 0x00};

	return spi_write_then_read(fd, wbuf, 5, buf, len);
}

// len should not greater than page_size(256).
// after CS is driven high, the self-timed page program cycle is initiated.
// the WIP is 1 when cycle is working.
static int do_page_program(int fd, int addr, uint8_t *buf, int len)
{
	uint8_t *wbuf = NULL;
	int ret;

	wbuf = malloc(len + 4);
	if (wbuf == NULL) {
		perror("malloc failed");
		return -1;
	}

	wbuf[0] = OPCODE_PP;
	wbuf[1] = (addr & 0xff0000) >> 16;
	wbuf[2] = (addr & 0xff00) >> 8;
	wbuf[3] = addr & 0xff;
	memcpy(wbuf + 4, buf, len);
	
	ret = spi_write(fd, wbuf, len + 4);
	free(wbuf);
	return ret;
}

// any address inside the sector is a valid address
// as soon as the CS is driven high, the self-timed sector erase cycle is initiated.
// the WIP is 1 when cycle is working.
static int do_sector_erase(int fd, int addr)
{
	uint8_t buf[4] = {OPCODE_SE, (addr & 0xff0000) >> 16, 
		(addr & 0xff00) >> 8, addr & 0xff};

	return spi_write(fd, buf, 4);
}

// as soon as the CS is driven high, the self-timed sector erase cycle is initiated.
// the WIP is 1 when cycle is working.
static int do_bulk_erase(int fd)
{
	uint8_t op = OPCODE_BE;

	return spi_write(fd, &op, 1);
}

static int do_deep_powerdown(int fd)
{
	uint8_t op = OPCODE_DP;

	return spi_write(fd, &op, 1);
}

// release from deep power-down and read electronic signature
// the electronic signature of M25P40 is 12h
// 3 dummy bytes.
static uint8_t do_res(int fd)
{
	uint8_t buf[4] = {OPCODE_RES, 0x00, 0x00, 0x00};
	uint8_t sign = 0;

	spi_write_then_read(fd, buf, 4, &sign, 1);
	return sign;
}

static int do_read_jedec(int fd)
{
	uint8_t buf[1] = {OPCODE_JEDEC};
	uint8_t id[3] = {0};
	int ret = 0;

	spi_write_then_read(fd, buf, 1, id, 3);
	ret = (id[2] << 16) | (id[1] << 8) | id[0];
	return ret;
}

// return 1 if write in progress
static int check_wip(int fd)
{
	return do_read_status_reg(fd) & STS_WIP;
}

// return 1 if write enable
static int check_wel(int fd)
{
	return do_read_status_reg(fd) & STS_WEL;
}

/*----------------------------------------------------------------------*/
int flash_write_enable(int fd)
{
    return do_write_enable(fd);
}

int flash_write_disable(int fd)
{
    return do_write_disable(fd);
}

uint8_t flash_read_status(int fd)
{
	return do_read_status_reg(fd);
}

uint8_t flash_read_sign(int fd)
{
	return do_res(fd);
}

int flash_read_jedec(int fd)
{
	return do_read_jedec(fd);
}

int flash_page_program(int fd, int addr, uint8_t *buf, int len)
{
    int i = 0;
	dprintf("== %s enter ==\n", __FUNCTION__);
	// write enalbe
	do_write_enable(fd);
	// check WEL
	while (!check_wel(fd)) {
        i++;
        if (i > 500) {
            printf("wait too long\n");
            return -1;
        }
		usleep(1000 * WAIT_TIME);
    }
	// page program
	do_page_program(fd, addr, buf, len);
	// check WIP
    i = 0;
	while (check_wip(fd)) {
        i++;
        if (i > 500) {
            printf("wait too long\n");
            return -1;
        }
		usleep(1000 * WAIT_TIME);
    }
	dprintf("== %s return ==\n", __FUNCTION__);
#if TEST_S_PAGE
    if (len < 256) {
        dump_buf(buf, len);
    }
#endif

	return 0;
}

int flash_sector_erase(int fd, int addr)
{
	dprintf("== %s enter ==\n", __FUNCTION__);
	// write enable
	do_write_enable(fd);
	// check WEL
	while (!check_wel(fd))
		usleep(1000 * WAIT_TIME);
	// sector erase
	do_sector_erase(fd, addr);
	// check WIP
	while (check_wip(fd))
		usleep(1000 * WAIT_TIME);
	dprintf("== %s return ==\n", __FUNCTION__);

	return 0;
}

// whole chip erased
int flash_bulk_erase(int fd)
{
	dprintf("== %s enter ==\n", __FUNCTION__);
	// write enable
	do_write_enable(fd);
	// check WEL
	while (!check_wel(fd))
		usleep(1000 * WAIT_TIME);
	// bulk erase
	do_bulk_erase(fd);
	// check WIP
	while (check_wip(fd))
		usleep(1000 * WAIT_TIME);
	dprintf("== %s return ==\n", __FUNCTION__);

	return 0;
}

// when WIP is 0, the funcion is completed.
int flash_write_status_reg(int fd, uint8_t val)
{
	dprintf("== %s enter ==\n", __FUNCTION__);
	// write enable
	do_write_enable(fd);
	// check WEL
	while (!check_wel(fd))
		usleep(1000 * WAIT_TIME);
	// write status reg
	do_write_status_reg(fd, val);
	// check WIP
	while (check_wip(fd))
		usleep(1000 * WAIT_TIME);
	dprintf("== %s return ==\n", __FUNCTION__);
	return 0;
}

int flash_read_data(int fd, int addr, uint8_t *buf, int len)
{
	int ret;

	dprintf("== %s entry ==\n", __FUNCTION__);
	ret = do_read_data(fd, addr, buf, len);
	dprintf("== %s return ==\n", __FUNCTION__);

	return ret;
}

int flash_fast_read(int fd, int addr, uint8_t *buf, int len)
{
	int ret;

	dprintf("== %s entry ==\n", __FUNCTION__);
	ret = do_fast_read(fd, addr, buf, len);
	dprintf("== %s return ==\n", __FUNCTION__);

	return ret;
}
