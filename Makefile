
.SUFFIXES:.asm .bin

APP_OBJS = \
	app/shell.o \
	app/task_a.o \
	app/task_b.o \
	app/task_c.o \
	app/star.o

KERN_OBJS = \
	kernel/kernel.o \
	kernel/console.o \
	kernel/global.o \
	kernel/syscall.o \
	kernel/syscall_table.o \
	kernel/kthread.o

X86_OBJS = \
	arch/x86/idt.o \
	arch/x86/gdt.o \
	arch/x86/isrs.o \
	arch/x86/x86.o \
	arch/x86/irq.o \
	arch/x86/timer.o \
	arch/x86/screen.o \
	arch/x86/io.o \
	arch/x86/init.o \
	arch/x86/pci.o \
	arch/x86/apic.o \
	arch/x86/keyboard.o \
	arch/x86/schedule.o
	
LIB_OBJS = \
	lib/string.o \
	lib/itoa.o \
	lib/mem.o

ALL_OBJS =  $(KERN_OBJS) $(LIB_OBJS) $(X86_OBJS) $(APP_OBJS)

all: floppy.img tags

floppy.img:boot/bootsect.bin kernel.bin
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	dd if=boot/bootsect.bin of=floppy.img bs=512 seek=0 conv=notrunc
	dd if=kernel.bin of=floppy.img bs=512 seek=1 conv=notrunc

test:floppy.img
	qemu -fda $< -s

%.bin:%.asm
	nasm -f bin $< -o $@

%.o:%.asm
	nasm -f elf $< -o $@

CFLAGS += -ffreestanding -nostdlib -I./ -I./include -m32
%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

%.s:%.c
	gcc -S $< -o $@
	
kernel.bin: $(ALL_OBJS)
	ld --oformat binary -m elf_i386 -nostdlib -T link.ld -o $@ $^

#kernel.elf: $(ALL_OBJS)
#	ld --oformat elf32-i386 -nostdlib -T link.ld -o $@ $^

# elf image (could be booted by other bootloader such as grub)
kernel.elf.bin: boot/entry.o $(ALL_OBJS)
	ld -nostdlib -T link.elf.ld -o $@ $^

#kernel.o2: $(ALL_OBJS)
	#ld -nostdlib -e main -Ttext 0x10000 -o $@ $^

#kernel.bin2:kernel.o2
	#objcopy -R .note -R .comment -S -O binary $< $@

clean:
	rm  -f *.o *.bin *.bin2 *.img *.o2 *.s  $(ALL_OBJS) boot/bootsect.bin tags

tags:
	ctags -R . 
	

# gcc - -freeseestanding
# ld    -nostdlib

.PHONY:
	tags
