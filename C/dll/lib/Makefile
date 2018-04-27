LIB=libtriadd.so
SRC=triadd.c
$(LIB):$(SRC)
	$(CC) -fdiagnostics-color -Wall -g -std=c11 -I../vendor -fPIC -c $(SRC)
	$(CC) triadd.o -ldl -shared -o $(LIB)

clean:
	rm -rf *.o *.so *.gch

install:
	install $(LIB) /tmp
