CC=/home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc
EXEC=fbtest

test:main.c
	$(CC) -Wall -g main.c -o $(EXEC)

clean:
	@rm -rf $(EXEC)

install:
	@cp -v $(EXEC) ~/workspace/tftphome
