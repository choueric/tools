#include "filesystem.h"

#include <stdio.h>


int GetFileSize_test() {
	const char *file = "./filesystem.c";
	int size = GetFileSize(file);
	printf("%s size = %d\n", file, size);

	return 0;
}

int DeleteFile_test() {
	return 0;
}

int GetFreeSpace_test() {
	int size = GetFreeSpace("/boot");
	printf("/boot free space is %d\n", size);
	return 0;
}

int main(int argc, char **argv) {
	GetFileSize_test();
	GetFreeSpace_test();

	return 0;
}
