#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdint.h>

// Read & write values to/from ports
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);
extern void cli(void);
extern void io_wait(void);
extern bool are_interrupts_enabled();

#endif
