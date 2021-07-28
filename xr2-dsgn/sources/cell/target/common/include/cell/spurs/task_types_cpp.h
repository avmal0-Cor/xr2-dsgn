/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TASK_TYPES_CPP_H__
#define __CELL_SPURS_TASK_TYPES_CPP_H__ 1

#include <cell/spurs/task.h>
#include "task_types.h"
#include <cell/spurs/types_cpp.h>
#include <cell/spurs/task.h>
#include <cell/spurs/event_flag.h>
#include <cell/spurs/barrier.h>
#include <cell/spurs/semaphore.h>
#include <cell/spurs/queue.h>		
#include <cell/spurs/lfqueue.h>


namespace cell {
	namespace Spurs {

#ifdef __PPU__
		int Taskset::createWithAttribute (CellSpurs* spurs, CellSpursTaskset* taskset, const CellSpursTasksetAttribute* attr)
		{
			return cellSpursCreateTasksetWithAttribute(spurs, taskset, attr);
		}
		int Taskset::create(CellSpurs* spurs , CellSpursTaskset* taskset, uint64_t argTaskset, const uint8_t priority[8], unsigned int max_contention)
		{
			return cellSpursCreateTaskset(spurs, taskset, argTaskset, priority, max_contention);
		}
		int Taskset::shutdown(void)
		{
			return cellSpursShutdownTaskset(this);
		}
		int Taskset::join(void)
		{
			return cellSpursJoinTaskset(this);
		}
		int Taskset::lookUpTasksetAddress(CellSpurs *spurs, CellSpursTaskset **taskset, CellSpursWorkloadId id)
		{
			return cellSpursLookUpTasksetAddress(spurs, taskset, id);
		}
		
		int Taskset::getTasksetId(CellSpursWorkloadId* wid) const
		{
			return cellSpursGetTasksetId(this, wid);
		}
		int Taskset::getSpursAddress(CellSpurs** ppSpurs) const 
		{
			return cellSpursTasksetGetSpursAddress(this, ppSpurs);
		}
		int	Taskset::createTaskWithAttribute (CellSpursTaskId* tid,	const CellSpursTaskAttribute* attr)
		{
			return cellSpursCreateTaskWithAttribute(this, tid, attr);
		}
		int Taskset::createTask(CellSpursTaskId* tid, const void *eaElf,	const void *eaContext,
					   uint32_t sizeContext, const CellSpursTaskLsPattern* lspattern, const CellSpursTaskArgument* arg)
		{
			return cellSpursCreateTask(this, tid, eaElf, eaContext, sizeContext, lspattern, arg);
		}

		int Taskset::sendSignal(CellSpursTaskId id)
		{
			return cellSpursSendSignal(this, id);
		}
		int Taskset::setExceptionEventHandler(CellSpursTasksetExceptionEventHandler handler,	void* arg)
		{
			return cellSpursTasksetSetExceptionEventHandler(this, handler,  arg);
			
		}
		int Taskset::unsetExceptionEventHandler(void)
		{
			return cellSpursTasksetUnsetExceptionEventHandler(this);
		}
		int Taskset::getTasksetInfo(CellSpursTasksetInfo* info) const
		{
			return cellSpursGetTasksetInfo(this, info);
		}
		

		struct TasksetAttribute2 : public CellSpursTasksetAttribute2
		{
		public:
			static void initialize(CellSpursTasksetAttribute2 *attribute)
				{
					cellSpursTasksetAttribute2Initialize(attribute);
				}
		};

#endif /*__PPU__*/

		struct TaskAttribute2 : public CellSpursTaskAttribute2 
		{
#ifdef __PPU__
			static void initialize(CellSpursTaskAttribute2 *pAttr)
				{
					cellSpursTaskAttribute2Initialize(pAttr);
				}
#endif /*__PPU__*/
		};

		class Taskset2 : public CellSpursTaskset2
		{
		public:
			static const uint32_t kSize  = CELL_SPURS_TASKSET2_SIZE;
			static const uint32_t kAlign = CELL_SPURS_TASKSET2_ALIGN;
#ifdef __PPU__
			static inline int create(CellSpurs* pSpurs, CellSpursTaskset2* pTaskset, const CellSpursTasksetAttribute2* pAttr)
				{
					return cellSpursCreateTaskset2(pSpurs, pTaskset, pAttr);
				}
			int destroy(void)
				{
					return cellSpursDestroyTaskset2(this);
				}

			int	createTask2 (CellSpursTaskId* id, const void *eaElf, CellSpursTaskArgument *argument, const CellSpursTaskAttribute2* attr)
				{
					return cellSpursCreateTask2 (this,  id,	eaElf, argument, attr);
				}
			int createTask2(CellSpursTaskId* id,
							const CellSpursTaskBinInfo *binInfo,	 const CellSpursTaskArgument *argument, void* contextBuffer, const char* name,
							void *__reserved__ = NULL)
				{
					return cellSpursCreateTask2WithBinInfo (this,	id,
															binInfo,	 argument,  contextBuffer, name, __reserved__);
				}
			
			int createTask2WithBinInfo (CellSpursTaskId* id,
										const CellSpursTaskBinInfo *binInfo,	 const CellSpursTaskArgument *argument, void* contextBuffer, const char* name,
										void *__reserved__ = NULL)
				{
					return cellSpursCreateTask2WithBinInfo (this,	id,
															binInfo,	 argument,  contextBuffer, name, __reserved__);
				}

			int joinTask2(CellSpursTaskId idTask, int *exitCode)
				{
					return cellSpursJoinTask2(this, idTask, exitCode);
				}
			int tryJoinTask2(CellSpursTaskId idTask, int *exitCode)
				{
					return cellSpursTryJoinTask2(this, idTask, exitCode);
				}
#endif /*__PPU__*/
			
		};

		class TasksetStub 
		{
		protected:
			uint64_t object_ea;
		public:
#ifdef __PPU__
			inline void setObject(CellSpursTaskset *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline Taskset *getObject(void) const
				{
					return (Taskset *)(uintptr_t)object_ea;
				}
			inline int createWithAttribute (CellSpurs* spurs, const CellSpursTasksetAttribute* attr) const
				{
					return cellSpursCreateTasksetWithAttribute(spurs, getObject(), attr);
				}
			inline int create(CellSpurs* spurs, uint64_t argTaskset, const uint8_t priority[8], unsigned int max_contention) const
				{
					return cellSpursCreateTaskset(spurs,  getObject(), argTaskset, priority, max_contention);
				}
			inline int shutdown(void) const
				{
					return cellSpursShutdownTaskset( getObject());
				}
			inline int join(void) const
				{
					return cellSpursJoinTaskset( getObject());
				}
			
			inline int getTasksetId(CellSpursWorkloadId* wid) const
				{
					return cellSpursGetTasksetId(getObject(), wid);
				}
			inline int getSpursAddress(CellSpurs** ppSpurs) const 
				{
					return cellSpursTasksetGetSpursAddress(getObject(), ppSpurs);
				}
			inline int	createTaskWithAttribute (CellSpursTaskId* tid,	const CellSpursTaskAttribute* attr) const
				{
					return cellSpursCreateTaskWithAttribute(getObject(), tid, attr);
				}
			inline int createTask(CellSpursTaskId* tid, const void *eaElf,	const void *eaContext,
								  uint32_t sizeContext, const CellSpursTaskLsPattern* lspattern, const CellSpursTaskArgument* arg) const
				{
					return cellSpursCreateTask(getObject(), tid, eaElf, eaContext, sizeContext, lspattern, arg);
				}
			
			inline int sendSignal(CellSpursTaskId id) const
				{
					return cellSpursSendSignal(getObject(), id);
				}
			inline int setExceptionEventHandler(CellSpursTasksetExceptionEventHandler handler,	void* arg) const
				{
					return cellSpursTasksetSetExceptionEventHandler(getObject(), handler,  arg);
					
				}
			inline int unsetExceptionEventHandler(void) const
				{
					return cellSpursTasksetUnsetExceptionEventHandler(getObject());
				}
			inline int getTasksetInfo(CellSpursTasksetInfo* info) const
				{
					return cellSpursGetTasksetInfo(getObject(), info);
				}
			
#endif /*__PPU__*/

#ifdef  __SPU__
			inline void setObject(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}
			int shutdown(void)
				{
					return cellSpursShutdownTaskset(getObject());
				}

			int	createTaskWithAttribute(CellSpursTaskId* tid, const CellSpursTaskAttribute* attr)
				{
					return cellSpursCreateTaskWithAttribute (getObject(), tid, attr);
				}
			int createTask(CellSpursTaskId* tid, uint64_t eaElf,
						   uint64_t eaContext, uint32_t sizeContext, vec_uint4 lsPattern, qword argument)
				{
					return cellSpursCreateTask(getObject(),	tid, eaElf, eaContext, sizeContext,lsPattern,argument);
				}
			int sendSignal(CellSpursTaskId tid)
				{
					return cellSpursSendSignal(getObject(), tid);
				}
#endif /*__SPU__*/
		};


		class Taskset2Stub : public TasksetStub
		{
		public:
			static const uint32_t kSize  = CELL_SPURS_TASKSET2_SIZE;
			static const uint32_t kAlign = CELL_SPURS_TASKSET2_ALIGN;

#ifdef __PPU__
			inline void setObject(CellSpursTaskset2 *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline Taskset2 *getObject(void) const
				{
					return (Taskset2 *)(uintptr_t)object_ea;
				}

			inline int create(CellSpurs* pSpurs, const CellSpursTasksetAttribute2* pAttr)
				{
					return cellSpursCreateTaskset2(pSpurs, getObject(), pAttr);
				}
			inline int destroy(void)
				{
					return cellSpursDestroyTaskset2(getObject());
				}

			inline int createTask2 (CellSpursTaskId* id, const void *eaElf, CellSpursTaskArgument *argument, const CellSpursTaskAttribute2* attr)
				{
					return cellSpursCreateTask2 (getObject(),  id,	eaElf, argument, attr);
				}
			inline int createTask2(CellSpursTaskId* id,
								   const CellSpursTaskBinInfo *binInfo,	 const CellSpursTaskArgument *argument, void* contextBuffer, const char* name,
								   void *__reserved__=NULL)
				{
					return cellSpursCreateTask2WithBinInfo (getObject(),	id,
															binInfo,	 argument,  contextBuffer, name, __reserved__);
				}
			
			inline int createTask2WithBinInfo (CellSpursTaskId* id,
											   const CellSpursTaskBinInfo *binInfo,	 const CellSpursTaskArgument *argument, void* contextBuffer, const char* name,
											   void *__reserved__=NULL)
				{
					return cellSpursCreateTask2WithBinInfo (getObject(),	id,
															binInfo,	 argument,  contextBuffer, name, __reserved__);
				}

			inline int joinTask2(CellSpursTaskId idTask, int *exitCode)
				{
					return cellSpursJoinTask2(getObject(), idTask, exitCode);
				}
			inline int tryJoinTask2(CellSpursTaskId idTask, int *exitCode)
				{
					return cellSpursTryJoinTask2(getObject(), idTask, exitCode);
				}

#endif /*__PPU__*/

#ifdef  __SPU__
			int	createTask2(CellSpursTaskId* tid, uint64_t eaElf,  qword argument,  const CellSpursTaskAttribute2* attr)
				{
					return cellSpursCreateTask2(getObject(), tid, eaElf, argument, attr);
				}
			int	createTask2(CellSpursTaskId* tid,  uint64_t eaTaskBinInfo, 
							qword argument,  uint64_t eaContext, const char*name, void *__reserved__=NULL)
				{
					return cellSpursCreateTask2WithBinInfo (getObject(), tid, eaTaskBinInfo,  
															argument,   eaContext, name, __reserved__);
				}

			int	createTask2WithBinInfo (CellSpursTaskId* tid, uint64_t eaTaskBinInfo, 
										   qword argument,  uint64_t eaContext, const char*name, void *__reserved__=NULL)
				{
					return cellSpursCreateTask2WithBinInfo (getObject(), tid, eaTaskBinInfo,  
															argument,   eaContext, name, __reserved__);
				}


			int	joinTask2 (CellSpursTaskId idTask, int* retval)
				{
					return cellSpursJoinTask2 (getObject(),	idTask, retval);
				}
			int	tryJoinTask2 (CellSpursTaskId idTask, int* retval)
				{
					return cellSpursTryJoinTask2 (getObject(),	idTask, retval);
				}
#endif /*__SPU__*/
			
			
		};
		  


		class EventFlag : public CellSpursEventFlag
		{
		public:
			static const uint32_t kAlign      = CELL_SPURS_EVENT_FLAG_ALIGN;
			static const uint32_t kSize       = CELL_SPURS_EVENT_FLAG_SIZE;

			static const CellSpursEventFlagWaitMode  kOr          = CELL_SPURS_EVENT_FLAG_OR;
			static const CellSpursEventFlagWaitMode  kAnd         = CELL_SPURS_EVENT_FLAG_AND;

			static const CellSpursEventFlagClearMode kClearAuto   = CELL_SPURS_EVENT_FLAG_CLEAR_AUTO;
			static const CellSpursEventFlagClearMode kClearManual = CELL_SPURS_EVENT_FLAG_CLEAR_MANUAL;

			static const CellSpursEventFlagDirection kSpu2Spu     = CELL_SPURS_EVENT_FLAG_SPU2SPU;
			static const CellSpursEventFlagDirection kSpu2Ppu     = CELL_SPURS_EVENT_FLAG_SPU2PPU;
			static const CellSpursEventFlagDirection kPpu2Spu     = CELL_SPURS_EVENT_FLAG_PPU2SPU;
			static const CellSpursEventFlagDirection kAny2Any     = CELL_SPURS_EVENT_FLAG_ANY2ANY;

#ifdef __PPU__
			inline static int initializeIWL(CellSpurs* spurs, CellSpursEventFlag* ev, 
											CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction)
				{
					return cellSpursEventFlagInitializeIWL(spurs, ev, clearMode, direction);
				}
			
			inline static int initialize(CellSpursTaskset* taskset, CellSpursEventFlag* ev, 
										 CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction)
				{
					return cellSpursEventFlagInitialize(taskset, ev, clearMode, direction);					
				}
			inline int attachLv2EventQueue(void)
				{
					return cellSpursEventFlagAttachLv2EventQueue(this);
				}

			inline int detachLv2EventQueue(void)
				{
					return cellSpursEventFlagDetachLv2EventQueue(this);
				}
			inline int set(uint16_t bits)
				{
					return cellSpursEventFlagSet(this, bits);
				}
			inline int clear(uint16_t bits)
				{
					return cellSpursEventFlagClear(this, bits);
				}
			inline int wait(uint16_t *bits, CellSpursEventFlagWaitMode mode)
				{
					return cellSpursEventFlagWait(this, bits, mode);
				}
			inline int tryWait(uint16_t *bits, CellSpursEventFlagWaitMode mode)
				{
					return cellSpursEventFlagTryWait(this, bits, mode);
				}

			inline int getDirection(CellSpursEventFlagDirection* d) const 
				{
					return cellSpursEventFlagGetDirection(this, d);
				}
			
			inline int getClearMode(CellSpursEventFlagClearMode*  d) const
				{
					return cellSpursEventFlagGetClearMode(this, d);
				}

			inline int getTasksetAddress(CellSpursTaskset** tea) const
				{
					return cellSpursEventFlagGetTasksetAddress(this, tea);
				}
#endif /*__PPU__*/
		};



		class EventFlagStub 
		{
		protected:
			uint64_t object_ea;
		public:
			static const uint32_t kAlign=CELL_SPURS_EVENT_FLAG_ALIGN;
			static const uint32_t kSize =CELL_SPURS_EVENT_FLAG_SIZE;

			static const CellSpursEventFlagWaitMode  kOr          = CELL_SPURS_EVENT_FLAG_OR;
			static const CellSpursEventFlagWaitMode  kAnd         = CELL_SPURS_EVENT_FLAG_AND;

			static const CellSpursEventFlagClearMode kClearAuto   = CELL_SPURS_EVENT_FLAG_CLEAR_AUTO;
			static const CellSpursEventFlagClearMode kClearManual = CELL_SPURS_EVENT_FLAG_CLEAR_MANUAL;

			static const CellSpursEventFlagDirection kSpu2Spu     = CELL_SPURS_EVENT_FLAG_SPU2SPU;
			static const CellSpursEventFlagDirection kSpu2Ppu     = CELL_SPURS_EVENT_FLAG_SPU2PPU;
			static const CellSpursEventFlagDirection kPpu2Spu     = CELL_SPURS_EVENT_FLAG_PPU2SPU;
			static const CellSpursEventFlagDirection kAny2Any     = CELL_SPURS_EVENT_FLAG_ANY2ANY;

#ifdef __PPU__
			inline void setObject(CellSpursEventFlag *ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline EventFlag *getObject(void) const
				{
					return (EventFlag *)(uintptr_t)object_ea;
				}

			inline int initializeIWL(CellSpurs* spurs, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction) const
				{
					return cellSpursEventFlagInitializeIWL(spurs, getObject(), clearMode, direction);
				}
			
			inline int initialize(CellSpursTaskset* taskset, CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction) const
				{
					return cellSpursEventFlagInitialize(taskset, getObject(), clearMode, direction);					
				}
			inline int attachLv2EventQueue(void) const
				{
					return cellSpursEventFlagAttachLv2EventQueue(getObject());
				}

			inline int detachLv2EventQueue(void) const
				{
					return cellSpursEventFlagDetachLv2EventQueue(getObject());
				}

			inline int getTasksetAddress(CellSpursTaskset** tea) const
				{
					return cellSpursEventFlagGetTasksetAddress(getObject(), tea);
				}

#endif /*__PPU__*/

#ifdef  __SPU__
			inline void setObject(uint64_t ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}

			inline int initialize(CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction) const
				{
					return cellSpursEventFlagInitialize(getObject(), clearMode, direction);					
				}

			inline int initializeIWL(CellSpursEventFlagClearMode clearMode, CellSpursEventFlagDirection direction) const
				{
					return cellSpursEventFlagInitializeIWL(getObject(), clearMode, direction);
				}
			
			inline int getTasksetAddress(uint64_t* tea) const
				{
					return cellSpursEventFlagGetTasksetAddress(getObject(), tea);
				}
#endif /*__SPU__*/

			inline int set(uint16_t bits) const
				{
					return cellSpursEventFlagSet(getObject(), bits);
				}
			inline int clear(uint16_t bits) const
				{
					return cellSpursEventFlagClear(getObject(), bits);
				}
			inline int wait(uint16_t *bits, CellSpursEventFlagWaitMode mode) const
				{
					return cellSpursEventFlagWait(getObject(), bits, mode);
				}
			inline int tryWait(uint16_t *bits, CellSpursEventFlagWaitMode mode) const
				{
					return cellSpursEventFlagTryWait(getObject(), bits, mode);
				}

			inline int getDirection(CellSpursEventFlagDirection* d) const
				{
					return cellSpursEventFlagGetDirection(getObject(), d);
				}
			
			inline int getClearMode(CellSpursEventFlagClearMode*  d) const
				{
					return cellSpursEventFlagGetClearMode(getObject(), d);
				}


		};







		class Barrier : public CellSpursBarrier
		{
		public:
			static const uint32_t kAlign=CELL_SPURS_BARRIER_ALIGN;
			static const uint32_t kSize =CELL_SPURS_BARRIER_SIZE;

#ifdef __PPU__
			inline static int initialize(CellSpursTaskset* taskset, CellSpursBarrier* barrier, unsigned int total)
				{
					return cellSpursBarrierInitialize(taskset, barrier, total);
				}
			inline int getTasksetAddress(CellSpursTaskset** taskset) const
				{
					return cellSpursBarrierGetTasksetAddress(this, taskset);
				}
#endif /*__PPU__*/

		};


		class BarrierStub
		{
		protected:
			uint64_t object_ea;
		public:
			static const uint32_t kAlign=CELL_SPURS_BARRIER_ALIGN;
			static const uint32_t kSize =CELL_SPURS_BARRIER_SIZE;

#ifdef __PPU__
			
			inline void setObject(CellSpursBarrier *ev)
				{
					object_ea = (uintptr_t)ev;
				}
			inline Barrier *getObject(void) const
				{
					return (Barrier *)(uintptr_t)object_ea;
				}
			inline int initialize(CellSpursTaskset* taskset, unsigned int total) const
				{
					return cellSpursBarrierInitialize(taskset, getObject(), total);
				}
			inline int getTasksetAddress(CellSpursTaskset** taskset) const
				{
					return cellSpursBarrierGetTasksetAddress(getObject(), taskset);
				}
			
#endif /*__PPU__*/
			
#ifdef  __SPU__
			inline void setObject(uint64_t br)
				{
					object_ea = (uintptr_t)br;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}
			
			inline int initialize(unsigned int total) const
				{
					return cellSpursBarrierInitialize(getObject(), total);
				}
			inline int getTasksetAddress(uint64_t* pEaTaskset) const
				{
					return cellSpursBarrierGetTasksetAddress(getObject(), pEaTaskset);
				}
			inline int tryNotify(void) const
				{
					return cellSpursBarrierTryNotify(getObject());
				}
			inline int notify(void) const
				{
					return cellSpursBarrierNotify(getObject());
				}
			inline int tryWait(void) const
				{
					return cellSpursBarrierTryWait(getObject());
				}
			inline int wait(void) const
				{
					return cellSpursBarrierWait(getObject());
				}
#endif /*__SPU__*/

		};

		class Queue : public CellSpursQueue
		{
		public:
			static const uint32_t kAlign = CELL_SPURS_QUEUE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_QUEUE_SIZE;
			static const int32_t kSpu2Spu = CELL_SPURS_QUEUE_SPU2SPU;
			static const int32_t kSpu2Ppu = CELL_SPURS_QUEUE_SPU2PPU;
			static const int32_t kPpu2Spu = CELL_SPURS_QUEUE_PPU2SPU;

#ifdef __PPU__


			static inline int initialize(CellSpursTaskset* taskset, CellSpursQueue* queue, const void *buffer, unsigned int size, 
										 unsigned int depth, CellSpursQueueDirection direction)
				{
					return cellSpursQueueInitialize(taskset,  queue, buffer,  size, depth, direction);
				}
			
			static inline int initializeIWL(CellSpurs* spurs, CellSpursQueue* queue, const void *buffer, unsigned int size, 
											unsigned int depth, CellSpursQueueDirection direction)
				{
					return cellSpursQueueInitializeIWL(spurs, queue, buffer, size, depth, direction);
				}

			inline int tryPush(const void *buffer)
				{
					return cellSpursQueueTryPush(this, buffer);
				}

			inline int push(const void *buffer)
				{
					return cellSpursQueuePush(this, buffer);
				}

			inline int tryPop(void* buffer)
				{
					return cellSpursQueueTryPop(this, buffer);
				}

			inline int pop(void *buffer)
				{
					return cellSpursQueuePop(this, buffer);
				}

			inline int tryPeek(void *buffer)
				{
					return cellSpursQueueTryPeek(this, buffer);
				}
			
			inline int peek(void *buffer)
				{
					return cellSpursQueuePeek(this, buffer);
				}
			inline int attachLv2EventQueue(void)
				{
					return cellSpursQueueAttachLv2EventQueue(this);
				}
			inline int detachLv2EventQueue(void)
				{
					return cellSpursQueueDetachLv2EventQueue(this);
				}
			
			inline int size(unsigned int *arg_size)
				{
					return cellSpursQueueSize(this, arg_size);
				}
			inline int depth(unsigned int *arg_depth)
				{
					return cellSpursQueueDepth(this, arg_depth);
				}
			inline int clear(void)
				{
					return cellSpursQueueClear(this);
				}
			inline int getDirection(CellSpursQueueDirection* direction)
				{
					return cellSpursQueueGetDirection(this, direction);
				}
			inline int getEntrySize(unsigned int *entry_size)
				{
					return cellSpursQueueGetEntrySize(this, entry_size);
				}
			inline int getTasksetAddress(CellSpursTaskset** pTaskset) const
				{
					return cellSpursQueueGetTasksetAddress(this, pTaskset);
				}
			
			
#endif /*__PPU__*/
		};


		class QueueStub
		{
		protected:
			uint64_t object_ea;
		public:
			static const uint32_t kAlign = CELL_SPURS_QUEUE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_QUEUE_SIZE;
			static const int32_t kSpu2Spu = CELL_SPURS_QUEUE_SPU2SPU;
			static const int32_t kSpu2Ppu = CELL_SPURS_QUEUE_SPU2PPU;
			static const int32_t kPpu2Spu = CELL_SPURS_QUEUE_PPU2SPU;
#ifdef __PPU__
			
			inline void setObject(CellSpursQueue *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline Queue *getObject(void) const
				{
					return (Queue*)(uintptr_t)object_ea;
				}

			inline int initialize(CellSpursTaskset* taskset, const void *buffer, unsigned int _size, 
								  unsigned int _depth, CellSpursQueueDirection direction) const
				{
					return cellSpursQueueInitialize(taskset,  getObject(), buffer,  _size, _depth, direction);
				}
			
			inline int initializeIWL(CellSpurs* spurs, const void *buffer, unsigned int _size, 
									 unsigned int _depth, CellSpursQueueDirection direction) const
				{
					return cellSpursQueueInitializeIWL(spurs, getObject(), buffer, _size, _depth, direction);
				}

			inline int tryPush(const void *buffer) const
				{
					return cellSpursQueueTryPush(getObject(), buffer);
				}

			inline int push(const void *buffer) const
				{
					return cellSpursQueuePush(getObject(), buffer);
				}

			inline int tryPop(void* buffer) const
				{
					return cellSpursQueueTryPop(getObject(), buffer);
				}

			inline int pop(void *buffer) const
				{
					return cellSpursQueuePop(getObject(), buffer);
				}

			inline int tryPeek(void *buffer) const
				{
					return cellSpursQueueTryPeek(getObject(), buffer);
				}
			
			inline int peek(void *buffer) const
				{
					return cellSpursQueuePeek(getObject(), buffer);
				}
			inline int attachLv2EventQueue(void) const
				{
					return cellSpursQueueAttachLv2EventQueue(getObject());
				}
			inline int detachLv2EventQueue(void) const
				{
					return cellSpursQueueDetachLv2EventQueue(getObject());
				}
			
			inline int size(unsigned int *arg_size) const
				{
					return cellSpursQueueSize(getObject(), arg_size);
				}
			inline int depth(unsigned int *arg_depth) const
				{
					return cellSpursQueueDepth(getObject(), arg_depth);
				}
			inline int clear(void) const
				{
					return cellSpursQueueClear(getObject());
				}
			inline int getDirection(CellSpursQueueDirection* direction) const
				{
					return cellSpursQueueGetDirection(getObject(), direction);
				}
			inline int getEntrySize(unsigned int *entry_size) const
				{
					return cellSpursQueueGetEntrySize(getObject(), entry_size);
				}
			inline int getTasksetAddress(CellSpursTaskset** pTaskset) const
				{
					return cellSpursQueueGetTasksetAddress(getObject(), pTaskset);
				}

#endif /*__PPU__*/
			
#ifdef  __SPU__
			inline void setObject(uint64_t br)
				{
					object_ea = (uintptr_t)br;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}

			inline int initialize(uint64_t buf,unsigned int _size,unsigned int _depth,CellSpursQueueDirection direction) const
				{
					return cellSpursQueueInitialize(getObject(),buf,_size,_depth,direction);
				}

			inline int initializeIWL(uint64_t buf,unsigned int _size,unsigned int _depth,CellSpursQueueDirection direction) const
				{
					return cellSpursQueueInitializeIWL(getObject(),buf,_size,_depth,direction);
				}

			inline int tryPushBegin(const void *buf,unsigned int tag) const
				{
					return cellSpursQueueTryPushBegin(getObject(),buf,tag);
				}

			inline int pushBegin(const void *buf,unsigned int tag) const
				{
					return cellSpursQueuePushBegin(getObject(),buf,tag);
				}

			inline int tryPopBegin(void *buf,unsigned int tag) const
				{
					return cellSpursQueueTryPopBegin(getObject(),buf,tag);
				}

			inline int popBegin(void *buf,unsigned int tag) const
				{
					return cellSpursQueuePopBegin(getObject(),buf,tag);
				}

 			inline int tryPeekBegin(void *buf,unsigned int tag) const
				{
					return cellSpursQueueTryPeekBegin(getObject(),buf,tag);
				}

			inline int peekBegin(void *buf,unsigned int tag) const
				{
					return cellSpursQueuePeekBegin(getObject(),buf,tag);
				}

			inline int popEnd(unsigned int tag) const
				{
					return cellSpursQueuePopEnd(getObject(),tag);
				}

			inline int peekEnd(unsigned int tag) const
				{
					return cellSpursQueuePeekEnd(getObject(),tag);
				}
			
			inline int pushEnd(unsigned int tag) const
				{
					return cellSpursQueuePushEnd(getObject(), tag);
				}

			inline int size(unsigned int *psize) const
				{
					return cellSpursQueueSize(getObject(), psize);
				}

			inline int depth(unsigned int *psize) const
				{
					return cellSpursQueueDepth(getObject(), psize);
				}

			inline int clear(void) const
				{
					return cellSpursQueueClear(getObject());
				}

			inline int getDirection(CellSpursQueueDirection* direction) const
				{
					return cellSpursQueueGetDirection(getObject(),  direction);
				}

			inline int getEntrySize(unsigned int *entry_size) const
				{
					return cellSpursQueueGetEntrySize(getObject(), entry_size);
				}

			inline int getTasksetAddress(uint64_t* pEaTaskset) const
				{
					return cellSpursQueueGetTasksetAddress(getObject(), pEaTaskset);
				}

#endif /*__SPU__*/

		};


#ifdef __PPU__

		class LFQueue : public CellSpursLFQueue
		{

		public:
			static const uint32_t kAlign = CELL_SPURS_LFQUEUE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_LFQUEUE_SIZE;
			static const int32_t kSpu2Ppu = CELL_SPURS_LFQUEUE_SPU2PPU;
			static const int32_t kPpu2Spu = CELL_SPURS_LFQUEUE_PPU2SPU;
			static const int32_t kSpu2Spu = CELL_SPURS_LFQUEUE_SPU2SPU;
			static const int32_t kAny2Any = CELL_SPURS_LFQUEUE_ANY2ANY;

			inline int attachLv2EventQueue(void)
				{
					return cellSpursLFQueueAttachLv2EventQueue(this);
				}
			inline int detachLv2EventQueue(void)
				{
					return cellSpursLFQueueDetachLv2EventQueue(this);
				}

			inline int getTasksetAddress(CellSpursTaskset** taskset) const
				{
					return cellSpursLFQueueGetTasksetAddress(this, taskset);
				}
			
			static inline int initialize(CellSpursTaskset *pTaskset,	 CellSpursLFQueue *pQueue,
										 const void *buffer,	 unsigned int size,		 unsigned int depth,
										 CellSpursLFQueueDirection direction)
				{
					return cellSpursLFQueueInitialize(pTaskset,  pQueue,  buffer,  size,  depth,  direction);
				}
			static inline int initializeIWL(CellSpurs *pSpurs,
											CellSpursLFQueue *pQueue,
											const void *buffer,
											unsigned int size,
											unsigned int depth,
											CellSpursLFQueueDirection direction)
				{
					return cellSpursLFQueueInitializeIWL(pSpurs, pQueue, buffer,
														 size,  depth, direction);
				}
			inline int size(unsigned int *psize)
				{
					return cellSpursLFQueueSize(this, psize);
				}
			inline int depth(unsigned int *pdepth)
				{
					return cellSpursLFQueueDepth(this, pdepth);
				}
			inline int getDirection(CellSpursLFQueueDirection* pDirection) const
				{
					return cellSpursLFQueueGetDirection(this, pDirection);
				}
			inline int getEntrySize(unsigned int* pSize) const
				{
					return cellSpursLFQueueGetEntrySize(this, pSize);
				}
			inline int clear(void)
				{
					return cellSpursLFQueueClear(this);
				}
			inline int push(const void *buffer)
				{
					return cellSpursLFQueuePush(this, buffer);
				}
			inline int tryPush(const void *buffer)
				{
					return cellSpursLFQueueTryPush(this, buffer);
				}
			inline int pop(void *buffer)
				{
					return cellSpursLFQueuePop(this, buffer);
				}
			inline int tryPop(void *buffer)
				{
					return cellSpursLFQueueTryPop(this, buffer);
				}
		};
#endif /* __PPU__ */	


#ifdef __SPU__
		class LFQueuePushContainer : public CellSpursLFQueuePushContainer
		{
		public:
			static void initialize(CellSpursLFQueuePushContainer *container, const void *buffer,	const unsigned int tag)
				{
					cellSpursLFQueuePushContainerInitialize(container,buffer,tag);
				}
		};
		
		class LFQueuePopContainer : public CellSpursLFQueuePopContainer
		{
		public:
			static void initialize(CellSpursLFQueuePopContainer *container, void *buffer,  const unsigned int tag)
				{
					cellSpursLFQueuePopContainerInitialize(container, buffer, tag);
				}
		};

#endif /* __SPU__ */	

		class LFQueueStub
		{
		protected:
			uint64_t object_ea;
		public:
			static const uint32_t kAlign = CELL_SPURS_LFQUEUE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_LFQUEUE_SIZE;
			static const int32_t kSpu2Ppu = CELL_SPURS_LFQUEUE_SPU2PPU;
			static const int32_t kPpu2Spu = CELL_SPURS_LFQUEUE_PPU2SPU;
			static const int32_t kSpu2Spu = CELL_SPURS_LFQUEUE_SPU2SPU;
			static const int32_t kAny2Any = CELL_SPURS_LFQUEUE_ANY2ANY;
#ifdef __PPU__
			
			inline void setObject(CellSpursLFQueue *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline LFQueue *getObject(void) const
				{
					return (LFQueue*)(uintptr_t)object_ea;
				}
			inline int attachLv2EventQueue(void) const
				{
					return cellSpursLFQueueAttachLv2EventQueue(getObject());
				}
			inline int detachLv2EventQueue(void) const
				{
					return cellSpursLFQueueDetachLv2EventQueue(getObject());
				}

			inline int getTasksetAddress(CellSpursTaskset** taskset) const
				{
					return cellSpursLFQueueGetTasksetAddress(getObject(), taskset);
				}
			
			inline int initialize(CellSpursTaskset *pTaskset,   
										 const void *buffer,	 unsigned int _size,  unsigned int _depth,
										 CellSpursLFQueueDirection direction) const
				{
					return cellSpursLFQueueInitialize(pTaskset,  getObject(),  buffer,  _size,  _depth,  direction);
				}
			inline int initializeIWL(CellSpurs *pSpurs,
											
											const void *buffer,
											unsigned int _size,
											unsigned int _depth,
											CellSpursLFQueueDirection direction) const
				{
					return cellSpursLFQueueInitializeIWL(pSpurs, getObject(), buffer,
														 _size,  _depth, direction);
				}
			inline int size(unsigned int *psize) const
				{
					return cellSpursLFQueueSize(getObject(), psize);
				}
			inline int depth(unsigned int *pdepth) const
				{
					return cellSpursLFQueueDepth(getObject(), pdepth);
				}
			inline int getDirection(CellSpursLFQueueDirection* pDirection) const
				{
					return cellSpursLFQueueGetDirection(getObject(), pDirection);
				}
			inline int getEntrySize(unsigned int* pSize) const
				{
					return cellSpursLFQueueGetEntrySize(getObject(), pSize);
				}
			inline int clear(void) const
				{
					return cellSpursLFQueueClear(getObject());
				}
			inline int push(const void *buffer) const
				{
					return cellSpursLFQueuePush(getObject(), buffer);
				}
			inline int tryPush(const void *buffer) const
				{
					return cellSpursLFQueueTryPush(getObject(), buffer);
				}
			inline int pop(void *buffer) const
				{
					return cellSpursLFQueuePop(getObject(), buffer);
				}
			inline int tryPop(void *buffer) const
				{
					return cellSpursLFQueueTryPop(getObject(), buffer);
				}

#endif /* __PPU__ */
#ifdef  __SPU__
			inline void setObject(uint64_t br)
				{
					object_ea = (uintptr_t)br;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}

			inline int initialize(uint64_t buffer,	   unsigned int _size,   unsigned int _depth,	   CellSpursLFQueueDirection direction) const
				{
					return cellSpursLFQueueInitialize(getObject(), buffer, _size, _depth, direction);
				}
			inline int initializeIWL(uint64_t buffer,  unsigned int _size,  unsigned int _depth,	  CellSpursLFQueueDirection direction) const
				{
					return cellSpursLFQueueInitializeIWL(getObject(), buffer, _size, _depth, direction);
				}
			inline int getTasksetAddress(uint64_t* pEaTaskset) const
				{
					return cellSpursLFQueueGetTasksetAddress(getObject(), pEaTaskset);
				}

			inline int pushEnd(CellSpursLFQueuePushContainer *pContainer) const
				{
					return cellSpursLFQueuePushEnd(getObject(), pContainer);
				}

			inline int popEnd(CellSpursLFQueuePopContainer *pContainer) const
				{
					return cellSpursLFQueuePopEnd(getObject(), pContainer);
				}

			inline int size(unsigned int *_size) const
				{
					return cellSpursLFQueueSize(getObject(), _size);
				}

			inline int depth(unsigned int *_depth) const
				{
					return cellSpursLFQueueDepth(getObject(), _depth);
				}

			inline int getDirection(CellSpursLFQueueDirection* pDirection) const
				{
					return cellSpursLFQueueGetDirection(getObject(), pDirection);
				}

			inline int getEntrySize(unsigned int* pSize) const
				{
					return cellSpursLFQueueGetEntrySize(getObject(), pSize);
				}

			inline int clear(void) const
				{
					return cellSpursLFQueueClear(getObject());
				}

			inline int pushBegin(CellSpursLFQueuePushContainer *pContainer) const
				{
					return cellSpursLFQueuePushBegin(getObject(), pContainer);
				}

			inline int tryPushBegin(CellSpursLFQueuePushContainer *pContainer) const
				{
					return cellSpursLFQueueTryPushBegin(getObject(), pContainer);
				}

			inline int popBegin(CellSpursLFQueuePopContainer *pContainer) const
				{
					return cellSpursLFQueuePopBegin(getObject(), pContainer);
				}

			inline int tryPopBegin(CellSpursLFQueuePopContainer *pContainer) const
				{
					return cellSpursLFQueueTryPopBegin(getObject(), pContainer);
				}
#endif /*__SPU__*/
		};

		class Semaphore : public CellSpursSemaphore
		{

		public:
			static const uint32_t kAlign = CELL_SPURS_SEMAPHORE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_SEMAPHORE_SIZE;

#ifdef  __PPU__
			static int initialize(CellSpursTaskset* taskset, CellSpursSemaphore* sem, int total)
				{
					return cellSpursSemaphoreInitialize(taskset, sem,total);
				}
			static int initializeIWL(CellSpurs* spurs,     CellSpursSemaphore* sem, int total)
				{
					return cellSpursSemaphoreInitializeIWL(spurs,   sem, total);
				}
			int getTasksetAddress(CellSpursTaskset** pTaskset) const
				{
					return cellSpursSemaphoreGetTasksetAddress(this, pTaskset);
				}
#endif /* __PPU__ */
		};

		class SemaphoreStub
		{
		protected:
			uint64_t object_ea;
		public:
			static const uint32_t kAlign = CELL_SPURS_SEMAPHORE_ALIGN;
			static const uint32_t kSize  = CELL_SPURS_SEMAPHORE_SIZE;

#ifdef  __PPU__
			inline void setObject(CellSpursSemaphore *obj)
				{
					object_ea = (uintptr_t)obj;
				}
			inline Semaphore *getObject(void) const
				{
					return (Semaphore*)(uintptr_t)object_ea;
				}

			inline int initialize(CellSpursTaskset* taskset, int total) const
				{
					return cellSpursSemaphoreInitialize(taskset, getObject(),total);
				}
			inline int initializeIWL(CellSpurs* spurs,     int total) const
				{
					return cellSpursSemaphoreInitializeIWL(spurs,   getObject(), total);
				}
			inline int getTasksetAddress(CellSpursTaskset** pTaskset) const
				{
					return cellSpursSemaphoreGetTasksetAddress(getObject(), pTaskset);
				}

#endif /*__PPU__*/
#ifdef  __SPU__
			inline void setObject(uint64_t br)
				{
					object_ea = (uintptr_t)br;
				}
			inline uint64_t getObject(void) const
				{
					return (uint64_t)(uintptr_t)object_ea;
				}

			inline int initialize(int total) const
				{
					return cellSpursSemaphoreInitialize(getObject(), total);
				}

			inline int initializeIWL(int total) const
				{
					return cellSpursSemaphoreInitializeIWL(getObject(), total);
				}
			inline int p(void) const
				{
					return cellSpursSemaphoreP(getObject());
				}
			inline int v(void) const
				{
					return cellSpursSemaphoreV(getObject());
				}
			inline int getTasksetAddress(uint64_t *pEaTaskset) const
				{
					return cellSpursSemaphoreGetTasksetAddress(getObject(), pEaTaskset);
				}

#endif /*__SPU__*/
		};
	}
}


#endif /* __CELL_SPURS_TASK_TYPES_CPP_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
