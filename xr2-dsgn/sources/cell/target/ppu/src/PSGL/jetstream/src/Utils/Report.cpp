/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Report functions.
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

static PSGLreportFunction _jsReportFunction = _jsDefaultReport;
static GLuint _jsReportClassMask = ~PSGL_REPORT_MEMORY_INFO_CLASSES;
static GLboolean _jsShowEnums = GL_FALSE;

JSreportInfo reportInfos[PSGL_REPORT_MAX+1];

typedef struct
{
    GLenum enumValue;
    GLuint reportClass;
    const char *enumName;
    const char *defaultMessage;
}
ReportInfoInit;

static ReportInfoInit reportInfoInit[] =
    {
#define REPORT(REPORT_ENUM,CLASS,DEFAULT_STRING) {REPORT_ENUM,CLASS,#REPORT_ENUM,DEFAULT_STRING}
        //      ENUM									CLASS								DEFAULT TEXT
        REPORT( PSGL_REPORT_UNKNOWN,							PSGL_REPORT_CLASS_UNKNOWN,				NULL ),
        REPORT( PSGL_REPORT_MISSING_STATE,					PSGL_REPORT_CLASS_WARNING,				NULL ),
        REPORT( PSGL_REPORT_ASSERT,							PSGL_REPORT_CLASS_INTERNAL_ERROR,		"JS_ASSERT" ),
#ifdef JS_REPORT
        REPORT( PSGL_REPORT_VERSION,							PSGL_REPORT_CLASS_INFO,					NULL ),
        REPORT( PSGL_REPORT_DEBUG,							PSGL_REPORT_CLASS_DEBUG,				"Debug" ),
        REPORT( PSGL_REPORT_GL_ERROR,						PSGL_REPORT_CLASS_API_ERROR,			"GL error" ),
        REPORT( PSGL_REPORT_VERTEX_SLOW_PATH,				PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL,	"Using slow vertex data path" ),
        REPORT( PSGL_REPORT_VERTEX_DATA_WARNING,				PSGL_REPORT_CLASS_PERFORMANCE_INFO,		"Sub-optimal vertex data" ),
        REPORT( PSGL_REPORT_COPY_TEXTURE_SLOW_PATH,			PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL,	"Using slow copy-texture path" ),
        REPORT( PSGL_REPORT_COPY_TEXTURE_WARNING,			PSGL_REPORT_CLASS_PERFORMANCE_INFO,		"Sub-optimal copy-texture path" ),
        REPORT( PSGL_REPORT_TEXTURE_COPY_BACK,				PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL,	"Copying back a texture from GPU" ),
        REPORT( PSGL_REPORT_TEXTURE_REALLOC,					PSGL_REPORT_CLASS_PERFORMANCE_INFO,		"Re-allocating a texture" ),
        REPORT( PSGL_REPORT_FP32_FILTERING,					PSGL_REPORT_CLASS_WARNING,				"Blending or filtering not supported for FP32" ),
        REPORT( PSGL_REPORT_TEXTURE_INCOMPLETE,				PSGL_REPORT_CLASS_WARNING,				"Incomplete texture" ),
        REPORT( PSGL_REPORT_FRAMEBUFFER_INCOMPLETE,			PSGL_REPORT_CLASS_WARNING,				"Incomplete framebuffer object" ),
        REPORT( PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED,			PSGL_REPORT_CLASS_WARNING,				"Unsupported framebuffer object configuration" ),
        REPORT( PSGL_REPORT_CG_ERROR,						PSGL_REPORT_CLASS_API_ERROR,			"Cg error" ),
        REPORT( PSGL_REPORT_CG_PROFILE_DEPRECATED,			PSGL_REPORT_CLASS_WARNING,			"Deprecated Cg profile" ),
        REPORT( PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS,		PSGL_REPORT_CLASS_USER_CRITICAL_ERROR,	"Drawing using data out of buffer object bounds" ),
        REPORT( PSGL_REPORT_GPU_MEMORY_ALLOC,				PSGL_REPORT_CLASS_GPU_MEMORY_INFO,		"Allocating GPU memory" ),
        REPORT( PSGL_REPORT_GPU_MEMORY_FREE,					PSGL_REPORT_CLASS_GPU_MEMORY_INFO,		"Freeing GPU memory" ),
        REPORT( PSGL_REPORT_GPU_MEMORY_ASYNC_FREE,			PSGL_REPORT_CLASS_GPU_MEMORY_INFO,		"Asynchronously freeing GPU memory" ),
        REPORT( PSGL_REPORT_HOST_MEMORY_ALLOC,				PSGL_REPORT_CLASS_HOST_MEMORY_INFO,		"Allocating host memory" ),
        REPORT( PSGL_REPORT_HOST_MEMORY_FREE,				PSGL_REPORT_CLASS_HOST_MEMORY_INFO,		"Freeing host memory" ),
        REPORT( PSGL_REPORT_HOST_MEMORY_ASYNC_FREE,			PSGL_REPORT_CLASS_HOST_MEMORY_INFO,		"Asynchronously freeing host memory" ),
        REPORT( PSGL_REPORT_MALLOC_MEMORY_ALLOC,				PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO,	"Allocating malloc memory" ),
        REPORT( PSGL_REPORT_MALLOC_MEMORY_REALLOC,			PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO,	"Reallocating malloc memory" ),
        REPORT( PSGL_REPORT_MALLOC_MEMORY_FREE,				PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO,	"Freeing malloc memory" ),
        REPORT( PSGL_REPORT_TEXTURE_CONVERSION,				PSGL_REPORT_CLASS_PERFORMANCE_INFO,		"Converting texture color format" ),
        REPORT( PSGL_REPORT_RESC,							PSGL_REPORT_CLASS_PERFORMANCE_INFO,		"Using GPU-based resolution conversion (RESC) video mode" ),
        REPORT( PSGL_REPORT_RESC_FLIP_ERROR,				PSGL_REPORT_CLASS_WARNING,				"Resc returned error code during flip" ),
		REPORT( PSGL_REPORT_COMMAND_RECORD_CALLBACK,		PSGL_REPORT_CLASS_WARNING,				"Out of space in User command buffer and no user psglStaticCommandBufferCallback is defined" ),
#endif
    };
const GLuint reportInfoInitCount = sizeof( reportInfoInit ) / sizeof( reportInfoInit[0] );

void _jsReportInit()
{
    for ( GLuint i = 0;i < reportInfoInitCount;++i )
    {
        GLenum v = reportInfoInit[i].enumValue;
        reportInfos[v].reportClass = reportInfoInit[i].reportClass;
        reportInfos[v].enumName = reportInfoInit[i].enumName;
        reportInfos[v].defaultMessage = reportInfoInit[i].defaultMessage;
        reportInfos[v].disable = GL_FALSE;
    }
}

void _jsReportEnableAll()
{
    for ( GLuint i = 0;i < reportInfoInitCount;++i )
    {
        GLenum v = reportInfoInit[i].enumValue;
        reportInfos[v].disable = GL_FALSE;
    }
}
#define _JS_MAX_REPORT_SIZE 1024

void _jsReportExtra( GLenum reportEnum, const char *format, ... )
{
    if ( !_jsReportFunction ) return;

    GLuint index = reportEnum < PSGL_REPORT_MAX ? reportEnum : 0;
    if ( reportInfos[index].disable ) return;

    GLuint reportClass = reportInfos[index].reportClass;

    if (( 1 << reportClass )&_jsReportClassMask )
    {
        char tempString[_JS_MAX_REPORT_SIZE];

        va_list argList;

        va_start( argList, format );
        vsnprintf( tempString, _JS_MAX_REPORT_SIZE, format, argList );
        va_end( argList );
        tempString[_JS_MAX_REPORT_SIZE-1] = 0;

        _jsReportFunction( reportEnum, reportClass, tempString );
    }
}

void _jsReport( GLenum reportEnum )
{
    GLuint index = reportEnum < PSGL_REPORT_MAX ? reportEnum : 0;
    const char *s = reportInfos[index].defaultMessage;
    if ( s ) _jsReportExtra( reportEnum, "%s", s );
    else _jsReportExtra( reportEnum, "Unknown error %d", reportEnum );
}

void _jsDefaultReport( GLenum reportEnum, GLuint reportClass, const char *string )
{
#ifdef JS_REPORT
    GLuint index = reportEnum < PSGL_REPORT_MAX ? reportEnum : 0;
    if ( _jsShowEnums )
    {
        char tempString[_JS_MAX_REPORT_SIZE];
        snprintf( tempString, _JS_MAX_REPORT_SIZE, "Report %s (%d) Class %d: %s", reportInfos[index].enumName, reportEnum, reportClass, string );
        string = tempString;
    }
    _jsPlatformReport( string );
#endif
}

_JS_EXTERN_C void _exit( int ret );

#ifdef JS_DEBUG
PSGL_EXPORT void _jsAssert( const char *file, unsigned int line, const char *cond )
{
    _JS_REPORT_EXTRA( PSGL_REPORT_ASSERT, "JS_ASSERT at %s:%d : %s failed\n", file, line, cond );
    _jsBackTrace( 10 );
    //on CELL the breakpoint is set on the caller, on other platform the program exists here
#ifndef JS_PLATFORM_CELL
    assert( 0 );
    exit( 0 );
#endif
}
#endif

_JS_EXTERN_C PSGL_EXPORT long _jsStopBackTraceAddress;
PSGL_EXPORT long _jsStopBackTraceAddress = 0;

#ifdef JS_PLATFORM_CELL
// - tflynn -
// Register r1 contains the stack pointer
// framepointer[1] will contain the back chain
// framepoitner[5] will contain the link register
// Note:  Even though pointers are 32bit,
//        the stack frame seems to be 64bit.
//
PSGL_EXPORT void _jsBackTrace( unsigned int n )
{
    _jsPlatformReport( "Back trace:" );
    char tempString[_JS_MAX_REPORT_SIZE];
    short count = 0;
    void **framepointer;
    register void **fp asm( "r1" );

    framepointer = fp;

    count = 0;
    if ((( void ** )NULL != framepointer ) && ((( void ** )( -1U ) ) >= framepointer ) )
    {
        while ( count < ( short )n )
        {
            if ((( void * )NULL == framepointer[1] ) ||
                    (( void * )NULL == framepointer[5] ) )
            {
                break;
            }

            long address = ( long ) framepointer[5];
            snprintf( tempString, _JS_MAX_REPORT_SIZE, "0x%0lx (level #%d)", address, count );
            _jsPlatformReport( tempString );

            count++;

            // let's point up the chain
            framepointer = ( void ** )framepointer[1];
        }
    }
}
#else
PSGL_EXPORT void _jsBackTrace( unsigned int n )
{
    _jsPlatformReport( "Back trace:" );
    char tempString[_JS_MAX_REPORT_SIZE];
#define BACK_TRACE(N) do { \
	if (n>=N) \
	{ \
		long address=(long)__builtin_return_address(N); \
		snprintf(tempString,_JS_MAX_REPORT_SIZE,"0x%0lx (level " #N ")",address); \
		_jsPlatformReport(tempString); \
		if (address==_jsStopBackTraceAddress) return; \
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
}
#endif

void _psglDisableReport( GLenum report )
{
    if ( report >= PSGL_REPORT_MAX ) return;
    reportInfos[report].disable = GL_TRUE;
}

void _psglEnableReport( GLenum report )
{
    if ( report >= PSGL_REPORT_MAX ) return;
    reportInfos[report].disable = GL_FALSE;
}

void _psglSetReportClassMask( GLuint reportClassMask )
{
    _jsReportClassMask = reportClassMask;
}

void _psglSetReportFunction( PSGLreportFunction function )
{
    _jsReportFunction = function;
}

void _psglSetDefaultReportParameters( GLboolean showEnums )
{
    _jsShowEnums = showEnums;
}



/**
 * @addtogroup PSGL
 * <!-- was JSReport -->
 *
 * @{
 */

/**
 * @short Returns whether reporting for a selected PSGL error, warning, or other event is enabled.
 *
 * For more information about PSGL reporting, see the corresponding function psglEnableReport()
 *
 * @param report Specifies report to query with one of the following report types:
        <UL>
	<LI><CODE>PSGL_REPORT_VERSION</CODE></LI>
	<LI><CODE>PSGL_REPORT_DEBUG</CODE></LI>
	<LI><CODE>PSGL_REPORT_ASSERT</CODE></LI>
	<LI><CODE>PSGL_REPORT_GL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_MISSING_STATE</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_COPY_BACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_FP32_FILTERING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_CG_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED</CODE></LI>
	<LI><CODE>PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_CONVERSION</CODE></LI>
	<LI><CODE>PSGL_REPORT_RESC</CODE></LI>
	<LI><CODE>PSGL_REPORT_RESC_FLIP_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_COMMAND_RECORD_CALLBACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_UNKNOWN</CODE></LI>
	</UL>
 *
 * @return None
 *
 * @note Reports are generated only if the "debug" version of the library is used.
 *       All reports except memory reports (<CODE>GL_REPORT_*_MEMORY_*</CODE>) are enabled by default.
 *
 * @sa  psglEnableReport
 */

PSGL_EXPORT GLboolean psglIsReportEnabled( GLenum report )
{
    if ( report >= PSGL_REPORT_MAX )
        return GL_FALSE;
    return ( reportInfos[report].disable == GL_FALSE );
}

/**
 * @short Turns off reporting for a selected PSGL error, warning, or other event
 *
 * For more information about PSGL reporting, see the corresponding function psglEnableReport()
 *
 * @param report Specifies report to deactivate with one of the following report types:
        <UL>
	<LI><CODE>PSGL_REPORT_VERSION</CODE></LI>
	<LI><CODE>PSGL_REPORT_DEBUG</CODE></LI>
	<LI><CODE>PSGL_REPORT_ASSERT</CODE></LI>
	<LI><CODE>PSGL_REPORT_GL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_MISSING_STATE</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_COPY_BACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_FP32_FILTERING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_CG_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED</CODE></LI>
	<LI><CODE>PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_CONVERSION</CODE></LI>
    <LI><CODE>PSGL_REPORT_RESC</CODE></LI>
	<LI><CODE>PSGL_REPORT_RESC_FLIP_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_COMMAND_RECORD_CALLBACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_UNKNOWN</CODE></LI>
</UL>
 *
 * @note Reports are generated only if the "debug" version of the library is used.
 *       All reports except memory reports (<CODE>GL_REPORT_*_MEMORY_*</CODE>) are enabled by default.
 *
 * @sa  psglEnableReport
 */

PSGL_EXPORT void psglDisableReport( GLenum report )
{
#ifndef JS_DPM
    _psglDisableReport( report );
#endif
}

/**
 * @short Turns on reporting for a selected PSGL error, warning, or other event
 *
 * psglEnableReport() activates reporting for a given error, warning, or other event.
 * By default, all reports except for memory reports (<CODE>GL_REPORT_*_MEMORY_*</CODE>) are enabled.
 * When an activated event occurs, a bitmask (set by psglSetReportClassMask())
 * is also checked.  If the bitmask is set to allow processing of the event, then
 * the function set by psglSetReportFunction() is invoked.
 *
<TABLE>
<TR>
<TD><CODE>PSGL_REPORT_VERSION</CODE></TD>
<TD>Reported once at the initialization of PSGL. Reports the version of PSGL compiled in.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_DEBUG</CODE>, <CODE>PSGL_REPORT_ASSERT</CODE>,
<CODE>PSGL_REPORT_UNKNOWN</CODE></TD>
<TD>Reported only when a PSGL defect was encountered.
Please contact developer support if you see any of those.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_GL_ERROR</CODE><TD>
<TD>Reported when PSGL needs to trigger a GL error, as per the GLES specification.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_CG_ERROR</CODE></TD>
<TD>Reported when PSGL needs to trigger a Cg error, as per the Cg specification.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_MISSING_STATE</CODE></TD>
<TD>Reported when PSGL needs a fixed function fragment program that was not loaded.
See psglLoadShaderLibrary() for more information.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_VERTEX_SLOW_PATH</CODE></TD>
<TD>Reported when the current vertex data setup requires software per-vertex setup.
An associated <CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE> report gives more information
as to why the slow path was taken.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE></TD>
<TD>Reported when the current vertex data setup requires software per-vertex setup. Includes a
description of why the slow path was taken.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_COPY_TEXTURE_SLOW_PATH</CODE></TD>
<TD>Reported when PSGL cannot use the hardware to copy part of the frame buffer to a texture.
An associated <CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE> report gives more information as to
why the slow path was taken.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE></TD>
<TD>Reported when PSGL cannot use the hardware to copy part of the frame buffer to a texture.
Includes a description of the conditions which triggered the slow path.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_TEXTURE_COPY_BACK</CODE></TD>
<TD>Reported when PSGL needs to copy a texture back from RSX&trade; memory to system memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_TEXTURE_REALLOC</CODE></TD>
<TD>Reported when PSGL needs to reallocate a texture.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_FP32_FILTERING</CODE><TD>
<TD>Reported when an FP32 texture requires an unsupported filtering mode (one that requires blending of texels).  
Includes information about which mode is unsupported, and which mode is used instead.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_TEXTURE_INCOMPLETE</CODE></TD>
<TD>Reported when a texture unit is used with a texture that is not complete.
The texture will be ignored.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_FRAMEBUFFER_INCOMPLETE</CODE></TD>
<TD>Reported when a frame buffer is used that is not complete.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED</CODE></TD>
<TD>Reported when the frame buffer used is not supported by PSGL. The report provides information about what is unsupported.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS</CODE></TD>
<TD>Reported when PSGL tries to draw using data that is out of the object bounds.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_GPU_MEMORY_ALLOC</CODE></TD>
<TD>Reported when PSGL allocates RSX&trade; memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_GPU_MEMORY_FREE</CODE></TD>
<TD>Reported when PSGL frees RSX&trade; memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_GPU_MEMORY_ASYNC_FREE</CODE></TD>
<TD>Reported when PSGL asynchronously frees RSX&trade; memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_HOST_MEMORY_ALLOC</CODE></TD>
<TD>Reported when PSGL allocates host memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_HOST_MEMORY_FREE</CODE></TD>
<TD>Reported when PSGL frees host memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_HOST_MEMORY_ASYNC_FREE</CODE></TD>
<TD>Reported when PSGL asynchronously frees host memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_MALLOC_MEMORY_ALLOC</CODE></TD>
<TD>Reported when PSGL allocates malloc memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_MALLOC_MEMORY_FREE</CODE></TD>
<TD>Reported when PSGL frees malloc memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_MALLOC_MEMORY_REALLOC</CODE></TD>
<TD>Reported when PSGL reallocates malloc memory.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_TEXTURE_CONVERSION</CODE></TD>
<TD>Reported when texture color format conversion occurs.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_RESC</CODE></TD>
<TD>Reported when resolution conversion (RESC) is enabled.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_RESC_FLIP_ERROR</CODE></TD>
<TD>Reported when an error occurs during resolution conversion (RESC) flip.
The report includes the error code returned by <c>cellRescSetConvertAndFlip()</c>.</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_COMMAND_RECORD_CALLBACK</CODE></TD>
<TD>Reported when recording with static command buffers in PSGL.
There is no more space for command in the current command buffer and there is no <c>psglStaticCommandBufferCallback</c> to call to resolve the issue.</TD>
</TR>
</TABLE>




 * @param report Specifies report to activate with one of the following report types:
        <UL>
	<LI><CODE>PSGL_REPORT_VERSION</CODE></LI>
	<LI><CODE>PSGL_REPORT_DEBUG</CODE></LI>
	<LI><CODE>PSGL_REPORT_ASSERT</CODE></LI>
	<LI><CODE>PSGL_REPORT_GL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_MISSING_STATE</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_SLOW_PATH</CODE></LI>
	<LI><CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_COPY_BACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_FP32_FILTERING</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_CG_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_INCOMPLETE</CODE></LI>
	<LI><CODE>PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED</CODE></LI>
	<LI><CODE>PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_GPU_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_HOST_MEMORY_ASYNC_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_ALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_FREE</CODE></LI>
	<LI><CODE>PSGL_REPORT_MALLOC_MEMORY_REALLOC</CODE></LI>
	<LI><CODE>PSGL_REPORT_TEXTURE_CONVERSION</CODE></LI>
	<LI><CODE>PSGL_REPORT_RESC</CODE></LI>
	<LI><CODE>PSGL_REPORT_RESC_FLIP_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_COMMAND_RECORD_CALLBACK</CODE></LI>
	<LI><CODE>PSGL_REPORT_UNKNOWN</CODE></LI>
	</UL>
 *
 * @note Reports are generated only if the "debug" version of the library is used.
 *
 * @sa  psglDisableReport, psglSetReportFunction, psglGetReportFunction, psglSetDefaultReportParameters, psglSetReportClassMask
 */
PSGL_EXPORT void psglEnableReport( GLenum report )
{
#ifndef JS_DPM
    _psglEnableReport( report );
#endif
}

/**
 * @short Allow or inhibit a category (class) of related reports
 *
 * psglSetReportClassMask() sets a bitmask that is examined when an event occurs that may issue
 * a report.  If the corresponding bit is set to 1, then the report is processed.
 * By default, all classes are enabled except for memory report classes (<CODE>GL_REPORT_CLASS_*_MEMORY_INFO</CODE>).
 *
 *
 * Several enumerants that represent error classes must be bit-shifted to create
 * an error mask. For instance, it is correct to call
 * <CODE>psglSetReportClassMask(1<<PSGL_REPORT_CLASS_API_ERROR)</CODE>.
 * The following enumerants must be shifted
 * to create a mask:
        <UL>
        <LI><CODE>PSGL_REPORT_CLASS_DEBUG</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_API_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_GL_CRITICAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_INTERNAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_USER_CRITICAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_GPU_MEMORY_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_HOST_MEMORY_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO</CODE></LI>
	</UL>
 *
 * In addition, to individual classes, several values allow you to set the bitmask for several
 * classes at once and are useful without shifting, such as
 * <CODE>psglSetReportClassMask(PSGL_REPORT_ERROR_CLASSES)</CODE>.
 * The following table lists the enumerants and the classes they encapsulate:

<TABLE>
<TR>
<TD>Enumerant</TD>
<TD>Classes Masked</TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_MISC_CLASSES</CODE></TD>
<TD><CODE>PSGL_REPORT_CLASS_DEBUG</CODE>,
<CODE>PSGL_REPORT_CLASS_INFO</CODE>,
<CODE>PSGL_REPORT_CLASS_WARNING</CODE></TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_PERFORMANCE_CLASSES</CODE></TD>
<TD> <CODE>PSGL_REPORT_CLASS_PERFORMANCE_INFO</CODE>, <CODE>PSGL_REPORT_CLASS_PERFORMANCE_WARNING</CODE>,
<CODE>PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL</CODE></TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_ERROR_CLASSES</CODE></TD>
<TD><CODE>PSGL_REPORT_CLASS_API_ERROR</CODE>,
<CODE>PSGL_REPORT_CLASS_GL_CRITICAL_ERROR</CODE>,
<CODE>PSGL_REPORT_CLASS_INTERNAL_ERROR</CODE></TD>
</TR>
<TR>
<TD><CODE>PSGL_REPORT_ALL</CODE></TD>
<TD>All classes.</TD>
</TR>
</TABLE>
 *
 * The following table shows class names and associated report names that are members of those classes.
 * <TABLE rules=all>
 * <TR>
 * <TD>Class</TD>
 * <TD>Reports Belonging to the Class</TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_DEBUG</CODE></TD>
 * <TD><CODE>PSGL_REPORT_DEBUG</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_INFO</CODE></TD>
 * <TD><CODE>PSGL_REPORT_VERSION</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_WARNING</CODE></TD>
 * <TD><CODE>PSGL_REPORT_MISSING_STATE</CODE>,
 * <CODE>PSGL_REPORT_FP32_FILTERING</CODE>,
 * <CODE>PSGL_REPORT_TEXTURE_INCOMPLETE</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_PERFORMANCE_INFO</CODE></TD>
 * <TD><CODE>PSGL_REPORT_VERTEX_DATA_WARNING</CODE>,
 * <CODE>PSGL_REPORT_COPY_TEXTURE_WARNING</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL</CODE></TD>
 * <TD><CODE>PSGL_REPORT_VERTEX_SLOW_PATH</CODE>,
 * <CODE>PSGL_REPORT_COPY_TEXTURE_SLOW_PATH</CODE>,
 * <CODE>PSGL_REPORT_TEXTURE_COPY_BACK</CODE> </TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_API_ERROR</CODE></TD>
 * <TD><CODE>PSGL_REPORT_GL_ERROR</CODE>,
 * <CODE>PSGL_REPORT_CG_ERROR</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_USER_CRITICAL_ERROR</CODE></TD>
 * <TD><CODE>PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_GPU_MEMORY_INFO</CODE></TD>
 * <TD><CODE>PSGL_REPORT_GPU_MEMORY_ALLOC</CODE>,
 * <CODE>PSGL_REPORT_GPU_MEMORY_FREE</CODE>,
 * <CODE>PSGL_REPORT_GPU_MEMORY_ASYNC_FREE</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_HOST_MEMORY_INFO</CODE></TD>
 * <TD><CODE>PSGL_REPORT_HOST_MEMORY_ALLOC</CODE>,
 * <CODE>PSGL_REPORT_HOST_MEMORY_FREE</CODE>,
 * <CODE>PSGL_REPORT_HOST_MEMORY_ASYNC_FREE</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO</CODE></TD>
 * <TD><CODE>PSGL_REPORT_MALLOC_MEMORY_ALLOC</CODE>,
 * <CODE>PSGL_REPORT_MALLOC_MEMORY_FREE</CODE>,
 * <CODE>PSGL_REPORT_MALLOC_MEMORY_REALLOC</CODE></TD>
 * </TR>
 * <TR>
 * <TD><CODE>PSGL_REPORT_CLASS_UNKNOWN</CODE></TD>
 * <TD><CODE>PSGL_REPORT_UNKNOWN</CODE></TD>
 * </TR>
 * </TABLE>
 *
 * @param reportClassMask Specifies a bitmask that is bitwise <CODE>OR</CODE>&rsquo;ed with report types.
 * <I><c>reportClassMask</c></I> accepts bit-shifted error class enumerants
 * such as <CODE>psglSetReportClassMask(1<<PSGL_REPORT_CLASS_GL_CRITICAL_ERROR)</CODE>.
A full list of these class enumerants includes
        <UL>
        <LI><CODE>PSGL_REPORT_CLASS_DEBUG</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_WARNING</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_API_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_GL_CRITICAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_INTERNAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_USER_CRITICAL_ERROR</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_GPU_MEMORY_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_HOST_MEMORY_INFO</CODE></LI>
	<LI><CODE>PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO</CODE></LI>
	</UL>
The following class masks encapsulate several classes and should not be bit-shifted:
<UL>
<LI><CODE>PSGL_REPORT_MISC_CLASSES</CODE></LI>
<LI><CODE>PSGL_REPORT_PERFORMANCE_CLASSES</CODE></LI>
<LI><CODE>PSGL_REPORT_ERROR_CLASSES</CODE></LI>
<LI><CODE>PSGL_REPORT_ALL</CODE></LI>
</UL>
 *
 * @note Reports are generated only if the "debug" version of the library is used.
 *       All reports except memory reports (<CODE>GL_REPORT_*_MEMORY_*</CODE>) are enabled by default.
 *
 * @sa  psglEnableReport, psglSetReportFunction, psglGetReportFunction, psglSetDefaultReportParameters
 */
PSGL_EXPORT void psglSetReportClassMask( GLuint reportClassMask )
{
#ifndef JS_DPM
    _psglSetReportClassMask( reportClassMask );
#endif
}

/**
 * @short Returns the current class mask.
 *
 * psglGetReportClassMask() retrieves the current class mask that is used to filter which reports to process.
 *
 * For more information about PSGL reporting classes, see the corresponding function psglSetReportClassMask()
 *
 * @return Returns the current class mask
 *
 * @note Reports are generated only if the "debug" version of the library is used.
 *       All reports except memory reports (<CODE>GL_REPORT_*_MEMORY_*</CODE>) are enabled by default.
 *
 * @sa  psglSetReportClassMask
 */

PSGL_EXPORT GLuint psglGetReportClassMask()
{
    return _jsReportClassMask;
}

/**
 * @short Returns the callback function that is executed for a reportable event
 *
 * psglGetReportFunction() retrieves the callback function that is used to process
 * an error, warning, or other reportable event.
 *
 * @return Returns a pointer to the current callback function
 *
 * @sa  psglSetReportFunction
 */

PSGL_EXPORT PSGLreportFunction psglGetReportFunction()
{
    return _jsReportFunction;
}

/**
 * @short Specifies the callback function to be executed when a reportable event occurs
 *
 * psglSetReportFunction() establishes the function that is used to process
 * an error, warning, or other reportable event.
 * The function must follow this prototype:
 * <CODE>void * function(GLenum reportEnum, GLuint reportClassMask, const char *string)</CODE>
 * When the function is called, its arguments are: the enumerant that identifies the event,
 * the current class bitmask, and a printable string that corresponds to the type of event.
 * If no callback function is specified, a default function is provided.
 *
 * @param function Specifies a function to be called to process a reportable event.
 *
 * @sa  psglEnableReport, psglGetReportFunction, psglSetDefaultReportParameters
 */
PSGL_EXPORT void psglSetReportFunction( PSGLreportFunction function )
{
#ifndef JS_DPM
    _psglSetReportFunction( function );
#endif
}

/**
 * @short Specifies whether to display details in the default reporting function
 *
 *  psglSetDefaultReportParameters() determines whether the default callback function
 *  displays the event enumerant and class bitmask.
 *  If psglSetReportFunction() is used to replace the callback function,
 *  then psglSetDefaultReportParameters() has no effect.
 *
 * @param showEnums a Boolean value that indicates whether to display details in a report
 *
 * @sa  psglEnableReport, psglSetReportFunction
 */
PSGL_EXPORT void psglSetDefaultReportParameters( GLboolean showEnums )
{
#ifndef JS_DPM
    _psglSetDefaultReportParameters( showEnums );
#endif
}

/** @} PSGL */
