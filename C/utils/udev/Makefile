TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)

CFLAGS += -Wall -Werror -std=gnu11 -g -fdiagnostics-color=auto

LDFLAGS += -ludev

LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile
