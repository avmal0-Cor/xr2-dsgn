/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* glue.h
 */

#ifndef __CELL_DAISY_GLUE_H__
#define __CELL_DAISY_GLUE_H__

#include <cell/daisy/v_memcpy.h>
#include <cell/daisy/daisy_defs.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_GLUE
#include <cell/daisy/daisy_debug.h>
#endif

#undef CELL_DAISY_BOOKMARK
#undef CELL_DAISY_BOOKMARK_CLASS
#define CELL_DAISY_BOOKMARK(x)
#define CELL_DAISY_BOOKMARK_CLASS(x,y)
#ifdef CELL_DAISY_BOOKMARK_GLUE
#include <cell/daisy/daisy_bookmark.h>
#endif

namespace cell {
	namespace Daisy {

#define CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS \
		((tOutPort::sBufferMode == COPY) && \
		 ( (tMode == TWO_PORT) | (tInPort::sBufferMode == COPY) ))
#define CELL_DAISY_GLUE_USE_IN_PORT_BUFFERS \
		((tInPort::sBufferMode == COPY) && tMode == TWO_PORT)

		static const SizeType MAX_PORTS = 6;

		__attribute__((always_inline)) inline
		static void clearBit(BitmapType &mask, unsigned int bitPos)
		{
			mask &= ~(1u << bitPos);
		}

		__attribute__((always_inline)) inline
		static void setBit(BitmapType &mask, unsigned int bitPos)
		{
			mask |= 1u << bitPos;
		}

		__attribute__((always_inline)) inline
		static unsigned int isFull(BitmapType mask, unsigned int size)
		{
			return (mask == (1u << size) - 1u);
		}

		__attribute__((always_inline)) inline
			static PointerType getAvailableBuffer(BitmapType useMask)
		{
			return (PointerType)spu_extract(spu_cntlz(spu_promote(~useMask, 0)), 0);
		}

		template<class tOutPort, class tInPort,
				 GlueMode tMode = TWO_PORT,
				 SizeType tOutPortBufferCount = 1,
				 SizeType tInPortBufferCount = 1>
		class Glue {
		public:

			typedef typename tOutPort::GlueDataType OutPortDataType;
			typedef typename tInPort::GlueDataType  InPortDataType;

		private:

			/* static assertion */
			static int sAssertDummy[
				(tMode != ONE_PORT ||
				 ((sizeof(OutPortDataType) == sizeof(InPortDataType)) &&
				  (tOutPort::sBufferMode == COPY || tInPort::sBufferMode == COPY)))
				 ? 1 : -1];

		protected:
			
			static void default_two_port_calculate(
				typename tInPort::GlueDataType *dst,
				const typename tOutPort::GlueDataType *src)
			{
				_cellDaisyMemcpy((void *)(uintptr_t)dst, (const void *)(uintptr_t)src, sizeof(*src));
			}

			void (*mTwoPortCalculate)(typename tInPort::GlueDataType *dst, const typename tOutPort::GlueDataType *src);
			void (*mOnePortCalculate)(typename tOutPort::GlueDataType *srcDst);

			static const SizeType sBufferSize =
			(CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS ? sizeof(OutPortDataType) * tOutPortBufferCount : 0) +
			(CELL_DAISY_GLUE_USE_IN_PORT_BUFFERS ? sizeof(InPortDataType) * tInPortBufferCount : 0) +
			(CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS | CELL_DAISY_GLUE_USE_IN_PORT_BUFFERS |
			 (tMode == ONE_PORT &&
			  tOutPort::sBufferMode == REFERENCE &&
			  tInPort::sBufferMode == REFERENCE) ? 0 : 1);
			

			/* out port definition */
			typedef struct {
				PointerType       mPointer;
				typename tOutPort::GlueDataType *mBuffer;
				uint32_t          mPort;
				uint32_t          padding;
			} OutPortBuffer;

			typedef struct {
				PointerType       mPointer;
				typename tInPort::GlueDataType  *mBuffer;
				uint32_t          padding[2];
			} InPortBuffer;

			SizeType             mOutPortCount                  __attribute__((aligned(16)));
			tOutPort            *mOutPort[MAX_PORTS]            __attribute__((aligned(16)));
			PointerType          mOutPortBeginPointer           __attribute__((aligned(16)));
			PointerType          mOutPortEndPointer             __attribute__((aligned(16)));
			Fifo<OutPortBuffer>  mOutPortBufferQueue[MAX_PORTS] __attribute__((aligned(16)));
			BitmapType           mOutPortTerminatedMask         __attribute__((aligned(16)));

			/* out port buffer definition */
			OutPortDataType     *mOutPortBuffers                __attribute__((aligned(128)));
			BitmapType          *mOutPortBufferUseMask          __attribute__((aligned(16)));
			Fifo<OutPortBuffer>  mOutPortDataReadyQueue         __attribute__((aligned(16)));
      
			/* in port definition */
			SizeType             mInPortCount                   __attribute__((aligned(16)));
			tInPort             *mInPort[MAX_PORTS]             __attribute__((aligned(16)));
			PointerType          mInPortBeginPointer            __attribute__((aligned(16)));
			PointerType          mInPortEndPointer              __attribute__((aligned(16)));
			Fifo<InPortBuffer>   mInPortBufferQueue[MAX_PORTS]  __attribute__((aligned(16)));
			BitmapType           mInPortTerminatedMask          __attribute__((aligned(16)));

			/* in port buffer definition */
			InPortDataType      *mInPortBuffers                 __attribute__((aligned(128)));
			BitmapType          *mInPortBufferUseMask           __attribute__((aligned(16)));
			qword                mInPortDataReady;
			PointerType          mInPortDataReadyPointer        __attribute__((aligned(16)));
			uint32_t             mInPortDataReadyPort           __attribute__((aligned(16)));
			InPortDataType      *mInPortData                    __attribute__((aligned(16)));

			Fifo<InPortBuffer>   mPostCalculatedDataQueue       __attribute__((aligned(16)));
			unsigned int         mPending                       __attribute__((aligned(128)));

			uint8_t              mBuffer[sBufferSize]           __attribute__((aligned(128)));

			BitmapType           mBufferUseMask[2]              __attribute__((aligned(128)));

			int                  mBookmarkId                    __attribute__((aligned(16)));

			/* dummy for padding */

			int                  mDummy                         __attribute__((aligned(16)));

		private:

			void initialize(int bookmarkId);

			unsigned int canCalculate() {
				if (tMode == TWO_PORT) {
					return (!mOutPortDataReadyQueue.isEmpty() &&
							__CELL_DAISY_SPU_GET_VAL__(mInPortDataReady));
				} else {
					return !mOutPortDataReadyQueue.isEmpty();
				}
			}

		public:
      
			/* constructor definition */
      
			explicit Glue(int bookmarkId=0);
			explicit Glue(
				void (*callback)(typename tInPort::GlueDataType *, const typename tOutPort::GlueDataType *),
				int bookmarkId=0);
			explicit Glue(
				void (*callback)(typename tOutPort::GlueDataType *),
				int bookmarkId=0);

			/* virtual destructor definition */
			virtual ~Glue(){}

			int addOutPort(tOutPort& outPort) {
				if (mOutPortCount >= MAX_PORTS) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}
				if (mOutPortCount>=1 && mInPortCount>1) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}
				if (tMode==ONE_PORT &&
					tOutPort::sBufferMode == REFERENCE && tInPort::sBufferMode == COPY &&
					mOutPortCount==1)
				{
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}

				mOutPort           [mOutPortCount] = &outPort;
				mOutPortBufferQueue[mOutPortCount].initialize();
				mOutPortCount++;

				return CELL_OK;
			}
				
			int addInPort(tInPort& inPort) {
				if (mInPortCount >= MAX_PORTS) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}
				if (mInPortCount>=1 && mOutPortCount>1) {
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}
				if (tMode==ONE_PORT &&
					tOutPort::sBufferMode == REFERENCE && tInPort::sBufferMode == COPY &&
					mInPortCount==1)
				{
					CELL_ERROR_CHECK_ERROR(CELL_DAISY_ERROR_INVALID_PORT_ATTACH);
					return CELL_DAISY_ERROR_INVALID_PORT_ATTACH;
				}
				
				mInPort           [mInPortCount] = &inPort;
				mInPortBufferQueue[mInPortCount].initialize();
				mInPortCount++;

				return CELL_OK;
			}

			/* execute 1-step of Glue dataflow */
			int step(BlockMode mode = NOT_STALL);

		protected:

			int beginPop(BlockMode mode, int isRetry);
			void calculate();
			int beginPush(BlockMode mode, int isRetry);
			void terminateInPorts();
			virtual void proceed(PointerType& pointer, const SizeType size, const InPortDataType *data);

			bool proceedOutPortBegin(BitmapType busyMask);
			bool proceedOutPortEnd(BitmapType busyMask);
			bool proceedInPortBegin(BitmapType busyMask, const InPortDataType *data);
			bool proceedInPortEnd(BitmapType busyMask);
		};

		/* constructor description(s) */
		
		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		inline void Glue<tOutPort, tInPort, tMode, tOutPortBufferCount, tInPortBufferCount>::initialize(int bookmarkId) {
			mOutPortCount            = 0   ;
			mOutPortBeginPointer     = 0   ;
			mOutPortEndPointer       = -1  ;
			mOutPortTerminatedMask   = 0   ;
			mInPortCount             = 0   ;
			mInPortBeginPointer      = 0   ;
			mInPortEndPointer        = -1  ;
			mInPortTerminatedMask    = 0   ;
			mInPortDataReady         = si_from_uint(false);
			mInPortDataReadyPointer  = -1  ;
			mInPortDataReadyPort     = 0   ;
			mBookmarkId              = bookmarkId;
			mBufferUseMask[0] = mBufferUseMask[1] = 0u;

			SizeType offset = 0;
			if (CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS) {
				mOutPortBuffers =  (OutPortDataType *)(uintptr_t)&mBuffer[offset];
				offset += sizeof(OutPortDataType)*tOutPortBufferCount;
			}
			if (CELL_DAISY_GLUE_USE_IN_PORT_BUFFERS) {
				mInPortBuffers =  (InPortDataType *)(uintptr_t)&mBuffer[offset];
			}
			
			mOutPortBufferUseMask = &mBufferUseMask[0];
			if ((tMode == ONE_PORT) && (tInPort::sBufferMode == COPY)) {
				mInPortBufferUseMask = &mBufferUseMask[0];
			} else {
				mInPortBufferUseMask = &mBufferUseMask[1];
			}

			mOutPortDataReadyQueue.initialize();
			mPostCalculatedDataQueue.initialize();
			mPending = 0;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		inline Glue<tOutPort, tInPort, tMode, tOutPortBufferCount, tInPortBufferCount>::Glue(int bookmarkId)
		{
			initialize(bookmarkId);

			if (tMode == ONE_PORT) {
				mOnePortCalculate = NULL;
			} else {
				mTwoPortCalculate = default_two_port_calculate;
			}
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		inline Glue<tOutPort, tInPort, tMode, tOutPortBufferCount, tInPortBufferCount>::
		Glue(void (*callback)(typename tInPort::GlueDataType *, const typename tOutPort::GlueDataType *), int bookmarkId)
		{
			cellDaisyAssert(tMode == TWO_PORT);
			initialize(bookmarkId);
			
			mTwoPortCalculate = callback;

		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		inline Glue<tOutPort, tInPort, tMode, tOutPortBufferCount, tInPortBufferCount>::
		Glue(void (*callback)(typename tOutPort::GlueDataType *), int bookmarkId)
		{
			cellDaisyAssert(tMode == ONE_PORT);
			initialize(bookmarkId);
			
			mOnePortCalculate = callback;
		}

		/* member function description(s) */

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		inline
			int Glue<tOutPort, tInPort, tMode, tOutPortBufferCount, tInPortBufferCount>::step(BlockMode mode)
		{
			if (__builtin_expect(mInPortCount == 0 || mOutPortCount == 0, 0)) return CELL_DAISY_ERROR_STAT;

			int isActive;
			for(int isRetry = false; !isRetry || ((mode==STALL) && !isActive); isRetry = true) {
				isActive = false;
				// begin out port loop
				if (__builtin_expect(tOutPort::sBufferMode == COPY || mOutPortDataReadyQueue.isEmpty(), 1)) {
					BitmapType busyMask = 0;
					for(unsigned isBreak = false; !isBreak; isBreak |= !proceedOutPortBegin(busyMask)) {
						int ret = beginPop(mode, isRetry);
						if (__builtin_expect(ret < 0, 0)) {
							/* CELL_DAISY_ERROR_AGAIN : cannot block        */
							/* CELL_DAISY_ERROR_BUSY  : PPU is initializing */
							return ret;
						} else if (__builtin_expect(ret == GLUE_ACTIVE, 1)) {
							isActive = true; isRetry = false;
							if (tOutPort::sBufferMode != COPY) {
								isBreak = true;
							}
						} else if (__builtin_expect((ret == TERMINATED) &&
													!mOutPort[mOutPortBeginPointer]->hasPendingEntry() &&
													mOutPortDataReadyQueue.isEmpty(), 0))
						{
							// mark OutPort[mOutPortBeginPointer] TERMINATED
							CELL_DAISY_DEBUG_PRINTF("glue: OutPort[%d] TERMINATED\n", mOutPortBeginPointer);
							setBit(mOutPortTerminatedMask, mOutPortBeginPointer);
						} else {
							// mark OutPort[mOutPortBeginPointer] BUSY
							setBit(busyMask, mOutPortBeginPointer);
						}
					}
				}

				// prepare OutPort buffer
				if (tOutPort::sBufferMode == COPY && (mOutPortDataReadyQueue.isEmpty() || isRetry)) {
					// COPY
					// end out port loop
					for (BitmapType busyMask = 0; proceedOutPortEnd(busyMask); setBit(busyMask, mOutPortEndPointer)) {
						int stat = mOutPort[mOutPortEndPointer]->tryEndPop();
						if (__builtin_expect(stat == CELL_OK, 1)) {
							if (tOutPort::sBufferMode == COPY) {
								mPending--;
							}
							CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, MAX_PORTS + mOutPortEndPointer);
							mOutPortDataReadyQueue.pushPtr(mOutPortBufferQueue[mOutPortEndPointer].popPtr());
							CELL_DAISY_DEBUG_PRINTF("glue: mOutPort[%d]->tryEndPop() pointer=%d\n",
													mOutPortEndPointer , mOutPortDataReadyQueue.getBack().mPointer);
							isActive = true;
							break;
						}
					}
				}
						
				if (__builtin_expect(!__CELL_DAISY_SPU_GET_VAL__(mInPortDataReady) && tMode == TWO_PORT, 1)) {
					// prepare InPort buffer
					PointerType bufferPointer;
					if (CELL_DAISY_GLUE_USE_IN_PORT_BUFFERS) {
						bufferPointer = getAvailableBuffer(*mInPortBufferUseMask);
						if (__builtin_expect(bufferPointer < (PointerType)tInPortBufferCount, 1)) {
							*mInPortBufferUseMask |= 1 << (31 - bufferPointer);

							__CELL_DAISY_SPU_SET_VAL__(mInPortDataReady, true);
							mInPortDataReadyPointer = bufferPointer;
							mInPortData = &mInPortBuffers[mInPortDataReadyPointer];
							CELL_DAISY_DEBUG_PRINTF("glue: mInPortBuffers[%d] allocated\n", bufferPointer);
							isActive = true;
						}
					}
				}

				// calculate
				if (__builtin_expect(canCalculate(), 1)) {
					calculate();
					isActive = true;
				}

				if (__builtin_expect(isFull(mOutPortTerminatedMask, mOutPortCount) && mPostCalculatedDataQueue.isEmpty(), 0)) {
					terminateInPorts();
					isActive = true;
				} else if (__builtin_expect(tInPort::sBufferMode == COPY | !__CELL_DAISY_SPU_GET_VAL__(mInPortDataReady), 1)) {
					// begin in port loop
					BitmapType busyMask = 0;
					for (unsigned isBreak = false; !isBreak; isBreak |= !proceedInPortBegin(busyMask,
																							(tInPort::sBufferMode == COPY) ?
																							mPostCalculatedDataQueue.getFront().mBuffer : 0)) {
						int ret = beginPush(mode, isRetry);
						if (__builtin_expect(ret < 0, 0)) {
							/* CELL_DAISY_ERROR_AGAIN : cannot block        */
							/* CELL_DAISY_ERROR_BUSY  : PPU is initializing */
							return ret;
						} else if (__builtin_expect(ret == GLUE_ACTIVE, 1)) {
							isActive = true; isRetry = false;
							if (tInPort::sBufferMode != COPY) {
								isBreak = true;
							}
						} else {
							setBit(busyMask, mInPortBeginPointer);
						}
					}
				}

				if (tInPort::sBufferMode == COPY) {
					// end in port loop
					for(uint32_t i = 0; i < mInPortCount; i++) {
						while(mInPort[i]->tryEndPush() == CELL_OK) {
							mPending--;
							CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_IN, MAX_PORTS + i);
							InPortBuffer inPortBuffer = mInPortBufferQueue[i].popPtr();
							CELL_DAISY_DEBUG_PRINTF("glue: mInPort[%d]->tryEndPush() pointer = %d\n",
													i, inPortBuffer.mPointer);
							if (tMode == ONE_PORT && tOutPort::sBufferMode == REFERENCE) {
								// only 1-to-1 connection is allowed
								mOutPort[0]->endPop();
								CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, MAX_PORTS + 0);
								CELL_DAISY_DEBUG_PRINTF("glue: mInPort[0]->endPop()\n");
							}
							*mInPortBufferUseMask &= ~(1 << (31 - inPortBuffer.mPointer)); // reset buffer use mask
							isActive = true;
						}
					}
				}
			}

			return (!isActive) ? QUEUE_IS_BUSY :
				(cell::Daisy::isFull(mInPortTerminatedMask, this->mInPortCount) ? TERMINATED : CELL_OK);
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			void Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::calculate()
		{
			OutPortBuffer outPortReadyData;
			outPortReadyData = mOutPortDataReadyQueue.popPtr();
			if (tMode == TWO_PORT) {
				CELL_DAISY_DEBUG_PRINTF("glue: calculate(0x%p,0x%p)\n",	mInPortData, outPortReadyData.mBuffer);
				(*mTwoPortCalculate)((typename tInPort::GlueDataType *)(uintptr_t)mInPortData,
									 (typename tOutPort::GlueDataType *)(uintptr_t)(outPortReadyData.mBuffer));
				CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, MAX_PORTS * 2);
				if (tInPort::sBufferMode == COPY) {
					InPortBuffer postCalcData = {
						mInPortDataReadyPointer,
						mInPortData,
						{0,0}
					};
					mPostCalculatedDataQueue.pushPtr(postCalcData);
				}
					
				__CELL_DAISY_SPU_SET_VAL__(mInPortDataReady, false);
					
				if (tOutPort::sBufferMode == COPY) {
					*mOutPortBufferUseMask &= ~(1 << (31 - outPortReadyData.mPointer)); // reset buffer use mask
				} else {
					mOutPort[outPortReadyData.mPort]->endPop();
					CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, MAX_PORTS + outPortReadyData.mPort);
					CELL_DAISY_DEBUG_PRINTF("glue: mOutPort[%d]->endPop()\n", outPortReadyData.mPort);
				}
				if (tInPort::sBufferMode == REFERENCE) {
					mInPort[mInPortDataReadyPort]->endPush();
					CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_IN, MAX_PORTS + mInPorDataReadyPort);
					CELL_DAISY_DEBUG_PRINTF("glue: mInPort[%d]->endPush()\n", mInPortDataReadyPort);
				}
			} else {
				if (mOnePortCalculate != NULL) {
					CELL_DAISY_DEBUG_PRINTF("glue: calculate(0x%p)\n",	mInPortData);
					(*mOnePortCalculate)((typename tOutPort::GlueDataType *)(uintptr_t)outPortReadyData.mBuffer);
					CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, MAX_PORTS * 2);
				}

				if (tInPort::sBufferMode == REFERENCE) {
					mInPort[mInPortDataReadyPort]->endPush();
					CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_IN, MAX_PORTS + mInPortDataReadyPort);
					CELL_DAISY_DEBUG_PRINTF("glue: mInPort[%d]->endPush()\n", mInPortDataReadyPort);
					if (!(tOutPort::sBufferMode == COPY && tMode == ONE_PORT)) {
						__CELL_DAISY_SPU_SET_VAL__(mInPortDataReady, false);
					}
				} else {
					InPortBuffer postCalcData = {outPortReadyData.mPointer,(typename tInPort::GlueDataType*)outPortReadyData.mBuffer,{0,0}};
//					postCalcData.mPointer = outPortReadyData.mPointer;
//					postCalcData.mBuffer  = (typename tInPort::GlueDataType*)outPortReadyData.mBuffer;
					mPostCalculatedDataQueue.pushPtr(postCalcData);
				}
			}
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			void Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::terminateInPorts()
		{
			for (unsigned int i = 0; i < mInPortCount; i++) { 
				if (tInPort::sBufferMode == REFERENCE &&
					mInPort[i]->hasPendingEntry())
				{
					mInPort[i]->cancelPush();
					CELL_DAISY_DEBUG_PRINTF("glue: mInPort[%d]->cancelPush()\n", i);
				}
				if (!mInPort[i]->hasPendingEntry()) {
					CELL_DAISY_DEBUG_PRINTF("glue: terminate mInPort[%d]\n", i);
					
					this->mInPort[i]->terminate();
					setBit(mInPortTerminatedMask, i);
				}
			}
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			int Glue<tOutPort, tInPort, tMode,
					 tOutPortBufferCount, tInPortBufferCount>::beginPop(BlockMode mode, int isRetry)
		{
			int ret = 0;
			PointerType      bufferPointer = 0; /* pointer to copy buffer */
			OutPortDataType *buffer = NULL; /* buffer to copy popped data */
			for(unsigned again = false ;
				!again ||
					((mode == STALL) && (tOutPort::sQueueControlType != QCTL_TYPE_LOCAL) && isRetry &&
					 !mPending);
				again = true)
			{
				if (__builtin_expect(!again, 1)) {
					if (tOutPort::sBufferMode == COPY) {
						/* Allocate outPort buffer */
						if (CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS) {
							if (__builtin_expect((bufferPointer = getAvailableBuffer(*mOutPortBufferUseMask))
												 < (PointerType)tOutPortBufferCount, 1)) {
								buffer = &mOutPortBuffers[bufferPointer];
							} else return false;
						} else {
							if (__builtin_expect(__CELL_DAISY_SPU_GET_VAL__(mInPortDataReady), 1)) {
								buffer = (OutPortDataType *)mInPortData;
							} else return false;
						}
						ret = mOutPort[mOutPortBeginPointer]->tryBeginPop(buffer);
					} else {
						ret = mOutPort[mOutPortBeginPointer]->tryBeginPop();
					}
				} else {
					if (tOutPort::sBufferMode == COPY) {
						ret = mOutPort[mOutPortBeginPointer]->beginPop(buffer);
					} else {
						ret = mOutPort[mOutPortBeginPointer]->beginPop();
					}
				}
				if (__builtin_expect(ret < 0, 0)) {
					/* CELL_DAISY_ERROR_AGAIN : cannot block        */
					/* CELL_DAISY_ERROR_BUSY  : PPU is initializing */
					return ret;
				} else if (__builtin_expect(ret == CELL_OK, 1)) {
					if (tOutPort::sBufferMode == COPY) {
						mPending++;
					}
					CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_OUT, mOutPortBeginPointer);
					CELL_DAISY_DEBUG_PRINTF("glue: mOutPort[%d]->tryBeginPop\n", mOutPortBeginPointer);
					OutPortBuffer popData = {0,0,0,0};
					popData.mPort = mOutPortBeginPointer;
					if (tOutPort::sBufferMode == COPY) {
						if (CELL_DAISY_GLUE_USE_OUT_PORT_BUFFERS) {
							*mOutPortBufferUseMask |= 1 << (31 - bufferPointer);
						} else {
							__CELL_DAISY_SPU_SET_VAL__(mInPortDataReady, false);
						}
						popData.mBuffer  = buffer;
						popData.mPointer = bufferPointer;
						mOutPortBufferQueue[mOutPortBeginPointer].pushPtr(popData);
					} else {
						popData.mBuffer =
							(OutPortDataType *)(uintptr_t)mOutPort[mOutPortBeginPointer]->getCurrentReference();
						mOutPortDataReadyQueue.pushPtr(popData);
					}
					ret = GLUE_ACTIVE;
					break;
				} else if (again) break;
			}

			return ret;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			int Glue<tOutPort, tInPort, tMode,
					 tOutPortBufferCount, tInPortBufferCount>::beginPush(BlockMode mode, int isRetry)
		{
			int ret = 0;
			if (__builtin_expect((tInPort::sBufferMode != COPY) || !mPostCalculatedDataQueue.isEmpty(), 1)) {
				InPortDataType *buffer = NULL;
				for(unsigned again = false;
					!again ||
						((mode == STALL) && (tInPort::sQueueControlType != QCTL_TYPE_LOCAL) && isRetry &&
						 !mPending);
					again = true)
				{
					if (__builtin_expect(!again, 1)) {
						if (tInPort::sBufferMode == COPY) {
							buffer = mPostCalculatedDataQueue.getFront().mBuffer; /* buffer to pe pushed */
							ret = mInPort[mInPortBeginPointer]->tryBeginPush(buffer);
						} else {
							ret = mInPort[mInPortBeginPointer]->tryBeginPush();
						}
					} else {
						if (tInPort::sBufferMode == COPY) {
							ret = mInPort[mInPortBeginPointer]->beginPush(buffer);
						} else {
							ret = mInPort[mInPortBeginPointer]->beginPush();
						}
					}
					if (__builtin_expect(ret < 0, 0)) {
						/* CELL_DAISY_ERROR_AGAIN : cannot block        */
						/* CELL_DAISY_ERROR_BUSY  : PPU is initializing */
						return ret;
					} else if (__builtin_expect(ret == CELL_OK, 1)) {
						if (tInPort::sBufferMode == COPY) {
							mPending++;
						}
						CELL_DAISY_BOOKMARK_CLASS(CELL_DAISY_BOOKMARK_PREFIX_GLUE_IN, mInPortBeginPointer);
						CELL_DAISY_DEBUG_PRINTF("glue: mInPort[%d]->tryBeginPush\n", mInPortBeginPointer);
						if (tInPort::sBufferMode == COPY) {
							mInPortBufferQueue[mInPortBeginPointer].pushPtr(
								mPostCalculatedDataQueue.popPtr()
								);
							mInPortDataReadyPort = mInPortBeginPointer;
						} else {
							__CELL_DAISY_SPU_SET_VAL__(mInPortDataReady, true);
							mInPortData = (InPortDataType *)(uintptr_t)mInPort[mInPortBeginPointer]->getCurrentReference();
							mInPortDataReadyPort = mInPortBeginPointer;
						}
						ret = GLUE_ACTIVE;
						break;
					}
				}
			}

			return ret;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			bool Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::proceedOutPortBegin(BitmapType busyMask)
		{
			uint32_t i=0;
			do {
				if (++i > mOutPortCount) return false;
				proceed(mOutPortBeginPointer, mOutPortCount, 0);
			} while(__builtin_expect((busyMask & (1 << mOutPortBeginPointer)) != 0 |
									 (mOutPortTerminatedMask & (1 << mOutPortBeginPointer)) != 0, 0));
			return true;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			bool Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::proceedOutPortEnd(BitmapType busyMask)
		{
			uint32_t i=0;
			do {
				if (++i > mOutPortCount) return false;
				proceed(mOutPortEndPointer, mOutPortCount, 0);
			} while(__builtin_expect((busyMask & (1 << mOutPortEndPointer)) != 0 |
									 (mOutPortTerminatedMask & (1 << mOutPortEndPointer)) != 0, 0));
			return true;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			bool Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::proceedInPortBegin(BitmapType busyMask,
																				   const InPortDataType *data)
		{
			uint32_t i=0;
			do {
				if (++i > mInPortCount) return false;
				proceed(mInPortBeginPointer, mInPortCount, data);
			} while(__builtin_expect((busyMask & (1 << mInPortBeginPointer)) != 0 |
									 (mInPortTerminatedMask & (1 << mInPortBeginPointer)) != 0, 0));
			return true;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			bool Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::proceedInPortEnd(BitmapType busyMask)
		{
			uint32_t i=0;
			do {
				if (++i > mInPortCount) return false;
				proceed(mInPortEndPointer, mInPortCount, 0);
			} while(__builtin_expect((busyMask & (1 << mInPortEndPointer)) != 0 |
									 (mInPortTerminatedMask & (1 << mInPortEndPointer)) != 0, 0));
			return true;
		}

		template<class tOutPort, class tInPort, GlueMode tMode,
				 SizeType tOutPortBufferCount, SizeType tInPortBufferCount>
		__attribute__((always_inline)) inline
			void Glue<tOutPort, tInPort, tMode,
					  tOutPortBufferCount, tInPortBufferCount>::proceed(PointerType& pointer,
																		const SizeType size, const InPortDataType *data)
		{
			(void)data;
			pointer++;
			if (__builtin_expect(pointer == (PointerType)size, 0)) {
				pointer = 0;
			}
		}

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
