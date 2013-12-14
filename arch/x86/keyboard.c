#include "x86/io.h"
#include "x86/keyboard.h"
#include "x86/keymap.h"
#include "x86/irq.h"
#include "x86/int.h"

#define NUL	0x00 
#define BS	0x08 
#define CR	0x0d 

#define FALSE 0
#define TRUE 1

KB_INPUT	kb_in;
int			code_with_E0;
int			shift_l;		/* l shift state	*/
int			shift_r;		/* r shift state	*/
int			alt_l;			/* l alt state		*/
int			alt_r;			/* r left state		*/
int			ctrl_l;			/* l ctrl state		*/
int			ctrl_r;			/* l ctrl state		*/
int			column = 0;		/* keyrow[column] 将是 keymap 中某一个值 */

/* interrupt handler */
void keyboard_handler(struct regs *r)
{
	unsigned char scan_code = inportb(KB_DATA);

	//r = r;
	
	if (kb_in.count < KB_IN_BYTES) {
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
	}
}

void init_keyboard()
{
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

	/* 設定irq handler */
	irq_install_handler(1, keyboard_handler);
}

int in_process(unsigned int key)
{

	if (!(key & FLAG_EXT)) {
		return key & 0xFF;
	}
	else
	{
		int raw_code = key & MASK_RAW;
		switch(raw_code)
		{
		case ENTER:
			return CR;
		case BACKSPACE:
			return BS;
		}
	}
	return NUL;
}

int keyboard_read()
{
	unsigned char	scan_code;
	int	make;	/* TRUE : make  */
			/* FALSE: break */
	unsigned int	key = 0;/* 用一個整型來表示一個鍵。 */
			/* 比如，如果 Home 被按下，則 key 值將為定義在 keyboard.h 中的 'HOME'。*/
	unsigned int*	keyrow;	/* 指向 keymap[] 的某一行 */

	if(kb_in.count > 0){
		code_with_E0 = FALSE;

		scan_code = get_byte_from_kb_buf();

		/* 下面開始解析掃描碼 */
		if (scan_code == 0xE1) {
			int i;
			unsigned char pausebreak_scan_code[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
			int is_pausebreak = TRUE;
			for(i=1;i<6;i++){
				if (get_byte_from_kb_buf() != pausebreak_scan_code[i]) {
					is_pausebreak = FALSE;
					break;
				}
			}
			if (is_pausebreak) {
				key = PAUSEBREAK;
			}
		}
		else if (scan_code == 0xE0) {
			scan_code = get_byte_from_kb_buf();

			/* PrintScreen 被按下 */
			if (scan_code == 0x2A) {
				if (get_byte_from_kb_buf() == 0xE0) {
					if (get_byte_from_kb_buf() == 0x37) {
						key = PRINTSCREEN;
						make = TRUE;
					}
				}
			}

			/* PrintScreen 被釋放 */
			if (scan_code == 0xB7) {
				if (get_byte_from_kb_buf() == 0xE0) {
					if (get_byte_from_kb_buf() == 0xAA) {
						key = PRINTSCREEN;
						make = FALSE;
					}
				}
			}

			/* 不是 PrintScreen。此時 scan_code 為 0xE0 緊跟的那個值。 */
			if (key == 0) {
				code_with_E0 = TRUE;
			}
		}
		if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
			/* 首先判斷Make Code 還是 Break Code */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
			
			/* 先定位到 keymap 中的行 */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];

			column = 0;

			if (shift_l || shift_r) {
				column = 1;
			}

			if (code_with_E0) {
				column = 2;
			}

			key = keyrow[column];

			switch(key) {
			case SHIFT_L:
				shift_l	= make;
				break;
			case SHIFT_R:
				shift_r	= make;
				break;
			case CTRL_L:
				ctrl_l	= make;
				break;
			case CTRL_R:
				ctrl_r	= make;
				break;
			case ALT_L:
				alt_l	= make;
				break;
			case ALT_R:
				alt_l	= make;
				break;
			default:
				break;
			}
		}

		if(make){ /* 忽略 Break Code */
			key |= shift_l	? FLAG_SHIFT_L	: 0;
			key |= shift_r	? FLAG_SHIFT_R	: 0;
			key |= ctrl_l	? FLAG_CTRL_L	: 0;
			key |= ctrl_r	? FLAG_CTRL_R	: 0;
			key |= alt_l	? FLAG_ALT_L	: 0;
			key |= alt_r	? FLAG_ALT_R	: 0;

			//in_process(key);
			return in_process(key);
		}
	}
	return 0;
}

unsigned char get_byte_from_kb_buf()	/* 從鍵盤緩沖區中讀取下一個字節 */
{
	unsigned char scan_code;

	while (kb_in.count <= 0) {}	/* 等待下一個字節到來 */

	disable_int();
	scan_code = *(kb_in.p_tail);
	kb_in.p_tail++;
	if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
		kb_in.p_tail = kb_in.buf;
	}
	kb_in.count--;
	enable_int();

	return scan_code;
}

