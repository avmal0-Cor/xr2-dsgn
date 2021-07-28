/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */
#ifndef _SPU_PSGL_h
#define _SPU_PSGL_h

#include <stdlib.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSGL_READ_TAG 30		// DMA tag used by psglSPUReadMappedBuffer
#define PSGL_WRITE_TAG 31		// DMA tag used by psglSPUWriteMappedBuffer

void psglSPUInit(unsigned long long initAddr);
void psglSPUWriteMappedBuffer(unsigned long long mappedAddress,const void* localAddress,size_t size);
void psglSPUReadMappedBuffer(unsigned long long mappedAddress,void* localAddress,size_t size);
void glSetMappedEventSCE(unsigned long long event);
void glSetMappedEventWithAddressTagSCE(unsigned long long event, unsigned int *zeroBuffer, unsigned int tag);

// function to fill command buffer hole from SPU
int psglFillCommandBufferHole(GLenum mode, GLsizei count, GLenum type, 
							  GLboolean isIndexMainMemory, uint32_t indexOffset, 
							  uint32_t holeSizeInWord, uint32_t* localHoleBuffer);

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif // _SPU_PSGL_h
