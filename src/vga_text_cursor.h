#ifndef VGA_TEXT_CURSOR_H
#define VGA_TEXT_CURSOR_H

#include "io.h"

void disable_cursor(void);
void enable_cursor(void);

#define VGA_WIDTH 80
void move_cursor(int x, int y); // coords are zero indexed
                                //
uint16_t get_cursor_position(void);

#endif
