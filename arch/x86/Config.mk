#######################################################################
# arch-dependent compiler setting
#######################################################################
CC = gcc
LD = ld
OBJCOPY = objcopy
ARCH_INCLUDES = 

ARCH_CFLAGS += -m32
ARCH_LDFLAGS += -m elf_i386

#######################################################################
# arch-dependent modules
#######################################################################

ARCH_OBJS = \
	arch/x86/idt.o \
	arch/x86/gdt.o \
	arch/x86/isr.o \
	arch/x86/isr_asm.o \
	arch/x86/irq.o \
	arch/x86/irq_asm.o \
	arch/x86/timer.o \
	arch/x86/screen.o \
	arch/x86/io.o \
	arch/x86/init.o \
	arch/x86/pci.o \
	arch/x86/apic.o \
	arch/x86/keyboard.o \
	arch/x86/task.o \
	arch/x86/syscall.o \
	arch/x86/ctx_sw.o \
	arch/x86/x86.o
