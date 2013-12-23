/**
 * @file    task.h
 * @author
 * @brief
 */

#ifndef __TASK_H__
#define __TASK_H__

/****************************************************************************
*						CONSTANTS
*****************************************************************************/
#define MAX_TASKS	64

typedef enum {
	TASK_INIT,
	TASK_READY,
	TASK_RUNNING,
	TASK_WAIT,
	TASK_EVENT_WAIT,
	TASK_BLOCK,
	TASK_DELAY,
	TASK_TERMINATE
} TASK_STATE;

struct task
{
	int tid;
	TASK_STATE state;
	void *stack;
	int prio;
	char fname[20];

	void (*func) ( void );
};

/****************************************************************************
*	FUNCTION PROTOTYPES
*****************************************************************************/
void task_init();
int task_create(void* stack, char* name, void (*func)(void), int prio);
void task_destroy(int fn);
void task_pick_next();
void* task_stack_init(void *start_addr, void *data, void *stack_top);
int need_resched();

#define current (&tasks[curr_task_tid])

/****************************************************************************
*	GLOBAL VARIABLES
*****************************************************************************/
extern int task_cnt;
extern int curr_task_tid;
extern int* curr_task_sp;
extern int next_task_tid;
extern struct task tasks[MAX_TASKS];

/* not implement here */
void task_start();

#endif
