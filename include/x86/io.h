
#ifndef __X86_IO_H__
#define __X86_IO_H__

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

/* double word */
unsigned long inportd (unsigned short _port);
unsigned short inportw( unsigned short port );

void outportd( unsigned short port, unsigned long value );
void outportw( unsigned short port, unsigned short value );

#endif
