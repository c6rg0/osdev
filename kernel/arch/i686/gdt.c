#include <kernel/gdt.h>
#include <kernel/panic.h>
#include "interupts.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint64_t gdt[6];

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor = limit & 0x000F0000;            // set limit bits 19:16
    descriptor |= (flag << 8) & 0x00F0FF00;    // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;    // set base bits 23:16
    descriptor |= base & 0xFF000000;            // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;

    // Create the low 32 bit segment
    descriptor |= base << 16;          // set base bits 15:0
    descriptor |= limit & 0x0000FFFF;  // set limit bits 15:0
    return descriptor;
}

tss_struct tss_entry;

void gdt_initialise(void)
{
    clear_interupt_flag();
    gdt[0] = create_descriptor(0, 0, 0); // Null descriptor
    gdt[1] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    gdt[2] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    gdt[3] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    gdt[4] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

    memset(&tss_entry, 0, sizeof(tss_entry));
    tss_entry.ss0 = 0x10;  // kernel data segment selector (index 2 * 8)
    gdt[5] = create_descriptor((uint32_t)&tss_entry, sizeof(tss_entry) - 1, (GDT_TSS));

    struct gdt_ptr ptr;
    ptr.limit = sizeof(gdt) - 1;
    ptr.base = (uint32_t)&gdt;

    if(!set_gdt(&ptr) ||
        !reload_segments() ||
        !load_tss())
        kernel_panic();
}
