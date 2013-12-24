
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

void RR()
{
	do {
        kt_ready++;
    } while (kt[kt_ready].pid < 0);
	if(kt_ready > kt_num)
		kt_ready = 0;
	if(kt_num <= 0)
		kt_ready = 0;
}


extern void os_ctx_sw();
extern int _g_timer_ticks;
void reschedule()
{

/*
	if( _g_timer_ticks % 10!=0 )
		return;
		*/

	kt[kt_current].state = KT_WAIT;
	kt[kt_current].stack = kt_current_sp;

	RR();
//	kt_ready = kt_current;

	/*
	console_puts("\n esp = "); console_puts(itoa(kt_current_sp,10));
	console_puts(" kt_current = "); console_puts(itoa(kt_current,10));
	console_puts(" kt_ready = "); console_puts(itoa(kt_ready,10));
	console_puts(" \n"); 
	*/

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
