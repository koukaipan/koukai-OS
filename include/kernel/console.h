
#ifndef __KEN_CONSOLE_H__
#define __KEN_CONSOLE_H__

#define NUL	0x00 
#define SOH	0x01 
#define STX	0x02 
#define ETX	0x03 
#define EOT	0x04 
#define ENQ	0x05 
#define ACK	0x06 
#define BEL	0x07 
#define BS	0x08 
#define HT	0x09 
#define LF	0x0a 
#define VT	0x0b 
#define FF	0x0c 
#define CR	0x0d 
#define SO	0x0e 
#define SI	0x0f 
#define DLE	0x10 
#define DC1	0x11 
#define DC2	0x12 
#define DC3	0x13 
#define DC4	0x14 
#define NAK	0x15 
#define SYN	0x16 
#define ETB	0x17 
#define CAN	0x18 
#define EM	0x19 
#define SUB	0x1a 
#define __ESC	0x1b 
#define FS	0x1c 
#define GS	0x1d 
#define RS	0x1e 
#define US	0x1f 
#define DEL	0x7f

void console_init(void);

void settextcolor(unsigned char forecolor, unsigned char backcolor);
void screen_pos_puts(const char *text, int new_x, int new_y);
void screen_pos_putch(char c, int new_x, int new_y);

void console_puts(const char *text);
void console_putc(char c);
void console_putch(char c);
void console_clear();

int console_getc(void);
int console_gets(char *s);

#define PROMPT_STRING "# "

#endif
