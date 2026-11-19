DESTDIR?= # $TMPDIR passed by flake
# The below diverge from config.sh
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -g -std=gnu11 -ffreestanding -O2 -Wall -Wextra $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude -Isrc/ -fno-exceptions -fno-rtti $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk $(KERNEL_ARCH_CPPFLAGS)
ARCHDIR=$(HOST)

include $(ARCHDIR)/make.config

CCODE := $(shell find stdio/ stdlib/ string/ $(ARCHDIR) -type f -name '*.c')
CPPCODE := $(shell find stdio/ stdlib/ string/ $(ARCHDIR) -type f -name '*.cpp')
FREEOBJS = $(CCODE:.c=.o) $(CPPCODE:.cpp=.o)

# "Hosted" is the opposite of "freestanding"
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.cpp.o:
	$(CXX) -MD -c $< -o $@ $(CPPFLAGS)

.cpp.S:
	$(CXX) -MD -c $< -o $@ $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)

.cpp.libk.o:
	$(CXX) -MD -c $< -o $@ $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
