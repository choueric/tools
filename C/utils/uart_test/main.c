#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

static int config_uart(int fd, unsigned int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;
    if (tcgetattr(fd,&oldtio) != 0) {
    	printf("tcgetattr error \n");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits) {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (nEvent) {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK/* | ISTRIP*/);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK/* | ISTRIP*/);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed) {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 19200:
        cfsetispeed(&newtio, B19200);
        cfsetospeed(&newtio, B19200);
        break;
    case 38400:
        cfsetispeed(&newtio, B38400);
        cfsetospeed(&newtio, B38400);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 230400:
        cfsetispeed(&newtio, B230400);
        cfsetospeed(&newtio, B230400);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }

    if (nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if (nStop == 2)
        newtio.c_cflag |= CSTOPB;

    newtio.c_cc[VTIME] = 5;
    newtio.c_cc[VMIN] = 0;
    tcflush (fd,TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
    	printf("com set error.\n");
        return -1;
    }
    return 0;
}

static int set_flag(int fd, int flags)
{
    int val=-1;
    if ((val=fcntl(fd, F_GETFL, 0))>=0) {
		val |= flags;
		val = fcntl(fd, F_SETFL, val);
	}
	return (val>=0)?1:0;
}

static int clr_flag(int fd, int flags)
{
    int val=-1;
    if ((val=fcntl(fd, F_GETFL, 0))>=0) {
    	val &= ~flags;
   		val =fcntl(fd, F_SETFL, val);
    }
    return (val>=0)?1:0;
}

unsigned int open_uart(const char *ttyS, unsigned int baud)
{
	unsigned int fd;
	fd = open(ttyS, O_RDWR);
	if(!fd)
		printf("%s  open Failed!\n", ttyS);
	else
		printf("%s open Success! set baudrate:%d\n", ttyS,baud);
	if(config_uart(fd, baud, 8, 'E', 1) < 0)
		printf("%s set failed!\n", ttyS);
	return fd;
}


int main(int argc, char **argv)
{
	unsigned int fd;
	char *dev = "/dev/ttyS0";
	int bdrate = 115200;
	char send_buf[] = "test\n";
	int ret;

	if (argc >= 2) {
		dev = argv[1];
	}

	printf("===== %s =======\n", dev);
	printf("bdrate: %d\n",  bdrate);

	fd = open_uart(dev, bdrate);
	if (fd < 0) {
		printf("open uart error: %d\n", fd);
		return -1;
	}

	while (1) {
		ret = write(fd, send_buf, sizeof(send_buf));
		if (ret < 0) {
			printf("write error: %d\n", ret);
			break;
		}
		usleep(1000*800);
	}

	close(fd);
	printf("\n test success\n");

	return 0;
}
