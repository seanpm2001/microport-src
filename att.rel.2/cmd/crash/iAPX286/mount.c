/*	Copyright (c) 1985 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#/*   @(#)mount.c	1.3 - 85/08/09 */
#include	"crash.h"
#include	"sys/mount.h"
#include	"sys/inode.h"
#include	"sys/buf.h"
#include	"sys/filsys.h"
#include	"sys/ino.h"

prmount(c, all)
	int	c;
	int	all;
{
	struct	mount	mbuf;
	struct	filsys	fbuf;
	struct	buf	bbuf;

	if(c == -1)
		return;
	if(c >= v.v_mount) {
		printf("%4d  out of range\n", c);
		return;
	}
#if iAPX286
	if (readmem(&mbuf,(char *)SYM_VALUE(Mount) + (c * sizeof mbuf),
#else
	if (readmem(&mbuf,(long)SYM_VALUE(Mount) + (c * sizeof mbuf),
#endif
		sizeof mbuf) != sizeof mbuf) {
		printf("%4d  read error on mount table\n", c);
		return;
	}

	if(!all && mbuf.m_flags == MFREE)
		return;
	printf("%4u  %3u  %3.3o  %5u  %3u  ",
		c, major(mbuf.m_dev), minor(mbuf.m_dev), mbuf.m_inodp ?
#if iAPX286
		((struct inode *)mbuf.m_inodp - (struct inode *)Inode->n_value)
			: 0,
		mbuf.m_bufp ? ((struct buf *)mbuf.m_bufp - 
			(struct buf *)Buf->n_value) : 0);
#else
		((unsigned)mbuf.m_inodp - Inode->n_value) /
		sizeof(struct inode) : 0,
		mbuf.m_bufp ? ((unsigned)mbuf.m_bufp - Buf->n_value) /
		sizeof(struct buf) : 0);
#endif
	if(mbuf.m_flags == MFREE) {
		printf("\n");
		return;
	}
#if iAPX286
	if(readmem(&bbuf, mbuf.m_bufp, sizeof bbuf) != sizeof bbuf) {
#else
	if(readmem(&bbuf, (long)((unsigned)mbuf.m_bufp&VIRT_MEM), sizeof bbuf) != sizeof bbuf) {
#endif
		printf("read error on mount buffer\n");
		return;
	}
#ifdef	pdp11
	if(readmem(&fbuf, (long)((unsigned)bbuf.b_paddr&VIRT_MEM), sizeof fbuf) != sizeof fbuf) {
#else
	if(readmem(&fbuf, bbuf.b_un.b_addr, sizeof fbuf) != sizeof fbuf) {
#endif
		printf("read error on super block\n");
		return;
	}
	printf("%-6.6s  %-6.6s",fbuf.s_fname, fbuf.s_fpack);
#if iAPX286
	printf(" %6lu  %5lx", fbuf.s_fsize, fbuf.s_isize * INOPB);
#else
	printf(" %6lu  %5u", fbuf.s_fsize, fbuf.s_isize * INOPB);
#endif
	printf(" %6lu  %5u\n", fbuf.s_tfree, fbuf.s_tinode);
}
