EXEC=file_rw_test
CC=gcc

all:main.c
	$(CC) -Wall --std=c11 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 main.c  -o $(EXEC)

clean:
	rm -rf $(EXEC) *.o file_*

scp:
	scp main.c Makefile ubuntu@test:~/setupKernel/emmc_test
