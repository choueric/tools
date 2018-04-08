#pragma once

#define BIT(n) (1 << (n))

// NOTE: only for unsigned char type (8bit)
#define BIT_MASK(a, b) (((uint8_t)-1 >> (7 - (b))) & ~((1U << (a)) - 1))

int MASK(int start, int len);
int fieldValue(int val, int mask);

/**
 * ffs - find first set-bit from LSB to MSB
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}
