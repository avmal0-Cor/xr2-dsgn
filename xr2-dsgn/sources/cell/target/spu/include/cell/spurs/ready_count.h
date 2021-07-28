/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_READY_COUNT_H__
#define __CELL_SPURS_READY_COUNT_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	_CELL_SPURS_READY_COUNT_SWAP,
	_CELL_SPURS_READY_COUNT_CAS,
	_CELL_SPURS_READY_COUNT_ADD
} _CellSpursReadyCountOpcode;

unsigned _cellSpursReadyCountOperator(CellSpursWorkloadId id, _CellSpursReadyCountOpcode opcode, unsigned value1, unsigned value2);
unsigned _cellSpursReadyCountSwap(CellSpursWorkloadId id, unsigned value);
unsigned _cellSpursReadyCountCompareAndSwap(CellSpursWorkloadId id, unsigned compare, unsigned swap);
unsigned _cellSpursReadyCountAdd(CellSpursWorkloadId id, int value);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*
 * for source code compatibility
 */
static inline
int cellSpursReadyCountSwap(
	unsigned char *ls, uint64_t eaSpurs,
	CellSpursWorkloadId id, unsigned *old, unsigned value)
{
	(void)ls;
	(void)eaSpurs;
	if (old == 0) {
		return CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER;
	}
	if (id >= CELL_SPURS_MAX_WORKLOAD || value >= 256) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	*old = _cellSpursReadyCountOperator(id, _CELL_SPURS_READY_COUNT_SWAP, value, 0);
	return CELL_OK;
}

static inline
int cellSpursReadyCountCompareAndSwap(
	unsigned char *ls, uint64_t eaSpurs,
	CellSpursWorkloadId id, unsigned *old, unsigned compare, unsigned swap)
{
	(void)ls;
	(void)eaSpurs;
	if (old == 0) {
		return CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER;
	}
	if (id >= CELL_SPURS_MAX_WORKLOAD || compare >= 256 || swap >= 256) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	*old = _cellSpursReadyCountOperator(id, _CELL_SPURS_READY_COUNT_CAS, compare, swap);
	return CELL_OK;
}

static inline
int cellSpursReadyCountAdd(
	unsigned char *ls, uint64_t eaSpurs,
	CellSpursWorkloadId id, unsigned *old, int value)
{
	(void)ls;
	(void)eaSpurs;
	if (old == 0) {
		return CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER;
	}
	if (id >= CELL_SPURS_MAX_WORKLOAD) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	*old = _cellSpursReadyCountOperator(id, _CELL_SPURS_READY_COUNT_ADD, value, 0);
	return CELL_OK;
}

static inline
int cellSpursReadyCountStore(
	unsigned char *ls, uint64_t eaSpurs,
	CellSpursWorkloadId id, unsigned value)
{
	(void)ls;
	(void)eaSpurs;
	if (id >= CELL_SPURS_MAX_WORKLOAD || value >= 256) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	(void)_cellSpursReadyCountOperator(id, _CELL_SPURS_READY_COUNT_SWAP, value, 0);
	return CELL_OK;
}

#endif /* __CELL_SPURS_READY_COUNT_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
