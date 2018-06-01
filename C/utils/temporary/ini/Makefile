CC=gcc
#CC=arm-none-linux-gnueabi-gcc

EXEC=test
FLAGS=-Wall -g

$(EXEC):main.c ini.c ini.h 
	$(CC) $(FLAGS) main.c ini.c ini.h -o $(EXEC)

cp:
	cp $(EXEC) /media/sf_tftp

clean:
	rm -f $(EXEC)
