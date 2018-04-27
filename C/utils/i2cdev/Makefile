CC ?= gcc
CC = arm-hisiv600-linux-gcc
FLAGS = -Wall -g -Werror -std=c11

SRC = test.c bus_i2cdev.c
EXEC = i2c-test

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o core

install:
	@#scp $(SRC) Makefile ubuntu@test:~/rtc/
	@sudo cp -v $(EXEC) /home/zhs/workspace/StreamArtPro/tools/mnt/home/root
