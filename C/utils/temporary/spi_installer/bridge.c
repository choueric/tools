#include <stdio.h>
#include "bridge.h"
#include "crc.h"
#include "flash.h"
#include "spidev.h"

/*
 * communitcate with spi FPGA bridge in BigDipper
 */

#define RTCMD 0x10  // read transmit depth
#define RRCMD 0x20  // read recive depth
#define WRCMD 0x30  // write data
#define RDCMD 0x40  // recive data
#define RX_DEP_LEN 3
#define TX_DEP_LEN 9
#define L1_ROUTE_SIZE 6
#define VIR_DEV_REPLY_SIZE 4

#define L1_ROUTE_HEAD 0xfb

#define UART1_PORT 0x01
#define UART2_PORT 0x02
#define UART3_PORT 0x03
#define UART4_PORT 0x13
#define VIR_DEV_PORT 0x81

static unsigned char *find_header(unsigned char *buf, int len, unsigned char header)
{
	unsigned char *p = NULL;
    unsigned char crc = 0;

    for (p = buf; len > L1_ROUTE_SIZE; len--, p++) {
        if (p[0] != header) {
            continue;
        }
        crc = p[L1_ROUTE_SIZE - 1];
        if (getCRC8(p, L1_ROUTE_SIZE - 1) == crc)
            return p;
    }

	return NULL;
}

short read_rx_dep(int fd)
{
	int ret = 0;
	short depth = -1;
	unsigned char cmd = RRCMD;
	unsigned char buf[RX_DEP_LEN] = {0};
	unsigned char try_count = 2;

	while (try_count) {
		ret = spi_write_then_read(fd, &cmd, 1, buf, RX_DEP_LEN);
		if ((ret < 0) || (getCRC8(buf, RX_DEP_LEN - 1) != buf[RX_DEP_LEN - 1])) {
			try_count--;
			printf("Read rx depth failed [%d].\n", try_count);
		} else {
			break;
		}
	}

	if (!try_count) {
        printf("Read rx depth failed [%d].\n", try_count);
		return -1;
	}

	depth = (short)((buf[0] << 8) + buf[1]);

	return depth;
}

short read_tx_dep(int fd)
{
    int ret = 0;
    short depth = -1;
    unsigned char cmd = RTCMD;
    unsigned char buf[TX_DEP_LEN] = {0};
    unsigned char try_count = 2;

    while (try_count) {
        ret = spi_write_then_read(fd, &cmd, 1, buf, TX_DEP_LEN);
		if ((ret < 0) || (getCRC8(buf, TX_DEP_LEN - 1) != buf[TX_DEP_LEN - 1])) {
			try_count--;
			printf("Read tx depth failed [%d].\n", try_count);
		} else {
			break;
		}
	}

	if (!try_count) {
        printf("Read tx depth failed [%d].\n", try_count);
		return -1;
	}

	depth = 4;

	return depth;
}

int send_data(int fd, unsigned char *buf, int len)
{
    int ret = 0;
    unsigned char cmd = WRCMD;

    ret = spi_write_twice(fd, &cmd, 1, buf, len);

	if (ret < 0) {
		printf("Send data failed.\n");
		return -1;
	}

	return 0;
}


int recv_data(int fd, unsigned char *buf, int len)
{
	int ret = 0;
	unsigned char cmd = RDCMD;

	ret = spi_write_then_read(fd, &cmd, 1, buf, len);

	if (ret < 0) {
		printf("Read data failed.\n");
		return -1;
	}

	return 0;
}


unsigned char *find_vir_dev_reply(unsigned char *buf, int data_size)
{
	int len = 0;
	unsigned char *head = NULL;
    unsigned char *p = buf;
    int left = data_size;

	do {
		p = find_header(p, left, L1_ROUTE_HEAD);
		if (p == NULL) {
            printf("can't find header\n");
            return NULL;
		}

        head = p;
		left -= L1_ROUTE_SIZE;
        p += L1_ROUTE_SIZE;

		len = (int)((head[L1_ROUTE_SIZE - 3] << 8) + head[L1_ROUTE_SIZE - 2]);
        len = left < len ? left: len;

        if (head[1] == VIR_DEV_PORT) {
            if (len != VIR_DEV_REPLY_SIZE)
                printf("len should be %d ,and actualis %d\n", VIR_DEV_REPLY_SIZE, len);
            return p;
        }
        left -= len;
        p += len;
	} while(1);

	return NULL;
}
