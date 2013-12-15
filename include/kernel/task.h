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

#define current (&tasks[task_current])

/****************************************************************************
*	GLOBAL VARIABLES
*****************************************************************************/
extern int task_cnt;
extern int task_current;
extern int* task_current_sp;
extern struct task tasks[MAX_TASKS];

#endif
