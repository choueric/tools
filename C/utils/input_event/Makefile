CC = gcc
#CC = arm-hisiv400-linux-gcc
FLAGS = -Wall -g -Werror

SRC = input_event.c
EXEC = test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome
