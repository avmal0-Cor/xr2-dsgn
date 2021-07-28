/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Stack trace helpers.
 */

#ifndef _JSSTACKTRACE_H_
#define _JSSTACKTRACE_H_

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#include <string>

class jsStackTrace
{
public:
    jsStackTrace();
    ~jsStackTrace();
    const char* print( void );

protected:
    std::string m_StackString;
};
#else
typedef struct _jsStackTrace jsStackTrace;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void jsGetStackTrace( std::string& s );
    jsStackTrace *jsNewStackTrace( void );
    void jsDeleteStackTrace( jsStackTrace *stack_trace );
    const char *jsPrintStackTrace( jsStackTrace *stack_trace );
    jsStackTrace *jsGetLastStackTrace( void );

#ifdef __cplusplus
}
#endif


#endif
