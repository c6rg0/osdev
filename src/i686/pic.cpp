#include <stdint.h>
#include <sys/io.h>
#include <sys/pic.h>

namespace Sys
{

Pic::Pic(int offset1, int offset2)
{
    Sys::io.outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the
                                                       // initialization
                                                       // sequence (in cascade
                                                       // mode)
    Sys::io.wait();
    Sys::io.outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    Sys::io.wait();
    Sys::io.outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
    Sys::io.wait();
    Sys::io.outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
    Sys::io.wait();
    Sys::io.outb(PIC1_DATA, 1 << CASCADE_IRQ); // ICW3: tell Master PIC that
                                               // there is a slave PIC at IRQ2
    Sys::io.wait();
    Sys::io.outb(PIC2_DATA, CASCADE_IRQ); // ICW3: tell Slave PIC its cascade
                                          // identity
    Sys::io.wait();

    Sys::io.outb(PIC1_DATA, ICW4_8086); // ICW4: have the PICs use 8086 mode
                                        // (and not 8080 mode)
    Sys::io.wait();
    Sys::io.outb(PIC2_DATA, ICW4_8086);
    Sys::io.wait();

    // Unmask both PICs.
    Sys::io.outb(PIC1_DATA, 0);
    Sys::io.outb(PIC2_DATA, 0);
}

void Pic::disable(void)
{
    Sys::io.outb(PIC1_DATA, 0xff);
    Sys::io.outb(PIC2_DATA, 0xff);
}

void Pic::send_eoi(uint8_t irq)
{
    if (irq >= 8)
        Sys::io.outb(PIC2_COMMAND, PIC_EOI);

    Sys::io.outb(PIC1_COMMAND, PIC_EOI);
}

void Pic::set_irq_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = Sys::io.inb(port) | (1 << irq_line);
    Sys::io.outb(port, value);
}

void Pic::clear_irq_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = Sys::io.inb(port) & ~(1 << irq_line);
    Sys::io.outb(port, value);
}

uint16_t Pic::get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    Sys::io.outb(PIC1_COMMAND, ocw3);
    Sys::io.outb(PIC2_COMMAND, ocw3);
    return (Sys::io.inb(PIC2_COMMAND) << 8) | Sys::io.inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t Pic::get_irr(void)
{
    return get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t Pic::get_isr(void)
{
    return get_irq_reg(PIC_READ_ISR);
}

}
