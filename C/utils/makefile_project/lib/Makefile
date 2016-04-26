SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
DEPS=$(SRCS:%.c=%.d)

TARGET=liblib.a

all:$(TARGET)

$(DEPS):%.d:%.c 
	@set -e rm -f $@
	$(CC) -MM $(CFLAGS) $< > $@.1234
	@sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.1234 > $@
	@rm -f $@.1234

-include $(DEPS)

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS)

clean:
	rm -rf *.d *.o $(TARGET)
