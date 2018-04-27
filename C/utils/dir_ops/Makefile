CC = gcc
FLAGS = -Wall -g -Werror

SRC = dir_ops.c main.c
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core
