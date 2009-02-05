
#include "kernel/kthread.h"
#include "kernel/console.h"
#include "kernel/types.h"
#include "lib/itoa.h"
#include "x86/timer.h"


int StackA[1024];

void task_a()
{
	int count = 0;
	while (1) {
		settextcolor(0xa,0x1);
		screen_pos_puts("task_a:", 45, 0);
		screen_pos_putch(star(count % 4), 52, 0);
		settextcolor(0xf,0x0);
		count++;
		timer_wait(2);
	}
}

void task_a_prepare()
{
	asm("cli");
	kt_create(StackA+1024-1, "task_a", task_a, 3);
	asm("sti");
}

void task_a_remove()
{
	asm("cli");
	kt_destroy(1);
	asm("sti");
	memset(StackA, 0, sizeof(StackA));
	settextcolor(0xf,0x0);
	screen_pos_puts("		", 45, 0);
	settextcolor(0xf,0x0);
}
