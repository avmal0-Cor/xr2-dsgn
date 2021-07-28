/* SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   * Copyright (C) 2008 Sony Computer Entertainment Inc.
   * All Rights Reserved.
   */

#ifndef __CELL_SPURS_JOB_CHAIN_TYPES_H__
#define __CELL_SPURS_JOB_CHAIN_TYPES_H__

#include <stdint.h>
#include <cell/spurs/types.h>
#include <cell/spurs/exception_common_types.h>
#include <cell/spurs/job_descriptor.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_SPURS_JOB_REVISION     3
#define CELL_SPURS_JOBCHAIN_CLASS_NAME	"JobChain"

#define CELL_SPURS_JOBCHAIN_ALIGN	128
#define CELL_SPURS_JOBCHAIN_SIZE	272

	typedef struct CellSpursJobChain {
		unsigned char skip[CELL_SPURS_JOBCHAIN_SIZE];
	} CellSpursJobChain  __attribute__((aligned(CELL_SPURS_JOBCHAIN_ALIGN)));

	/*
	 * NOTE: Don't create array of CellSpursJobChain structure.
	 *             Entries will be misaligned.
	 *             Use CellSpursJobChainForArray structutre instead of
	 *			   CellSpursJobChain for the purpose.
	 */

	typedef struct CellSpursJobChainForArray {
		CellSpursJobChain	jobChain;
		uint8_t				padding[128 * 3 - CELL_SPURS_JOBCHAIN_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBCHAIN_ALIGN))) CellSpursJobChainForArray;

#define CELL_SPURS_JOBCHAIN_ATTRIBUTE_ALIGN 8
#define CELL_SPURS_JOBCHAIN_ATTRIBUTE_SIZE 512

	typedef struct CellSpursJobChainAttribute {
		unsigned char skip[CELL_SPURS_JOBCHAIN_ATTRIBUTE_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBCHAIN_ATTRIBUTE_ALIGN))) CellSpursJobChainAttribute;


	typedef void (*CellSpursJobChainExceptionEventHandler)(
		CellSpurs*,
		CellSpursJobChain*,
		const CellSpursExceptionInfo*,
		const void*,
		uint32_t,
		void*
	);


	typedef struct CellSpursJobChainInfo {
		uint64_t urgentCommandSlot[4];
		const uint64_t* programCounter;
		const uint64_t* linkRegister[3];
		const void* cause;
		uint32_t statusCode;
		uint32_t maxSizeJobDescriptor;
		CellSpursWorkloadId idWorkload;
		__extension__ union {
			bool autoReadyCount;
			bool autoRequestSpuCount;
		};
		bool isHalted;
		bool isFixedMemAlloc;
		uint8_t padding8;
		uint16_t maxGrabbedJob;
		uint16_t padding16;
		const char* name;
		CellSpursJobChainExceptionEventHandler exceptionEventHandler;
		void* exceptionEventHandlerArgument;
		uint8_t reserved[264 - sizeof(uint64_t) * 4
		                     - sizeof(const uint64_t*) * 4
		                     - sizeof(const void*)
		                     - sizeof(uint32_t) * 2
		                     - sizeof(CellSpursWorkloadId)
		                     - sizeof(bool) * 3
		                     - sizeof(uint8_t)
		                     - sizeof(uint16_t) * 2
		                     - sizeof(const char*)
		                     - sizeof(CellSpursJobChainExceptionEventHandler)
		                     - sizeof(void*)
				];
	} CellSpursJobChainInfo;


	typedef struct CellSpursJobPipelineInfo {
		struct {
			__extension__ union {
				uint8_t jobDescriptorStorage[1024];
				CellSpursJob256 job;
			};
			uint8_t guid[8];
			uint8_t reserved[128 - sizeof(uint8_t) * 8];
		} fetchStage;
		struct {
			__extension__ union {
				uint8_t jobDescriptorStorage[1024];
				CellSpursJob256 job;
			};
			uint8_t guid[8];
			uint8_t reserved[128 - sizeof(uint8_t) * 8];
		} inputStage;
		struct {
			__extension__ union {
				uint8_t jobDescriptorStorage[1024];
				CellSpursJob256 job;
			};
			uint8_t guid[8];
			uint8_t reserved[128 - sizeof(uint8_t) * 8];
		} executeStage;
		struct {
			__extension__ union {
				uint8_t jobDescriptorStorage[1024];
				CellSpursJob256 job;
			};
			uint8_t guid[8];
			uint8_t reserved[128 - sizeof(uint8_t) * 8];
		} outputStage;
	} CellSpursJobPipelineInfo;


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __CELL_SPURS_JOB_CHAIN_TYPES_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
