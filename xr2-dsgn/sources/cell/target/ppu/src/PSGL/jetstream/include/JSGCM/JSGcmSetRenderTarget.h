/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * JSGcmSetRenderTarget.h
 */

#ifndef _H_JSGCM_API_SETRENDERTARGET_H_
#define _H_JSGCM_API_SETRENDERTARGET_H_

#include <JSGCM/JSGcmFifoUtils.h>


// Set current render target to args
void _jsGcmFifoGlSetRenderTarget( jsGcmRenderTargetEx const * const args );

// Explicitly invalidate the L2 texture cache
void static inline _jsGcmFifoGlInvalidateTextureCache( void )
{
    GCM_FUNC( cellGcmSetInvalidateTextureCache, CELL_GCM_INVALIDATE_TEXTURE );
}

#endif
