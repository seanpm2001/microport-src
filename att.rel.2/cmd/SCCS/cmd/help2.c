/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)	1.1	*/
#include <stdio.h>
#include <sys/types.h>
#include <macros.h>

/*
	Program to locate helpful info in an ascii file.
	The program accepts a variable number of arguments.

	The file to be searched is determined from the argument. If the
	argument does not contain numerics, the search 
	will be attempted on '/usr/lib/help/cmds', with the search key
	being the whole argument.
	If the argument begins with non-numerics but contains
	numerics (e.g, zz32) the file /usr/lib/help/helploc 
	will be checked for a file corresponding to the non numeric prefix,
	That file will then be seached for the mesage. If /usr/lib/help/helploc
	does not exist or the prefix is not found there the search will
	be attempted on '/usr/lib/help/<non-numeric prefix>', 
	(e.g,/usr/lib/help/zz), with the search key being <remainder of arg>, 
	(e.g., 32).
	If the argument is all numeric, or if the file as
	determined above does not exist, the search will be attempted on
	'/usr/lib/help/default' with the search key being
	the entire argument.
	In no case will more than one search per argument be performed.

	File is formatted as follows:

		* comment
		* comment
		-str1
		text
		-str2
		text
		* comment
		text
		-str3
		text

	The "str?" that matches the key is found and
	the following text lines are printed.
	Comments are ignored.

	If the argument is omitted, the program requests it.
*/
#define HELPLOC "/usr/lib/help/helploc"

char	dftfile[]   =   "/usr/lib/help/default";
char	helpdir[]   =   "/usr/lib/help/";
char	hfile[64];
char	*repl();
struct	stat	Statbuf;
FILE	*iop;
char	line [512];


main(argc,argv)
int argc;
char *argv[];
{
	register int i;
	int numerrs=0;

	if (argc == 1)
		exit(numerrs=1);
	else
		for (i = 1; i < argc; i++)
			numerrs += findprt(argv[i]);

	exit((numerrs == (argc-1)) ? 1 : 0);
}


findprt(p)
char *p;
{
	register char *q;
	char key[150];
	char *strcpy();
	if (size(p) > 50)
		return(1);

	q = p;

	while (*q && !numeric(*q))
		q++;

	if (*q == '\0') {		/* all alphabetics */
		strcpy(key,p);
		sprintf(hfile,"%s%s",helpdir,"cmds");
		if (!exists(hfile))
			strcpy(hfile,dftfile);
	}
	else
		if (q == p) {		/* first char numeric */
			strcpy(key,p);
			strcpy(hfile,dftfile);
		}
	else {				/* first char alpha, then numeric */
		strcpy(key,p);		/* key used as temporary */
		*(key + (q - p)) = '\0';
		if(!lochelp(key,hfile))
			sprintf(hfile,"%s%s",helpdir,key);
		else
			cat(hfile,hfile,"/",key,0);
		strcpy(key,q);
		if (!exists(hfile)) {
			strcpy(key,p);
			strcpy(hfile,dftfile);
		}
	}

	if((iop = fopen(hfile,"r")) == NULL)
		return(1);

	/*
	Now read file, looking for key.
	*/
	while ((q = fgets(line,512,iop)) != NULL) {
		repl(line,'\n','\0');		/* replace newline char */
		if (line[0] == '-' && equal(&line[1],key))
			break;
	}

	if (q == NULL) {	/* endfile? */
		fclose(iop);
		return(1);
	}

	printf("\n%s:\n",p);

	while (fgets(line,512,iop) != NULL && line[0] == '-')
		;

	do {
		if (line[0] != '*')
			printf("%s",line);
	} while (fgets(line,512,iop) != NULL && line[0] != '-');

	fclose(iop);
	return(0);
}


/* lochelp finds the file which contains the help messages 
if none found returns 0
*/
lochelp(ky,fi)
	char *ky,*fi; /*ky is key  fi is found file name */
{
	FILE *fp;
	char locfile[513];
	char *hold;
	if(!(fp = fopen(HELPLOC,"r")))
	{
		/*no lochelp file*/
		return(0); 
	}
	while(fgets(locfile,512,fp)!=NULL)
	{
		hold=(char *)strtok(locfile,"\t ");
		if(!(strcmp(ky,hold)))
		{
			hold=(char *)strtok(0,"\n");
			strcpy(fi,hold); /* copy file name to fi */
			fclose(fp);
			return(1); /* entry found */
		}
	}
	fclose(fp);
	return(0); /* no entry found */
}
