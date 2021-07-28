/* SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   * Copyright (C) 2008 Sony Computer Entertainment Inc.
   * All Rights Reserved.
   */

#ifndef __CELL_SPURS_JOB_CHAIN_H__
#define __CELL_SPURS_JOB_CHAIN_H__

#include <stdint.h>
#include <cell/spurs/job_chain_types.h>
#include <cell/spurs/error.h>
#include <cell/spurs/version.h>
#include <cell/spurs/control.h>
#include <cell/spurs/exception_types.h>
#include <cell/spurs/job_descriptor.h>
#include <cell/spurs/job_commands.h>
#include <cell/spurs/job_guard.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Job Chain
 */

	int _cellSpursJobChainAttributeInitialize(unsigned int jmRevsion,
											  unsigned int sdkRevision,
											  CellSpursJobChainAttribute *attr,
											  const uint64_t *jobChainEntry,
											  uint16_t sizeJobDescriptor,
											  uint16_t maxGrabdedJob,
											  const uint8_t priorityTable[8],
											  unsigned int maxContention,
											  bool autoRequestSpuCount,
											  unsigned int tag1,
											  unsigned int tag2,
											  bool isFixedMemAlloc,
											  unsigned int maxSizeJobDescriptor,
											  unsigned int initialRequestSpuCount);

	static inline
	int cellSpursJobChainAttributeInitialize(CellSpursJobChainAttribute *attr,
											 const uint64_t *jobChainEntry,
											 uint16_t sizeJobDescriptor,
											 uint16_t maxGrabdedJob,
											 const uint8_t priorityTable[8],
											 unsigned int maxContention,
											 bool autoRequestSpuCount,
											 unsigned int tag1,
											 unsigned int tag2,
											 bool isFixedMemAlloc,
											 unsigned int maxSizeJobDescriptor,
											 unsigned int initialRequestSpuCount)
	{
		int ret = _cellSpursJobChainAttributeInitialize(CELL_SPURS_JOB_REVISION,
														_CELL_SPURS_INTERNAL_VERSION,
														attr,
														jobChainEntry,
														sizeJobDescriptor,
														maxGrabdedJob,
														priorityTable,
														maxContention,
														autoRequestSpuCount,
														tag1,
														tag2,
														isFixedMemAlloc,
														maxSizeJobDescriptor,
														initialRequestSpuCount);
		return ret;
	}
	int cellSpursJobChainAttributeSetName(CellSpursJobChainAttribute* attr, const char* name);
	int cellSpursJobChainAttributeSetHaltOnError(CellSpursJobChainAttribute* attr);
	int cellSpursJobChainAttributeSetJobTypeMemoryCheck(CellSpursJobChainAttribute* attr);

	int cellSpursCreateJobChainWithAttribute(CellSpurs *spurs,
											 CellSpursJobChain *jobChain,
											 const CellSpursJobChainAttribute *attr);

	int cellSpursShutdownJobChain(const CellSpursJobChain* jobChain);

	/* This works with job chain created by cellSpursCreateJobChainWithAttribute() */
	int cellSpursRunJobChain(const CellSpursJobChain* jobChain);

	int cellSpursJoinJobChain(CellSpursJobChain *jobChain_);
	int cellSpursGetJobChainId(const CellSpursJobChain*, CellSpursWorkloadId*);
	int cellSpursJobChainGetError(CellSpursJobChain *jobChain_, void** cause);

	int cellSpursJobChainGetSpursAddress(const CellSpursJobChain* jobChain, CellSpurs** ppSpurs);
	int cellSpursJobSetMaxGrab(CellSpursJobChain* jobChain, unsigned int maxGrab);

/*
 *  Urgent Call Registration
 */
	int cellSpursAddUrgentCall(CellSpursJobChain* jobChain,
							   uint64_t *commandList);
	int cellSpursAddUrgentCommand(CellSpursJobChain* jobChain,
								  uint64_t command);

/*
 *  SPURS Job Guard
 */
	int cellSpursJobGuardInitialize(const CellSpursJobChain* jobChain,
									CellSpursJobGuard* jobGuard,
									uint32_t notifyCount,
									uint8_t requestSpuCount,
									uint8_t autoReset);
	int cellSpursJobGuardReset(CellSpursJobGuard* jobGuard);
	int cellSpursJobGuardNotify(CellSpursJobGuard* jobGuard);

/*
 * Exception Event Handling
 */
	int cellSpursJobChainSetExceptionEventHandler(
		CellSpursJobChain* jobChain,
		CellSpursJobChainExceptionEventHandler handler,
		void* arg
	);
	int cellSpursJobChainUnsetExceptionEventHandler( CellSpursJobChain* jobChain );

/*
 * Information Access
 */

	int cellSpursGetJobChainInfo( const CellSpursJobChain* jobChain, CellSpursJobChainInfo* info );
	int cellSpursGetJobPipelineInfo(CellSpurs* spurs, sys_spu_thread_t spu_thread, CellSpursJobPipelineInfo* info);



#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#ifdef __cplusplus
#include <cell/spurs/job_chain_types_cpp.h>
#endif /*__cplusplus*/



#endif /* __CELL_SPURS_JOB_CHAIN_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
