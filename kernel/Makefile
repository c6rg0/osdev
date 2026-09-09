DESTDIR?= # $TMPDIR passed by flake
# The below diverge from config.sh
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS := $(CFLAGS) -g -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Isrc/ -MMD -MP -fcommon -mgeneral-regs-only $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -Iinclude $(KERNEL_ARCH_CPPLAGS)
LDFLAGS := -L$(SYSROOT)/usr/lib $(KERNEL_ARCH_LDFLAGS)
LIBS := -nostdlib -lk -lgcc $(KERNEL_ARCH_LIBS)
ARCHDIR = arch/$(HOST)

include $(ARCHDIR)/make.config

CCODE := $(shell find kernel/ $(ARCHDIR) -type f -name '*.c')
SCODE := $(shell find $(ARCHDIR) -type f -name '*.S')
SCODE := $(filter-out $(ARCHDIR)/crti.S, $(SCODE))
SCODE := $(filter-out $(ARCHDIR)/crtn.S, $(SCODE))

KERNEL_OBJS = $(CCODE:.c=.o) $(SCODE:.S=.o)

OBJS = \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST = \
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

DEPS = OBJS:.o=.d

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .h .S

all: myos.elf

myos.elf: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(LDFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.elf

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f myos.elf
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -r --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: myos.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.elf $(DESTDIR)$(BOOTDIR)

-include $(DEPS)
