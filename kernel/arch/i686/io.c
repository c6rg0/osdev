#include <kernel/io.h>

// https://en.wikipedia.org/wiki/Memory-mapped_I/O_and_port-mapped_I/O
// https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access

extern inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

extern inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
