#include <stdio.h>


#include "print_time.h"


int test()
{
	printf("---- time date ----\n");
	struct tm tt;
    char str[TIME_LEN];
	fetch_time(&tt);
    time2str(&tt, str, TIME_LEN);
    printf("[%s]\n", str);
	return 0;
}

int main(int argc, char **argv)
{
	test();
	printf("--- print timeofday ----\n");
	print_timeofday();

	return 0;
}
