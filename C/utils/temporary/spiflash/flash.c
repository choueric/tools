#include "flash.h"


/*
 * 524,288 butes (8 bits each)
 * 8 sectors (512Kbits, 65536 bytes each)
 * 2048 pages (256 bytes each)
 * 256 B/page, 256 pages/sector, 8 secotrs/chip
 * page program, sector / chip earse, but no page erase
 * sector 0: 00000h - 0ffffh
 * sector 1: 10000h - 1ffffh
 * sector 2: 20000h - 2ffffh
 * sector 3: 30000h - 3ffffh
 * sector 4: 40000h - 4ffffh
 * sector 5: 50000h - 5ffffh
 * sector 6: 60000h - 6ffffh
 * sector 7: 70000h - 7ffffh
 */

/*
 * w25q16
 *
 * 2MB
 *
 * 32 blocks/chip (64KB/block)
 * 16 sectors/block (4KB/sector) (000h - fffh)
 * 16 pages/sector (256B/page) (00h - ffh)
 *
 * sector 0: xx0000h - xx0fffh
 * sector 1: xx1000h - xx1fffh
 * sector 2: xx2000h - xx2fffh
 * sector 3: xx3000h - xx3fffh
 * sector 4: xx4000h - xx4fffh
 * sector 5: xx5000h - xx5fffh
 * sector 6: xx6000h - xx6fffh
 * sector 7: xx7000h - xx7fffh
 * sector 8: xx8000h - xx8fffh
 * sector 9: xx9000h - xx9fffh
 * sector 10: xxa000h - xxafffh
 * sector 11: xxb000h - xxbfffh
 * sector 12: xxc000h - xxcfffh
 * sector 13: xxd000h - xxdfffh
 * sector 14: xxe000h - xxefffh
 * sector 15: xxf000h - xxffffh
 */

#define	OPCODE_WREN			0x06    // write enable
#define	OPCODE_WRDI			0x04    // write disable
#define	OPCODE_RDSR			0x05    // read status register
#define	OPCODE_WRSR			0x01    // write status register
#define	OPCODE_READ			0x03    // read data bytes
#define	OPCODE_FAST_READ	0x0b    // read data bytes at higher speed
#define OPCODE_PP			0x02    // page program
#if 1 //w25q16
#define OPCODE_SE			0x20	// sector erase
#else
#define OPCODE_SE			0xd8
#endif
#define OPCODE_BE			0xc7	// bulk earse
#define OPCODE_DP			0xb9	// deep power-down
#define OPCODE_RES			0xab	// release from deep power-down / read Electornic signature
#define OPCODE_JEDEC		0x9f	// read jedec id

#define STS_WIP (1 << 0)  // RO
#define STS_WEL (1 << 1)  // RO
#define STS_BP0 (1 << 2)  // RW
#define STS_BP1 (1 << 3)  // RW
#define STS_BP2 (1 << 4)  // RW
#define STS_SRWD (1 << 7) // RW

#define WAIT_TIME 100   // ms

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf
#endif

struct flash_info {
	int blocks_per_chip;
	int sectors_per_block;
	int pages_per_sector;
	int page_size;  // Byte
};

struct flash_info w25q16 = {
	.blocks_per_chip = 32,
	.sectors_per_block = 16,
	.pages_per_sector = 16,
	.page_size = 256,
};

/**********************************************************/

static int spi_write(int fd, uint8_t *buf, int len)
{
	int ret = 0;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)buf,
		.len = len,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("spi write then read fail");
	}

	return ret;
	
}

static int spi_write_then_read(int fd, uint8_t *wbuf, int wlen, uint8_t *rbuf, int rlen)
{
	struct spi_ioc_transfer tr[2];
	int ret = 0;

	memset((void *)tr, 0, sizeof(tr));
	tr[0].tx_buf = (unsigned long)wbuf;
	tr[0].len = wlen;
	tr[1].rx_buf = (unsigned long)rbuf;
	tr[1].len = rlen;

	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &tr);
	if (ret < 2) {
		perror("spi write then read fail");
	}

	return ret;
}

/**********************************************************/

static int do_write_enable(int fd)
{
	unsigned char op = OPCODE_WREN;

	return spi_write(fd, &op, 1);
}

#if 0
static int do_write_disable(int fd)
{
	uint8_t op = OPCODE_WRDI;

	return spi_write(fd, &op, 1);
}
#endif

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

static int do_init_write(int fd, struct flash_info *p)
{
	uint8_t *buf = NULL;
	int i, n;
	int addr = 0;

	buf = malloc(p->page_size);
	if (buf == NULL) {
		perror("malloc failed");
		return -1;
	}

	n = p->pages_per_sector * p->sectors_per_block * p->blocks_per_chip;
	for (i = 0, addr = 0x0; i < n; i++) {
		memset(buf, i, p->page_size);
		flash_page_program(fd, addr, buf, p->page_size);
		addr += p->page_size;
	}

	free(buf);
	return 0;
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
	dprintf("== %s enter ==\n", __FUNCTION__);
	// write enalbe
	do_write_enable(fd);
	// check WEL
	while (!check_wel(fd))
		usleep(1000 * WAIT_TIME);
	// page program
	do_page_program(fd, addr, buf, len);
	// check WIP
	while (check_wip(fd))
		usleep(1000 * WAIT_TIME);
	dprintf("== %s return ==\n", __FUNCTION__);

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
	ret = do_read_data(fd, addr, buf, len);
	dprintf("== %s return ==\n", __FUNCTION__);

	return ret;
}

int flash_w25q16_init_write(int fd)
{
	return do_init_write(fd, &w25q16);
}
