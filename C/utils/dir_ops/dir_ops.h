#pragma once
#include <dirent.h>

/*
 * if @path is a file, delete this file.
 * if @path is a directory, delete this directory recursely
 */
void delete_files(const char *path);

/*
 * if @appendix is "dmp", delete all *.dmp in @path
 */
int delete_appendix(char *path, char *appendix);

int is_dir(const char *path);

int is_file(const char *path);

int is_special_dir(const char *path);

/*
 * @path: [in] /home/user/dir
 * @filename: [in] test.c
 * @file_path: [out] /home/user/dir/test.c
 */
void get_file_path(const char *path, const char *filename, char *file_path);

/* scan @dir_path using @filter to filter, return number of matched entries */
int scan_dir(const char *dir_path, int (*filter)(const struct dirent *));
