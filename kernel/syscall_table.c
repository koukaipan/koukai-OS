#include "kernel/syscall_table.h"

void* sys_call_table[NR_SYS_CALL] = {sys_get_ticks, 0};

extern int _g_timer_ticks;	/* _g_timer_ticks is @(timer.c) */
int sys_get_ticks()
{
	return _g_timer_ticks;
}

