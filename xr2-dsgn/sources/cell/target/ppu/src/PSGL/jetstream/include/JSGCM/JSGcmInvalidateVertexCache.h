/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmInvalidateVertexCache.h
 */

#ifndef _JSGCM_INVALIDATEVERTEXCACHE_H_
#define _JSGCM_INVALIDATEVERTEXCACHE_H_

/* invalidates the gpu's internal vertex cache
**
** notes:
**   - the vertex cache is based on indices. If the vertex data (the index
**     is pointing at) change, the vertex cache needs to be manually
**     invalidated or stale vertex information may be used.
**   - call whenever the vertex buffers get updated or glVertexAttribPointer
**     points to new address
*/
void static inline _jsGcmFifoGlInvalidateVertexCache( void )
{
    GCM_FUNC_NO_ARGS( cellGcmSetInvalidateVertexCache );
}

// cache commands in cmdBuffer, return updated pointer
static inline GLvoid* _jsGcmFifoGlInvalidateVertexCacheBuffered( GLvoid* cmdBuffer )
{
    GCM_FUNC_BUFFERED_NO_ARGS( cellGcmSetInvalidateVertexCache, cmdBuffer );
    return cmdBuffer;
}

#endif

