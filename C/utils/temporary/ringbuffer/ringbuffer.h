/*
 * ringbuffer.h
 *
 *  Created on: 2014-7-22
 *      Author: 50219854
 *     文件参考内容参考内核kfifo.c和kfifo.h文件
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct kfifo {
	unsigned char *buffer;	/* the buffer holding the data */
	size_t size;	/* the size of the allocated buffer */
	unsigned int in;	/* data is added at offset (in % size) */
	unsigned int out;	/* data is extracted from off. (out % size) */
	pthread_mutex_t lock;
};

struct kfifo *kfifo_init(unsigned char *buffer, unsigned int size);
struct kfifo *kfifo_alloc(unsigned int size);
void kfifo_free(struct kfifo *fifo);
void FIFO_Lock(pthread_mutex_t *lock);

void FIFO_unLock(pthread_mutex_t *lock);
/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
void kfifo_reset(struct kfifo *fifo);

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
unsigned int kfifo_put(struct kfifo *fifo, unsigned char *buffer, unsigned int len);
/**
 * kfifo_get - gets some data from the FIFO
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most 'len' bytes from the FIFO into the
 * 'buffer' and returns the number of copied bytes.
 */
unsigned int kfifo_get(struct kfifo *fifo, unsigned char *buffer, unsigned int len);

/**
 * kfifo_len - returns the number of bytes available in the FIFO
 * @fifo: the fifo to be used.
 */
int kfifo_len(struct kfifo *fifo);
int kfifo_free_len(struct kfifo *fifo);

#endif /* RINGBUFFER_H_ */
