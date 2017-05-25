/*
 * bus_i2c.cpp
 *
 *  Created on: 2014-7-7
 *      Author: 50215238
 */

#include "bus_i2cdev.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int i2cdev_transfer(int fd, uint8_t *buf, size_t count, uint16_t addr, int is_read)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg msgs;
	int ret = 0;

	msgs.addr  = addr;
	msgs.flags = is_read ? I2C_M_RD: 0;  // 0 for write
	msgs.len  = count;
	msgs.buf  = buf;

	msg_rdwr.msgs = &msgs;
	msg_rdwr.nmsgs = 1;

	if ( (ret = ioctl(fd, I2C_RDWR, &msg_rdwr)) < 0) {
		perror("i2c_ioctl rdwr");
		return -1;
	}

	return (ret == 1) ? 0: -1;
}

////////////////////////////////////////////////////////////////////////////////

int i2cdev_open(const char *path, int flags)
{
	int fd = 0;

	fd = open(path, flags);
	if (fd < 0) {
		perror("i2c_open failed");
		return -1;
	}

	return fd;
}

int i2cdev_close(int fd)
{
	return close(fd);
}

int i2cdev_read(int fd, uint8_t *buf, size_t count, uint16_t addr)
{
	return i2cdev_transfer(fd, buf, count, addr, 1);
}

int i2cdev_write(int fd, uint8_t *buf, size_t count, uint16_t addr)
{
	return i2cdev_transfer(fd, buf, count, addr, 0);
}


int i2cdev_write_then_read(int fd, uint8_t *wbuf, int wlen, uint16_t waddr, uint8_t *rbuf, int rlen, uint16_t raddr)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg msgs[2];
	int ret = 0;

	msgs[0].addr  = waddr;
	msgs[0].flags = 0;  // 0 for write
	msgs[0].len  = wlen;
	msgs[0].buf  = wbuf;

	msgs[1].addr  = raddr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len  = rlen;
	msgs[1].buf  = rbuf;

	msg_rdwr.msgs = msgs;
	msg_rdwr.nmsgs = 2;

	if ( (ret = ioctl(fd, I2C_RDWR, &msg_rdwr)) < 0) {
		perror("i2c_ioctl rdwr");
		return -1;
	}

	return (ret == 2) ? 0: -1;
}

int i2cdev_write_twice(int fd, uint8_t *buf1, int len1, uint16_t addr1, uint8_t *buf2, int len2, uint16_t addr2)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg msgs[2];
	int ret = 0;

	msgs[0].addr  = addr1;
	msgs[0].flags = 0;  // 0 for write
	msgs[0].len  = len1;
	msgs[0].buf  = buf1;

	msgs[1].addr  = addr2;
	msgs[1].flags = 0;
	msgs[1].len  = len2;
	msgs[1].buf  = buf2;

	msg_rdwr.msgs = msgs;
	msg_rdwr.nmsgs = 2;

	if ( (ret = ioctl(fd, I2C_RDWR, &msg_rdwr)) < 0) {
		perror("i2c_ioctl rdwr");
		return -1;
	}

	return (ret == 2) ? 0: -1;
}

int i2cdev_ioctl(int fd, uint32_t cmd, void *arg)
{
	return ioctl(fd, cmd, arg);
}
