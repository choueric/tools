CC=gcc
EXEC=test

INC=-I../lib
LIBS=../lib/liblib.a
CFLAGS=-Wall -Werror -g -O2 -std=c99 $(INC)

SUBDIRS=../lib 

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

export

.PHONY: all $(SUBDIRS) clean

all:$(SUBDIRS) $(EXEC)

$(EXEC):$(LIBS) $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(SUBDIRS):
	$(MAKE) -C $@

clean_dirs := $(addprefix _clean_,$(SUBDIRS) )

$(clean_dirs):    
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean

clean: $(clean_dirs)    
	rm -rf $(EXEC) *.o
