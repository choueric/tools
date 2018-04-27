TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)

CFLAGS += -Wall -g -fdiagnostics-color=auto

LDFLAGS ?=

LOCAL_LDFLAGS = -ljpeg

export CFLAGS
export LDFLAGS

include ../Makefile
