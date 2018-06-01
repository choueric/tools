#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;

    for (i = 0; i < 100; i++) {
        printf("%d%%\r", i);
        fflush(stdout);
        usleep(50 * 1000);
    }
    printf("%d%%\r", i);
    printf("\n");

	return 0;
}
