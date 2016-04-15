CC=/home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc
EXEC=gv7600
SRC=main.c bus_spidev.c bus_spidev.h


test:$(SRC)
	$(CC) -Wall -g $(SRC) -o $(EXEC)

clean:
	@rm -rf $(EXEC)

install:
	@cp -v $(EXEC) ~/workspace/tftphome
