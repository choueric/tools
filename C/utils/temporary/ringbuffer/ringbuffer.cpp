/*
 * A simple kernel FIFO implementation.
 *
 * Copyright (C) 2004 Stelian Pop <stelian@popies.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "ringbuffer.h"
#include <pthread.h>

void FIFO_Lock(pthread_mutex_t *lock){
    pthread_mutex_lock(lock);
}

void FIFO_unLock(pthread_mutex_t *lock){
    pthread_mutex_unlock(lock);
}

#ifndef min
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
/**
 * kfifo_init - allocates a new FIFO using a preallocated buffer
 * @buffer: the preallocated buffer to be used.
 * @size: the size of the internal buffer, this have to be a power of 2.
 * @gfp_mask: get_free_pages mask, passed to kmalloc()
 * @lock: the lock to be used to protect the fifo buffer
 *
 * Do NOT pass the kfifo to kfifo_free() after use ! Simply free the
 * struct kfifo with kfree().
 */
struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size)
{
	struct kfifo *fifo;

	fifo = new struct kfifo;
	if (!fifo) {
		printf("new fifo failed.\n");
		return NULL;
	}

	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = fifo->out = 0;
//	fifo->lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&fifo->lock, NULL);

	return fifo;
}
/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */

static inline int fls(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

/**
 * kfifo_alloc - allocates a new FIFO and its internal buffer
 * @size: the size of the internal buffer to be allocated.
 * @gfp_mask: get_free_pages mask, passed to kmalloc()
 * @lock: the lock to be used to protect the fifo buffer
 *
 * The size will be rounded-up to a power of 2.
 */
struct kfifo *kfifo_alloc(unsigned int size)
{
	unsigned char *buffer;
	struct kfifo *ret;
	int l = 0;

	/* size must be a power of 2 */
	if ((size & (size - 1))) {
		//DRV_LOG("buffer size: 0x%x is not a power of 2. \n", size);
		while((1UL << l) < size) {
			l++;
		}

		size = 1UL << l;
	}

	/*
	 * round up to the next power of 2, since our 'let the indices
	 * wrap' tachnique works only in this case.
	 */
	if (size & (size - 1)) {
		if (size > 0x80000000) {
			printf("FIFO size over the max.\n");
			size = 1UL << fls(size - 1);
		}
	}

	buffer = new unsigned char[size];
	if (!buffer) {
		printf("new buffer failed.\n");
		return NULL;
	}

	ret = kfifo_init(buffer, size);
	if (ret == NULL) {
		printf("%s:ret failed.\n", __func__);
		delete[] buffer;
	}

	return ret;
}

/**
 * kfifo_free - frees the FIFO
 * @fifo: the fifo to be freed.
 */
void kfifo_free(struct kfifo *fifo)
{
	if (fifo == NULL)
		return;
	delete[] fifo->buffer;
	delete fifo;
}

/**
 * __kfifo_put - puts some data into the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most 'len' bytes from the 'buffer' into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
static unsigned int __kfifo_put(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->size - fifo->in + fifo->out);

	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);

	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;

	return len;
}

/**
 * __kfifo_get - gets some data from the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most 'len' bytes from the FIFO into the
 * 'buffer' and returns the number of copied bytes.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
static unsigned int __kfifo_get(struct kfifo *fifo,
			 unsigned char *buffer, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);

	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);

	fifo->out += len;

	return len;
}

static void __kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}

/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
void kfifo_reset(struct kfifo *fifo)
{

	FIFO_Lock(&fifo->lock);

	__kfifo_reset(fifo);

	FIFO_unLock(&fifo->lock);
}

/**
 * kfifo_put - puts some data into the FIFO
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most 'len' bytes from the 'buffer' into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 */
unsigned int kfifo_put(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned int ret;

	FIFO_Lock(&fifo->lock);

	ret = __kfifo_put(fifo, buffer, len);

	FIFO_unLock(&fifo->lock);

	return ret;
}

/**
 * kfifo_get - gets some data from the FIFO
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most 'len' bytes from the FIFO into the
 * 'buffer' and returns the number of copied bytes.
 */
unsigned int kfifo_get(struct kfifo *fifo,
				     unsigned char *buffer, unsigned int len)
{
	unsigned int ret;

	FIFO_Lock(&fifo->lock);

	ret = __kfifo_get(fifo, buffer, len);

	/*
	 * optimization: if the FIFO is empty, set the indices to 0
	 * so we don't wrap the next time
	 */
	if (fifo->in == fifo->out)
		fifo->in = fifo->out = 0;

	FIFO_unLock(&fifo->lock);

	return ret;
}

/**
 * __kfifo_len - returns the number of bytes available in the FIFO, no locking version
 * @fifo: the fifo to be used.
 */
static unsigned int __kfifo_len(struct kfifo *fifo)
{
	return fifo->in - fifo->out;
}

/**
 * kfifo_len - returns the number of bytes available in the FIFO
 * @fifo: the fifo to be used.
 */
int kfifo_len(struct kfifo *fifo)
{
	unsigned int ret;

	FIFO_Lock(&fifo->lock);

	ret = __kfifo_len(fifo);

	FIFO_unLock(&fifo->lock);

	return ret;
}

static unsigned int __kfifo_free_len(struct kfifo *fifo)
{
	return (fifo->size - (fifo->in - fifo->out));
}

int kfifo_free_len(struct kfifo *fifo)
{
	unsigned int ret;

	FIFO_Lock(&fifo->lock);

	ret = __kfifo_free_len(fifo);

	FIFO_unLock(&fifo->lock);

	return ret;
}
