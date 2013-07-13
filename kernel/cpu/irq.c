#include <system.h>

extern char _irq0[];
extern char _irq1[];
extern char _irq2[];
extern char _irq3[];
extern char _irq4[];
extern char _irq5[];
extern char _irq6[];
extern char _irq7[];
extern char _irq8[];
extern char _irq9[];
extern char _irq10[];
extern char _irq11[];
extern char _irq12[];
extern char _irq13[];
extern char _irq14[];
extern char _irq15[];

void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void __irq_remap(int offset1, int offset2)
{
    uint8_t master_mask = 0;
    uint8_t slave_mask = 0;
    outb(PIC_MASTER + PIC_COMMAND,  PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb(PIC_SLAVE  + PIC_COMMAND,  PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb(PIC_MASTER + PIC_DATA,     offset1);
    outb(PIC_SLAVE  + PIC_DATA,     offset2);
    outb(PIC_MASTER + PIC_DATA,     0x04); /* slave PIC at IRQ2 */
    outb(PIC_SLAVE  + PIC_DATA,     0x02); /* cascade identity */
    outb(PIC_MASTER + PIC_DATA,     PIC_MODE_8086);
    outb(PIC_SLAVE  + PIC_DATA,     PIC_MODE_8086);
    outb(PIC_MASTER + PIC_DATA,     master_mask);
    outb(PIC_SLAVE  + PIC_DATA,     slave_mask);
}

void __send_eoi(uint8_t irq)
{
    if(irq >= 8)
        outb(PIC_SLAVE + PIC_COMMAND, PIC_EOI);

    outb(PIC_MASTER + PIC_COMMAND, PIC_EOI);
}

void irq_install()
{
    __irq_remap(32, 40); /* remap to first free IDT entry */

    idt_set_gate(32, (uint32_t) _irq0,  0x08, 0x8e);
    idt_set_gate(33, (uint32_t) _irq1,  0x08, 0x8e);
    idt_set_gate(34, (uint32_t) _irq2,  0x08, 0x8e);
    idt_set_gate(35, (uint32_t) _irq3,  0x08, 0x8e);
    idt_set_gate(36, (uint32_t) _irq4,  0x08, 0x8e);
    idt_set_gate(37, (uint32_t) _irq5,  0x08, 0x8e);
    idt_set_gate(38, (uint32_t) _irq6,  0x08, 0x8e);
    idt_set_gate(39, (uint32_t) _irq7,  0x08, 0x8e);
    idt_set_gate(40, (uint32_t) _irq8,  0x08, 0x8e);
    idt_set_gate(41, (uint32_t) _irq9,  0x08, 0x8e);
    idt_set_gate(42, (uint32_t) _irq10, 0x08, 0x8e);
    idt_set_gate(43, (uint32_t) _irq11, 0x08, 0x8e);
    idt_set_gate(44, (uint32_t) _irq12, 0x08, 0x8e);
    idt_set_gate(45, (uint32_t) _irq13, 0x08, 0x8e);
    idt_set_gate(46, (uint32_t) _irq14, 0x08, 0x8e);
    idt_set_gate(47, (uint32_t) _irq15, 0x08, 0x8e);

    asm volatile("sti"); /* set interrupt flag */
}

void irq_install_handler(uint8_t i, void (*h)(struct regs *r))
{
    irq_routines[i] = h;
}


void _irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);

    handler = irq_routines[r->int_no - 32];
    if(handler)
    {
        handler(r);
    }

    if(r->int_no >= 40)
        outb(PIC_SLAVE + PIC_COMMAND, PIC_EOI);

    outb(PIC_MASTER + PIC_COMMAND, PIC_EOI);
}


int irq_enabled()
{
    int f;
    asm volatile ("pushf\n\t" "popl %0" : "=g"(f));
    return f & (1 << 9);
}