/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * FIFO management utility function definitions.
 */

#ifndef _H_JSGCM_FIFO_UTILS_H_
#define _H_JSGCM_FIFO_UTILS_H_

#include <JSGCM/JSGcmVertexAttribPointer.h>

// Prints out an int in hexedecimal and binary, broken into bytes.
// Can be used for printing out macro and constant values.
// example: _jsPrintIt( JSGCM_3DCONST(SET_SURFACE_FORMAT, COLOR, LE_A8R8G8B8) );
//          00 00 00 08 : 00000000 00000000 00000000 00001000 */
void static inline _jsPrintIt( unsigned int v )
{
    // HEX (space between bytes)
    printf( "%02x %02x %02x %02x : ", ( v >> 24 )&0xff, ( v >> 16 )&0xff, ( v >> 8 )&0xff, v&0xff );

    // BINARY (space between bytes)
    for ( unsigned int mask = ( 0x1 << 31 ), i = 1; mask != 0; mask >>= 1, i++ )
        printf( "%d%s", ( v & mask ) ? 1 : 0, ( i % 8 == 0 ) ? " " : "" );
    printf( "\n" );
}

// prints the last numWords of the command fifo
void static inline _jsPrintFifoFromPut( unsigned int numWords ) 
{
    for ( int i = -numWords; i <= -1; i++ )
        _jsPrintIt((( uint32_t* )_jsGcmState.fifo.current )[i] );
}

// prints the last numWords of the command fifo
void static inline _jsPrintFifoFromGet( unsigned int numWords ) 
{
    for ( int i = -numWords; i <= -1; i++ )
        _jsPrintIt((( uint32_t* )_jsGcmState.fifo.lastGetRead )[i] );
}

// Determine whether a given location in a command buffer has been passed, by 
// using reference markers.
GLboolean static inline _jsGcmFifoGlTestFenceRef( const GLuint ref )
{
    jsGcmFifo *fifo = &_jsGcmState.fifo;
    return _jsGcmFifoReferenceInUse( fifo, ref );
}

// Add a reference marker to the command buffer to determine whether a location 
// in the command buffer has been passed
void static inline _jsGcmFifoGlIncFenceRef( GLuint *ref )
{
    jsGcmFifo *fifo = &_jsGcmState.fifo;
    *ref = _jsGcmFifoPutReference( fifo );
}

// Flush the current FIFO.
void static inline _jsGcmFifoGlFlush( void )
{
    _jsGcmFifoGlInvalidateVertexCache();
    _jsGcmFifoFlush( &_jsGcmState.fifo );
}

// Begin a new occlusion query.  Must have finished any previous 
// queries already (no nested queries).
// Only JSGCM_SAMPLES_PASSED target is supported.
void static inline _jsGcmFifoGlBeginQuery( jsGcmEnum target, GLuint queryId, GLboolean resetCompletionFlag )
{
    jsGcmQueryState *queries = &_jsGcmState.state.queries;

	if ( _jsGcmState.reportsInMainMemory )
	{
		JS_ASSERT( queryId < JSGCM_MM_MAX_ZPASS_REPORTS );

		// NOTE setting main memory report zero to non zero value so we can check it later 
		// for completion 
		if (_jsGcmState.reportAddressMain)
		{
			_jsGcmState.reportAddressMain[queryId].zero		= 0xFFFFFFFF;
			_jsGcmState.reportAddressMain[queryId].value	= 0xFFFFFFFF; 
		}
	}
	else
	{
	    JS_ASSERT( queryId < JSGCM_LM_MAX_ZPASS_REPORTS );

		// [RSTENSON] need to clear the current query completion value 
		if (_jsGcmState.reportCompletionValues )
			_jsGcmState.reportCompletionValues[queryId] = 0; 
		_jsGcmState.reportCompletionValue = JSGCM_REPORT_COMPLETE_VALUE; 
	}

    // pick proper query type...
    switch ( target )
    {
        case JSGCM_SAMPLES_PASSED:

            GCM_FUNC( cellGcmSetZpassPixelCountEnable, CELL_GCM_TRUE ); 
            GCM_FUNC( cellGcmSetClearReport, CELL_GCM_ZPASS_PIXEL_CNT );

            queries->currentZPassQueryId = queryId;
			
		   	_jsGcmPrintf(" Begin Query id %d \n", queries->currentZPassQueryId ); 
			
            break;
        default:
            JS_ASSERT( 0 );
            break;

    }
}

// End the current occlusion query results..
// Only JSGCM_SAMPLES_PASSED target is supported.
void static inline _jsGcmFifoGlEndQuery( jsGcmEnum target )
{
    jsGcmQueryState *queries = &_jsGcmState.state.queries;
    GLuint hwOffset;

	_jsGcmState.reportCompletionValue = JSGCM_REPORT_COMPLETE_VALUE; 
	
    switch ( target )
    {
	case JSGCM_SAMPLES_PASSED:
		{
		
            // pick target offset            
            hwOffset = sizeof( jsGcmGetReport ) * queries->currentZPassQueryId;
            GCM_FUNC( cellGcmSetReport, CELL_GCM_ZPASS_PIXEL_CNT, queries->currentZPassQueryId );
            GCM_FUNC( cellGcmSetZpassPixelCountEnable, CELL_GCM_FALSE );
			

			// Now that we have the ability to maintain reports in main memory we don't need to 
			// do this inline transfer below to ensure that the RSX had consumed the above SetReport and 
			// ZPass disable. [RSTENSON]
			if ( !_jsGcmState.reportsInMainMemory )
			{	            
				
				// [RSTENSON] 
				// This inline transfer will be used to dma our predetermined value from mainmemory
				// right back to main memory by the rsx.  However we are doing this after the actual 
				// setReport and in a way using it as a label, but in PPU memory vs GPU local store. 
				// this will be much more efficient.  
				JS_ASSERT( queries->currentZPassQueryId < JSGCM_LM_MAX_USER_QUERIES );
	            
				// Use the reportCompletionOffset we got from cellGcmMapMemory during initialization of PSGL 
				GCM_FUNC( cellGcmInlineTransfer, _jsGcmState.reportCompletionOffset + 
						  (queries->currentZPassQueryId * 4), &_jsGcmState.reportCompletionValue, 
						  1, CELL_GCM_LOCATION_MAIN);
			}
			else
			{
				JS_ASSERT( queries->currentZPassQueryId < JSGCM_MM_MAX_USER_QUERIES );
			}

			_jsGcmPrintf(" End Query id %d \n", queries->currentZPassQueryId ); 
						
			queries->currentZPassQueryId = 0xffffffff;
            break;
		}
        default:
            JS_ASSERT( 0 );
            break;
    }
}

// Determine the status of the previous occlusion query.
// Returns false if the given queryID has not been used in a beginQuery / 
// endQuery pair, and places the value of the query in result.
GLboolean static inline _jsGcmFifoGlTestQuery( jsGcmEnum target, GLuint queryId, GLuint *result )
{
    switch ( target )
    {
        case JSGCM_SAMPLES_PASSED:
            // pick adjusted target offset
			JS_ASSERT( _jsGcmState.state.queries.currentZPassQueryId != queryId );

            // update the fifo put 
			_jsGcmFifoFlush( &_jsGcmState.fifo );

			// if reports in main memory we need to check that the zero value of the report has been set to zero
			// if that is the case then the report is ready.  
			if ( _jsGcmState.reportsInMainMemory )
			{	
				JS_ASSERT( queryId < JSGCM_MM_MAX_USER_QUERIES );

				if ( *(volatile GLuint *)&_jsGcmState.reportAddressMain[queryId].zero == 0 )
				{
					// need to get report on current id and return 
					*result = _jsGcmState.reportAddressMain[queryId].value; 
					//	cellGcmGetReport( CELL_GCM_ZPASS_PIXEL_CNT, queryId );

					_jsGcmPrintf(" Test Query return %d id %d \n", *result, queryId );				
					return GL_TRUE;
				}
				else
				{
					_jsGcmPrintf(" Test Query... Not Chet :(  reportMainZero Value %d \n", 
							*(volatile GLuint *)&_jsGcmState.reportAddressMain[queryId].zero ); 
					return GL_FALSE;
				}
			}
			// in the case of reports in local memory we have to wait for our inlineTransfer value to arrive in our 
			// reportCompletionLocation, much less efficient but it does work and not bad if a test is done for hundreds of queries 
			// not just a few  
			else
			{
	            JS_ASSERT( queryId < JSGCM_LM_MAX_USER_QUERIES );
    
				if ( *(volatile GLuint *)&_jsGcmState.reportCompletionValues[queryId] == JSGCM_REPORT_COMPLETE_VALUE )
				{
					// need to get report on current id and return 
					*result = cellGcmGetReport( CELL_GCM_ZPASS_PIXEL_CNT, queryId );

					_jsGcmPrintf(" Test Query return %d id %d \n", *result, queryId );				
					return GL_TRUE; 	
				}
				else
				{
					_jsGcmPrintf(" Test Query... Not Chet :(  reportValue %d \n", 
							*(volatile GLuint *)&_jsGcmState.reportCompletionValues[queryId]); 
					return GL_FALSE;
				}
			}


        default:
            JS_ASSERT( 0 );
            return GL_FALSE;
    }
    return GL_FALSE;
}

// Conditionally render any subsequent geometry based upon the ZCull value 
// from the query represented by queryId.  That is, if the query value is 
// 0, skip geometry rendering (though all state changes will occur).
void static inline _jsGcmFifoGlBeginConditionalRendering( GLuint queryId )
{
  	JS_ASSERT( queryId < JSGCM_MAX_USER_QUERIES );

	// Conditional rendering workaround bug 45096 
	// for more info on this issues see this technote https://ps3.scedev.net/technotes/view/614 
	 // Suggest workaround option (1) RSTENSON 
	GCM_FUNC( cellGcmSetReport, CELL_GCM_ZPASS_PIXEL_CNT,1024); 

    GCM_FUNC( cellGcmSetRenderEnable, CELL_GCM_CONDITIONAL, queryId );
}

// End conditionally rendering geometry.
void static inline _jsGcmFifoGlEndConditionalRendering( void )
{
    GCM_FUNC( cellGcmSetRenderEnable, CELL_GCM_TRUE, 0 );
}



// Masks the currently used clip planes, with each of the lowest 6 bits of 
// clip flags being the on / off state for a single clip plane.
void static inline _jsGcmFifoGlClipPlanes( GLuint clipFlags )
{
    // update attributes used
    jsGcmInterpolantState *s = &_jsGcmState.state.interpolant;
    s->clipMask = ( clipFlags << 6 ) | 0xfffff03f;

    GCM_FUNC( cellGcmSetVertexAttribOutputMask,
              ( s->vertexProgramAttribMask | s->pointSpriteMask ) &
              s->fragmentProgramAttribMask &
              s->clipMask &
              s->sideMask );
}

// Set the given alpha test function.
// Alpha reference is converted to a type appropriate to the render target type.
void static inline _jsGcmFifoGlAlphaFunc( jsGcmEnum af, GLfloat ref )
{
    jsGcmBlendState *blend = &_jsGcmState.state.blend;
    GLuint alphaRef = 0;

    blend->alphaFunc = af;
    blend->alphaRef = ref;

    // syntax check
    switch ( af )
    {
        case JSGCM_NEVER:
        case JSGCM_LESS:
        case JSGCM_EQUAL:
        case JSGCM_LEQUAL:
        case JSGCM_GREATER:
        case JSGCM_NOTEQUAL:
        case JSGCM_GEQUAL:
        case JSGCM_ALWAYS:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    // clamp argument
    switch ( _jsGcmState.renderTarget.colorFormat )
    {
        case JSGCM_NONE:
        case JSGCM_ARGB8:
            ref = JSGCM_CLAMPF_01( ref );
            alphaRef = JSGCM_QUICK_FLOAT2UINT( ref * 255.0f );
            break;
        case JSGCM_FLOAT_RGBX16:
        case JSGCM_FLOAT_RGBA16:
            alphaRef = _jsGcmGLfloattoGLushort( ref );
            break;
        case JSGCM_FLOAT_R32:
        case JSGCM_LUMINANCE32F_ARB:
        case JSGCM_FLOAT_RGBX32:
        case JSGCM_FLOAT_RGBA32:
            // no native support
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    GCM_FUNC( cellGcmSetAlphaFunc, af, alphaRef );
}

// Set blending constant, used for certain blending modes.
void static inline _jsGcmFifoGlBlendColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    jsGcmBlendState *blend = &_jsGcmState.state.blend;
    GLuint hwColor;
    GLuint hwColor2;

    // syntax check
    blend->r = r;
    blend->g = g;
    blend->b = b;
    blend->a = a;

    // program hw
    switch ( _jsGcmState.renderTarget.colorFormat )
    {
        case JSGCM_ARGB8:
            r = JSGCM_CLAMPF_01( r );
            g = JSGCM_CLAMPF_01( g );
            b = JSGCM_CLAMPF_01( b );
            a = JSGCM_CLAMPF_01( a );
            JSGCM_CALC_COLOR_LE_ARGB8( &hwColor, r, g, b, a );
            GCM_FUNC( cellGcmSetBlendColor, hwColor, hwColor );
            break;
        case JSGCM_FLOAT_RGBX16:
        case JSGCM_FLOAT_RGBA16:
            JSGCM_CALC_COLOR_F_ARGB16( &hwColor, &hwColor2, r, g, b, a );
            GCM_FUNC( cellGcmSetBlendColor, hwColor, hwColor2 );
            break;
        case JSGCM_NONE:
        case JSGCM_FLOAT_R32:
        case JSGCM_LUMINANCE32F_ARB:
        case JSGCM_FLOAT_RGBX32:
        case JSGCM_FLOAT_RGBA32:
            // no native support support
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

// Set the current blend equation.
void static inline _jsGcmFifoGlBlendEquation( jsGcmEnum mode, jsGcmEnum modeAlpha )
{
    // syntax check
    switch ( mode )
    {
        case JSGCM_FUNC_ADD:
        case JSGCM_MIN:
        case JSGCM_MAX:
        case JSGCM_FUNC_SUBTRACT:
        case JSGCM_FUNC_REVERSE_SUBTRACT:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    switch ( modeAlpha )
    {
        case JSGCM_FUNC_ADD:
        case JSGCM_MIN:
        case JSGCM_MAX:
        case JSGCM_FUNC_SUBTRACT:
        case JSGCM_FUNC_REVERSE_SUBTRACT:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetBlendEquation, mode, modeAlpha );
}

// Sets the source and destination factor used for blending.
void static inline _jsGcmFifoGlBlendFunc( jsGcmEnum sf, jsGcmEnum df, jsGcmEnum sfAlpha, jsGcmEnum dfAlpha )
{
    // syntax check
    switch ( sf )
    {
        case JSGCM_ZERO:
        case JSGCM_ONE:
        case JSGCM_SRC_COLOR:
        case JSGCM_ONE_MINUS_SRC_COLOR:
        case JSGCM_SRC_ALPHA:
        case JSGCM_ONE_MINUS_SRC_ALPHA:
        case JSGCM_DST_ALPHA:
        case JSGCM_ONE_MINUS_DST_ALPHA:
        case JSGCM_DST_COLOR:
        case JSGCM_ONE_MINUS_DST_COLOR:
        case JSGCM_SRC_ALPHA_SATURATE:
        case JSGCM_CONSTANT_COLOR:
        case JSGCM_ONE_MINUS_CONSTANT_COLOR:
        case JSGCM_CONSTANT_ALPHA:
        case JSGCM_ONE_MINUS_CONSTANT_ALPHA:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    switch ( sfAlpha )
    {
        case JSGCM_ZERO:
        case JSGCM_ONE:
        case JSGCM_SRC_COLOR:
        case JSGCM_ONE_MINUS_SRC_COLOR:
        case JSGCM_SRC_ALPHA:
        case JSGCM_ONE_MINUS_SRC_ALPHA:
        case JSGCM_DST_ALPHA:
        case JSGCM_ONE_MINUS_DST_ALPHA:
        case JSGCM_DST_COLOR:
        case JSGCM_ONE_MINUS_DST_COLOR:
        case JSGCM_SRC_ALPHA_SATURATE:
        case JSGCM_CONSTANT_COLOR:
        case JSGCM_ONE_MINUS_CONSTANT_COLOR:
        case JSGCM_CONSTANT_ALPHA:
        case JSGCM_ONE_MINUS_CONSTANT_ALPHA:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    switch ( df )
    {
        case JSGCM_ZERO:
        case JSGCM_ONE:
        case JSGCM_SRC_COLOR:
        case JSGCM_ONE_MINUS_SRC_COLOR:
        case JSGCM_SRC_ALPHA:
        case JSGCM_ONE_MINUS_SRC_ALPHA:
        case JSGCM_DST_ALPHA:
        case JSGCM_ONE_MINUS_DST_ALPHA:
        case JSGCM_DST_COLOR:
        case JSGCM_ONE_MINUS_DST_COLOR:
        case JSGCM_SRC_ALPHA_SATURATE:
        case JSGCM_CONSTANT_COLOR:
        case JSGCM_ONE_MINUS_CONSTANT_COLOR:
        case JSGCM_CONSTANT_ALPHA:
        case JSGCM_ONE_MINUS_CONSTANT_ALPHA:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    switch ( dfAlpha )
    {
        case JSGCM_ZERO:
        case JSGCM_ONE:
        case JSGCM_SRC_COLOR:
        case JSGCM_ONE_MINUS_SRC_COLOR:
        case JSGCM_SRC_ALPHA:
        case JSGCM_ONE_MINUS_SRC_ALPHA:
        case JSGCM_DST_ALPHA:
        case JSGCM_ONE_MINUS_DST_ALPHA:
        case JSGCM_DST_COLOR:
        case JSGCM_ONE_MINUS_DST_COLOR:
        case JSGCM_SRC_ALPHA_SATURATE:
        case JSGCM_CONSTANT_COLOR:
        case JSGCM_ONE_MINUS_CONSTANT_COLOR:
        case JSGCM_CONSTANT_ALPHA:
        case JSGCM_ONE_MINUS_CONSTANT_ALPHA:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetBlendFunc, sf, df, sfAlpha, dfAlpha );
}


// Set up culling of front or back faces.
void static inline _jsGcmFifoGlCullFace( jsGcmEnum cfm )
{
    // syntax check
    switch ( cfm )
    {
        case JSGCM_FRONT:
        case JSGCM_BACK:
        case JSGCM_FRONT_AND_BACK:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetCullFace, cfm );
}

// Set depth bounds.  Default is [0, 1], and all accepted ranges must 
// be within that range.  Pixels outside of the specified range will be 
// discarded.
void static inline _jsGcmFifoGlDepthBoundsf( GLclampf zmin, GLclampf zmax )
{
    GLfloat zminf = ( GLfloat )zmin;
    GLfloat zmaxf = ( GLfloat )zmax;

    // spec says clamped.
    zminf = JSGCM_CLAMPF_01( zminf );
    zmaxf = JSGCM_CLAMPF_01( zmaxf );

    // syntax check (supposed to be a nop in release builds)
    if ( zmin > zmax )
    {
        JS_ASSERT( 0 );
    }

    GCM_FUNC( cellGcmSetDepthBounds, zminf, zmaxf );
}

// Specify winding mode for polygon facing.
void static inline _jsGcmFifoGlFrontFace( jsGcmEnum dir )
{
    jsGcmRenderTarget *rt = &_jsGcmState.renderTarget;
    jsGcmViewportState *v = &_jsGcmState.state.viewport;

    // syntax check
    switch ( dir )
    {
        case JSGCM_CW:
        case JSGCM_CCW:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    // frontFace depends on yInverted
    v->frontFace = dir;
    if ( !rt->yInverted )
    {
        dir = ( dir == JSGCM_CW ) ? JSGCM_CCW : JSGCM_CW;
        }

    GCM_FUNC( cellGcmSetFrontFace, dir );
}

// Set rendered line width
void static inline _jsGcmFifoGlLineWidth( GLfloat width )
{
    const float ALIASED_LINE_WIDTH_MINIMUM = 0.0f;
    const float ALIASED_LINE_WIDTH_MAXIMUM = 63.875f; // 63.875 (fixed-point 6.3 format)
    GLuint hwLineWidth;

    // Clamp to permissible range
    JS_ASSERT( width > 0.0f );
    if ( width < ALIASED_LINE_WIDTH_MINIMUM )
    {
        width = ALIASED_LINE_WIDTH_MINIMUM;
    }
    if ( width > ALIASED_LINE_WIDTH_MAXIMUM )
    {
        width = ALIASED_LINE_WIDTH_MAXIMUM;
    }

    hwLineWidth = JSGCM_QUICK_FLOAT2UINT( width*8.0f );
    GCM_FUNC( cellGcmSetLineWidth, hwLineWidth );
}

// Set a logical operation to perform between source and destination colors
// at a given location in the frame buffer.
void static inline _jsGcmFifoGlLogicOp( jsGcmEnum op )
{
    // syntax check
    switch ( op )
    {
        case JSGCM_CLEAR:
        case JSGCM_AND:
        case JSGCM_AND_REVERSE:
        case JSGCM_COPY:
        case JSGCM_AND_INVERTED:
        case JSGCM_NOOP:
        case JSGCM_XOR:
        case JSGCM_OR:
        case JSGCM_NOR:
        case JSGCM_EQUIV:
        case JSGCM_INVERT:
        case JSGCM_OR_REVERSE:
        case JSGCM_COPY_INVERTED:
        case JSGCM_OR_INVERTED:
        case JSGCM_NAND:
        case JSGCM_SET:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetLogicOp, op );
}

// Set the a polygon offset scale and bias used for depth value computation.
void static inline _jsGcmFifoGlPolygonOffset( GLfloat factor, GLfloat units )
{
    // We bias by two steps in the Z buffer for each step specified in <units>
    // because one step is not enough in some cases due to the implementation
    // of the floating point math that the bias is applied to.  See bug 67560.
    GCM_FUNC( cellGcmSetPolygonOffset, factor, 2.0f*units );
}

// Set the size (diameter) of rendered points.
void static inline _jsGcmFifoGlPointSize( GLfloat size )
{
    const float ALIASED_POINT_SIZE_MINIMUM = 1.0f;
    const float ALIASED_POINT_SIZE_MAXIMUM = 63.0f; // our OpenGL default

    // Clamp to permissible range
    JS_ASSERT( size > 0.0f );
    if ( size < ALIASED_POINT_SIZE_MINIMUM )
    {
        size = ALIASED_POINT_SIZE_MINIMUM;
    }
    if ( size > ALIASED_POINT_SIZE_MAXIMUM )
    {
        size = ALIASED_POINT_SIZE_MAXIMUM;
    }

    GCM_FUNC( cellGcmSetPointSize, size );
}

// Specify the shade model.
void static inline _jsGcmFifoGlShadeModel( jsGcmEnum sm )
{
    // syntax check
    switch ( sm )
    {
        case JSGCM_FLAT:
        case JSGCM_SMOOTH:
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    GCM_FUNC( cellGcmSetShadeMode, sm );
}

// Set the active stencil face 
void static inline _jsGcmFifoGlActiveStencilFace( jsGcmEnum face )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;

    // cache state for glStencilFunc/Op/Mask
    switch ( face )
    {
        case JSGCM_FRONT:
            s->activeStencilFace = 0;
            break;
        case JSGCM_BACK:
            s->activeStencilFace = 1;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

void _jsEndGPUFinishFence();
void _jsBeginGPUFinishFence();

// Wait for the gpu to pass the given fence in the command buffer.
void static inline _jsGcmFifoGlFinishFenceRef( const GLuint ref )
{
    jsGcmFifo *fifo = &_jsGcmState.fifo;

    // wait for completion
    _jsBeginGPUFinishFence();
    for ( ;; )
    {
        // gpu passed reference ?
        if ( !_jsGcmFifoReferenceInUse( fifo, ref ) )
        {
            break;
        }

        // avoid polling on bus (interrupts memory traffic)
        _jsGcmFifoDelayRegisterRead( fifo );
    }
    _jsEndGPUFinishFence();
}

#define JSGCM_UTIL_LABEL_INDEX 253

// Utility function that does GPU 'finish'.
void static inline _jsGcmUtilFinish(void)
{
	GCM_FUNC( cellGcmSetWriteBackEndLabel, JSGCM_UTIL_LABEL_INDEX, 
	                             _jsGcmState.labelValue );
	cellGcmFlush();

	while( *(cellGcmGetLabelAddress( JSGCM_UTIL_LABEL_INDEX)) != _jsGcmState.labelValue)
	{
		sys_timer_usleep(30);
	}

	_jsGcmState.labelValue++;
}


// Utility to let RSX wait for complete RSX pipeline idle
void static inline _jsGcmUtilWaitForIdle()
{
	// set write label command in push buffer, and wait
	// NOTE: this is for RSX to wailt
	GCM_FUNC( cellGcmSetWriteBackEndLabel, JSGCM_UTIL_LABEL_INDEX, _jsGcmState.labelValue );
	GCM_FUNC( cellGcmSetWaitLabel, JSGCM_UTIL_LABEL_INDEX, _jsGcmState.labelValue );

	// increment label value for next time. 
	_jsGcmState.labelValue++; 

	// make sure the entire pipe in clear not just the front end 
	_jsGcmUtilFinish();
}

#endif
