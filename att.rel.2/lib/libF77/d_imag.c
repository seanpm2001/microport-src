/*      Copyright (c) 1985 AT&T */
/*        All Rights Reserved   */

/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     */
/*      The copyright notice above does not evidence any        */
/*      actual or intended publication of such source code.     */

/* iAPX286 @(#)d_imag.c	1.3 85/09/16 */
/*	@(#)d_imag.c	1.2	*/
#include "complex"

double d_imag(z)
dcomplex *z;
{
return(z->dimag);
}
