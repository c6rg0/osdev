#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint16_t*) 0xB8000

namespace Sys
{

class Cursor {
public:
    /*
        "The highest scanline is 0 and the lowest scanline
        is the maximum scanline (usually 15)."
    */

    Cursor(void);
    void enable(uint8_t cursor_start, uint8_t cursor_end);
    void disable(void);
    void move(int x, int y); // zero indexed
    uint16_t get_position(void);
};

class Terminal {
public:
    Terminal(void);
    void write_string(const char* data);
    void put_char(char c);

private:
    Cursor cursor;

    size_t terminal_row = 0;
    size_t terminal_column = 0;
    uint8_t terminal_colour;
    uint16_t* terminal_buffer;

    void set_colour(uint8_t colour);
    void put_entry_at(unsigned char c, uint8_t colour, size_t x, size_t y);
};

extern Terminal terminal;

}

#endif
