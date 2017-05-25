CC = gcc
FLAGS = -Wall -g -Werror -I../i2cdev

SRC = ../i2cdev/bus_i2cdev.c ../i2cdev/bus_i2cdev.h main.c 
EXEC = redriver

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:
	scp $(SRC) Makefile ubuntu@test:~/redriver/
