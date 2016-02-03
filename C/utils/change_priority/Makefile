#CC = gcc
CC = arm-arago-linux-gnueabi-gcc
FLAGS = -Wall -g -Werror

SRC = test.c change_priority.c change_priority.h
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC) -lpthread

clean:
	rm -f $(EXEC) *.o core
