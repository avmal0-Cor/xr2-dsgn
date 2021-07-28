/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Contains the functions to disable various capabilities
 */


#ifndef _H_JSGCM_DISABLE_H_
#define _H_JSGCM_DISABLE_H_

/////////////////////////////////////////////////////////////////////////////
// Platform device specific part of GlDisable
//
// Directly set the capability into the command buffer
// Mostly maps one to between the GL state and GCM states
void static inline _jsGcmFifoGlDisable( jsGcmEnum cap )
{

    switch ( cap )
    {
        case JSGCM_ALPHA_TEST:
            GCM_FUNC( cellGcmSetAlphaTestEnable, JSGCM_FALSE );
            break;
        case JSGCM_BLEND:
            GCM_FUNC( cellGcmSetBlendEnable, JSGCM_FALSE );
            GCM_FUNC( cellGcmSetBlendEnableMrt, JSGCM_FALSE, JSGCM_FALSE, JSGCM_FALSE );
            break;
        case JSGCM_CULL_FACE:
            GCM_FUNC( cellGcmSetCullFaceEnable, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_BOUNDS_TEST:
            GCM_FUNC( cellGcmSetDepthBoundsTestEnable, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_TEST:
        {
            jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
            rt->depthTest = JSGCM_FALSE;
            GCM_FUNC( cellGcmSetDepthTestEnable, JSGCM_FALSE );
        }
        break;
        case JSGCM_DITHER:
            GCM_FUNC( cellGcmSetDitherEnable, JSGCM_FALSE );
            break;
        case JSGCM_COLOR_LOGIC_OP:
            GCM_FUNC( cellGcmSetLogicOpEnable, JSGCM_FALSE );
            break;
        case JSGCM_SCISSOR_TEST:
        {
            // we have no scissor_test enable, set to max values and
            // restore original user values
            jsGcmScissorState *s = &_jsGcmState.state.scissor;
            s->enabled = JSGCM_FALSE;
            _jsGcmFifoGlScissor( s->x, s->y, s->w, s->h );
        }
        break;
        case JSGCM_STENCIL_TEST:
            GCM_FUNC( cellGcmSetStencilTestEnable, JSGCM_FALSE );
            break;
        case JSGCM_STENCIL_TEST_TWO_SIDE:
            GCM_FUNC( cellGcmSetTwoSidedStencilTestEnable, JSGCM_FALSE );
            break;
        case JSGCM_LIGHT_MODEL_TWO_SIDE:
        {
            jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
            s->sideMask =
                ~( CELL_GCM_ATTRIB_OUTPUT_MASK_BACKDIFFUSE ) & 	// mask out back diffuse enable
                ~( CELL_GCM_ATTRIB_OUTPUT_MASK_BACKSPECULAR ); 	// mask out back specular enable

            GCM_FUNC( cellGcmSetTwoSideLightEnable, JSGCM_FALSE );

            // vertex output attributes
            GCM_FUNC( cellGcmSetVertexAttribOutputMask,
                      ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                      s->fragmentProgramAttribMask &
                      s->clipMask &
                      s->sideMask );

        }
        break;
        case JSGCM_POLYGON_OFFSET_FILL:
            GCM_FUNC( cellGcmSetPolygonOffsetFillEnable, JSGCM_FALSE );
            break;
        case JSGCM_POLYGON_OFFSET_LINE:
            GCM_FUNC( cellGcmSetPolygonOffsetLineEnable, JSGCM_FALSE );
            break;
        case JSGCM_PRIMITIVE_RESTART:
            GCM_FUNC( cellGcmSetRestartIndexEnable, JSGCM_FALSE );
            break;
        case JSGCM_POINT_SPRITE:
            // setting false for disable, setting 0x01 for FROM_R, and tex unit 0
            GCM_FUNC( cellGcmSetPointSpriteControl, JSGCM_FALSE, 1, 0 );
            break;
        case JSGCM_PSHADER_SRGB_REMAPPING:
            GCM_FUNC( cellGcmSetFragmentProgramGammaEnable, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_CLAMP:
            // set near to false, setting z clamp to true, setting ignoreW to false
            GCM_FUNC( cellGcmSetZMinMaxControl, JSGCM_TRUE, JSGCM_FALSE, JSGCM_FALSE );
			break;
        default:
            JS_ASSERT( 0 ); // not supported by PSGL - GCM
            break;
    }
}

// Same disables as above, but add the command to the given buffer instead of
// the current one.
static inline GLvoid* _jsGcmFifoGlDisableBuffered( GLvoid* cmdBuffer, jsGcmEnum cap )
{

    switch ( cap )
    {
        case JSGCM_ALPHA_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetAlphaTestEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_BLEND:
            GCM_FUNC_BUFFERED( cellGcmSetBlendEnable, cmdBuffer, JSGCM_FALSE );
            GCM_FUNC_BUFFERED( cellGcmSetBlendEnableMrt, cmdBuffer, JSGCM_FALSE, JSGCM_FALSE, JSGCM_FALSE );
            break;
        case JSGCM_CULL_FACE:
            GCM_FUNC_BUFFERED( cellGcmSetCullFaceEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_BOUNDS_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetDepthBoundsTestEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_TEST:
        {
            jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
            rt->depthTest = JSGCM_FALSE;
            GCM_FUNC_BUFFERED( cellGcmSetDepthTestEnable, cmdBuffer, JSGCM_FALSE );
        }
        break;
        case JSGCM_DITHER:
            GCM_FUNC_BUFFERED( cellGcmSetDitherEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_COLOR_LOGIC_OP:
            GCM_FUNC_BUFFERED( cellGcmSetLogicOpEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_SCISSOR_TEST:
        {
            // we have no scissor_test enable, set to max values and
            // restore original user values
            jsGcmScissorState *s = &_jsGcmState.state.scissor;
            s->enabled = JSGCM_FALSE;
            _jsGcmFifoGlScissor( s->x, s->y, s->w, s->h );
        }
        break;
        case JSGCM_STENCIL_TEST:
            GCM_FUNC_BUFFERED( cellGcmSetStencilTestEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_STENCIL_TEST_TWO_SIDE:
            GCM_FUNC_BUFFERED( cellGcmSetTwoSidedStencilTestEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_LIGHT_MODEL_TWO_SIDE:
        {

            jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
            s->sideMask =
                ~( CELL_GCM_ATTRIB_OUTPUT_MASK_BACKDIFFUSE ) & 	// mask out back diffuse enable
                ~( CELL_GCM_ATTRIB_OUTPUT_MASK_BACKSPECULAR ); 	// mask out back specular enable

            GCM_FUNC_BUFFERED( cellGcmSetTwoSideLightEnable, cmdBuffer, JSGCM_FALSE );
            GCM_FUNC_BUFFERED( cellGcmSetVertexAttribOutputMask, cmdBuffer,
                               ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
                               s->fragmentProgramAttribMask &
                               s->clipMask &
                               s->sideMask );
            return cmdBuffer;
        }
        break;
        case JSGCM_POLYGON_OFFSET_FILL:
            GCM_FUNC_BUFFERED( cellGcmSetPolygonOffsetFillEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_POLYGON_OFFSET_LINE:
            GCM_FUNC_BUFFERED( cellGcmSetPolygonOffsetLineEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_PRIMITIVE_RESTART:
            GCM_FUNC_BUFFERED( cellGcmSetRestartIndexEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_POINT_SPRITE:
            // setting true for enable, setting 0x01 for FROM_R, and tex unit 0
            GCM_FUNC_BUFFERED( cellGcmSetPointSpriteControl, cmdBuffer, JSGCM_FALSE, 1, 0 );
            break;
        case JSGCM_PSHADER_SRGB_REMAPPING:
            GCM_FUNC_BUFFERED( cellGcmSetFragmentProgramGammaEnable, cmdBuffer, JSGCM_FALSE );
            break;
        case JSGCM_DEPTH_CLAMP:
            GCM_FUNC_BUFFERED( cellGcmSetZMinMaxControl, cmdBuffer, JSGCM_TRUE, JSGCM_FALSE, JSGCM_FALSE );
            break;
        default:
            JS_ASSERT( 0 ); // not supported by PSGL - GCM
            break;
    }
    return cmdBuffer;
}

#endif
