/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __SPURS_TASK_EXIT_CODE_TYPES_H__
#define __SPURS_TASK_EXIT_CODE_TYPES_H__ 1

#define CELL_SPURS_TASK_EXIT_CODE_ALIGN		128
#define CELL_SPURS_TASK_EXIT_CODE_SIZE		128

typedef struct CellSpursTaskExitCode {
	unsigned char skip[CELL_SPURS_TASK_EXIT_CODE_SIZE];
} __attribute__((aligned(CELL_SPURS_TASK_EXIT_CODE_ALIGN))) CellSpursTaskExitCode;


#endif /* __SPURS_TASK_EXIT_CODE_TYPES_H__ */
