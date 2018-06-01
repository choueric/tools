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
#include <stdarg.h>

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
	.device = "/dev/spidev0.0",
	.mode = 0,
	.bits = 8,
	.speed = 10000000,
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

static void do_log(const char *format, ...)
{
	char str[50] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);
	gettimeofday(&tv, NULL);
	snprintf(str, 50, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, 50, format, args);
	va_end(args);
	printf("%s", str);
}

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

	printf("%s %x %x %u\n", entry->cmd, addr, data, size);

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
	printf("%s: %x %u\n", entry->cmd, addr, size);
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
	printf("%s: %x %u\n", entry->cmd, addr, size);
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
	char *label;
};

static uint8_t test_read_buf[1024];
static uint8_t test_send_buf[56];

static void time_print(char *str)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	printf("%ld.%06ld: %s", t.tv_sec, t.tv_usec, str);
}

#define READ_LEN_DATA (4 * 1024)
#define WRITE_LEN_DATA 256
#define READ_LEN_STA 10
static int cmd_test(struct cmd_args *args, struct cmd_entry *entry)
{
	struct part parts[] = {
		{.addr = 0x0000, .size = READ_LEN_STA, .is_read = 1, .label = "read state 1", },
		{.addr = 0x1000, .size = READ_LEN_DATA, .is_read = 1, .label = "read data",},
		{.addr = 0x2000, .size = READ_LEN_STA, .is_read = 1, .label = "read state 2", },
		{.addr = 0x3000, .size = WRITE_LEN_DATA, .is_read = 0, .label = "write data", },
	};
	unsigned char *read_buf;
	unsigned char *write_buf;
	int n = 0, i;
	struct part *p = NULL;

	read_buf = calloc(1, READ_LEN_DATA);
	if (read_buf == NULL) {
		perror("malloc failed");
		exit(1);
	}

	write_buf = calloc(1, WRITE_LEN_DATA);
	if (write_buf == NULL) {
		perror("malloc write failed");
		exit(1);
	}

	n = ARRAY_SIZE(parts);
	while (1) {
		time_print("=========================================\n");
		for (i = 0; i < n; i++) {
			p = parts + i;
			do_log("---> %s <---\n", p->label);
			if (p->is_read) {
				memset(read_buf, 0, p->size);
				flash_read_data(args->fd, p->addr, read_buf, p->size);
				//print_buf(read_buf, p->size);
			} else {
				// TODO: erase before write
				//printf("write %x %d\n", p->addr, p->size);
				flash_page_program(args->fd, p->addr, write_buf, p->size);
			}
		}
		time_print("---------------- end -------------------\n");
	}

    free(read_buf);
    free(write_buf);

	return 0;
}

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
	print_cfg(p);

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
