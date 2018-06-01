#CC=arm-none-linux-gnueabi-gcc
CC=i686-pc-linux-gnu-gcc

all:main.c edid.c
	$(CC) -Wall main.c  edid.c -o parse_edid 
	$(CC) -Wall parse-edid.c -o test2
cp:
	cp parse_edid /media/sf_tftp/
