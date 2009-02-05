
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 */

/* Notes: No warranty expressed or implied. Use at own risk. */
#include "x86/x86.h"
#include "x86/gdt.h"
#include "x86/idt.h"
#include "x86/irq.h"
#include "x86/isrs.h"
#include "x86/apic.h"

void x86_init()
{
	gdt_install();
	idt_install();
	isrs_install();
	apic_init();	/* apic */
	irq_install();
	
	console_init();
	
	timer_install();
	init_keyboard();
	
	__asm__ __volatile__ ("sti");
}