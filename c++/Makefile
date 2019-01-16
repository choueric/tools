CC ?= g++
AR ?= ar

export CC
export AR

c-objs := $(filter %.o, $(SRCS:.c=.o))
cpp-objs := $(filter %.o, $(SRCS:.cpp=.o))

c-deps := $(filter %.d, $(SRCS:.c=.d))
cpp-deps := $(filter %.d, $(SRCS:.cpp=.d))

ifneq ($(cpp-objs),)
	LDCC = $(CXX)
else
	LDCC = $(CC)
endif

# 需要排除的目录
exclude_dirs := temporary

# 取得当年子目录深度为1的所有目录名称
dirs := $(shell find . -maxdepth 1 -type d)
dirs := $(basename $(patsubst ./%,%,$(dirs)))
dirs := $(filter-out $(exclude_dirs),$(dirs))

# 避免clean子目录操作同名，加上_clean_前缀

SUBDIRS := $(dirs)

all: $(c-deps) $(cpp-deps) __SUBDIR__ $(TARGET) $(TARGET_LIBA) $(TARGET_LIB)

$(TARGET) : $(c-objs) $(cpp-objs) $(LOCAL_LIBS) $(LIBS)
	$(LDCC) -o $@ $^ $(LDFLAGS) $(LOCAL_LDFLAGS)

$(TARGET_LIBA) : $(c-objs) $(cpp-objs)
	$(AR) -r $@ $^

$(TARGET_LIB) : $(c-objs) $(cpp-objs)
	$(CC) $^ -shared $(LOCAL_LDFLAGS) -o $@

__SUBDIR__ :
	@for i in $(SUBDIRS); do \
		make -C $$i; \
		if [ $$? -ne 0 ]; then \
			exit 1; \
		fi \
		done

clean:
	@for i in $(SUBDIRS); do make -C $$i clean; done
	@for i in $(c-objs); do if [ -e $$i ] ; then echo "rm $$i"; rm $$i; fi done
	@for i in $(cpp-objs); do if [ -e $$i ] ; then echo "rm $$i"; rm $$i; fi done
	@for i in $(c-deps); do if [ -e $$i ] ; then echo "rm $$i"; rm $$i; fi done
	@for i in $(cpp-deps); do if [ -e $$i ] ; then echo "rm $$i"; rm $$i; fi done
	@if [ -e "$(TARGET)" ] ; then echo "rm $(TARGET)"; rm $(TARGET) ; fi
	@if [ -e "$(TARGET_LIBA)" ] ; then echo "rm $(TARGET_LIBA)"; rm $(TARGET_LIBA) ; fi
	@if [ -e "$(TARGET_LIB)" ] ; then echo "rm $(TARGET_LIB)"; rm $(TARGET_LIB) ; fi

$(c-objs): %.o:%.c
	$(CC) -c $(CFLAGS) $(LOCAL_CFLAGS) $< -o $*.o

$(cpp-objs): %.o:%.cpp
	$(CXX) -c $(CFLAGS) $(LOCAL_CFLAGS) $< -o $*.o

$(c-deps): %.d:%.c
	@set -e; $(CC) -MM $(CFLAGS) $(LOCAL_CFLAGS) $< \
		| sed 's,\($(*F)\)\.o[ :]*,$*.o $*.d : ,g' > $*.d; \
		[ -s $@ ] || rm -f $@

$(cpp-deps): %.d:%.cpp
	@set -e; $(CXX) -MM $(CFLAGS) $(LOCAL_CFLAGS) $< \
		| sed 's,\($(*F)\)\.o[ :]*,$*.o $*.d : ,g' > $*.d; \
		[ -s $@ ] || rm -f $@

c-dep_files := $(foreach n, $(c-deps), $(wildcard $(n)))
-include $(c-dep_files)
cpp-dep_files := $(foreach n, $(cpp-deps), $(wildcard $(n)))
-include $(cpp-dep_files)

.PHONY: all clean depend $(SUBDIRS) __SUBDIR__
