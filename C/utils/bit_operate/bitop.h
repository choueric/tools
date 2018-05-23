#pragma once

#include <stdint.h>

#define BIT(n) (1 << (n))

// Create mask start from a to b. e.g. BIT_MASK(2, 3) => 0000,0110
// Start from 0.
#define BIT_MASK(a, b) (((uint8_t)-1 >> (7 - (b))) & ~((1U << (a)) - 1)) // for 8bit
#define BIT_MASK_16(a, b) (((uint16_t)-1 >> (15-(b))) & ~((1U<<(a))-1))  // for 16bit
#define BIT_MASK_32(a, b) (((uint32_t)-1 >> (31-(b))) & ~((1U<<(a))-1))  // for 32bit


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

/*
 * mask out the value of a range filed in register
 *
 * e.g.: register A is 8bit, and bit[2-3] means control mode,
 * 0: mode A, 1: mode B, 2: mode C, 3: mode D.
 * this function get the mode value from value of register A
 *   mode = mask_value(regVal, BIT(2) | BIT(3))
 */
static inline uint8_t mask_value(uint8_t val, uint8_t mask)
{
	return (val & mask) >> (ffs(mask) - 1);
}
