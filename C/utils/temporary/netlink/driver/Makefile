CC=arm-none-linux-gnueabi-gcc

ifneq ($(KERNELRELEASE),)
obj-m := nltest.o
else
KERNELDIR ?= /home/mindray/workspace/t1dock/T1-docking/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

cp:
	cp nltest.ko /media/sf_tftp/
