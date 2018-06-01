#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <unistd.h>

static int is_dir(const char *path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
		return S_ISDIR(statbuf.st_mode) != 0;
	else {
		printf("lstat %s failed\n", path);
		perror("    ");
	}
	return 0;
}

static int is_file(const char *path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
		return S_ISREG(statbuf.st_mode) != 0;
	else {
		printf("lstat %s failed\n", path);
		perror("    ");
	}
	return 0;
}

static int is_special_dir(const char *path)
{
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

void get_file_path(const char *path, const char *filename, char *file_path)
{
	strcpy(file_path, path);
	if (file_path[strlen(path) - 1] != '/')
		strcat(file_path, "/");
	strcat(file_path, filename);
}

static int callback(char *file, void *data)
{
	int *p = (int *)data;
	int i = 0;

	for (i = 0; i < *p; i++)
		printf("    ");
	printf("[%s]\n", file);
	return 0;
}

static int check_file_1(char *file, void *data)
{
	int fd;

	//printf("%s\n", file);
	if (!is_file(file))
		return 0;

	fd = open(file, O_RDWR);
	if (fd < 0) {
		printf("open %s failed\n", file);
		perror("    ");
		exit(1);
	}
	if (close(fd) < 0) {
		printf("close %s failed\n", file);
		perror("    ");
		exit(1);
	}
	return 0;
}

static int check_file(char *file, void *data)
{
	int fd;
	char *buf;
	struct stat statbuf;
	int size;

	//printf("%s\n", file);
	if (!is_file(file))
		return 0;

#if 0
	if (lstat(file, &statbuf) <= 0) {
		printf("stat %s failed\n", file);
		perror("    ");
		exit(1);
	}
	size = statbuf.st_size;
#else
	size = 100;
#endif

	buf = malloc(size);
	if (buf == NULL) {
		printf("malloc failed\n");
		return 1;
	}

	fd = open(file, O_RDWR);
	if (fd < 0) {
		printf("open %s failed\n", file);
		perror("    ");
		exit(1);
	}
	if (read(fd, buf, size) < 0) {
		printf("read %s failed\n", file);
		perror("    ");
		exit(1);
	}
	if (close(fd) < 0) {
		printf("close %s failed\n", file);
		perror("    ");
		exit(1);
	}
	return 0;
}

static int recurse_dir(char *path, int (*func)(char *, void *))
{
	DIR *dir;
	struct dirent ent;
	struct dirent *result;
	char file_path[PATH_MAX];
	static int level = 0;
	int ret;

	if (is_file(path)) {
		func(path, &level);
		return 0;
	} else if (is_dir(path)) {
		func(path, &level);
		if ((dir = opendir(path)) == NULL) {
			printf("open %s failed\n", path);
			perror("    ");
			return 1;
		}
		level++;
		while (1) {
			ret = readdir_r(dir, &ent, &result);
			if (ret != 0) {
				printf("readdir %s failed\n", path);
				perror("     ");
				break;
			} else if (ret == 0 && result == NULL)
				break;
			get_file_path(path, ent.d_name, file_path);
			if (is_special_dir(ent.d_name))
				continue;
			recurse_dir(file_path, func);
		}
		closedir(dir);
		level--;
	} else {
		printf("un-supported type\n");
		return 0;
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("invalid args\n");
		return 0;
	}

	return recurse_dir(argv[1], check_file);
}
