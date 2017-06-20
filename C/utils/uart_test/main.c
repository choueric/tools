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
#include <pthread.h>

#include "options.c"

#define SLEEP_TIME 1  // second

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

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;
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

unsigned int open_uart(const char *dev)
{
	unsigned int fd;

	fd = open(dev, O_RDWR | O_NOCTTY);
	if (fd < 0)
		printf("%s  open Failed!\n", dev);
	else
		printf("%s open Success!\n", dev);

	return fd;
}

static int test_write(int fd, char *buf, int size)
{
	int ret = 0;

	while (1) {
		printf("--> write %s\n", buf);
		ret = write(fd, buf, size);
		if (ret < 0 || ret != size) {
			printf("write error: %d\n", ret);
			break;
		}
		sleep(SLEEP_TIME);
	}

	return ret;
}


static void *read_thread(void *data)
{
	int ret = 0;
	char *recv = NULL;
	int size = 128;
	int fd = (int)data;

	recv = malloc(size * sizeof(char));
	if (recv == NULL) {
		printf("mallc failed\n");
		return NULL;
	}

	while (1) {
		ret = read(fd, recv, size);
		if (ret < 0) {
			printf("read error: %d (%s)\n", ret, strerror(errno));
			break;
		}
		if (ret == 0) {
			printf("no read %d\n", ret);
		} else {
			printf("--> recv: %s", recv);
		}
		sleep(SLEEP_TIME);
	}

	return NULL;
}

static int test_loop(int fd, char *buf, int size)
{
	int ret = 0;
	pthread_t pid;

	pthread_create(&pid, NULL, read_thread, fd);

	while (1) {
		printf("--> write %s\n", buf);
		ret = write(fd, buf, size);
		if (ret < 0 || ret != size) {
			printf("write error: %d\n", ret);
			break;
		}

		sleep(SLEEP_TIME);
	}

	return ret;
}

int main(int argc, char **argv)
{
	unsigned int fd;
	int bdrate = 115200;
	char send_buf[] = "test\n";
	int ret;

	parse_args(argc, argv);
	print_options();

	printf("bdrate: %d\n",  bdrate);

	fd = open_uart(device);
	if (fd < 0) {
		printf("open uart error: %d\n", fd);
		return -1;
	}

	if (config_uart(fd, bdrate, 8, 'E', 1) < 0) {
		printf("config uart failed!\n");
		return -1;
	}

	if (loop) {
		ret = test_loop(fd, send_buf, sizeof(send_buf));
	} else {
		ret = test_write(fd, send_buf, sizeof(send_buf));
	}

	close(fd);
	printf("\n test success\n");

	return ret;
}
