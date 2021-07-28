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
#include <cell/spurs/types.h>			/* CellSpurs, CellSpursWorkloadId */
#include <cell/spurs/error.h>
#include <cell/spurs/workload_types.h>
#include <cell/spurs/version.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	_CELL_SPURS_WORKLOAD_ATTRIBUTE_REVISION	(0x01)

int	_cellSpursWorkloadAttributeInitialize (CellSpursWorkloadAttribute*, unsigned int revision, unsigned int sdkVersion, const void *pm, unsigned size, uint64_t data, const uint8_t priorityTable[8], unsigned int minContention, unsigned int maxContention);

int _cellSpursWorkloadFlagReceiver(CellSpurs*, CellSpursWorkloadId, unsigned set);
int _cellSpursWorkloadFlagReceiver2(CellSpurs *spurs_, CellSpursWorkloadId id, unsigned set, unsigned isClear);

static inline
int	cellSpursWorkloadAttributeInitialize (CellSpursWorkloadAttribute* attr, const void *pm, unsigned size, uint64_t data, const uint8_t priorityTable[8], unsigned int minContention, unsigned int maxContention)
{
	int	ret = _cellSpursWorkloadAttributeInitialize(attr, _CELL_SPURS_WORKLOAD_ATTRIBUTE_REVISION, _CELL_SPURS_INTERNAL_VERSION, pm, size, data, priorityTable, minContention, maxContention);
	return	ret;
}

int	cellSpursWorkloadAttributeSetName(CellSpursWorkloadAttribute* attr, const char* nameClass, const char* nameInstance);

int	cellSpursAddWorkloadWithAttribute (CellSpurs*, CellSpursWorkloadId*, const CellSpursWorkloadAttribute*);

int cellSpursAddWorkload(CellSpurs*, CellSpursWorkloadId*, const void *pm, unsigned size, uint64_t data, const uint8_t priorityTable[8], unsigned int minContention, unsigned int maxContention);

int cellSpursShutdownWorkload(CellSpurs*, CellSpursWorkloadId);
int cellSpursWaitForWorkloadShutdown(CellSpurs*, CellSpursWorkloadId);
int cellSpursRemoveWorkload(CellSpurs*, CellSpursWorkloadId);

int cellSpursSendWorkloadSignal(CellSpurs*, CellSpursWorkloadId);
int cellSpursGetWorkloadData(const CellSpurs*, uint64_t *data, CellSpursWorkloadId);

int cellSpursGetWorkloadFlag(const CellSpurs*, CellSpursWorkloadFlag**);

static inline
int cellSpursSetWorkloadFlagReceiver(CellSpurs *spurs, CellSpursWorkloadId id)
{
	return _cellSpursWorkloadFlagReceiver(spurs, id, 1);
}

static inline
int cellSpursUnsetWorkloadFlagReceiver(CellSpurs *spurs, CellSpursWorkloadId id)
{
	return _cellSpursWorkloadFlagReceiver(spurs, id, 0);
}

static inline
int cellSpursSetWorkloadFlagReceiver2(CellSpurs *spurs, CellSpursWorkloadId id)
{
	return _cellSpursWorkloadFlagReceiver2(spurs, id, 1, 0);
}

static inline
int cellSpursUnsetWorkloadFlagReceiver2(CellSpurs *spurs, CellSpursWorkloadId id)
{
	return _cellSpursWorkloadFlagReceiver2(spurs, id, 0, 0);
}

int cellSpursWorkloadAttributeSetShutdownCompletionEventHook(
		CellSpursWorkloadAttribute* attr,
		CellSpursShutdownCompletionEventHook hook,
		void* arg
	);

int cellSpursGetWorkloadInfo( const CellSpurs*, CellSpursWorkloadId, CellSpursWorkloadInfo* );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_WORKLOAD_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
