#include "kernel/syscall_table.h"

void* sys_call_table[NR_SYS_CALL] = {
	[0] = sys_get_ticks,
	0
};

unsigned int __get_ticks();
unsigned int sys_get_ticks()
{
	return __get_ticks();
}
