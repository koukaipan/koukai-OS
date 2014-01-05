/**
 * @file    arch/x86/syscall.S
 * @author  koukaipan
 * @brief
 */

#include "kernel/unistd.h"

#define INT_VECTOR_SYS_CALL	0x90

.section	.text
	.global	sys_call
	.global get_ticks

sys_call:
	pusha
	push 	%ds
	push 	%es
	push 	%fs
	push 	%gs

	call 	*sys_call_table(,%eax,4)	/* return value is at eax */
	mov	%eax, 44(%esp)			/* put eax back to original eax */

	pop	%gs
	pop 	%fs
	pop 	%es
	pop 	%ds
	popa

	iret

/* syscall 0: get_ticks */
get_ticks:
	mov		$__NR_GET_TICKS, %eax
	int		$INT_VECTOR_SYS_CALL
	ret


