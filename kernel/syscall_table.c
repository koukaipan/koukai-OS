#include "kernel/syscall_table.h"

void* sys_call_table[NR_SYS_CALL] = {sys_get_ticks, 0};


int sys_get_ticks()
{
	return get_tick();
}

