#ifndef BUS_I2CDEV_H_
#define BUS_I2CDEV_H_

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * @path: dev file path
 * @flags: O_RDWR, O_SYNC
 * @return: fd.
 */
int i2cdev_open(const char *path, int flags);

int i2cdev_close(int fd);

/*
 * just use one i2c_msg in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * i2c_msgs in one ioctl, should use i2c_ioctl directly.
 * @return: 0, succeed; else, failed
 */
int i2cdev_read(int fd, uint8_t *buf, size_t count, uint16_t addr);

/*
 * just use one i2c_msg in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * i2c_msgs in one ioctl, should use i2c_ioctl directly.
 * @return: 0, succeed; else, failed
 */
int i2cdev_write(int fd, uint8_t *buf, size_t count, uint16_t addr);

/*
 * first write @wbuf to @addr, then read data from @addr to @rbuf
 * @return: 0, succeed; else, failed
 */
int i2cdev_write_then_read(int fd, uint8_t *wbuf, int wlen, uint16_t waddr, uint8_t *rbuf, int rlen, uint16_t raddr);

/* @return: 0, succeed; else, failed */
int i2cdev_write_twice(int fd, uint8_t *buf1, int len1, uint16_t addr1, uint8_t *buf2, int len2, uint16_t addr2);

/*
 * @cmd: I2C_TIMEOUT, arg, int
 *       I2C_RETRIES, arg, int
 */
int i2cdev_ioctl(int fd, uint32_t cmd, void *arg);

#endif /* BUS_I2CDEV_H_ */
