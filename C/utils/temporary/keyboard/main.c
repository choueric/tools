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

struct kbd_data {
	unsigned int scan_code;
	unsigned int value;
	unsigned int down_flag;
	unsigned int led_value;
};

int main(int argc, char **argv)
{
    int fd = -1;
    int ret = 0;
	int flags = 0;
	struct kbd_data key;

	flags = O_RDONLY;
	if (argc == 2 && !strcmp(argv[1], "nonblock")) {
		printf("set to nonblock\n");
		flags |= O_NONBLOCK;
	} else
		printf("set to block\n");

    if ( (fd = open("/dev/kbd", flags)) < -1) {
        perror("open failed");
        return 1;
    }

	while (1) {
		usleep(40000);
		ret = read(fd, &key, sizeof(key));
		if (ret < 0) {
			//perror("read failed");
			continue;
		}
		printf("ret = %d\n", ret);
		printf("scancode = %d, value = %d, down_flag = %d, led_value = %d\n",
				key.scan_code, key.value, key.down_flag, key.led_value);
	}

	close(fd);

    return 0;
}
