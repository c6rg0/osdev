#ifndef IO_H
#define IO_H

#include <stdint.h>

// Read & write values to/from ports
extern inline uint8_t inb(uint16_t port);
extern inline void outb(uint16_t port, uint8_t val);

#endif
