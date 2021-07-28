/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* pipe.h
 */

#ifndef __CELL_DAISY_PIPE_H__
#define __CELL_DAISY_PIPE_H__

#include <cell/daisy/daisy_defs.h>
#include <cell/daisy/snprintf.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_PIPE
#include <cell/daisy/daisy_debug.h>
#endif

#undef CELL_DAISY_BOOKMARK
#define CELL_DAISY_BOOKMARK(x)
#ifdef CELL_DAISY_BOOKMARK_PIPE
#define __CELL_DAISY_BOOKMARK_PREFIX__ CELL_DAISY_BOOKMARK_PREFIX_PIPE
#include <cell/daisy/daisy_bookmark.h>
#endif

#define CELL_DAISY_ASSERT_REFERENCE	cellDaisyAssert(tMode == REFERENCE && this->mBuffer.sBufferType == BUFFER_TYPE_LOCAL)
#define CELL_DAISY_ASSERT_COPY		cellDaisyAssert(tMode == COPY)

namespace cell {
	namespace Daisy {
		namespace Pipe {
    
			template<class tBuffer, class tQueueControl, BufferMode tMode = COPY>
			class Port
			{
			private:

				/* static assertion */
				static int sAssertDummy[
					(tQueueControl::sSize == tBuffer::sSize) && /* 1. queue control size is equal to buffer size */
					(tMode != REFERENCE || tBuffer::sBufferType == BUFFER_TYPE_LOCAL) /* 2.if tMode is REFERENCE then queue control is local */
					? 1 : -1];

			protected:
#ifdef __SPU__

				qword     mIsTerminated;
      
				qword     mCancelled;

#else

				bool      mIsTerminated;

				bool      mCancelled;

#endif
      
				tBuffer& mBuffer               __CELL_DAISY_SPU_ALIGN16__;
      
				tQueueControl& mQueueControl   __CELL_DAISY_SPU_ALIGN16__;
      
				Fifo<PointerType> mPendingFifo __CELL_DAISY_SPU_ALIGN16__;

				PointerType mLastPointer       __CELL_DAISY_SPU_ALIGN16__;
				int mPendingCount              __CELL_DAISY_SPU_ALIGN16__;
      
				int mBookmarkId                __CELL_DAISY_SPU_ALIGN16__;

#ifdef __SPU__
				/* dummy for padding */      
				int mDummy                     __CELL_DAISY_SPU_ALIGN16__;
#endif

			public:
      
				typedef typename tBuffer::DataType Type;

				static const QueueControlType sQueueControlType = tQueueControl::sQueueControlType;
				static const BufferMode sBufferMode = tMode;

				virtual const char  *getClassName(){return 0;}

				/* constructor definition */
      
				explicit Port(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId=0);

				/* virtual destructor definition */
				virtual ~Port(){}
				
				__attribute__((always_inline)) inline
					volatile Type* getCurrentReference()
				{
					
					CELL_DAISY_ASSERT_REFERENCE;

					unsigned int isOutOfOrder = mQueueControl.isOutOfOrder();
					if (__builtin_expect(
							(isOutOfOrder &&
							 (mPendingFifo.isEmpty() ||
							  (__CELL_DAISY_SPU_GET_VAL__(mCancelled) != 0u && (mPendingFifo.getSize() == 1)))) ||
							(!isOutOfOrder && (
								mPendingCount==0 ||
								(__CELL_DAISY_SPU_GET_VAL__(mCancelled) != 0u && mPendingCount == 1))),0))
					{
						return NULL;
					}

					register PointerType ptr;
					if (__builtin_expect(mQueueControl.isOutOfOrder(), 0)) {
						ptr = mPendingFifo.getBack();
					} else {
						ptr = mQueueControl.getPointerFromSequenceNumber(mLastPointer);
					}

					CELL_DAISY_DEBUG_PRINTF("%s(getCurrentReference): ptr=%d(%p)\n",
											getClassName(), ptr, mBuffer.getEntryReference(ptr));
					return mBuffer.getEntryReference(ptr);
				}
      
				__attribute__((always_inline)) inline
					bool hasPendingEntry()
				{
					unsigned int isOutOfOrder = mQueueControl.isOutOfOrder();
					return !((isOutOfOrder && (
								  mPendingFifo.isEmpty() |
								  (__CELL_DAISY_SPU_GET_VAL__(mCancelled) != 0u && (mPendingFifo.getSize() == 1)))) |
							 (!isOutOfOrder && (
								 mPendingCount==0 |
								 (__CELL_DAISY_SPU_GET_VAL__(mCancelled) != 0u && mPendingCount == 1))));
				}

				void cancel();
      
				int terminate() {
					cellDaisyAssert(!hasPendingEntry());
					
					if (!__CELL_DAISY_SPU_GET_VAL__(mIsTerminated)) {
						__CELL_DAISY_SPU_SET_VAL__(mIsTerminated, true);
						int ret = this->mQueueControl.terminate();
						if (ret) return ret;
					}
					return CELL_OK;
				}

			};

			template<class tBuffer, class tQueueControl, BufferMode tMode>
			inline Port<tBuffer, tQueueControl, tMode>::Port(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId):
#ifdef __SPU__
				mIsTerminated   (si_from_uint(0)),
				mCancelled      (si_from_uint(0)),
#else
				mIsTerminated   (0),
				mCancelled      (0),
#endif
				mBuffer         (buffer),
				mQueueControl   (queueControl),
				mBookmarkId     (bookmarkId)
            {
				cellDaisyAssert(bookmarkId < CELL_DAISY_MAX_BOOKMARK_ID);
				
#ifdef __SPU__				
				/* DMA tag */
				cellDaisyAssert(! ( (buffer.sBufferType == BUFFER_TYPE_REMOTE) &&
									(queueControl.sQueueControlType == QCTL_TYPE_SIGNAL_NOTIFICATION) )
								||
								(buffer.getTag() == queueControl.getTag()));
#endif /* __SPU__ */
								
				if (__builtin_expect(mQueueControl.isOutOfOrder(), 0)) {
					mPendingFifo.initialize();
				} else {
					mPendingCount = 0;
				}
			}

			/* cancel */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				void Port<tBuffer, tQueueControl, tMode>::cancel()
			{

				unsigned int isOutOfOrder = mQueueControl.isOutOfOrder();
				if (__builtin_expect(!((isOutOfOrder  && mPendingFifo.isEmpty()) |
									   (!isOutOfOrder && (mPendingCount == 0))), 0))
				{
					__CELL_DAISY_SPU_SET_VAL__(mCancelled, true);
				}
			}

			template<class tBuffer, class tQueueControl, BufferMode tMode = COPY>
			class InPort: public Port<tBuffer,tQueueControl,tMode>
			{
			private:
				/* static assertion */
				static int sAssertDummy[
					tQueueControl::sPort == INPUT
					? 1 : -1];

			public:

				typedef typename tBuffer::DataType GlueDataType;

				/* producer's methods */
      
				const char  *getClassName() {
#ifdef CELL_DAISY_DEBUG_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										   "Pipe::InPort(%s,%s)",
										   this->mBuffer.getClassName(),
										   this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}

				explicit InPort(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId=0):
					Port<tBuffer, tQueueControl, tMode> (buffer, queueControl, bookmarkId)
				{
				}
    
				virtual ~InPort() {
					int ret = this->terminate();
					cellDaisyAssert(ret == CELL_OK);
					while(this->hasUnfinishedConsumer()){}
				}
    
				typedef typename tBuffer::DataType Type;

				int push(Type * __restrict__ data);

				virtual int beginPush(Type * __restrict__ data);

				virtual int beginPush();
      
				virtual int tryBeginPush(Type * __restrict__ data);
				
				virtual int tryBeginPush();
      
				virtual int endPush();
      
				virtual int tryEndPush();

				void cancelPush();

				bool hasUnfinishedConsumer();

			}; /* class InPort */
    
			template<class tBuffer, class tQueueControl, BufferMode tMode = COPY>
			class OutPort: public Port<tBuffer,tQueueControl,tMode>
			{
			private:
				/* static assertion */
				static int sAssertDummy[
					tQueueControl::sPort == OUTPUT
					? 1 : -1];

			public:

				typedef typename tBuffer::DataType GlueDataType;

				/* consumer's methods */
      
				const char  *getClassName() {
#ifdef CELL_DAISY_DEBUG_PIPE
					static char __buf[64];
					cell::Daisy::_snprintf(__buf, 64,
										  "Pipe::OutPort(%s,%s)",
										  this->mBuffer.getClassName(),
										  this->mQueueControl.getClassName()
						);
					return (const char*)__buf;
#else
					return 0;
#endif
				}

				explicit OutPort(tBuffer& buffer, tQueueControl& queueControl, int bookmarkId=0):
					Port<tBuffer, tQueueControl, tMode>(buffer, queueControl, bookmarkId)
				{
				}

				virtual ~OutPort() {
					int ret = this->terminate();
					cellDaisyAssert(ret == CELL_OK);
				}
    
				typedef typename tBuffer::DataType Type;

				int pop(Type * __restrict__ data);

				virtual int beginPop(Type * __restrict__ data);

				virtual int beginPop();
      
                virtual int tryBeginPop(Type * __restrict__ data);

				virtual int tryBeginPop();
      
				virtual int endPop();
      
				int tryEndPop();

				void cancelPop();
				
			}; /* class OutPort */

			/* producer's methods */

			/* push (tMode == COPY)
			 * beginPush() followed by endPush()
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer,tQueueControl,tMode>::push(Type * __restrict__ data)
			{
				CELL_DAISY_ASSERT_COPY;
				int ret = beginPush(data);
				if (__builtin_expect(ret == CELL_DAISY_ERROR_AGAIN, 0)) return ret;
				(void)endPush();
				return CELL_OK;
			}
			
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

				this->mBuffer.beginWrite(ptr, data);
				CELL_DAISY_DEBUG_PRINTF(
					"%s(beginPush): mBuffer.beginWrite(%d, %p)\n",
					getClassName(), ptr, data
					);

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */
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

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */
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
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
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
				}
				if (__builtin_expect((stat = this->mBuffer.tryBeginWrite(ptr, data)) != CELL_OK, 0)) {
					return stat; /* write cannot start */
				}
				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryBeginPush): mBuffer.tryBeginWrite(%d, %p)\n",
					getClassName(), ptr, data
					);

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}
				
				this->mQueueControl.beginCompleteProduce(ptr); /* tMode == COPY */

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

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
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
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
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				CELL_DAISY_BOOKMARK(3);
				return CELL_OK;
			}

    
			/* endPush
			 * 1.transfer end
			 * 2.beginCompleteProduce() (if tMode==REFERENCE)
			 * 3.endCompleteProduce()
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl, tMode>::endPush()
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
      
				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteProduce(ptr);
				}
				this->mQueueControl.endCompleteProduce(ptr);
				CELL_DAISY_BOOKMARK(4);

				return CELL_OK;
			}

			/* tryEndPush
			 * 1.transfer end if possible,otherwise abort
			 * 2.beginCompleteProduce() (if tMode==REFERENCE)
			 * 3.endCompleteProduce()
			 * @retval CELL_OK, QUEUE_IS_BUSY or CELL_DAISY_ERROR_NO_BEGIN 
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int InPort<tBuffer, tQueueControl, tMode>::tryEndPush()
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

				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteProduce(ptr);
				}
				this->mQueueControl.endCompleteProduce(ptr);

				CELL_DAISY_BOOKMARK(5);
				return CELL_OK;
			}

			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				void InPort<tBuffer,tQueueControl,tMode>::cancelPush()
			{
				this->cancel();
			}

 			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				bool InPort<tBuffer,tQueueControl,tMode>::hasUnfinishedConsumer()
			{
				return this->mQueueControl.hasUnfinishedConsumer(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled));
			}

			/* consumer's methods */
    
			/* pop (tMode == COPY)
			 * beginPop() followed by endPop()
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::pop(Type * __restrict__ data)
			{
				CELL_DAISY_ASSERT_COPY;
				int ret = beginPop(data);
				if (__builtin_expect(ret == CELL_DAISY_ERROR_AGAIN, 0)) {
					return ret;
				}
				if (__builtin_expect(ret == TERMINATED, 0)) {
					return TERMINATED;
				}
				ret = endPop();
				return ret;
			}

			/* beginPop (tMode == COPY)
			 * 1.get head_ptr
			 * 2.initiate transfer
			 * 3.beginCompleteConsume (tMode == COPY)
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::beginPop(Type * __restrict__ data)
			{
				CELL_DAISY_ASSERT_COPY;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}

				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mIsTerminated), 0)) {
					return TERMINATED;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					if (__builtin_expect(this->mPendingFifo.isFull(), 0)) return PTR_UNAVAILABLE;
				}

				register PointerType ptr = this->mQueueControl.getPointer();
				if (__builtin_expect(ptr == PTR_UNAVAILABLE && ptr != PTR_TERMINATED, 1)) {
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

				this->mBuffer.beginRead(ptr, data);
				CELL_DAISY_DEBUG_PRINTF(
					"%s(beginPop): mBuffer.beginRead(%d, %p)\n",
					getClassName(), ptr, data
					);

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
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
    
			/* beginPop (tMode == REFERENCE)
			 * 1.get head_ptr
			 * 2.initiate transfer
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::beginPop()
			{
				CELL_DAISY_ASSERT_REFERENCE;

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
						(void)this->terminate(); /* never receive return code */
					}
					return TERMINATED;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */

				CELL_DAISY_BOOKMARK(7);
				return CELL_OK;
			}

			/* tryBeginPop (tMode == COPY)
			 * 1.get head_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 * 3.beginCompleteConsume (tMode == COPY)
			 * @retval CELL_OK, QUEUE_IS_BUSY
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::tryBeginPop(Type * __restrict__ data)
			{

				CELL_DAISY_ASSERT_COPY;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
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
						(void)this->terminate(); /* never receive return code */
					}
					return TERMINATED;
				}
				register int stat = this->mBuffer.tryBeginRead(ptr, data);
				if (__builtin_expect(stat != CELL_OK, 0)) {
					return stat; /* read cannot be started */
				}
				CELL_DAISY_DEBUG_PRINTF(
					"%s(tryBeginPop): mBuffer.tryBeginRead(%d, %p)\n",
					getClassName(), ptr, data
					);

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
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

			/* tryBeginPop (tMode == REFERENCE)
			 * 1.get head_ptr if possible,otherwise abort
			 * 2.initiate transfer
			 * @retval CELL_OK, QUEUE_IS_BUSY
			 */
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				int OutPort<tBuffer, tQueueControl, tMode>::tryBeginPop()
			{

				CELL_DAISY_ASSERT_REFERENCE;
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mCancelled), 0)) {
					__CELL_DAISY_SPU_SET_VAL__(this->mCancelled, false);
					return CELL_OK;
				}
				if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(this->mIsTerminated), 0)) {
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
						(void)this->terminate(); /* never receive return code */
					}
					return TERMINATED;
				}

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.pushPtr(ptr);
				} else {
					this->mLastPointer = ptr;
					this->mPendingCount++;
				}

				this->mQueueControl.setPointer(PTR_UNAVAILABLE); /* reset pointer */
      
				CELL_DAISY_BOOKMARK(9);
				return CELL_OK;
			}
    
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
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					ptr = this->mPendingFifo.getFront();
				} else {
					ptr = this->mQueueControl.getPointerFromSequenceNumber(this->mLastPointer - (this->mPendingCount - 1));
				}

				this->mBuffer.endRead(ptr);
				CELL_DAISY_DEBUG_PRINTF(
					"%s(endPop): mBuffer.endRead(%d)\n",
					getClassName(),ptr
					);
      
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.popPtr();
				} else {
					this->mPendingCount--;
				}

				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteConsume(ptr);
				}
				this->mQueueControl.endCompleteConsume(ptr);
				if (!this->hasPendingEntry() && this->mQueueControl.getPointer() == PTR_TERMINATED) {
					(void)this->terminate(); /* never receive return code */
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
				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
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

				if (__builtin_expect(this->mQueueControl.isOutOfOrder(), 0)) {
					this->mPendingFifo.popPtr();
				} else {
					this->mPendingCount--;
				}

				if (tMode == REFERENCE) {
					this->mQueueControl.beginCompleteConsume(ptr);
				}
                this->mQueueControl.endCompleteConsume(ptr);
			
				if (!this->hasPendingEntry() && this->mQueueControl.getPointer() == PTR_TERMINATED) {
					(void) this->terminate(); /* never receive return code */
				}
				CELL_DAISY_BOOKMARK(11);
				return CELL_OK;
			}
    
			template<class tBuffer, class tQueueControl, BufferMode tMode>
			__attribute__((always_inline)) inline
				void OutPort<tBuffer,tQueueControl,tMode>::cancelPop()
			{
				this->cancel();
			}

		} /* namespace Pipe */
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_PIPE_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
