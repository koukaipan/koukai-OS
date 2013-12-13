
#include "x86/schedule.h"
#include "kernel/kthread.h"
#include "kernel/console.h"

int total_csw_cnt = 0;

/**
 * @brief load a method to run
 *
 * where the state of ready task is KT_INIT, kernel will call run_kt()
 * to run the newly task, and it will *NOT* return.
 *
 */
void run_kt()
{
	int *s= current->stack;
	void (*func)(void) = current->func;
	current->state = KT_RUNNING;

	__asm__  __volatile__ (
			"mov %0,%%esp\n\t"
			"push %1\n\t"
			"sti\n\t"
			"ret\n\t"
			:
			: "m"(s) , "m"(func)
			: "esp"
			);
	
}

/**
 * @brief Round-Robin scheduler
 * @param current current thread number =/= tid
 * @return thread number ready to run
 *
 * pick up a ready task to run
 *
 */
int RR(int curr)
{
	int ready = curr;
	/* To find a valid kt */
	do {
		ready++;
	} while (kt[ready].tid < 0);
	if(ready > kt_num)
		ready = 0;
	if(kt_num <= 0)
		ready = 0;

	return ready;
}


extern void os_ctx_sw();

void reschedule()
{
	int kt_ready;

	current->state = KT_WAIT;
	current->stack = kt_current_sp;

	kt_ready = RR(kt_current);

	if(kt[kt_ready].state == KT_INIT){
		//console_puts(" try run_kt\n");
		kt_current = kt_ready;
		run_kt(); 

		// never run here
	}
	if (kt_current == kt_ready)
	{
		kt[kt_current].state = KT_RUNNING;
		return;
	}

	kt_current = kt_ready;
	current->state = KT_RUNNING;
	kt_current_sp = kt[kt_current].stack;

	/* we are now in interrupt context */
//	return;
	total_csw_cnt++;
	__asm__  __volatile__ (
		"jmp os_ctx_sw\n\t"
		);
} 
