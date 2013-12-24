
#ifndef __X86_IRQ_H__
#define __X86_IRQ_H__


#include "x86/x86.h"

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();


#endif
