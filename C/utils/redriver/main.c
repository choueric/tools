#include "bus_i2cdev.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MODE_INVALID 0
#define MODE_READ 1
#define MODE_WRITE 2

#define I2CDEV_PATH "/dev/i2c-1"
#define I2CDEV_ADDR 0x77
#define BUF_SIZE 16

#define IDX_A0 3
#define IDX_A1 4
#define IDX_A2 5
#define IDX_A3 6
#define IDX_B0 7
#define IDX_B1 8
#define IDX_B2 9
#define IDX_B3 10

static void print_register(int fd)
{
	int i = 0;
	uint8_t buf[BUF_SIZE];
	int ret = 0;

	printf("== registers ==\n");

	ret = i2cdev_read(fd, buf, BUF_SIZE, I2CDEV_ADDR);
	if (ret) {
		printf("i2c read failed: %s\n", strerror(errno));
		return;
	}

	for (i = 0; i < BUF_SIZE; i++) {
		printf("%02d: 0x%02x\n", i, buf[i]);
	}
	printf("=============\n");
}

static int write_register(int fd, uint8_t index, uint8_t data)
{
	uint8_t buf[2];
	int ret = 0;

	buf[0] = index;
	buf[1] = data;

	ret = i2cdev_write(fd, buf, 2, I2CDEV_ADDR);
	if (ret) {
		printf("i2c write failed: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}

static void print_usage()
{
		printf("Usage: ./rederiver <EQ> <FG> <SW> or ./redriver read.\n");
		printf("   EQ: 0-15\n");
		printf("   FG: 0-3\n");
		printf("   SW: 0-1\n");
}

int main(int argc, char **argv)
{
	int fd = -1;
	int idx = 0;
	int eq, fg, sw;
	uint8_t data;
	int mode = MODE_INVALID;

	printf("===== PI3EQX8908A i2c configuration =====\n");

	if (argc == 2 && !strcmp(argv[1], "read")) {
		mode = MODE_READ;
		goto START;
	}

	if (argc != 4) {
		print_usage();
		return -2;
	}
	mode = MODE_WRITE;

	eq = atoi(argv[1]);
	fg = atoi(argv[2]);
	sw = atoi(argv[3]);
	if (eq < 0 || eq > 15 || fg < 0 || fg > 3 || sw < 0 || sw > 1) {
		print_usage();
		return -3;
	}
	data = (uint8_t)((eq << 4) | (fg << 2) | sw);

	printf("EQ: %d\n", eq);
	printf("FG: %d\n", fg);
	printf("SW: %d\n", sw);
	printf("data = 0x%02x\n\n", data);

START:
	fd = i2cdev_open(I2CDEV_PATH, O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("i2cdev_open failed: %s\n", strerror(errno));
		return -1;
	}

	print_register(fd);

	if (mode == MODE_WRITE) {
		for (idx = IDX_A0; idx <= IDX_B3; idx++)
			write_register(fd, idx, data);
		print_register(fd);
	}

	i2cdev_close(fd);

	return 0;
}
