#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "block_read.h"

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyS0"

int main(int argc, char **argv)
{
	int fd, ret;
	size_t size;
	struct termios oldtio,newtio;
	char buf[32] = {0};

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		perror(MODEMDEVICE);
		exit(-1);
	}

	tcgetattr(fd, &oldtio); /* save current port settings */

	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	size = 32;
	ret = block_read(fd, buf, &size, 5*1000);
	printf("ret = %d, size = %ld\n", ret, size);
	if (ret >= 0)
		printf("[%s]\n", buf);

	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);

	return 0;
}
