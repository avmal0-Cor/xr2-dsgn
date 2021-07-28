/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _UTIME_H_
#define _UTIME_H_

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <time.h> /* Lv2 changes */

struct utimbuf {
  time_t actime ;
  time_t modtime ;
} ;

_C_STD_BEGIN
_C_LIB_DECL
int   utime(const char *,const struct utimbuf *) ;
_END_C_LIB_DECL
_C_STD_END

#if defined(_STD_USING)
using _CSTD utime;
#endif

#endif /* !defined(_UTIME_H_) */
