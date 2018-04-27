TARGET = $(shell basename $(PWD))

SRCS := bufhead.c main.c

CFLAGS += -Wall -g -fdiagnostics-color=auto

LDFLAGS ?=

LOCAL_LDFLAGS = -lpthread

export CFLAGS
export LDFLAGS

include ../Makefile
