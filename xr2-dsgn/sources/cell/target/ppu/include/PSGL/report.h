/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _PSGL_report_h
#define _PSGL_report_h

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum {
        PSGL_REPORT_UNKNOWN,
        PSGL_REPORT_VERSION,
        PSGL_REPORT_DEBUG,
        PSGL_REPORT_ASSERT,
        PSGL_REPORT_GL_ERROR,
        PSGL_REPORT_MISSING_STATE,
        PSGL_REPORT_VERTEX_SLOW_PATH,
        PSGL_REPORT_VERTEX_DATA_WARNING,
        PSGL_REPORT_COPY_TEXTURE_SLOW_PATH,
        PSGL_REPORT_COPY_TEXTURE_WARNING,
        PSGL_REPORT_TEXTURE_COPY_BACK,
        PSGL_REPORT_TEXTURE_REALLOC,
        PSGL_REPORT_FP32_FILTERING,
        PSGL_REPORT_TEXTURE_INCOMPLETE,
        PSGL_REPORT_FRAMEBUFFER_INCOMPLETE,
        PSGL_REPORT_FRAMEBUFFER_UNSUPPORTED,
        PSGL_REPORT_CG_ERROR,
        PSGL_REPORT_CG_PROFILE_DEPRECATED,
        PSGL_REPORT_BUFFER_OBJECT_OUT_OF_BOUNDS,
        PSGL_REPORT_GPU_MEMORY_ALLOC,
        PSGL_REPORT_GPU_MEMORY_FREE,
        PSGL_REPORT_GPU_MEMORY_ASYNC_FREE,
        PSGL_REPORT_HOST_MEMORY_ALLOC,
        PSGL_REPORT_HOST_MEMORY_FREE,
        PSGL_REPORT_HOST_MEMORY_ASYNC_FREE,
        PSGL_REPORT_MALLOC_MEMORY_ALLOC,
        PSGL_REPORT_MALLOC_MEMORY_REALLOC,
        PSGL_REPORT_MALLOC_MEMORY_FREE,
        PSGL_REPORT_TEXTURE_CONVERSION,
        PSGL_REPORT_RESC,
        PSGL_REPORT_RESC_FLIP_ERROR,
		PSGL_REPORT_COMMAND_RECORD_CALLBACK,

        PSGL_REPORT_MAX
    } PSGL_REPORT_ENUMS;

    typedef enum {
        PSGL_REPORT_CLASS_UNKNOWN = 0,
        PSGL_REPORT_CLASS_DEBUG,
        PSGL_REPORT_CLASS_INFO,
        PSGL_REPORT_CLASS_WARNING,

        PSGL_REPORT_CLASS_PERFORMANCE_INFO = 8,
        PSGL_REPORT_CLASS_PERFORMANCE_WARNING,
        PSGL_REPORT_CLASS_PERFORMANCE_CRITICAL,

        PSGL_REPORT_CLASS_API_ERROR = 16,
        PSGL_REPORT_CLASS_GL_CRITICAL_ERROR,
        PSGL_REPORT_CLASS_INTERNAL_ERROR,
        PSGL_REPORT_CLASS_USER_CRITICAL_ERROR,

        PSGL_REPORT_CLASS_GPU_MEMORY_INFO = 24,
        PSGL_REPORT_CLASS_HOST_MEMORY_INFO,
        PSGL_REPORT_CLASS_MALLOC_MEMORY_INFO,
    } PSGL_REPORT_CLASSES;

#define PSGL_REPORT_ALL					0xffffffff
#define PSGL_REPORT_MISC_CLASSES			0x000000ff
#define PSGL_REPORT_PERFORMANCE_CLASSES	0x0000ff00
#define PSGL_REPORT_ERROR_CLASSES			0x00ff0000
#define PSGL_REPORT_MEMORY_INFO_CLASSES			0xff000000

    typedef void( *PSGLreportFunction )( GLenum, GLuint , const char * );
    PSGL_EXPORT void psglDisableReport( GLenum report );
    PSGL_EXPORT void psglEnableReport( GLenum report );
    PSGL_EXPORT GLboolean psglIsReportEnabled( GLenum report );
    PSGL_EXPORT void psglSetReportClassMask( GLuint reportClassMask );
    PSGL_EXPORT GLuint psglGetReportClassMask();
    PSGL_EXPORT PSGLreportFunction psglGetReportFunction();
    PSGL_EXPORT void psglSetReportFunction( PSGLreportFunction function );
    PSGL_EXPORT void psglSetDefaultReportParameters( GLboolean showEnums );

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif
