/* SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   * Copyright (C) 2008 Sony Computer Entertainment Inc.
   * All Rights Reserved.
   */

#ifndef __CELL_SPURS_JOB_CHAIN_H__
#define __CELL_SPURS_JOB_CHAIN_H__


#include <cell/spurs/job_context.h>
#include <spu_mfcio.h>

static inline
int cellSpursJobStartNextJob(CellSpursJob256 *job)
{
	if (__builtin_expect(!(job->header.jobType & CELL_SPURS_JOB_TYPE_STALL_SUCCESSOR), 0)) {
		return CELL_SPURS_JOB_ERROR_INVAL;
	}
	unsigned list_stall_tagmask_status = mfc_read_list_stall_status();
	unsigned stall_tag = 31 - spu_extract(spu_cntlz(spu_promote(list_stall_tagmask_status, 0)), 0);
	mfc_write_list_stall_ack(stall_tag);

	return CELL_OK;
}

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	void cellSpursJobMain2(CellSpursJobContext2* jobContext,
						   CellSpursJob256 *job) __attribute__((unused));
	void cellSpursJobInitialize(void);

/*
 *  SPURS Job Chain
 */
	/* This works with job chain created by cellSpursCreateJobChain() */
	int cellSpursKickJobChain(uint64_t eaJobChain, uint8_t numReadyCount);
	/* This works with job chain created by cellSpursCreateJobChainWithAttribute() */
	int cellSpursRunJobChain(uint64_t eaJobChain);

	int cellSpursShutdownJobChain(uint64_t eaJobChain);
	int cellSpursJobSetMaxGrab(uint64_t eaJobChain, unsigned int maxGrab);

/*
 *  SPURS Job Guard
 */
	int cellSpursJobGuardInitialize(uint64_t eaJobChain,
									uint64_t eaJobGuard,
									uint32_t notifyCount,
									uint8_t requestSpuCount,
									uint8_t autoReset,
									unsigned int tag);
	int cellSpursJobGuardReset(uint64_t eaJobGuard);
	int cellSpursJobGuardNotify(uint64_t eaJobGuard);

/*
 *  SPURS Urgent Job
 */
	int cellSpursAddUrgentCommand(uint64_t eaJobChain, uint64_t command);
	int cellSpursAddUrgentCall(uint64_t eaJobChain, uint64_t commandList);

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
