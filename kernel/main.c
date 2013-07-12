#include <system.h>
#include <vga.h>
#include <mem.h>
#include <multiboot.h>
#include <version.h>

#include <stdint.h>

extern void _asm_print_test();

void kmain(struct multiboot_info *mbt)
{
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    syscalls_install();

    vga_init();
    kprintf(__kernel_name " v" __kernel_version_str "\n\n");

    uint64_t mem;
    get_multiboot_info(mbt, &mem);

    extern uint32_t _kernel_memory_end[];
    /**
    kprintf("End of kernel's memory: 0x%x\n", (uint64_t) (uint32_t) _kernel_memory_end);
    kprintf("Memory:\n%l B\n%l KB\n%l MB\n%l GB\n", mem, mem / 1024, mem / 1024 / 1024, mem / 1024 / 1024 / 1024);
    /**/

    
    init_paging();
    map_page(0x003000, 0x60000, 3);
    int *p = (int *) 0x003000;
    *p = 12;
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p, *p);

    map_page(0x2000, 0x60000, 3);
    int *p2 = (int *) 0x2000;
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p2, *p2);

    _asm_print_test();
    /*  code for function:
     *
     *   _asm_print_test:
     *       mov eax, 0x04  ; write
     *       mov ebx, 0x01  ; to terminal
     *       mov ecx, hello ; the buffer to write from
     *       int 0x80
     *
     *   [...]
     *
     *   section .data
     *   hello: db "Hello, World!", 0x0a
     * 
     */

    return;
}