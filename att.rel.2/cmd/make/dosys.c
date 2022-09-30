/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/

# include "defs"
# include "sys/types.h"
# include "sys/stat.h"

extern char Makecall;

dosys(comstring, nohalt)
register CHARSTAR comstring;
int nohalt;
{
	register CHARSTAR p;
	register int i;
	int status;

	p = comstring;
	while(	*p == BLANK ||
		*p == TAB) p++;
	if(!*p)
		return(-1);

	if(IS_ON(NOEX) && Makecall == NO)
		return(0);

	if(metas(comstring))
		status = doshell(comstring,nohalt);
	else
		status = doexec(comstring);

	return(status);
}



metas(s)   /* Are there are any  Shell meta-characters? */
register CHARSTAR s;
{
	while(*s)
		if( funny[*s++] & META)
			return(YES);

	return(NO);
}

doshell(comstring,nohalt)
register CHARSTAR comstring;
register int nohalt;
{
	register CHARSTAR shell;

	if((waitpid = fork()) == 0)
	{
		enbint(0);
		doclose();

		setenv();
		shell = varptr("SHELL")->varval;
		if(shell == 0 || shell[0] == CNULL)
			shell = SHELLCOM;
#if iAPX286
		execl(shell, "sh", (nohalt ? "-c" : "-ce"), comstring, 0L);
#else
		execl(shell, "sh", (nohalt ? "-c" : "-ce"), comstring, 0);
#endif
		fatal("Couldn't load Shell");
	}

	return( await() );
}




await()
{
	int intrupt();
	int status;
	int pid;

	enbint(intrupt);
	while( (pid = wait(&status)) != waitpid)
		if(pid == -1)
			fatal("bad wait code");
	waitpid = 0;
	return(status);
}






doclose()	/* Close open directory files before exec'ing */
{
	register OPENDIR od;

	for (od = firstod; od != 0; od = od->nextopendir)
		if (od->dirfc != NULL)
			fclose(od->dirfc);
}





doexec(str)
register CHARSTAR str;
{
	register CHARSTAR t;
	register CHARSTAR *p;
	CHARSTAR argv[200];
#if iAPX286
	/*
	 *	The following will get overwritten in execvp (see misc.c)
	 *	if the file cannot be exec'd and an attempt is made to
	 *	run it as a shell script.
	 *	The code in execvp does a *--argv = "sh" ;
	 *	Declaring status as an int that is trashable is ok on
	 *	the vax where an int and a pointer are the same size
	 *	and also on the iAPX286 when make is built small model
	 *	but when built large a pointer is the same size as a long.
	 *	(Surely it would have been cleaner to start the argument
	 *	 list at argv[1])
	 */

	CHARSTAR status ;	/* this can be overwritten by execvp */
#else
	int status;
#endif

	while( *str==BLANK || *str==TAB )
		++str;
	if( *str == CNULL )
		return(-1);	/* no command */

	p = argv;
	for(t = str ; *t ; )
	{
		*p++ = t;
		while(*t!=BLANK && *t!=TAB && *t!=CNULL)
			++t;
		if(*t)
			for( *t++ = CNULL ; *t==BLANK || *t==TAB  ; ++t);
	}

	*p = NULL;

	if((waitpid = fork()) == 0)
	{
		enbint(0);
		doclose();
		setenv();
		execvp(str, argv);
		fatal1("Cannot load %s",str);
	}

	return( await() );
}

touch(force, name)
register int force;
register char *name;
{
        struct stat stbuff;
        char junk[1];
        int fd;

        if( stat(name,&stbuff) < 0)
                if(force)
                        goto create;
                else
                {
                        fprintf(stderr,"touch: file %s does not exist.\n",name);
                        return;
                }
        if(stbuff.st_size == 0)
                goto create;
        if( (fd = open(name, 2)) < 0)
                goto bad;
        if( read(fd, junk, 1) < 1)
        {
                close(fd);
                goto bad;
        }
        lseek(fd, 0L, 0);
        if( write(fd, junk, 1) < 1 )
        {
                close(fd);
                goto bad;
        }
        close(fd);
        return;
bad:
        fprintf(stderr, "Cannot touch %s\n", name);
        return;
create:
        if( (fd = creat(name, 0666)) < 0)
                goto bad;
        close(fd);
}
