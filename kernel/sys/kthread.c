#include <kthread.h>

extern struct regs last_interrupt_regs;

extern void _set_registers(struct kthread_context *context);
extern void _get_registers(struct kthread_context *context);

void start_kthread(struct kthread *thread);
void end_kthread(struct kthread *thread);

void create_kthread(void (*func)(), struct kthread *thread)
{
    thread->func = func;
    thread->context.eax = thread->context.ebx = thread->context.ecx = thread->context.edx = 0;
    thread->context.gs = last_interrupt_regs.gs;
    thread->context.fs = last_interrupt_regs.fs;
    thread->context.ds = last_interrupt_regs.ds;
    thread->context.es = last_interrupt_regs.es;

    uint32_t stack = allocate_page(0);
    kprintf("Stack = 0x%x\n", (uint64_t) stack);
    map_page(stack, stack, 3);
    thread->context.esp = stack + 0x1000;

    kprintf("Created thread\n");
}

struct kthread_context last_context;

void start_kthread(struct kthread *thread)
{
    /*
    uint32_t esp = 0;

    asm volatile (
        "movl %%esp, %%eax"
        : "=a" (esp)
    );

    kprintf("ESP = 0x%x\n", (uint64_t) esp);
    */

    /*
    extern char kthread_done[];
    kprintf("kthread_done = 0x%x\n", (uint64_t) (uint32_t) kthread_done);
    *((uint32_t *) thread->context.esp) = (uint32_t) kthread_done;
    */

    /*
    asm volatile (
        "pushf                      \n\t"
        "pusha                      \n\t"
        "movl %%esp,%%edx           \n\t"
        "movl %%eax,(%%esp)         \n\t"
        "call *%%ecx                \n\t"
        "kthread_done:              \n\t"
        "movl %%edx,%%esp           \n\t"
        "popa                       \n\t"
        "popf                       \n\t"

        :
        : "a" (thread->context.esp), "c" (thread->func)
    );
    */

    
    asm volatile (
        "call kthread_start_asm"
        :
        : "a" (thread->context.esp), "c" (thread->func), "b" (end_kthread)
        );

    kprintf("Ended thread.");
    return;
}

void end_kthread(struct kthread *thread)
{
    kprintf("Done.\n");
    asm volatile("hlt");
}