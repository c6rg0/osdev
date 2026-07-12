#include "interupts.h"

extern void clear_interupt_flag(void)
{
    __asm__ volatile ("cli");
}
