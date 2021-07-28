/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Debug macros
 */


#ifndef _JSDebug_h
#define _JSDebug_h

#include <PSGL/export.h>
#include <PSGL/psgl.h>

#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
extern "C"
{
#endif

#define JS_COMPILE_TIME_ASSERT(exp) { switch (0) { case 0: case (exp):; } }

#ifdef JS_DEBUG
    PSGL_EXPORT void _jsAssert( const char *file, unsigned int line, const char *cond );

#ifdef JS_PLATFORM_CELL
#ifdef __SNC__
#define JS_BREAK __builtin_snpause()
#else
#define JS_BREAK __asm__ volatile ( "tw 31,1,1" )
#endif
#else
#define JS_BREAK
#endif

#define JS_ASSERT(cond) \
	if (!(cond)) do \
	{ \
	_jsAssert(__FILE__,__LINE__,#cond); \
	JS_BREAK; \
	} while (0)

#else
#define JS_ASSERT(cond) ((void)0)
#endif

#define JS_TRACE() _JS_REPORT_EXTRA(PSGL_REPORT_DEBUG,"PSGL: %s:%d %s\n",__FILE__,__LINE__,__FUNCTION__)
    void _jsBackTrace( unsigned int n );

#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif	// _JSDebug_h
