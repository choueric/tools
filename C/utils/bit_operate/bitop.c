#include <stdio.h>

#define BIT(n) (1 << n)

/* create mask from bit offset @start, bit len is @len
 * e.g. MASK(1, 2) = b00000110
 */
int MASK(int start, int len)
{
    int v = 0;
    int i = 0;

    for (i = start; i < start + len; i++)
        v |= BIT(i);

    return v;
}

/*
 * fetch the value of one field in a register.
 *
 * e.g.: register A is 8bit, and bit[2-3] means control mode,
 * 0: mode A, 1: mode B, 2: mode C, 3: mode D.
 * this function get the mode value from value of register A
 *   mode = fieldValue(regVal, BIT(2) | BIT(3))
 */
int fieldValue(int val, int mask)
{
	int i = 0;
	int v = mask;

	while ((v & 0x01) == 0) {
		v = v >> 1;
		i++;
	}

	return (val & mask) >> i;
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
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


    return 0;
}

