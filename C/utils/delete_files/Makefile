CC = gcc
FLAGS = -Wall -g -Werror

SRC = test.c delete.c delete.h
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core
