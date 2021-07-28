/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TASK_H__
#define __CELL_SPURS_TASK_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdio.h>

/* SPU C/C++ Language Extention */
#include <spu_intrinsics.h>

/* SPURS */
#include <cell/spurs/task_types.h>
#include <cell/spurs/types.h>			/* CellSpursWorkloadId */
#include <cell/spurs/error.h>
#include <cell/spurs/version.h>
#include <cell/spurs/workload.h>		/* cellSpursGetWorkloadData */
#include <cell/spurs/common.h>			/* functions moved to common.h */

#define CELL_SPURS_TASK_LS_ALL		((vec_uint4){CELL_SPURS_TASK_TOP_MASK,0xffffffffu,0xffffffffu,0xffffffffu})
#define CELL_SPURS_TASK_POLL_FOUND_TASK		1
#define CELL_SPURS_TASK_POLL_FOUND_WORKLOAD	2
#define	_CELL_SPURS_TASK_ATTRIBUTE_REVISION	(0x01)

typedef struct CellSpursTaskSaveConfig {
	uint64_t	eaContext;
	uint32_t	sizeContext;
	vec_uint4	lsPattern;
} CellSpursTaskSaveConfig;

/* for convenience */
static const CellSpursTaskLsPattern	gCellSpursTaskLsAll= {
	{
		CELL_SPURS_TASK_TOP_MASK,
		0xffffffffu,
		0xffffffffu,
		0xffffffffu
	}
};


#ifdef __cplusplus
extern "C" {
#endif

/*
 * taskset control
 */
int cellSpursShutdownTaskset(uint64_t eaTaskset);
static inline
int cellSpursLookUpTasksetAddress(uint64_t *taskset, CellSpursWorkloadId id);

/*
 * task control for SPURS task
 */
CellSpursTaskId cellSpursGetTaskId(void);
uint64_t        cellSpursGetTasksetAddress(void);
uint64_t        cellSpursGetElfAddress(void);
void            cellSpursMain(qword argTask, uint64_t argTaskset);
void            cellSpursExit(void) __attribute__((noreturn));
int             cellSpursYield(void);
int             cellSpursWaitSignal(void);
unsigned        cellSpursTaskPoll(void);
int             cellSpursTaskMain(qword argTask, uint64_t argTaskset);
void            cellSpursTaskExit(int) __attribute__((noreturn));
int             cellSpursGetTaskVolatileArea(void **ptr, uint32_t *size);
/*
 * task control for SPU common
 */
int	_cellSpursTaskAttributeInitialize (CellSpursTaskAttribute* pAttr, unsigned int revision, unsigned int sdkVersion,
									   uint64_t	eaElf, const CellSpursTaskSaveConfig *saveConfig, qword argument);
static inline
int	cellSpursTaskAttributeInitialize (CellSpursTaskAttribute* pAttr,  uint64_t eaElf,
									  const CellSpursTaskSaveConfig *saveConfig, qword argument);
int cellSpursTaskAttributeSetExitCodeContainer (CellSpursTaskAttribute* pAttr,	uint64_t exitCodeContainer);
int	cellSpursCreateTaskWithAttribute (uint64_t eaTaskset, CellSpursTaskId*, 	const CellSpursTaskAttribute*);
int	cellSpursCreateTask2 (uint64_t eaTaskset, CellSpursTaskId*, uint64_t eaElf,  qword argument,  const CellSpursTaskAttribute2*);
int	cellSpursCreateTask2WithBinInfo (uint64_t eaTaskset, CellSpursTaskId*, uint64_t eaTaskBinInfo, 
									 qword argument,  uint64_t eaContext, const char*name,void *__reserved__);

int cellSpursCreateTask(uint64_t eaTaskset,	CellSpursTaskId*,	uint64_t eaElf,
						uint64_t eaContext, 	uint32_t sizeContext,	vec_uint4 lsPattern,	qword  argument);
int cellSpursSendSignal(uint64_t eaTaskset, CellSpursTaskId);
int	cellSpursJoinTask2 (uint64_t eaTaskset,	CellSpursTaskId idTask, int* retval);
int	cellSpursTryJoinTask2 (uint64_t eaTaskset,	CellSpursTaskId idTask, int* retval);


void _cellSpursTaskAttribute2Initialize(CellSpursTaskAttribute2 *pAttr, uint32_t revision);

static inline void cellSpursTaskAttribute2Initialize(CellSpursTaskAttribute2 *pAttr)
{
	_cellSpursTaskAttribute2Initialize(pAttr, CELL_SPURS_TASK2_REVISION);
}



/*
 * LS context pattern
 */
vec_uint4 cellSpursContextGetLsPattern(void);
int cellSpursContextSetLsPattern(vec_uint4 ls_pattern);
static inline
vec_uint4 cellSpursContextGenerateLsPattern(intptr_t start, int size);
int cellSpursTaskGetReadOnlyAreaPattern(vec_uint4 *lspattern, uint64_t ea_elf, void *buf, uint32_t tag);
int cellSpursTaskGetLoadableSegmentPattern(vec_uint4 *lspattern, uint64_t ea_elf, void *buf, uint32_t tag);
int cellSpursTaskGenerateLsPattern(vec_uint4 *pattern, uint32_t start, uint32_t size);
int cellSpursTaskGetContextSaveAreaSize(uint32_t *size, vec_uint4 lsPattern);



/*
 * Workload Flag
 */ 
int cellSpursTaskReceiveWorkloadFlag(void);


#ifdef __cplusplus
} /* extern "C" */
#endif


/* inline functions */
static inline
int cellSpursLookUpTasksetAddress(uint64_t *taskset, CellSpursWorkloadId id)
{
	if (taskset == 0) {
		return CELL_SPURS_TASK_ERROR_NULL_POINTER;
	}
	if (id >= CELL_SPURS_MAX_WORKLOAD) {
		return CELL_SPURS_TASK_ERROR_INVAL;
	}
	*taskset = _cellSpursGetWorkloadData(id);
	return CELL_OK;
}

static inline
int	cellSpursTaskAttributeInitialize (
						CellSpursTaskAttribute* pAttr,
						uint64_t	eaElf,
						const CellSpursTaskSaveConfig *saveConfig,
						qword		argument)
{
	int	ret = _cellSpursTaskAttributeInitialize(
						pAttr,
						_CELL_SPURS_TASK_ATTRIBUTE_REVISION,
						_CELL_SPURS_INTERNAL_VERSION,
						eaElf,
						saveConfig,
						argument);
	return	ret;
}

static inline
vec_uint4 cellSpursContextGenerateLsPattern(intptr_t start, int size)
{
	int mbegin = CELL_SPURS_CONTEXT_SIZE2BITS(start);
	int mend   = CELL_SPURS_CONTEXT_SIZE2BITS(start + size + 2047);
	vec_uint4 mask_base  = (vec_uint4){-1, -1, -1, -1};
	vec_uint4 mask_begin = spu_rlmaskqw(spu_rlmaskqwbytebc(mask_base,7-mbegin),-mbegin);
	vec_uint4 mask_end   = spu_slqw(spu_slqwbytebc(mask_base,128-mend),128-mend);
	return spu_and(mask_begin, mask_end);
}



#ifdef __cplusplus
#include <cell/spurs/task_types_cpp.h>
#endif /*__cplusplus*/


#endif /* __CELL_SPURS_TASK_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
