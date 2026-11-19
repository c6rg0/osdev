#ifndef IDT_H
#define IDT_H

#include <stdbool.h>
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 32

extern "C" {
extern void* isr_stub_table [];
__attribute__((noreturn)) void exception_handler(void);
}

namespace Sys
{

typedef struct {
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into
                        // CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attributes; see the IDT page
    uint16_t isr_high;  // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

/*
 * I don't want this class to be constructed more than once,
 * because exception_handler() will be used multiple times
 */
class Idt {
public:
    Idt(void);

private:
    // Create an array of IDT entries aligned for performance
    __attribute__((aligned(0x10))) static idt_entry_t idt [256];
    static bool vectors [IDT_MAX_DESCRIPTORS];
    void set_descriptor(uint8_t vector, void* isr, uint8_t flags);
};

}

#endif
