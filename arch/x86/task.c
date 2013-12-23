/**
 * @file    x86/task.c
 */

#include "x86/task.h"
#include "kernel/task.h"
#include "kernel/console.h"

int total_csw_cnt = 0;

extern void os_ctx_sw();
extern void os_ctx_sw_int();

/* we are *NOW* in interrupt context */
void task_resched_int()
{
	need_resched();
	total_csw_cnt++;
	__asm__  __volatile__ (
		"jmp os_ctx_sw_int\n\t"
	);
}

void schedule()
{
	/* suspend current task */
	current->state = TASK_WAIT;
	current->stack = curr_task_sp;

	task_pick_next();

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

void* task_stack_init(void *start_addr, void *data, void *stack_top)
{
	unsigned long *pstk = stack_top;

	/* Simulate call to function with argument */
	*pstk-- = (unsigned long)data;
	/* return address */
	*pstk-- = (unsigned long)start_addr;
	*pstk-- = 0x000000ff;	/* err_code  */
	*pstk-- = 0x000000ff;	/* int_no */

	*pstk-- = 0xAAAAAAAA;	/* EAX = 0xAAAAAAAA */
	*pstk-- = 0xCCCCCCCC;	/* ECX = 0xCCCCCCCC */
	*pstk-- = 0xDDDDDDDD;	/* EDX = 0xDDDDDDDD */
	*pstk-- = 0xBBBBBBBB;	/* EBX = 0xBBBBBBBB */
	*pstk-- = 0x00000000;	/* ESP = 0x00000000 */
	*pstk-- = 0x11111111;	/* EBP = 0x11111111 */
	*pstk-- = 0x22222222;	/* ESI = 0x22222222 */
	*pstk-- = 0x33333333;	/* EDI = 0x33333333 */

	*pstk-- = 0x00000010;	/* DS = 0x00000010 2nd GDT */
	*pstk-- = 0x00000010;	/* ES = 0x00000010 */
	*pstk-- = 0x00000010;	/* FS = 0x00000010 */
	*pstk   = 0x00000010;	/* GS = 0x00000010 */

	return (void*)pstk;
}
