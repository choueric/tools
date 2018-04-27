TARGET = $(shell basename $(shell pwd))

SRCS := $(shell ls *.c)
SRCS += ../semaphore_cond/semaphore_cond.c
SRCS += ../queue_demo/queue.c

CFLAGS += -Wall -Werror -std=c11 -g -fdiagnostics-color=auto

LDFLAGS = -lpthread
LOCAL_LDFLAGS =

export CFLAGS
export LDFLAGS

include ../Makefile
