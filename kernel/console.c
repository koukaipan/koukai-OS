

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
#include "x86/screen.h"
#include "x86/keyboard.h"
#include "kernel/console.h"

/* 將螢幕操作的實做放在screen裡面, 用console包起來 */
void console_init(void)
{
	screen_init();
}

void console_puts(const char *text)
{
	screen_puts(text);
}

void console_putch(char c)
{
	screen_putch(c);
}

void console_clear()
{
	screen_clear();
}

int console_getc(void)
{
	int c;
	do{
		c = keyboard_read();
	}while(c==0 || !(c==CR || c==BS || (c>0x1f && c<0x7f)));

	return c & 0xFF;
}

int console_gets(char *s)
{
	int cnt = 0;
	char  c;

	while ((c = console_getc()) != CR) 
	{
		if (c != BS) {
			cnt++;
			*s++ = c;
			screen_putch(c);	//echo
		}
		else {
			if (cnt > 0) {
				cnt--; 
				*--s = '\0';
				console_puts("\b \b");
			}
		}
	}
	*s = 0;
	return cnt;
}
