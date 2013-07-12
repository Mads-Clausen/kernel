.section .bootstrap_stack
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
_start:
	movl $stack_top, %esp
	push %ebx
	call kmain
	# call _asm_print_test
	# ^ general protection fault
	cli

hang:
	hlt
	jmp hang
