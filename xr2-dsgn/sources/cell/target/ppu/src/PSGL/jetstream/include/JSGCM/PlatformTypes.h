/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _PLATFORM_TYPES_h
#define _PLATFORM_TYPES_h

#include <JSGCM/JSGcmDriver.h>
#include <PSGL/Types.h>

struct jsPlatformFramebuffer: public jsFramebuffer
{
    jsGcmRenderTargetEx rt;
    GLuint colorBufferMask;
    GLboolean complete;
    jsPlatformFramebuffer(): jsFramebuffer()
    {
        memset( &rt, 0, sizeof( rt ) );
    };
    virtual ~jsPlatformFramebuffer() {};
    void validate( PSGLcontext *LContext );
};


#endif
