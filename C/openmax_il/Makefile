OPENMXLIB=xyzomx

CC = gcc
FLAGS = -Wall -g -I/usr/include/openmax -L/usr/lib/arm-linux-gnueabihf/
LIBS = -l$(OPENMXLIB)
EXEC = test

SRC = main.c omx.c

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/bin
