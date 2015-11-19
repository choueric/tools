CC = gcc
FLAGS = -Wall -g -Werror

SRC = test.c print_time.c print_time.h
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core
