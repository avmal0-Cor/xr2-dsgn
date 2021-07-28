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

 #ifdef _WIN32_WCE
  #define _WIDEN(x) _SYSCH(x)

   #define assert(test)	((test) ? (void)0 \
	: _CSTD _Assert(_WIDEN(__FILE__) _WIDEN(":") _WIDEN(_STRIZE(__LINE__)) \
		_WIDEN(" ") _WIDEN(_STRIZE(test))))

_C_LIB_DECL
void _Assert(const _Sysch_t *);
_END_C_LIB_DECL

 #else /* _WIN32_WCE */

 #if defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__

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

   #if defined(__CELL_ASSERT__) || defined(__SPURS_JOB__)
   #define assert(test)	((test) ? (void)0 \
    : _CSTD _SCE_Assert(__FILE__ ":" _STRIZE(__LINE__) " " #test, _FUNNAME))
   #else /* __CELL_ASSERT__ || __SPURS_JOB__ */
   #define assert(test)	((test) ? (void)0 \
    : _CSTD _Assert(__FILE__ ":" _STRIZE(__LINE__) " " #test, _FUNNAME))
   #endif /* __CELL_ASSERT__ || __SPURS_JOB__ */

  #endif /* _VERBOSE_DEBUGGING */

#ifndef _ASSERT_H_DECL
#define _ASSERT_H_DECL
_C_LIB_DECL
void _Assert(const char *, const char *);
void _SCE_Assert(const char *, const char *);
_END_C_LIB_DECL
#endif /* _ASSERT_H_DECL */
 #endif /* _WIN32_WCE */

 #endif /* NDEBUG */
_C_STD_END

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
