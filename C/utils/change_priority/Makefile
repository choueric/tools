CC = gcc
FLAGS = -Wall -g -Werror

SRC = test.c change_priority.c change_priority.h
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core
