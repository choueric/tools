#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "bitop.h"

static void test_equal(uint8_t a, uint8_t b)
{
	if (a != b) {
		printf("%#x != %#x\n", a, b);
		exit(1);
	}
}

static void test_bit_mask()
{
	for (int i = 0; i < 8; i++)
		for (int l = 1; (i+l) < 8; l++)
			test_equal(MASK(i, l), BIT_MASK(i, i + l-1));
}

int main()
{
	int ret; 
	unsigned char s = BIT(3) | BIT(6) | BIT(0) | BIT(7);

    printf("***** build time %s %s %d %s\n", __TIME__,__DATE__,__GNUC__,__VERSION__);

    printf("ret = 0xf0 & bit(0) = %x\n", fieldValue(0xf0, BIT(0)));
    printf("ret = 0xf0 & bit(1) = %x\n", fieldValue(0xf0, BIT(1)));
    printf("ret = 0xf0 & bit(2) = %x\n", fieldValue(0xf0, BIT(2)));
    printf("ret = 0xf0 & bit(3) = %x\n", fieldValue(0xf0, BIT(3)));
    printf("ret = 0xf0 & bit(4) = %x\n", fieldValue(0xf0, BIT(4)));
    printf("ret = 0xf0 & bit(5) = %x\n", fieldValue(0xf0, BIT(5)));
    printf("ret = 0xf0 & bit(6) = %x\n", fieldValue(0xf0, BIT(6)));
    printf("ret = 0xf0 & bit(7) = %x\n", fieldValue(0xf0, BIT(7)));

    printf("ret = 0xf0 & bit(2|3) = %x\n", fieldValue(0xf0, BIT(2) | BIT(3)));
    printf("ret = 0xf0 & bit(3|4) = %x\n", fieldValue(0xf0, BIT(3) | BIT(4)));
    printf("ret = 0xf0 & bit(4|5) = %x\n", fieldValue(0xf0, BIT(4) | BIT(5)));
    printf("ret = 0xf0 & bit(6|7) = %x\n", fieldValue(0xf0, BIT(6) | BIT(7)));

    printf("mask(3, 3) = 0x%02x\n", MASK(3, 3));
    printf("mask(3, 4) = 0x%02x\n", MASK(3, 4));
    printf("mask(4, 4) = 0x%02x\n", MASK(4, 4));

    printf("ret = 0xf0 & mask(2, 2) = %x\n", fieldValue(0xf0, MASK(2, 2)));
    printf("ret = 0xf0 & mask(3, 2) = %x\n", fieldValue(0xf0, MASK(3, 2)));
    printf("ret = 0xf0 & mask(4, 2) = %x\n", fieldValue(0xf0, MASK(4, 2)));
    printf("ret = 0xf0 & mask(6, 2) = %x\n", fieldValue(0xf0, MASK(6, 2)));
    printf("ret = 0xf0 & mask(3, 3) = %x\n", fieldValue(0xf0, MASK(3, 3)));
    printf("ret = 0xf0 & mask(3, 4) = %x\n", fieldValue(0xf0, MASK(3, 4)));
    printf("ret = 0xf0 & mask(4, 4) = %x\n", fieldValue(0xf0, MASK(4, 4)));

	test_bit_mask();

	while (s) {
		ret = ffs(s);
		printf("%02x -> %d\n", s, ret);
		s &= ~BIT((ret - 1));
	}

    return 0;
}
