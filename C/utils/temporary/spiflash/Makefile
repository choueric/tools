CC=gcc
#CC=arm-none-linux-gnueabi-gcc

EXEC=spiflash
FLAGS=-Wall -g

$(EXEC):main.c ini.c ini.h flash.c flash.h
	$(CC) $(FLAGS) main.c ini.c ini.h flash.c flash.h -o $(EXEC)

cp:
	cp $(EXEC) /media/sf_tftp

clean:
	rm -f $(EXEC)
