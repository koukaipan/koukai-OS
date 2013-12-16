global irq_timer_stub
global os_ctx_sw

extern task_current_sp
extern os_timer_ticks
extern reschedule

irq_timer_stub:
    pushad
    push ds
    push es
    push fs
    push gs

    ;the 2nd entry of GDT
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 保留目前的stack
    mov     eax, task_current_sp
    mov     [eax],esp

    mov eax, os_timer_ticks
    call eax
    mov eax, reschedule
    call eax

os_ctx_sw:
	; 切換到新的stack
    mov     eax, task_current_sp
    mov     esp,[eax]

    pop gs
    pop fs
    pop es
    pop ds
    popad

    iret

