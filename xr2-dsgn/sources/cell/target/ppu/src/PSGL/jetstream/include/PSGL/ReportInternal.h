/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _JSReportInternal_h
#define _JSReportInternal_h

#include <PSGL/psgl.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        GLuint reportClass;
        const char *enumName;
        const char *defaultMessage;
        GLboolean disable;
    }
    JSreportInfo;

    typedef struct
    {
        GLenum e;
        const char *s;
    }
    JSenumMap;

    const char *_jsMapLookupEnum( const JSenumMap* map, unsigned int count, GLenum e );
    GLenum _jsMapLookupString( const JSenumMap* map, unsigned int count, const char *s );
#define _JS_MAP_LOOKUP_ENUM(MAP,ENUM) _jsMapLookupEnum(MAP,sizeof(MAP)/sizeof(MAP[0]),ENUM)
#define _JS_MAP_LOOKUP_STRING(MAP,STRING) _jsMapLookupString(MAP,sizeof(MAP)/sizeof(MAP[0]),STRING)

    void _jsReportInit();
    void _jsReportExit();
    void _jsReportEnableAll();
    void _jsReport( GLenum reportEnum );
    void _jsReportExtra( GLenum reportEnum, const char *format, ... );

    void _jsDefaultReport( GLenum reportEnum, GLuint reportClass, const char *string );

    void _jsPlatformReport( const char *s );

    void _psglDisableReport( GLenum report );
    void _psglEnableReport( GLenum report );
    void _psglSetReportClassMask( GLuint reportClassMask );
    void _psglSetReportFunction( PSGLreportFunction function );
    void _psglSetDefaultReportParameters( GLboolean showEnums );

    const char *_jsGetGLEnumName( GLenum e );
    const char *_jsGetGLErrorName( GLenum e );

#ifdef JS_REPORT

#define _JS_REPORT(ENUM) _jsReport(ENUM)

#ifndef MSVC
#define _JS_REPORT_EXTRA(ENUM, ...) _jsReportExtra(ENUM, __VA_ARGS__)
#endif

#else
#define _JS_REPORT(ENUM) ((void)0)

#ifndef MSVC
#define _JS_REPORT_EXTRA(ENUM, ...) ((void)0)
#endif

#endif

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration that encloses file.
#endif

#endif
