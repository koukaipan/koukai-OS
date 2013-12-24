/* 提供shell的操作 */

#include "inc/x86/keyboard.h"
#include "inc/console.h"
#include "inc/shell.h"
#include "inc/lib.h"

void run_cmd(char* cmd);
void print_tick_s(void);
void print_tick(void);
void ls(void);

char shell_cmd[32] = {0};	/* command buffer */
int shell_cmd_len = 0;

void init_shell(void)
{
	console_puts(PROMPT_STRING);
	memset(shell_cmd, 0, 32);
	shell_cmd_len = 0;
}

/********************************************
 * in_process: 處理鍵盤的動作
 *******************************************/
void in_process(unsigned int key)
{
	char c = '\0';

	if (!(key & FLAG_EXT)) {
		c = key;// & 0xFF;
		console_putch(c);
		shell_cmd[shell_cmd_len++] = c;
	}
	else
	{
		int raw_code = key & MASK_RAW;
		switch(raw_code)
		{
		case ENTER:
			console_putch('\n');
			run_cmd(shell_cmd);
			init_shell();
			break;
		case BACKSPACE:
			if(shell_cmd_len>0)
			{
				console_putch('\b');	/* back */
				console_putch(' ');		/* clear the char */
				console_putch('\b');	/* back again */
				shell_cmd[--shell_cmd_len] = '\0';
			}
			break;
		}
	}		
}

/* 分析指令，並執行*/
void run_cmd(char* cmd)
{
	if(strcmp(cmd, "lspci") == 0)
		lspci();
	else if(strcmp(cmd, "clear") == 0)
		console_clear();
	else if(strcmp(cmd, "tick") == 0)
		print_tick();
	else if(strcmp(cmd, "sys_tick") == 0)
		print_tick_s();
	else if(strcmp(cmd, "ls") == 0)
		ls();
	else
	{
		console_puts(cmd);
		console_puts(": command not found\n");
	}
	
}

void print_tick(void)
{
	console_puts("current tick = ");
	console_puts(itoa(get_tick(), 10));		/* call get_tick() in timer.c */
	console_putch('\n');
}

void print_tick_s(void)
{
	console_puts("[system call] current tick = ");
	console_puts(itoa(get_ticks(), 10));	/* system call */
	console_putch('\n');
}

void ls(void)
{
	console_puts("---------------------------->this is fake ls<----------------------------\n");
	console_puts("total 24\n");
	console_puts("-rw------- 1 koukai koukai  680 2008-06-21 20:13 VirtualBox-14479.log\n");
	console_puts("drwxr-xr-x 3 koukai koukai 4096 2008-05-26 15:48 album\n");
	console_puts("drwxr-xr-x 2 koukai koukai 4096 2008-06-16 17:08 bin\n");
	console_puts("lrwxrwxrwx 1 koukai koukai   19 2008-05-16 16:12 Desktop\n");
	console_puts("lrwxrwxrwx 1 koukai koukai   26 2008-05-16 15:55 Examples\n");
	console_puts("drwxr-xr-x 2 koukai koukai 4096 2008-05-16 16:11 Public\n");
	console_puts("drwxr-xr-x 2 koukai koukai 4096 2008-05-16 16:11 Template\n");
}
