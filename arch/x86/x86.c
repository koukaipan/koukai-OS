/**
 * @file    arch/x86/x86.c
 * @author  koukaipan
 * @brief   This file defines some architecture dependent functions
 */

#include "kernel/kernel.h"

void default_idle()
{
	asm volatile ("sti;hlt");
}
