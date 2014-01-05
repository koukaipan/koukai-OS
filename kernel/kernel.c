
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 */

/* Notes: No warranty expressed or implied. Use at own risk. */
#include "kernel/console.h"
#include "x86/x86.h"
#include "x86/timer.h"
#include "x86/int.h"
#include "kernel/task.h"
#include "app/shell.h"
#include "lib/string.h"

int init_stack[1024];
int init_tid = -1;

int shell_stack[1024];
int shell_tid = -1;

void init();
void start_kernel();

void c_main()
{
	x86_init();
	
	task_init();

	tasks[0].tid = 0;
	strcpy(tasks[0].fname, "0");
	start_kernel();

	init_tid = task_create(init_stack+1024-1, "init", init, 0);
	/* nothing to do */
	while(1)
		default_idle();
}

void start_kernel()
{
	asm __volatile__ ("sti");
}

void init()
{
	shell_tid = task_create(shell_stack+1024-1, "shell", shell_main, 0);
	/* nothing to do */
	while(1)
		default_idle();
}
