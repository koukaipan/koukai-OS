
#ifndef __X86_IDT_H__
#define __X86_IDT_H__

extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

#endif
