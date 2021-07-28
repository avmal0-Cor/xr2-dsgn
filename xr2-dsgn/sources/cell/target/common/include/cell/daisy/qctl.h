/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* QueueControl::Abstract - queue control base class
 */

#ifndef __CELL_DAISY_QCTL_H__
#define __CELL_DAISY_QCTL_H__

#include <cell/daisy/daisy_defs.h>
#include <cell/daisy/snprintf.h>

namespace cell {
	namespace Daisy {
		namespace QueueControl {

			template<SizeType tSize, QueueIO tQueueIO>
			class Abstract {
			public:
				static const QueueControlType sQueueControlType; 
				static const QueueIO sPort = tQueueIO;
				static const SizeType sSize = tSize;
 			    static const SizeType sQueueMaxSize = 0x3fffffff;

			private:

				/* static assertion */
				static int sAssertDummy[
					(tSize != 0 &&
					 sQueueMaxSize < PTR_UNAVAILABLE &&
					 sQueueMaxSize < PTR_TERMINATED)
					? 1 : -1];

			protected:

				/* current pointer */
#ifdef __SPU__
				qword mPointer;
#else
				int32_t mPointer;
#endif

			public:
	
				__attribute__((always_inline)) inline
				uint32_t getTag(){return 0;}

				__attribute__((always_inline)) inline
				unsigned int isOutOfOrder() {
					return 0;
				}

				/* constructor definition */
				Abstract();

				/* virtual destructor definition */
				virtual ~Abstract(){}

				__attribute__((always_inline)) inline
					PointerType getPointerFromSequenceNumber(int sequenceNumber)
					{
						if (tSize == 1 || tSize == 2 || tSize == 4 || tSize == 8 ||
							tSize == 16 || tSize == 32) {
							return (sequenceNumber + tSize) % tSize;
						} else {
							if (__builtin_expect(sequenceNumber < (int)tSize && sequenceNumber >= 0, 1)) {
								return sequenceNumber;
							} else {
								return (sequenceNumber + tSize) % tSize;
							}
						}
					}
      
				__attribute__((always_inline)) inline
					PointerType getPointerFromSequenceNumber(uint64_t sequenceNumber)
					{
						if (tSize == 1 || tSize == 2 || tSize == 4 || tSize == 8 ||
							tSize == 16 || tSize == 32) {
							return sequenceNumber % tSize;
						} else {
							if (__builtin_expect(sequenceNumber < tSize, 1)) {
								return sequenceNumber;
							} else {
								return sequenceNumber % tSize;
							}
						}
					}
      
				__attribute__((always_inline)) inline
					BitmapType getMask(uint32_t sequenceNumber)
					{
						return 1 << getPointerFromSequenceNumber(sequenceNumber);
					}
      
				__attribute__((always_inline)) inline
					BitmapType getMask(uint64_t sequenceNumber)
					{
						return 1 << getPointerFromSequenceNumber(sequenceNumber);
					}
      
				virtual const char *getClassName()
					{
#if defined(CELL_DAISY_DEBUG_ATO_QCTL) || \
    defined(CELL_DAISY_DEBUG_SNR_QCTL) || \
    defined(CELL_DAISY_DEBUG_LQCTL)    || \
    defined(CELL_DAISY_DEBUG_PRIV_RW)  || \
    defined(CELL_DAISY_DEBUG_SHM_RW)
						static char __buf[64];
						cell::Daisy::_snprintf(__buf, 64, "Abstract(%s)[tSize:%d]",
											   tQueueIO == INPUT ? "In" : "Out",
											   tSize);
						return (const char *)__buf;
#else
						return 0;
#endif
					}
      
				virtual void dump(const char *messages = 0){(void)messages;};

				/* get mPointer
				 * @retval mPointer
				 */
				__attribute__((always_inline)) inline
				PointerType getPointer(){return (PointerType)__CELL_DAISY_SPU_GET_VAL__(mPointer) ;}

				/* set mPointer
				 * @param pointer
				 */
				__attribute__((always_inline)) inline
				void setPointer(PointerType pointer){
					__CELL_DAISY_SPU_SET_VAL__(mPointer, pointer);
				}

				/* get available entry num for getNextTailPointer()
				 * @retval available entry num for getNextTailPointer()
				 */
				__attribute__((always_inline)) inline
				uint32_t getCompleteNumber(){return 0;}
      
				/* Consumer packet#=sequenceNumber completed
				 * @param sequenceNumber
				 *    packet# that is completed by Consumer
				 */
				__attribute__((always_inline)) inline
				void beginCompleteConsume(uint32_t sequenceNumber){ (void)sequenceNumber; }
      
				/* Consumer packet#=sequenceNumber completed
				 * @param sequenceNumber
				 *    packet# that is completed by Consumer
				 */
				__attribute__((always_inline)) inline
				void endCompleteConsume(uint32_t sequenceNumber, unsigned int noQueueFull = false){
					(void)sequenceNumber;
					(void)noQueueFull;
				}
      
				/* Producer packet#=sequenceNumber completed
				 * @param sequenceNumber
				 *    packet# that is produced by Producer
				 */
				__attribute__((always_inline)) inline
				void beginCompleteProduce(uint32_t sequenceNumber){ (void)sequenceNumber; }
      
				/* Producer packet#=sequenceNumber completed
				 * @param sequenceNumber
				 *    packet# that is produced by Producer
				 */
				__attribute__((always_inline)) inline
				void endCompleteProduce(uint32_t sequenceNumber, unsigned int noQueueFull = false){
					(void)sequenceNumber;
					(void)noQueueFull;
				}
      
				/* Get Next Tail pointer
				 * @retval Next tail Abstract pointer if available,
				 *         otherwise PTR_UNAVAILABLE or PTR_TERMINATED
				 * @param stall
				 *    STALL      : stall until pointer becomes available
				 *    NOT_STALL  : not stall until pointer becomes available
				 */
				__attribute__((always_inline)) inline
				PointerType getNextTailPointer(BlockMode stall){(void)stall; return 0 ;}

#ifdef __SPU__      
				/* Set Next Tail pointer
				 * @retval CELL_OK or QUEUE_IS_BUSY
				 * @param stall
				 *    STALL      : stall until pointer becomes available
				 *    NOT_STALL  : not stall until pointer becomes available
				 * @param pointer poinnter to be set to tail pointer
				 */
				__attribute__((always_inline)) inline
				int setNextTailPointer(BlockMode stall, PointerType pointer){
					(void)stall;
					(void)pointer;
					return CELL_OK ;
				}
#endif /* __SPU__ */
      
				/* Get Next Head pointer
				 * @retval Next head Abstract pointer if available, otherwise PTR_UNAVAILABLE
				 * @param stall
				 *    STALL      : stall until pointer becomes available
				 *    NOT_STALL  : not stall until pointer becomes available
				 */
				__attribute__((always_inline)) inline
				PointerType getNextHeadPointer(BlockMode stall){(void)stall; return 0;}
      
				/* Terminate produce */
				__attribute__((always_inline)) inline
				int terminate(){ return CELL_OK; }

				/* Check if there are any unfinished consumers
				 * @param isCancelled true if last operation was cancel
				 * @retval true: unfinished consumers exist
				 */
				__attribute__((always_inline)) inline
				bool hasUnfinishedConsumer(bool isCancelled){(void)isCancelled; return false;}

			};

			/* constructor description */
			template<SizeType tSize, QueueIO tQueueIO>
			inline Abstract<tSize, tQueueIO>::Abstract():
#ifdef __SPU__
				mPointer   (si_from_int(PTR_UNAVAILABLE))
#else
				mPointer   (PTR_UNAVAILABLE)
#endif
			{}

		} /* namespace QueueControl */
	} /* namespace Daisy */
} /* namespcae cell */

#endif /* __CELL_DAISY_QCTL_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
