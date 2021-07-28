/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __CELL_RTC_H__
#define __CELL_RTC_H__

#include <stdint.h>
#include <cell/error.h>

#if !defined(SCE_RTC_DONT_INCLUDE_SYSTIME)
#include <sys/time.h>
#endif	/* !defined(SCE_RTC_DONT_INCLUDE_SYSTIME) */

/* librtc error codes */
#include <cell/rtc/error.h>

/* librtc library */
#include <cell/rtc/rtcsvc.h>

#endif /* __CELL_RTC_H__ */
