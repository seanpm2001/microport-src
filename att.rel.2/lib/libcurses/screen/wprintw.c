/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/

# include	"curses.ext"
# include	<varargs.h>

/*
 *	This routine implements a printf on the given window.
 */
/* VARARGS */
wprintw(win, fmt, va_alist)
WINDOW	*win;
char	*fmt;
va_dcl
{
	va_list ap;

	va_start(ap);
	return _sprintw(win, fmt, ap);
}
