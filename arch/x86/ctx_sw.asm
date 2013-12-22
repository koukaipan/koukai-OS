global os_ctx_sw_int
global os_ctx_sw

extern curr_task_sp

; invoked in normal process context, therefore, we need to
; save context by ourselves
os_ctx_sw:
	; 255 to identify context saved in process context
	push byte 255
	push byte 255
	pushad
	push ds
	push es
	push fs
	push gs

; context saved by HW
os_ctx_sw_int:
    ; switch to new stack
    mov     eax, curr_task_sp
    mov     esp,[eax]

restore:
    pop gs
    pop fs
    pop es
    pop ds

    ;test where the context from
	mov ax, [esp+64]
	cmp ax, 0xFFFF
	je ret_from_normal

ret_from_int:		; return from interrupt context
	popad
	add esp, 8	; handle the 2 parameters pushed by irq(n)
	iret

ret_from_normal:	; return from process context
	popad
	add esp, 8
	sti
	ret
