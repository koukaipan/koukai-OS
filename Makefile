#######################################################################
# compiler setting
#######################################################################
CC = gcc
LD = ld
OBJCOPY = objcopy
INCLUDE_DIR = -I. -I./include

CFLAGS += -m32 -Wall -ffreestanding -nostdlib $(INCLUDE_DIR)
LDFLAGS += -m elf_i386 -nostdlib

APP_OBJS = \
	app/shell.o \
	app/task_a.o \
	app/task_b.o \
	app/task_c.o

KERN_OBJS = \
	kernel/kernel.o \
	kernel/console.o \
	kernel/syscall_table.o \
	kernel/kthread.o

X86_OBJS = \
	arch/x86/global.o \
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
	arch/x86/schedule.o \
	arch/x86/syscall.o
	
LIB_OBJS = \
	lib/string.o

ALL_OBJS =  $(KERN_OBJS) $(LIB_OBJS) $(X86_OBJS) $(APP_OBJS)

#######################################################################
# common modules
#######################################################################
.PHONY: tags all
.SUFFIXES:.asm .bin

TARGET = koukai-OS.img

all: $(TARGET) tags

$(TARGET):boot/bootsect.bin kernel.bin
	dd if=/dev/zero of=$(TARGET) bs=512 count=2880
	dd if=boot/bootsect.bin of=$(TARGET) bs=512 seek=0 conv=notrunc
	dd if=kernel.bin of=$(TARGET) bs=512 seek=1 conv=notrunc

%.bin:%.asm
	nasm -f bin $< -o $@

%.o:%.asm
	nasm -f elf $< -o $@

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

%.s:%.c
	gcc -S $< -o $@
	
kernel.bin: $(ALL_OBJS)
	ld $(LDFLAGS) --oformat binary -T link.ld -o $@ $^

# elf image (could be booted by other bootloader such as grub)
kernel.elf.bin: boot/entry.o $(ALL_OBJS)
	ld $(LDFLAGS) --oformat elf32-i386 -T link.elf.ld -o $@ $^

clean:
	rm  -f *.o *.bin *.bin2 *.img $(ALL_OBJS) boot/bootsect.bin tags

tags:
	ctags -R . 

run: $(TARGET)
	qemu -fda $< -boot a

