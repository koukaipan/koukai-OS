

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

#include "x86/io.h"

unsigned char ioread8(unsigned short port)
{
	unsigned char v;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (v) : "dN" (port));
	return v;
}

unsigned short ioread16(unsigned short port)
{
	unsigned short v;
	__asm__ __volatile__ ("inw %1, %0" : "=a" (v) : "d" (port));
	return v;
}

unsigned int ioread32(unsigned short port)
{
	unsigned int v;
	__asm__ __volatile__ ("inl %1, %0" : "=a" (v) : "d" (port));
	return v;
}

void iowrite8(unsigned short port, unsigned char v)
{
	__asm__ __volatile__ ("outb %0, %1" : : "a" (v), "dN" (port));
}

void iowrite16(unsigned short port, unsigned short v) {
	__asm__ __volatile__ ("outw %0, %1" : : "a" (v), "d" (port));
}
void iowrite32(unsigned short port, unsigned int v) {
	__asm__ __volatile__ ("outl %0, %1" : : "a" (v), "d" (port));
}
