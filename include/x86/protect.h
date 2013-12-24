
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            protect.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_TINIX_PROTECT_H_
#define	_TINIX_PROTECT_H_

#if 0
/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor		/* 共 8 个字节 */
{
	t_16	limit_low;		/* Limit */
	t_16	base_low;		/* Base */
	t_8	base_mid;		/* Base */
	t_8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	t_8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	t_8	base_high;		/* Base */
}DESCRIPTOR;
#endif

/* 門描述符 */
typedef struct s_gate
{
	unsigned short	offset_low;	/* Offset Low */
	unsigned short	selector;	/* Selector */
	unsigned char	dcount;		/* 該字段只在調用門描述符中有效。
				如果在利用調用門調用子程序時引起特權級的轉換和堆棧的改變，需要將外層堆棧中的參數複制到內層堆棧。
				該雙字計數字段就是用於說明這種情況發生時，要複制的雙字參數的數量。 */
	unsigned char	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
	unsigned short	offset_high;	/* Offset High */
}GATE;

typedef struct s_tss {
	unsigned int	backlink;
	unsigned int	esp0;		/* stack pointer to use during interrupt */
	unsigned int	ss0;		/*   "   segment  "  "    "        "     */
	unsigned int	esp1;
	unsigned int	ss1;
	unsigned int	esp2;
	unsigned int	ss2;
	unsigned int	cr3;
	unsigned int	eip;
	unsigned int	flags;
	unsigned int	eax;
	unsigned int	ecx;
	unsigned int	edx;
	unsigned int	ebx;
	unsigned int	esp;
	unsigned int	ebp;
	unsigned int	esi;
	unsigned int	edi;
	unsigned int	es;
	unsigned int	cs;
	unsigned int	ss;
	unsigned int	ds;
	unsigned int	fs;
	unsigned int	gs;
	unsigned int	ldt;
	unsigned short	trap;
	unsigned short	iobase;	/* I/O位圖基址大於或等於TSS段界限，就表示沒有I/O許可位圖 */
	/*unsigned char	iomap[2];*/
}TSS;

/* 权限 */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3
/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3

/* GDT */
/* 描述符索引 */
#define	INDEX_DUMMY		0	// ┓
#define	INDEX_FLAT_C		1	// ┣ LOADER 裏面已經確定了的.
#define	INDEX_FLAT_RW		2	// ┃
#define	INDEX_VIDEO		3	// ┛
#define	INDEX_TSS		4
#define	INDEX_LDT_FIRST		5
/* 選擇子 */
#define	SELECTOR_DUMMY		   0		// ┓
#define	SELECTOR_FLAT_C		0x08		// ┣ LOADER 裏面已經確定了的.
#define	SELECTOR_FLAT_RW	0x10		// ┃
#define	SELECTOR_VIDEO		(0x18+3)	// ┛<-- RPL=3
#define	SELECTOR_TSS		0x20		// TSS. 從外層跳到內存時 SS 和 ESP 的值從裏面獲得.
#define SELECTOR_LDT_FIRST	0x28

#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
#define	SELECTOR_KERNEL_GS	SELECTOR_VIDEO

/* 每個任務有一個單獨的 LDT, 每個 LDT 中的描述符個數: */
#define LDT_SIZE		2

/* 描述符類型值說明 */
#define	DA_32			0x4000	/* 32 位段				*/
#define	DA_LIMIT_4K		0x8000	/* 段界限粒度為 4K 字節			*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* 存儲段描述符類型值說明 */
#define	DA_DR			0x90	/* 存在的只讀數據段類型值		*/
#define	DA_DRW			0x92	/* 存在的可讀寫數據段屬性值		*/
#define	DA_DRWA			0x93	/* 存在的已訪問可讀寫數據段類型值	*/
#define	DA_C			0x98	/* 存在的只執行代碼段屬性值		*/
#define	DA_CR			0x9A	/* 存在的可執行可讀代碼段屬性值		*/
#define	DA_CCO			0x9C	/* 存在的只執行一致代碼段屬性值		*/
#define	DA_CCOR			0x9E	/* 存在的可執行可讀一致代碼段屬性值	*/
/* 系統段描述符類型值說明 */
#define	DA_LDT			0x82	/* 局部描述符表段類型值			*/
#define	DA_TaskGate		0x85	/* 任務門類型值				*/
#define	DA_386TSS		0x89	/* 可用 386 任務狀態段類型值		*/
#define	DA_386CGate		0x8C	/* 386 調用門類型值			*/
#define	DA_386IGate		0x8E	/* 386 中斷門類型值			*/
#define	DA_386TGate		0x8F	/* 386 陷阱門類型值			*/

/* 選擇子類型值說明 */
/* 其中, SA_ : Selector Attribute */
#define	SA_RPL_MASK	0xFFFC
#define	SA_RPL0		0
#define	SA_RPL1		1
#define	SA_RPL2		2
#define	SA_RPL3		3

#define	SA_TI_MASK	0xFFFB
#define	SA_TIG		0
#define	SA_TIL		4

/* 中斷向量 */
#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT		0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

/* 中斷向量 */
#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28


/* 宏 */
/* 線性地址 → 物理地址 */
#define vir2phys(seg_base, vir)	(unsigned int)(((unsigned int)seg_base) + (unsigned int)(vir))


#endif /* _TINIX_PROTECT_H_ */
