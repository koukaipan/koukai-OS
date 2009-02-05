
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
#include "x86/protect.h"
#include "x86/process.h"
#include "x86/pci.h"
#include "x86/timer.h"
#include "kernel/kthread.h"
#include "kernel/global.h"
#include "app/shell.h"
#include "lib/lib.h"

void c_main()
{
	int i;
	x86_init();
	
	/* init kernel threads */
	for (i=0; i<64; i++)
		kt[i].pid = -1;
	kt[0].pid = 0;
	strcpy(kt[0].fname, "init");

	console_puts("init...finished\n\n");

	shell_main();
}
