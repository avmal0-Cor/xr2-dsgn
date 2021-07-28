/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */



/*
 * File description here
 */



#ifndef _JSGCM_ENABLE_H_
#define _JSGCM_ENABLE_H_

using namespace cell::Gcm;

#include "stdio.h"

void static inline _jsGcmFifoGlScissor( GLint x, GLint y, GLsizei width, GLsizei height );

/////////////////////////////////////////////////////////////////////////////
// Platform device specific part of GlEnable
//
// Mostly maps one to one between GL and GCM.
void static inline _jsGcmFifoGlEnable( jsGcmEnum cap )
{
    switch ( cap )
    {
        case JSGCM_ALPHA_TEST:
            GCM_FUNC( cellGcmSetAlphaTestEnable, JSGCM_TRUE );
            break;
        case JSGCM_BLEND:
            GCM_FUNC( cellGcmSetBlendEnable,  JSGCM_TRUE );
            GCM_FUNC( cellGcmSetBlendEnableMrt, JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE );
            break;
        case JSGCM_COLOR_LOGIC_OP:
            GCM_FUNC( cellGcmSetLogicOpEnable, JSGCM_TRUE );
            break;
        case JSGCM_CULL_FACE:
            GCM_FUNC( cellGcmSetCullFaceEnable, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_BOUNDS_TEST:
            GCM_FUNC( cellGcmSetDepthBoundsTestEnable, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_TEST:
        {
            jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
            rt->depthTest = JSGCM_TRUE;
            if ( rt->depthFormat != JSGCM_NONE )
            {
                GCM_FUNC( cellGcmSetDepthTestEnable, JSGCM_TRUE );
            }
            else
            {
                GCM_FUNC( cellGcmSetDepthTestEnable, JSGCM_FALSE );
            }
        }
        break;
        case JSGCM_DITHER:
            GCM_FUNC( cellGcmSetDitherEnable, JSGCM_TRUE );
            break;
        case JSGCM_SCISSOR_TEST:
        {
            // scissor_disable is implemented by (0, 0, max, max)
            jsGcmScissorState *s = &_jsGcmState.state.scissor;
            s->enabled = JSGCM_TRUE;
            _jsGcmFifoGlScissor( s->x, s->y, s->w, s->h );
        }
        break;
        case JSGCM_STENCIL_TEST:
            GCM_FUNC( cellGcmSetStencilTestEnable, JSGCM_TRUE );
            break;
        case JSGCM_STENCIL_TEST_TWO_SIDE:
            GCM_FUNC( cellGcmSetTwoSidedStencilTestEnable, JSGCM_TRUE );
            break;
        case JSGCM_LIGHT_MODEL_TWO_SIDE:
        {
            jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
            s->sideMask = ~0;
            GCM_FUNC( cellGcmSetTwoSideLightEnable, JSGCM_TRUE );
            GCM_FUNC( cellGcmSetVertexAttribOutputMask,
                      ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                      s->fragmentProgramAttribMask &
                      s->clipMask &
                      s->sideMask );
        }
        break;
        case JSGCM_POLYGON_OFFSET_FILL:
            GCM_FUNC( cellGcmSetPolygonOffsetFillEnable, JSGCM_TRUE );
            break;
        case JSGCM_POLYGON_OFFSET_LINE:
            GCM_FUNC( cellGcmSetPolygonOffsetLineEnable, JSGCM_TRUE );
            break;
        case JSGCM_PRIMITIVE_RESTART:
            GCM_FUNC( cellGcmSetRestartIndexEnable, JSGCM_TRUE );
            break;
        case JSGCM_POINT_SPRITE:
            // setting true for enable, setting 0x01 for FROM_R, and tex unit 0
            GCM_FUNC( cellGcmSetPointSpriteControl, JSGCM_TRUE, 1, 0 );
            break;
        case JSGCM_PSHADER_SRGB_REMAPPING:
            GCM_FUNC( cellGcmSetFragmentProgramGammaEnable, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_CLAMP:
            // set near to false, setting z clamp to true, setting ignoreW to false
            GCM_FUNC( cellGcmSetZMinMaxControl, JSGCM_FALSE, JSGCM_TRUE, JSGCM_FALSE );
            break;
        default:
            JS_ASSERT( 0 ); // not supported by GCM
            break;
    }
}


void printInfo( GLvoid* cmdBuffer );

static inline GLvoid* _jsGcmFifoGlEnableBuffered( GLvoid* cmdBuffer, jsGcmEnum cap )
{
    switch ( cap )
    {
        case JSGCM_ALPHA_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetAlphaTestEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_BLEND:
            GCM_FUNC_BUFFERED( cellGcmSetBlendEnable, cmdBuffer, JSGCM_TRUE );
            GCM_FUNC_BUFFERED( cellGcmSetBlendEnableMrt, cmdBuffer, JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE );
            break;
        case JSGCM_COLOR_LOGIC_OP:
            GCM_FUNC_BUFFERED( cellGcmSetLogicOpEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_CULL_FACE:
            GCM_FUNC_BUFFERED( cellGcmSetCullFaceEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_BOUNDS_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetDepthBoundsTestEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_TEST:
        {
            jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
            rt->depthTest = JSGCM_TRUE;
            if ( rt->depthFormat != JSGCM_NONE )
            {
                GCM_FUNC_BUFFERED( cellGcmSetDepthTestEnable, cmdBuffer, JSGCM_TRUE );
            }
            else
            {
                GCM_FUNC_BUFFERED( cellGcmSetDepthTestEnable, cmdBuffer, JSGCM_TRUE );
            }
        }
        break;
        case JSGCM_DITHER:
            GCM_FUNC_BUFFERED( cellGcmSetDitherEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_SCISSOR_TEST:
        {
            // scissor_disable is implemented by (0, 0, max, max)
            jsGcmScissorState *s = &_jsGcmState.state.scissor;
            s->enabled = JSGCM_TRUE;
            _jsGcmFifoGlScissor( s->x, s->y, s->w, s->h );
        }
        break;
        case JSGCM_STENCIL_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetStencilTestEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_STENCIL_TEST_TWO_SIDE:
            GCM_FUNC_BUFFERED( cellGcmSetTwoSidedStencilTestEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_LIGHT_MODEL_TWO_SIDE:
        {
            jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
            s->sideMask = ~0;
            GCM_FUNC_BUFFERED( cellGcmSetTwoSideLightEnable, cmdBuffer, JSGCM_TRUE );
            GCM_FUNC_BUFFERED( cellGcmSetVertexAttribOutputMask, cmdBuffer,
                               ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                               s->fragmentProgramAttribMask &
                               s->clipMask &
                               s->sideMask );
        }
        break;
        case JSGCM_POLYGON_OFFSET_FILL:
            GCM_FUNC_BUFFERED( cellGcmSetPolygonOffsetFillEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_POLYGON_OFFSET_LINE:
            GCM_FUNC_BUFFERED( cellGcmSetPolygonOffsetLineEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_PRIMITIVE_RESTART:
            GCM_FUNC_BUFFERED( cellGcmSetRestartIndexEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_POINT_SPRITE:
            // setting true for enable, setting 0x01 for FROM_R, and tex unit 0
            GCM_FUNC_BUFFERED( cellGcmSetPointSpriteControl, cmdBuffer, JSGCM_TRUE, 1, 0 );
            break;
        case JSGCM_PSHADER_SRGB_REMAPPING:
            GCM_FUNC_BUFFERED( cellGcmSetFragmentProgramGammaEnable, cmdBuffer, JSGCM_TRUE );
            break;
        case JSGCM_DEPTH_CLAMP:
            GCM_FUNC_BUFFERED( cellGcmSetZMinMaxControl, cmdBuffer, JSGCM_FALSE, JSGCM_TRUE, JSGCM_FALSE );
            break;
        default:
            JS_ASSERT( 0 ); // not supported by GCM
            break;
    }
    return cmdBuffer;
}

#endif
