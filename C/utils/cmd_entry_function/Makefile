CC = gcc
#CC = arm-hisiv400-linux-gcc
FLAGS = -Wall -g -Werror

SRC = main.c cmd_entry.c cmd_entry.h
EXEC = prpctrl_test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome
