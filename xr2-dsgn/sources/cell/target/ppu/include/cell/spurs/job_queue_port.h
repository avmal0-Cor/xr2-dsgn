/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_PORT_H__
#define __CELL_SPURS_JOBQUEUE_PORT_H__

/* standard C++ header */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */

/* spurs */
#include <cell/spurs/job_descriptor.h>

/* job queue */
#include <cell/spurs/job_queue.h>
#include <cell/spurs/job_queue_port_types.h>

#ifdef __cplusplus
extern "C" {
#endif
	/* APIs for CellSpursJobQueuePort */
	/* get information */
	CellSpursJobQueue*	cellSpursJobQueuePortGetJobQueue(const CellSpursJobQueuePort *pPort);

	int	cellSpursJobQueuePortInitialize(CellSpursJobQueuePort *pPort, CellSpursJobQueue *pJobQueue, bool isMTSafe);
	int	cellSpursJobQueuePortInitializeWithDescriptorBuffer(CellSpursJobQueuePort *pPort, CellSpursJobQueue *pJobQueue, CellSpursJobHeader* buffer, size_t sizeDescriptor, unsigned numEntries, bool isMTSafe);
	int cellSpursJobQueuePortFinalize(CellSpursJobQueuePort *pPort);

	int _cellSpursJobQueuePortPushBody(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync, bool isBlocking);
	int _cellSpursJobQueuePortCopyPushBody(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync, bool isBlocking);

	static inline int _cellSpursJobQueuePortPush(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync, bool isBlocking){
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJob, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(pPort))));
#endif

		return _cellSpursJobQueuePortPushBody(pPort, pJob, sizeJobDesc, isSync, isBlocking);
	}

	static inline int _cellSpursJobQueuePortCopyPush(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync, bool isBlocking){
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJob, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(pPort))));
#endif
		return _cellSpursJobQueuePortCopyPushBody(pPort, pJob, sizeJobDesc, isSync, isBlocking);
	}

	int _cellSpursJobQueuePortPushJobBody(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isBlocking);
	int _cellSpursJobQueuePortPushJobBody2(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isExclusive, bool isBlocking);
	int _cellSpursJobQueuePortCopyPushJobBody(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isBlocking);
	int _cellSpursJobQueuePortCopyPushJobBody2(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isExclusive, bool isBlocking);

	int _cellSpursJobQueuePortPushJobListBody(CellSpursJobQueuePort *pPort, CellSpursJobList *pJobList, unsigned tag, bool isSync, bool isBlocking);

	static inline int _cellSpursJobQueuePortPushJob2(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isExclusive, bool isBlocking){
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJob, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(pPort))));
#endif
		return _cellSpursJobQueuePortPushJobBody2(pPort, pJob, sizeJobDesc, tag, isSync, isExclusive, isBlocking);
	}

	static inline int _cellSpursJobQueuePortPushJobList(CellSpursJobQueuePort *pPort, CellSpursJobList *pJobList, unsigned tag, bool isSync, bool isBlocking){
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		for(unsigned index = 0; index < pJobList->numJobs; index++) {
			CellSpursJob256*	pJob = (CellSpursJob256*)(uintptr_t)pJobList->eaJobList + pJobList->sizeOfJob * index;
			__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)pJob, pJobList->sizeOfJob, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(pPort))));
		}
#endif
		return _cellSpursJobQueuePortPushJobListBody(pPort, pJobList, tag, isSync, isBlocking);
	}

	static inline int _cellSpursJobQueuePortCopyPushJob2(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync, bool isExclusive, bool isBlocking){
#ifdef CELL_SPURS_JOBDESCRIPTOR_ERROR_CHECK
		__CELL_SPURS_RETURN_IF(cellSpursJobQueueCheckJob((const CellSpursJob256 *)(uintptr_t)pJob, sizeJobDesc, cellSpursJobQueueGetMaxSizeJobDescriptor(cellSpursJobQueuePortGetJobQueue(pPort))));
#endif
		return _cellSpursJobQueuePortCopyPushJobBody2(pPort, pJob, sizeJobDesc, tag, isSync, isExclusive, isBlocking);
	}

	/* external push interfaces */
	static inline int cellSpursJobQueuePortPush(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync){
		return _cellSpursJobQueuePortPush(pPort, pJob, sizeJobDesc, isSync, 1);
	}
	static inline int cellSpursJobQueuePortCopyPush(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync){
		return _cellSpursJobQueuePortCopyPush(pPort, pJob, sizeJobDesc, isSync, 1);
	}

	static inline int cellSpursJobQueuePortPushJob(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, false, 1);
	}
	static inline int cellSpursJobQueuePortPushExclusiveJob(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, true, 1);
	}
	static inline int cellSpursJobQueuePortPushJobList(CellSpursJobQueuePort *pPort, CellSpursJobList *pJobList, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJobList(pPort, pJobList, tag, isSync, 1);
	}
	static inline int cellSpursJobQueuePortCopyPushJob(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortCopyPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, false, 1);
	}
	static inline int cellSpursJobQueuePortCopyPushExclusiveJob(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortCopyPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, true, 1);
	}

	static inline int cellSpursJobQueuePortTryPushJob(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, false, 0);
	}
	static inline int cellSpursJobQueuePortTryPushExclusiveJob(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, true, 0);
	}
	static inline int cellSpursJobQueuePortTryPushJobList(CellSpursJobQueuePort *pPort, CellSpursJobList *pJobList, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortPushJobList(pPort, pJobList, tag, isSync, 0);
	}
	static inline int cellSpursJobQueuePortTryCopyPushJob(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortCopyPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, false, 0);
	}
	static inline int cellSpursJobQueuePortTryCopyPushExclusiveJob(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync){
		return _cellSpursJobQueuePortCopyPushJob2(pPort, pJob, sizeJobDesc, tag, isSync, true, 0);
	}

	static inline int cellSpursJobQueuePortTryPush(CellSpursJobQueuePort *pPort, CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync){
		return _cellSpursJobQueuePortPush(pPort, pJob, sizeJobDesc, isSync, 0);
	}
	static inline int cellSpursJobQueuePortTryCopyPush(CellSpursJobQueuePort *pPort, const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync){
		return _cellSpursJobQueuePortCopyPush(pPort, pJob, sizeJobDesc, isSync, 0);
	}

	int cellSpursJobQueuePortSync(CellSpursJobQueuePort *pPort);
	int cellSpursJobQueuePortTrySync(CellSpursJobQueuePort *pPort);

	int _cellSpursJobQueuePortPushFlush(CellSpursJobQueuePort *pPort, bool isBlocking);
	static inline int cellSpursJobQueuePortPushFlush(CellSpursJobQueuePort *pPort) {
		return _cellSpursJobQueuePortPushFlush(pPort, true);
	}
	static inline int cellSpursJobQueuePortTryPushFlush(CellSpursJobQueuePort *pPort) {
		return _cellSpursJobQueuePortPushFlush(pPort, false);
	}
	
	int _cellSpursJobQueuePortPushSync(CellSpursJobQueuePort *pPort, unsigned tagMask, bool isBlocking);
	static inline int cellSpursJobQueuePortPushSync(CellSpursJobQueuePort *pPort, unsigned tagMask) {
		return _cellSpursJobQueuePortPushSync(pPort, tagMask, true);
	}
	static inline int cellSpursJobQueuePortTryPushSync(CellSpursJobQueuePort *pPort, unsigned tag) {
		return _cellSpursJobQueuePortPushSync(pPort, tag, false);
	}
	
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* C++ interfaces */

__CELL_SPURS_JOBQUEUE_BEGIN

class Port : public CellSpursJobQueuePort
{
protected:
	Port(const Port&);
	Port& operator = (const Port&);

public:
	Port() {}
	~Port() {}

	int	initialize(CellSpursJobQueue* pJobQueue, bool isMTSafe = true)
	{
		return cellSpursJobQueuePortInitialize(this, pJobQueue, isMTSafe);
	}

	int	finalize()
	{
		return cellSpursJobQueuePortFinalize(this);
	}

	CellSpursJobQueue *getJobQueue()
	{
		return cellSpursJobQueuePortGetJobQueue(this);
	}

	int push(CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync)
	{
		return cellSpursJobQueuePortPush(this, pJob, sizeJobDesc, isSync);
	}
	int copyPush(const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync)
	{
		return cellSpursJobQueuePortCopyPush(this, pJob, sizeJobDesc, isSync);
	}

	int pushJob(CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortPushJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int pushExclusiveJob(CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortPushExclusiveJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int pushJobList(CellSpursJobList *pJobList, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortPushJobList(this, pJobList, tag, isSync);
	}
	int copyPushJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortCopyPushJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int copyPushExclusiveJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortCopyPushExclusiveJob(this, pJob, sizeJobDesc, tag, isSync);
	}

	int sync()
	{
		return cellSpursJobQueuePortSync(this);
	}

	int pushFlush()
	{
		return cellSpursJobQueuePortPushFlush(this);
	}

	int pushSync(unsigned tagMask)
	{
		return cellSpursJobQueuePortPushSync(this, tagMask);
	}

	int tryPush(CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync)
	{
		return cellSpursJobQueuePortTryPush(this, pJob, sizeJobDesc, isSync);
	}
	int tryCopyPush(const CellSpursJobHeader *pJob, size_t sizeJobDesc, bool isSync)
	{
		return cellSpursJobQueuePortTryCopyPush(this, pJob, sizeJobDesc, isSync);
	}

	int tryPushJob(CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortTryPushJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int tryPushExclusiveJob(CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortTryPushExclusiveJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int tryPushJobList(CellSpursJobList *pJobList, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortTryPushJobList(this, pJobList, tag, isSync);
	}
	int tryCopyPushJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortTryCopyPushJob(this, pJob, sizeJobDesc, tag, isSync);
	}
	int tryCopyPushExclusiveJob(const CellSpursJobHeader *pJob, size_t sizeJobDesc, unsigned tag, bool isSync)
	{
		return cellSpursJobQueuePortTryCopyPushExclusiveJob(this, pJob, sizeJobDesc, tag, isSync);
	}

	int trySync()
	{
		return cellSpursJobQueuePortTrySync(this);
	}

	int tryPushFlush()
	{
		return cellSpursJobQueuePortTryPushFlush(this);
	}

	int tryPushSync(unsigned tagMask)
	{
		return cellSpursJobQueuePortTryPushSync(this, tagMask);
	}
};

template <typename JobType, int numEntries> class PortWithDescriptorBuffer : public cell::Spurs::JobQueue::Port
{
private:
	JobType	mDescriptorBuffer[numEntries];

public:
	int	initialize(CellSpursJobQueue* pJobQueue, bool isMTSafe = true)
	{
		return cellSpursJobQueuePortInitializeWithDescriptorBuffer(this, pJobQueue, (CellSpursJobHeader*)mDescriptorBuffer, sizeof(JobType), numEntries, isMTSafe);
	}
	int	copyPush(const JobType* job, size_t sizeJobDesc, bool isSync) {
		return Port::copyPush((const CellSpursJobHeader*)job, sizeJobDesc, isSync);
	}

	int	copyPushJob(const JobType* job, size_t sizeJobDesc, unsigned tag, bool isSync) {
		return Port::copyPushJob((const CellSpursJobHeader*)job, sizeJobDesc, tag, isSync);
	}

	int	tryCopyPush(const JobType* job, size_t sizeJobDesc, bool isSync) {
		return Port::tryCopyPush((const CellSpursJobHeader*)job, sizeJobDesc, isSync);
	}

	int	tryCopyPushJob(const JobType* job, size_t sizeJobDesc, unsigned tag, bool isSync) {
		return Port::tryCopyPushJob((const CellSpursJobHeader*)job, sizeJobDesc, tag, isSync);
	}
};

__CELL_SPURS_JOBQUEUE_END

#endif /* #ifdef __cplusplus */

#endif /* __CELL_SPURS_JOBQUEUE_PORT_H__ */
