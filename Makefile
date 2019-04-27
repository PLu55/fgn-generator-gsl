PROJPATH := $(shell pwd)

INSTALLPATH := /usr/local
INSTALLLIB = $(INSTALLPATH)/lib
INSTALLINC = $(INSTALLPATH)/include

# Probe environment and decide compiler setup
include Makefile.platform

DLIB_SUFFIX = .so

export PROJPATH
LIBS = -lm -lgsl -lfftw3
DLDFLAGS += -rdynamic -shared

SUBDIRS := . 
CSUBDIRS := .
TESTSUBDIRS := $(addsuffix /test,$(SUBDIRS))
ALLSUBDIRS := $(SUBDIRS) $(TESTSUBDIRS)

CSRCS := fgn.c 
CSRCS += spectrum.c

HDRS := fgn.h
BIN := fgntest$(BIN_SUFFIX)
LIB := libfgn$(LIB_SUFFIX)
DLIB := libfgn$(DLIB_SUFFIX)

OBJS := $(CSRCS:.c=$(OBJ_SUFFIX))
CSRC = $(wildcard *.c)
OBJS += $(CSRC:.c=$(OBJ_SUFFIX))
CFLAGS += -fPIC
CFLAGS += -O3
CFLAGS += -mfpmath=sse
CFLAGS += -msse3

ifeq ($(wildcard "Makefile.local"), "Makefile.local")
include Makefile.local
endif

.PHONY: all dlib clean clean-custom

all: $(LIB) dlib $(BIN)

dlib: $(DLIB)

$(LIB): $(OBJS)
	$(AR) $(ARCRFLAGS)$@ $^ 

$(DLIB): $(OBJS)
	$(CC) -o $@  $^ $(DLDFLAGS) $(LIBS)

$(BIN): $(OBJS)
	$(CC) $(EXEOUT)$@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o *.obj *.*deps Makefile.mf *~ *.pdb *.gcno gmon.out *.gcov
	rm -f *.so
	rm -f test/*.o test/*.obj  test/*.*deps
	rm -f \#*\#
	rm -f $(BIN) $(LIB)

install: $(LIB) $(DLIB)
	install $(HDRS) $(INSTALLINC)
	install $(LIB) $(INSTALLLIB)
	install $(DLIB) $(INSTALLLIB)

%$(OBJ_SUFFIX) : %.c
	$(CC) $(CFLAGS) -c $< $(OBJOUT)$@ 

DEPS += $(SRC:.c=.cdeps)

ifneq ($(UVASBUILDER), "visualc++")
%.cdeps : %.c
	$(CC) -MM $< -MF $@ $(CINCS)

-include $(CSRC:.c=.cdeps)
endif
