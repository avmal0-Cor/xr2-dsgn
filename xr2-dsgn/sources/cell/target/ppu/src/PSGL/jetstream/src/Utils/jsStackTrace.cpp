/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include "PSGL/Utils.h"
#include "PSGL/jsStackTrace.h"

#ifdef JS_PLATFORM_LINUX
#include <execinfo.h>
#endif

static jsStackTrace *g_lastStackTrace = NULL;
static char          jsStackTraceBuffer[512];

void jsGetStackTrace( std::string& stackString )
{
    int n = 10; // number of stack frames


#ifdef JS_PLATFORM_CELL
    // Register r1 contains the stack pointer
    // framepointer[1] will contain the back chain
    // framepoitner[5] will contain the link register
    // Note:  Even though pointers are 32bit,
    //        the stack frame seems to be 64bit.
    //
    short count = 0;
    void **framepointer;
    register void **fp asm( "r1" );

    framepointer = fp;

    count = 0;
#ifdef JS_DEBUG
    int iCapBefore = stackString.capacity();
#endif
    stackString.erase( stackString.begin(), stackString.end() );
#ifdef JS_DEBUG
    JS_ASSERT( iCapBefore == ( int )stackString.capacity() );
#endif
    stackString.reserve( 512 );
    if ((( void ** )NULL != framepointer ) && ((( void ** )( -1U ) ) >= framepointer ) )
    {
        while ( count < n )
        {
            if ((( void * )NULL == framepointer[1] ) ||
                    (( void * )NULL == framepointer[5] ) )
            {
                break;
            }

            long address = ( long ) framepointer[5];
            snprintf( jsStackTraceBuffer, 512, "0x%08x (level #%d)\n", ( unsigned int )address, count );
            stackString += jsStackTraceBuffer;

            count++;

            // let's point up the chain
            framepointer = ( void ** )framepointer[1];
        }
    }
#endif
#ifdef JS_PLATFORM_WIN32
#define BACK_TRACE(N) do { \
	if (n>=N) \
	{ \
		long address=(long)__builtin_return_address(N); \
		snprintf(jsStackTraceBuffer,512,"0x%0lx (level " #N ")",address); \
        stackString += jsStackTraceBuffer; \
		if (address==0) return; \
	} \
} while (0)

    BACK_TRACE( 0 );
    BACK_TRACE( 1 );
    BACK_TRACE( 2 );
    BACK_TRACE( 3 );
    BACK_TRACE( 4 );
    BACK_TRACE( 5 );
    BACK_TRACE( 6 );
    BACK_TRACE( 7 );
    BACK_TRACE( 8 );
    BACK_TRACE( 9 );
    BACK_TRACE( 10 );
    BACK_TRACE( 11 );
    BACK_TRACE( 12 );
    BACK_TRACE( 13 );
    BACK_TRACE( 14 );
    BACK_TRACE( 15 );
    BACK_TRACE( 16 );
    BACK_TRACE( 17 );
    BACK_TRACE( 18 );
    BACK_TRACE( 19 );
#endif
#ifdef JS_PLATFORM_LINUX
    void *array[n];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace( array, n );
    strings = backtrace_symbols( array, size );

    for ( i = 0; i < size; i++ )
    {
        snprintf( jsStackTraceBuffer, 512, "%s\n", strings[i] );
        stackString += jsStackTraceBuffer;
    }

    free( strings );
#endif
}

jsStackTrace::jsStackTrace()
{
    g_lastStackTrace = this;
    jsGetStackTrace( m_StackString );
}

jsStackTrace::~jsStackTrace()
{
    if ( g_lastStackTrace == this )
    {
        g_lastStackTrace = NULL;
    }
}

const char* jsStackTrace::print( void )
{
    g_lastStackTrace = this;
    return m_StackString.c_str();
}

jsStackTrace *jsNewStackTrace( void )
{
    jsStackTrace *new_stack = NULL;
    new_stack = new jsStackTrace();
    return new_stack;
}

void jsDeleteStackTrace( jsStackTrace *stack_trace )
{
    if ( stack_trace == NULL )
    {
        return;
    }

    delete stack_trace;
}

const char *jsPrintStackTrace( jsStackTrace *stack_trace )
{
    if ( stack_trace == NULL )
    {
        return NULL;
    }

    return stack_trace->print();
}

jsStackTrace *jsGetLastStackTrace( void )
{
    return g_lastStackTrace;
}

