/**
 * @file    include/x86/idt.h
 * @brief   Interrupt description table
 */

#ifndef __X86_IDT_H__
#define __X86_IDT_H__

#define NR_IDT_ENTRIES		256

void idt_set_gate(unsigned char num, unsigned long base,
		unsigned short sel, unsigned char flags);
void idt_install();

#endif
