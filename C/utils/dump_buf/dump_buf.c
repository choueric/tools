#include <stdio.h>
#include <stdlib.h>


void dump_buf(const unsigned char *buf, int len)
{
    int i = 0;

	printf("--------------------------------");
	for (i = 0; i < len; i++) {
		if ((i & 0xf) == 0)
			printf("\n");
		printf("%.2x ", buf[i]);
	}
	printf("\n--------------------------------\n");
}


int main(void)
{
	int count = 134;
	unsigned char *buf = malloc(count);
	int i;

	for (i = 0; i < count; i++)
		buf[i] = i;

	dump_buf(buf, count);

	free(buf);

	return 0;
}
