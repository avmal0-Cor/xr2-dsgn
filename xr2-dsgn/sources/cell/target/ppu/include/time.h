/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* time.h standard header */
#ifndef _TIME
#define _TIME
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h> /* reffer definition of size_t */

_C_STD_BEGIN

		/* macros */
#ifndef NULL
 #define NULL	_NULL
#endif /* NULL */

#define CLOCKS_PER_SEC	_CPS

		/* type definitions */

 #if !defined(_CLOCK_T) && !defined(__clock_t_defined) \
	&& !defined(_BSD_CLOCK_T_DEFINED_)
  #define _CLOCK_T
  #define __clock_t_defined
  #define _BSD_CLOCK_T_DEFINED_
  #define _STD_USING_CLOCK_T

// LV2_MODIFIED
typedef int64_t clock_t;

 #endif /* !defined(_CLOCK_T) etc. */

 #if !defined(_TIME_T) && !defined(__time_t_defined) \
	&& !defined(_BSD_TIME_T_DEFINED_)
  #define _TIME_T
  #define __time_t_defined
  #define _BSD_TIME_T_DEFINED_
  #define _STD_USING_TIME_T

// LV2_MODIFIED
typedef int64_t time_t;

 #endif /* !defined(_TIME_T) etc. */

 #ifndef _TM_DEFINED
  #define _TM_DEFINED
struct tm
	{	/* date and time components */
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	};
 #endif /* _TM_DEFINED */

 #if _POSIX_C_LIB

  #if !defined(_CLOCKID_T) && !defined(__clockid_t_defined)
   #define _CLOCKID_T
   #define __clockid_t_defined
   #define _STD_USING_CLOCKID_T
typedef int clockid_t;
  #endif /* !defined(_CLOCKID_T) && !defined(__clockid_t_defined) */

 #endif /* _POSIX_C_LIB */

_EXTERN_C	/* low-level functions */
time_t time(time_t *);
_END_EXTERN_C

_C_LIB_DECL	/* declarations */
char * asctime(const struct tm *);
clock_t clock(void);
char * ctime(const time_t *);
double difftime(time_t, time_t);
struct tm * gmtime(const time_t *);
struct tm * localtime(const time_t *);
time_t mktime(struct tm *);
size_t strftime(char *_Restrict, size_t,
	const char *_Restrict, const struct tm *_Restrict);
_END_C_LIB_DECL
_C_STD_END
#endif /* _TIME */

 #if defined(_STD_USING)

  #ifdef _STD_USING_CLOCK_T
using _CSTD clock_t;
  #endif /* _STD_USING_CLOCK_T */

  #ifdef _STD_USING_TIME_T
using _CSTD time_t;
  #endif /* _STD_USING_TIME_T */

  #ifdef _STD_USING_CLOCKID_T
using _CSTD clockid_t;
  #endif /* _STD_USING_CLOCKID_T */

using _CSTD tm;
using _CSTD asctime; using _CSTD clock; using _CSTD ctime;
using _CSTD difftime; using _CSTD gmtime; using _CSTD localtime;
using _CSTD mktime; using _CSTD strftime; using _CSTD time;
 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
