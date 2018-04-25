#include <stdio.h>
#include <stdlib.h>
#include <linux/version.h>

int main(int argc __attribute__((unused)),
         char** argv __attribute__((unused)))
{
    printf("LINUX_VERSION_CODE = %x\n", LINUX_VERSION_CODE);
    return 0;
}
