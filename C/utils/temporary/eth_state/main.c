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
#include <linux/ethtool.h>


/*
 * -1, error
 * 1, link up
 * 0, link down
 */
int get_netlink_status(const char *ifname)
{
	int fd;
	struct ifreq ifr;
	struct ethtool_value data;
	data.cmd = ETHTOOL_GLINK;
	data.data = 0;
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
	ifr.ifr_data = (char *)&data;
	if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
		perror("open socket failed");
		return -1;
	}

	if (ioctl(fd, SIOCETHTOOL, &ifr) == -1) {
		perror("ioctl failed");
		close(fd);
		return -1;
	}
	close(fd);
	return data.data;
}

int enable_eth(const char *iface, int up)
{
	int sockfd;
	struct ifreq ifr;
	int ret = -1;

	if (iface == NULL) 
		return -1;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return sockfd;
	strncpy(ifr.ifr_name, iface, 16);
	if(ret = ioctl(sockfd, SIOCGIFFLAGS, (caddr_t)&ifr))
		goto _end;
	if(up) {
		ifr.ifr_flags |= IFF_UP;
	} else {
		ifr.ifr_flags &= (~IFF_UP);
	}
	ret = ioctl(sockfd, SIOCSIFFLAGS, (caddr_t)&ifr);

_end:
	close(sockfd);

	return ret;
}

int main(int argc, char **argv)
{
	int ret;

	if (!strcmp(argv[1], "down")) {
		enable_eth("eth0", 0);
		return 0;
	} else if (!strcmp(argv[1], "up")) {
		enable_eth("eth0", 1);
		return 0;
	}

	ret = get_netlink_status("eth0");
	printf("link status = %d\n", ret);

	return 0;
}

