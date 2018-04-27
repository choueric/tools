#ifndef _DELETE_H_
#define _DELETE_H_

/*
 * if @path is a file, delete this file.
 * if @path is a directory, delete this directory recursely
 */
void delete_files(const char *path);

/*
 * if @appendix is "dmp", delete all *.dmp in @path
 */
int delete_appendix(char *path, char *appendix);

#endif

