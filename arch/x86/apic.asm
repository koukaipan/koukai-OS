%include "arch/x86/apic_addr.inc"

[section .text]	

global apic_init

apic_init:
	
	;mov ax, 3<<3	; select 3rd entry
	;mov gs, ax
	
	;mov	ah, 0Ch				; 0000:黑底, 1100:紅字
	
	;mov al,'A'
	;mov [gs:((80*0 + 70)*2)], ax
	;mov al,'P'
	;mov [gs:((80*0 + 71)*2)], ax
	;mov al,'I'
	;mov [gs:((80*0 + 72)*2)], ax
	;mov al,'C'
	;mov [gs:((80*0 + 73)*2)], ax
	;mov al,':'
	;mov [gs:((80*0 + 74)*2)], ax
	
	;Enable APIC
	mov	ecx, 0x1b
	rdmsr
	or	eax, 0x00000800
	wrmsr

	; setup timer
	mov	[APIC_TimerDIV_ADDR], dword 0x0000000b
	mov	[APIC_TimerINIT_ADDR], dword 0x00f0000b
	mov	[APIC_TimerCNT_ADDR], dword 0x0000000b
	
	; enable local apic
	mov	[APIC_SPURINT_ADDR], dword 0x100
	
	; enable timer
	mov	[APIC_Timer_ADDR], dword 0x00020010

	ret
