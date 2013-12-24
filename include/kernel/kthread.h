
#ifndef __K_THREAD_H__
#define __K_THREAD_H__

#define KT_INIT		1
#define KT_READY	2
#define KT_RUNNING	3
#define KT_WAIT		4

struct kthread_t
{

	int pid;
	int state;
	int *stack;
    char fname[8];

	void (*func) ( void );
};

void kt_create(int* stack, int fn, void (*func)(void));
void kt_destroy(int fn);

extern int kt_num;
extern int kt_current;
extern int kt_ready;
extern int* kt_current_sp;
extern struct kthread_t kt[64];

#endif
