#ifndef __PROC_H_INCLUDED__
#define __PROC_H_INCLUDED__

#include <stdint.h>
#include <system.h>
#include <mem.h>

struct kthread_context
{
    uint32_t eax, ebx, ecx, edx, esp, dbp, esi, edi;
    uint32_t gs, fs, es, ds;
} __attribute__ ((packed));

struct kthread
{
	struct kthread_context context;
	void (*func)();
};

/** Create a kthread and initialise it with a 4K stack */
void create_kthread(void (*func)(), struct kthread *thread);

/** Start a kthread. It will be ended once done */
void start_kthread(struct kthread *thread);

#endif