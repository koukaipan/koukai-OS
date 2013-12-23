/**
 * @file    include/x86/irq.h
 * @brief   IRQ routines and operations
 */

#ifndef __X86_IRQ_H__
#define __X86_IRQ_H__

#include "x86/x86.h"

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();

#endif
