#include <stdio.h>

#include "put_bits.h"

void test_1()
{
	uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
	BitStream s;

	init_bit_stream(&s, data, 4);

	print_stream(&s);
}

void test_2()
{
	uint8_t data[4] = {0x00};
	BitStream s;

	init_bit_stream(&s, data, 4);

	put_bits(&s, 1, 1);
	put_bits(&s, 2, 2);
	put_bits(&s, 3, 7);
	put_bits(&s, 2, 3);

	put_bits(&s, 3, 5);
	put_bits(&s, 2, 2);
	skip_put_bits(&s, 2);
	put_bits(&s, 5, 25);

	printf("used %d bits\n", put_bits_count(&s));

	flush_put_bits(&s);

	print_stream(&s);

}


int main()
{
	test_2();

	return 0;
}

