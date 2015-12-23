CC=arm-none-linux-gnueabi-gcc
#CC=gcc
EXEC=test

all:main.c
	$(CC) -E main.c -o main.i
	$(CC) -S main.i -o main.s
	$(CC) -c main.s -o main.o
	$(CC) main.o -o $(EXEC)

$(EXEC):main.c
	$(CC) -Wall main.c -o $(EXEC)


clean:
	rm -rf $(EXEC) *.i *.o *.s
