obj-m += sample.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: driver test

driver:
	        $(MAKE) -C $(KDIR) M=$(PWD) modules

test:
	gcc -Wall -g test.c -o test
clean:
	        $(MAKE) -C $(KDIR) M=$(PWD) clean
		rm -rf test
