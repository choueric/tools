LIB=libvendor.so
SRC=add.c
$(LIB):$(SRC)
	$(CC) -Wall -g -I../lowlevel -std=c11 -fPIC -c $(SRC)
	$(CC) add.o -L/tmp -llowlevel -shared -o $(LIB)

clean:
	rm -rf *.o *.so *.gch

install:
	install $(LIB) /tmp
