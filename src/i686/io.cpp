#include <sys/io.h>

// https://en.wikipedia.org/wiki/Memory-mapped_I/O_and_port-mapped_I/O
// https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access

namespace Sys
{

uint8_t Io::inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void Io::outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void Io::cli(void)
{
    __asm__ volatile("cli");
}

void Io::wait(void)
{
    outb(0x80, 0);
}

bool Io::are_interrupts_enabled()
{
    unsigned long flags;
    __asm__ volatile("pushf\n\t"
                     "pop %0"
                     : "=g"(flags));
    return flags & (1 << 9);
}

Io io;

}
