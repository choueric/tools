#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define STR_LEN 10

int main(int argc, char *argv[])
{
    char str1[STR_LEN] = "test";
    char *str2 = "testtesttest";

    size_t ret = strnlen(str1, STR_LEN);
    printf("strnlen(%s, 10) is %zu\n", str1, ret);

    ret = strnlen(str2, STR_LEN);
    printf("strnlen(%s, 10) is %zu\n", str2, ret);

    str2 = NULL;
    ret = strnlen(str2, STR_LEN);
    printf("strnlen(%s, 10) is %zu\n", str2, ret);

    return 0;
}
