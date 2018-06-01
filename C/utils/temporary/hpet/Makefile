CC=gcc
#CC=i686-pc-linux-gnu-gcc
EXEC=test
SRC=main.c
HEADER=

FLAGS=-Wall -g

$(EXEC):$(SRC) $(HEADER)
	$(CC) $(FLAGS) $(SRC) $(HEADERS) -o $(EXEC)

clean:
	rm -rf *.o $(EXEC)

cp:
	sudo cp $(EXEC) /media/sf_tftp/
