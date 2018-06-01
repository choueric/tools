#CC=gcc
CC=i686-pc-linux-gnu-gcc

EXEC=spi_installer
FLAGS=-Wall -g -DTEST_S_PAGE
SRC=main.c flash.c spidev.c upgrade.c crc.c bridge.c ui.c

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

cp:
	sudo cp $(EXEC) /media/sf_tftp

clean:
	rm -f $(EXEC)
