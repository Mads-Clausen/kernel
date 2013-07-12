#include <syscall.h>

extern char _isr80[];

void syscalls_install()
{
	idt_set_gate(  80, (uint32_t) _isr80, 0x08, 0x8E);
	idt_set_gate(0x80, (uint32_t) _isr80, 0x08, 0x8E);
}

void syscall_handler(struct regs *r)
{
	switch(r->eax)
	{
		case 0x04: /* write */
		{
			kprintf((char *) r->ecx);

			break;
		}
	}
}