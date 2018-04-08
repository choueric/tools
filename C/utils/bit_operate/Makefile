TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)

CFLAGS += -Wall -Werror -std=c11 -g -fdiagnostics-color=auto

LDFLAGS ?=

LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile
