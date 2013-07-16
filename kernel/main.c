#include <system.h>
#include <vga.h>
#include <mem.h>
#include <multiboot.h>
#include <version.h>

#include <stdint.h>

extern void _asm_print_test();

void kmain(struct multiboot_info *mbt)
{
    vga_init();

    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    syscalls_install();

    puts_c(__kernel_name " kernel v" __kernel_version_str "\n\n", COLOR_LIGHT_BLUE, COLOR_DEFAULT_BG);

    uint64_t mem;
    get_multiboot_info(mbt, &mem);

    extern uint32_t _kernel_memory_end[];
    kprintf("End of kernel's memory: 0x%x\n", (uint64_t) (uint32_t) _kernel_memory_end);
    kprintf("Memory:\n%l B\n%l KB\n%l MB\n%l GB\n", mem, mem / 1024, mem / 1024 / 1024, mem / 1024 / 1024 / 1024);
    
    init_paging();
    map_page(0xFD7FF000, 0x60000, 3);
    int *p = (int *) 0xFD7FF000;
    *p = 12;
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p, (uint64_t) *p);

    map_page(0x10000000, 0x60000, 3);
    int *p2 = (int *) 0x10000000;
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p2, (uint64_t) *p2);

    uint32_t addr = allocate_page();
    uint32_t pdindex = addr >> 22;
    uint32_t ptindex = addr >> 12 & 0x3FF;
    kprintf("Allocated page: 0x%x, pdi: %u, pti: %u\n", (uint64_t) addr, (uint64_t) pdindex, (uint64_t) ptindex);

    _asm_print_test();
    /*  code for function:
     *
     *  section .text
     *      global _asm_print_test
     *
     *  _asm_print_test:
     *      mov eax, 0x04  ; write
     *      mov ebx, 0x01  ; to terminal
     *      mov ecx, hello ; the buffer to write from
     *      int 0x80
     *
     *  [...]
     *
     *  section .rodata
     *  hello: db "Hello, World!", 0x0a
     */

    kprintf("\n");

    return;
}