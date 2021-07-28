/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * 
 */

#ifndef _PLATFORM_DEVICE_INIT_H_
#define _PLATFORM_DEVICE_INIT_H_

#include <JSGCM/JSGcmResource.h>

int _jsGcmInitRM( jsGcmResource *rmResource, unsigned int hostMemorySize, int inSysMem, unsigned int dmaPushBufferSize );
void _jsGcmDestroyRM( jsGcmResource *rmResource );

int _jsGcmAllocateColorSurfaceVideoMemory( GLuint width, GLuint height, GLuint bytesPerPixel, GLuint scanoutSupported, GLuint antiAliasing, void **adr, GLuint *pitchAllocated, GLuint *bytesAllocated );
void _jsGcmFreeColorSurfaceVideoMemory( void *adr );

int _jsGcmAllocateDepthStencilSurfaceVideoMemory( GLuint width, GLuint height, GLuint antiAliasing, void **adr, GLuint *pitchAllocated, GLuint *bytesAllocated );
void _jsGcmFreeDepthStencilSurfaceVideoMemory( void *adr );

int _jsGcmAllocateTextureVideoMemory( GLuint mipmaps, GLuint width, GLuint height, GLuint depth, GLuint bytesPerPixel, GLuint swizzled, GLuint cubemap, void **adr, GLuint *bytesPerFace, GLuint *bytesAllocated );
void _jsGcmFreeTextureVideoMemory( void *adr );

int _jsGcmAllocateLinearVideoMemory( GLuint sizeInBytes, void **adr, GLuint *bytesAllocated );
void _jsGcmFreeLinearVideoMemory( void *adr );


void _jsGcmTiledMemoryInit( void );
void _jsGcmTiledMemoryFinish( void );

#define _JSGCM_TILED_BUFFER_ALIGNMENT 0x10000 // 64KB
#define _JSGCM_TILED_BUFFER_HEIGHT_ALIGNMENT 64

#define _JSGCM_ZCULL_BUFFER_SIZE 0x300000
#define _JSGCM_ZCULL_BUFFER_ALIGNMENT 0x1000 // 4KB

#define _JSGCM_MAX_TILED_REGIONS 15
#define _JSGCM_MAX_ZCULL_REGIONS 8

#define _JSGCM_CTAG_CORRESPONDING_SIZE 0x10000 // each byte in Tag RAM corresponds to 64KB of tile memory
#define _JSGCM_CTAG_CORRESPONDING_TOTAL_SIZE 0x8000000 // 128MB, maximum total area that compression can be set 

#define _JSGCM_HAS_INVALIDATE_TILE
#define cellGcmInitExpectedReturn 0

#ifndef _JSGCM_HAS_INVALIDATE_TILE
#define _JSGCM_RESERVED_VIDEO_MEMORY (1 << 16)
#else
#define _JSGCM_RESERVED_VIDEO_MEMORY (0)
#endif



#endif // !defined(_PLATFORM_DEVICE_INIT_H_)
