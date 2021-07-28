/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_TYPES_CPP_H__
#define __CELL_SPURS_TYPES_CPP_H__ 1

#include "types.h"

#ifdef __PPU__
#include <cell/spurs/control.h>
#include <cell/spurs/exception.h>
#include <cell/spurs/lv2_event_queue.h>
#endif /* __PPU__ */



namespace cell {
	namespace Spurs {	

#ifdef __PPU__
		class SpursAttribute : public CellSpursAttribute
		{
		public:
			static int initialize(SpursAttribute *attribute, int nSpus, int spuPriority, int ppuPriority,  bool exitIfNoWork)
				{
					return cellSpursAttributeInitialize(attribute, nSpus, spuPriority, ppuPriority, exitIfNoWork);
				}
			int	setMemoryContainerForSpuThread(sys_memory_container_t container)
				{
					return cellSpursAttributeSetMemoryContainerForSpuThread(this, container);
				}
			
			int	setNamePrefix(const char* name,size_t size)
				{
					return cellSpursAttributeSetNamePrefix(this, name, size);
				}
			
			int setSpuThreadGroupType(int type)
				{
					return cellSpursAttributeSetSpuThreadGroupType(this, type);
				}
			
			int enableSpuPrintfIfAvailable(void)
				{
					return cellSpursAttributeEnableSpuPrintfIfAvailable(this);
				}
		};
#endif /* __PPU__ */			


		class Spurs : public CellSpurs 
		{
		public:
			static const uint32_t kMaxWorkload     = CELL_SPURS_MAX_WORKLOAD;
			static const uint32_t kMaxPriority     = CELL_SPURS_MAX_PRIORITY;
			static const uint32_t kNameMaxLength   = CELL_SPURS_NAME_MAX_LENGTH;
			static const uint32_t kAlign           = CELL_SPURS_ALIGN;
			static const uint32_t kSize            = CELL_SPURS_SIZE;
			static const uint32_t kInterruptVector = CELL_SPURS_INTERRUPT_VECTOR;
			static const uint32_t kLockLine        = CELL_SPURS_LOCK_LINE;
			static const uint32_t kKernelDmaTagId  = CELL_SPURS_KERNEL_DMA_TAG_ID;


#ifdef __PPU__
			static int initialize(Spurs* spurs,	unsigned nSpus,	int spuPriority,int ppuPriority,  bool exitIfNoWork)
				{
					return cellSpursInitialize(spurs,nSpus,spuPriority,ppuPriority,exitIfNoWork);
				}
			static int initialize(Spurs* spurs, const CellSpursAttribute* attr)
				{
					return cellSpursInitializeWithAttribute (spurs, attr);
				}
		
			static int initializeWithAttribute(Spurs* spurs, const CellSpursAttribute* attr)
				{
					return cellSpursInitializeWithAttribute (spurs, attr);
				}
			int finalize(void)
				{
					return cellSpursFinalize(this);
				}
			int wakeUp(void)
				{
					return cellSpursWakeUp(this);
				}
			int getNumSpuThread(unsigned *nThreads)
				{
					return cellSpursGetNumSpuThread(this, nThreads);
				}
			int setMaxContention(CellSpursWorkloadId wid,  unsigned int maxContention)
				{
					return cellSpursSetMaxContention(this,  wid, maxContention);
				}
			int setPriorities(CellSpursWorkloadId wid, const uint8_t priorities[CELL_SPURS_MAX_SPU])
				{
					return cellSpursSetPriorities(this,  wid, priorities);
				}
			int getSpuThreadId(sys_spu_thread_t* thr, unsigned *nThreads)
				{
					return cellSpursGetSpuThreadId(this, thr, nThreads);
				}
			int getSpuThreadGroupId(sys_spu_thread_group_t* thr_group)
				{
					return cellSpursGetSpuThreadGroupId(this, thr_group);
				}
			int getInfo(CellSpursInfo *info)
				{
					return cellSpursGetInfo(this, info);
				}
			int attachLv2EventQueue(sys_event_queue_t queue, uint8_t* port, int isDynamic)
				{
					return cellSpursAttachLv2EventQueue(this, queue, port, isDynamic);
				}
			int detachLv2EventQueue(uint8_t port)
				{
					return cellSpursDetachLv2EventQueue(this, port);
				}
			int enableExceptionEventHandler(bool flag)
				{
					return cellSpursEnableExceptionEventHandler(this, flag);
				}
			int setExceptionEventHandler(CellSpursWorkloadId id,	CellSpursExceptionEventHandler eaHandler,	void* arg)
				{
					return cellSpursSetExceptionEventHandler(this, id, eaHandler, arg);
				}
			int unsetExceptionEventHandler(CellSpursWorkloadId id)
				{
					return cellSpursUnsetExceptionEventHandler(this, id);
				}
			int setGlobalExceptionEventHandler(CellSpursGlobalExceptionEventHandler eaHandler, void* arg)
				{
					return cellSpursSetGlobalExceptionEventHandler(this, eaHandler, arg);
				}
			int unsetGlobalExceptionEventHandler(void)
				{
					return cellSpursUnsetGlobalExceptionEventHandler(this);
				}

#endif /* __PPU__ */	
		};

	}
}

#endif /* __CELL_SPURS_TYPES_CPP_H__ */
