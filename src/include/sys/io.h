#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdint.h>

namespace Sys
{

class Io {
public:
    uint8_t inb(uint16_t port);
    void outb(uint16_t port, uint8_t val);
    void cli(void);
    void wait(void);
    bool are_interrupts_enabled();
};

extern Io io;

}

#endif
