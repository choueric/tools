CC=arm-none-linux-gnueabi-gcc
NLEVENT_INC = $(obj)/../driver
EXTRA_CFLAGS +=-I$(NLEVENT_INC)
KBUILD_EXTRA_SYMBOLS=$(obj)/../driver/Module.symvers

ifneq ($(KERNELRELEASE),)
obj-m := nlcli.o
else
KERNELDIR ?= /home/mindray/workspace/t1dock/T1-docking/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *.order *.symvers *.cmd *.ko *.mod.c .tmp_versions .*.cmd
