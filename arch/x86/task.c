#include "x86/task.h"
#include "kernel/task.h"
#include "kernel/console.h"

int total_csw_cnt = 0;

/**
 * @brief load a method to run
 *
 * If the state of ready task is TASK_INIT, kernel will call run_task()
 * to run the newly task, and it will *NOT* return.
 */
void task_start()
{
	int *s= current->stack;
	void (*func)(void) = current->func;
	current->state = TASK_RUNNING;

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

extern void os_ctx_sw();
extern void os_ctx_sw_int();

void task_resched_int()
{
	if(need_resched()) {
	/* we are *NOW* in interrupt context */
		total_csw_cnt++;
		__asm__  __volatile__ (
			"jmp os_ctx_sw_int\n\t"
		);
	}
}

void reschedule()
{
	/* suspend current task */
	current->state = TASK_WAIT;
	current->stack = curr_task_sp;

	task_pick_next();

	if(tasks[next_task_tid].state == TASK_INIT){
		curr_task_tid = next_task_tid;
		task_start();
		/* never reach here */
	}

	/* run continualy */
	if (curr_task_tid == next_task_tid)
	{
		tasks[curr_task_tid].state = TASK_RUNNING;
		return;
	}

	/* resume ready task */
	curr_task_tid = next_task_tid;
	current->state = TASK_RUNNING;
	curr_task_sp = tasks[curr_task_tid].stack;

	/* we are now in interrupt context */
	total_csw_cnt++;
	__asm__  __volatile__ (
		"jmp os_ctx_sw_int\n\t"
		);
}
