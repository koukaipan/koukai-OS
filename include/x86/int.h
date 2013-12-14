#ifndef __X86_INT_H__
#define __X86_INT_H__

inline void disable_int()
{
	__asm__  __volatile__ ("cli");
}

inline void enable_int()
{
	__asm__  __volatile__ ("sti");
}

#endif
