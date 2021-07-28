/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* stddef.h standard header */
#ifndef _STDDEF
#define _STDDEF
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

		/* macros */
#ifndef NULL
 #define NULL	_NULL
#endif /* NULL */

#ifndef offsetof
 #ifdef __SNC__
  #define offsetof(T, member) ((_Sizet)__INTADDR__(&(((T *)0)->member)))

 #elif __GNUC__
  #define offsetof(T, member) (__builtin_offsetof (T, member))

 #elif __EDG__
  #define offsetof(T, member) ((_Sizet)__INTADDR__(&(((T *)0)->member)))

 #else /* __EDG__ */
  #define offsetof(T, member)	((_CSTD size_t)&(((T *)0)->member))
 #endif /* __EDG__ */

#endif /* offsetof */

		/* type definitions */

 #if !defined(_PTRDIFF_T) && !defined(_PTRDIFFT)
  #define _PTRDIFF_T
  #define _PTRDIFFT
  #define _STD_USING_PTRDIFF_T
typedef _Ptrdifft ptrdiff_t;
 #endif /* !defined(_PTRDIFF_T) && !defined(_PTRDIFFT) */

#ifndef _WCHART
 #define _WCHART
typedef _Wchart wchar_t;
#endif /* _WCHART */

_C_STD_END
#endif /* _STDDEF */

 #if defined(_STD_USING)

  #ifdef _STD_USING_PTRDIFF_T
using _CSTD ptrdiff_t;
  #endif /* _STD_USING_PTRDIFF_T */

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
