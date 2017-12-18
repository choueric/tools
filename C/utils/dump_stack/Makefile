TARGET = $(shell basename $(shell pwd))
SRCS := $(shell ls *.c)
CFLAGS += -Wall -Werror -g -fdiagnostics-color=auto -rdynamic

LDFLAGS ?=
LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile

symbol:
	nm -a $(TARGET) | sort
