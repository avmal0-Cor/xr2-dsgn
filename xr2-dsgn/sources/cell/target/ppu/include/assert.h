/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* assert.h standard header */
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN

#undef assert	/* remove existing definition */

#ifdef NDEBUG
 #define assert(test)	((void)0)

#else /* NDEBUG */
  #define _STRIZE(x)	_VAL(x)
  #define _VAL(x)	#x

 #if 199901L <= __STDC_VERSION__

  #ifdef __cplusplus
   #define _FUNNAME	0

  #else /* __cplusplus */
   #define _FUNNAME	__func__
  #endif /* __cplusplus */

 #else /* 199901L <= __STDC_VERSION__ */
  #define _FUNNAME	0
 #endif /* 199901L <= __STDC_VERSION__ */

  #ifdef _VERBOSE_DEBUGGING
   #include <stdio.h>

   #define assert(test)	((test) ? (void)_CSTD fputs( \
	__FILE__ ":" _STRIZE(__LINE__) " " #test " -- OK\n", stderr) \
	: _CSTD _Assert(__FILE__ ":" _STRIZE(__LINE__) " " #test, \
		_FUNNAME))

  #else /* _VERBOSE_DEBUGGING */

   #ifdef __CELL_ASSERT__
   #define assert(test)	((test) ? (void)0 \
    : _CSTD _SCE_Assert(__FILE__ ":" _STRIZE(__LINE__) " " #test, _FUNNAME))
   #else /* __CELL_ASSERT__ */
   #define assert(test)	((test) ? (void)0 \
    : _CSTD _Assert(__FILE__ ":" _STRIZE(__LINE__) " " #test, _FUNNAME))
   #endif /* __CELL_ASSERT__ */

  #endif /* _VERBOSE_DEBUGGING */

#ifndef _ASSERT_H_DECL
#define _ASSERT_H_DECL
_C_LIB_DECL
void _Assert(const char *, const char *);
void _SCE_Assert(const char *, const char *);
_END_C_LIB_DECL
#endif /* _ASSERT_H_DECL */

 #endif /* NDEBUG */
_C_STD_END

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
