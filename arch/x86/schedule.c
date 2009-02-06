
#include "x86/schedule.h"
#include "kernel/kthread.h"
#include "kernel/console.h"

void run_kt()
{
	int *s= kt[kt_current].stack;
	void (*func)(void) = kt[kt_current].func;
	kt[kt_current].state = KT_RUNNING;

	/* In either case, we need to send an EOI to the master
	 *  interrupt controller too */
	outportb(0x20, 0x20);

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
 *
 * pick up a ready task to run
 */
void RR()
{
	/* To find a valid kt */
	do {
		kt_ready++;
	} while (kt[kt_ready].tid < 0);
	if(kt_ready > kt_num)
		kt_ready = 0;
	if(kt_num <= 0)
		kt_ready = 0;
}


extern void os_ctx_sw();

void reschedule()
{
	kt[kt_current].state = KT_WAIT;
	kt[kt_current].stack = kt_current_sp;

	RR();


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

	kt[kt_current].state = KT_WAIT;
	kt[kt_current].stack = kt_current_sp;

	kt_current = kt_ready;
	kt[kt_current].state = KT_RUNNING;
	kt_current_sp = kt[kt_current].stack;

//	return;
	__asm__  __volatile__ (
			"jmp os_ctx_sw\n\t"
			);
} 
