CC = gcc
#FLAGS = -Wall -g -Werror
#FLAGS = -Wall -g -Werror --std=c99
FLAGS = -Wall -g -Werror -std=gnu99

SRC = main.c
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome
