/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TASK_H__
#define __CELL_SPURS_TASK_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/task_types.h>

#include <cell/spurs/types.h>			/* CellSpurs */
#include <cell/spurs/error.h>
#include <cell/spurs/version.h>
#include <cell/spurs/exception_types.h>
#include <cell/spurs/task_exit_code_types.h>

#define	_CELL_SPURS_TASKSET_ATTRIBUTE_REVISION	(0x01)

typedef struct CellSpursTaskSaveConfig {
	void *eaContext;
	uint32_t sizeContext;
	const CellSpursTaskLsPattern *lsPattern;
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

#define	_CELL_SPURS_TASK_ATTRIBUTE_REVISION	(0x01)


#ifdef __cplusplus
extern "C" {
#endif


/*
 *  SPURS taskset
 */
int	_cellSpursTasksetAttributeInitialize (CellSpursTasksetAttribute* pAttr, unsigned int revision,
										  unsigned int sdkVersion,  uint64_t argTaskset, const uint8_t priority[8],
										  unsigned int max_contention);
static inline
int	cellSpursTasksetAttributeInitialize (CellSpursTasksetAttribute* pAttr,	uint64_t argTaskset,
										 const uint8_t priority[8],	unsigned int max_contention);
int	cellSpursTasksetAttributeSetName(CellSpursTasksetAttribute* attr, const char* name);
int	cellSpursTasksetAttributeSetTasksetSize(CellSpursTasksetAttribute* attr,size_t size);
int cellSpursTasksetAttributeEnableClearLS(CellSpursTasksetAttribute* pAttr,	int enable);
int	cellSpursCreateTasksetWithAttribute (CellSpurs*, CellSpursTaskset*,	const CellSpursTasksetAttribute*);
int cellSpursCreateTaskset(	CellSpurs*, CellSpursTaskset*, uint64_t argTaskset, 	const uint8_t priority[8], unsigned int max_contention);
int cellSpursShutdownTaskset(CellSpursTaskset*);
int cellSpursJoinTaskset(CellSpursTaskset*);
int cellSpursLookUpTasksetAddress(CellSpurs *spurs, CellSpursTaskset **taskset, CellSpursWorkloadId id);
int cellSpursGetTasksetId(const CellSpursTaskset*,CellSpursWorkloadId*);
int cellSpursTasksetGetSpursAddress(const CellSpursTaskset*, CellSpurs** ppSpurs);
int cellSpursCreateTaskset2(CellSpurs* pSpurs, CellSpursTaskset2* pTaskset, const CellSpursTasksetAttribute2* pAttr);
int cellSpursDestroyTaskset2(CellSpursTaskset2 *pTaskset);


/*
 * SPURS task
 */


int	_cellSpursTaskAttributeInitialize (CellSpursTaskAttribute *pAttr,	unsigned int revision,	unsigned int sdkVersion,
									   const void *eaElf,	   const CellSpursTaskSaveConfig *saveConfig, const CellSpursTaskArgument*);
static inline
int	cellSpursTaskAttributeInitialize (CellSpursTaskAttribute *pAttr, const void *eaElf,	const CellSpursTaskSaveConfig *saveConfig,
									  const CellSpursTaskArgument *argument);

void _cellSpursTaskAttribute2Initialize(CellSpursTaskAttribute2 *pAttr, uint32_t revision);

static inline void cellSpursTaskAttribute2Initialize(CellSpursTaskAttribute2 *pAttr)
{
	_cellSpursTaskAttribute2Initialize(pAttr, CELL_SPURS_TASK2_REVISION);
}

void _cellSpursTasksetAttribute2Initialize(CellSpursTasksetAttribute2 *pAttr, uint32_t revision);
static inline void cellSpursTasksetAttribute2Initialize(CellSpursTasksetAttribute2 *pAttr)
{
	_cellSpursTasksetAttribute2Initialize(pAttr, CELL_SPURS_TASK2_REVISION);
}

int cellSpursTaskAttributeSetExitCodeContainer (CellSpursTaskAttribute *pAttr,	CellSpursTaskExitCode *exitCodeContainer);
int	cellSpursCreateTaskWithAttribute (CellSpursTaskset*,	CellSpursTaskId*,	const CellSpursTaskAttribute*);
int cellSpursCreateTask(CellSpursTaskset*,		CellSpursTaskId*,		const void *eaElf,			const void *eaContext,
						uint32_t sizeContext,		const CellSpursTaskLsPattern*,		const CellSpursTaskArgument*);
int _cellSpursSendSignal(CellSpursTaskset *, CellSpursTaskId);
static inline
int cellSpursSendSignal(CellSpursTaskset *taskset, CellSpursTaskId id);
int	cellSpursCreateTask2 (CellSpursTaskset2* taskset,		  CellSpursTaskId* id,
						  const void *eaElf,	 const CellSpursTaskArgument *argument, const CellSpursTaskAttribute2*);
int	cellSpursCreateTask2WithBinInfo (CellSpursTaskset2* taskset,		  CellSpursTaskId* id,
									 const CellSpursTaskBinInfo *binInfo,	 const CellSpursTaskArgument *argument, void* contextBuffer, const char* name,
									 void *__reserved__);
int cellSpursJoinTask2(CellSpursTaskset2* pTaskset,	   CellSpursTaskId idTask,		   int *exitCode);
int cellSpursTryJoinTask2(CellSpursTaskset2* pTaskset,	   CellSpursTaskId idTask,		   int *exitCode);

/*
 *  Exception handler
 */
int cellSpursTasksetSetExceptionEventHandler(CellSpursTaskset* taskset,	CellSpursTasksetExceptionEventHandler handler,	void* arg);
int cellSpursTasksetUnsetExceptionEventHandler( CellSpursTaskset* taskset );

/*
 *  Task infomation
 */
int cellSpursGetTasksetInfo( const CellSpursTaskset*, CellSpursTasksetInfo* );
int cellSpursTaskGenerateLsPattern(CellSpursTaskLsPattern *pattern, uint32_t start, uint32_t size);
int cellSpursTaskGetReadOnlyAreaPattern(CellSpursTaskLsPattern *pattern, const void *elf);
int cellSpursTaskGetLoadableSegmentPattern(CellSpursTaskLsPattern *pattern, const void *elf);
int cellSpursTaskGetContextSaveAreaSize(uint32_t *size, const CellSpursTaskLsPattern *lspattern);



#ifdef __cplusplus
} /* extern "C" */
#endif

/* inline functions */
static inline
int	cellSpursTasksetAttributeInitialize (CellSpursTasksetAttribute* pAttr,	uint64_t argTaskset,
										 const uint8_t priority[8],	unsigned int max_contention)
{
	int	ret = _cellSpursTasksetAttributeInitialize(pAttr,
												   _CELL_SPURS_TASKSET_ATTRIBUTE_REVISION,
												   _CELL_SPURS_INTERNAL_VERSION,
												   argTaskset,
												   priority,
												   max_contention);
	return	ret;
}


static inline
int	cellSpursTaskAttributeInitialize (CellSpursTaskAttribute *pAttr, const void *eaElf,	const CellSpursTaskSaveConfig *saveConfig,
									  const CellSpursTaskArgument *argument)
{
	int	ret = _cellSpursTaskAttributeInitialize(pAttr,
												_CELL_SPURS_TASK_ATTRIBUTE_REVISION,
												_CELL_SPURS_INTERNAL_VERSION,
												eaElf,
												saveConfig,
												argument);
	return	ret;
}


static inline
int cellSpursSendSignal(CellSpursTaskset *taskset, CellSpursTaskId id) {
	return _cellSpursSendSignal(taskset, id);
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
