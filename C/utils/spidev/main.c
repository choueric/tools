#include <stdio.h>
#include "bus_spidev.h"

void dump_mem(FILE *fp, const unsigned char *buf, int len, const char *desc)
{
	int i = 0;
	if (desc)
		fprintf(fp, "%s:\n", desc);

	fprintf(fp, "--------------------------------------------------------\n");
	for (i = 0; i < len; i++) {
		fprintf(fp, "0x%02x ", (unsigned char)buf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(fp, "\n");
	}

	fprintf(fp, "\n--------------------------------------------------------\n");
}

int gv7600_read_reg(int fd, uint16_t offset)
{
     uint8_t txbuf[2] = {0x90, 0x00};
     uint8_t rxbuf[4] = {0x10, 0x01, 0x55, 0xaa};

     txbuf[0] = txbuf[0] | ((offset & 0x0fff) >> 8);
     txbuf[1] = (offset & 0x00ff);

     spidev_write_then_read(fd, txbuf, 2, rxbuf, 4);

     printf("%02x %02x %02x %02x\n", rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3]);

     return 0;
}

int main()
{
    int fd = spidev_open("/dev/spidev.0", 0);

    gv7600_read_reg(fd, 0);

    spidev_close(fd);

    return 0;
}
