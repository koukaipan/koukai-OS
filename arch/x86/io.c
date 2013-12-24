

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

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

unsigned long inportd( unsigned short port ) {
	unsigned long ret;

	__asm__( "inl %%dx, %%eax" : "=a" (ret) : "d" (port) );

	return ret;
}

unsigned short inportw( unsigned short port ) {
	unsigned short ret;
	__asm__( "inw %%dx, %%ax" : "=a" (ret) : "d" (port) );

	return ret;
}


void outportd( unsigned short port, unsigned long value ) {
	__asm__( "outl %%eax, %%dx" : : "a" (value), "d" (port) );
}
void outportw( unsigned short port, unsigned short value ) {
	__asm__( "outw %%ax, %%dx" : : "a" (value), "d" (port) );
}
