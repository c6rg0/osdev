#include <stdbool.h>
#include <sys/idt.h>

namespace Sys
{

idt_entry_t Idt::idt [256];
bool Idt::vectors [IDT_MAX_DESCRIPTORS];

Idt::Idt()
{
    idtr_t idtr;
    idtr.base = (uintptr_t) &idt [0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        set_descriptor(vector, isr_stub_table [vector], 0x8E);
        vectors [vector] = true;
    }

    // load the new IDT
    __asm__ volatile("lidt %0" : : "m"(idtr));

    // set the interrupt flag
    __asm__ volatile("sti");
}

void Idt::set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idt_entry_t* descriptor = &idt [vector];

    descriptor->isr_low = (uint32_t) isr & 0xFFFF;
    descriptor->kernel_cs = 0x08; // this value can be whatever offset your
                                  // kernel code selector is in your GDT
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t) isr >> 16;
    descriptor->reserved = 0;
}

}

void exception_handler(void) {
    __asm__ volatile ("cli; hlt");
    __builtin_unreachable();
}
