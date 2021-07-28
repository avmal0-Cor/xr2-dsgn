/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_WORKLOAD_H__
#define __CELL_SPURS_WORKLOAD_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpursWorkloadId */
#include <cell/spurs/error.h>
#include <cell/spurs/workload_types.h>

#ifdef __cplusplus
extern "C" {
#endif

void _cellSpursSendWorkloadSignal(CellSpursWorkloadId);
int _cellSpursShutdownWorkload(CellSpursWorkloadId);
uint64_t _cellSpursGetWorkloadData(CellSpursWorkloadId);

#ifdef __cplusplus
} /* extern "C" */
#endif

static inline
int cellSpursSendWorkloadSignal(unsigned char *ls, uint64_t eaSpurs, CellSpursWorkloadId id)
{
	(void)ls;
	(void)eaSpurs;
	if (id >= CELL_SPURS_MAX_WORKLOAD) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	_cellSpursSendWorkloadSignal(id);
	return CELL_OK;
}

static inline
int cellSpursShutdownWorkload(uint64_t eaSpurs, CellSpursWorkloadId id)
{
	(void)eaSpurs;
	if (id >= CELL_SPURS_MAX_WORKLOAD) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	return _cellSpursShutdownWorkload(id);
}

static inline
int cellSpursGetWorkloadData(uint64_t *data, CellSpursWorkloadId id)
{
	if (data == 0) {
		return CELL_SPURS_POLICY_MODULE_ERROR_NULL_POINTER;
	}
	if (id >= CELL_SPURS_MAX_WORKLOAD) {
		return CELL_SPURS_POLICY_MODULE_ERROR_INVAL;
	}
	*data = _cellSpursGetWorkloadData(id);
	return CELL_OK;
}

#endif /* __CELL_SPURS_WORKLOAD_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
