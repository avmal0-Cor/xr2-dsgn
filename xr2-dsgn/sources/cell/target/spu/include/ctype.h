/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* ctype.h standard header */
#ifndef _CTYPE
#define _CTYPE
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN

		/* _Ctype code bits */
#define _XB		0x400 /* extra blank */
#define _XA		0x200 /* extra alphabetic */
#define _XS		0x100 /* extra space */
#define _BB		0x80 /* BEL, BS, etc. */
#define _CN		0x40 /* CR, FF, HT, NL, VT */
#define _DI		0x20 /* '0'-'9' */
#define _LO		0x10 /* 'a'-'z' */
#define _PU		0x08 /* punctuation */
#define _SP		0x04 /* space */
#define _UP		0x02 /* 'A'-'Z' */
#define _XD		0x01 /* '0'-'9', 'A'-'F', 'a'-'f' */

_C_LIB_DECL

_INLINE_DEF int isalnum(int);
_INLINE_DEF int isalpha(int);
_INLINE_DEF int iscntrl(int);
_INLINE_DEF int isdigit(int);
_INLINE_DEF int isgraph(int);
_INLINE_DEF int islower(int);
_INLINE_DEF int isprint(int);
_INLINE_DEF int ispunct(int);
_INLINE_DEF int isspace(int);
_INLINE_DEF int isupper(int);
_INLINE_DEF int isxdigit(int);
_INLINE_DEF int tolower(int);
_INLINE_DEF int toupper(int);

 #if _HAS_C9X
int isblank(int);
 #endif /* _IS_C9X */

_END_C_LIB_DECL

#include <bits/sce_ctype.h>

_C_STD_END

#endif /* _CTYPE */

#ifdef _STD_USING
using _CSTD isalnum; using _CSTD isalpha; using _CSTD iscntrl;
using _CSTD isdigit; using _CSTD isgraph; using _CSTD islower;
using _CSTD isprint; using _CSTD ispunct; using _CSTD isspace;
using _CSTD isupper; using _CSTD isxdigit; using _CSTD tolower;
using _CSTD toupper;

 #if _HAS_C9X
using _CSTD isblank;
 #endif /* _IS_C9X */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
