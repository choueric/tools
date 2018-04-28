#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#define MASK_32(a, b) (((uint32_t)-1 >> (31-(b))) & ~((1U<<(a))-1))
static uint32_t loop_mask_32(int n)
{
	uint32_t val = 0;
	int i = 0;

	for (i = 0; i <= n; i++)
		val |= (1 << i);

	return val;
}

static uint32_t mask_32(int a, int b)
{
	if (a > b) {
		int t = a;
		a = b;
		b = t;
	}
	uint32_t b_mask = loop_mask_32(b);
	uint32_t a_mask = ~(loop_mask_32(a-1));

	return b_mask & a_mask;
}

#define MASK_8(a, b) (((uint8_t)-1 >> (7-(b))) & ~((1U<<(a))-1))

static uint8_t loop_mask_8(int n)
{
	uint8_t val = 0;

	for (int i = 0; i <= n; i++)
		val |= (1 << i);

	return val;
}

static uint8_t mask_8(int a, int b)
{
	if (a > b) {
		int t = a;
		a = b;
		b = t;
	}

	if (a == b)
		return (1 << a);

	uint8_t b_mask = loop_mask_8(b);
	uint8_t a_mask = ~(loop_mask_8(a-1));

	return b_mask & a_mask;
}

int main()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (i < j) {
				uint8_t a = mask_8(i, j);
				uint8_t b = MASK_8(i, j);
				if (a != b)
					printf("(%d, %d): %#x, %#x\n", i, j, a, b);
			}

	for (int i = 0; i < 31; i++)
		for (int j = 0; j < 31; j++)
			if (i < j) {
				uint32_t a = mask_32(i, j);
				uint32_t b = MASK_32(i, j);
				if (a != b)
					printf("(%d, %d): %#x, %#x\n", i, j, a, b);
			}

	return 0;
}
