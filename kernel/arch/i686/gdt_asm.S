.section .data
.align 4
gdtr:
    .word 0 // For limit storage
    .long 0 // For base storage

.section .text
.global set_gdt
.type set_gdt, @function
set_gdt:
    mov 4(%esp), %eax
    lgdt (%eax)
    ret

.global reload_segments
.type reload_segments, @function
reload_segments:
    // Reload CS register containing code selector:
    ljmp $0x08, $.reload_CS // 0x08 is a stand-in for your code segment
    ret

.type reload_CS, @function
.reload_CS:
    // Reload data segment registers:
    mov $0x10, %ax // 0x10 is a stand-in for your data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
