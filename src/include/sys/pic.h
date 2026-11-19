#ifndef PIC_H
#define PIC_H

#include <stdint.h>

namespace Sys
{

class Pic {
public:
    // TODO: I don't know which of these should be public or
    // private

    /*
    Arguments:
        offset1 - vector offset for master PIC
            vectors on the master become offset1..offset1+7
        offset2 - same for slave PIC: offset2..offset2+7
    */
    Pic(int offset1, int offset2);

    void disable(void);

    /*
     * Reinitialize the PIC controllers, giving them specified
     * vector offsets rather than 8h and 70h, as configured by
     * default
     */
    void send_eoi(uint8_t irq);

    /*
     * Returns the combined value of the cascaded PICs irq
     * request register
     */
    uint16_t get_irr(void);

    /*
     * Returns the combined value of the cascaded PICs
     * in-service register
     */
    uint16_t get_isr(void);

    void set_irq_mask(uint8_t irq_line);
    void clear_irq_mask(uint8_t irq_line);

public:
    uint16_t get_irq_reg(int ocw3);
};

}

#define PIC1         0x20 // IO base address for master PIC
#define PIC2         0xA0 // IO base address for slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)
#define PIC_EOI      0x20 // End-of-interrupt command code

#define ICW1_ICW4      0x01 // Indicates that ICW4 will be present
#define ICW1_SINGLE    0x02 // Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL     0x08 // Level triggered (edge) mode
#define ICW1_INIT      0x10 // Initialization - required!

#define ICW4_8086       0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM       0x10 // Special fully nested (not)

#define CASCADE_IRQ 2

#define PIC_READ_IRR 0x0a // OCW3 irq ready next CMD read
#define PIC_READ_ISR 0x0b // OCW3 irq service next CMD read

#endif
