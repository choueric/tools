#include "bus_spidev.h"
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>


int spidev_open(const char *path, int flags)
{
	int fd = 0;

	fd = open(path, flags | O_RDWR | O_SYNC);
	if (fd < 0) {
		perror("spidev_open failed");
		return -1;
	}

	return fd;
}

int spidev_close(int fd)
{
	return close(fd);
}

int spidev_read(int fd, uint8_t *buf, size_t count)
{
	int ret = 0;
	struct spi_ioc_transfer tr;

	bzero(&tr, sizeof(struct spi_ioc_transfer));
	tr.rx_buf = (unsigned long)buf;
	tr.len = count;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		perror("spi read ioctl failed.");
		return -1;
	}

	return ret;
}

int spidev_write(int fd, uint8_t *buf, size_t count)
{
	int ret = 0;
	struct spi_ioc_transfer tr;

	bzero(&tr, sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)buf;
	tr.len = count;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		perror("spi write ioctl failed.");
		return -1;
	}

	return ret;
}

int spidev_write_then_read(int fd, uint8_t *wbuf, int wlen, uint8_t *rbuf, int rlen)
{
	struct spi_ioc_transfer tr[2];
	int ret = 0;

	bzero(tr, sizeof(struct spi_ioc_transfer) * 2);
	tr[0].tx_buf = (unsigned long)wbuf;
	tr[0].len = wlen;
	tr[1].rx_buf = (unsigned long)rbuf;
	tr[1].len = rlen;

	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &tr);
	if (ret < 0) {
		perror("spi write ioctl failed.");
		return -1;
	}

	return ret;
}

int spidev_write_twice(int fd, uint8_t *buf1, int len1, uint8_t *buf2, int len2)
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

int spidev_duplex(int fd, uint8_t *rbuf, uint8_t *wbuf, int len)
{
	int ret = 0;
	struct spi_ioc_transfer tr;

	bzero(&tr, sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)wbuf;
	tr.rx_buf = (unsigned long)rbuf;
	tr.len = len;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		perror("spi write ioctl failed.");
		return -1;
	}

	return ret;
}

int spidev_write_then_duplex(int fd, uint8_t *buf1, int len1, uint8_t *wbuf, uint8_t *rbuf, int len)
{
	struct spi_ioc_transfer tr[2];
	int ret = 0;

	bzero(tr, sizeof(struct spi_ioc_transfer) * 2);
	tr[0].tx_buf = (unsigned long)buf1;
	tr[0].len = len1;
	tr[1].tx_buf = (unsigned long)wbuf;
	tr[1].rx_buf = (unsigned long)rbuf;
	tr[1].len = len;

	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &tr);
	if (ret < 0) {
		perror("spi write ioctl failed.");
		return -1;
	}

	return ret;
}

int spidev_set_attr(int fd, struct spi_attr *p)
{
	int ret = 0;

	ret = ioctl(fd, SPI_IOC_WR_MODE, &p->mode);
	if (ret < 0) {
		perror("set spi mode failed");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &p->bits);
	if (ret < 0) {
		perror("set spi bits failed");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &p->speed);
	if (ret < 0) {
		perror("set spi speed failed");
		return -1;
	}

	return 0;
}

void spidev_print_attr(struct spi_attr *p)
{
	printf("-------------------------------------\n");
	printf("spi mode: %d\n", p->mode);
	printf("bits per word: %d\n", p->bits);
	printf("max speed: %d Hz (%d KHz)\n", p->speed, p->speed/1000);
	printf("delay = %d\n", p->delay);
	printf("-------------------------------------\n");
}
