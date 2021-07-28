/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
#ifndef __CELL_FIBER_SPU_CONTEXT_H__
#define __CELL_FIBER_SPU_CONTEXT_H__	1

#include <stddef.h> /* size_t */
#include <cell/fiber/spu_context_types.h>
#include <cell/fiber/error.h>
#include <spu_intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif
int cellFiberSpuContextInitialize(
		CellFiberSpuContext* fiber,
		CellFiberSpuContextEntry entry,
		uint64_t arg,
		void* stack,
		size_t stackSize
	);

int cellFiberSpuContextRun(
		CellFiberSpuContext	*fiber,
		CellFiberSpuContext *callerContext
	);

CellFiberSpuContext* cellFiberSpuContextSelf(void);

void _cellFiberSpuContextSwitch(CellFiberSpuContext* context);

__attribute__((always_inline))
static inline 
int cellFiberSpuContextSwitch(CellFiberSpuContext* context)
{
#ifndef CELL_FIBER_SPU_CONTEXT_SWITCH_NO_ERROR_CHECK	
	if (__builtin_expect(context == NULL, 0)) {
		return CELL_FIBER_ERROR_NULL_POINTER;
	}
	if (__builtin_expect(((uintptr_t)context & 15) != 0, 0)) {
		return CELL_FIBER_ERROR_ALIGN;
	}
	if (__builtin_expect(cellFiberSpuContextSelf() == NULL, 0)) {
		return CELL_FIBER_ERROR_PERM;
	}
#endif
	_cellFiberSpuContextSwitch(context);
	return CELL_OK;
}

#ifdef __cplusplus
}
#endif

#endif
