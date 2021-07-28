/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOB_CHAIN_TYPES_CPP_H__
#define __CELL_SPURS_JOB_CHAIN_TYPES_CPP_H__ 1

#include "job_chain_types.h"
#include "job_guard.h"
#include <cell/spurs/job_chain.h>
#include <stdio.h>

namespace cell {
	namespace Spurs {

#ifdef __PPU__
		class JobChainAttribute : public CellSpursJobChainAttribute
		{
		public:
			static const uint32_t kAlign = CELL_SPURS_JOBCHAIN_ATTRIBUTE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOBCHAIN_ATTRIBUTE_SIZE;

			inline static int initialize(CellSpursJobChainAttribute *attr,
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
					return cellSpursJobChainAttributeInitialize(attr,
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
				}
			inline int setName(const char* name)
				{
					return cellSpursJobChainAttributeSetName(this, name);
				}
			inline int setHaltOnError(void)
				{
					return cellSpursJobChainAttributeSetHaltOnError(this);
				}
		};

#endif /* __PPU__ */

		class JobChain : public CellSpursJobChain
		{
		public:
 			static const uint32_t kAlign = CELL_SPURS_JOBCHAIN_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOBCHAIN_SIZE;

#ifdef __PPU__
			inline static int createWithAttribute(CellSpurs *spurs,
										   CellSpursJobChain *jobChain,
										   const CellSpursJobChainAttribute *attr)
				{
					return cellSpursCreateJobChainWithAttribute(spurs,jobChain,attr);
				}
			inline int shutdown(void) const
				{
					return cellSpursShutdownJobChain(this);
				}
			inline int run(void) const
				{
					return cellSpursRunJobChain(this);
				}
			inline int join(void)
				{
					return cellSpursJoinJobChain(this);
				}
			inline int getId(CellSpursWorkloadId* wid) const
				{
					return cellSpursGetJobChainId(this, wid);
				}
			inline int getError(void** cause)
				{
					return cellSpursJobChainGetError(this, cause);
				}
			inline int getSpursAddress(CellSpurs** ppSpurs) const
				{
					return cellSpursJobChainGetSpursAddress(this, ppSpurs);
				}
			inline int setMaxGrab(unsigned int maxGrab)
				{
					return cellSpursJobSetMaxGrab(this, maxGrab);
				}
			
			inline int setExceptionEventHandler(CellSpursJobChainExceptionEventHandler &handler, void* arg)
				{
					return cellSpursJobChainSetExceptionEventHandler(this,handler,arg);
				}
			inline int unsetExceptionEventHandler(void)
				{
					return cellSpursJobChainUnsetExceptionEventHandler(this);
				}
			inline int getInfo( CellSpursJobChainInfo* info ) const
				{
					return cellSpursGetJobChainInfo( this, info);
				}
			inline static int getJobPipelineInfo(CellSpurs* spurs, sys_spu_thread_t spu_thread, CellSpursJobPipelineInfo* info)
				{
					return cellSpursGetJobPipelineInfo(spurs, spu_thread, info);
				}

			inline int addUrgentCall(uint64_t *commandList)
				{
					return cellSpursAddUrgentCall(this, commandList);
				}
			inline int addUrgentCommand(uint64_t command)
				{
					return cellSpursAddUrgentCommand(this, command);
				}

#endif /* __PPU__ */
		} __attribute__((aligned(CELL_SPURS_JOBCHAIN_ALIGN)));

		class JobChainStub 
		{
			uint64_t object_ea;
		public:
 			static const uint32_t kAlign = CELL_SPURS_JOBCHAIN_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOBCHAIN_SIZE;

#ifdef __PPU__
			inline void setObject(CellSpursJobChain *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline JobChain *getObject(void) const
				{
					return (JobChain*)(uintptr_t)object_ea;
				}

			inline int createWithAttribute(CellSpurs *spurs,  const CellSpursJobChainAttribute *attr)
				{
					return cellSpursCreateJobChainWithAttribute(spurs,getObject(),attr);
				}
			inline int shutdown(void) const
				{
					return cellSpursShutdownJobChain(getObject());
				}
			inline int run(void) const
				{
					return cellSpursRunJobChain(getObject());
				}
			inline int join(void) const
				{
					return cellSpursJoinJobChain(getObject());
				}
			inline int getId(CellSpursWorkloadId* wid) const
				{
					return cellSpursGetJobChainId(getObject(), wid);
				}
			inline int getError(void** cause) const
				{
					return cellSpursJobChainGetError(getObject(), cause);
				}
			inline int getSpursAddress(CellSpurs** ppSpurs) const
				{
					return cellSpursJobChainGetSpursAddress(getObject(), ppSpurs);
				}
			inline int setMaxGrab(unsigned int maxGrab) const
				{
					return cellSpursJobSetMaxGrab(getObject(), maxGrab);
				}
			
			inline int setExceptionEventHandler(CellSpursJobChainExceptionEventHandler &handler, void* arg) const
				{
					return cellSpursJobChainSetExceptionEventHandler(getObject(), handler, arg);
				}
			inline int unsetExceptionEventHandler(void) const
				{
					return cellSpursJobChainUnsetExceptionEventHandler(getObject());
				}
			inline int getInfo( CellSpursJobChainInfo* info ) const
				{
					return cellSpursGetJobChainInfo(getObject(), info);
				}
			inline int addUrgentCall(uint64_t *commandList) const
				{
					return cellSpursAddUrgentCall(getObject(), commandList);
				}
			inline int addUrgentCommand(uint64_t command) const
				{
					return cellSpursAddUrgentCommand(getObject(), command);
				}
			
#endif /* __PPU__ */
#ifdef __SPU__
			inline void setTarget(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline void setObject(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}
			int run(void)
				{
					return cellSpursRunJobChain(getObject());
				}
			int shutdown(void)
				{
					return cellSpursShutdownJobChain(getObject());
				}
			int setMaxGrab(unsigned int maxGrab)
				{
					return cellSpursJobSetMaxGrab(getObject(), maxGrab);
				}
			int addUrgentCommand(uint64_t command)
				{
					return cellSpursAddUrgentCommand(getObject(), command);
				}
			int addUrgentCall(uint64_t commandList)
				{
					return cellSpursAddUrgentCall(getObject(), commandList);
				}

#endif /* __SPU__ */
		};



		class JobGuard : public CellSpursJobGuard
		{
		public:

			static const uint32_t kAlign = CELL_SPURS_JOB_GUARD_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOB_GUARD_SIZE;

#ifdef __PPU__
			static inline int initialize(const CellSpursJobChain* jobChain,
										 CellSpursJobGuard* jobGuard,
										 uint32_t notifyCount,
										 uint8_t requestSpuCount,
										 uint8_t autoReset)
				{
					return cellSpursJobGuardInitialize(jobChain,jobGuard,notifyCount,
													   requestSpuCount,autoReset);
				}
			inline int reset(void)
				{
					return cellSpursJobGuardReset(this);
				}
			inline int notify(void)
				{
					return cellSpursJobGuardNotify(this);
				}
#endif /* __PPU__ */
		};

		class JobGuardStub 
		{
			uint64_t object_ea;
		public:
			static const uint32_t kAlign = CELL_SPURS_JOB_GUARD_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOB_GUARD_SIZE;

#ifdef __PPU__
			inline void setObject(CellSpursJobGuard *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline JobGuard *getObject(void) const
				{
					return (JobGuard*)(uintptr_t)object_ea;
				}
			inline int initialize(const CellSpursJobChain* jobChain,
								  uint32_t notifyCount,
								  uint8_t requestSpuCount,
								  uint8_t autoReset) const
				{
					return cellSpursJobGuardInitialize(jobChain,getObject(),notifyCount,
													   requestSpuCount,autoReset);
				}
			inline int reset(void) const
				{
					return cellSpursJobGuardReset(getObject());
				}
			inline int notify(void) const
				{
					return cellSpursJobGuardNotify(getObject());
				}

#endif /* __PPU__ */
#ifdef __SPU__
			inline void setTarget(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline void setObject(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}
			inline int initialize(uint64_t eaJobChain,  uint32_t notifyCount,	  uint8_t requestSpuCount,
								  uint8_t autoReset,  unsigned int tag) const
				{
					return cellSpursJobGuardInitialize(eaJobChain,   getObject(),   notifyCount,   requestSpuCount,   autoReset,  tag);
				}
			inline int reset(void) const
				{
					return cellSpursJobGuardReset(getObject());
				}
			inline int notify(void) const
				{
					return cellSpursJobGuardNotify(getObject());
				}

#endif /* __SPU__ */			
		};


		class JobList : public CellSpursJobList
		{
		public:

			static const uint32_t kAlign = CELL_SPURS_JOB_LIST_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOB_LIST_SIZE;

#ifdef __PPU__
			static inline int initialize(JobList *jl, uint32_t _numJobs,	  uint32_t _sizeOfJob,  uint64_t _eaJobList)
				{
					if( (_sizeOfJob & 127) != 0){
						if(_sizeOfJob != 64){
							return CELL_SPURS_JOB_ERROR_INVAL;
						}
					}
					if(_eaJobList & 15 != 0){
						return CELL_SPURS_JOB_ERROR_ALIGN;
					}
					
					jl->numJobs = _numJobs;
					jl->sizeOfJob = _sizeOfJob;
					jl->eaJobList = _eaJobList;
					
					return CELL_OK;
				}
#endif /* __PPU__ */
		};

		class JobListStub 
		{
			uint64_t object_ea;
		public:
			static const uint32_t kAlign = CELL_SPURS_JOB_LIST_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_JOB_LIST_SIZE;

#ifdef __PPU__
			inline void setObject(CellSpursJobList *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline JobList *getObject(void) const
				{
					return (JobList*)(uintptr_t)object_ea;
				}
			inline int initialize( uint32_t _numJobs,	  uint32_t _sizeOfJob,  uint64_t _eaJobList) const
				{
					JobList *jl = getObject();
					if(jl == NULL){
						return CELL_SPURS_JOB_ERROR_NULL_POINTER;
					}

					return JobList::initialize(jl, _numJobs,	_sizeOfJob, _eaJobList);
				}

#endif /* __PPU__ */
#ifdef __SPU__
			inline void setObject(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}
			inline int initialize( uint32_t _numJobs,	  uint32_t _sizeOfJob,  uint64_t _eaJobList, uint32_t tag) const
				{
					if( (_sizeOfJob & 127) != 0){
						if(_sizeOfJob != 64){
							return CELL_SPURS_JOB_ERROR_INVAL;
						}
					}
					if(_eaJobList & 15 != 0){
						return CELL_SPURS_JOB_ERROR_ALIGN;
					}

					uint64_t eaJl = getObject();
					if(eaJl == NULL){
						return CELL_SPURS_JOB_ERROR_NULL_POINTER;
					}
					if(eaJl % 16 != 0){
						return CELL_SPURS_JOB_ERROR_ALIGN;
					}

					JobList  *list = (JobList*)CELL_SPURS_LOCK_LINE;
					list->numJobs = _numJobs;
					list->sizeOfJob = _sizeOfJob;
					list->eaJobList = _eaJobList;
					
					mfc_put(list, eaJl, sizeof(CellSpursJobList), tag, 0,0);
					mfc_write_tag_mask(1<<tag);
					mfc_read_tag_status_all();

					return CELL_OK;
				}

#endif /* __SPU__ */			
		};

	}
}
#endif /* __CELL_SPURS_JOB_CHAIN_TYPES_CPP_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
