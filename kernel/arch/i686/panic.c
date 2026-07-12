#include <kernel/panic.h>
#include "interupts.h"
#include <stdio.h>

void kernel_halt(void)
{
    for (;;){
        __asm__ volatile ("hlt");
    }
}

void kernel_panic(void)
{
    clear_interupt_flag();
    printf("KERNEL PANIC: you are on your own now (good luck)");
    kernel_halt();
}
