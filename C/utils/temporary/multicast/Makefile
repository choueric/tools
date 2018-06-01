#CC=gcc
CC=i686-pc-linux-gnu-gcc
FLAGS=-Wall -g

all:send.c recv.c
	$(CC) $(FLAGS) send.c -o send
	$(CC) $(FLAGS) recv.c -o recv

clean:
	rm -rf *.o $(EXEC)

cp:
	sudo cp send recv /media/sf_tftp/
