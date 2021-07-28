/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_CONTROL_H__
#define __CELL_SPURS_CONTROL_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>

/* Lv2 */
#include <sys/types.h>
#include <sys/spu_thread.h>				/* sys_spu_thread_t */
#include <sys/spu_thread_group.h>		/* SYS_SPU_THREAD_GROUP_TYPE_* */
#include <sys/ppu_thread.h>				/* sys_ppu_thread_t */

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpurs */
#include <cell/spurs/error.h>
#include <cell/spurs/version.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	_CELL_SPURS_ATTRIBUTE_REVISION	(0x02)

int	_cellSpursAttributeInitialize (CellSpursAttribute*,
									unsigned int revision,
									unsigned int sdkVersion,
									unsigned nSpus,
									int spuPriority,
									int ppuPriority,
									bool exitIfNoWork);

static inline
int cellSpursAttributeInitialize(CellSpursAttribute* attr,
								unsigned nSpus,
								int spuPriority,
								int ppuPriority,
								bool exitIfNoWork)
{
	int	ret = _cellSpursAttributeInitialize(attr,
								_CELL_SPURS_ATTRIBUTE_REVISION,
								_CELL_SPURS_INTERNAL_VERSION,
								nSpus,
								spuPriority,
								ppuPriority,
								exitIfNoWork);
	return	ret;
}

int	cellSpursAttributeSetNamePrefix(CellSpursAttribute* attr,
								const char* name,
								size_t size);

int	cellSpursAttributeSetMemoryContainerForSpuThread(
								CellSpursAttribute* attr,
								sys_memory_container_t container);

int cellSpursAttributeSetSpuThreadGroupType(CellSpursAttribute* attr, int type);

int cellSpursAttributeEnableSpuPrintfIfAvailable(CellSpursAttribute* attr);

int	cellSpursInitializeWithAttribute (CellSpurs*, const CellSpursAttribute*);

int cellSpursInitialize(CellSpurs* spurs,
						unsigned nSpus,
						int spuPriority,
						int ppuPriority,
						bool exitIfNoWork);
int cellSpursFinalize(CellSpurs*);
int cellSpursWakeUp(CellSpurs*);
int cellSpursGetNumSpuThread(CellSpurs*, unsigned *nThreads);

int cellSpursSetMaxContention(CellSpurs*,
						CellSpursWorkloadId,
						unsigned int maxContention);
int cellSpursSetPriorities(CellSpurs*,
						CellSpursWorkloadId,
						const uint8_t priorities[CELL_SPURS_MAX_SPU]);

int cellSpursGetSpuThreadId(CellSpurs*, sys_spu_thread_t*, unsigned *nThreads);
int cellSpursGetSpuThreadGroupId(CellSpurs*, sys_spu_thread_group_t*);

int cellSpursGetInfo(CellSpurs* spurs, CellSpursInfo *info);
int cellSpursGetSpuGuid(const void* pSpuGUID, uint64_t *dest);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_CONTROL_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
