#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "bitop.h"
#include "print.h"

#define test_equal(a, b) do { \
	int aa = (a), bb = (b); \
	if (aa != bb) { \
		printf("%d: %#x != %#x\n", __LINE__, aa, bb);\
		exit(1); \
	} \
} while (0)

static void test_bit_mask()
{
	for (int i = 0; i < 8; i++)
		for (int l = 1; (i+l) < 8; l++)
			test_equal(MASK(i, l), BIT_MASK(i, i + l-1));
}

static void test_ffs()
{
	char str_buf[128] = {0};
	uint8_t s = BIT(3) | BIT(6) | BIT(0) | BIT(7);
	int results[4] = {1, 4, 7, 8};

	for (int i = 0; i < 4; i++) {
		int ret = ffs(s);
		test_equal(results[i], ret);
		printf("%02x(%s) -> %d\n", s, bin_str(&s, 1, str_buf, 128), ret);
		s &= ~BIT((ret - 1));
	}
}

int main()
{
	printf("***** build time %s %s %d %s *****\n", __TIME__,__DATE__,__GNUC__,__VERSION__);

    test_equal(fieldValue(0xf0, BIT(0)), 0);
    test_equal(fieldValue(0xf0, BIT(1)), 0);
    test_equal(fieldValue(0xf0, BIT(2)), 0);
    test_equal(fieldValue(0xf0, BIT(3)), 0);
    test_equal(fieldValue(0xf0, BIT(4)), 1);
    test_equal(fieldValue(0xf0, BIT(5)), 1);
    test_equal(fieldValue(0xf0, BIT(6)), 1);
    test_equal(fieldValue(0xf0, BIT(7)), 1);

    test_equal(fieldValue(0xf0, BIT(2) | BIT(3)), 0);
    test_equal(fieldValue(0xf0, BIT(3) | BIT(4)), 2);
    test_equal(fieldValue(0xf0, BIT(4) | BIT(5)), 3);
    test_equal(fieldValue(0xf0, BIT(6) | BIT(7)), 3);

    test_equal(MASK(3, 3), 0x38);
    test_equal(MASK(3, 4), 0x78);
    test_equal(MASK(4, 4), 0xf0);

    test_equal(fieldValue(0xf0, MASK(2, 2)), 0);
    test_equal(fieldValue(0xf0, MASK(3, 2)), 2);
    test_equal(fieldValue(0xf0, MASK(4, 2)), 3);
    test_equal(fieldValue(0xf0, MASK(6, 2)), 3);
    test_equal(fieldValue(0xf0, MASK(3, 3)), 6);
    test_equal(fieldValue(0xf0, MASK(3, 4)), 0xe);
    test_equal(fieldValue(0xf0, MASK(4, 4)), 0xf);

	test_bit_mask();

	test_ffs();

    return 0;
}
