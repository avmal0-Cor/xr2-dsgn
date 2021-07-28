/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmValidateInlines.h>
#include <JSGCM/JSGcmFBOps.h>
#include <JSGCM/JSGcmVertexAttribPointer.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmFPShaderMan.h>


void _jsFBClear( GLbitfield mask )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    jsGcmDriver *driver = _jsGetGcmDriver();
    if ( !driver->rtValid ) return;

    GLbitfield newmask = 0;
    if (( mask & GL_COLOR_BUFFER_BIT ) && driver->rt.colorBufferCount ) newmask |= JSGCM_COLOR_BUFFER_BIT;
    if (( mask & GL_DEPTH_BUFFER_BIT ) && ( driver->rt.depthFormat != JSGCM_NONE ) )
        newmask |= JSGCM_DEPTH_BUFFER_BIT;
    if (( mask & GL_STENCIL_BUFFER_BIT ) && ( driver->rt.depthFormat == JSGCM_DEPTH_COMPONENT24 ) )
        newmask |= JSGCM_STENCIL_BUFFER_BIT;

    if ( !newmask ) return;

    GLbitfield clearMask = newmask;
    if ( driver->rt.colorFormat != JSGCM_ARGB8 ) clearMask &= ~JSGCM_COLOR_BUFFER_BIT;

    // always use quad clear for colors with MRT
    //  There is one global clear mask for all render targets.  This doesn't
    //  work nicely for all render target combinations, e.g. only the first
    //  and last targets enabled.  Quad clear works because color mask is
    //  per target.
    //
    //  TODO: Clear could be used if the enabled render targets are
    //  contiguous from 0, i.e. {0,1}, {0,1,2}, {0,1,2,3}.  If this is done,
    //  parallel changes need to made in _jsValidateWriteMask because we
    //  bypass calling _jsGcmFifoGlColorMask there and the mask used by nv_glClear
    //  is not updated.
    if ( driver->rt.colorBufferCount > 1 )
        clearMask &= ~JSGCM_COLOR_BUFFER_BIT;

    if ( clearMask )
    {
        _jsGcmFifoGlClearColor( LContext->ClearColor.R, LContext->ClearColor.G, LContext->ClearColor.B, LContext->ClearColor.A );
        _jsGcmFifoGlClearDepthf(( GLclampf )( LContext->ClearDepth ) );
        _jsGcmFifoGlClearStencil( LContext->ClearStencil );

        _jsGcmFifoGlClear( clearMask );
        newmask &= ~clearMask;
    }

    if ( newmask )
    {
        // draw a quad to erase everything.

        // disable/set up a lot of states
        //
        // leave scissors and color write mask alone
        if ((( newmask & JSGCM_DEPTH_BUFFER_BIT ) != 0 ) && (( LContext->WriteMask&_JS_CONTEXT_DEPTH_MASK ) != 0 ) )
        {
            _jsGcmFifoGlEnable( JSGCM_DEPTH_TEST );
            _jsGcmFifoGlDepthFunc( JSGCM_ALWAYS );
            _jsGcmFifoGlDepthMask( GL_TRUE );
        }
        else
        {
            _jsGcmFifoGlDisable( JSGCM_DEPTH_TEST );
            _jsGcmFifoGlDepthMask( GL_FALSE );
        }
        JS_ASSERT( newmask&JSGCM_COLOR_BUFFER_BIT );

        if ( newmask & JSGCM_STENCIL_BUFFER_BIT )
        {
            _jsGcmFifoGlStencilFunc( JSGCM_ALWAYS, LContext->ClearStencil, -1U );
            _jsGcmFifoGlStencilOp( JSGCM_REPLACE, JSGCM_REPLACE, JSGCM_REPLACE );
            _jsGcmFifoGlStencilMask( LContext->StencilWriteMask );
            LContext->needValidate |= PSGL_VALIDATE_STENCIL_OP_AND_MASK;
            _jsGcmFifoGlEnable( JSGCM_STENCIL_TEST );
        }
        else
        {
            _jsGcmFifoGlDisable( JSGCM_STENCIL_TEST );
        }

        static float _jsClearVertexBuffer[4*3] __attribute__(( aligned( JS_ALIGN_FAST_TRANSFER ) ) ) =
            {
                -1.f, -1.f, 0.f,
                -1.f, 1.f, 0.f,
                1.f, -1.f, 0.f,
                1.f, 1.f, 0.f,
            };
        for ( int i = 0;i < 4;++i ) _jsClearVertexBuffer[2+i*3] = LContext->ClearDepth * 2.f - 1.f;

        _jsAllocSetHint( _JS_ALLOCTYPE_BLIT_QUAD_VERTEX_BUFFER, 0 );
        GLuint bufferId = _jsGcmAllocateMemory( sizeof( _jsClearVertexBuffer ) );
        _jsAllocClearHint();
        JS_ASSERT( bufferId != GMM_ERROR );
        fast_memcpy( gmmIdToAddress(bufferId), _jsClearVertexBuffer, sizeof( _jsClearVertexBuffer ) );
        _jsGcmFifoGlVertexAttribPointer( 0, 3, JSGCM_FLOAT, JSGCM_FALSE, 3*sizeof( GLfloat ), 1, 0, gmmIdToOffset(bufferId) );
        JSBIT_TRUE( LContext->attribs->DirtyMask, 0 );

        for ( int i = 1;i < _JS_MAX_VERTEX_ATTRIBS;++i )
        {
            _jsGcmFifoGlVertexAttribPointer( i, 0, JSGCM_FLOAT, 0, 0, 0, 0, 0 );
            JSBIT_TRUE( LContext->attribs->DirtyMask, i );
        }
        _jsGcmFifoGlVertexAttrib4fv( _JS_ATTRIB_PRIMARY_COLOR_INDEX, ( GLfloat* )&LContext->ClearColor );

        // use compiled in color-only shader
        //  The embedded shader (built from fpColorOnly.cg) outputs to all
        //  render targets.  We bypass the fragment shader manager because
        //  this shader could be overridden by a loaded shader.
        //
        // XXX Do we need separate shaders for non-MRT and MRT clears to
        // improve performance?
        jsGcmShader *nvShader = _jsGcmFPGetDummyShader();
        JS_ASSERT( nvShader->loadAddressId != GMM_ERROR );
        _jsGcmSetNativeCgFragmentProgram(&(nvShader->program), nvShader->loadAddressId);
        driver->fpLoadProgramId = nvShader->loadAddressId;
        driver->fpLoadProgramOffset = 0;

        _jsDrawUtilQuad( GL_TRUE, GL_FALSE, 0, 0, driver->rt.width*driver->xSuperSampling, driver->rt.height*driver->ySuperSampling );
        LContext->needValidate |= PSGL_VALIDATE_DEPTH_TEST
                                  | PSGL_VALIDATE_STENCIL_TEST
                                  | PSGL_VALIDATE_WRITE_MASK
                                  | PSGL_VALIDATE_FRAGMENT_PROGRAM | PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;

        _jsGcmFreeMemory( bufferId );
    }

    _jsGcmFifoGlFlush();
}


void _jsValidateFog()
{
    PSGLcontext*	LContext = _CurrentContext;
    jsGcmDriver *driver = _jsGetGcmDriver();
    if ( LContext->Fog )
        _jsGcmFPShaderManagerSetFog( &driver->shaderManager, LContext->FogMode, ( float * )&LContext->FogColor,
                                     LContext->FogStart, LContext->FogEnd, LContext->FogDensity );
    else
        _jsGcmFPShaderManagerSetFog( &driver->shaderManager, 0, ( float * )&LContext->FogColor, 0, 0, 0 );

}

