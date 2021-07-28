/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_BARRIER_H__
#define __CELL_SPURS_BARRIER_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/barrier_types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursBarrierInitialize(uint64_t ea, unsigned int total);
int _cellSpursBarrierNotify(uint64_t ea, unsigned isBlocking);
int _cellSpursBarrierWait(uint64_t ea, unsigned isBlocking);
int cellSpursBarrierGetTasksetAddress(uint64_t ea, uint64_t* pEaTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

#define cellSpursBarrierTryNotify(ea) \
		_cellSpursBarrierNotify((ea), 0)
#define cellSpursBarrierNotify(ea) \
		_cellSpursBarrierNotify((ea), 1)

#define cellSpursBarrierTryWait(ea) \
		_cellSpursBarrierWait((ea), 0)
#define cellSpursBarrierWait(ea) \
		_cellSpursBarrierWait((ea), 1)

#endif /* __CELL_SPURS_BARRIER_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
