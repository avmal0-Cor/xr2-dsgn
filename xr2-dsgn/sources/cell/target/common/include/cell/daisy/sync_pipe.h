/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* sync_pipe.h
 */

#ifndef __CELL_DAISY_SYNC_PIPE_H__
#define __CELL_DAISY_SYNC_PIPE_H__

#include <cell/daisy/pipe.h>
#include <cell/daisy/sync_pipe_interlock.h>
#include <cell/daisy/seq_data.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_SYNC_PIPE
#include <cell/daisy/daisy_debug.h>
#endif

#undef CELL_DAISY_BOOKMARK
#define CELL_DAISY_BOOKMARK(x)
#ifdef CELL_DAISY_BOOKMARK_PIPE
#define __CELL_DAISY_BOOKMARK_PREFIX__ CELL_DAISY_BOOKMARK_PREFIX_PIPE
#include <cell/daisy/daisy_bookmark.h>
#endif

namespace cell {
	namespace Daisy {
		namespace ScatterPipe {

			template<class tBuffer, class tQueueControl, BufferMode tMode = COPY>
			class InPort:
				public ScatterGatherInterlock,
				public Pipe::InPort<tBuffer, tQueueControl, tMode>
			{
		
			public:
      
				typedef typename tBuffer::DataType Type;

				typedef typename tBuffer::DataType GlueDataType;

				const char  *getClassName(){
#ifdef CELL_DAISY_DEBUG_SYNC_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										   "ScatterPipe::InPort(%s,%s)",
										   this->mBuffer.getClassName(),
										   this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}
#ifdef __SPU__
				explicit InPort(tBuffer& buffer, tQueueControl& queueControl, uint64_t shmEa,
								uint64_t eaSignal = 0,
								uint32_t (*fpGetId)(void) = 0,
								int (*fpWaitSignal)(void) = 0,
								int bookmarkId = 0):
					ScatterGatherInterlock                      (shmEa, tQueueControl::sSize,
																 eaSignal, fpGetId, fpWaitSignal),
					Pipe::InPort<tBuffer, tQueueControl, tMode> (buffer, queueControl, bookmarkId)
					{}
#else
				explicit InPort(tBuffer& buffer, tQueueControl& queueControl, AtomicInterlock *shmEa,
								sys_spu_thread_t *ids = 0,
								unsigned int numSpus = 0,
								uint8_t spup = 0,
								int bookmarkId = 0):
					ScatterGatherInterlock                      (shmEa, tQueueControl::sSize,
																 ids, numSpus, spup),
					Pipe::InPort<tBuffer, tQueueControl, tMode> (buffer, queueControl, bookmarkId)
					{}
#endif

				int beginPush(Type * __restrict__ data);

				int beginPush();
      
				int tryBeginPush(Type * __restrict__ data);

				int tryBeginPush();
      
			}; /* class InPort */
    
			/* beginPush (tMode == COPY)
			 * 1.get tail_ptr
			 * 2.initiate transfer
			 * 3.beginCompleteProduce() (tMode == COPY)
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer,tQueueControl,tMode>::beginPush(Type * __restrict__ data)
			{

				CELL_DAISY_ASSERT_COPY;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr= this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextTailPointer(STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
				}

				int ret = probe(STALL);
				if (__builtin_expect(ret < 0, 0)) return ret;
                
				this->mBuffer.beginWrite(ptr, data);
				CELL_DAISY_DEBUG_PRINTF(
					"%s(beginPush): mBuffer.beginWrite(%d, 0x%p)\n",
					getClassName(),ptr, data
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */
				
				proceedSequenceNumber(); /* proceed mSequenceNumber for next time */

				CELL_DAISY_BOOKMARK(0);
				return CELL_OK;
			}
    
			/* beginPush (tMode == REFERENCE)
			 * 1.get tail_ptr
			 * 2.initiate transfer
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer,tQueueControl,tMode>::beginPush()
			{

				CELL_DAISY_ASSERT_REFERENCE;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr= this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextTailPointer(STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
				}

				int ret = probe(STALL);
				if (__builtin_expect(ret < 0, 0)) return ret;

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				proceedSequenceNumber(); /* proceed mSequenceNumber for next time */

				CELL_DAISY_BOOKMARK(1);
				return CELL_OK;
			}

			/* tryBeginPush (tMode == COPY)
			 * 1.get tail_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 * 3.beginCompleteProduce() (tMode == COPY)
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl, tMode>::tryBeginPush(Type * __restrict__ data)
			{
				register int stat;

				CELL_DAISY_ASSERT_COPY;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextTailPointer(NOT_STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
					if (__builtin_expect(ptr == PTR_UNAVAILABLE, 0)) {
						return QUEUE_IS_BUSY;
					}
					this->mQueueControl.setPointer(ptr);
				}

				int ret = probe(NOT_STALL);
				if (__builtin_expect(ret >= (int)tQueueControl::sSize, 0)) {
					return QUEUE_IS_BUSY;
				} else if (__builtin_expect(ret < 0, 0)) {
					return ret;
				}

				if (__builtin_expect((stat = this->mBuffer.tryBeginWrite(ptr, data)) != CELL_OK, 0)) {
					return stat; /* write cannot start */
				}

				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryBeginPush): mBuffer.tryBeginWrite(%d, 0x%p)\n",
					getClassName(),ptr,data
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}
				
				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				proceedSequenceNumber(); /* proceed mSequenceNumber for next time */

				CELL_DAISY_BOOKMARK(2);
				return CELL_OK;
			}

			/* tryBeginPush (tMode == REFERENCE)
			 * 1.get tail_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl, tMode>::tryBeginPush()
			{
				CELL_DAISY_ASSERT_REFERENCE;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr= this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextTailPointer(NOT_STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
					if (__builtin_expect(ptr == PTR_UNAVAILABLE, 0)) {
						return QUEUE_IS_BUSY;
					}
					this->mQueueControl.setPointer(ptr);
				}
				int ret = probe(NOT_STALL);
				if (__builtin_expect(ret >= (int)tQueueControl::sSize, 0)) {
					return QUEUE_IS_BUSY;
				} else if (__builtin_expect(ret < 0, 0)) {
					return ret;
				}

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				proceedSequenceNumber(); /* proceed mSequenceNumber for next time */

				CELL_DAISY_BOOKMARK(3);
				return CELL_OK;
			}
#ifdef __SPU__    
			template<class tBuffer, class tQueueControl>
			class OutPort:
				public Pipe::OutPort<tBuffer, tQueueControl, COPY>
			{

			public:

				typedef typename tBuffer::DataType Type;

				typedef TaggedData<typename tBuffer::DataType> GlueDataType;

				const char  *getClassName(){
#ifdef CELL_DAISY_DEBUG_SYNC_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										   "ScatterPipe::OutPort(%s,%s)",
										   this->mBuffer.getClassName(),
										   this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}
				explicit OutPort(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId=0):
					Pipe::OutPort<tBuffer, tQueueControl, COPY> (buffer, queueControl, bookmarkId)
					{}
				
				using Pipe::OutPort<tBuffer, tQueueControl, COPY>::beginPop;
				using Pipe::OutPort<tBuffer, tQueueControl, COPY>::tryBeginPop;
				using Pipe::OutPort<tBuffer, tQueueControl, COPY>::getCurrentReference;

				int beginPop(TaggedData<Type> *taggedData);

				int tryBeginPop(TaggedData<Type> *taggedData);

				volatile TaggedData<Type> *getCurrentReference(){return (volatile TaggedData<Type> *)NULL;}

			}; /* class OutPort */

			/* beginPop
			 * 1.get head_ptr
			 * 2.initiate transfer
			 * 3.beginCompleteConsume
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl>::beginPop(TaggedData<Type> *taggedData)
			{
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mIsTerminated) != 0u, 0)) {
					return TERMINATED;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextHeadPointer(STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
				}
				if (__builtin_expect(ptr == PTR_TERMINATED, 0)) {
					if (!this->hasPendingEntry()) {
						(void)this->terminate(); /* never receive error code */
					}
					return TERMINATED;
				}

				taggedData->mTag = ptr; /* set sequence number */
				this->mBuffer.beginRead(ptr, (Type *)(uintptr_t)&(taggedData->mContent));
				CELL_DAISY_DEBUG_PRINTF(
					"%s(beginScatterPop): mBuffer.beginRead(%d, 0x%p)\n",
					getClassName(),ptr,&(taggedData->mContent)
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.beginCompleteConsume(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				CELL_DAISY_BOOKMARK(6);
				return CELL_OK;
			}

			/* tryBeginPop
			 * 1.get head_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 * 3.beginCompleteConsume
			 * @retval CELL_OK, QUEUE_IS_BUSY
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl>::tryBeginPop(TaggedData<Type> *taggedData)
			{
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mIsTerminated) != 0u, 0)) {
					return TERMINATED;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE, 1)) {
					ptr = this->mQueueControl.getNextHeadPointer(NOT_STALL);
					if (__builtin_expect(ptr < 0, 0)) {
						return ptr;
					}
					if (__builtin_expect(ptr == PTR_UNAVAILABLE, 0)) {
						return QUEUE_IS_BUSY;
					}
				}
				if (__builtin_expect(ptr == PTR_TERMINATED, 0)) {
					if (!this->hasPendingEntry()) {
						(void)this->terminate(); /* never receive error code */
					}
					return TERMINATED;
				}

				taggedData->mTag = ptr; /* set sequence number */
				register int stat =
					this->mBuffer.tryBeginRead(ptr, (Type*)(uintptr_t)&(taggedData->mContent));
				if (__builtin_expect(stat != CELL_OK, 0)) {
					return stat; /* read cannot be started */
				}
				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryBeginScatterPop): mBuffer.tryBeginRead(%d, 0x%p)\n",
					getClassName(),ptr, &(taggedData->mContent)
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.beginCompleteConsume(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */
      
				CELL_DAISY_BOOKMARK(8);
				return CELL_OK;
			}
#endif /* __SPU__ */

		} /* namespace ScatterPipe */

		namespace GatherPipe {

#ifdef __SPU__
			template<class tBuffer, class tQueueControl>
			class InPort:
				public Pipe::InPort<tBuffer, tQueueControl, COPY>
			{
		
			public:
      
				typedef typename tBuffer::DataType Type;

				typedef TaggedData<typename tBuffer::DataType> GlueDataType;

				const char  *getClassName(){
#ifdef CELL_DAISY_DEBUG_SYNC_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										   "GatherPipe::InPort(%s,%s)",
										   this->mBuffer.getClassName(),
										   this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}

				explicit InPort(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId=0):
					Pipe::InPort<tBuffer, tQueueControl, COPY> (buffer, queueControl, bookmarkId)
					{}

				using Pipe::InPort<tBuffer, tQueueControl, COPY>::beginPush;
				using Pipe::InPort<tBuffer, tQueueControl, COPY>::tryBeginPush;

				int beginPush(TaggedData<Type> *taggedData);

				int tryBeginPush(TaggedData<Type> *taggedData);

				int endPush();
      
				int tryEndPush();

			}; /* class InPort */
    
			/* beginPush
			 * 1.get tail_ptr
			 * 2.initiate transfer
			 * 3.beginCompleteProduce()
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int InPort<tBuffer,tQueueControl>::beginPush(TaggedData<Type> *taggedData)
			{
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled) != 0u, 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}
				
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = taggedData->mTag;
				int ret = this->mQueueControl.setNextTailPointer(STALL, ptr);
				if (__builtin_expect(ret < 0, 0)) {	return ret; }

				this->mBuffer.beginWrite(ptr, (Type *)(uintptr_t)&(taggedData->mContent));
				CELL_DAISY_DEBUG_PRINTF(
					"%s(beginGatherPush): mBuffer.beginWrite(%d, 0x%p)\n",
					getClassName(),ptr, &(taggedData->mContent)
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */
				CELL_DAISY_BOOKMARK(0);
				return CELL_OK;
			}
    
			/* tryBeginPush
			 * 1.get tail_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 * 3.beginCompleteProduce()
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl>::tryBeginPush(TaggedData<Type> *taggedData)
			{
				register int stat;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = taggedData->mTag;
				stat = this->mQueueControl.setNextTailPointer(NOT_STALL, ptr);
				if (__builtin_expect(stat < 0, 0)) { return stat; }
				if (__builtin_expect(stat, 0)) {
					return QUEUE_IS_BUSY;
				}

				if (__builtin_expect((stat =
									  this->mBuffer.tryBeginWrite(ptr, (Type*)(uintptr_t)&(taggedData->mContent)))
									 != CELL_OK, 0)) {
					return stat; /* write cannot start */
				}

				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryBeginGatherPush): mBuffer.tryBeginWrite(%d, 0x%p)\n",
					getClassName(),ptr,&(taggedData->mContent)
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}
				
				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */

				CELL_DAISY_BOOKMARK(2);
				return CELL_OK;

			}

			/* endPush
			 * 1.transfer end
			 * 2.endCompleteProduce()
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl>::endPush()
			{
				if (__builtin_expect(!this->hasPendingEntry(),0)) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_NO_BEGIN);
					return CELL_DAISY_ERROR_NO_BEGIN; /* unexpected endPush */
				}

				register PointerType ptr;
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					ptr = this->mPendingFifo.popPtr();
				} else {
					ptr = this->mQueueControl.getPointerFromSequenceNumber(this->mLastPointer - (--this->mPendingCount));
				}

				this->mBuffer.endWrite(ptr);
				CELL_DAISY_DEBUG_PRINTF("%s(endPush): mBuffer.endWrite(%d)\n",
										getClassName(), ptr);
      
				this->mQueueControl.endCompleteProduce(ptr, true /* noQueueFull=true */);
				CELL_DAISY_BOOKMARK(4);

				return CELL_OK;
			}

			/* tryEndPush
			 * 1.transfer end if possible,otherwise abort
			 * 2.endCompleteProduce()
			 * @retval CELL_OK, QUEUE_IS_BUSY or CELL_DAISY_ERROR_NO_BEGIN 
			 */
			template<class tBuffer, class tQueueControl>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl>::tryEndPush()
			{

				if (__builtin_expect(!this->hasPendingEntry(), 0)) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_NO_BEGIN);
					return CELL_DAISY_ERROR_NO_BEGIN; /* unexpected tryEndPush() */
				}
	
				register PointerType ptr;
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					ptr = this->mPendingFifo.getFront();
				} else {
					ptr = this->mQueueControl.getPointerFromSequenceNumber(this->mLastPointer - (this->mPendingCount - 1));
				}

				register int stat = this->mBuffer.tryEndWrite(ptr);
				if (__builtin_expect(stat != CELL_OK, 0)) {
					return stat; /* write not completed */
				}
				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryEndPush): mBuffer.tryEndWrite(%d)\n",
					getClassName(),ptr
					);

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.popPtr();
				} else {
					this->mPendingCount--;
				}

				this->mQueueControl.endCompleteProduce(ptr, true /* noQueueFull=true */);

				CELL_DAISY_BOOKMARK(5);
				return CELL_OK;
			}
#endif /* __SPU__ */

			template<class tBuffer, class tQueueControl, BufferMode tMode = COPY>
			class OutPort:
				public ScatterGatherInterlock,
				public Pipe::OutPort<tBuffer, tQueueControl, tMode>
			{

			public:
      
				typedef typename tBuffer::DataType Type;

				typedef typename tBuffer::DataType GlueDataType;

				const char  *getClassName(){
#ifdef CELL_DAISY_DEBUG_SYNC_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										   "GatherPipe::OutPort(%s,%s)",
										   this->mBuffer.getClassName(),
										   this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}
#ifdef __SPU__
				explicit OutPort(tBuffer& buffer, tQueueControl& queueControl, uint64_t shmEa,
								 uint64_t eaSignal = 0,
								 int (*fpSendSignal)(uint64_t, uint32_t) = 0,
								 int bookmarkId = 0):
					ScatterGatherInterlock                       (shmEa, tQueueControl::sSize,
																  eaSignal, fpSendSignal),
					Pipe::OutPort<tBuffer, tQueueControl, tMode> (buffer, queueControl, bookmarkId)
					{}
#else
				explicit OutPort(tBuffer& buffer, tQueueControl& queueControl, AtomicInterlock *shmEa,
								 void *eaSignal = 0,
								 int (*fpSendSignal)(void *, uint32_t) = 0,
								 int bookmarkId = 0):
					ScatterGatherInterlock                       (shmEa, tQueueControl::sSize,
																  eaSignal, fpSendSignal),
					Pipe::OutPort<tBuffer, tQueueControl, tMode> (buffer, queueControl, bookmarkId)
					{}
#endif

				int endPop();
      
				int tryEndPop();

			}; /* class OutPort */

			/* endPop
			 * 1.transfer end
			 * 2.beginCompleteConsume (if tMode==REFERENCE)
			 * 3.endCompleteConsume()
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::endPop()
			{
				if (__builtin_expect(!this->hasPendingEntry(),0)) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_NO_BEGIN);
					return CELL_DAISY_ERROR_NO_BEGIN; /* unexpected endPop() */
				}

				register PointerType ptr;
				if (this->mQueueControl.isOutOfOrder()) {
					ptr = this->mPendingFifo.getFront();
				} else {
					ptr = this->mQueueControl.getPointerFromSequenceNumber(this->mLastPointer - (this->mPendingCount - 1));
				}

				this->mBuffer.endRead(ptr);
				CELL_DAISY_DEBUG_PRINTF(
					"%s(endPop): mBuffer.endRead(%d)\n",
					getClassName(),ptr
					);
      
				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.popPtr();
				} else {
					this->mPendingCount--;
				}

				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteConsume(ptr);
				}
				this->mQueueControl.endCompleteConsume(ptr, true /* noQueueFull=true */);

				int ret = release(); /* update compeleNumber */
				if (__builtin_expect(ret, 0)) return ret;
				if (!this->hasPendingEntry() && this->mQueueControl.getPointer() == PTR_TERMINATED) {
					(void)this->terminate(); /* never receive error code */
				}
				CELL_DAISY_BOOKMARK(10);

				return CELL_OK;
			}
    
			/* tryEndPop
			 * 1.transfer end if possible, otherwise abort
			 * 2.beginCompleteConsume (if tMode==REFERENCE)
			 * 3.endCompleteConsume()
			 * @retval CELL_OK, QUEUE_IS_BUSY or CELL_DAISY_ERROR_NO_BEGIN
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::tryEndPop()
			{

				if (__builtin_expect(!this->hasPendingEntry(),0)) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_NO_BEGIN);
					return CELL_DAISY_ERROR_NO_BEGIN; /* unexpected tryEndPop() */
				}

				register PointerType ptr;
				if (this->mQueueControl.isOutOfOrder()) {
					ptr = this->mPendingFifo.getFront();
				} else {
					ptr = this->mQueueControl.getPointerFromSequenceNumber(this->mLastPointer - (this->mPendingCount - 1));
				}

				register int stat = this->mBuffer.tryEndRead(ptr);
				if (__builtin_expect(stat != CELL_OK, 0)) {
					return stat; /* read not completed */
				}
				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryEndPop): mBuffer.tryEndRead(%d)\n",
					getClassName(),ptr
					);

				if (this->mQueueControl.isOutOfOrder()) {
					this->mPendingFifo.popPtr();
				} else {
					this->mPendingCount--;
				}

				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteConsume(ptr);
				}
				this->mQueueControl.endCompleteConsume(ptr, true /* noQueueFull=true */);
                
				int ret = release(); /* update completeNumber */
				if (__builtin_expect(ret, 0)) return ret;
				if (!this->hasPendingEntry() && this->mQueueControl.getPointer() == PTR_TERMINATED) {
					(void)this->terminate(); /* never receive error code */
				}
			
				CELL_DAISY_BOOKMARK(11);
				return CELL_OK;
			}

		} /* namespace GatherPipe */
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_SYNC_PIPE_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
