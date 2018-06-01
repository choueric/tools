#include "spidev.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int spi_write(int fd, uint8_t *buf, int len)
{
	int ret = 0;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)buf,
		.len = len,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		perror("spi write then read fail");
	}

	return ret;
	
}

int spi_write_twice(int fd, uint8_t *buf1, int len1, uint8_t *buf2, int len2)
{
	int ret = 0;
	struct spi_ioc_transfer	tr[2];

	memset(tr, 0, sizeof(struct spi_ioc_transfer) * 2);
	tr[0].tx_buf = (unsigned long)buf1;
	tr[0].len = len1;
	tr[1].tx_buf = (unsigned long)buf2;
	tr[1].len = len2;

	ret = ioctl(fd, SPI_IOC_MESSAGE(2), tr);
	if (ret < 0) {
		perror("spi write twice failed.");
		return -1;
	}

	return ret;
}

int spi_write_then_read(int fd, uint8_t *wbuf, int wlen, uint8_t *rbuf, int rlen)
{
	struct spi_ioc_transfer tr[2];
	int ret = 0;

	memset((void *)tr, 0, sizeof(tr));
	tr[0].tx_buf = (unsigned long)wbuf;
	tr[0].len = wlen;
	tr[1].rx_buf = (unsigned long)rbuf;
	tr[1].len = rlen;

	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &tr);
	if (ret < 0) {
		perror("spi write then read fail");
	}

	return ret;
}

int spi_init_attr(int fd, struct spi_cfg *p)
{
	int ret = 0;

	/* spi mode */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &p->mode);
	if (ret == -1) {
		perror("can't set spi write mode");
        return -1;
    }
	ret = ioctl(fd, SPI_IOC_RD_MODE, &p->mode);
	if (ret == -1) {
		perror("can't get spi read mode");
        return -1;
    }

	 /* bits per word */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &p->bits);
	if (ret == -1) {
		perror("can't set bits per word");
        return -1;
    }
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &p->bits);
	if (ret == -1) {
		perror("can't get bits per word");
        return -1;
    }

	 /* max speed hz */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &p->speed);
	if (ret == -1) {
		perror("can't set max speed hz");
        return -1;
    }
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &p->speed);
	if (ret == -1) {
		perror("can't get max speed hz");
        return -1;
    }

    return 0;
}

void spi_print_cfg(struct spi_cfg *p)
{
	printf("-------------------------------------\n");
	printf("device: %s\n", p->device);
	printf("spi mode: %d\n", p->mode);
	printf("bits per word: %d\n", p->bits);
	printf("max speed: %d Hz (%d KHz)\n", p->speed, p->speed/1000);
	printf("delay = %d\n", p->delay);
	printf("-------------------------------------\n");
}

