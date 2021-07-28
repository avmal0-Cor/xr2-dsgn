/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2007 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef CGB_REPORT_HEADER
#define CGB_REPORT_HEADER

#include <stdio.h>
#define ANSI            /* Comment out for UNIX version     */
#ifdef ANSI             /* ANSI compatible version          */
#include <stdarg.h>
#else                   /* UNIX compatible version          */
#include <varargs.h>
#endif

#ifdef PS3

//PS3 specifics
#define TST_REPORT(txt) printf txt;
#ifdef __SNC__
#define TST_BREAK __builtin_snpause();
#else
#define TST_BREAK __asm__ volatile ( "tw 31,1,1" );
#endif

#else
#ifdef WIN32

//Win32 specifics
#include <windows.h>

//#define TST_BREAK _asm int 3
//gcc doesn't support that and libcgc.a for windows ( cygwin lib ) is built with it so disable for now
#define TST_BREAK

class CellCgeDbgStr
{
public:
	char buf[2048];
	void Format(const char* expr,...)
	{
		va_list arguments;
		va_start( arguments ,expr);
		_vsnprintf( buf, sizeof(buf)-1,expr, arguments );
	}
};

#define TST_REPORT(text) { \
	CellCgeDbgStr str; \
	str.Format text; \
	printf(str.buf); \
}

#else

//Linux specifics
#define TST_REPORT(txt) printf txt;
#define TST_BREAK  // how to define this on Linux?

#endif // #ifdef WIN32
#endif // #ifdef PS3

//Common
#define TST_ASSERT(assertion) { \
	if (!(assertion)) \
	{	\
		TST_REPORT(("assertion failed : " #assertion "\n"))	\
		TST_BREAK \
	}	\
}

#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(assertion) TST_ASSERT(assertion)

#ifdef REPORT
#undef REPORT
#endif
#define REPORT(txt) TST_REPORT(txt)

#endif // #ifndef CGB_REPORT_HEADER

