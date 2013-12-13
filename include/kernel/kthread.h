
#ifndef __K_THREAD_H__
#define __K_THREAD_H__

//#define KT_INIT		1
//#define KT_READY	2
//#define KT_RUNNING	3
//#define KT_WAIT		4

typedef enum {
	KT_INIT,
	KT_READY,
	KT_RUNNING,
	KT_WAIT,
	KT_EVENT_WAIT,
	KT_BLOCK,
	KT_DELAY,
	KT_TERMINATE
} KTHREAD_STATE;

#define NR_THREADS	64

struct kthread_t
{
	int tid;
	KTHREAD_STATE state;
	int *stack;
	int prio;
	char fname[20];

	void (*func) ( void );
};

int kt_create(int* stack, char* name, void (*func)(void), int prio);
void kt_destroy(int fn);

extern int kt_num;
extern int kt_current;
extern int* kt_current_sp;
extern struct kthread_t kt[NR_THREADS];

#define current (&kt[kt_current])

#endif
