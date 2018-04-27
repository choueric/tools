#CC=arm-none-linux-gnueabi-gcc
#CC=arm-arago-linux-gnueabi-gcc
CC=arm-linux-gnueabihf-gcc
#CC=gcc
EXEC=test
FLAGS=-mfpu=neon -mfloat-abi=hard

all:main.c
	$(CC) -E main.c -o main.i
	$(CC) $(FLAGS) -S main.i -o main.s
	$(CC) $(FLAGS) -c main.s -o main.o
	$(CC) main.o -o $(EXEC)

$(EXEC):main.c
	$(CC) -Wall main.c -o $(EXEC)

clean:
	rm -rf $(EXEC) *.i *.o *.s
