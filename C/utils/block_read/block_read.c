#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>

#include "block_read.h"

typedef enum _bool_t {
	FALSE = 0,
	TRUE = 1,
} bool_t;

typedef struct _context {
	int fd;
	void *data;
	size_t count, request_size, buf_size;
	struct timeval *p_timeout;
} context;

// check if time is out, if is return true, otherwise return false and setup the
// new timeout value into @timeout
static bool_t check_and_adjust_timeout(struct timeval *start, int timeout_ms,
		struct timeval *p_timeout)
{
	struct timeval now;
	int elapse_ms = 0;

	// calculate elapse time(ms)
	gettimeofday(&now, NULL);
	elapse_ms = (now.tv_sec - start->tv_sec) * 1000000;
	elapse_ms += now.tv_usec - start->tv_usec;
	elapse_ms /= 1000;

	if (elapse_ms > timeout_ms)
		return TRUE;

	// ajust timeout
	timeout_ms -= elapse_ms;
	p_timeout->tv_sec = timeout_ms / 1000;
	p_timeout->tv_usec = (timeout_ms % 1000) * 1000;

	return FALSE;
}

static int select_read(context *ctx)
{
	fd_set fds;
	int ret;

	FD_ZERO(&fds);
	FD_SET(ctx->fd, &fds);

	ret = select(ctx->fd + 1, &fds, NULL, NULL, ctx->p_timeout);
	if (ret < 0)
		return -errno;
	else if (ret == 0 && !FD_ISSET(ctx->fd, &fds))
		return -ETIME;

	ret = read(ctx->fd, (uint8_t *)ctx->data + ctx->count, ctx->buf_size);
	if (ret < 0)
		return -errno;

	ctx->count += ret;
	ctx->buf_size = ctx->request_size - ctx->count;

	return 0;
}

static int read_nonblock(int fd, void *data, size_t *p_nbytes)
{
	ssize_t ret;

	ret = read(fd, data, *p_nbytes);
    if (ret < 0)
		return -errno;

    *p_nbytes = ret;
	return 0;
}

static int read_block(int fd, void *data, size_t *p_nbytes)
{
	int retval = 0;
	context ctx = {
		.fd = fd,
		.data = data,
		.count = 0,
		.request_size = *p_nbytes,
		.buf_size = *p_nbytes,
		.p_timeout = NULL,
	};

	while (ctx.count < ctx.request_size) {
		retval = select_read(&ctx);

		if (retval < 0) {
			if (retval != -EINTR && retval != -EAGAIN)
				break;
		}
	}

	*p_nbytes = ctx.count;
	return retval;
}

// return when:
// 1. error
// 2. get requested number data
// 3. timeout
static int read_timeout(int fd, void *data, size_t *p_nbytes, int timeout_ms)
{
	int retval = 0;
	struct timeval timeout, start;
	context ctx = {
		.fd = fd,
		.data = data,
		.count = 0,
		.request_size = *p_nbytes,
		.buf_size = *p_nbytes,
		.p_timeout = &timeout,
	};

	gettimeofday(&start, NULL);

	while (ctx.count < ctx.request_size) {
		if (check_and_adjust_timeout(&start, timeout_ms, ctx.p_timeout)) {
			retval = -ETIME;
			printf("timeout\n");
			break;
		}

		retval = select_read(&ctx);
		if (retval < 0) {
			if (retval != -EINTR && retval != -EAGAIN)
				break;
		}
	}

	*p_nbytes = ctx.count;
	return retval;
}

int block_read(int fd, void *data, size_t *p_nbytes, int timeout_ms)
{
	if (timeout_ms == 0)
		return read_nonblock(fd, data, p_nbytes);
	else if (timeout_ms < 0)
		return read_block(fd, data, p_nbytes);
	else
		return read_timeout(fd, data, p_nbytes, timeout_ms);
}
