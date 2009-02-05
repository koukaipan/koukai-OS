
#include "kernel/console.h"
#include "kernel/types.h"
#include "kernel/kthread.h"
#include "lib/string.h"
#include "lib/itoa.h"
#include "x86/screen.h"

#define NULL 0

void task_a_prepare();
void task_b_prepare();
void task_c_prepare();

struct Command {
	char *name;
	char *desc;
	void (*func)(int argc, char **argv);
};

void shell_cmds(int argc, char **argv);
void shell_mem(int argc, char **argv);
void shell_lspci(int argc, char **argv);
void shell_date(int argc, char **argv);
void shell_ticks(int argc, char **argv);
void shell_tasks(int argc, char **argv);
void shell_task_a(int argc, char **argv);
void shell_task_b(int argc, char **argv);
void shell_task_c(int argc, char **argv);
void shell_cls(int argc, char **argv);

static struct Command commands[] = {
	{"cmds",	"Show all supported commands", shell_cmds},
	{"minfo",	"Test memory size", shell_mem},
	{"lspci",	"List all PCI devices", shell_lspci},
	{"date",	"Print the system date", shell_date},
	{"ticks",	"Display kernel ticks", shell_ticks},
	{"ps",	"Report all tasks", shell_tasks},
	{"taska",	"Attach/detach task a", shell_task_a},
	{"taskb",	"Attach/detach task b", shell_task_b},
	{"taskc",	"Attach/detach task c", shell_task_c},
	{"clear",	"Clear the terminal screen", shell_cls},
};
#define NCOMMANDS (sizeof(commands)/sizeof(commands[0]))



/***** Implementations of basic kernel monitor commands *****/

void shell_cls(int argc, char **argv)
{
	screen_clear();
}
void shell_cmds(int argc, char **argv)
{
	int i = 0;

	for (i = 0; i < NCOMMANDS; i++){
		settextcolor(0x3,0x0);
		console_puts( "  ");
		console_puts( commands[i].name);
		settextcolor(0x7,0x0);
		console_puts( "\t- ");
		console_puts( commands[i].desc);
		console_puts( "\n");
	}
	settextcolor(0xf,0x0);
}

extern int _g_timer_ticks;
void shell_ticks(int argc, char **argv)
{
	console_puts(" ticks = ");
	console_puts(itoa(_g_timer_ticks,10));
	console_puts("\n");
}

//extern void count_memory(void);
//extern unsigned long mem_end, bse_end;
void shell_mem(int argc, char **argv)
{
	//count_memory();
	
	console_puts(" memory = ");
	//console_puts(itoa(mem_end,10));
	console_puts("\n");
}
extern int lspci();
void shell_lspci(int argc, char **argv)
{
	 lspci();	
}

//extern int printDate();
void shell_date(int argc, char **argv)
{
	//printDate();	
}
void shell_task_a(int argc, char **argv)
{
    static int toggle_a = 0;
    if (!toggle_a)
	    task_a_prepare();
    else
        task_a_remove();
    toggle_a = !toggle_a;
}
void shell_task_b(int argc, char **argv)
{
    static int toggle_b = 0;
    if (!toggle_b)
    	task_b_prepare();
    else
        task_b_remove();
    toggle_b = !toggle_b;
}
void shell_task_c(int argc, char **argv)
{
    static int toggle_c = 0;
    if (!toggle_c)
    	task_c_prepare();
    else
        task_c_remove();
    toggle_c = !toggle_c;
}
void shell_tasks(int argc, char **argv)
{
    int i;

    settextcolor(0xa,0x0);
    console_puts("PID\tTASK\tSTATE\n");
    for (i=0; i<=kt_num; i++)
    {
        if (kt[i].pid < 0) continue;
        console_puts("  ");
        settextcolor(0xf,0x0);
        console_puts(itoa(kt[i].pid,10));
        console_puts("\t");
        settextcolor(0xe,0x0);
        console_puts(kt[i].fname);
        console_puts("\t");
        settextcolor(0xd,0x0);
        switch (kt[i].state)
        {
            case 1: console_puts("INIT\n"); break;
            case 2: console_puts("READY\n"); break;
            case 3: console_puts("RUNNING\n"); break;
            case 4: console_puts("WAIT\n"); break;
            default:	console_puts("UNKNOWN\n"); break;
        }
    }
    settextcolor(0xf,0x0);
}

//int strcmp2(char* str1, char* str2)
//{
	//int i = 0;
	//console_puts(str1);
	//console_puts(str2); 
	//while(str1[i]!=0 && str2[i]!=0 && str1[i]==str2[i])
	//{
		//console_puts(itoa(i, 10));
		//console_puts(" a:");
		//screen_putch(str1[i]);
		//console_puts(" b:");
		//console_putch(str2[i]);
		//console_puts("\n");
		//++i;
	//}
	//console_puts(itoa(i, 10));
		//console_puts(" a:");
		//console_puts(itoa(str1[i],10));
		//console_puts(" b:");
		//console_puts(itoa(str2[i],10));
	//console_puts("\n");
	//return str1[i]-str2[i];
//}

static void runcmd(char *buf)
{
	int i =0;
    if (buf[0] == 0) return;
	for (i = 0; i < NCOMMANDS; i++) {
		if (strcmp(buf, commands[i].name) == 0) {
			commands[i].func(0,NULL);
			return;
		}
	}
	
	console_puts("Unknown command : ");
	console_puts(buf); 
	console_puts("\n");
}

void shell_main()
{
	char shell_cmd[255];
	
	while (1) {
		memset(shell_cmd, 0, 255);
		settextcolor(0xc,0x0);
		console_puts("k");
		settextcolor(0xd,0x0);
		console_puts("o");
		settextcolor(0x9,0x0);
		console_puts("u");
		settextcolor(0xb,0x0);
		console_puts("k");
		settextcolor(0xa,0x0);
		console_puts("a");
		settextcolor(0xe,0x0);
		console_puts("i");
		settextcolor(0xf,0x0);
		//console_puts("s");
		console_puts("-OS $ ");
		console_gets(shell_cmd);
		console_putch('\n');
		if (shell_cmd != NULL ){
			runcmd(shell_cmd);
		}
	}
}
