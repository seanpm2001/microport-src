/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/
#include "curses.ext"
#include "../local/uparm.h"

extern	struct term *cur_term;

/*
 * This is useful after saving/restoring memory from a file (e.g. as
 * in a rogue save game).  It assumes that the modes and windows are
 * as wanted by the user, but the terminal type and baud rate may
 * have changed.
 */
restartterm(term, filenum, errret)
char *term;
int filenum;	/* This is a UNIX file descriptor, not a stdio ptr. */
int *errret;
{
	int saveecho = SP->fl_echoit;
	int savecbreak = SP->fl_rawmode;
	int saveraw;
	int savenl;

#ifdef USG
	saveraw = (cur_term->Nttyb).c_cc[VINTR] == 0377;
	savenl = (cur_term->Nttyb).c_iflag & ICRNL;
#else
	saveraw = (cur_term->Nttyb).sg_flags | RAW;
	savenl = (cur_term->Nttyb).sg_flags & CRMOD;
#endif

	setupterm(term, filenum, errret);

	/*
	 * Restore curses settable flags, leaving other stuff alone.
	 */
	if (saveecho)
		echo();
	else
		noecho();

	if (savecbreak)
		cbreak(), noraw();
	else if (saveraw)
		nocbreak(), raw();
	else
		nocbreak(), noraw();
	
	if (savenl)
		nl();
	else
		nonl();

	reset_prog_mode();

	LINES = lines;
	COLS = columns;
}
