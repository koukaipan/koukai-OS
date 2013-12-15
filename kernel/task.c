#include "kernel/types.h"
#include "kernel/task.h"
#include "lib/string.h"

int task_cnt = 0;
int task_current = 0; //kernel main thread
int* task_current_sp = 0;
struct task tasks[MAX_TASKS];

/**
 * @brief tasks structures initialzation
 *
 * fill -1 to each entry
 */
void task_init()
{
	int i;
	for (i=0; i<MAX_TASKS; i++)
		tasks[i].tid = -1;
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
	task_cnt++;

	tasks[task_cnt].tid = task_cnt;
	tasks[task_cnt].stack = stack;
	tasks[task_cnt].func = func;
	tasks[task_cnt].state = TASK_INIT;
	tasks[task_cnt].prio = prio;
	strcpy(tasks[task_cnt].fname, name);

	return tasks[task_cnt].tid;
}

/**
 * @brief Destroy a task
 *
 * @param tid task ID
 */
void task_destroy(int tid)
{
	tasks[tid].tid = -1;
	tasks[tid].stack = NULL;
	tasks[tid].func = NULL;
	tasks[tid].state = 0;
	tasks[tid].fname[0] = '\0';
}