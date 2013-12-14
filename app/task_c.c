#include "kernel/kthread.h"
#include "kernel/console.h"
#include "kernel/types.h"

#include "lib/string.h"

static int StackC[1024];
static int taskc_tid = -1;
static char star[4] = {'|', '\\', '-', '/'};

void c_delay()
{
	int i;
	for( i = 0 ; i < 0x2ffffff; i ++)
		;
}

void task_c()
{
	int count = 0;
	while (1) {
		settextcolor(0xe,0x1);
		//screen_pos_puts("task_c:", 63, 0);
		screen_pos_putch(star[count % 4], 70, 0);
		settextcolor(0xf,0x0);
		count++;
		c_delay();
	}
}

void task_c_prepare()
{
	asm("cli");
	taskc_tid = kt_create(StackC+1024-1, "task_c", task_c, 3);
	asm("sti");
}

void task_c_remove()
{
	asm("cli");
	kt_destroy(taskc_tid);
	asm("sti");
	memset(StackC, 0, sizeof(StackC));
	settextcolor(0xf,0x0);
	screen_pos_puts("        ", 63, 0);
}
