/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_POLICY_MODULE_H__
#define __CELL_SPURS_POLICY_MODULE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpursWorkloadId */
#include <cell/spurs/error.h>
#include <cell/spurs/policy_module_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* new entry with status */
void cellSpursModuleEntryStatus(uintptr_t, uint64_t arg, CellSpursModulePollStatus status) __attribute__((naked,noreturn));
void cellSpursModuleMainStatus(CellSpursModulePollStatus status, uint64_t arg);

/* for compatibility: no more APIs requiring context */
void cellSpursModuleEntry(uintptr_t context, uint64_t ea) __attribute__((naked,noreturn));
void cellSpursModuleMain(uintptr_t context, uint64_t ea);

void cellSpursModuleExit(void) __attribute__((__noreturn__));
unsigned cellSpursModulePoll(void);
unsigned cellSpursModulePollStatus(CellSpursModulePollStatus *status);
void _cellSpursModuleTrace(unsigned tag, uint64_t data);
void _cellSpursRequestIdleSpu(CellSpursWorkloadId, unsigned count);
int _cellSpursWorkloadFlagReceiver(CellSpursWorkloadId, unsigned set);
int _cellSpursWorkloadFlagReceiver2(CellSpursWorkloadId, unsigned set);
uint64_t _cellSpursGetWorkloadFlag(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*
 * for compatibility
 */
#define cellSpursModuleExit(context)\
		cellSpursModuleExit()

#define cellSpursModulePoll(context)\
		cellSpursModulePoll()

#define cellSpursModuleTrace(context,tag,data)\
		_cellSpursModuleTrace(tag,data)

/*
 * alias of SPURS common interfaces
 */
#include <cell/spurs/common.h>

#define cellSpursModuleGetSpuId(context)\
		cellSpursGetCurrentSpuId()

#define cellSpursModuleGetWorkloadId(context)\
		cellSpursGetWorkloadId()

/*
 * ready count interfaces for policy module
 */
#include <cell/spurs/ready_count.h>

#define cellSpursModuleReadyCountSwap(context, value)\
		_cellSpursReadyCountOperator(cellSpursGetWorkloadId(), _CELL_SPURS_READY_COUNT_SWAP, value, 0);

#define cellSpursModuleReadyCountCompareAndSwap(context, compare, swap)\
		_cellSpursReadyCountOperator(cellSpursGetWorkloadId(), _CELL_SPURS_READY_COUNT_CAS, compare, swap)

#define cellSpursModuleReadyCountAdd(context, value)\
		_cellSpursReadyCountOperator(cellSpursGetWorkloadId(), _CELL_SPURS_READY_COUNT_ADD, value, 0);

/*
 * idle SPU request operation for policy module
 */
#define cellSpursModuleRequestIdleSpu(context, count)\
		_cellSpursRequestIdleSpu(cellSpursGetWorkloadId(), count)

static inline
int cellSpursRequestIdleSpu(unsigned char *ls, uint64_t eaSpurs, CellSpursWorkloadId id, unsigned count)
{
	(void)ls;
	(void)eaSpurs;
	if (id >= CELL_SPURS_MAX_WORKLOAD || count > CELL_SPURS_MAX_SPU) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	_cellSpursRequestIdleSpu(id, count);
	return CELL_OK;
}

/*
 * workload flag
 */
static inline
int cellSpursModuleSetWorkloadFlagReceiver(void)
{
	return _cellSpursWorkloadFlagReceiver(cellSpursGetWorkloadId(), 1);
}

static inline
int cellSpursModuleUnsetWorkloadFlagReceiver(void)
{
	return _cellSpursWorkloadFlagReceiver(cellSpursGetWorkloadId(), 0);
}

static inline
int cellSpursModuleSetWorkloadFlagReceiver2(void)
{
	return _cellSpursWorkloadFlagReceiver2(cellSpursGetWorkloadId(), 1);
}

static inline
int cellSpursModuleUnsetWorkloadFlagReceiver2(void)
{
	return _cellSpursWorkloadFlagReceiver2(cellSpursGetWorkloadId(), 0);
}

static inline
int cellSpursGetWorkloadFlag(uint64_t *flag)
{
	if (flag == 0) {
		return CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER;
	}
	*flag = _cellSpursGetWorkloadFlag();
	return CELL_OK;
}

#endif /* __CELL_SPURS_POLICY_MODULE_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
