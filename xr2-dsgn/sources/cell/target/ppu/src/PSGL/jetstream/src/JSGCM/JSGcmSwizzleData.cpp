/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmSwizzleData.cpp
 */

#include <JSGCM/JSGcmUtils.h>
#include <string.h>

/* file content: generic cpu texture swizzler:
**
** notes:
**   - cpu swizzle: straight forward and good to understand but terrible slow
**   - gpu swizzle: does not handle all cases, but is much faster
*/

/* calculate the swizzled address given texel coordinates
** and the log2 extents of the texture
**
** note:
**  - generic code, not optimized
*/
static GLuint _jsGcmCalcSwizzledAddress
(
    GLuint u,
    GLuint v,
    GLuint p,
    GLuint log2depth,
    GLuint log2heigth,
    GLuint log2width )
{
    GLuint offsetBitPos = 0;
    GLuint offset = 0;

    while ( log2depth + log2heigth + log2width )
    {
        if ( log2width )
        {
            log2width--;
            offset |= ( u & 1 ) << offsetBitPos;
            u >>= 1;
            offsetBitPos++;
        }
        if ( log2heigth )
        {
            log2heigth--;
            offset |= ( v & 1 ) << offsetBitPos;
            v >>= 1;
            offsetBitPos++;
        }
        if ( log2depth )
        {
            log2depth--;
            offset |= ( p & 1 ) << offsetBitPos;
            p >>= 1;
            offsetBitPos++;
        }
    }

    return offset;
}

/* swizzle data for non-square textures.  This is a generic function that
** uses the CPU to swizzle a texture (w/without power of two extents) into
** another texture that has power of two extents.
**
** note:
**  - a lot of parameters but there but essentially it is just a source and destintion
**    surface description
**  - this function does not sync anything (the caller is responsilbe for this)
**  - this function is slow because it is generic code, not even attempted to
**    have any optimization (does a per texel copy on the cpu)
*/
void _jsGcmSwizzleDataCPU
(
    char      *swizzledTexture,
    const char*texture,
    GLuint               dstx0,
    GLuint               dsty0,
    GLuint               dstz0,
    GLuint               dstWidth,
    GLuint               dstHeight,
    GLuint               dstDepth,
    GLuint               srcx0,
    GLuint               srcy0,
    GLuint               srcz0,
    GLuint               srcx1,
    GLuint               srcy1,
    GLuint               srcz1,
    GLuint               srcWidth,
    GLuint               srcHeight,
    GLuint               srcDepth,
    GLuint               dstLog2cdepth,
    GLuint               srcColordepth,
    GLuint               border,
    GLuint               dim,
    void( *copyTexel )( void *dst, const void *src )
)
{
    // handle semi-fat/fat as a wider 32-bit texture
    switch ( dstLog2cdepth )
    {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            dstWidth <<= 1;
            dstx0 <<= 1;
            srcx0 <<= 1;
            srcx1 <<= 1;
            srcWidth <<= 1;
            dstLog2cdepth -= 1;
            srcColordepth >>= 1;
            break;
        case 4:
            dstWidth <<= 2;
            dstx0 <<= 2;
            srcx0 <<= 2;
            srcx1 <<= 2;
            srcWidth <<= 2;
            dstLog2cdepth -= 2;
            srcColordepth >>= 2;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    GLuint dstLog2Width  = JSGCM_FLOOR_LOG2( dstWidth );
    GLuint dstLog2Height = JSGCM_FLOOR_LOG2( dstHeight );
    GLuint dstLog2Depth  = JSGCM_FLOOR_LOG2( dstDepth );
    GLuint texel = 0;
    GLuint dstU, dstV, dstP;
    GLuint srcU, srcV, srcP;

    // If we are doing a sub-texture update, we need to offset the
    // destination by 3 pixels.
    if ( border )
    {
        if ( srcx0 != 0 )
        {
            dstx0 += 3;
        }

        if ( srcy0 != 0 )
        {
            dsty0 += 3;
        }

        if ( dim > 2 )
        {
            dstz0 += 3;
        }
    }

    srcP = srcz0;
    dstP = dstz0;

    while ( srcP < srcz1 )
    {
        srcV = srcy0;
        dstV = dsty0;

        while ( srcV < srcy1 )
        {
            srcU = srcx0;
            dstU = dstx0;

            while ( srcU < srcx1 )
            {
                GLuint newTexel = _jsGcmCalcSwizzledAddress( dstU, dstV, dstP,
                                  dstLog2Depth,
                                  dstLog2Height,
                                  dstLog2Width ) << dstLog2cdepth;

                texel = (( srcP * srcWidth * srcHeight ) +
                         ( srcV * srcWidth ) +
                         ( srcU ) ) * srcColordepth;

                // use the caller supplied texel copy function if there is one...
                if ( copyTexel )
                {
                    copyTexel( &swizzledTexture[newTexel], &texture[texel] );
                }
                else
                {
                    memcpy( &swizzledTexture[newTexel], &texture[texel], 1 << dstLog2cdepth );
                }

                // Increment the src location if:
                //
                // - We aren't dealing with a bordered texture
                // - We are a bordered texture, but we have replicated the border to be a
                //   four pixel border ( hardware feature )
                //
                if ( border &&
                        ((( srcU == 0 ) && ( dstU < 3 ) ) ||
                         (( srcU == ( srcWidth - 1 ) ) && ( dstU < ( srcWidth + 6 ) ) ) ) )
                {}
                else
                {
                    srcU++;
                }

                dstU++;
            }

            // See comment above
            if ( !( border &&
                    ((( srcV == 0 ) && ( dstV < 3 ) ) ||
                     (( srcV == ( srcHeight - 1 ) ) && ( dstV < ( srcHeight + 6 ) ) ) ) ) )
            {
                srcV++;
            }

            dstV++;
        }

        srcP++;
        dstP++;
    }
}


/* asynchronous gpu swizzle function (vid::linear to vid::swizzled)
**
** notes:
**   - don't use for now.
*/


void _jsGcmSwizzleDataVidToVid
(
    GLuint dstVidId,        // swizzled destination surface
    GLuint dstVidIdOffset,
    GLuint dstWidth,
    GLuint dstHeight,
    GLuint dstX,
    GLuint dstY,
    GLuint srcVidId, // linear source surface
    GLuint srcVidIdOffset,
    GLuint srcPitch,
    GLuint srcX,
    GLuint srcY,
    GLuint width,            // update area
    GLuint height,
    GLuint bytesPerPixel
)
{
    GLuint srcOffset = gmmIdToOffset( srcVidId ) + srcVidIdOffset;
    GLuint dstOffset = gmmIdToOffset( dstVidId ) + dstVidIdOffset;

    uint8_t mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;

    if ( gmmIdIsMain(srcVidId) && gmmIdIsMain(dstVidId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_MAIN;
    }

    // choose source DMA context
    else if ( gmmIdIsMain(srcVidId) )
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_LOCAL;
    }

    // choose destination DMA context
    else if ( gmmIdIsMain(dstVidId) )
    {
        mode = CELL_GCM_TRANSFER_LOCAL_TO_MAIN;
    }

	// [RSTENSON] GCM_FUNCTION_160_UPDATE
    GCM_FUNC( cellGcmSetConvertSwizzleFormat, dstOffset, dstWidth, dstHeight, dstX, dstY, srcOffset, srcPitch, srcX, srcY, width, height, bytesPerPixel, mode );
}
