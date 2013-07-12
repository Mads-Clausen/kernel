/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

MULTIBOOT_MAGIC = 0x1badb002;
MULTIBOOT_MEMORY_INFO = 2;
MULTIBOOT_FLAGS = MULTIBOOT_MEMORY_INFO;
MULTIBOOT_CHECKSUM = -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS);

PAGE_SIZE = 0x1000;

SECTIONS
{   
    . = 1M;

    .multiboot ALIGN(4) :
    {
        /* multiboot header should fit in the first 8KiB */
        LONG(MULTIBOOT_MAGIC);
        LONG(MULTIBOOT_FLAGS);
        LONG(MULTIBOOT_CHECKSUM);
    }

    /* First put the multiboot header, as it is required to be put very early
       early in the image or the bootloader won't recognize the file format.
       Next we'll put the .text section. */
    .text ALIGN(4K) :
    {
        *(.text)
    }

    /* Read-only data. */
    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data ALIGN(4K) :
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }

    /DISCARD/ : {
        *(.comment .note* .eh_frame .dtors) /* FIXME: eh_frame is needed for dwarf debug info! */
    }

    _kernel_memory_end = ALIGN(PAGE_SIZE);

    /* The compiler may produce other sections, by default it will put them in
       a segment with the same name. Simply add stuff here as needed. */
}