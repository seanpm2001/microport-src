/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.2	*/
/*LINTLIBRARY*/
/*
 * return offset in file.
 */

extern long lseek();

long
tell(f)
int	f;
{
	return(lseek(f, 0L, 1));
}
