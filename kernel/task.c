/**
 * @file    task.c
 * @author  Ying-Shiuan Pan
 * @brief   task manager (arch-independent) and scheduler
 */

#include "kernel/types.h"
#include "kernel/task.h"
#include "lib/string.h"
#include "x86/int.h"

int task_cnt = 0;
int curr_task_tid = 0; //kernel main thread
int* curr_task_sp = 0;
int next_task_tid = 0;
struct task tasks[MAX_TASKS];

/**
 * @brief tasks structures initialzation
 *
 * fill -1 to each entry
 */
void task_init()
{
	int i;
	for (i=0; i<MAX_TASKS; i++) {
		tasks[i].tid = -1;
		tasks[i].state = TASK_TERMINATE;
	}
}

/**
 * @brief
 *
 * @param stack
 * @param name
 * @param func
 * @param prio
 *
 * @return task ID
 */
int task_create(void* stack, char* name, void (*func)(void), int prio)
{
	int new_tid = 0;
	void* pstk = NULL;

	disable_int();
	/* find a empty tid */
	while(new_tid < MAX_TASKS && tasks[new_tid].tid != -1)
		new_tid++;

	if(new_tid < MAX_TASKS) {
		task_cnt++;
		pstk = task_stack_init(func, NULL, stack);
		tasks[new_tid].tid = new_tid;
		tasks[new_tid].stack = pstk;
		tasks[new_tid].func = func;
		tasks[new_tid].state = TASK_WAIT;
		tasks[new_tid].prio = prio;
		strcpy(tasks[new_tid].fname, name);

		enable_int();
		return tasks[new_tid].tid;
	}

	enable_int();
	return -1;
}

/**
 * @brief Destroy a task
 *
 * @param tid task ID
 */
void task_destroy(int tid)
{
	task_cnt--;

	tasks[tid].tid = -1;
	tasks[tid].stack = NULL;
	tasks[tid].func = NULL;
	tasks[tid].state = TASK_TERMINATE;
	tasks[tid].fname[0] = '\0';
}


/**
 * @brief Round-Robin scheduler
 */
void task_sched_RR()
{
	next_task_tid = curr_task_tid;
	/* To find a valid task */
	do {
		next_task_tid++;
	} while (tasks[next_task_tid].tid < 0 && next_task_tid < MAX_TASKS);
	if(next_task_tid >= MAX_TASKS)
		next_task_tid = 0;
	else if(next_task_tid < 0)
		next_task_tid = 0;
}

/**
 * @brief pick up a ready task to run with specific scheduler
 */
void task_pick_next()
{
	task_sched_RR();
}

/**
 * @brief Do we need schedule a new task?
 *
 * @retval TRUE we need reschedule a ready task
 * @retval FALSE we don't need.
 */
int need_resched()
{
	/* suspend current task */
	current->state = TASK_WAIT;
	current->stack = curr_task_sp;

	task_pick_next();

	/* run continualy */
	if (curr_task_tid == next_task_tid)
	{
		tasks[curr_task_tid].state = TASK_RUNNING;
		return FALSE;
	}

	/* resume ready task */
	curr_task_tid = next_task_tid;
	current->state = TASK_RUNNING;
	curr_task_sp = tasks[curr_task_tid].stack;

	return TRUE;
}
