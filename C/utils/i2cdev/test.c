#include "bus_i2cdev.h"
#include <unistd.h>

#define TEST_TMDS181
//#define TEST_RX8010

#ifdef TEST_RX8010
#define ADDR_RX8010 0x32
#define REG_RX8010 0x10
#endif

#ifdef TEST_TMDS181
#define ADDR_TMDS181_TX (0xbc/2)
#define ADDR_TMDS181_RX (0xb8/2)
#define REG_TMDS181 0x00
#endif

int read_reg(int fd, uint16_t addr, uint8_t reg)
{
	uint8_t data = 0;
	int ret = i2cdev_write_then_read(fd, &reg, 1, addr, &data, 1, addr);
	if (ret != 0) {
		printf("i2cdev read test failed\n");
	} else {
		printf("i2cdev read [0x%02x:0x%02x] OK.\n", addr, reg);
		printf("read data = 0x%02x[%c]\n", data, data);
	}

	return 0;
}

int write_reg(int fd, uint16_t addr, uint8_t reg, uint8_t data)
{
	uint8_t buf[2];
	buf[0] = reg;
	buf[1] = data;

	int ret = i2cdev_write(fd, buf, 2, addr);
	if (ret != 0) {
		printf("i2cdev write failed\n");
	} else {
		printf("i2cdev write [0x%02x:0x%02x] OK.\n", addr, reg);
	}

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

#ifdef TEST_RX8010
	write_reg(fd, ADDR_RX8010, REG_RX8010, 0x00);
	read_reg(fd, ADDR_RX8010, REG_RX8010);
#endif

#ifdef TEST_TMDS181
	printf("====== TMDS181 TX ==========\n");
	for (uint8_t i = 0; i < 9; i++)
		read_reg(fd, ADDR_TMDS181_TX, i);

	printf("====== TMDS181 RX ==========\n");
	for (uint8_t i = 0; i < 9; i++)
		read_reg(fd, ADDR_TMDS181_RX, i);
#endif

	i2cdev_close(fd);

	return 0;
}
