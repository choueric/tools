#ifndef BUS_I2CDEV_H_
#define BUS_I2CDEV_H_

#include "common.h"
#include "MM_Types.h"

/*
 * @path: dev file path
 * @flags: O_RDWR, O_SYNC
 * @return: fd.
 */
int i2cdev_open(const char *path, int flags = O_RDWR | O_SYNC);

int i2cdev_close(int fd);

/*
 * just use one i2c_msg in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * i2c_msgs in one ioctl, should use i2c_ioctl directly.
 * @return: 0, succeed; else, failed
 */
int i2cdev_read(int fd, uint8 *buf, size_t count, uint16 addr);

/*
 * just use one i2c_msg in one ioctl to transfer
 * @buf to @addr device. If want more effective way, that is using many
 * i2c_msgs in one ioctl, should use i2c_ioctl directly.
 * @return: 0, succeed; else, failed
 */
int i2cdev_write(int fd, uint8 *buf, size_t count, uint16 addr);

/*
 * first write @wbuf to @addr, then read data from @addr to @rbuf
 * @return: 0, succeed; else, failed
 */
int i2cdev_write_then_read(int fd, uint8 *wbuf, int wlen, uint16 waddr, uint8 *rbuf, int rlen, uint16 raddr);

/* @return: 0, succeed; else, failed */
int i2cdev_write_twice(int fd, uint8 *buf1, int len1, uint16 addr1, uint8 *buf2, int len2, uint16 addr2);

/*
 * @cmd: I2C_TIMEOUT, arg, int
 *       I2C_RETRIES, arg, int
 */
int i2cdev_ioctl(int fd, uint32 cmd, void *arg);

#endif /* BUS_I2CDEV_H_ */
