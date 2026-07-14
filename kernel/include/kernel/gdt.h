#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// Each define here is for a specific flag in the descriptor:
// https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html (iirc chapter 3.4/5)
#define SEG_DESCTYPE(x)     ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)         ((x) << 0x07) // Present
#define SEG_SAVL(x)         ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)         ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)         ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)         ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)         (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)

#define SEG_DATA_RD         0x00 // Read-Only
#define SEG_DATA_RDA        0x01 // Read-Only, accessed
#define SEG_DATA_RDWR       0x02 // Read/Write
#define SEG_DATA_RDWRA      0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD     0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA    0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD   0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA  0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX         0x08 // Execute-Only
#define SEG_CODE_EXA        0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD       0x0A // Execute/Read
#define SEG_CODE_EXRDA      0x0B // Execute/Read, accessed
#define SEG_CODE_EXC        0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA       0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC      0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA     0x0F // Execute/Read, conforming, accessed
                                 
#define TSS_ACCESS_BYTE     0x89

#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
    SEG_LONG(0)     | SEG_SIZE(1) |     SEG_GRAN(1) | \
    SEG_PRIV(0)     | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
    SEG_LONG(0)     | SEG_SIZE(1) |     SEG_GRAN(1) | \
    SEG_PRIV(0)     | SEG_DATA_RDWR

#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
    SEG_LONG(0)     | SEG_SIZE(1) |     SEG_GRAN(1) | \
    SEG_PRIV(3)     | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
    SEG_LONG(0)     | SEG_SIZE(1) |     SEG_GRAN(1) | \
    SEG_PRIV(3)     | SEG_DATA_RDWR

#define GDT_TSS SEG_DESCTYPE(0) | SEG_PRES(1) | SEG_SAVL(0) | \
    SEG_LONG(0)     | SEG_SIZE(1) |     SEG_GRAN(0) | \
    SEG_PRIV(0)     | TSS_ACCESS_BYTE

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct __tss_struct {
    uint32_t prev_tss;   // Previous TSS (used with hardware task switching, unused here)
    uint32_t esp0;        // Stack pointer to load on ring 0 interrupt/privilege switch
    uint32_t ss0;         // Stack segment to load on ring 0 interrupt/privilege switch
    uint32_t esp1, ss1, esp2, ss2;
    uint32_t cr3;
    uint32_t eip, eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_struct;

// gdt_asm.S
int set_gdt(struct gdt_ptr* ptr);
int reload_segments(void);
int load_tss(void);

// gdt.c
uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void gdt_initialise(void);

#endif
