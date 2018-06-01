CC=gcc
EXEC=test
SRC=main.c counter.c
HEADER=counter.h

FLAGS=-Wall -g

$(EXEC):$(SRC) $(HEADER)
	$(CC) $(FLAGS) $(SRC) $(HEADERS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)

cp:
	sudo cp $(EXEC) /media/sf_tftp/
