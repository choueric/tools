#include "bus_i2cdev.h"
#include <unistd.h>

static int read_reg(int fd)
{
	uint8_t reg = 0x10;
	uint8_t data = 0x00;
	int ret = i2cdev_write_then_read(fd, &reg, 1, 0x32, &data, 1, 0x32);
	if (ret != 0) {
		printf("i2cdev read test failed\n");
	} else {
		printf("i2cdev read OK.\n");
		printf("data = 0x%02x\n", data);
	}

	return 0;
}

static int write_reg(int fd)
{
	uint8_t reg = 0x10;
	uint8_t buf[2];
	buf[0] = reg;
	buf[1] = 0x10;

	int ret = i2cdev_write(fd, buf, 2, 0x32);
	if (ret != 0) {
		printf("i2cdev write failed\n");
	}

	printf("i2cdev write OK.\n");
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("need i2c dev\n");
		return 1;
	}

	int fd = i2cdev_open(argv[1], O_RDWR);
	if (fd <= 0) {
		printf("open failed\n");
		return 2;
	}

	write_reg(fd);
	read_reg(fd);

	i2cdev_close(fd);

	return 0;
}
