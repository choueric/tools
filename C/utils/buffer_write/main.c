#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
	uint8_t *start;
	size_t size;
	uint8_t *ptr;
} buffer_t;

int buf_create(buffer_t *buf, size_t size)
{
	buf->start = (uint8_t *)malloc(sizeof(uint8_t) * size);
	if (buf->start == NULL)
		return -ENOMEM;

	buf->ptr = buf->start;
	buf->size = size;
	return 0;
}

void buf_destroy(buffer_t *buf)
{
	free(buf->start);
	buf->start = buf->ptr = NULL;
	buf->size = 0;
}

bool buf_is_full(buffer_t *buf)
{
	return (buf->ptr == (buf->start + buf->size)) ? true: false;
}

bool buf_is_empty(buffer_t *buf)
{
	return (buf->ptr == buf->start) ? true: false;
}

size_t buf_remain(buffer_t *buf)
{
	return buf->size - (buf->ptr - buf->start);
}

void buf_flush(buffer_t *buf)
{
	// TODO: handle the data in buffer.
	buf->ptr = buf->start;
}

// write @nbytes bytes from @data into buffer @buf,
// @nbytes may be less or greater than @size.
void buf_write(buffer_t *buf, uint8_t *data, size_t nbytes)
{
	while (nbytes > 0) {
		size_t k, rem;
		if (buf_is_full(buf))
			buf_flush(buf);
		rem = buf_remain(buf);
		k = nbytes > rem ? rem: nbytes;
		memcpy(buf->ptr, data, k);
		buf->ptr += k;
		data += k;
		nbytes -= k;
	}
}

int main()
{
    return 0;
}
