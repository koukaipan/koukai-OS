global os_ctx_sw

extern task_current_sp

os_ctx_sw:
	; 切換到新的stack
    mov     eax, task_current_sp
    mov     esp,[eax]

    pop gs
    pop fs
    pop es
    pop ds
    popad

    add esp, 8
    iret

