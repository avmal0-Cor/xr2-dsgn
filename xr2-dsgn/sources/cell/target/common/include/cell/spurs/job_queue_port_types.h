/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOBQUEUE_PORT_TYPES_H__
#define __CELL_SPURS_JOBQUEUE_PORT_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CELL_SPURS_JOBQUEUE_PORT_ALIGN 128
#define CELL_SPURS_JOBQUEUE_PORT_SIZE 128

	typedef struct CellSpursJobQueuePort
	{
		unsigned char skip[CELL_SPURS_JOBQUEUE_PORT_SIZE];
	} __attribute__((aligned(CELL_SPURS_JOBQUEUE_PORT_ALIGN))) CellSpursJobQueuePort;

	//typedef struct CellSpursJobQueuePortWithDescriptor
	//{
	//	unsigned char skip[CELL_SPURS_JOBQUEUE_PORT_SIZE];
	//} __attribute__((aligned(CELL_SPURS_JOBQUEUE_PORT_ALIGN))) CellSpursJobQueuePortWithDescriptor;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_JOBQUEUE_PORT_TYPES_H__ */
