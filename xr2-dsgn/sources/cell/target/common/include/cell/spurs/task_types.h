/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TASK_TYPES_H__
#define __CELL_SPURS_TASK_TYPES_H__ 1

#include <stdint.h>
#include <cell/spurs/types.h>
#include <cell/spurs/exception_common_types.h>
#include <cell/spurs/task_exit_code_types.h>

#ifdef __SPU__
#include <spu_intrinsics.h>
#endif /* __SPU__ */

/*
 * SPURS task
 */
#define CELL_SPURS_MAX_TASK			              128
#define CELL_SPURS_TASK_TOP			              0x3000
#define CELL_SPURS_TASK_BOTTOM		              0x40000 /* LS_BOTTOM */
#define CELL_SPURS_CONTEXT_SIZE2BITS(size)	      ((size)>>11)
#define CELL_SPURS_TASK_TOP_BIT		              CELL_SPURS_CONTEXT_SIZE2BITS(CELL_SPURS_TASK_TOP)
#define CELL_SPURS_TASK_TOP_MASK	              (0xffffffffu>>CELL_SPURS_TASK_TOP_BIT)

#define CELL_SPURS_TASK_CONTEXT_ALIGN	          128

#define CELL_SPURS_TASK_EXECUTION_CONTEXT_SIZE	  1024
#define CELL_SPURS_TASK_MEMORY_CONTEXT_SIZE(size) ((size + 2047) & ~2047)
#define CELL_SPURS_TASK_CONTEXT_SIZE(size)        (CELL_SPURS_TASK_EXECUTION_CONTEXT_SIZE +\
												   CELL_SPURS_TASK_MEMORY_CONTEXT_SIZE(size))
#define CELL_SPURS_TASK_CONTEXT_SIZE_ALL          (CELL_SPURS_TASK_CONTEXT_SIZE(CELL_SPURS_TASK_BOTTOM-CELL_SPURS_TASK_TOP))

#define CELL_SPURS_TASK_CEIL_2048(arg)            (( (arg) + 2047) & ~2047)
#define CELL_SPURS_TASK_FLOOR_2048(arg)           ( (arg) & ~2047)

typedef unsigned CellSpursTaskId;

typedef union CellSpursTaskArgument {
	uint32_t u32[4];
	uint64_t u64[2];
} CellSpursTaskArgument;

typedef union CellSpursTaskLsPattern {
	uint32_t u32[4];
	uint64_t u64[2];
} CellSpursTaskLsPattern;


/*
 * SPURS task attribute
 */

#define CELL_SPURS_TASK2_REVISION 0


#define	CELL_SPURS_TASK_ATTRIBUTE_ALIGN	          16
#define	CELL_SPURS_TASK_ATTRIBUTE_SIZE	          256

typedef struct CellSpursTaskAttribute {
	unsigned char	skip[CELL_SPURS_TASK_ATTRIBUTE_SIZE];
} __attribute__((aligned(CELL_SPURS_TASK_ATTRIBUTE_ALIGN))) CellSpursTaskAttribute;

#define	CELL_SPURS_TASK_ATTRIBUTE2_ALIGN	       CELL_SPURS_TASK_ATTRIBUTE_ALIGN
#define	CELL_SPURS_TASK_ATTRIBUTE2_SIZE	          CELL_SPURS_TASK_ATTRIBUTE_SIZE	          

typedef struct CellSpursTaskAttribute2 
{
	uint32_t        revision;
	uint32_t		sizeContext;
	uint64_t		eaContext;
	CellSpursTaskLsPattern lsPattern;
	const char*           name;
	uint8_t __reserved__[CELL_SPURS_TASK_ATTRIBUTE2_SIZE
						 - sizeof(uint32_t)  * 2
						 - sizeof(uint64_t)
						 - sizeof(CellSpursTaskLsPattern)
						 - sizeof(char*)
		];
} __attribute__((aligned(CELL_SPURS_TASK_ATTRIBUTE2_ALIGN))) CellSpursTaskAttribute2;

/*
 * Exception Event Handling
 */
struct CellSpursTaskset;

typedef void (*CellSpursTasksetExceptionEventHandler)(
		CellSpurs*,
		struct CellSpursTaskset*,
		CellSpursTaskId,
		const CellSpursExceptionInfo*,
		void*
	);


/*
 * Information access
 */

typedef enum {
	CELL_SPURS_TASK_STATE_NO_EXISTENT = 0,
	CELL_SPURS_TASK_STATE_RUNNING,
	CELL_SPURS_TASK_STATE_WAITING,
	CELL_SPURS_TASK_STATE_READY
} CellSpursTaskState;

typedef struct CellSpursTaskInfo {
	CellSpursTaskLsPattern lsPattern;
	CellSpursTaskArgument argument;
	const void* eaElf;
	const void* eaContext;
	uint32_t sizeContext;
	uint8_t state;
	uint8_t hasSignal;
	uint8_t padding[2];
	const CellSpursTaskExitCode* eaTaskExitCode;
	uint8_t guid[8];
	uint8_t reserved[ 64
			- sizeof(CellSpursTaskLsPattern)
			- sizeof(CellSpursTaskArgument)
			- sizeof(const void*) * 2
			- sizeof(uint8_t) * 4
			- sizeof(const CellSpursTaskExitCode*)
			- sizeof(uint8_t) * 8
		];
} CellSpursTaskInfo;

typedef struct CellSpursTasksetInfo {
	CellSpursTaskInfo taskInfo[CELL_SPURS_MAX_TASK];
	uint64_t argument;
	CellSpursWorkloadId idWorkload;
	CellSpursTaskId idLastScheduledTask;
	const char* name;
	CellSpursTasksetExceptionEventHandler exceptionEventHandler;
	void* exceptionEventHandlerArgument;
	size_t sizeTaskset;
	uint8_t reserved[9360
			- sizeof(CellSpursTaskInfo) * CELL_SPURS_MAX_TASK
			- sizeof(uint64_t)
			- sizeof(CellSpursWorkloadId)
			- sizeof(CellSpursTaskId)
			- sizeof(const char*)
			- sizeof(CellSpursTasksetExceptionEventHandler)
			- sizeof(void*)
			- sizeof(size_t)
		];
} CellSpursTasksetInfo;


/*
 * SPURS taskset
 */
#define CELL_SPURS_TASKSET_CLASS0_SIZE            (128 * 50)
#define CELL_SPURS_TASKSET_SIZE                   CELL_SPURS_TASKSET_CLASS0_SIZE
#define CELL_SPURS_TASKSET_ALIGN                  128
#define	_CELL_SPURS_TASKSET_CLASS1_EXTENDED_SIZE  (128 + 128 * 16 + 128 * 15)
#define	CELL_SPURS_TASKSET_CLASS1_SIZE            (CELL_SPURS_TASKSET_CLASS0_SIZE + _CELL_SPURS_TASKSET_CLASS1_EXTENDED_SIZE)
#define CELL_SPURS_TASKSET_CLASS_NAME	"taskset"

/*
 * SPURS taskset attribute 
 */
#define	CELL_SPURS_TASKSET_ATTRIBUTE_ALIGN	      8
#define	CELL_SPURS_TASKSET_ATTRIBUTE_SIZE	      512

typedef struct CellSpursTasksetAttribute {
	unsigned char	skip[CELL_SPURS_TASKSET_ATTRIBUTE_SIZE];
} __attribute__((aligned(CELL_SPURS_TASKSET_ATTRIBUTE_ALIGN))) CellSpursTasksetAttribute;

typedef struct CellSpursTaskset {
	unsigned char skip[CELL_SPURS_TASKSET_SIZE];
} __attribute__((aligned(CELL_SPURS_TASKSET_ALIGN))) CellSpursTaskset;

#ifdef __cplusplus
#include "task1_types_cpp.h"
#endif /*__cplusplus*/


#define CELL_SPURS_TASKSET2_SIZE                  (CELL_SPURS_TASKSET_CLASS1_SIZE)
#define CELL_SPURS_TASKSET2_ALIGN	              128

#ifdef __cplusplus
typedef struct CellSpursTaskset2 : public cell::Spurs::Taskset {
	unsigned char skip[CELL_SPURS_TASKSET2_SIZE - CELL_SPURS_TASKSET_SIZE];
} __attribute__((aligned(CELL_SPURS_TASKSET_ALIGN))) CellSpursTaskset2;
#else /* __cplusplus */
typedef struct CellSpursTaskset2 {
	unsigned char skip[CELL_SPURS_TASKSET2_SIZE];
} __attribute__((aligned(CELL_SPURS_TASKSET_ALIGN))) CellSpursTaskset2;
#endif /* __cplusplus */




#define	CELL_SPURS_TASKSET_ATTRIBUTE2_ALIGN	      CELL_SPURS_TASKSET_ATTRIBUTE_ALIGN
#define	CELL_SPURS_TASKSET_ATTRIBUTE2_SIZE	      CELL_SPURS_TASKSET_ATTRIBUTE_SIZE

#define CELL_SPURS_MAX_TASK_NAME_LENGTH 32

typedef struct CellSpursTaskNameBuffer 
{ 
	char taskName[CELL_SPURS_MAX_TASK][CELL_SPURS_MAX_TASK_NAME_LENGTH]; 
} __attribute__((aligned(16))) CellSpursTaskNameBuffer;

typedef struct CellSpursTasksetAttribute2
{
	uint32_t revision;
	const char *name;
	uint64_t argTaskset;
	uint8_t  priority[8];
	uint32_t maxContention;
	int32_t  enableClearLs;
	CellSpursTaskNameBuffer *taskNameBuffer;
	uint8_t __reserved__[CELL_SPURS_TASKSET_ATTRIBUTE2_SIZE 
						  - (sizeof(uint32_t) 
							 + sizeof(char*) 
							 + sizeof(uint64_t) 
							 + sizeof(uint8_t)*8 
							 + sizeof(uint32_t)
							 + sizeof(int32_t)
							 + sizeof(CellSpursTaskNameBuffer*) )
		];

} __attribute__((aligned(CELL_SPURS_TASKSET_ATTRIBUTE2_ALIGN))) CellSpursTasksetAttribute2;



typedef struct CellSpursTaskBinInfo 
{
   uint64_t eaElf;
   uint32_t sizeContext;
   uint32_t __reserved__;
   CellSpursTaskLsPattern lsPattern;
} __attribute__((aligned(16))) CellSpursTaskBinInfo;






#endif /* __CELL_SPURS_TASK_TYPES_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
