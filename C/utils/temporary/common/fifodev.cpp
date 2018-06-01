/*
 * fifodev.cpp
 *
 *  Created on: 2014-9-4
 *      Author: 50215238
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "fifodev.h"

struct fifodev {
	struct kfifo *kbuf;
};

struct fifodev *fifodev_open(size_t size)
{
	struct fifodev *p = NULL;

	if (size == 0) {
		return NULL;
	}

	p = (struct fifodev *)malloc(sizeof(struct fifodev));
	if (p == NULL) {
		perror("malloc failed");
		return NULL;
	}
	memset(p, 0, sizeof(struct fifodev));

	p->kbuf = kfifo_alloc(size);
	if (p->kbuf == NULL) {
		free(p);
		return NULL;
	}

	return p;
}

int fifodev_close(struct fifodev *p)
{
	if (p == NULL || p->kbuf == NULL)
		return -1;

	kfifo_free(p->kbuf);
	free(p);

	return 0;
}

int fifodev_read(struct fifodev *p, unsigned char *buf, int count)
{
	int len;
	int ret;

	if (p == NULL || p->kbuf == NULL || buf == NULL || count <= 0)
		return -1;

	ret = kfifo_len(p->kbuf);
	len = count > ret ? ret: count;
	return kfifo_get(p->kbuf, buf, len);
}

int fifodev_write(struct fifodev *p, unsigned char *buf, int count)
{
	if (p == NULL || p->kbuf == NULL || buf == NULL || count <= 0)
		return -1;

	if (kfifo_free_len(p->kbuf) < count) {
		return 0;
	}

	return kfifo_put(p->kbuf, buf, count);
}

int fifodev_ioctl(struct fifodev *p, int cmd, int args)
{
	int ret = -1;

	if (p == NULL || p->kbuf == NULL)
		return -1;

	switch (cmd) {
	case FD_CMD_GET_DATA_LEN:
		ret = kfifo_len(p->kbuf);
		break;
	case FD_CMD_GET_FREE_LEN:
		ret = kfifo_free_len(p->kbuf);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}
