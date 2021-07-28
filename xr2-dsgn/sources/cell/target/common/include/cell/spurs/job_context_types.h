/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOB_CONTEXT_TYPES_H__
#define __CELL_SPURS_JOB_CONTEXT_TYPES_H__

#include <stdint.h>
#include <cell/spurs/job_descriptor.h>


typedef struct CellSpursJobContext2
{
	/// input/inout buffer
	void *ioBuffer;

	/// read-only cache buffer(s)
	void *cacheBuffer[4];

	uint32_t sizeJobDescriptor:4; /// 0:64bytes,otherwise sizeJobDescriptor*128 bytes
	uint32_t pad:28;

	/// number of io buffers
	uint16_t numIoBuffer;

	/// number of cache input buffers
	uint16_t numCacheBuffer;

	/// Output buffer
	void* oBuffer;

	/// Scratch buffer
	void* sBuffer;

	/// DMA tag for this job's I/O dmas. This will be one of: {20,21}.
	/// This value identifies a single job.
	unsigned int dmaTag;

	/// Job descriptor Effective Address
	uint64_t eaJobDescriptor;

} CellSpursJobContext2;

typedef struct CellSpursJobInputDataElement
{
	uint32_t size;
	void    *pointer;
} CellSpursJobInputDataElement;


#endif /* __CELL_SPURS_JOB_CONTEXT_TYPES_H__ */
