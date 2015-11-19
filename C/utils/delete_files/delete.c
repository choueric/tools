#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int is_dir(const char *path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
		return S_ISDIR(statbuf.st_mode) != 0;
	return 0;
}

static int is_file(const char *path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf) == 0)
		return S_ISREG(statbuf.st_mode) != 0;
	return 0;
}

static int is_special_dir(const char *path)
{
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}


/*
 * @path: [in] /home/user/dir
 * @filename: [in] test.c
 * @file_path: [out] /home/user/dir/test.c
 */
static void get_file_path(const char *path, const char *filename, char *file_path)
{
	strcpy(file_path, path);
	if (file_path[strlen(path) - 1] != '/')
		strcat(file_path, "/");
	strcat(file_path, filename);
}

// recurse to delete path(maybe dir or file)
void delete_files(const char *path)
{
	DIR *dir;
	struct dirent *dir_info;
	char file_path[PATH_MAX];

	if (is_file(path)) {
		remove(path);
		printf("delete file %s\n", path);
		return;
	}

	if (is_dir(path)) {
		if ((dir = opendir(path)) == NULL)
			return;
		while ((dir_info = readdir(dir)) != NULL) {
			get_file_path(path, dir_info->d_name, file_path);
			if (is_special_dir(dir_info->d_name))
				continue;
			delete_files(file_path);
			rmdir(file_path);
		}
	}
	printf("delete dir %s\n", path);
}

/*
 * if @appendix is ".dmp", delete all *.dmp in @path
 */
int delete_appendix(char *path, char *appendix)
{
	DIR *dir;
	struct dirent *dir_info;
	char file_path[PATH_MAX];

	dir = opendir(path);
	if (dir == NULL) {
		perror("open dir failed");
		return 1;
	}

	while ((dir_info = readdir(dir)) != NULL) {
		if (strstr(dir_info->d_name, appendix) == NULL)
			continue;
		get_file_path(path, dir_info->d_name, file_path);
		printf("delete %s\n", file_path);
		if (is_file(file_path))
			remove(file_path);
	}

	return 0;
}


