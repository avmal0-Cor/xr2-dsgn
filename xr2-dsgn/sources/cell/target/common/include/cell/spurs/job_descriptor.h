/* SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   * Copyright (C) 2009 Sony Computer Entertainment Inc.
   * All Rights Reserved.
   */

#ifndef __CELL_SPURS_JOB_DESCRIPTOR_H__
#define __CELL_SPURS_JOB_DESCRIPTOR_H__


#include <stdint.h>
#include <cell/spurs/error.h>

#define CELL_SPURS_MAX_SIZE_JOB_MEMORY (234*1024)
#define CELL_SPURS_JOBQUEUE_MAX_SIZE_JOB_MEMORY (221 * 1024)

#define CELL_SPURS_JOB_BINARY_CACHE_INHIBIT 1

// jobType
#define CELL_SPURS_JOB_TYPE_STALL_SUCCESSOR 1
#define CELL_SPURS_JOB_TYPE_MEMORY_CHECK    2
#define CELL_SPURS_JOB_TYPE_BINARY2         4

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct CellSpursJobID
	{
		uint32_t notify : 2;
		uint32_t id : 30;
	} CellSpursJobID;

	typedef union CellSpursJobInputList
	{
		struct {
			uint32_t size;
			uint32_t eal;
		} asInputList;
		uint64_t asUint;
	} CellSpursJobInputList;

	typedef union CellSpursJobCacheList
	{
		struct {
			uint32_t size;
			uint32_t eal;
		} asCacheList;
		uint64_t asUint;
	} CellSpursJobCacheList;

	typedef struct CellSpursJobHeader
	{
		__extension__ union {
			/// Job binary information for legacy binary format
			__extension__ struct {
				/// Job binary Effective Address
				uint64_t eaBinary;

				/// Job binary size (actual size in bytes) >> 4
				uint16_t sizeBinary __attribute__((__packed__)); //16bits
			} __attribute__((__packed__));

			/// Job binary information for jobbinary2 format
			uint8_t binaryInfo[10];
		};

		/// Input DMA List size (in bytes)
		uint16_t sizeDmaList __attribute__((__packed__));//8 bits

		/// High 32-bits of all input addresses
		uint32_t eaHighInput __attribute__((__packed__));

		/// Input or Input-Output flag
		uint32_t useInOutBuffer; // Input if zero, otherwise Input-Output
		
		/// Input size or Input-Output size
		uint32_t sizeInOrInOut;//18 bits

		/// Output size
		uint32_t sizeOut;//18 bits

		/// Scratch size
		uint16_t sizeStack;//14 bits (quad word size)

		/// Scratch size
		//14 bits (quad word size)
		__extension__ union {
			uint16_t sizeHeap __attribute__((deprecated));
			uint16_t sizeScratch;
		};

		/// Cache-hinted read-only input Effective Address
		uint32_t eaHighCache;

		/// Cache-hinted read-only input DMA List size (in bytes)
		uint32_t sizeCacheDmaList;

		__extension__ union {
			/// 30-bit Job ID for completion notification
			/// High 2 bits define notificication type
			///  0 = No notification
			///  1 = Poll notification
			///  2 = Event notification
			///  3 = unused
			CellSpursJobID idJob __attribute__((deprecated));
			uint8_t __reserved2__[4];
		};

		__extension__ union {
			uint32_t __reserved__ __attribute__((deprecated));
			__extension__ struct {
				uint8_t jobType;
				uint8_t __reserved3__[3];
			};
		};

	} __attribute__((__aligned__(16))) CellSpursJobHeader;

#define DEFINE_TYPE_CELL_SPURS_JOB(N,N_LIST) \
typedef struct _CellSpursJob##N\
{\
	CellSpursJobHeader header;\
	uint64_t dmaList[N_LIST];\
	uint64_t userData[(N-sizeof(CellSpursJobHeader))/sizeof(uint64_t) -N_LIST]; \
} CellSpursJob##N;

#define CELL_SPURS_GET_SIZE_BINARY(size_in_byte) \
  (uint16_t)(((uintptr_t)(size_in_byte) + 15) >> 4)

 typedef struct CellSpursJob64
 {
	 CellSpursJobHeader header;
	 union {
		 uint64_t dmaList[2];
		 uint64_t userData[2];
	 } workArea;
 } __attribute__((aligned(16))) CellSpursJob64;

 typedef struct CellSpursJob128
 {
	 CellSpursJobHeader header;
	 union {
		 uint64_t dmaList[10];
		 uint64_t userData[10];
	 } workArea;
 } __attribute__((aligned(128))) CellSpursJob128;


 typedef struct CellSpursJob256
 {
	 CellSpursJobHeader header;
	 union {
		 uint64_t dmaList[26];
		 uint64_t userData[26];
	 } workArea;
 } __attribute__((aligned(128))) CellSpursJob256;

#define CELL_SPURS_JOB_LIST_ALIGN 16
#define CELL_SPURS_JOB_LIST_SIZE 16

 typedef struct CellSpursJobList
 {
	 uint32_t numJobs;
	 uint32_t sizeOfJob;
	 uint64_t eaJobList;
 } __attribute__((aligned(16))) CellSpursJobList;


#ifdef __PPU__
int cellSpursJobHeaderSetJobbin2Param(CellSpursJobHeader *header, const void *jobbin2);
#endif /* __PPU__ */

#ifdef __SPU__
int cellSpursJobHeaderSetJobbin2Param(CellSpursJobHeader *header, uint64_t  eaJobbin2);
#endif /* __SPU__ */



#ifdef __cplusplus
}
#endif

static inline
int cellSpursJobGetInputList(uint64_t *inputList,
							 uint16_t size,
							 uint32_t eal)
{
	if	((size > 0x4000) || (size > 0x10 && (size&0xf)) || (size & (size-1) & 0xf)) return CELL_SPURS_JOB_ERROR_INVAL;
	else if (size && (eal & ((size-1) & 0xf))) return CELL_SPURS_JOB_ERROR_ALIGN;
	*inputList = ((uint64_t)size << 32) | (uint64_t)eal;

	return CELL_OK;
}

static inline
int cellSpursJobGetCacheList(uint64_t *cacheList,
							 uint32_t size,
							 uint32_t eal)
{
	int ret;
	ret = ((size & 0xf) |
		   (eal & 0xf)) ? CELL_SPURS_JOB_ERROR_ALIGN : CELL_OK;
	*cacheList = ((uint64_t)size << 32) | (uint64_t)eal;
	
	return ret;
}

static inline
int _cellSpursCheckJob(const CellSpursJob256 *pJob, unsigned int size, unsigned int maxSize, unsigned int maxUsableMemorySize, unsigned int align, int isScratchStackUnified)
{
	uint32_t eaBin = pJob->header.eaBinary & ~1ull;
	if ((pJob == 0) || (eaBin == 0))
	{
		return CELL_SPURS_JOB_ERROR_NULL_POINTER;
	}
	if (((uintptr_t)pJob % 16) ||
		(eaBin % 16) ||
		(pJob->header.sizeOut % 16) ||
		(pJob->header.sizeInOrInOut % 16) ||
		(pJob->header.sizeDmaList % 8) ||
		(pJob->header.sizeCacheDmaList %8))
	{
		return CELL_SPURS_JOB_ERROR_ALIGN;
	}
	if ((pJob->header.sizeBinary == 0) ||
		(pJob->header.sizeDmaList + pJob->header.sizeCacheDmaList > size - sizeof(CellSpursJobHeader)) ||
		(pJob->header.sizeCacheDmaList/8 > 4) ||
		(size != 64 &&
		 ((size % 128) ||
		  (size > 1024) ||
		  (size < 64))) ||
		size > maxSize)
	{
		return CELL_SPURS_JOB_ERROR_INVAL;
	}
	unsigned int sizeInput = 0;
	unsigned int memCheckMarker = (pJob->header.jobType & CELL_SPURS_JOB_TYPE_MEMORY_CHECK) ? 16 : 0;
	// check Input DMA list
	for(int i = 0; i < pJob->header.sizeDmaList/8; i++)
	{
		uint32_t tsize = (pJob->workArea.dmaList[i]>>32)&0x7fff;
		uint32_t eal = pJob->workArea.dmaList[i];
		if (tsize) {
			if (!eal)
			{
				return CELL_SPURS_JOB_ERROR_NULL_POINTER;
			}
			if (((pJob->workArea.dmaList[i]>>32)&0x80000000u) ||
				(tsize > 16 * 1024) ||
				(tsize < 16 && (tsize != 1 && tsize !=2 && tsize != 4 && tsize != 8)))
			{
				return CELL_SPURS_JOB_ERROR_INVAL;
			}
			if (eal & (((tsize < 16) ? tsize : 16)-1))
			{
				return CELL_SPURS_JOB_ERROR_ALIGN;
			}
		}
		sizeInput += (tsize + 15) & ~15;
	}
	if (pJob->header.sizeInOrInOut < sizeInput)
	{
		return CELL_SPURS_JOB_ERROR_INVAL;
	}
	unsigned sizeCache = 0;
	// check cache data
	for(unsigned i = 0; i < pJob->header.sizeCacheDmaList/8; i++)
	{
		uint32_t tsize = pJob->workArea.dmaList[pJob->header.sizeDmaList/8+i]>>32;
		uint32_t eal = pJob->workArea.dmaList[pJob->header.sizeDmaList/8+i];
		if (tsize) {
			if (!eal)
			{
				return CELL_SPURS_JOB_ERROR_NULL_POINTER;
			}
			if (tsize < 16)
			{
				return CELL_SPURS_JOB_ERROR_INVAL;
			}
			if ((tsize % 16) ||	(eal % 16))
			{
				return CELL_SPURS_JOB_ERROR_ALIGN;
			}
		}
		sizeCache += (tsize + memCheckMarker + align - 1) & ~(align-1);
	}

	unsigned int total_mem_size =
		((((pJob->header.sizeBinary + ((pJob->header.jobType & CELL_SPURS_JOB_TYPE_BINARY2) ? *((uint32_t*)(uintptr_t)pJob) : 0))<<4) + align-1) & ~(align-1)) +
		((pJob->header.sizeInOrInOut + (pJob->header.sizeInOrInOut ? memCheckMarker : 0) + align-1) & ~(align-1)) +
		(((pJob->header.sizeOut + (pJob->header.sizeOut ? memCheckMarker : 0)) + align-1) & ~(align-1)) +
		((
		(isScratchStackUnified ? (unsigned int)(pJob->header.sizeScratch<<4) : ((pJob->header.sizeScratch<<4) + (pJob->header.sizeScratch ? memCheckMarker : 0) + align-1) & ~(align-1))+
			(pJob->header.sizeStack?(pJob->header.sizeStack<<4):8*1024) + memCheckMarker + align-1) & ~(align-1)) +
		sizeCache;
	if (total_mem_size > maxUsableMemorySize - 4*(maxSize-256))
	{
		return CELL_SPURS_JOB_ERROR_MEMORY_SIZE;
	}

	return CELL_OK;
}

static inline
int cellSpursCheckJob(const CellSpursJob256 *pJob, unsigned int sizeJob, unsigned int maxSizeJob)
{
	if (maxSizeJob < 256 || maxSizeJob > 1024 || (maxSizeJob%128)) return CELL_SPURS_JOB_ERROR_INVAL;
	return _cellSpursCheckJob(pJob, sizeJob, maxSizeJob, CELL_SPURS_MAX_SIZE_JOB_MEMORY, 1024, 1);
}

#ifdef __cplusplus
// C++ job descriptor definitions
namespace cell {
	namespace Spurs {
		template<unsigned int tSize> class Job :public CellSpursJobHeader {
		private:
			int __dummyForAssert__[((tSize > 0 && tSize <= 1024 && (tSize%128) == 0) || (tSize == 64))?0:-1];
		public:
			static const int NUM_DATA = (tSize - sizeof(CellSpursJobHeader))/8;
			union {
				uint64_t dmaList[NUM_DATA];
				uint64_t userData[NUM_DATA];
			} workArea;

#ifndef __SPU__
			int setJobbin2Param(const void *jobbin2) { return cellSpursJobSetJobbin2Param(this, jobbin2); }
#else
			int setJobbin2Param(uint64_t eaJobbin2) { return cellSpursJobSetJobbin2Param(this, eaJobbin2); }
#endif
			int checkForJobChain(unsigned int maxSizeJob = 1024)
			{
				if (__builtin_expect(maxSizeJob < 256 || maxSizeJob > 1024 || (maxSizeJob%128), 0))
				{
					return CELL_SPURS_JOB_ERROR_INVAL;
				}
				return _cellSpursCheckJob(this, tSize, maxSizeJob, CELL_SPURS_MAX_SIZE_JOB_MEMORY, 1024, 1);
			}
			int checkForJobQueue(unsigned int maxSizeJob = 896)
			{
				if (__builtin_expect(maxSizeJob < 256 || maxSizeJob >= 1024 || (maxSizeJob%128), 0))
				{
					return CELL_SPURS_JOB_ERROR_INVAL;
				}
				return _cellSpursCheckJob(this, tSize, maxSizeJob, CELL_SPURS_JOBQUEUE_MAX_SIZE_JOB_MEMORY, 128, 0);
			}
		};
		
		template<typename tType, unsigned int tSize> class JobTypeOf :public CellSpursJobHeader {
		private:
			int __dummyForAssert__[((sizeof(tType) % 8) == 0 && ((tSize > 0 && tSize <= 1024 && (tSize%128) == 0) || (tSize == 64)))?0:-1];
		public:
			static const int NUM_DMA = (tSize - sizeof(CellSpursJobHeader) - sizeof(tType))/8;
			CellSpursJobInputList dmaList[NUM_DMA];
			tType userData;

#ifndef __SPU__
			int setJobbin2Param(const void *jobbin2) { return cellSpursJobSetJobbin2Param(this, jobbin2); }
#else
			int setJobbin2Param(uint64_t eaJobbin2) { return cellSpursJobSetJobbin2Param(this, eaJobbin2); }
#endif
			int checkForJobChain(unsigned int maxSizeJob = 1024)
			{
				if (__builtin_expect(maxSizeJob < 256 || maxSizeJob > 1024 || (maxSizeJob%128), 0))
				{
					return CELL_SPURS_JOB_ERROR_INVAL;
				}
				return _cellSpursCheckJob(this, tSize, maxSizeJob, CELL_SPURS_MAX_SIZE_JOB_MEMORY, 1024, 1);
			}
			int checkForJobQueue(unsigned int maxSizeJob = 896)
			{
				if (__builtin_expect(maxSizeJob < 256 || maxSizeJob >= 1024 || (maxSizeJob%128), 0))
				{
					return CELL_SPURS_JOB_ERROR_INVAL;
				}
				return _cellSpursCheckJob(this, tSize, maxSizeJob, CELL_SPURS_JOBQUEUE_MAX_SIZE_JOB_MEMORY, 128, 0);
			}
		};
	}
}
#endif // __cplusplus

#endif /* __CELL_SPURS_JOB_DESCRIPTOR_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
