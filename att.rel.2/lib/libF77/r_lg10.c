/*      Copyright (c) 1985 AT&T */
/*        All Rights Reserved   */

/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     */
/*      The copyright notice above does not evidence any        */
/*      actual or intended publication of such source code.     */

/* iAPX286 @(#)r_lg10.c	1.3 85/09/16 */
/*	@(#)r_lg10.c	1.2	*/
#define log10e 0.43429448190325182765

double r_lg10(x)
float *x;
{
double log();

return( log10e * log(*x) );
}
