/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Platform-specific performance counter header - Cell platform
 */ 

#ifndef _PLATFORM_PERF_COUNTER_h
#define _PLATFORM_PERF_COUNTER_h

static inline jsUint64 _jsGetPerformanceCounter()
{
    jsUint64 time;
    asm volatile( "mftb %0" : "=r"( time ) );
    return time;
}

#endif
