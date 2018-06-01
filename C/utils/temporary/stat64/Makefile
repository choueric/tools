CC=gcc
#CC=arm-none-linux-gnueabi-gcc

EXEC=test
FLAGS=-Wall -g -D__USE_LARGEFILE64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE

$(EXEC):main.c 
	$(CC) $(FLAGS) main.c  -o $(EXEC)

cp:
	cp $(EXEC) /media/sf_tftp

clean:
	rm -f $(EXEC)
