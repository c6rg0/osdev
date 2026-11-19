#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/io.h>
#include <sys/tty.h>
#include "vga.h"

namespace Sys
{

Terminal::Terminal(void)
{
    terminal_colour = vga_entry_colour(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    terminal_buffer = VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer [index] = vga_entry(' ', terminal_colour);
        }
    }

    cursor.move(terminal_column, terminal_row);
}

void Terminal::set_colour(uint8_t colour)
{
    terminal_colour = colour;
}

void Terminal::put_entry_at(unsigned char c, uint8_t colour, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer [index] = vga_entry(c, colour);
}

void Terminal::put_char(char c)
{
    unsigned char uc = c;

    if (uc == '\n') {
        terminal_column = 0;
        terminal_row++;
    }

    else {
        put_entry_at(uc, terminal_colour, terminal_column, terminal_row);
        cursor.move(terminal_column, terminal_row);

        // The below handles out of bounds + 1.
        // ++var is pre-incrementing; increments and uses.
        if (++terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }

        if (terminal_row >= VGA_HEIGHT) {
            for (size_t row = 0; row < (VGA_HEIGHT + 1); row++) {
                for (size_t column = 0; column < VGA_WIDTH; column++) {
                    const size_t index = row * VGA_WIDTH + column;
                    terminal_buffer [index - VGA_WIDTH] =
                        terminal_buffer [index];
                }
            }
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void Terminal::write_string(const char* data)
{
    for (size_t i = 0; i < strlen(data); i++)
        put_char(data [i]);
}

Cursor::Cursor(void)
{
    enable(0, 0);
}

void Cursor::enable(uint8_t x, uint8_t y)
{
    Sys::io.outb(0x3D4, 0x0A);
    Sys::io.outb(0x3D5, (Sys::io.inb(0x3D5) & 0xC0) | x);

    Sys::io.outb(0x3D4, 0x0B);
    Sys::io.outb(0x3D5, (Sys::io.inb(0x3D5) & 0xE0) | y);
}

void Cursor::disable(void)
{
    Sys::io.outb(0x3D4, 0x0A);
    Sys::io.outb(0x3D5, 0x20);
}

void Cursor::move(int column, int row)
{
    uint16_t pos = row * VGA_WIDTH + column;

    Sys::io.outb(0x3D4, 0x0F);
    Sys::io.outb(0x3D5, (uint8_t) (pos & 0xFF));
    Sys::io.outb(0x3D4, 0x0E);
    Sys::io.outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t Cursor::get_position(void)
{
    uint16_t pos = 0;
    Sys::io.outb(0x3D4, 0x0F);
    pos |= Sys::io.inb(0x3D5);
    Sys::io.outb(0x3D4, 0x0E);
    pos |= ((uint16_t) Sys::io.inb(0x3D5)) << 8;
    return pos;
}

Terminal terminal;

}
