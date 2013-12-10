#include "kernel/types.h"
#include "kernel/kthread.h"
#include "lib/string.h"

int kt_num = 0;
int kt_current = 0; //kernel main thread
int kt_ready = 0;
int* kt_current_sp = 0;
struct kthread_t kt[NR_THREADS];

/**
 * @brief kthreads initialzation
 *
 * fill -1 to each entry
 */
void init_kthreads()
{
	int i;
	for (i=0; i<NR_THREADS; i++)
		kt[i].tid = -1;

	kt[0].tid = 0;
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
 * @return thread ID
 */
int kt_create(int* stack, char* name, void (*func)(void), int prio)
{
	kt_num++;

	kt[kt_num].tid = kt_num;
	kt[kt_num].stack = stack;
	kt[kt_num].func = func;
	kt[kt_num].state = KT_INIT;
	kt[kt_num].prio = prio;
	strcpy(kt[kt_num].fname, name);

	if(kt_ready == -1)
		kt_ready = kt_num;

	return kt[kt_num].tid;
}

/**
 * @brief Destroy a kernel thread
 *
 * @param tid kernel thread ID
 */
void kt_destroy(int tid)
{
	kt[tid].tid = -1;
	kt[tid].stack = NULL;
	kt[tid].func = NULL;
	kt[tid].state = 0;
	kt[tid].fname[0] = '\0';

}
