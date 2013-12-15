/**
 * @file	apic.S
 * @author	koukaipan
 * @brief
 */

#include "x86/apic_addr.h"

.code32

.text
	.global apic_init

apic_init:
	/* Enable APIC */
	movl	$0x0000001B, %ecx
	rdmsr
	orl		$0x00000800, %eax
	wrmsr

	/* setup timer */
	movl	$0x0000000B, (APIC_TimerDIV_ADDR)
	movl	$0x00F0000B, (APIC_TimerINIT_ADDR)
	movl	$0x0000000B, (APIC_TimerCNT_ADDR)

	/* enable local apic */
	movl	$0x00000100, (APIC_SPURINT_ADDR)

	/* enable timer */
	movl	$0x00020010, (APIC_Timer_ADDR)

	ret
