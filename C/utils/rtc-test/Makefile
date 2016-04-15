CC = /home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc
FLAGS = -Wall -g -Werror

SRC = main.c
EXEC = rtc-test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core
