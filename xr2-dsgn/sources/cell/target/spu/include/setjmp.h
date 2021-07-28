/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _SETJMP_H_
#define _SETJMP_H_

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN
_C_LIB_DECL

typedef	int jmp_buf[51*4] __attribute__ ((__aligned__(16)));

void	longjmp(jmp_buf __jmpb, int __retval);
int	setjmp(jmp_buf __jmpb);

_END_C_LIB_DECL
_C_STD_END

#endif /* _SETJMP_H_ */

#if defined(_STD_USING)

using _CSTD jmp_buf;
using _CSTD longjmp;
using _CSTD setjmp;

#endif
