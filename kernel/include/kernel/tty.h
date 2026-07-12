#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

// Cursor is enabled by default, enable_cursor() is experimental.
// "The highest scanline is 0 and the lowest scanline is the maximum scanline (usually 15)."
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor(void);
void move_cursor(int x, int y); // coords are zero indexed
uint16_t get_cursor_position(void);

void terminal_initialise(void);
extern void terminal_putchar(char c);
extern void terminal_write(const char* data, size_t size);
extern void terminal_writestring(const char* data);

#endif
