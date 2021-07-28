/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* xmtx.h internal header */
#ifndef _XMTX
#define _XMTX
#include <stdlib.h>
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_LIB_DECL

void _Mtxinit(_Rmtx *);
void _Mtxdst(_Rmtx *);
void _Mtxlock(_Rmtx *);
void _Mtxunlock(_Rmtx *);

 #if !_MULTI_THREAD
  #define _Mtxinit(mtx)
  #define _Mtxdst(mtx)
  #define _Mtxlock(mtx)
  #define _Mtxunlock(mtx)

typedef char _Once_t;

  #define _Once(cntrl, func)	if (*(cntrl) == 0) (func)(), *(cntrl) = 2
  #define _ONCE_T_INIT	0

 #elif _WIN32_C_LIB
typedef long _Once_t;

void _Once(_Once_t *, void (*)(void));
  #define _ONCE_T_INIT	0

 #elif _POSIX_C_LIB

/* LV2-MODIFIED */
/*
 * Lv-2 specific implementation for the
 * multithread supported
 */
   
  #if defined(__PPC64__)

typedef long _Once_t;
void _Once(_Once_t *, void (*)(void));
  #define _ONCE_T_INIT	0

  #else
/* LV2-MODIFIED END */

  #include <setjmp.h>
  #include <time.h>
  #include <pthread.h>

typedef pthread_once_t _Once_t;

  #define _Once(cntrl, func)	pthread_once(cntrl, func)
  #define _ONCE_T_INIT	PTHREAD_ONCE_INIT

/* LV2-MODIFIED */
    #endif /* __ppc64__ */
/* LV2-MODIFIED END */

 #else /* library type */
  #error unknown library type
 #endif /* library type */

_END_C_LIB_DECL
#endif /* _XMTX */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
