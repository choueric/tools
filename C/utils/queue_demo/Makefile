CC = gcc
FLAGS = -Wall -g -Werror -std=c11

SRC = main.c  queue.c
EXEC = $(shell basename $(PWD))

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:
	scp $(SRC) Makefile ubuntu@test:~/rtc/
