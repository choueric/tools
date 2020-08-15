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

int main(int argc, char *argv[])
{
    struct stat st;
    int mjr, mnr;
    char *dev = "/dev/tty0";

    if (stat(dev, &st)) {
        printf("stat failed: %s\n", strerror(errno));
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        printf("not char\n");
        return -2;
    }

    printf("%lx\n", st.st_rdev);
    mjr = major(st.st_rdev);
    mnr = minor(st.st_rdev);
    printf("%s: %d:%d\n", dev, mjr, mnr);

    return 0;
}
