SRC=main.c item.c
HEADER=item.h

EXEC=voc

CC=gcc

#FLAGS=-DDEBUG
FLAGS=

$(EXEC): $(SRC) $(HEADER)
	$(CC) -Wall -g $(FLAGS) $(SRC) $(HEADER) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)
