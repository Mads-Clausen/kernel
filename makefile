CC   	  = gcc
CFLAGS    = -m32 -c -std=gnu99 -ffreestanding -O0 -I./kernel/include

LINKER    = gcc
LINKFILE  = link.ld
BIN       = grub/boot/kernel.bin
LFLAGS    = -Wl,-Map,map.txt -m32 -T link.ld -o $(BIN) -ffreestanding -O2 -nostdlib -lgcc

ISO       = kernel.iso
BURNER    = grub-mkrescue -o $(ISO) ./grub
EMULATOR  = qemu-system-i386

HEADERS   = \
kernel/include/vga.h \
kernel/include/syscall.h \
kernel/include/version.h \
kernel/include/system.h \
kernel/include/mem.h \
kernel/include/multiboot.h

OBJS      = \
obj/idt.o \
obj/irq.o \
obj/gdt.o \
obj/main.o \
obj/system.o \
obj/syscall.o \
obj/vga.o \
obj/mem.o \
obj/multiboot.o \
obj/start.o \
obj/bootload.o

default: $(OBJS)
	$(LINKER) $(OBJS) $(LFLAGS)
	# $(BURNER)

run: default
	$(EMULATOR) -kernel $(BIN)

obj/idt.o: kernel/cpu/idt.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/irq.o: kernel/cpu/irq.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/gdt.o: kernel/cpu/gdt.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/main.o: kernel/main.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/system.o: kernel/sys/system.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/syscall.o: kernel/sys/syscall.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/vga.o: kernel/video/vga.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/mem.o: kernel/sys/mem.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/multiboot.o: kernel/boot/multiboot.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

obj/bootload.o: kernel/boot/bootload.s $(HEADERS)
	as --32 $< -o $@

obj/start.o: kernel/start.s $(HEADERS)
	nasm -felf $< -o $@
