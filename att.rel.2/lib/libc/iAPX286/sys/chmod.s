	.file	"chmod.s"
/	@(#)	1.1
	.text
/
/ chmod(path, mode);
/ char	*path;
/ int	mode;

#include "lib.s.h"
	.globl	chmod

chmod:
	push	%bp		/ enter subroutine
	mov	%sp,%bp
	MCOUNT			/ call subroutine entry counter if profiling

#if LARGE_M | HUGE_M | COMPACT_M
	push	PARAM(3)	/ mode
	push	PARAM(2)	/ <s>path
	push	PARAM(1)	/ path
#else
	push	PARAM(2)	/ mode
	push	%ds		/ <s> path
	push	PARAM(1)	/ path
#endif

	push	$CHMOD
	lcall	SYSCALL		/ call gate into OS
	jnc	noerror
	JMP	_cerror
noerror:
	LVRET			/ restore stack frame and return to caller
