CC=arm-none-linux-gnueabi-gcc
EXEC=nltest

all:main.c listen.c
	$(CC) -Wall -g main.c  -o $(EXEC)
	cp $(EXEC) /media/sf_tftp/
	$(CC) -Wall -g listen.c  -o listen
	cp listen /media/sf_tftp/
cp:
	cp $(EXEC) /media/sf_tftp/

