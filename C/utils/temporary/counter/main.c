#include <stdio.h>

#include "counter.h"

int main(int argc, char **argv)
{
    struct counter *c = NULL;
    int i;
    int n;

    c = alloc_counter(10, 3);
    for (i = 0; i < 1001; i++) {
        //printf("--------------------\n");
        //print_counter(c, stdout);
        n = inc_counter(c);
        if (n == 1)
            printf("++++\n");
        print_counter(c, stdout);
    }

    free_counter(c);

	return 0;
}
