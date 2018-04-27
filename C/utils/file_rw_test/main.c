#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define __USE_GNU 1
#include <fcntl.h>


static size_t data_size = 512 * 1024; // 512K
static uint8_t *data = NULL;
static uint8_t *read_data = NULL;


static int file_test(int num)
{
	char name[128];

	snprintf(name, 128, "file_%d", num);
	printf("file test for %s\r", name);
	fflush(stdout);

	int fd = open(name, O_SYNC | O_RDWR | O_TRUNC | O_DIRECT | O_CREAT);
	if (fd < 0) {
		printf("  open %s for write failed: %m\n", name);
		return -1;
	}

	int ret = write(fd, data, data_size);
	if (ret < 0) {
		printf("  write error: %m\n");
		goto fail;
	}
	if (ret != data_size) {
		printf("  write count error: %m\n");
		goto fail;
	}
	close(fd);

	fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf("  open %s for read failed: %m\n", name);
		return -1;
	}

	ret = read(fd, read_data, data_size);
	if (ret < 0) {
		printf("  read error: %m\n");
		goto fail;
	}
	if (ret != data_size) {
		printf("  read count error: %m\n");
		goto fail;
	}

	ret = memcmp(read_data, data, data_size);
	if (ret) {
		printf("  data dose not match\n");
	}

fail:
	close(fd);

	return 0;
}

int small_file_test(int argc, char **argv)
{
	int count = 5;
	if (argc == 2) {
		count = atoi(argv[1]);
	}

	printf("== emmc test ==\n");
	uint8_t *start = NULL;

	start = malloc(sizeof(uint8_t) * data_size + 512);
	if (start == NULL) {
		printf("malloc failed: %m\n");
		return -1;
	}
	long p = (long)start + 512;
	data = (uint8_t *)(p&(~511)); // aligned to 512 because of O_DIRECT
	for (int i = 0; i < data_size; i++)
		data[i] = i;
	printf("  start = %p, aligned to %p\n", start, data);

	read_data = malloc(sizeof(uint8_t) * data_size);
	if (read_data == NULL) {
		printf("malloc failed: %m\n");
		return -1;
	}

	for (int i = 0; i < count; i++)
		file_test(i);

	free(start);
	free(read_data);
	return 0;
}


int big_file_test(int argc, char **argv)
{
	unsigned long long size = 6LL * 1024 * 1024 * 1024;
	char *name = "file_big";
	uint8_t *data = NULL;
	uint8_t *read_data = NULL;
	size_t data_size = 512 * 1024;

	printf("size = %llu\n", size);

	data = malloc(data_size);
	if (data == NULL) {
		printf("mallco failed: %m\n");
		return -1;
	}
	for (int i = 0; i < data_size; i++)
		data[i] = i;
	read_data = malloc(data_size);
	if (read_data == NULL) {
		printf("mallco failed: %m\n");
		return -1;
	}

	int fd = open(name, O_RDWR | O_CREAT | O_TRUNC);
	if (fd < 0) {
		printf("  open %s for read failed: %m\n", name);
		return -1;
	}

	for (unsigned long long i = 0; i < size; i++) {
		printf("writen %lld Byte\r", i);
		fflush(stdout);
		int ret = write(fd, data, data_size);
		if (ret < 0) {
			printf("\n  write error: %m\n");
			return -1;
		}
		i += data_size;
		fsync(fd);
	}

	close(fd);

	fd = open(name, O_RDONLY);
	if (fd < 0) {
		printf("open for read failed: %m\n");
		return -1;
	}

	for (unsigned long long i = 0; i < size; i++) {
		printf("read %lld Byte\r", i);
		fflush(stdout);
		int ret = read(fd, read_data, data_size);
		if (ret < 0) {
			printf("\n read error: %m\n");
			return -1;
		}
		i += data_size;
		if (memcmp(data, read_data, data_size) != 0) {
			printf(" compare failed\n");
			return -1;
		}
	}

	close(fd);

	free(data);
	free(read_data);
	printf("\n");

	return 0;
}

int main(int argc, char **argv)
{
	//small_file_test(argc, argv);

	int ret = 0;
	while (ret == 0) {
	ret = big_file_test(argc, argv);
	}

	return 0;
}
