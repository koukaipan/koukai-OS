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
	int new_tid = 0;

	/* find a empty tid */
	while(new_tid < MAX_TASKS && tasks[new_tid].tid != -1)
		new_tid++;

	if(new_tid < MAX_TASKS) {
		task_cnt++;
		tasks[new_tid].tid = new_tid;
		tasks[new_tid].stack = stack;
		tasks[new_tid].func = func;
		tasks[new_tid].state = TASK_INIT;
		tasks[new_tid].prio = prio;
		strcpy(tasks[new_tid].fname, name);

		return tasks[new_tid].tid;
	}

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
