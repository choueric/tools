LIB=liblowlevel.so
SRC=lowlevel.c
$(LIB):$(SRC)
	$(CC) -Wall -g -std=c11 -fPIC -c $(SRC)
	$(CC) lowlevel.o -shared -o $(LIB)

clean:
	rm -rf *.o *.so *.gch

install:
	install $(LIB) /tmp
