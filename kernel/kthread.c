#include "kernel/types.h"
#include "kernel/kthread.h"
#include "lib/string.h"

#define NULL 0

int kt_num = 0;
int kt_current = 0; //kernel main thread
int kt_ready = 0;
int* kt_current_sp = 0;
struct kthread_t kt[64];
char fname[8];

void kt_create(int* stack, int fn, void (*func)(void))
{
	kt_num++;

	kt[kt_num].pid = kt_num;
	kt[kt_num].stack = stack;
	kt[kt_num].func = func;
	kt[kt_num].state = KT_INIT;
    switch (fn)
    {
        case 1: strcpy(kt[kt_num].fname, "task_a"); break;
        case 2: strcpy(kt[kt_num].fname, "task_b"); break;
        case 3: strcpy(kt[kt_num].fname, "task_c"); break;
    }

	if(kt_ready == -1) kt_ready = kt_num;

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
