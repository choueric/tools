TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)

CFLAGS += -Wall -Werror -std=gnu11 -g -fdiagnostics-color=auto

LDFLAGS ?= -lpthread

LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile
