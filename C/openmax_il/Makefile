CC = gcc
#FLAGS = -Wall -g -Werror -I/usr/include/openmax -L/usr/lib/arm-linux-gnueabihf/tegra
FLAGS = -Wall -g -I/usr/include/openmax -L/usr/lib/arm-linux-gnueabihf/tegra
LIBS = -lnvomx
EXEC = omx_client_test

SRC = main.c omx.c statistic.c

$(EXEC):$(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC) *.o core

install:$(EXEC)
	@cp -v $(EXEC) $(HOME)/workspace/tftphome
