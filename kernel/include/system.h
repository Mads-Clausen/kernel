#ifndef __SYSTEM_H_INCLUDED__
#define __SYSTEM_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include <vga.h>

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define PIC_MASTER          0x20
#define PIC_SLAVE           0xa0
#define PIC_COMMAND         0x00
#define PIC_DATA            0x01
#define PIC_EOI             0x20
#define PIC_CMD_ENDINTR     0x20
#define PIC_ICW1_ICW4       0x01
#define PIC_ICW1_SINGLE     0x02
#define PIC_ICW1_INTERVAL4  0x04
#define PIC_ICW1_LEVEL      0x08
#define PIC_CMD_INIT        0x10
#define PIC_MODE_8086       0x01
#define PIC_MODE_AUTO       0x02
#define PIC_MODE_BUF_SLAVE  0x08
#define PIC_MODE_BUF_MASTER 0x0c
#define PIC_MODE_SFNM       0x10

struct regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

/* I/O functions */
uint8_t  inb(uint16_t port);
void    outb(uint16_t port, uint8_t val);
void io_wait(void);
void   cpuid(int code, uint32_t *a, uint32_t *d);
void   wrmsr(uint32_t msr_id, uint64_t msr_value);

/** Install the GDT */
void gdt_install(void);

/** Install the IDT */
void idt_install(void);

/** Set an entry in the IDT */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

/** Install the ISRs into the IDT */
void isr_install(void);

/** Reprogram the PICs and install the IRQs */
void irq_install(void);

/** Install an IRQ handler */
void irq_install_handler(uint8_t index, void (*handler)(struct regs *));

/** Install syscalls */
void syscalls_install(void);

/** Dump the registers to the terminal */
void dump_regs(struct regs *r);

/** See glibc documentation */
int   strlen(char *s);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t count);

void  kprintf(char *format, ...);
void ksprintf(char *str, char *format, ...);

#endif