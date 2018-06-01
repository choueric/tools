#ifndef _SPIDEV_H_
#define _SPIDEV_H_

#include <stdint.h>
#include <sys/types.h>

// spidev kernel driver's buffer size 4K -> 16K
#define SPIDEV_DRIVER_BUF_SIZE (4 * 1024)

struct spi_cfg {
	char *device;
	uint8_t mode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
};


int spi_write(int fd, uint8_t *buf, int len);

int spi_write_then_read(int fd, uint8_t *wbuf, int wlen, uint8_t *rbuf, int rlen);
int spi_write_twice(int fd, uint8_t *buf1, int len1, uint8_t *buf2, int len2);

int spi_init_attr(int fd, struct spi_cfg *p);
void spi_print_cfg(struct spi_cfg *p);

#endif
