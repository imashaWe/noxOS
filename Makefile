    OBJECTS = loader.o kmain.o io.o gdt.o framebuffer.o serial.o keyboard.o idt.o memory_segments.o interrupt_handlers.o interrupts.o pic.o paging_enable.o paging.o kheap.o common.o  hardware_interrupt_enabler.o  user_mode.o
    CC = gcc
    CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
    LDFLAGS = -T link.ld -melf_i386
    AS = nasm
    ASFLAGS = -f elf

    all: kernel.elf
    %.o: %.c
	# Compile c files with gcc
	$(GCC) $(CFLAGS) $< -o $@

    %.o: %.s
	# assemble s files with nasm
	$(NASM) $(ASFLAGS) $< -o $@

    kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

    start_user_program.o: start_user_program.s
	$(NASM) $(ASFLAGS) $< -o $@

    user_program.o: user_program.c
	$(GCC) $(CFLAGS) $< -o $@
    
    user_program.bin: user_program.o start_user_program.o
	$(LD) -T link_user_program.ld -melf_i386 $^ -o $@
    
     os.iso: kernel.elf user_program.bin menu.lst
	cp kernel.elf iso/boot/kernel.elf
	cp user_program.bin iso/modules
	genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    iso

    run: os.iso
	bochs -f bochsrc.txt -q

    %.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

    %.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

    clean:
	rm -rf *.o kernel.elf os.iso
