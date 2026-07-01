CC := i686-elf-gcc
AA := i686-elf-as
BUILDDIR ?= build
PREFIX ?= $(BUILDDIR)

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LFLAGS := -ffreestanding -O2 -nostdlib -lgcc


SRCS_S := $(wildcard src/*.s)
SRCS_C := $(wildcard src/*.c)
OBJS   := $(SRCS_S:src/%.s=$(BUILDDIR)/%.o) $(SRCS_C:src/%.c=$(BUILDDIR)/%.o) 

$(BUILDDIR)/%.o: src/%.s | $(BUILDDIR)
	$(AA) $< -o $@

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/kernel.elf: $(OBJS) src/linker.ld | $(BUILDDIR)
	$(CC) -T src/linker.ld -o $@ $(OBJS) $(LFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

install: $(BUILDDIR)/kernel.elf
	mkdir -p $(PREFIX)
	cp $(BUILDDIR)/kernel.elf $(PREFIX)/

.PHONY: install clean
clean:
	rm -rf $(BUILDDIR)
