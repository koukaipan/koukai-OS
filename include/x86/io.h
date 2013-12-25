
#ifndef __X86_IO_H__
#define __X86_IO_H__

unsigned char ioread8(unsigned short port);
unsigned short ioread16(unsigned short port);
unsigned int ioread32( unsigned short port );

void iowrite8(unsigned short port, unsigned char v);
void iowrite16(unsigned short port, unsigned short v);
void iowrite32(unsigned short port, unsigned int v);

#endif
