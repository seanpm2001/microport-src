/*      Copyright (c) 1985 AT&T */
/*        All Rights Reserved   */

/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     */
/*      The copyright notice above does not evidence any        */
/*      actual or intended publication of such source code.     */

/*
** @(#)mmu.h	1.27
**	Definitions for the iAPX 286 Internal MMU
*/

/*
** various handy definitions
*/
#define	DSC_SZ		8		/* # of bytes in a descriptor	*/
#define	SEL_INCR	0x80000L	/* added to a pointer to 	*/
					/* increment only the selector	*/

/*
** gdt selector definitions
*/
#define	MM_SEL			62	/* /dev/mem selector		*/
#define	WNDSEL			63	/* wndump tss gate desc		*/
#define	KCS_SEL			64 	/* kernel CS selector		*/
#define	KDS_SEL			68 	/* kernel DS selector		*/
#define	SCRATCH_SEL		74	/* scratch segment selector	*/
#define	SWTCH_SCRATCH_SEL	75	/* scratch segment selector	*/
#define	COPY0SEL		76	/* scratch segment selector	*/
#define	COPY1SEL		77	/* scratch segment selector	*/
#define	CLSEGSEL		78	/* scratch segment selector	*/
#define	I544SEL			79	/* 544 controller mem-mapped io	*/
#define	SWAPSEL			80	/* selector for swap to use	*/
#define	SIGSEL			81	/* selector for sigcode use	*/
#define	FPSEL			82	/* selctor for fpsave to use	*/
#define	MSG_SEL			83	/* message buffer selector	*/
#define	TFLTSEL			84	/* tss fault desc. selector	*/
#define	DFLTSEL			85	/* double fault desc. selector	*/
#define	GDT_SEL			86	/* gdt alias selector		*/
#define	IDT_SEL			87	/* idt alias selector		*/
#define	SCALL_SEL		89 	/* system call selector in GDT	*/
#define	PSLOT_SEL		90	/* process slot selector	*/

/*
** ldt selector defines
*/
#define EMUL_SEL	7		/* 287 emulator segment selector*/
#define	UPAGE_SEL	8		/* U page segment selector	*/
#define	STACK_SEL	9		/* LDT stack segment selector	*/
#define	CODE1_SEL	10		/* LDT 1st code segment selector*/

/*
** error code format ( on the stack after an mmu fault )
*/
#define	ERR_EXT		0x0001		/* error while trying to invoke	*/
					/* an external interrupt handler*/
#define	ERR_IDT		0x0002		/* selector is in the IDT	*/
#define	ERR_TI		0x0004		/* 1 = sel in LDT, 0 = sel in	*/
					/* GDT if ERR_IDT bit is not set*/
#define	ERR_SEL		0xFFF8		/* selector index		*/

/*
** selector defines
*/
#define	SEL_INDEX	0xFFF8		/* selector index		*/
#define	SEL_TI		0x0004		/* table indicator - 1 = LDT	*/
#define	SEL_RPL		0x0003		/* requested privilege level	*/

/*
** things that go in the selector
*/
#define	GDT_TI		0x0000		/* selector is in the GDT	*/
#define	LDT_TI		0x0004		/* selector is in the LDT	*/
#define	USER_RPL	0x0003		/* user privilege level		*/
#define	KER_RPL		0x0000		/* kernel privilege level	*/

/*
** descriptor access rights byte defines
** common to all descriptors
*/
#define	DSC_PRESENT	0x80		/* is present and addressable	*/
#define	DSC_DPL		0x60		/* descriptor privilege level	*/
#define	DSC_SEG		0x10		/* 1 = segment, 0 = control	*/
#define	KER_DPL		0x00		/* kernel priv. level		*/
#define	USR_DPL		0x60		/* user priv. level		*/

/*
** segment descriptor access rights byte defines
** these are applicable if the DSC_SEG bit is set
*/
#define	SD_CODE		0x08		/* segment is executable	*/
#define	SD_CONFORM	0x04		/* conforming segment		*/
#define	SD_EXPND_DN	0x04		/* expand down segment		*/
#define	SD_READ		0x02		/* segment is readable		*/
#define	SD_WRITE	0x02		/* segment is writeable		*/
#define	SD_ACCESSED	0x01		/* segment has been accessed	*/

/*
** gate descriptor ( control descriptors ) access rights byte defines
** these are applicable if the DSC_SEG bit is reset
*/
#define	G_CALL		0x04		/* call gate descriptor		*/
#define	G_TASK		0x05		/* task gate descriptor		*/
#define	G_INT		0x06		/* interrupt gate descriptor	*/
#define	G_TRAP		0x07		/* trap gate descriptor		*/

/*
** system segment descriptor access rights byte defines
** these are applicable if the DSC_SEG bit is reset
*/
#define	CD_AVAIL_TSK	0x01		/* available task state segment	*/
#define	CD_LDT		0x02		/* ldt descriptor		*/
#define	CD_BUSY_TSK	0x03		/* busy task state segment	*/

/*
** Some handy combinatorial defines for access bytes
*/
/*
** kernel code access byte
*/
#define	ACC_KCODE	DSC_PRESENT | DSC_SEG | KER_DPL | SD_CODE | SD_READ

/*
** kernel data access byte
*/
#define	ACC_KDATA	DSC_PRESENT | DSC_SEG | KER_DPL | SD_WRITE

/*
** user code access byte
*/
#define	ACC_UCODE	DSC_PRESENT | DSC_SEG | USR_DPL | SD_CODE | SD_READ

/*
** user data access byte
*/
#define	ACC_UDATA	DSC_PRESENT | DSC_SEG | USR_DPL | SD_WRITE

/*
** 24 bit address of kernel code, and data
*/
#define	PHYS_KDATA	4096		/* here for wake-up blk		*/

/*
** how big the wake up block is
** gdt starts at PHYS_KDATA + WUBSIZ
*/
#define	WUBSIZ		6

/*
** LDT and GDT selectors to virtual addresses.
*/
#define	gstokv(x)	((char *)((long)((x) << 3 | GDT_TI | KER_RPL) << 16))
#define	lstokv(x)	((char *)((long)((x) << 3 | LDT_TI | KER_RPL) << 16))
#define	lstouv(x)	((char *)((long)((x) << 3 | LDT_TI | USER_RPL) << 16))

/*
** Selector to real number.
*/
#define	ston( x )	( ( x ) >> 3 )

