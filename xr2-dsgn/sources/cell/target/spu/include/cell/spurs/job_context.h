/* SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   * Copyright (C) 2008 Sony Computer Entertainment Inc.
   * All Rights Reserved.
   */

#ifndef __CELL_SPURS_JOB_CONTEXT_H__
#define __CELL_SPURS_JOB_CONTEXT_H__

#include <stdint.h>
#include <cell/spurs/job_context_types.h>
#include <cell/spurs/job_descriptor.h>

#ifdef __cplusplus
extern "C" {
#endif

	static inline
	int cellSpursJobGetPointerList(void *ptrList[], const CellSpursJobHeader *jobHeader, const CellSpursJobContext2 *jobContext) {
		if (__builtin_expect(ptrList    == 0 ||
							 jobHeader  == 0 ||
							 jobContext == 0, 0))
			return CELL_SPURS_JOB_ERROR_NULL_POINTER;
		if (__builtin_expect((uintptr_t)ptrList    % 4 != 0 ||
							 (uintptr_t)jobHeader  % 8 != 0 ||
							 (uintptr_t)jobContext % 8 != 0, 0))
			return CELL_SPURS_JOB_ERROR_ALIGN;
		int numIoPtrs = jobHeader->sizeDmaList / sizeof(uint64_t);
		uintptr_t ioBuffer = (uintptr_t)jobContext->ioBuffer;
		uintptr_t dmaList  = (uintptr_t)jobHeader + sizeof(CellSpursJobHeader);
		for(int i = 0; i < numIoPtrs; i++) {
			uint32_t eal = ((uint64_t *)dmaList)[i];
			ptrList[i] = (void *)((uintptr_t)ioBuffer|(eal&0xf));
			ioBuffer += (((((uint64_t *)dmaList)[i] >> 32) & 0x7fff) + 0xf) & ~0xf;
		}

		return CELL_OK;
	}

	static inline
	int cellSpursJobGetInputDataElements(CellSpursJobInputDataElement *elemList, const CellSpursJobHeader *jobHeader, const CellSpursJobContext2 *jobContext) {
		if (__builtin_expect(elemList    == 0 ||
							 jobHeader  == 0 ||
							 jobContext == 0, 0))
			return CELL_SPURS_JOB_ERROR_NULL_POINTER;
		if (__builtin_expect((uintptr_t)elemList    % 4 != 0 ||
							 (uintptr_t)jobHeader  % 8 != 0 ||
							 (uintptr_t)jobContext % 8 != 0, 0))
			return CELL_SPURS_JOB_ERROR_ALIGN;
		int numIoPtrs = jobHeader->sizeDmaList / sizeof(uint64_t);
		uintptr_t ioBuffer = (uintptr_t)jobContext->ioBuffer;
		uintptr_t dmaList  = (uintptr_t)jobHeader + sizeof(CellSpursJobHeader);
		for(int i = 0; i < numIoPtrs; i++) {
			uint32_t size = (((uint64_t *)dmaList)[i] >> 32) & 0x7fff;
			uint32_t eal = ((uint64_t *)dmaList)[i];
			elemList[i].size    = size;
			elemList[i].pointer = (void *)((uintptr_t)ioBuffer|(eal&0xf));
			ioBuffer += ((size + 0xf) & ~0xf);
		}

		return CELL_OK;
	}

#ifdef __cplusplus
}
#endif


#endif /* __CELL_SPURS_JOB_CONTEXT_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
