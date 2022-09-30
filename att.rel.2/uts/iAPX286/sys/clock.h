/*      Copyright (c) 1985 AT&T */
/*        All Rights Reserved   */

/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     */
/*      The copyright notice above does not evidence any        */
/*      actual or intended publication of such source code.     */

/* @(#)clock.h	1.6 */
/*
** definitions for the 8254 CTC 
*/

/*
** port addresses
*/
#define	CNT0MODE	0x00D0		/* counter 0 mode	*/
#define	CNT1MODE	0x00D2		/* counter 1 mode	*/
#define	CNT2MODE	0x00D4		/* counter 2 mode	*/
#define	CTCMODE		0x00D6		/* modes for all cntrs	*/

/*
** definitions for the control word
*/
#define	BCDCNT		0x01		/* bcd counter		*/
#define	MODE0		0x00		/* intr on terminal cnt	*/
#define	MODE1		0x02		/* hardware one shot	*/
#define	MODE2		0x04		/* rate generator	*/
#define	MODE3		0x06		/* square wave mode	*/
#define	MODE4		0x08		/* software triggered	*/
#define	MODE5		0x0A		/* hardware triggered	*/
#define	CNTRLTCH	0x00		/* counter latch cmd	*/
#define	LSBONLY		0x10		/* r/w lsb only		*/
#define	MSBONLY		0x20		/* r/w msb only		*/
#define	LSBMSB		0x30		/* r/w lsb then msb	*/
#define	CNT0		0x00		/* select counter 0	*/
#define	CNT1		0x40		/* select counter 1	*/
#define	CNT2		0x80		/* select counter 2	*/
#define	RDBACK		0xC0		/* read-back cmd	*/

/*
** combinatorial defines for the "real time" clock
*/
#define	RTCMODE		MODE2 | LSBMSB | CNT0

/*
** combinatorial defines for the baud rate generators
*/
#define	BRGA		MODE3 | LSBMSB | CNT2	/* 8274 channel A	*/
#define	BRGB		MODE3 | LSBMSB | CNT1	/* 8274 channel B	*/

/*
** value to write to CTC to get a 60Hz clock
** The minus 1 is to adjust for the clock to reload itself ( 1 clock cycle )
*/
#define	RTCCNT		20499		/* ( 1.23Mhz clock / 60Hz ) - 1	*/

#define	SECHR	(60*60)	/* seconds/hr */
#define	SECDAY	(24*SECHR)	/* seconds/day */
#define	SECYR	(365*SECDAY)	/* seconds/common year */
