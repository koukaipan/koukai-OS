/**
 * @file    arch/x86/ctx_sw.S
 * @author  koukaipan
 * @brief   context switch (sava and restore)
 */

.section	.text
	.global os_ctx_sw_int
	.global os_ctx_sw

os_ctx_sw:
	/* indicates that context saved in normal process context */
	push $0x000000FF
	push $0x000000FF
	pusha	/* eax, ecx, edx, ebx, esp, ebp, esi, edi */
	push %ds
	push %es
	push %fs
	push %gs
	
os_ctx_sw_int:
	/* switch to stack */
	movl	 $curr_task_sp, %eax
	movl	 (%eax), %esp

restore:
	pop %gs
	pop %fs
	pop %es
	pop %ds
	/* check the error code is 0xff */
	mov 32(%esp), %ax
	cmp $0x00ff, %ax
	je ret_from_normal

/* iret only if the context is saved in inerrupt context */
ret_from_int:
	popa
	addl $8, %esp	/* remove (int_no, err_code) pushed by irq(n) */
	iret

ret_from_normal:
	popa
	addl $8, %esp
	sti
	ret
