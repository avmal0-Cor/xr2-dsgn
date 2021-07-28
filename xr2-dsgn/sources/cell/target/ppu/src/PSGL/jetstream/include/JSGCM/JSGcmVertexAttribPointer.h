/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmVertexAttribPointer.h
 */

#ifndef _H_JSGCM_VERTEXATTRIBPOINTER_H_
#define _H_JSGCM_VERTEXATTRIBPOINTER_H_

void static inline _jsGcmFifoGlVertexAttribPointer
(
    GLuint          index,
    GLint           size,
    jsGcmEnum       type,
    GLboolean       normalized,
    GLsizei         stride,
    GLushort        frequency,
    GLboolean       isMain,
    GLuint          offset
)
{
    // syntax check
    JS_ASSERT( index <= 15 );
    JS_ASSERT( stride >= 0 );
    switch ( size )
    {
        case 0: // disable
            stride = 0;
            normalized = 0;
            type = JSGCM_FLOAT;
            offset = 0;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // valid
            break;
        default:
            JS_ASSERT( 0 ); // invalid
            break;
    }

    // mapping to native types
    uint8_t gcmType = 0;
    switch ( type )
    {
        case JSGCM_UNSIGNED_BYTE:
            if (normalized)
            {
                gcmType = CELL_GCM_VERTEX_UB;
            }
            else
            {
                JS_ASSERT( size == 4 ); // 4 components only
                gcmType = CELL_GCM_VERTEX_UB256;
            }
            break;

        case JSGCM_SHORT:
            gcmType = normalized ? CELL_GCM_VERTEX_S1 : CELL_GCM_VERTEX_S32K;
            break;

        case JSGCM_FLOAT:
            gcmType = CELL_GCM_VERTEX_F;
            break;

        case JSGCM_HALF_FLOAT:
            gcmType = CELL_GCM_VERTEX_SF;
            break;

        case JSGCM_CMP:
            size = 1;   // required for this format
            gcmType = CELL_GCM_VERTEX_CMP;
            break;

        default:
            JS_ASSERT( 0 ); // invalid
            break;
    }

    uint8_t location = CELL_GCM_LOCATION_LOCAL;

    if ( isMain )
    {
        location = CELL_GCM_LOCATION_MAIN;
    }

    GCM_FUNC( cellGcmSetVertexDataArray, index, frequency, stride, size, gcmType, location, offset );
}

// cache commands in cmdBuffer, return updated pointer
static inline GLvoid* _jsGcmFifoGlVertexAttribPointerBuffered
(
    GLvoid         *cmdBuffer,
    GLuint          index,
    GLint           size,
    jsGcmEnum        type,
    GLboolean       normalized,
    GLsizei         stride,
    GLushort        frequency,
    GLboolean       isMain,
    GLuint          offset
)
{
    // syntax check
    JS_ASSERT( index <= 15 );
    JS_ASSERT( stride >= 0 );
    switch ( size )
    {
        case 0: // disable
            stride = 0;
            normalized = 0;
            type = JSGCM_FLOAT;
            offset = 0;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // valid
            break;
        default:
            JS_ASSERT( 0 ); // invalid
            break;
    }

    // mapping to native types
    uint8_t gcmType = 0;
    switch ( type )
    {
        case JSGCM_UNSIGNED_BYTE:
            if (normalized)
            {
                gcmType = CELL_GCM_VERTEX_UB;
            }
            else
            {
                JS_ASSERT( size == 4 ); // 4 components only
                gcmType = CELL_GCM_VERTEX_UB256;
            }
            break;

        case JSGCM_SHORT:
            gcmType = normalized ? CELL_GCM_VERTEX_S1 : CELL_GCM_VERTEX_S32K;
            break;

        case JSGCM_FLOAT:
            gcmType = CELL_GCM_VERTEX_F;
            break;

        case JSGCM_HALF_FLOAT:
            gcmType = CELL_GCM_VERTEX_SF;
            break;

        case JSGCM_CMP:
            size = 1;   // required for this format
            gcmType = CELL_GCM_VERTEX_CMP;
            break;

        default:
            JS_ASSERT( 0 ); // invalid
            break;
    }

    uint8_t location = CELL_GCM_LOCATION_LOCAL;   

    if ( isMain ) 
    {
        location = CELL_GCM_LOCATION_MAIN; 
    }

    GCM_FUNC_BUFFERED( cellGcmSetVertexDataArray, cmdBuffer, index, frequency, stride, size, gcmType, location, offset );
    return cmdBuffer;
}

// set the vertex attribute to the specified value.
void static inline _jsGcmFifoGlVertexAttrib4fv( GLuint index, const GLfloat v[4] )
{
    GCM_FUNC( cellGcmSetVertexData4f, index, v );
}

// cache commands in cmdBuffer, return updated pointer
static inline GLvoid* _jsGcmFifoGlVertexAttrib4fvBuffered( GLvoid* cmdBuffer, GLuint index, const GLfloat v[4] )
{
    GCM_FUNC_BUFFERED( cellGcmSetVertexData4f, cmdBuffer, index, v );
    return cmdBuffer;
}

// specifices the vertex frequency operation
void static inline _jsGcmFifoGlVertexFrequencyOperation( GLushort operation )
{
    GCM_FUNC( cellGcmSetFrequencyDividerOperation, operation );
}

// cache commands in cmdBuffer, return updated pointer
static inline GLvoid* _jsGcmFifoGlVertexFrequencyOperationBuffered( GLvoid* cmdBuffer, GLushort operation )
{
    GCM_FUNC_BUFFERED( cellGcmSetFrequencyDividerOperation, cmdBuffer, operation );
    return cmdBuffer;
}


// specified the restart index used by glDrawElements when JSGCM_PRIMITIVE_RESTART is enabled.
void static inline _jsGcmFifoGlPrimitiveRestartIndex( GLuint index )
{
    GCM_FUNC( cellGcmSetRestartIndex, index );
}

static inline GLvoid* _jsGcmFifoGlPrimitiveRestartIndexBuffered( GLvoid* cmdBuffer, GLuint index )
{
    GCM_FUNC_BUFFERED( cellGcmSetRestartIndex, cmdBuffer, index );
    return cmdBuffer;
}

#endif
