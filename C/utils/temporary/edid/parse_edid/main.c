#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/types.h>
#include <errno.h>

#include "edid.h"

#ifdef DEBUG
#define debug_print printf
#else
#define debug_print
#endif

static void dump_buf(const unsigned char *buf, int len)
{
    int i = 0;

	printf("--------------------------------");
	for (i = 0; i < len; i++) {
		if (i % 16 == 0)
			printf("\n");
		printf("0x%02x ", buf[i]);
	}
	printf("\n--------------------------------\n");
}


static int get_file_size(char *name)
{
	if (NULL == name)
		return -1;

	struct stat st;
	if (stat(name, &st) < 0) {
		perror("stat failed");
		return -1;
	}

	return st.st_size;
}

static unsigned char *read_edid_file(char *path, int *psize)
{
    unsigned char *buf = NULL;
    int size = 0;
    int fd = 0;
    int ret = 0;

    size = get_file_size(path);
    if (size == -1) {
        perror("get file size failed");
        return NULL;
    }

    buf = malloc(size);
    if (buf == NULL) {
        perror("malloc failed");
        return NULL;
    }
    memset(buf, 0, size);
    
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        free(buf);
        return NULL;
    }

    *psize = size;
    ret = read(fd, buf, size);
    if (size != ret) {
        perror("read firmware failed");
        free(buf);
        close(fd);
        return NULL;
    }

    close(fd);

    return buf;
}


int main(int argc, char *argv[])
{
    unsigned char *buf = NULL;
    struct edid_info info;
    int size;

    if (argc != 2) {
        printf("usage: ./test [edid]\n");
        return 1;
    }

    buf = read_edid_file(argv[1], &size);

    dump_buf(buf, size);
    parse_ddc_edid(buf, &info);
    dump_edidinfo(&info);
    free(buf);
    
    return 0;
}
