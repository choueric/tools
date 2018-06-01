#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>

static int is_dir(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == 0)
		return S_ISDIR(statbuf.st_mode) != 0;
	return 0;
}

static int is_file(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == 0)
		return S_ISREG(statbuf.st_mode) != 0;
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

// recurse to delete path(maybe dir or file)
void delete_file(const char *path)
{
	DIR *dir;
	struct dirent *dir_info;
	char file_path[PATH_MAX];

	if (is_file(path)) {
		remove(path);
		return;
	}

	if (is_dir(path)) {
		if ((dir = opendir(path)) == NULL)
			return;
		while ((dir_info = readdir(dir)) != NULL) {
			get_file_path(path, dir_info->d_name, file_path);
			if (is_special_dir(dir_info->d_name))
				continue;
			delete_file(file_path);
			rmdir(file_path);
		}
	}
}

static int clean_dump(char *path)
{
	DIR *dir;
	struct dirent *dir_info;
	char file_path[PATH_MAX];

	dir = opendir(path);
	if (dir == NULL) {
		perror("open dir failed");
		return 1;
	}

	printf("path max %d\n", PATH_MAX);

	while ((dir_info = readdir(dir)) != NULL) {
		if (strstr(dir_info->d_name, ".dmp") == NULL)
			continue;
		get_file_path(path, dir_info->d_name, file_path);
		printf("%s\n", file_path);
		if (is_file(file_path))
			remove(file_path);
	}

	return 0;
}

int main(int argc, char **argv)
{
	clean_dump("./testdir");

	return 0;
}
