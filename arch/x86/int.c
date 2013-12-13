#include "x86/int.h"

inline void disable_int()
{
	__asm__  __volatile__ ("cli");
}
	
inline void enable_int()
{
	__asm__  __volatile__ ("sti");
}
	