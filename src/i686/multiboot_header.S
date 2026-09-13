// Declaring constants for multiboot header
.set ALIGN,    1<<0             // Align loaded modules on page boundaries
.set MEMINFO,  1<<1             // Provide memory map
.set FLAGS,    ALIGN | MEMINFO  // Multiboot flag field
.set MAGIC,    0x1BADB002       // ELF magic number lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) // Checksum of above to prove we are multiboot

// Declaring a multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
