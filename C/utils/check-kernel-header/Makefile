#CC=gcc
CC = /home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc

EXEC=check
FLAGS=-Wall -g 

$(EXEC):*.c
	$(CC) $(FLAGS) $(CFLAGS_ABI) main.c -o $(EXEC)

cp:
	cp $(EXEC) /home/zhs/workspace/tftphome

clean:
	rm -f $(EXEC)

