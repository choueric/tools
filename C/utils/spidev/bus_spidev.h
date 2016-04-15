#ifndef BUS_SPIDEV_H_
#define BUS_SPIDEV_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>

struct spi_attr {
	uint8_t mode;
	uint8_t bits;     // bits_per_word
	uint16_t delay;   // usec
	uint32_t speed;   // Hz
};

/*
 * @path: dev file path
 * @flags: O_RDWR, O_SYNC
 * @return: fd.
 */
int spidev_open(const char *path, int flags);

int spidev_close(int fd);

/*
 * just use one spi_ioc_transfer in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * spi_ioc_transfer in one ioctl, should use ioctl directly.
 * @return: return the length of data transfered; < 0, failed
 */
int spidev_read(int fd, uint8_t *buf, size_t count);

/*
 * just use one spi_ioc_transfer in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * spi_ioc_transfer in one ioctl, should use ioctl directly.
 * @return: return the length of data transfered; < 0, failed
 */
int spidev_write(int fd, uint8_t *buf, size_t count);

/*
 * first write @wbuf to @addr, then read data from @addr to @rbuf
 * @return: return the length of data transfered; < 0, failed
 */
int spidev_write_then_read(int fd, uint8_t *wbuf, int wlen, uint8_t *rbuf, int rlen);

/*
 * @return: return the length of data transfered; < 0, failed
 */
int spidev_write_twice(int fd, uint8_t *buf1, int len1, uint8_t *buf2, int len2);

/*
 * the length of @rbuf and @wbuf is @len.
 * @return: return the length of data transfered; < 0, failed
 */
int spidev_duplex(int fd, uint8_t *rbuf, uint8_t *wbuf, int len);

int spidev_write_then_duplex(int fd, uint8_t *buf1, int len1, uint8_t *wbuf, uint8_t *rbuf, int len);

/*
 * just set spidev's speed, bits_per_word and mode.
 */
int spidev_set_attr(int fd, struct spi_attr *p);

void spidev_print_attr(struct spi_attr *p);

#endif /* BUS_SPIDEV_H_ */
