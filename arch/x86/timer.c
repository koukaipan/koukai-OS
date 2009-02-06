

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
#include "kernel/types.h"
#include "lib/itoa.h"
#include "x86/x86.h"

/* This will keep track of how many ticks that the system
*  has been running for */
static int _g_timer_ticks = 0;

/**
 * @brief timer interrupt handler
 * Handles the timer. In this case, it's very simple: We
 * increment the 'timer_ticks' variable every time the
 * timer fires. By default, the timer fires 18.222 times
 * per second. Why 18.222Hz? Some engineer at IBM must've
 * been smoking something funky
 */
void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    _g_timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    //if (_g_timer_ticks % 180 == 0)
    //{
        //console_puts("10 seconds has passed -- ");
        //console_puts("Ticks = ");
        //console_puts(itoa(_g_timer_ticks,10));
        //console_puts("\n");
    //}
}

int get_tick()
{
	return _g_timer_ticks;
}

void os_timer_ticks()
{
    /* Increment our 'tick count' */
    _g_timer_ticks++;

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = _g_timer_ticks + ticks;
    while(_g_timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    console_puts("timer installed.\n");
    irq_install_handler(0, timer_handler);
    _g_timer_ticks = 0;
}
