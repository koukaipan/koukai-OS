#include "x86/gdt.h"

#define LDT_SIZE 2

typedef struct s_stackframe {	/* proc_ptr points here							↑ Low			*/
	unsigned int	gs;					/* ┓											│			*/
	unsigned int	fs;					/* ┃											│			*/
	unsigned int	es;					/* ┃											│			*/
	unsigned int	ds;					/* ┃											│			*/
	unsigned int	edi;				/* ┃											│			*/
	unsigned int	esi;				/* ┣ pushed by save()							│			*/
	unsigned int	ebp;				/* ┃											│			*/
	unsigned int	kernel_esp;			/* <- 'popad' will ignore it					│			*/
	unsigned int	ebx;				/* ┃											↑栈从高地址往低地址增长*/		
	unsigned int	edx;				/* ┃											│			*/
	unsigned int	ecx;				/* ┃											│			*/
	unsigned int	eax;				/* ┛											│			*/
	unsigned int	retaddr;			/* return address for assembly code save()		│			*/
	unsigned int	eip;				/*  ┓											│			*/
	unsigned int	cs;					/*  ┃											│			*/
	unsigned int	eflags;				/*  ┣ these are pushed by CPU during interrupt	│			*/
	unsigned int	esp;				/*  ┃											│			*/
	unsigned int	ss;					/*  ┛											┷High		*/
}STACK_FRAME;


typedef struct s_proc {
	STACK_FRAME			regs;			/* process' registers saved in stack frame */

	unsigned short		ldt_sel;		/* selector in gdt giving ldt base and limit*/
	struct gdt_entry 	ldts[LDT_SIZE];		/* local descriptors for code and data */
								/* 2 is LDT_SIZE - avoid include protect.h */
	unsigned int		pid;			/* process id passed in from MM */
	char				p_name[16];		/* name of the process */
}PROCESS;


/* Number of tasks */
#define NR_TASKS	1

/* stacks of tasks */
#define STACK_SIZE_TESTA	0x8000

#define STACK_SIZE_TOTAL	STACK_SIZE_TESTA
