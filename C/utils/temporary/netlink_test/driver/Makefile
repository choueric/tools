CC=arm-none-linux-gnueabi-gcc

ifneq ($(KERNELRELEASE),)
obj-m := nlevent.o
else
KERNELDIR ?= /home/mindray/workspace/t1dock/T1-docking/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *.order *.symvers *.cmd *.ko *.mod.c .tmp_versions .*.cmd
