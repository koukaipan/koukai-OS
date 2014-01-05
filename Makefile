include arch/x86/Config.mk

#######################################################################
# compiler setting
#######################################################################
CC = gcc
LD = ld
OBJCOPY = objcopy
INCLUDE_DIR += -I. -I./include $(ARCH_INCLUDES)

CFLAGS += -Werror -ffreestanding -nostdlib $(INCLUDE_DIR) $(ARCH_CFLAGS)
LDFLAGS += -nostdlib $(ARCH_LDFLAGS)

APP_OBJS = \
	app/shell.o \
	app/task_a.o \
	app/task_b.o \
	app/task_c.o

KERN_OBJS = \
	kernel/kernel.o \
	kernel/console.o \
	kernel/syscall_table.o \
	kernel/task.o

LIB_OBJS = \
	lib/string.o

ALL_OBJS =  $(KERN_OBJS) $(LIB_OBJS) $(ARCH_OBJS) $(APP_OBJS)

#######################################################################
# common modules
#######################################################################
.PHONY: tags all
.SUFFIXES:.asm .bin .S

IMAGE = koukai-OS.img

all: $(IMAGE) tags

$(IMAGE):boot/bootsect.bin kernel.bin
	dd if=/dev/zero of=$(IMAGE) bs=512 count=2880
	dd if=boot/bootsect.bin of=$(IMAGE) bs=512 seek=0 conv=notrunc
	dd if=kernel.bin of=$(IMAGE) bs=512 seek=1 conv=notrunc

%.bin:%.asm
	nasm -f bin $< -o $@

%.o:%.asm
	nasm -f elf $< -o $@

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

%.o:%.S
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

run: $(IMAGE)
	qemu -fda $< -boot a

