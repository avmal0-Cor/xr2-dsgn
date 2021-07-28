/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * File description here
 */



#ifndef  _H_JSGCM_DRAWELEMENTS_H_
#define _H_JSGCM_DRAWELEMENTS_H_

/////////////////////////////////////////////////////////////////////////////
// Platform device specific part of GlDrawElements
//
// Mostly maps one to one between GL and GCM.
// mode includes JSGCM_POLYGON which is not supported at GL layer.
// count and indices must be in range between 0 to 0xffff
// types must be JSGCM_UNSIGNED_INT or JSGCM_UNSIGNED_SHORT
// 16bit should be selected for maximum performance
// tristrip_restart index is 0xffff/0xffffffff
void _jsDumpFifo( char * name );

void static inline _jsGcmFifoGlDrawElements( jsGcmEnum mode, GLsizei count, jsGcmEnum type,     
                                             GLboolean       isMain,
                                             const GLuint offset )
{
    uint8_t gcmMode = 0;
    switch ( mode )
    {
        case JSGCM_POINTS:
            gcmMode = CELL_GCM_PRIMITIVE_POINTS;
            break;
        case JSGCM_LINES:
            gcmMode = CELL_GCM_PRIMITIVE_LINES;
            break;
        case JSGCM_LINE_LOOP:
            gcmMode = CELL_GCM_PRIMITIVE_LINE_LOOP;
            break;
        case JSGCM_LINE_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_LINE_STRIP;
            break;
        case JSGCM_TRIANGLES:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLES;
            break;
        case JSGCM_TRIANGLE_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_STRIP;
            break;
        case JSGCM_TRIANGLE_FAN:
            gcmMode = CELL_GCM_PRIMITIVE_TRIANGLE_FAN;
            break;
        case JSGCM_QUADS:
            gcmMode = CELL_GCM_PRIMITIVE_QUADS;
            break;
        case JSGCM_QUAD_STRIP:
            gcmMode = CELL_GCM_PRIMITIVE_QUAD_STRIP;
            break;
        case JSGCM_POLYGON:
            gcmMode = CELL_GCM_PRIMITIVE_POLYGON;
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }

    JS_ASSERT( _jsGcmCheckConsistency() );

    uint8_t gcmType = 0;
    switch ( type )
    {
        case JSGCM_UNSIGNED_INT:
            gcmType = CELL_GCM_DRAW_INDEX_ARRAY_TYPE_32; // 0
            break;
        case JSGCM_UNSIGNED_SHORT:
            gcmType = CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16; // 1
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    uint8_t location = CELL_GCM_LOCATION_LOCAL;  

    if ( isMain ) 
    {
        location = CELL_GCM_LOCATION_MAIN;
    }

    if ( count )
    {
        GCM_FUNC( cellGcmSetDrawIndexArray, gcmMode, count, gcmType, location, offset );
    }

}

#endif
