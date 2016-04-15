CC = gcc
FLAGS = -rdynamic -Wall -g -Werror

SRC = main.c
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome

symbol:$(EXEC)
	objdump -t $(EXEC) > symbol
