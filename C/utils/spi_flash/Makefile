#CC=gcc
CC = /home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc

EXEC=spiflash
FLAGS=-Wall -g 
CFLAGS_ABI	:=-mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp

$(EXEC):main.c ini.c ini.h flash.c flash.h
	$(CC) $(FLAGS) $(CFLAGS_ABI) main.c ini.c ini.h flash.c flash.h -o $(EXEC)

cp:
	cp $(EXEC) /home/zhs/workspace/tftphome

clean:
	rm -f $(EXEC)
