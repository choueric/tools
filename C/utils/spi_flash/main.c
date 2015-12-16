#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "ini.h"
#include "flash.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

struct spi_cfg {
	char *device;
	uint8_t mode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
};

struct spi_cfg cfg_info = {
	.device = "/dev/spidev1.0",
	.mode = 0,
	.bits = 8,
	.speed = 12000000,
	.delay = 0,
};

struct cmd_entry;
struct cmd_args;

typedef int (*entry_func) (struct cmd_args *, struct cmd_entry *);

struct cmd_args {
	int fd;
	int argc;
	char **argv;
};

struct cmd_entry {
	char *cmd;
	entry_func func;
};

/**********************************************************/

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void print_buf(uint8_t *buf, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		if (i % 16 == 0)
			printf("\n");
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

static void print_cfg(struct spi_cfg *p)
{
	printf("-------------------------------------\n");
	printf("device: %s\n", p->device);
	printf("spi mode: %d\n", p->mode);
	printf("bits per word: %d\n", p->bits);
	printf("max speed: %d Hz (%d KHz)\n", p->speed, p->speed/1000);
	printf("delay = %d\n", p->delay);
	printf("-------------------------------------\n");
}

static int parse_func(char *key, char *val, void *arg)
{
	struct spi_cfg *p = (struct spi_cfg *)arg;

	if (!strcmp(key, "device")) {
		p->device = strdup(val);
	} else if (!strcmp(key, "mode")) {
		p->mode = strtol(val, NULL, 0);
	} else if (!strcmp(key, "bits")) {
		p->bits = strtol(val, NULL, 0);
	} else if (!strcmp(key, "speed")) {
		p->speed = strtol(val, NULL, 0);
	} else if (!strcmp(key, "delay")) {
		p->delay = strtol(val, NULL, 0);
	} else {
		printf("invalid key[%s]\n", key);
	}
		
	return 0;
}

/*----------------------------------------------------------------------*/

static int cmd_read_sign(struct cmd_args *args, struct cmd_entry *entry)
{
	uint8_t val = 0;

	val = flash_read_sign(args->fd);
	printf("%s: sign = %02x\n", entry->cmd, val);

	return 0;
}

static int cmd_read_jedec(struct cmd_args *args, struct cmd_entry *entry)
{
	int id = 0;

	id = flash_read_jedec(args->fd);
	printf("%s: jedec id = %x\n", entry->cmd, id);

	return 0;
}

static int cmd_read_status(struct cmd_args *args, struct cmd_entry *entry)
{
	uint8_t val = 0;

	val = flash_read_status(args->fd);
	printf("%s: status = %02x\n", entry->cmd, val);

	return 0;
}

static int cmd_write(struct cmd_args *args, struct cmd_entry *entry)
{
	uint8_t *buf = NULL;
	int addr = 0;
	size_t size = 0;
	uint8_t data = 0x00;

	if (args->argc != 3) {
		printf("usage: %s <addr> <data> <size>\n", entry->cmd);
		return -1;
	}
	addr = strtol(args->argv[0], NULL, 0);
	data = strtol(args->argv[1], NULL, 0);
	size = strtol(args->argv[2], NULL, 0);

	printf("%s %x %x %d\n", entry->cmd, addr, data, size);

	buf = malloc(size);
	if (buf == NULL)
		pabort("failed malloc");

	memset(buf, data, size);
	flash_page_program(args->fd, addr, buf, size);

	free(buf);

	return 0;
}

static int cmd_sector_erase(struct cmd_args *args, struct cmd_entry *entry)
{
	int addr;

	if (args->argc != 1) {
		printf("usage: %s <addr>\n", entry->cmd);
		return -1;
	}
	addr = strtol(args->argv[0], NULL, 0);

	printf("%s: addr %x\n", entry->cmd, addr);
	flash_sector_erase(args->fd, addr);

	return 0;
}

static int cmd_bulk_erase(struct cmd_args *args, struct cmd_entry *entry)
{
	printf("%s: erase\n", entry->cmd);
	flash_bulk_erase(args->fd);

	return 0;
}

static int cmd_read_data(struct cmd_args *args, struct cmd_entry *entry)
{
	int addr;
	size_t size;
	uint8_t *buf = NULL;

	if (args->argc != 2) {
		printf("usage: %s <addr> <size>\n", entry->cmd);
		return -1;
	}
	addr = strtol(args->argv[0], NULL, 0);
	size = strtol(args->argv[1], NULL, 0);

	buf = malloc(size);
	if (buf == NULL)
		pabort("failed malloc");
	memset(buf, 0x00, size);

	flash_read_data(args->fd, addr, buf, size);
	printf("%s: %x %d\n", entry->cmd, addr, size);
	print_buf(buf, size);

	free(buf);
	return 0;
}

static int cmd_fast_read(struct cmd_args *args, struct cmd_entry *entry)
{
	int addr;
	size_t size;
	uint8_t *buf = NULL;

	if (args->argc != 2) {
		printf("usage: %s <addr> <size>\n", entry->cmd);
		return -1;
	}
	addr = strtol(args->argv[0], NULL, 0);
	size = strtol(args->argv[1], NULL, 0);

	buf = malloc(size);
	if (buf == NULL)
		pabort("failed malloc");
	memset(buf, 0x00, size);

	flash_fast_read(args->fd, addr, buf, size);
	printf("%s: %x %d\n", entry->cmd, addr, size);
	print_buf(buf, size);


	free(buf);
	return 0;
}

static int cmd_init_write(struct cmd_args *args, struct cmd_entry *entry)
{
	return flash_w25q16_init_write(args->fd);
}

struct part {
	int addr;
	int size;
	int is_read;
};

static uint8_t test_read_buf[1024];
static uint8_t test_send_buf[56];

static void time_print(char *str)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	printf("[%ld.%06ld] %s", t.tv_sec, t.tv_usec, str);
}

static int cmd_test(struct cmd_args *args, struct cmd_entry *entry)
{
	struct part parts[] = {
		{.addr = 0x000000, .size = 1024, .is_read = 1,},
		{.addr = 0x000400, .size = 1024, .is_read = 1,},
		{.addr = 0x000800, .size = 1024, .is_read = 1,},
		{.addr = 0x001000, .size = 56, .is_read = 0,},
		{.addr = 0x002000, .size = 56, .is_read = 0,},
		{.addr = 0x003000, .size = 56, .is_read = 0,},
		{.addr = 0x004000, .size = 56, .is_read = 0,},
		{.addr = 0x005000, .size = 56, .is_read = 0,},
		{.addr = 0x006000, .size = 56, .is_read = 0,},
		{.addr = 0x007000, .size = 56, .is_read = 1,},
		{.addr = 0x008000, .size = 56, .is_read = 1,},
		{.addr = 0x009000, .size = 56, .is_read = 1,},
	};
	int n = 0, i;
	struct part *p = NULL;

	time_print("----> start test\n");
	n = ARRAY_SIZE(parts);
	for (i = 0; i < n; i++) {
		p = parts + i;
		if (p->is_read) {
			memset(test_read_buf, 0, p->size);
			flash_read_data(args->fd, p->addr, test_read_buf, p->size);
			print_buf(test_read_buf, p->size);
		} else {
			// TODO: erase before write
			printf("write %x %d\n", p->addr, p->size);
			flash_page_program(args->fd, p->addr, test_send_buf, p->size);
		}
	}
	time_print("<---- end test\n");

	return 0;
}

#if 1
#define TEST_READ_SIZE (1024 *3)
#define TEST_SEND_SIZE 256
static uint8_t stable_test_read[TEST_READ_SIZE];
static uint8_t stable_test_send[TEST_SEND_SIZE];
#define even_data 0x00
#define odd_data 0xaa
#define ADDR 0x00000

#define CH 0
static int check_ops(uint8_t *buf, int size, uint8_t data1, uint8_t data2) {
	int i;
	int err_count = 0;
	int first_err = 0;	
	for (i = 0; i < size; i += 2) {
		if ((data1 != buf[i]) ||  (data2 != buf[i+1])) {
			if (0 == err_count)
				first_err = i;
			err_count++; // error number
		}
	}
	if (err_count)
		printf("total:%d error num:%d\n", size, err_count);

	return ((err_count == 0)? 1:first_err);
}
static int cmd_stable_test(struct cmd_args *args, struct cmd_entry *entry)
{
	struct part parts[] = {
		{.addr = ADDR, .size = TEST_SEND_SIZE, .is_read = 0,},
		{.addr = ADDR, .size = TEST_READ_SIZE, .is_read = 1,},

	};
	struct part *p = NULL;
	int index, i, n = 0;
	int ret = -1;
	int ctrl = 0;

	for (i=0; i < TEST_SEND_SIZE; i += 2) {
		stable_test_send[i] = even_data;
		stable_test_send[i+1] = odd_data;
	}
	n = ARRAY_SIZE(parts);

	do {
//		time_print("=================================\n");
		time_print("----> stable  start\n");
	    ctrl++;

	    for (i = 0; i< n; i++) {
			p = parts+i;
			if (p->is_read) {
				time_print("----> stable  read start\n");
				memset(test_read_buf, 0, p->size);
				flash_read_data(args->fd, p->addr, stable_test_read, p->size);
				
				time_print("<---- stable read end\n");
#if CH
				print_buf(stable_test_read, p->size);
				printf("read %x %d\n", p->addr, p->size);
#endif
				ret = check_ops(stable_test_read, p->size, even_data, odd_data);
				if ((ret % 2) == 0) {
					printf("first error offset:%d addr:0x%x test count:%d\n", ret, p->addr, ctrl);
					return -1;
				}
			} else {
				p->addr = ADDR;	
				flash_sector_erase(args->fd, p->addr);
				
				memset(test_read_buf, 0, p->size);
				flash_read_data(args->fd, p->addr, stable_test_read, TEST_READ_SIZE);
				ret = check_ops(stable_test_read, TEST_READ_SIZE, 0xff, 0xff);
				if ((ret % 2) == 0) {
					printf("first error offset:%d addr:0x%x test count:%d\n", ret, p->addr, ctrl);
					return -1;
				}

				time_print("----> stable  write start\n");
				for (index = 0; index < (TEST_READ_SIZE / TEST_SEND_SIZE); index++) {
#if CH
					print_buf(stable_test_send, p->size);
					printf("write %x %d %d\n", p->addr, p->size, index);
#endif
					flash_page_program(args->fd, p->addr, stable_test_send, p->size);
					p->addr += 256; 
			
				}
				time_print("<---- stable write end\n");
			}
		}
//		time_print("------------end----------------\n");	
#if 0
		time_print("----> stable end\n");
	}while(1);
#else 
	}while(0);
	time_print("----> stable end\n");
	return 0;
#endif
}
#endif

static struct cmd_entry entrys[] = {
	{ "sign", cmd_read_sign, },
	{ "jedec", cmd_read_jedec, },
	{ "status", cmd_read_status, },
	{ "write", cmd_write, },
	{ "serase", cmd_sector_erase, },
	{ "berase", cmd_bulk_erase, },
	{ "read", cmd_read_data, },
	{ "fastread", cmd_fast_read, },
	{ "init_write", cmd_init_write, },
	{ "test", cmd_test, },
	{ "stable_test", cmd_stable_test, },
};

static void print_usage()
{
	int i, n;
	struct cmd_entry *p = NULL;

	n = ARRAY_SIZE(entrys);
	printf("usage: \n");
	for (i = 0; i < n; i++) {
		p = entrys + i;
		printf("    %s\n", p->cmd);
	}
}

static void init_spi_attr(int fd, struct spi_cfg *p)
{
	int ret = 0;

	/* spi mode */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &p->mode);
	if (ret == -1)
		pabort("can't set spi write mode");
	ret = ioctl(fd, SPI_IOC_RD_MODE, &p->mode);
	if (ret == -1)
		pabort("can't get spi read mode");

	 /* bits per word */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &p->bits);
	if (ret == -1)
		pabort("can't set bits per word");
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &p->bits);
	if (ret == -1)
		pabort("can't get bits per word");

	 /* max speed hz */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &p->speed);
	if (ret == -1)
		pabort("can't set max speed hz");
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &p->speed);
	if (ret == -1)
		pabort("can't get max speed hz");
}

int main(int argc, char *argv[])
{
	struct spi_cfg *p = &cfg_info;
	struct cmd_args args;
	struct cmd_entry *entry = NULL;
	char *cmd = NULL;
	int ret = 0;
	int n = 0, i = 0;

	if (argc < 2) {
		print_usage();
		goto L_END;
	}

	/* parse ini file configuration */
	ParseIni("./cfg.ini", parse_func, p);

	args.fd = open(p->device, O_RDWR);
	if (args.fd < 0)
		pabort("can't open device");

	init_spi_attr(args.fd, p);
	print_cfg(p);

	cmd = argv[1];
	n = ARRAY_SIZE(entrys);
	for (i = 0; i < n; i++) {
		entry = entrys + i;
		if (!strcmp(cmd, entry->cmd)) {
			args.argc = argc - 2;
			args.argv = argv + 2;
			ret = entry->func(&args, entry);
			break;
		}
	}
	if (i == n)
		print_usage();
	else
		printf("ret = %d\n", ret);

	close(args.fd);

L_END:
	printf("\n\n");

	return ret;
}
