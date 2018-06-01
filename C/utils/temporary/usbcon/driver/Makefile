CC=i686-pc-linux-gnu-gcc

ifneq ($(KERNELRELEASE),)
obj-m := usbcon.o
else
KERNELDIR ?= /home/mindray/workspace/dipper/Trunk/_build
#KERNELDIR ?= /usr/src/linux-headers-3.2.0-4-486
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

cp:
	sudo cp usbcon.ko /media/sf_tftp/
