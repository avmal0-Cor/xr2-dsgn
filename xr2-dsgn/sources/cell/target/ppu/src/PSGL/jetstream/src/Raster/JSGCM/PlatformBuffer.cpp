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
#include <PSGL/Utils.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDriver.h>

// Buffers are tagged with one of four usage hints: SYSTEM, STATIC, STREAM,
// and DYNAMIC.  STREAM and DYNAMIC behave the same way (which was not the
// case through 0.8.1).  SYSTEM buffers are allocated from the SYSTEM memory
// pool, while the others map are allocated from LINEAR GPU memory.
//
// Aside from their memory pool, the buffer types work the same way *except*
// when setting data into a proper subset of the buffer via glBufferSubData.
// In this case, the data is transferred via a bounce buffer for
// STREAM/DYNAMIC buffers and a Finish is issued for SYSTEM and STATIC
// buffers.

static void _jsDeallocateBuffer( jsBufferObject* bufferObject )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    // free buffer
    //  These frees are fenced so the memory cannot be reallocated while the
    //  GPU is still using it.
    switch ( jsBuffer->pool )
    {
        case _JSGCM_SURFACE_POOL_SYSTEM:
            _jsGcmFreeHostMemory( jsBuffer->bufferId );
            break;
        case _JSGCM_SURFACE_POOL_LINEAR:
            _jsGcmFreeMemory( jsBuffer->bufferId );
            break;
        case _JSGCM_SURFACE_POOL_TILED_COLOR:
        case _JSGCM_SURFACE_POOL_SYSTEM_TILED_COLOR:
            _jsGcmFreeTiledSurface( jsBuffer->bufferId );
            break;
        case _JSGCM_SURFACE_POOL_TILED_DEPTH:
            _jsGcmFreeDepthStencilSurface( GL_TRUE, jsBuffer->bufferId );
            break;
        case _JSGCM_SURFACE_POOL_SYSTEM_TILED_DEPTH:
            _jsGcmFreeDepthStencilSurface( GL_FALSE, jsBuffer->bufferId );
            break;
        case _JSGCM_SURFACE_POOL_NONE:
            break;
        default:
            JS_ASSERT( 0 );
    }

    jsBuffer->pool = _JSGCM_SURFACE_POOL_NONE;
    jsBuffer->bufferId = GMM_ERROR;
}

static void _jsAllocateBuffer( jsBufferObject* bufferObject )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    // free current buffer (if any)
    _jsDeallocateBuffer( bufferObject );

    if ( bufferObject->isSurface )
    {
        GLuint antiAliasingMode = 1;
        GLboolean isLocalMemory = (bufferObject->usage == GL_SYSTEM_DRAW_SCE) ? GL_FALSE : GL_TRUE;
        switch ( bufferObject->multisampling )
        {
            case GL_MULTISAMPLING_NONE_SCE:
                antiAliasingMode = 1;
                break;
            case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
                antiAliasingMode = 2;
                break;
            case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
                antiAliasingMode = 3;
                break;
            case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
                antiAliasingMode = 4;
                break;
            default:
                break;
        }
        if ( bufferObject->internalFormat == JSGCM_DEPTH_COMPONENT16 || bufferObject->internalFormat == JSGCM_DEPTH_COMPONENT24 )
        {
            jsBuffer->pool = isLocalMemory ? _JSGCM_SURFACE_POOL_TILED_DEPTH : _JSGCM_SURFACE_POOL_SYSTEM_TILED_DEPTH;
            _jsGcmAllocateDepthStencilSurface( isLocalMemory, bufferObject->width, bufferObject->height, bufferObject->internalFormat, antiAliasingMode, &jsBuffer->bufferId, &jsBuffer->pitch, &jsBuffer->bufferSize );
        }
        else
        {
            jsBuffer->pool = isLocalMemory ? _JSGCM_SURFACE_POOL_TILED_COLOR : _JSGCM_SURFACE_POOL_SYSTEM_TILED_COLOR;
            _jsGcmAllocateColorSurface( isLocalMemory, bufferObject->width, bufferObject->height, _jsPlatformGetBitsPerPixel( bufferObject->internalFormat ), 0, antiAliasingMode, &jsBuffer->bufferId, &jsBuffer->pitch, &jsBuffer->bufferSize );
        }
        bufferObject->size = jsBuffer->bufferSize;
    }
    else
    {
        switch ( bufferObject->usage )
        {
            case GL_SYSTEM_DRAW_SCE:
                // allocate in host memory
                jsBuffer->pool = _JSGCM_SURFACE_POOL_SYSTEM;
                jsBuffer->bufferId = _jsGcmAllocateHostMemory( jsBuffer->bufferSize );
                jsBuffer->pitch = 0;
                break;
            default:
                // allocate in GPU memory
                jsBuffer->pool = _JSGCM_SURFACE_POOL_LINEAR;
                jsBuffer->bufferId = _jsGcmAllocateMemory( jsBuffer->bufferSize );
                jsBuffer->pitch = 0;
                break;
        }
    }

    if ( jsBuffer->bufferId == GMM_ERROR )
        jsBuffer->pool = _JSGCM_SURFACE_POOL_NONE;

    GLuint referenceCount = bufferObject->textureReferences.getCount();
    if ( referenceCount > 0 )
    {
        for ( GLuint i = 0;i < referenceCount;++i )
        {
            jsTexture *texture = bufferObject->textureReferences[i];
            JS_ASSERT( texture->referenceBuffer == bufferObject );
            jsGcmTexture *gcmTexture = ( jsGcmTexture * )texture->platformTexture;
            gcmTexture->gpuAddressId = jsBuffer->bufferId;
            gcmTexture->gpuAddressIdOffset = texture->offset;
            texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
            _jsTextureTouchFBOs( texture );
        }
        _CurrentContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED ;
    }
}

int _jsPlatformBufferObjectSize()
{
    return sizeof( jsGcmBufferObject );
}

GLboolean _jsPlatformCreateBufferObject( jsBufferObject* bufferObject )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    jsBuffer->pool = _JSGCM_SURFACE_POOL_NONE;
    jsBuffer->bufferId = GMM_ERROR;
    jsBuffer->mapCount = 0;
    jsBuffer->mapAccess = GL_NONE;

    // allocate initial buffer
    jsBuffer->bufferSize = _jsPad( bufferObject->size, _JS_BUFFER_OBJECT_BLOCK_SIZE );
    _jsAllocateBuffer( bufferObject );

    return jsBuffer->bufferId != GMM_ERROR;
}

void _jsPlatformDestroyBufferObject( jsBufferObject* bufferObject )
{
    _jsDeallocateBuffer( bufferObject );
}

void _jsPlatformBufferObjectSetData( jsBufferObject* bufferObject, GLintptr offset, GLsizeiptr size, const GLvoid *data, GLboolean tryImmediateCopy )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    if ( size == bufferObject->size && tryImmediateCopy )
    {
        fast_memcpy( gmmIdToAddress( jsBuffer->bufferId ) + offset, data, size );
    }
    else
        if ( size >= bufferObject->size )
        {
            // writing the entire buffer
            JS_ASSERT( offset == 0 );

            // reallocate the buffer
            //  To avoid waiting for the GPU to finish with the buffer, just
            //  allocate a whole new one.
            jsBuffer->bufferSize = _jsPad( size, _JS_BUFFER_OBJECT_BLOCK_SIZE );
            _jsAllocateBuffer( bufferObject );

            // copy directly to newly allocated memory
            //  TODO: For GPU destination, should we copy to system memory and
            //  pull from GPU?
            switch ( jsBuffer->pool )
            {
                case _JSGCM_SURFACE_POOL_NONE:
                    _jsSetError( GL_OUT_OF_MEMORY );
                    return;
                default:
                    fast_memcpy( gmmIdToAddress( jsBuffer->bufferId ), data, size );
                    break;
            }
        }
        else
        {
            if ( tryImmediateCopy )
            {
                fast_memcpy( gmmIdToAddress( jsBuffer->bufferId ) + offset, data, size );
            }
            else
            {
                // partial buffer write
                //  STREAM and DYNAMIC buffers get transfer via a bounce buffer.
                //  STATIC and SYSTEM buffers issue a sync.
                switch ( bufferObject->usage )
                {
                    case GL_STREAM_DRAW:
                    case GL_STREAM_READ:
                    case GL_STREAM_COPY:
                    case GL_DYNAMIC_DRAW:
                    case GL_DYNAMIC_READ:
                    case GL_DYNAMIC_COPY:
                        // copy via bounce buffer
                        _jsGcmSend( jsBuffer->bufferId, offset, jsBuffer->pitch, ( const char * )data, size );
                        break;
                    default:	// STATIC and SYSTEM
                        // sync and copy direct
                        //  This is slow for host memory, but applications should be
                        //  mapping for performance.
                        _jsGcmFifoGlFinish();
                        fast_memcpy( gmmIdToAddress( jsBuffer->bufferId ) + offset, data, size );
                        break;
                };
            }
        }

    // be conservative here. Whenever we write to any Buffer Object, invalidate the vertex cache
    _jsGetGcmDriver()->invalidateVertexCache = GL_TRUE;
}

GLvoid _jsPlatformBufferObjectCopyData(
    jsBufferObject* bufferObjectDst,
    jsBufferObject* bufferObjectSrc )
{
    JS_ASSERT( bufferObjectDst != NULL );
    JS_ASSERT( bufferObjectSrc != NULL );

    jsGcmBufferObject* dst = ( jsGcmBufferObject* )bufferObjectDst->platformBufferObject;
    jsGcmBufferObject* src = ( jsGcmBufferObject* )bufferObjectSrc->platformBufferObject;
    JS_ASSERT( dst->bufferId != GMM_ERROR );
    JS_ASSERT( src->bufferId != GMM_ERROR );
    JS_ASSERT( dst->bufferSize >= src->bufferSize );

    // copy data
    //  There is currently no requirement to copy from one pool to another.
    JS_ASSERT( dst->pool == src->pool );
    switch ( dst->pool )
    {
        case _JSGCM_SURFACE_POOL_LINEAR:
            _jsGcmMemcpy( dst->bufferId, 0, dst->pitch, src->bufferId, 0, src->bufferSize );
            break;
        case _JSGCM_SURFACE_POOL_SYSTEM:
        case _JSGCM_SURFACE_POOL_SYSTEM_TILED_COLOR:
        case _JSGCM_SURFACE_POOL_SYSTEM_TILED_DEPTH:
            _jsGcmFifoGlFinish();
            fast_memcpy(
                gmmIdToAddress( dst->bufferId ),
                gmmIdToAddress( src->bufferId ),
                src->bufferSize );
            break;
    }

    // be conservative here. Whenever we write to any Buffer Object, invalidate the vertex cache
    _jsGetGcmDriver()->invalidateVertexCache = GL_TRUE;
}

char *_jsPlatformBufferObjectMap( jsBufferObject* bufferObject, GLenum access )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    if ( jsBuffer->mapCount++ == 0 )
    {
        if ( access == GL_WRITE_ONLY )
        {
            // replace entire buffer
            //  To avoid waiting for the GPU to finish using the buffer,
            //  just allocate a new one.
            _jsAllocateBuffer( bufferObject );
            if ( jsBuffer->pool == _JSGCM_SURFACE_POOL_NONE )
            {
                _jsSetError( GL_OUT_OF_MEMORY );
                return NULL;
            }
        }
        else
            // must wait in order to read
            _jsGcmFifoGlFinish();

        jsBuffer->mapAccess = access;

        // count writable mapped buffers
        //  If any buffers are left mapped when a draw is invoked, we must
        //  flush the vertex cache in case VBO data has been modified.
        if ( jsBuffer->mapAccess != GL_READ_ONLY )
        {
            jsGcmDriver *driver = _jsGetGcmDriver();
            ++driver->flushBufferCount;
        }

        // only need to pin the first time we map
        gmmPinId( jsBuffer->bufferId );
    }
    else
        JS_ASSERT( access == GL_READ_ONLY || jsBuffer->mapAccess == access );

    return gmmIdToAddress( jsBuffer->bufferId );
}

GLboolean _jsPlatformBufferObjectUnmap( jsBufferObject* bufferObject )
{
    // can't unmap if not mapped
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * )bufferObject->platformBufferObject;

    if ( --jsBuffer->mapCount == 0 )
    {
        // count writable mapped buffers
        //  If any buffers are left mapped when a draw is invoked, we must
        //  flush the vertex cache in case VBO data has been modified.
        if ( jsBuffer->mapAccess != GL_READ_ONLY )
        {
            jsGcmDriver *driver = _jsGetGcmDriver();
            --driver->flushBufferCount;

            // make sure we flush for the next draw
            driver->invalidateVertexCache = GL_TRUE;
        }

        jsBuffer->mapAccess = GL_NONE;

        gmmUnpinId( jsBuffer->bufferId );
    }

    return GL_TRUE;
}

void _jsPlatformGetBufferParameteriv( jsBufferObject *bufferObject, GLenum pname, int *params )
{
    jsGcmBufferObject *jsBuffer = ( jsGcmBufferObject * ) & bufferObject->platformBufferObject;
    switch ( pname )
    {
        case GL_BUFFER_PITCH_SCE:
            params[0] = jsBuffer->pitch;
            break;
        default:
            JS_ASSERT( 0 );
            return;
    }
}
