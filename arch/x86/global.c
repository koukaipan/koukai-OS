#include "x86/process.h"
#include "x86/protect.h"

//PROCESS		proc_table[NR_TASKS];
//unsigned char	task_stack[STACK_SIZE_TOTAL];
//PROCESS*	p_proc_ready;
TSS		tss;

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[10];
struct gdt_ptr gp;
