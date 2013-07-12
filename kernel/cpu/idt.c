#include <stddef.h>
#include <stdint.h>

#include <system.h>
#include <syscall.h>

extern char _isr0[];
extern char _isr1[];
extern char _isr2[];
extern char _isr3[];
extern char _isr4[];
extern char _isr5[];
extern char _isr6[];
extern char _isr7[];
extern char _isr8[];
extern char _isr9[];
extern char _isr10[];
extern char _isr11[];
extern char _isr12[];
extern char _isr13[];
extern char _isr14[];
extern char _isr15[];
extern char _isr16[];
extern char _isr17[];
extern char _isr18[];
extern char _isr19[];
extern char _isr20[];
extern char _isr21[];
extern char _isr22[];
extern char _isr23[];
extern char _isr24[];
extern char _isr25[];
extern char _isr26[];
extern char _isr27[];
extern char _isr28[];
extern char _isr29[];
extern char _isr30[];
extern char _isr31[];

/* Defines an IDT entry */
struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void _load_idt();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].always0 = 0;
}

void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t) &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    int i;
    for(i = 32; i < 256; ++i)
    {
        idt_set_gate(i, (uint32_t) _isr31, 0x08, 0x8E);
    }

    /* Points the processor's internal register to the new IDT */
    _load_idt();
}

void isr_install()
{
    idt_set_gate(0,  (uint32_t) _isr0,  0x08, 0x8E);
    idt_set_gate(1,  (uint32_t) _isr1,  0x08, 0x8E);
    idt_set_gate(2,  (uint32_t) _isr2,  0x08, 0x8E);
    idt_set_gate(3,  (uint32_t) _isr3,  0x08, 0x8E);
    idt_set_gate(4,  (uint32_t) _isr4,  0x08, 0x8E);
    idt_set_gate(5,  (uint32_t) _isr5,  0x08, 0x8E);
    idt_set_gate(6,  (uint32_t) _isr6,  0x08, 0x8E);
    idt_set_gate(7,  (uint32_t) _isr7,  0x08, 0x8E);
    idt_set_gate(8,  (uint32_t) _isr8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t) _isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t) _isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t) _isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t) _isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t) _isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t) _isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t) _isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t) _isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t) _isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t) _isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t) _isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t) _isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t) _isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t) _isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t) _isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t) _isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t) _isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t) _isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t) _isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t) _isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t) _isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t) _isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t) _isr31, 0x08, 0x8E);
}

char *exception_msgs[] = 
{
    "Cannot divide by 0.\n",
    "Debug exception.\n",
    "Non maskable interrupt exception.\n",
    "Breakpoint exception.\n",
    "Into detected overflow exception.\n",
    "Out of bounds exception.\n",
    "Invalid opcode exception.\n",
    "No coprocessor exception.\n",
    "Double fault exception.\n",
    "Coprocessor segment overrun exception.\n",
    "Bad TSS exception.\n",
    "Segment not present exception.\n",
    "Stack fault exception.\n",
    "General protection fault exception.\n",
    "Page fault exception: ",
    "Unknown interrupt exception.\n",
    "Coprocessor fault exception.\n",
    "Alignment check exception.\n",
    "Machine check exception.\n"
};

extern uint32_t _read_cr2();

void _interrupt_handler(struct regs *r)
{
    // kprintf("INTERRUPT 0x%x, %u\n", (uint64_t) r->int_no, (uint64_t) r->int_no);
    if(r->int_no < 19)
    {
        puts_c(exception_msgs[r->int_no], COLOR_RED, COLOR_BLACK);
        if(r->int_no == 14) /* page fault */
        {
            if(r->err_code & 1) /* 00001 */
            {
                puts_c("page-protection violation.\n", COLOR_RED, COLOR_BLACK);
            }
            else if(r->err_code & 2) /* 00010 */
            {
                puts_c("error while writing.\n", COLOR_RED, COLOR_BLACK);
            }
            else if(r->err_code & 4) /* 00100 */
            {
                puts_c("CPL = 3.\n", COLOR_RED, COLOR_BLACK);
            }
            else if(r->err_code & 8) /* 01000 */
            {
                puts_c("write to reserved.\n", COLOR_RED, COLOR_BLACK);
            }
            else if(r->err_code & 16) /* 10000 */
            {
                puts_c("instruction fetch.\n", COLOR_RED, COLOR_BLACK);
            }
        }

        kprintf("Error code: 0x%x, %u\n", (uint64_t) r->err_code, (uint64_t) r->err_code);
        kprintf("Error occured at virtual address 0x%x\n", (uint32_t) r->eip);

        asm volatile("hlt");
    }

    switch(r->int_no)
    {
        case 80: case 0x80:
            syscall_handler(r);
            break;
        default:
            break;
    }
}