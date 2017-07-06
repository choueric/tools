CC = gcc
FLAGS = -Wall -g -Werror -std=c11

SRC = main.c args.c
EXEC = rtc-test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) main.c -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:
	scp $(SRC) Makefile ubuntu@test:~/rtc/
