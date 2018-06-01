CC=arm-none-linux-gnueabi-gcc
#CC=gcc
EXEC=wext

SOURCES=main.c cmd.c
HERADERS=cmd.h

all:$(SOURCES) $(HEADERS)
	$(CC) -Wall -g $(SOURCES) $(HEADERS)  -o $(EXEC)
	cp $(EXEC) /media/sf_tftp/
cp:
	cp $(EXEC) /media/sf_tftp/
