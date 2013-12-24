__NR_GET_TICKS			equ 0

INT_VECTOR_SYS_CALL		equ 0x90

extern sys_call_table	;syscall_table.c

; ====================================================================================
;                                 sys_call
; ====================================================================================
global sys_call
sys_call:
	;call	save

    pushad			; ┓
    push 	ds		; ┃
    push 	es		; ┣ 保存原寄存器值
    push 	fs		; ┃
    push 	gs		; ┛
	
    call	[sys_call_table + eax * 4]	;return value is @eax
    mov		[esp+44], eax				;put eax back to original eax
    
    pop		gs		; ┓
    pop 	fs		; ┃
    pop 	es		; ┣ 還原原寄存器值
    pop 	ds		; ┃
    popad			; ┛

    iretd
	
;syscall 0: get_ticks
global get_ticks
get_ticks:
	mov		eax, __NR_GET_TICKS
	int		INT_VECTOR_SYS_CALL
	ret
	