#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "filesystem.h"

int GetFileSize(const char *name)
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

int DeleteFile(const char *name)
{
	if (NULL == name)
		return -1;
	return unlink(name);
}

int GetFreeSpace(const char *path)
{
	struct statfs diskInfo;
	unsigned long long freeDisk = 0;

	if (path == NULL)
		return -1;

	statfs(path, &diskInfo);

	freeDisk = diskInfo.f_bfree * diskInfo.f_bsize;
	if( freeDisk > 0x7FFFFFFFUL )
		freeDisk = 0x7FFFFFFF;

	return (int)freeDisk;
}
