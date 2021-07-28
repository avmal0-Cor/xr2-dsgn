/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * Functions to transfer data, including between different source
 * and destination locations (i.e., RSX and main memory).
 */

#include <JSGCM/JSGcmUtils.h>


/* blit function
**
** notes:
**  - pitch values can be negative
*/
void _jsGcmTransferData
(
    GLuint dstId,
    GLuint dstIdOffset, 
    GLint dstPitch,
    GLuint srcId,
    GLuint srcIdOffset,
    GLint srcPitch,
    GLint bytesPerRow,
    GLint rowCount
)
{
    GLuint dstOffset = gmmIdToOffset(dstId) + dstIdOffset;
    GLuint srcOffset = gmmIdToOffset(srcId) + srcIdOffset;

    uint8_t mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;

    if ( gmmIdIsMain(srcId) && gmmIdIsMain(dstId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_MAIN;
    }

    // choose source DMA context
    else if ( gmmIdIsMain(srcId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL;
    }

    // choose destination DMA context
    else if ( gmmIdIsMain(dstId) )
    {
        mode = CELL_GCM_TRANSFER_LOCAL_TO_MAIN;
    }

    GCM_FUNC( cellGcmSetTransferData, mode, dstOffset, dstPitch, srcOffset, srcPitch, bytesPerRow, rowCount );
}


/* vid->vid blit function (transfer data for images)
**
** note:
**   - the destination surface&pitch must be 64byte aligned
**   - (0, 0) is in the upper left!
*/
void _jsGcmTransferDataVidToVid
(
    GLuint dstId,        // linear destination surface
    GLuint dstIdOffset,
    GLuint dstPitch,
    GLuint dstX,
    GLuint dstY,
    GLuint srcId,        // linear source surface
    GLuint srcIdOffset,
    GLuint srcPitch,
    GLuint srcX,
    GLuint srcY,
    GLuint width,            // size in pixel
    GLuint height,
    GLuint bytesPerPixel
)
{
    GLuint dstOffset = gmmIdToOffset(dstId) + dstIdOffset;
    GLuint srcOffset = gmmIdToOffset(srcId) + srcIdOffset;

    uint8_t mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;

    if ( gmmIdIsMain(srcId) && gmmIdIsMain(dstId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_MAIN;
    }

    // choose source DMA context
    else if ( gmmIdIsMain(srcId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL;
    }

    // choose destination DMA context
    else if ( gmmIdIsMain(dstId) )
    {
        mode = CELL_GCM_TRANSFER_LOCAL_TO_MAIN;
    }

    GCM_FUNC( cellGcmSetTransferImage, mode, dstOffset, dstPitch, dstX, dstY, srcOffset, srcPitch, srcX, srcY, width, height, bytesPerPixel );
}
