/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmConsistency.cpp
 */

#include <JSGCM/JSGcmUtils.h>

// note: it is assumed the compiler/linker removes dead code

/* update the consistency state machine
**
*/
GLboolean _jsGcmSetPixelShaderConsistency( GLuint isPShaderPrecisonR )
{
    jsGcmConsistencyTracker *consistency = &_jsGcmState.consistency;
    consistency->isPShaderPrecisonR = isPShaderPrecisonR;
    return GL_TRUE;
}

/* asserts we have a valid state
**
*/
GLboolean _jsGcmCheckConsistency()
{
#ifdef JSGCM_CHECK_CONSISTENCY
    jsGcmConsistencyTracker *consistency = &_jsGcmState.consistency;
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;

    // verify that the pixel shader output register precision matches the fb format
    // 1) if they don't match we inccur +1 penalty
    // 2) SRGB/LRPD only work on fp16 values and hence would be silently disabled
    switch ( rt->colorFormat )
    {
        case JSGCM_NONE:
        case JSGCM_ARGB8:
        case JSGCM_FLOAT_RGBA16:
            JS_ASSERT( !consistency->isPShaderPrecisonR );
            break;
        case JSGCM_FLOAT_R32:
        case JSGCM_FLOAT_RGBA32:
            JS_ASSERT( consistency->isPShaderPrecisonR );
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // gcc: avoid release warning
    consistency = consistency;
#endif
    return GL_TRUE;
}

