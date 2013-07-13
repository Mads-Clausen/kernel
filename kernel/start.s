section .text
extern idtp
global _load_idt
_load_idt:
    lidt [idtp]
    ret

global _gdt_flush
extern gp
_gdt_flush:
    lgdt [gp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; interrupt handlers
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

; syscalls
global _isr80

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 0: Divide By Zero Exception
_isr0:
    cli
    push byte 0
    push byte 0
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 1: Debug Exception
_isr1:
    cli
    push byte 0
    push byte 1
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 2: Non-maskable interrupt exception
_isr2:
    cli
    push byte 0
    push byte 2
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 3: Breakpoint exception
_isr3:
    cli
    push byte 0
    push byte 3
    jmp _isr_common


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 4: Into detected overflow exception
_isr4:
    cli
    push byte 0
    push byte 4
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 5: Out of bounds exception
_isr5:
    cli
    push byte 0
    push byte 5
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 6: Invalid opcode exception
_isr6:
    cli
    push byte 0
    push byte 6
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 7: No coprocessor exception
_isr7:
    cli
    push byte 0
    push byte 7
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 8: Double fault exception
_isr8:
    cli
    push byte 8
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 9: Coprocessor segment overrun exception
_isr9:
    cli
    push byte 0
    push byte 9
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 10: Bad TSS exception
_isr10:
    cli
    push byte 10
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 11: Segment not present exception
_isr11:
    cli
    push byte 11
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 12: Stack fault exception
_isr12:
    cli
    push byte 12
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 13: General protection fault exception
_isr13:
    cli
    push byte 13
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 14: Page fault exception
_isr14:
    cli
    push byte 14
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 15: Unknown interrupt exception
_isr15:
    cli
    push byte 0
    push byte 15
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 16: Coprocessor fault exception
_isr16:
    cli
    push byte 0
    push byte 16
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 17: Alignment check exception(486+)
_isr17:
    cli
    push byte 0
    push byte 17
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 18: Machine check exception(Pentium/586+)
_isr18:
    cli
    push byte 0
    push byte 18
    jmp _isr_common

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 19-32: Reserved
_isr19:
_isr20:
_isr21:
_isr22:
_isr23:
_isr24:
_isr25:
_isr26:
_isr27:
_isr28:
_isr29:
_isr30:
_isr31:
    cli
    push byte 0
    push byte 19
    jmp _isr_common


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 80: Syscalls
_isr80:
    cli
    push byte 0
    push byte 80
    jmp _isr_common

extern _interrupt_handler

_isr_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _interrupt_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 32: IRQ0
_irq0:
    push byte 0
    push byte 32
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 33: IRQ1
_irq1:
    push byte 0
    push byte 33
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 34: IRQ2
_irq2:
    push byte 0
    push byte 34
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 35: IRQ3
_irq3:
    push byte 0
    push byte 35
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 36: IRQ4
_irq4:
    push byte 0
    push byte 36
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 37: IRQ5
_irq5:
    push byte 0
    push byte 37
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 38: IRQ6
_irq6:
    push byte 0
    push byte 38
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 39: IRQ7
_irq7:
    push byte 0
    push byte 39
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 40: IRQ8
_irq8:
    push byte 0
    push byte 40
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 41: IRQ9
_irq9:
    push byte 0
    push byte 41
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 42: IRQ10
_irq10:
    push byte 0
    push byte 42
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 43: IRQ11
_irq11:
    push byte 0
    push byte 43
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 44: IRQ12
_irq12:
    push byte 0
    push byte 44
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 45: IRQ13
_irq13:
    push byte 0
    push byte 45
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 46: IRQ14
_irq14:
    push byte 0
    push byte 46
    jmp _irq_common_stub


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
; 47: IRQ15
_irq15:
    push byte 0
    push byte 47
    jmp _irq_common_stub

extern _irq_handler

_irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global _read_cr0
_read_cr0:
    mov eax, cr0
    retn

global _write_cr0
_write_cr0:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr0,  eax
    pop ebp
    retn

global _read_cr1
_read_cr1:
    mov eax, cr1
    retn

global _write_cr1
_write_cr1:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr1, eax
    pop ebp
    retn

global _read_cr2
_read_cr2:
    mov eax, cr2
    retn

global _write_cr2
_write_cr2:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr2, eax
    pop ebp
    retn

global _read_cr3
_read_cr3:
    mov eax, cr3
    retn

global _write_cr3
_write_cr3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    retn

global _flush_tlb
_flush_tlb:
    mov eax, cr3
    mov cr3, eax
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global _asm_print_test
_asm_print_test:
    pusha
    mov eax, 0x04
    mov ebx, 0x01
    mov ecx, hello
    int 0x80
    popa
    ret

section .rodata
hello: db "Hello, World!", 0x0a