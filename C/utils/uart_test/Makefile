TARGET = $(shell basename $(shell pwd))

SRCS := main.c

CFLAGS += -Wall -Werror -std=c11 -g -fdiagnostics-color=auto

LDFLAGS ?=

LOCAL_LDFLAGS = -lpthread

export CFLAGS
export LDFLAGS

include ../Makefile
