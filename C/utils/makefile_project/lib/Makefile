SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

TARGET=liblib.a

all:dep $(TARGET)

dep: $(SRCS)
	$(CC) $(CFLAGS) -MM $(SRCS) > dep.mk
-include dep.mk

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS)

clean:
	rm -rf dep.mk *.o $(TARGET)
