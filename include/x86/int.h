#ifndef __X86_INT_H__
#define __X86_INT_H__

static inline void disable_int()
{
	__asm__  __volatile__ ("cli");
}

static inline void enable_int()
{
	__asm__  __volatile__ ("sti");
}

#endif
