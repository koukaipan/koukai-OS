/**
 * @file    include/x86/timer.h
 * @brief   timer interrupt operation
 */

#ifndef __X86_TIMER_H__
#define __X86_TIMER_H__

void timer_install();
void timer_wait(int ticks);

#endif
