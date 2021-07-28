/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::SignalNotification - signal notification queue control class base
 */

#ifndef __CELL_DAISY_SNR_QCTL_H__
#define __CELL_DAISY_SNR_QCTL_H__

#include <cell/daisy/qctl.h>
#include <cell/daisy/nbsnr.h>

#undef CELL_DAISY_DEBUG_PRINTF
#define CELL_DAISY_DEBUG_PRINTF(...)
#ifdef CELL_DAISY_DEBUG_SNR_QCTL
#include <cell/daisy/daisy_debug.h>
#endif

namespace cell {
	namespace Daisy {
		namespace QueueControl {    
			static const int MAX_PIPE = 4;
    
			/*  ==== Message Data format ====
			 * byte    0     1     2     3
			 *      +-+-+--+-----+-----+-----+
			 *      |v|e|id|      data       |
			 *      +-+-+--+-----+-----+-----+
			 *      bit0    : message valid
			 *      bit1    : end mark
			 *      bit2:7  : pipe_id
			 *      bit8:31 : message data
			 */

#define CELL_DAISY_NO_MESSAGE 0
    
			enum MessageFormat {
				SHIFT_MSG_PIPEID = 24,
				MASK_PIPEID      = 0x3,
				MASK_MSG_PIPEID  = 0x3000000,
				MASK_MSG_VALID   = 0x80000000,
				MASK_MSG_DATA    = 0x00FFFFFF,
				MASK_END_MARK    = 0x40000000
			};
    
#define CELL_DAISY_MSG2PIPEID(receiveMessage)  ((receiveMessage >> SHIFT_MSG_PIPEID) & MASK_PIPEID)
#define CELL_DAISY_MSG2DATA(receiveMessage)    ( receiveMessage & MASK_MSG_DATA )
#define CELL_DAISY_GENERATE_MESSAGE(pipe_id, data)                           \
  (                                                      \
   (((data)==0)?(enum MessageFormat)0:MASK_MSG_VALID) |  \
   ((pipe_id)<<SHIFT_MSG_PIPEID) |                       \
   ((data)&MASK_MSG_DATA)                                \
   )
    
			/* Message sent status */
			enum SendMessageStatus {
				MSG_SENT         = 1,
				NO_MSG_SENT      = 0,
				MSG_QUE_CTL_FULL = -1
			};
    
			/* Message receiver stub */
			class MessageReceiverStub {
      
			public:
      
				QueueIO    mQueueIoVal __attribute__((aligned(16))); /* setup when constructed */
      
				/* constructor definition */
      
				MessageReceiverStub(){}

				/* virtual destructor */
				virtual ~MessageReceiverStub(){}
      
				/* member function definition */
      
				/* handle received message */
				virtual void handleMessage(uint32_t message){(void)message;}
      
			};
    
			static MessageReceiverStub
				*sSnr1DefinedQueue[MAX_PIPE] __attribute__((aligned(16)));
			static MessageReceiverStub
				*sSnr2DefinedQueue[MAX_PIPE] __attribute__((aligned(16)));
    
			/* initialize signal notification */
			void initializeSignalNotification(
				uint64_t parameterEa,
				int mySpuNum,
				int groupNum
			);

			/* sequential pipe id for auto alloc */
			static uint32_t sSeqReceiveSnr1PipeId = 0;
			static uint32_t sSeqReceiveSnr2PipeId = 0;

			template<SizeType tSize, QueueIO tQueueIO,
						ConstructorMode tConstructorMode = NO_PARAMETER>
			class SignalNotification:
				public Abstract<tSize, tQueueIO>,
				public MessageReceiverStub  
			{
			private:

				/* static assertion */
				static int sAssertDummy[
					(tSize <= 12)
					? 1 : -1];

				/*    sender -> receiver notification = SNR1
				 *    sender <- receiver notification = SNR2
				 */

			protected:

				/* tail pointer points the last queue entry Producer completed */
				PointerType    mTailPointer      __attribute__((aligned(16)));
      
				/* head pointer points the last queue entry Consumer started */
				PointerType    mHeadPointer      __attribute__((aligned(16)));
      
				/* Consumer consumed mask */
				/* (bit i='1' stands for queue#i's consume completion) */
				BitmapType mConsumeMask          __attribute__((aligned(16)));
      
				/* Producer consumed mask */
				/* (bit i='1' stands for queue#i's produce completion) */
				BitmapType mProduceMask          __attribute__((aligned(16)));
      
				/* Next head pointer mask */
				BitmapType mPendingHeadCountMask __attribute__((aligned(16)));
      
				/* completed transfer number */
				uint32_t mPendingTailCount       __attribute__((aligned(16)));
				BitmapType mCompleteNumberMask   __attribute__((aligned(16)));

				/* flag indicating if next_mTailPointer is already invoked before */
				int mStartFlag                   __attribute__((aligned(16)));

				/* flag indicating queue is already terminated */
				int mIsTerminated                __attribute__((aligned(16)));

				/* sender Pipe ID */
				uint32_t mSendPipeId             __attribute__((aligned(16)));
      
				/* receiver Pipe ID */
				uint32_t mReceivePipeId          __attribute__((aligned(16)));
      
				/* pointer to (already)defined queue array */
				MessageReceiverStub **mDefinedQueue  __attribute__((aligned(16)));
      
				/* DMA tag */
				uint32_t mDmaTag                 __attribute__((aligned(16)));
      
				/* SNR1/2 EA */
				uint64_t mSnrEa                  __attribute__((aligned(16)));

				int      mTerminatedAssert       __attribute__((aligned(16)));

				uint32_t mDmaTagRangeMask        __attribute__((aligned(16)));

				/* padding to align on 16bytes boundary */
				int      mDummy                  __attribute__((aligned(16)));
      
			private:

				/* initialize function */

				/* @param snrEa destination Signal Notification effective address
				 * @param dmaTag DMA tag used to send NonblockSnr
				 * @param sendPipeId Pipe ID used to send Signal Notification
				 * @param receivePipeId Pipe ID used to receive Signal Notification
				 */
				void initialize(
					uint64_t snrEa,
					uint32_t dmaTag,
					uint32_t sendPipeId,
					uint32_t receivePipeId
				);
      
			public:

				/* constructor definition(s) */
      
				explicit SignalNotification(){}
      
				/* Manual association constructor
				 * @param snrEa destination Signal Notification effective address
				 * @param dmaTag DMA tag used to send NonblockSnr
				 * @param sendPipeId Pipe ID used to send Signal Notification
				 * @param receivePipeId Pipe ID used to receive Signal Notification
				 */
				explicit SignalNotification(
					uint64_t snrEa,
					uint32_t dmaTag,
					uint32_t sendPipeId,
					uint32_t receivePipeId
				);
      
				/* Automatic association with shared memory
				 * @param parameterEa paramter exchange buffer EA
				 * @param dmaTag DMA tag used to send NonblockSnr
				 * @param receivePipeId Pipe ID used to receive Signal Notification
				 */
				explicit SignalNotification(
					uint64_t parameterEa, /* 128bytes */
					uint32_t dmaTag,
					int receivePipeId = -1
				);
	
				/* member function definitions */
      
				virtual const char  *getClassName()
					{
#ifdef CELL_DAISY_DEBUG_SNR_QCTL
						static char __buf[64];
						cell::Daisy::_snprintf(__buf, 64,
											   "SignalNotification(%s)[pipe_id:(%d,%d),tSize:%d]",
											   tQueueIO == INPUT ? "In" : "Out",
											   this->mSendPipeId,
											   this->mReceivePipeId,
											   tSize);
						return (const char *)__buf;
#else
						return 0;
#endif
					}

				void setDmaTagRangeMask(const uint32_t dmaTagRangeMask) {
					mDmaTagRangeMask = dmaTagRangeMask;
				}
      
			protected:
      
				/* send message by NonblockSnr
				 * @param pointer data(=1<<pointer) is sent with tag=mDmaTag+ptr
				 * @retval always MSG_SENT
				 */
				__attribute__((always_inline)) inline
					int sendMessage(uint32_t pointer)
					{
						cellDaisyAssert(mTerminatedAssert==0);
						cellDaisyNbsnrSndsigf(
							CELL_DAISY_GENERATE_MESSAGE(this->mSendPipeId, 1<<pointer),
							mSnrEa, (mDmaTag+(pointer&mDmaTagRangeMask))%32, 0, 0);
						CELL_DAISY_DEBUG_PRINTF(
							"%s(sendMessage): Signal(=0x%x) sent to 0x%llx with tag=0x%x\n",
							getClassName(),
							CELL_DAISY_GENERATE_MESSAGE(this->mSendPipeId, 1<<pointer),
							mSnrEa, (mDmaTag+(pointer&mDmaTagRangeMask))%32);
						return MSG_SENT;
					}
      
				/* return message if it exists in Nonblocksnr
				 * @retval message read
				 */
				__attribute__((always_inline)) inline
					uint32_t receiveMessage()
					{
						uint32_t msg;
						if (tQueueIO == INPUT) {
							msg = cellDaisyNbsnrReadSignal2IfExists();
							CELL_DAISY_DEBUG_PRINTF(
								"%s(receiveMessage): readSignal2 = 0x%x\n",
								getClassName(), msg);
						} else {
							msg = cellDaisyNbsnrReadSignal1IfExists();
							CELL_DAISY_DEBUG_PRINTF(
								"%s(receiveMessage): readSignal1 = 0x%x\n",
								getClassName(), msg);
						}
						return msg;
					}
      
				virtual const char *getTargetName(){return (const char *)"---";}
      
				void handleMessage(uint32_t message);
      
			public:
      
				virtual void dump(const char *);
      
				__attribute__((always_inline)) inline
					uint32_t getTag(){return mDmaTag;}

				__attribute__((always_inline)) inline
					uint32_t getCompleteNumber(){ return mPendingTailCount - tSize + 1; }
      
				void beginCompleteConsume(uint32_t sequenceNumber);

				/* Get next Head pointer */
				PointerType  getNextHeadPointer(BlockMode stall);
      
				void beginCompleteProduce(uint32_t sequenceNumber);
      
				/* Get next Tail pointer */
				PointerType  getNextTailPointer(BlockMode stall);

				/* Terminate produce */
				int terminate();

				/* Check if there are any unfinished consumers
				 * @param isCancelled true if last operation was cancel
				 * @retval true: unfinished consumers exist
				 */
				bool hasUnfinishedConsumer(bool isCancelled);

				static const QueueControlType sQueueControlType = QCTL_TYPE_SIGNAL_NOTIFICATION;
				static const SizeType sQueueMaxSize = 12;
      
			};

			/* initialize function */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			inline void SignalNotification<tSize, tQueueIO, tConstructorMode>::initialize(
				uint64_t snrEa,
				uint32_t dmaTag,
				uint32_t sendPipeId,
				uint32_t receivePipeId
			)
			{
				mTailPointer          = tSize - 1;
				mHeadPointer          = tSize - 1;
				mConsumeMask          = 0;
				mProduceMask          = 0;
				mPendingHeadCountMask = 1;
				mPendingTailCount     = tSize - 1;
				mCompleteNumberMask   = 1;
				mStartFlag            = 0;
				mIsTerminated         = 0;
				mSnrEa                = snrEa;
				mDmaTag               = dmaTag;
				mSendPipeId           = sendPipeId;
				mReceivePipeId        = receivePipeId;
      			mQueueIoVal           = tQueueIO;
				if (tQueueIO == INPUT) {
					sSnr2DefinedQueue[mReceivePipeId] = this;
					mDefinedQueue         = sSnr2DefinedQueue;
				} else {
					sSnr1DefinedQueue[mReceivePipeId] = this;
					mDefinedQueue         = sSnr1DefinedQueue;
				}
				mTerminatedAssert     = 0;
				mDmaTagRangeMask      = 0xffffffff;

				dump(" <-- constructor");

			}
    
			/* constructor descriptions */
    
			template<SizeType tSize, QueueIO tQueueIO,
						ConstructorMode tConstructorMode>
			SignalNotification<tSize, tQueueIO, tConstructorMode>::SignalNotification(
				uint64_t snrEa,
				uint32_t dmaTag,
				uint32_t sendPipeId,
				uint32_t receivePipeId
			)
			{

				cellDaisyAssert(tConstructorMode == NO_PARAMETER);
				if (tQueueIO == INPUT) {
					cellDaisyAssert(
					((uint32_t)snrEa & 0xfffffU) == (uint32_t)SYS_SPU_THREAD_SNR1);
				} else {
					cellDaisyAssert(
					((uint32_t)snrEa & 0xfffffU) == (uint32_t)SYS_SPU_THREAD_SNR2);
				}
				cellDaisyAssert(dmaTag <= CELL_DAISY_MAX_DMA_TAG);
				cellDaisyAssert(sendPipeId < (uint32_t)MAX_PIPE);
				cellDaisyAssert(receivePipeId < (uint32_t)MAX_PIPE);
				cellDaisyAssert(cellDaisyNbsnrGetMySpunum() >= 0);

				initialize(snrEa, dmaTag, sendPipeId, receivePipeId);
			}
    
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			SignalNotification<tSize, tQueueIO, tConstructorMode>::SignalNotification(
				uint64_t parameterEa, /* 128bytes */
				uint32_t dmaTag,
				int receivePipeId
			)
			{

				cellDaisyAssert(tConstructorMode == PARAMETER);
				cellDaisyAssert(CELL_DAISY_EA_ATOMIC((uint32_t)parameterEa));
				cellDaisyAssert(dmaTag <= CELL_DAISY_MAX_DMA_TAG);

				int mySpuNum = cellDaisyNbsnrGetMySpunum();
				cellDaisyAssert(mySpuNum >= 0);

				/* Atomic LS buffer */
				uint8_t param0[sizeof(ParameterType)+127];
      
				/* allocate param receive area */
				ParameterType *param;
				param = (ParameterType *)(((uintptr_t)param0 + 127) & ~127);
      
				if (receivePipeId >= 0) {
					/* pipe id specified */
					CELL_DAISY_DEBUG_PRINTF(
						"%s(Constructor): mReceivePipeId = %d\n",
						getClassName(), receivePipeId);
				} else {
					/* automatic pipe id allocation */
					CELL_DAISY_DEBUG_PRINTF(
						"%s(Constructor): receivePipeId autoallocated(= %d)\n",
						getClassName(), (tQueueIO==INPUT) ? sSeqReceiveSnr2PipeId : sSeqReceiveSnr1PipeId);
					if (tQueueIO == INPUT) {
						receivePipeId = sSeqReceiveSnr2PipeId++;
					} else {
						receivePipeId = sSeqReceiveSnr1PipeId++;
					}
				}
				cellDaisyAssert(receivePipeId < MAX_PIPE);

				/* set receiver's parameter */
				do {
					mfc_getllar((void *)param, parameterEa, 0, 0);
					mfc_read_atomic_status();
					spu_dsync();
					cellDaisyAssert(param->depth == 0 || param->depth == tSize);
					param->depth = tSize;
					if (tQueueIO == OUTPUT) {
						cellDaisyAssert(param->snr1Ea == 0);
						cellDaisyAssert(param->sig1PipeId == 0);
						param->snr1Ea = CELL_DAISY_GET_SNR1_AREA(mySpuNum);
						param->sig1PipeId = receivePipeId;
					} else {
						cellDaisyAssert(param->snr2Ea == 0);
						cellDaisyAssert(param->sig2PipeId == 0);
						param->snr2Ea = CELL_DAISY_GET_SNR2_AREA(mySpuNum);
						param->sig2PipeId = receivePipeId;
					}
					spu_dsync();
					mfc_putllc((void *)param, parameterEa, 0, 0);
				} while (mfc_read_atomic_status());
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): set param done\n", getClassName());
      
				/* wait for sender's parameter */
				do {
					mfc_getllar((volatile void *)param, parameterEa, 0, 0);
					mfc_read_atomic_status();
					spu_dsync();
				} while ((tQueueIO == OUTPUT && param->snr2Ea == 0) ||
					   (tQueueIO == INPUT  && param->snr1Ea == 0));
				CELL_DAISY_DEBUG_PRINTF("%s(Constructor): param received\n", getClassName());
      
				/* reset reservation */
				mfc_putllc((volatile void *)param, parameterEa, 0, 0);
				mfc_read_atomic_status();
      
				/* call initializer */
				if (tQueueIO == OUTPUT) {
					initialize(
						param->snr2Ea,
						dmaTag,
						param->sig2PipeId,
						param->sig1PipeId
					);
				} else {
					initialize(
						param->snr1Ea,
						dmaTag,
						param->sig1PipeId,
						param->sig2PipeId
					);
				}
			}

			/* member functions descriptions */
    
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			void SignalNotification<tSize, tQueueIO, tConstructorMode>::dump(const char *messages)
			{
#ifndef CELL_DAISY_DEBUG_SNR_QCTL
				(void)messages;
#else
      
				static char __dump_buf[256];
      
				cell::Daisy::_snprintf(__dump_buf, 256,
									   "%s,mTailPointer=%d,mHeadPointer=%d,mPointer=%d, comp_num = %d[%s],"
									   "mConsumeMask =[%s],mProduceMask=[%s],pipe_id=(%d,%d),mDmaTag=0x%x,"
									   "mSnrEa=0x%llx,mIsTerminated=%s,mDmaTagRangeMask=0x%x",
									   tQueueIO==INPUT?"INPUT":"OUTPUT",
									   mTailPointer,
									   mHeadPointer,
									   this->getPointer(),
									   mPendingTailCount - tSize + 1,
									   getMaskString(mCompleteNumberMask, tSize),
									   getMaskString(mConsumeMask, tSize),
									   getMaskString(mProduceMask, tSize),
									   mSendPipeId, mReceivePipeId,
									   mDmaTag,
									   mSnrEa,
									   mIsTerminated?"Terminated":"Running",
									   mDmaTagRangeMask
					);
				strcat(__dump_buf, messages);
				CELL_DAISY_DEBUG_PRINTF("%s %s\n", getClassName(), __dump_buf);
#endif
			}
    
			/* beginCompleteProduce
			 * @param sequenceNumber packet# that is produced by Producer
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				void SignalNotification<tSize, tQueueIO, tConstructorMode>::beginCompleteProduce(uint32_t sequenceNumber)
			{

				/* send Sig1 to Consumer */
				CELL_DAISY_DEBUG_PRINTF("%s(bedingCompleteProduce): sendMessage(ptr=0x%x)\n",
										getClassName(),this->getPointerFromSequenceNumber((int)sequenceNumber));
				sendMessage(this->getPointerFromSequenceNumber((int)sequenceNumber));
			}
    
			/* beginCompleteConsume
			 * @param sequenceNumber packet# that is completed by Consumer
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				void SignalNotification<tSize, tQueueIO, tConstructorMode>::beginCompleteConsume(uint32_t sequenceNumber)
			{
				CELL_DAISY_DEBUG_PRINTF("%s(bedingCompleteConsume): sendMessage(ptr=0x%x)\n",
										getClassName(),this->getPointerFromSequenceNumber((int)sequenceNumber));
				sendMessage(this->getPointerFromSequenceNumber((int)sequenceNumber));
			}
    
			/* return 0 if x&mask != mask_v, otherwise pass through */
			__attribute__((always_inline)) inline
				vec_uint4 filter_out_masked_unmatch(BitmapType x, BitmapType mask, BitmapType mask_v)
			{
				register vec_uint4 vec_x = spu_promote(x,0);
				return spu_and(vec_x, spu_cmpeq(spu_and(vec_x, mask), mask_v));
			}

			/* getNextHeadPointer
			 * @retval Next head Queue pointer
			 *   PTR_UNAVAILABLE(=-1) or PTR_TERMINATED(=-2) if pointer not available
			 * @param stall STALL    (=1)  : stall until pointer is available
			 *              NOT_STALL(=0)  : not stall until pointer is available
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				PointerType SignalNotification<tSize, tQueueIO, tConstructorMode>::getNextHeadPointer(BlockMode stall)
			{
				register PointerType rHeadPonter           = mHeadPointer;
				register BitmapType  rPendingHeadCountMask = mPendingHeadCountMask;
				register BitmapType  rProduceMask          = mProduceMask;
				register PointerType rPointer              = PTR_UNAVAILABLE;
				register uint32_t    msgData;
				register uint32_t    msgDataMasked;

				if (__builtin_expect(mIsTerminated != 0 && (rProduceMask == 0), 0)) {
					return PTR_TERMINATED;
				}

				do {

					msgData = receiveMessage();
					msgDataMasked =
						spu_extract(
							filter_out_masked_unmatch(
								msgData,
								MASK_MSG_VALID | MASK_MSG_PIPEID,
								MASK_MSG_VALID | CELL_DAISY_GENERATE_MESSAGE(mReceivePipeId, CELL_DAISY_NO_MESSAGE)
							), 0);
	
					if (__builtin_expect(msgDataMasked != CELL_DAISY_NO_MESSAGE, 1)) {
						/* message for myself */
						if (__builtin_expect((msgDataMasked & MASK_END_MARK)!=0, 0)) {
							mIsTerminated = 1;
							if (__builtin_expect(rProduceMask == 0, 0)) {
								return PTR_TERMINATED;
							}
						}
						CELL_DAISY_DEBUG_PRINTF(
							"%s(receiveMessage): msgData=0x%08x\n",
							getClassName(),
							msgData);
						register BitmapType newProduceMask = CELL_DAISY_MSG2DATA(msgData);
						/* update mPendingHeadCount */
						if (__builtin_expect(newProduceMask == rPendingHeadCountMask, 1)) {
							CELL_DAISY_ROTATE_LEFT1(rPendingHeadCountMask);
							rPointer = rHeadPonter = CELL_DAISY_INCREMENT_POINTER(rHeadPonter);
							CELL_DAISY_DEBUG_PRINTF(
								"%s(%s:getNextHeadPointer(%s)): mPointer=%d, mProduceMask=[%s]\n",
								getClassName(), getTargetName(), stall==STALL?"STALL":"NON-STALL",
								rPointer, getMaskString(mProduceMask, tSize));
							break;
						} else {
							rProduceMask |= newProduceMask;
						}

					} else if (msgData != 0) {
						/* message for other pipe */
						CELL_DAISY_DEBUG_PRINTF(
							"%s(receiveMessage): msgData=0x%08x for other pipe\n",
							getClassName(),
							msgData);
						mDefinedQueue[CELL_DAISY_MSG2PIPEID(msgData)]->handleMessage(msgData);
					}

					if (__builtin_expect((rProduceMask&rPendingHeadCountMask)!=0, 1)) {
						rProduceMask &= ~rPendingHeadCountMask;
						CELL_DAISY_ROTATE_LEFT1(rPendingHeadCountMask);
						rPointer = rHeadPonter = CELL_DAISY_INCREMENT_POINTER(rHeadPonter);
						CELL_DAISY_DEBUG_PRINTF(
							"%s(%s:getNextHeadPointer(%s)): mPointer=%d, mProduceMask=[%s]\n",
							getClassName(), getTargetName(), stall==STALL?"STALL":"NON-STALL",
							rPointer, getMaskString(mProduceMask, tSize));
						break;
					}

				} while (__builtin_expect((rPointer == PTR_UNAVAILABLE) && (stall == STALL), 0));

				/* write back */
				mProduceMask          = rProduceMask;
				this->setPointer(rPointer);
				mHeadPointer          = rHeadPonter;
				mPendingHeadCountMask = rPendingHeadCountMask;
      
				return rPointer;
			}

			/* mark mConsumeMask,mProduceMask, or mIsTerminated
			 * @param message received message
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				void SignalNotification<tSize, tQueueIO, tConstructorMode>::handleMessage(uint32_t message)
			{
				uint32_t data = CELL_DAISY_MSG2DATA(message);

				if (tQueueIO == INPUT) {
					/* accumulate received new consumeMask to mConsumeMask */
					mConsumeMask |= data;
					CELL_DAISY_DEBUG_PRINTF(
						"%s(%s:handleMessage(%s)): mConsumeMask changed = [%s]\n",
						getClassName(), getTargetName(),
						getMaskString(message,tSize), getMaskString(mConsumeMask,tSize));
					
				} else if (tQueueIO == OUTPUT) {
					if (__builtin_expect((message&MASK_END_MARK)!=0,0)) {
						/* mark mIsTerminated = true */
						mIsTerminated = 1;
					} else {
						/* accumulate received new produceMask to mProduceMask */
						mProduceMask |= data;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(%s:handleMessage(%s)): mProduceMask changed = [%s]\n",
							getClassName(), getTargetName(),
							getMaskString(message, tSize), getMaskString(mProduceMask, tSize));
					}
				}
			}
    
			/* getNextTailPointer
			 * @retval Next tail Queue pointer, PTR_UNAVAILABLE(=-1) if pointer not available
			 * @param stall STALL(=1)     : stall until pointer is available
			 *              NOT_STALL(=0) : not stall until pointer is available
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				PointerType SignalNotification<tSize, tQueueIO, tConstructorMode>::getNextTailPointer(BlockMode stall)
			{
				/* check if this is first invokation */
				if (__builtin_expect(mStartFlag == 0, 0)) {
					mStartFlag = 1;
					this->setPointer(0);
					return 0;
				}

				register PointerType rTailPointer          = mTailPointer;
				register BitmapType  rConsumeMask          = mConsumeMask;
				register BitmapType  rCompleteNumberMask   = mCompleteNumberMask;
				register uint32_t    rPendingTailCount     = mPendingTailCount;
      
				/* get next tail pointer */
				register PointerType rTailPointerPlus1;
				register PointerType rPointer;
				rTailPointer          = CELL_DAISY_INCREMENT_POINTER(rTailPointer);
				rTailPointerPlus1     = CELL_DAISY_INCREMENT_POINTER(rTailPointer);
				rPointer              = PTR_UNAVAILABLE;

				register uint32_t msgData;
				register uint32_t msgDataMasked;

				do {

					/* check if mTailPointer is available */
					if (__builtin_expect(rPendingTailCount > 0, 0)) {
						rPendingTailCount--;
						/* write back */
						mTailPointer = rTailPointer;
						rPointer     = rTailPointerPlus1;
						CELL_DAISY_DEBUG_PRINTF(
							"%s(%s:getNextTailPointer(%s)): mPointer=%d, mConsumeMask=[%s], mPendingTailCount=%d\n",
								getClassName(), getTargetName(), stall==STALL?"STALL":"NON-STALL",
								rPointer, getMaskString(mConsumeMask, tSize), mPendingTailCount);
						break;
					}

					msgData = receiveMessage();
					msgDataMasked =
						spu_extract(
							filter_out_masked_unmatch(
								msgData,
								MASK_MSG_VALID | MASK_MSG_PIPEID,
								MASK_MSG_VALID | CELL_DAISY_GENERATE_MESSAGE(mReceivePipeId, CELL_DAISY_NO_MESSAGE)
							), 0);
	
					if (__builtin_expect(msgDataMasked != CELL_DAISY_NO_MESSAGE, 1)) {
						/* message for myself */
						CELL_DAISY_DEBUG_PRINTF(
							"%s(receiveMessage): msgData=0x%08x\n",
							getClassName(),
							msgData);
						register BitmapType newConsumeMask = CELL_DAISY_MSG2DATA(msgData);
						/* update mPendingTailCount for remaining mConsumeMask */
						if (__builtin_expect(newConsumeMask == rCompleteNumberMask, 1)) {
							/* mConsumeMask clear */
							CELL_DAISY_ROTATE_LEFT1(rCompleteNumberMask);
							
							/* write back */
							mTailPointer          = rTailPointer;
							rPointer              = rTailPointerPlus1;
							break;
						} else {
							rConsumeMask |= newConsumeMask;
						}

					} else if (msgData != 0) {
						/* message for other pipe */
						CELL_DAISY_DEBUG_PRINTF(
							"%s(receiveMessage): msg(=0x%08x) for other pipe\n",
							getClassName(),
							msgData);
						mDefinedQueue[CELL_DAISY_MSG2PIPEID(msgData)]->handleMessage(msgData);
					}

					if (__builtin_expect((rConsumeMask&rCompleteNumberMask) != 0, 1)) {
						rConsumeMask &= ~rCompleteNumberMask;
						CELL_DAISY_ROTATE_LEFT1(rCompleteNumberMask);
						
						/* write back */
						mTailPointer = rTailPointer;
						rPointer     = rTailPointerPlus1;
						break;
					}

				} while (__builtin_expect((rPointer == PTR_UNAVAILABLE) && (stall == STALL), 0));
      
				/* write back */
				mCompleteNumberMask = rCompleteNumberMask;
				mPendingTailCount   = rPendingTailCount;
				mConsumeMask        = rConsumeMask;
				this->setPointer(rPointer);
      
				CELL_DAISY_DEBUG_PRINTF(
					"%s(%s:getNextTailPointer(%s)): mPointer=%d, mTailPointer=%d, mConsumeMask=[%s], comp_num=%d\n",
						getClassName(),
						getTargetName(),
						stall == STALL ? "STALL" : "NON-STALL",
						this->getPointer(),
						mTailPointer,
						getMaskString(mConsumeMask, tSize),
						mPendingTailCount - tSize + 1);
      
				return rPointer;
			}

			/* Terminate produce */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				int SignalNotification<tSize, tQueueIO, tConstructorMode>::terminate()
			{
				cellDaisyAssert(mTerminatedAssert==0);
				mTerminatedAssert=1;
				/* send end mark */
				cellDaisyNbsnrSndsigf(CELL_DAISY_GENERATE_MESSAGE(this->mSendPipeId, CELL_DAISY_NO_MESSAGE) |
									  MASK_END_MARK,
									  mSnrEa, 31, 0, 0);
				return CELL_OK;
			}

			/* Check if there are any unfinished consumers
			 * @param isCancelled true if last operation was cancel
			 * @retval true: unfinished consumers exist
			 */
			template<SizeType tSize, QueueIO tQueueIO, ConstructorMode tConstructorMode>
			__attribute__((always_inline)) inline
				bool SignalNotification<tSize, tQueueIO, tConstructorMode>::hasUnfinishedConsumer(bool isCancelled)
			{
				register uint32_t msgData;
				register uint32_t msgDataMasked;

				msgData = receiveMessage();
				msgDataMasked =
					spu_extract(
						filter_out_masked_unmatch(
							msgData,
							MASK_MSG_VALID | MASK_MSG_PIPEID,
							MASK_MSG_VALID | CELL_DAISY_GENERATE_MESSAGE(mReceivePipeId, CELL_DAISY_NO_MESSAGE)
							), 0);
	
				if (__builtin_expect(msgDataMasked != CELL_DAISY_NO_MESSAGE, 1)) {
					/* message for myself */
					CELL_DAISY_DEBUG_PRINTF(
						"%s(receiveMessage): msgData=0x%08x\n",
						getClassName(),
						msgData);
					if ((mConsumeMask&MASK_END_MARK)==0) {
						mConsumeMask |= CELL_DAISY_MSG2DATA(msgData);
					}
				} else if (msgData != 0) {
					/* message for other pipe */
					CELL_DAISY_DEBUG_PRINTF(
						"%s(receiveMessage): msg(=0x%08x) for other pipe\n",
						getClassName(),
						msgData);
					
					mDefinedQueue[CELL_DAISY_MSG2PIPEID(msgData)]->handleMessage(msgData);
				}

				if (isCancelled) {
					// rotate right 1bit
					BitmapType oldMask = mCompleteNumberMask >> 1;
					if (oldMask == 0) oldMask=1 << (tSize-1);
					mConsumeMask |= oldMask; // revert cancelled mask
				}

				return (mStartFlag != 0 && (mConsumeMask != (1u<<(tSize-mPendingTailCount))-1u));
			}

		} /* namespace QueueControl */    
	} /* namespace Daisy */
} /* namespcae cell */

#endif /* __CELL_DAISY_SNR_QCTL_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
