/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* daisy_defs.h
 * common definitions for namespace cell::Daisy
 */

#ifndef __CELL_DAISY_DEFS_H__
#define __CELL_DAISY_DEFS_H__

#ifdef __SPU__
#include <sys/spu_thread.h>	/* SYS_SPU_THREAD_OFFSET */
#include <spu_printf.h>
#include <spu_intrinsics.h>
#include <sys/spu_thread.h>
#include <spu_mfcio.h>

/* MMIO EA */
#define CELL_DAISY_GET_LS_AREA(spuNum) (SYS_SPU_THREAD_OFFSET * spuNum + SYS_SPU_THREAD_BASE_LOW + SYS_SPU_THREAD_LS_BASE)
#define CELL_DAISY_GET_SNR1_AREA(spuNum) (SYS_SPU_THREAD_OFFSET * spuNum + SYS_SPU_THREAD_BASE_LOW + SYS_SPU_THREAD_SNR1)
#define CELL_DAISY_GET_SNR2_AREA(spuNum) (SYS_SPU_THREAD_OFFSET * spuNum + SYS_SPU_THREAD_BASE_LOW + SYS_SPU_THREAD_SNR2)

#endif /* __SPU__ */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <cell/daisy/return_code.h>
#include <cell/daisy/error.h>
#include <cell/daisy/daisy_assert.h>

/* rotate left 1bit */
#define CELL_DAISY_ROTATE_LEFT1(x) \
  do { \
  (x) <<= 1; \
  if (__builtin_expect((tSize != 32) && ((x) >= (uint32_t)(1ull << tSize)) || (tSize == 32) && ((x) == 0), 0)) x = 1;\
  } while (0)

/* identical to following expression
 * x = (x+1)%tSize
 */
#define CELL_DAISY_INCREMENT_POINTER(x) \
    ((tSize == 1 || tSize == 2 || tSize == 4 || tSize == 8 || tSize == 16 || tSize == 32 || tSize == 64 || tSize == 128) ? \
      (((x) + 1) % tSize) : \
      ((x) == (int)tSize - 1 ? 0 : (x) + 1))

#define CELL_DAISY_MAX_BOOKMARK_ID 8

#define __CELL_DAISY_COMPATIBILITY_MASK 0x2

#ifdef __SPU__
#define __CELL_DAISY_SPU_ALIGN16__ __attribute__((aligned(16)))
#define __CELL_DAISY_SPU_GET_VAL__(x) si_to_uint(x)
#define __CELL_DAISY_SPU_SET_VAL__(x, val) do { x = si_from_uint(val); } while(0)
#else
#define __CELL_DAISY_SPU_ALIGN16__
#define __CELL_DAISY_SPU_GET_VAL__(x) (x)
#define __CELL_DAISY_SPU_SET_VAL__(x, val) do { x = val; } while(0)
#endif

namespace cell {
	namespace Daisy {

		static char sCellDaisyTaskName[64] __attribute__((unused)) = "";

		enum QueueIO {
			OUTPUT,
			INPUT
		};
    
		enum BlockMode{
			NOT_STALL= 0,       /* Non-blocking mode */
			STALL    = 1        /* Blocking mode */
		};
    
		enum QueueStatus {
			PTR_UNAVAILABLE = 0x40000001, /* Queue has no available pointer */
			PTR_TERMINATED  = 0x40000002  /* Queue is terminated */
		};
    
		enum BufferMode {
			COPY       = 0,     /* Copy to buffer mode */
			REFERENCE  = 1      /* buffer reference mode */
		};
    
#ifdef __SPU__
		enum ConstructorMode {
			NO_PARAMETER = 0,   /* association without paramter */
			PARAMETER    = 1    /* association with parameter */
		};
#endif /* __SPU__ */

		enum GlueMode {
			TWO_PORT = 2   , /* 2-ports(input/output) calculation */
			ONE_PORT = 1     /* 1-port(inoutport) calculation */
		};
    
		enum QueueMode {
			IN_ORDER,           /* In-order begin/end */
			OUT_OF_ORDER        /* Our-of-order begin/end */
		};
		
		enum BufferType {
			BUFFER_TYPE_REMOTE   = 1,
			BUFFER_TYPE_LOCAL    = 2
		};
		enum QueueControlType {
			QCTL_TYPE_ATOMIC = 1,
			QCTL_TYPE_LOCAL  = 2,
			QCTL_TYPE_SIGNAL_NOTIFICATION   = 3
		};

		/* typedefs */

		typedef uint32_t SizeType;

		typedef int PointerType;

		typedef uint32_t BitmapType;

		typedef struct ParameterType {
			uint64_t bufferEa;
			uint64_t snr1Ea;
			uint64_t snr2Ea;
			uint32_t sig1PipeId;
			uint32_t sig2PipeId;
			uint32_t depth;
			uint32_t size;
			uint8_t pad[128
						-sizeof(uint64_t) * 3
						-sizeof(uint32_t) * 4];
		} __attribute__((aligned(128))) ParameterType;


		__attribute__((always_inline)) inline
		const char *getMaskString(unsigned int value, unsigned int size)
		{
			static char buf[4][33];
			static int ptr = 0;
			ptr %= 4;
			for(int i = 0; i < 32 && i < (int)size; i++) {
				buf[ptr][i] = value & (1 << i) ? 'o' : '-';
			}
			buf[ptr][size] = 0;
			return buf[ptr++];
		}

		/* Pointer Queue */
		template<typename tType>
		struct Fifo {

			/* array to hold pointer queue */
			tType mPtrFifo[32];
      
#ifdef __SPU__
			/* front pointer */
			qword mFrontPtr;
      
			/* back pointer */
			qword mBackPtr;
      
			/* queue size */
			qword mPendingSize;
#else /* __SPU__ */
			/* front pointer */
			int32_t mFrontPtr;
      
			/* back pointer */
			int32_t mBackPtr;
      
			/* queue size */
			uint32_t mPendingSize;
#endif /* __SPU__ */
      
			/* initialize queue */
			void initialize() {
				__CELL_DAISY_SPU_SET_VAL__(mFrontPtr, 0);
				__CELL_DAISY_SPU_SET_VAL__(mBackPtr, 0);
				__CELL_DAISY_SPU_SET_VAL__(mPendingSize, 0);
			}
      
			/* get front value */
			__attribute__((always_inline)) inline
			tType getFront()
				{
				  return mPtrFifo[__CELL_DAISY_SPU_GET_VAL__(mFrontPtr)];
				}
      
			/* get back value */
			__attribute__((always_inline)) inline
			tType getBack()
				{
					register int ptr = (__CELL_DAISY_SPU_GET_VAL__(mBackPtr) + 31) % 32;
					return mPtrFifo[ptr];
				}
      
			/* push pointer to back */
			__attribute__((always_inline)) inline
			void pushPtr(tType ptr)
				{
					mPtrFifo[__CELL_DAISY_SPU_GET_VAL__(mBackPtr)] = ptr;
#ifdef __SPU__
					mBackPtr = si_andi(si_ai(mBackPtr, 1), 0x1f);
					mPendingSize = si_ai(mPendingSize, 1);
#else
					mBackPtr = (mBackPtr + 1) & 0x1f;
					mPendingSize++;
#endif
				}
      
			/* pop pointer from front */
			__attribute__((always_inline)) inline
			tType popPtr()
				{
					tType val = getFront();
#ifdef __SPU__
					mFrontPtr = si_andi(si_ai(mFrontPtr, 1), 0x1f);
					mPendingSize = si_ai(mPendingSize, -1);
#else
					mFrontPtr = (mFrontPtr + 1) & 0x1f;
					mPendingSize--;
#endif

					return val;
				}
      
			/* get size */
			__attribute__((always_inline)) inline
			SizeType getSize(){ return __CELL_DAISY_SPU_GET_VAL__(mPendingSize); }
      
			/* check if queue is empty */
			__attribute__((always_inline)) inline
			bool isEmpty(){ return __CELL_DAISY_SPU_GET_VAL__(mPendingSize) == 0;  }
      
			/* check if queue is full */
			__attribute__((always_inline)) inline
			bool isFull(){ return __CELL_DAISY_SPU_GET_VAL__(mPendingSize) == 32;  }
      
		};

	} /* namespace Daisy */
} /* namespcae cell */


#endif /* __CELL_DAISY_DEFS_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
