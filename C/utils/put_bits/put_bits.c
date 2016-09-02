#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "put_bits.h"


/* @buffer_size in byte */
void init_bit_stream(BitStream *s, uint8_t *buffer, int buffer_size)
{
    if(buffer_size < 0) {
        buffer_size = 0;
        buffer = NULL;
    }

    s->size_in_bits= 8*buffer_size;
    s->buf = buffer;
    s->buf_end = s->buf + buffer_size;
    s->buf_ptr = s->buf;
    s->bit_left=32;
    s->bit_buf=0;
}

// count of bits used
int put_bits_count(BitStream *s)
{
    return (s->buf_ptr - s->buf) * 8 + (32 - s->bit_left);
}

void flush_put_bits(BitStream *s)
{
    while (s->bit_left < 32) {
        /* TODO: should test end of buffer */
        *s->buf_ptr++=s->bit_buf;
        s->bit_buf>>=8;
        s->bit_left+=8;
    }
    s->bit_left=32;
    s->bit_buf=0;
}

void put_bits(BitStream *s, int n, unsigned int value)
{
    unsigned int bit_buf;
    int bit_left;

    assert(n <= 31 && value < (1U << n));

    bit_buf = s->bit_buf;
    bit_left = s->bit_left;

    bit_buf |= value << (32 - bit_left);
    if (n >= bit_left) {
        *(uint32_t *)s->buf_ptr = bit_buf;
        s->buf_ptr+=4;
        bit_buf = (bit_left==32)?0:value >> bit_left;
        bit_left+=32;
    }
    bit_left-=n;

    s->bit_buf = bit_buf;
    s->bit_left = bit_left;
}

void put_sbits(BitStream *pb, int n, int32_t value)
{
    assert(n >= 0 && n <= 31);

    put_bits(pb, n, value & ((1<<n)-1));
}

void skip_put_bytes(BitStream *s, int n)
{
	assert((put_bits_count(s)&7)==0);
	assert(s->bit_left==32);
	s->buf_ptr += n;
}

void skip_put_bits(BitStream *s, int n)
{
	s->bit_left -= n;
	s->buf_ptr-= 4*(s->bit_left>>5);
	s->bit_left &= 31;
}

/* left is high bit, MSB */
static void print_binray(uint8_t v)
{
	int i = 0;
	for (; i < 8; i++) {
		printf("%d", (v & 0x80) >> 7);
		if (i == 3)
			printf(",");
		v = v << 1;
	}
	printf("\n");
}

/* high is high byte */
void print_stream(BitStream *s)
{
	int byte_num = s->size_in_bits / 8 - 1;
	for (; byte_num >= 0; byte_num--) {
		printf("[%03d]: ", byte_num);
		print_binray(s->buf[byte_num]);
	}
}
