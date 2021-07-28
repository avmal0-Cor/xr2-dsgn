/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_PORT_H__
#define __CELL_SPURS_JOBQUEUE_PORT_H__

/* standard C++ header */
#include <stdint.h>

/* spurs */
#include <cell/spurs/job_descriptor.h>

/* job queue */
#include <cell/spurs/job_queue.h>
#include <cell/spurs/job_queue_port_types.h>

#ifdef __cplusplus
extern "C" {
#endif
	/* get information */
	uint64_t cellSpursJobQueuePortGetJobQueue(uint64_t eaPort);

	int	cellSpursJobQueuePortInitialize(uint64_t eaPort, uint64_t eaJobQueue, unsigned isMTSafe);
	int	cellSpursJobQueuePortInitializeWithDescriptorBuffer(uint64_t eaPort, uint64_t eaJobQueue, uint64_t eaBuffer, size_t sizeDesc, unsigned numEntries, unsigned isMTSafe);
	int cellSpursJobQueuePortFinalize(uint64_t eaPort);

	int _cellSpursJobQueuePortPushBody(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking);
	int _cellSpursJobQueuePortCopyPushBody(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking);

	static inline int _cellSpursJobQueuePortPush(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		char jobHeader[1024];
		cellDmaGet(jobHeader, eaJobDescriptor, sizeDesc, dmaTag, 0, 0); cellDmaWaitTagStatusAll(1<<dmaTag);
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)jobHeader, sizeDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(eaPort))));
#endif
		return _cellSpursJobQueuePortPushBody(eaPort, eaJobDescriptor, sizeDesc, dmaTag, isSync, isBlocking);
	}
	static inline int _cellSpursJobQueuePortCopyPush(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJobDescriptor, sizeDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(eaPort))));
#endif
		return _cellSpursJobQueuePortCopyPushBody(eaPort, pJobDescriptor, sizeDesc, dmaTag, isSync, isBlocking);
	}

	int _cellSpursJobQueuePortPushJobBody(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isExclusive, unsigned isBlocking);

	int _cellSpursJobQueuePortPushJobListBody(uint64_t eaPort, uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking);

	int _cellSpursJobQueuePortCopyPushJobBody(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isExclusive,  unsigned isBlocking);

	static inline int _cellSpursJobQueuePortPushJob(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isExclusive, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		char jobHeader[1024];
		cellDmaGet(jobHeader, eaJobDescriptor, sizeDesc, dmaTag, 0, 0); cellDmaWaitTagStatusAll(1<<dmaTag);
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)jobHeader, sizeDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(eaPort))));
#endif
		return _cellSpursJobQueuePortPushJobBody(eaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync, isExclusive, isBlocking);
	}

	static inline int _cellSpursJobQueuePortPushJobList(uint64_t eaPort, uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		CellSpursJobList	joblist;
		cellDmaGet(&joblist, eaJobList, sizeof(CellSpursJobList), dmaTag, 0, 0);
		cellDmaWaitTagStatusAll(1 << dmaTag);
		unsigned	maxSizeOfJobDescriptor = cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(eaPort));
		for(unsigned index = 0; index < joblist.numJobs; index++) {
			char jobHeader[1024];
			uint64_t	eaJobHeader = joblist.eaJobList 
										+ joblist.sizeOfJob * index;
			cellDmaGet(jobHeader, eaJobHeader, joblist.sizeOfJob, dmaTag, 0, 0); cellDmaWaitTagStatusAll(1<<dmaTag);
			__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)jobHeader, joblist.sizeOfJob, maxSizeOfJobDescriptor));
		}
#endif
		return _cellSpursJobQueuePortPushJobListBody(eaPort, eaJobList, tag, dmaTag, isSync, isBlocking);
	}

	static inline int _cellSpursJobQueuePortCopyPushJob(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync, unsigned isExclusive, unsigned isBlocking) {
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJobDescriptor, sizeDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(eaPort))));
#endif
		return _cellSpursJobQueuePortCopyPushJobBody(eaPort, pJobDescriptor, sizeDesc, tag, dmaTag, isSync, isExclusive, isBlocking);
	}

	/* external interface for push */
	static inline int cellSpursJobQueuePortPush(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPush(eaPort, eaJobDescriptor, sizeDesc, dmaTag, isSync, 1);
	}
	static inline int cellSpursJobQueuePortCopyPush(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortCopyPush(eaPort, pJobDescriptor, sizeDesc, dmaTag, isSync, 1);
	}

	static inline int cellSpursJobQueuePortPushJob(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJob(eaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync, 0, 1);
	}

	static inline int cellSpursJobQueuePortPushExclusiveJob(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJob(eaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync, 1, 1);
	}

	static inline int cellSpursJobQueuePortPushJobList(uint64_t eaPort, uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJobList(eaPort, eaJobList, tag, dmaTag, isSync, 1);
	}

	static inline int cellSpursJobQueuePortCopyPushJob(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortCopyPushJob(eaPort, pJobDescriptor, sizeDesc, tag, dmaTag, isSync, 0, 1);
	}

	static inline int cellSpursJobQueuePortCopyPushExclusiveJob(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortCopyPushJob(eaPort, pJobDescriptor, sizeDesc, tag, dmaTag, isSync, 1, 1);
	}

	static inline int cellSpursJobQueuePortTryPushJob(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJob(eaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync, 0, 0);
	}

	static inline int cellSpursJobQueuePortTryPushExclusiveJob(uint64_t eaPort, uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJob(eaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync, 1, 0);
	}

	static inline int cellSpursJobQueuePortTryPushJobList(uint64_t eaPort, uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortPushJobList(eaPort, eaJobList, tag, dmaTag, isSync, 0);
	}

	static inline int cellSpursJobQueuePortTryCopyPushJob(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortCopyPushJob(eaPort, pJobDescriptor, sizeDesc, tag, dmaTag, isSync, 0, 0);
	}

	static inline int cellSpursJobQueuePortTryCopyPushExclusiveJob(uint64_t eaPort, const CellSpursJobHeader* pJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return _cellSpursJobQueuePortCopyPushJob(eaPort, pJobDescriptor, sizeDesc, tag, dmaTag, isSync, 1, 0);
	}

	int cellSpursJobQueuePortSync(uint64_t eaPort);

	int cellSpursJobQueuePortTrySync(uint64_t eaPort);

	int _cellSpursJobQueuePortPushFlush(uint64_t eaPort, unsigned int dmaTag, unsigned isBlocking);
	static inline int cellSpursJobQueuePortPushFlush(uint64_t eaPort, unsigned int dmaTag) {
		return _cellSpursJobQueuePortPushFlush(eaPort, dmaTag, 1);
	}
	static inline int cellSpursJobQueuePortTryPushFlush(uint64_t eaPort, unsigned int dmaTag) {
		return _cellSpursJobQueuePortPushFlush(eaPort, dmaTag, 0);
	}

	int _cellSpursJobQueuePortPushSync(uint64_t eaPort, unsigned tagMask, unsigned int dmaTag, unsigned isBlocking);
	static inline int cellSpursJobQueuePortPushSync(uint64_t eaPort, unsigned tagMask, unsigned int dmaTag) {
		return _cellSpursJobQueuePortPushSync(eaPort, tagMask, dmaTag, 1);
	}
	static inline int cellSpursJobQueuePortTryPushSync(uint64_t eaPort, unsigned tagMask, unsigned int dmaTag) {
		return _cellSpursJobQueuePortPushSync(eaPort, tagMask, dmaTag, 0);
	}

#ifdef __cplusplus
}

__CELL_SPURS_JOBQUEUE_BEGIN

class PortContainer
{
protected:
	// member variables
	uint64_t				mEaPort;

	PortContainer(const PortContainer&);
	PortContainer& operator = (const PortContainer&);

private:

public:
	PortContainer(uint64_t ea) 
		:mEaPort(ea) {}
	~PortContainer() {}

	int	initialize(uint64_t eaJobQueue, unsigned isMTSafe = 1)
	{
		return cellSpursJobQueuePortInitialize(mEaPort, eaJobQueue, isMTSafe);
	}

	int finalize()
	{
		return cellSpursJobQueuePortFinalize(mEaPort);
	}

	uint64_t getJobQueue()
	{
		return cellSpursJobQueuePortGetJobQueue(mEaPort);
	}

	int push(uint64_t eaJobDescriptor, size_t sizeDesc, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortPush(mEaPort, eaJobDescriptor, sizeDesc, dmaTag, isSync);
	}
	int	copyPush(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned int dmaTag, unsigned int isSync) {
		return cellSpursJobQueuePortCopyPush(mEaPort, pJob, sizeJobDesc, dmaTag, isSync);
	}

	int pushJob(uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortPushJob(mEaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync);
	}
	int pushExclusiveJob(uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortPushExclusiveJob(mEaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync);
	}
	int tryPushJob(uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortTryPushJob(mEaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync);
	}
	int tryPushExclusiveJob(uint64_t eaJobDescriptor, size_t sizeDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortTryPushExclusiveJob(mEaPort, eaJobDescriptor, sizeDesc, tag, dmaTag, isSync);
	}
	int pushJobList(uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortPushJobList(mEaPort, eaJobList, tag, dmaTag, isSync);
	}
	int tryPushJobList(uint64_t eaJobList, unsigned tag, unsigned int dmaTag, unsigned int isSync)
	{
		return cellSpursJobQueuePortTryPushJobList(mEaPort, eaJobList, tag, dmaTag, isSync);
	}
	int	copyPushJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return cellSpursJobQueuePortCopyPushJob(mEaPort, pJob, sizeJobDesc, tag, dmaTag, isSync);
	}
	int	copyPushExclusiveJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return cellSpursJobQueuePortCopyPushExclusiveJob(mEaPort, pJob, sizeJobDesc, tag, dmaTag, isSync);
	}
	int	tryCopyPushJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return cellSpursJobQueuePortTryCopyPushJob(mEaPort, pJob, sizeJobDesc, tag, dmaTag, isSync);
	}
	int	tryCopyPushExclusiveJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return cellSpursJobQueuePortTryCopyPushExclusiveJob(mEaPort, pJob, sizeJobDesc, tag, dmaTag, isSync);
	}
	int sync(void)
	{
		return cellSpursJobQueuePortSync(mEaPort);
	}
	int trySync(void)
	{
		return cellSpursJobQueuePortTrySync(mEaPort);
	}
	int pushFlush(unsigned int dmaTag)
	{
		return cellSpursJobQueuePortPushFlush(mEaPort, dmaTag);
	}
	int tryPushFlush(unsigned int dmaTag)
	{
		return cellSpursJobQueuePortTryPushFlush(mEaPort, dmaTag);
	}
	int pushSync(unsigned tagMask, unsigned int dmaTag)
	{
		return cellSpursJobQueuePortPushSync(mEaPort, tagMask, dmaTag);
	}
	int tryPushSync(unsigned tagMask, unsigned int dmaTag)
	{
		return cellSpursJobQueuePortTryPushSync(mEaPort, tagMask, dmaTag);
	}
};

template <typename JobType, int numEntries> class PortWithDescriptorBufferContainer : public cell::Spurs::JobQueue::PortContainer
{
public:
	PortWithDescriptorBufferContainer(uint64_t ea) 
		:PortContainer(ea) {}
	~PortWithDescriptorBufferContainer() {}

	int	initialize(uint64_t eaJobQueue, unsigned isMTSafe = 1) {
		return cellSpursJobQueuePortInitializeWithDescriptorBuffer(mEaPort, eaJobQueue, mEaPort + sizeof(CellSpursJobQueuePort), sizeof(JobType), numEntries, isMTSafe);
	}
	int	copyPush(const JobType *pJob, size_t sizeJobDesc, unsigned int dmaTag, unsigned int isSync) {
		return PortContainer::copyPush((const CellSpursJobHeader*)pJob, sizeJobDesc, dmaTag, isSync);
	}
	int	copyPushJob(const JobType *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return PortContainer::copyPushJob((const CellSpursJobHeader*)pJob, sizeJobDesc, tag, dmaTag, isSync);
	}

	int	tryCopyPushJob(const JobType *pJob, size_t sizeJobDesc, unsigned tag, unsigned int dmaTag, unsigned int isSync) {
		return PortContainer::tryCopyPushJob((const CellSpursJobHeader*)pJob, sizeJobDesc, tag, dmaTag, isSync);
	}
};

__CELL_SPURS_JOBQUEUE_END

#endif /* #ifdef __cplusplus */

#endif /* __CELL_SPURS_JOBQUEUE_PORT_H__ */
