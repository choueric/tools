#CC = gcc
CC = arm-arago-linux-gnueabi-gcc
FLAGS = -Wall -g -Werror

SRC = main.c
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome
