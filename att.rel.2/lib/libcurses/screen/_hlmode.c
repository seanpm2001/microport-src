/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/

#include "curses.ext"

_hlmode (on)
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_hlmode(%o).\n", on);
#endif
	SP->virt_gr = on;
}
