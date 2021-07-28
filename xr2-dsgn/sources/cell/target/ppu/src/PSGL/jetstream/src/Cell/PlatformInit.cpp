/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Platform-specific initialization for Cell platform
 */

#include <PSGL/Debug.h>
#include <PSGL/readelf.h>
#include <PSGL/Thread.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/private.h>
#include <spu/spuInterface.h>
#include <string.h>
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////
// fast_memcpy - leverage different copy strategies depending on the
// size and alignment of data.

static inline void memcpy_word( void * __restrict to, const void * __restrict from, unsigned int n )
{
    JS_ASSERT(( JS_PTR_TO_EA( to )&3 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( from )&3 ) == 0 );
    JS_ASSERT(( n&3 ) == 0 );
    int count = n / 4;
    unsigned int *toLong = ( unsigned int * )to;
    unsigned int *fromLong = ( unsigned int * )from;
    for ( int i = 0;i < count;++i ) *toLong++ = *fromLong++;
}

static inline void memcpy_dword( void * __restrict to, const void * __restrict from, unsigned int n )
{
    JS_ASSERT(( JS_PTR_TO_EA( to )&7 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( from )&7 ) == 0 );
    JS_ASSERT(( n&7 ) == 0 );
    int count = n / 8;
    unsigned long long *toLong = ( unsigned long long * )to;
    unsigned long long *fromLong = ( unsigned long long * )from;
    for ( int i = 0;i < count;++i ) *toLong++ = *fromLong++;
}

static inline void memcpy_qword( void* __restrict to, const void* __restrict from, size_t n )
{
    JS_ASSERT(( JS_PTR_TO_EA( to )&15 ) == 0 );
    JS_ASSERT(( JS_PTR_TO_EA( from )&15 ) == 0 );
    JS_ASSERT(( n&15 ) == 0 );
    const int count = n / 16;
    vector float* dptr = ( vector float* )to;
    vector float* sptr = ( vector float* )from;
    for ( int i = 0; i < count; ++i )
        *dptr++ = *sptr++;
}

#if 0
void fast_memcpy( void * __restrict to, const void * __restrict from, unsigned int n )
{
  memcpy( to, from, n );
}
#else

#define _JS_ASYNC_MIN_SIZE 128
extern int _jsMaxSpu;
void fast_memcpy( void * __restrict to, const void * __restrict from, unsigned int n )
{
    _JS_PERFORMANCE_START( FastMemcpyCounter );
    jsEA fromEA = JS_PTR_TO_EA( from );
    jsEA toEA = JS_PTR_TO_EA( to );
    unsigned int size;
    if (( fromEA&15 ) == 0 && ( toEA&15 ) == 0 && n >= _JS_ASYNC_MIN_SIZE  && _jsMaxSpu>0)
    {
        _JS_PERFORMANCE_START( SPUMemcpyCounter );
        size = n & ~15;
        _jsAsyncCopy( to, from, size );
        _jsAsyncCopyFinish();
        _JS_PERFORMANCE_STOP( SPUMemcpyCounter );
    }
    else
    {
        if ((( fromEA&15 ) == 0 ) && (( toEA&15 ) == 0 ) )
        {
            size = n & ~15;
            memcpy_qword( to, from, size );
        }
        else if ((( fromEA&7 ) == 0 ) && (( toEA&7 ) == 0 ) )
        {
            size = n & ~7;
            memcpy_dword( to, from, size );
        }
        else if ((( fromEA&3 ) == 0 ) && (( toEA&3 ) == 0 ) )
        {
            size = n & ~3;
            memcpy_word( to, from, size );
        }
        else
        {
            size = n;
            memcpy( to, from, n );
        }
    }
    if ( JS_UNLIKELY( n > size ) )
    {
        memcpy(( char * )to + size, ( const char * )from + size, n - size );
    }
    _JS_PERFORMANCE_STOP( FastMemcpyCounter );
}
#endif

///////////////////////////////////////////////////////////////////////////
// Platform-specific initialization for Cell processor:
// manage allocation/free of SPUs, and optional debugging console.

// raw SPU management; implemented in spuInterface.cpp
int _jsInitSPUs( GLint maxSpus, GLboolean initializeSPUs );
void _jsFinishSPUs();

#define _JS_MAX_SPUS_DEFAULT 1

#ifdef JS_REPORT
// debugging message console; implemented in PlatformReport.cpp
extern void _jsInitConsole( GLuint enable );
extern void _jsExitConsole();
extern char* jsVersion;
#endif


void _jsPlatformInit( PSGLinitOptions* options )
{
    // option defaults
    GLuint maxSPUs = _JS_MAX_SPUS_DEFAULT;
    GLboolean initializeSPUs = GL_TRUE;
    int consoleEnabled = 0;

    if ( options != NULL )
    {
        if ( options->enable & PSGL_INIT_MAX_SPUS )
            maxSPUs = options->maxSPUs;
        if ( options->enable & PSGL_INIT_INITIALIZE_SPUS)
            initializeSPUs = options->initializeSPUs;
        if ( options->enable & PSGL_INIT_ERROR_CONSOLE )
            consoleEnabled = options->errorConsole;

        if (maxSPUs==0)
            initializeSPUs = GL_FALSE;
    }

#ifdef JS_REPORT
    _jsInitConsole( consoleEnabled );
    _JS_REPORT_EXTRA( PSGL_REPORT_VERSION, "%s", jsVersion );
#endif

    _jsInitSPUs( maxSPUs, initializeSPUs );
}


void _jsPlatformExit()
{
#ifdef JS_REPORT
    _jsExitConsole();
#endif

    _jsFinishSPUs();
}
