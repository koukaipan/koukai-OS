#ifndef	_GLOBAL_H_
#define	_GLOBAL_H_

extern PROCESS* proc_table;
extern struct gdt_entry gdt[10];
extern struct gdt_ptr gp;
extern unsigned char	task_stack[STACK_SIZE_TOTAL];
extern PROCESS*	p_proc_ready;
extern TSS tss;

#endif
