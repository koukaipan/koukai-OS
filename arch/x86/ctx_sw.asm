global os_ctx_sw_int
global os_ctx_sw

extern curr_task_sp

os_ctx_sw:
    ; switch to new stack
    mov     eax, curr_task_sp
    mov     esp,[eax]

    pop gs
    pop fs
    pop es
    pop ds
    popad

    ret

os_ctx_sw_int:
    ; switch to new stack
    mov     eax, curr_task_sp
    mov     esp,[eax]

    pop gs
    pop fs
    pop es
    pop ds
    popad

    add esp, 8	;handle the 2 parameters pushed by irq(n)
    iret

