#pragma once
#include <stdint.h>

typedef struct BitStream {
    uint32_t bit_buf;
    int bit_left;
    uint8_t *buf, *buf_ptr, *buf_end;
    int size_in_bits;
}BitStream;

void init_bit_stream(BitStream *s, uint8_t *buffer, int buffer_size);
int put_bits_count(BitStream *s);
void flush_put_bits(BitStream *s);
void put_bits(BitStream *s, int n, unsigned int value);

void put_sbits(BitStream *pb, int n, int32_t value);
void skip_put_bytes(BitStream *s, int n);
void skip_put_bits(BitStream *s, int n);
void print_stream(BitStream *s);
