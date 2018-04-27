#include "bitop.h"

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
