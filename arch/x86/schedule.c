#include "x86/schedule.h"
#include "kernel/task.h"
#include "kernel/console.h"

int total_csw_cnt = 0;

/**
 * @brief load a method to run
 *
 * If the state of ready task is TASK_INIT, kernel will call run_task()
 * to run the newly task, and it will *NOT* return.
 */
void run_task()
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

/**
 * @brief Round-Robin scheduler
 * @param current current task id
 * @return task id which is ready to run
 *
 * pick up a ready task to run
 *
 */
int RR(int curr)
{
	int ready = curr;
	/* To find a valid task */
	do {
		ready++;
	} while (tasks[ready].tid < 0);
	if(ready > task_cnt)
		ready = 0;
	if(task_cnt <= 0)
		ready = 0;

	return ready;
}


extern void os_ctx_sw();

void reschedule()
{
	int task_ready;

	current->state = TASK_WAIT;
	current->stack = task_current_sp;

	task_ready = RR(task_current);

	if(tasks[task_ready].state == TASK_INIT){
		//console_puts(" try run_task\n");
		task_current = task_ready;
		run_task();

		// never run here
	}
	if (task_current == task_ready)
	{
		tasks[task_current].state = TASK_RUNNING;
		return;
	}

	task_current = task_ready;
	current->state = TASK_RUNNING;
	task_current_sp = tasks[task_current].stack;

	/* we are now in interrupt context */
//	return;
	total_csw_cnt++;
	__asm__  __volatile__ (
		"jmp os_ctx_sw\n\t"
		);
} 
