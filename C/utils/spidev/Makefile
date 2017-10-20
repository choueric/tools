CC=/home/zhs/workspace/ti8107/toolchain/cgt_a8/arago/linux-devkit/bin/arm-arago-linux-gnueabi-gcc

TARGET = $(shell basename $(PWD))

SRCS := $(shell ls *.c)

CFLAGS = -Wall -g 

LDFLAGS ?=

LOCAL_LDFLAGS = -lpthread

export CFLAGS
export LDFLAGS

include ../Makefile
