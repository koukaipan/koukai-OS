#include "kernel/types.h"
#include "kernel/kthread.h"
#include "lib/string.h"

#define NULL 0

int kt_num = 0;
int kt_current = 0; //kernel main thread
int kt_ready = 0;
int* kt_current_sp = 0;
struct kthread_t kt[NR_THREADS];
char fname[20];

/**
 * @brief kthreads initialzation
 *
 * fill -1 to each entry
 */
void init_kthreads()
{
	int i;
	for (i=0; i<NR_THREADS; i++)
		kt[i].pid = -1;

	kt[0].pid = 0;
	strcpy(kt[0].fname, "init");
}

/**
 * @brief
 *
 * @param stack
 * @param name
 * @param func
 * @param prio
 *
 * @return PID
 */
int kt_create(int* stack, char* name, void (*func)(void), int prio)
{
	kt_num++;

	kt[kt_num].pid = kt_num;
	kt[kt_num].stack = stack;
	kt[kt_num].func = func;
	kt[kt_num].state = KT_INIT;
	kt[kt_num].prio = prio;
	strcpy(kt[kt_num].fname, name);

	if(kt_ready == -1)
		kt_ready = kt_num;

	return kt[kt_num].pid;
}

void kt_destroy(int fn)
{
	int i;
	switch (fn)
	{
		case 1: strcpy(fname, "task_a"); break;
		case 2: strcpy(fname, "task_b"); break;
		case 3: strcpy(fname, "task_c"); break;
	}

	for (i=0; i<=kt_num; i++)
	{
		if (!strcmp(kt[i].fname, fname))
		{
			kt[i].pid = -1;
			kt[i].stack = NULL;
			kt[i].func = NULL;
			kt[i].state = 0;
			kt[i].fname[0] = '\0';
			break;
		}
	}
}
