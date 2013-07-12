#ifndef __SYSCALL_H_INCLUDED__
#define __SYSCALL_H_INCLUDED__

#include <system.h>

/** Install syscalls to the local table, and the syscall handler to the IDT */
void syscalls_install();

/** Handle system calls */
void syscall_handler(struct regs *r);

#endif