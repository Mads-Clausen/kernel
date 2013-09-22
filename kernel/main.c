#include <system.h>
#include <vga.h>
#include <mem.h>
#include <multiboot.h>
#include <version.h>
#include <kthread.h>

#include <stdint.h>

extern void _asm_print_test();

void thread_test()
{
    kprintf("Hello from thread.\n");
}

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
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p, *p);

    map_page(0x10000000, 0x60000, 3);
    int *p2 = (int *) 0x10000000;
    kprintf("*(0x%x) = %i\n", (uint64_t) (uint32_t) p2, *p2);

    print_next_available_page();
    uint32_t ap = allocate_page(203);
    map_page(ap, 0x60000, 3);
    int *p3 = (int *) ap;
    kprintf("*(0x%x) = %i\n", (uint64_t) ap, *p3);

    print_next_available_page();
    ap = allocate_page(203);
    kprintf("ap = 0x%x\n", (uint32_t) ap);

    struct kthread thread;
    create_kthread(thread_test, &thread);
    start_kthread(&thread);
    kprintf("Returned from thread.\n");

    _asm_print_test();

    return;
}