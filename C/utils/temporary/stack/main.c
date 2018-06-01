#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <errno.h>
#include <poll.h>
#include <linux/soundcard.h>
#include <linux/input.h>
#include <linux/spi/spidev.h>
#include <termios.h>
#include <linux/serial.h>
#include <linux/i2c.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/uio.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <linux/tcp.h>
#include <linux/ethtool.h>
#include <signal.h>
#include <bits/signum.h>
#include <execinfo.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <stdio.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>

int print_stack(int no)
{
	int a = 0xaabb;
	void *p = &no;
	void *sp;
	void *fp;

	__asm("mov %0,ip": "=r"(sp));
	printf("ip\t[%p]\t%p\n", &sp, sp);
	__asm("mov %0,r11": "=r"(fp));
	__asm("mov %0,sp": "=r"(sp));
	printf("sp\t[%p]\t%p\n", &sp, sp);
	printf("fp\t[%p]\t%p\n", &fp, fp);

#if 0
	printf("PC\t[%p]\t%p\n", (void *)((int)p + 16), (void *)(*((int *)((int)p + 16))));
	printf("LR\t[%p]\t%p\n", (void *)((int)p + 12), (void *)(*((int *)((int)p + 12))));
	printf("sp\t[%p]\t%p\n", (void *)((int)p + 8), (void *)(*((int *)((int)p + 8))));
	printf("fp\t[%p]\t%p\n", (void *)((int)p + 4), (void *)(*((int *)((int)p + 4))));
	printf("no\t[%p]\t%p\n", &no, (void *)no);
	printf("a\t[%p]\t%p\n", &a, (void *)a);
	printf("p\t[%p]\t%p\n", &p, p);
#endif

	if (no != 0)
		print_stack(--no);
	else {
		for (p = fp + 24 * 6; p >= sp; p -= 4) {
			if (p == &p)
				printf("[%p]\t- p addr -\n", &p);
			else
				printf("[%p]\t%p\n", p, (void *)(*(int *)p));
		}
		printf("-----------------------------\n");
	}

	a = 0;

	return 0;
}

int main(int argc, char **argv)
{
	int a = 0xccddeeff;
	print_stack(0x3);

	a = 0;

    return 0;
}
