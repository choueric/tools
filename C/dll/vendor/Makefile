LIB=libvendor.so
SRC=add.c
$(LIB):$(SRC)
	$(CC) -Wall -g -std=c11 -fPIC -c $(SRC)
	$(CC) add.o -shared -o $(LIB)

clean:
	rm -rf *.o *.so *.gch

install:
	install $(LIB) /tmp
