#CC=arm-none-linux-gnueabi-gcc
CC=gcc
EXEC=test

# release
#FLAGS=-O2 -fno-optimize-sibling-calls -fno-omit-frame-pointer -fno-strict-aliasing -fno-delete-null-pointer-checks -fno-gcse -fno-guess-branch-probability -g -Wall -c -fmessage-length=0 -fms-extensions -fsigned-char

#FLAGS=-g -Wall -fomit-frame-pointer

FLAGS=-g -Wall -O1

# standard mapcs stack frame
#FLAGS=-mapcs-frame

#FLAGS=-O2 -g -Wall
#LIB_FLAGS=-g -Wall -fomit-frame-pointer

$(EXEC):main.c
	$(CC) $(FLAGS) main.c  -o $(EXEC)

clean:
	rm $(EXEC)
