#!/usr/bin/env bash

i686-elf-as src/boot.s -o target/boot.o
i686-elf-gcc -c src/kernel.c -o target/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T src/linker.ld -o target/myos -ffreestanding -O2 -nostdlib target/boot.o target/kernel.o -lgcc

if grub-file --is-x86-multiboot target/myos; then
  echo "Multiboot confirmed"
else
  echo "The file is not multiboot"
  exit
fi

mv target/myos target/isodir/boot/myos
grub-mkrescue -o target/myos.iso target/isodir
