
#include "kernel/kthread.h"
#include "kernel/console.h"
#include "kernel/types.h"
#include "x86/timer.h"


static int StackB[1024];
static int taskb_tid = -1;

void task_b()
{
	int count = 0;
	while (1) {
		settextcolor(0xd, 0x1);
		//screen_pos_puts("task_b:", 54, 0);
		screen_pos_putch(star(count % 4), 61, 0);
		settextcolor(0xf,0x0);
		count++;
		timer_wait(10);
	}
}

void task_b_prepare()
{
	asm("cli");
	taskb_tid = kt_create(StackB+1024-1, "task_b", task_b, 3);
	asm("sti");
}

void task_b_remove()
{
	asm("cli");
	kt_destroy(taskb_tid);
	asm("sti");
	memset(StackB, 0, sizeof(StackB));
	settextcolor(0xf,0x0);
	screen_pos_puts("        ", 54, 0);
}
