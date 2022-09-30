/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/
# include	"curses.ext"
# include	<signal.h>

char	*calloc();
char	*malloc();
extern	char	*getenv();

extern	WINDOW	*makenew();

/*
 *	This routine initializes the current and standard screen.
 *
 */

WINDOW *
initscr()
{
	register char *sp;
	struct screen *scp;
	extern char *_c_why_not;
	
# ifdef DEBUG
	if (outf == NULL) {
		if( ( outf = fopen("trace", "w") ) == NULL)
		{
			perror("trace");
			exit(-1);
		}
	}
#endif

	if( ( sp = getenv( "TERM" ) ) == NULL )
	{
		sp = Def_term;
	}
# ifdef DEBUG
	if(outf) fprintf(outf, "INITSCR: term = %s\n", sp);
# endif
	if( ( scp = newterm( sp, stdout, stdin ) ) == NULL )
	{
		_ec_quit(_c_why_not, sp);
	}
	return stdscr;
}
