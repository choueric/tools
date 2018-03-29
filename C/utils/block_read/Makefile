TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)

FT_CFLAGS=$(shell freetype-config --cflags)

CFLAGS += -Wall -Werror -std=c11 -g -fdiagnostics-color=auto $(FT_CFLAGS)

LDFLAGS += $(shell freetype-config --libs)

LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile
