/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __SPURS_EVENT_FLAG_TYPES_H__
#define __SPURS_EVENT_FLAG_TYPES_H__ 1

typedef enum CellSpursEventFlagWaitMode {
	CELL_SPURS_EVENT_FLAG_OR = 0,
	CELL_SPURS_EVENT_FLAG_AND = 1
} CellSpursEventFlagWaitMode;

typedef enum CellSpursEventFlagClearMode {
	CELL_SPURS_EVENT_FLAG_CLEAR_AUTO = 0,
	CELL_SPURS_EVENT_FLAG_CLEAR_MANUAL = 1
} CellSpursEventFlagClearMode;

typedef enum CellSpursEventFlagDirection {
	CELL_SPURS_EVENT_FLAG_SPU2SPU,
	CELL_SPURS_EVENT_FLAG_SPU2PPU,
	CELL_SPURS_EVENT_FLAG_PPU2SPU,
	CELL_SPURS_EVENT_FLAG_ANY2ANY
} CellSpursEventFlagDirection;

#define CELL_SPURS_EVENT_FLAG_ALIGN		128
#define CELL_SPURS_EVENT_FLAG_SIZE		128
typedef struct CellSpursEventFlag {
	unsigned char skip[CELL_SPURS_EVENT_FLAG_SIZE];
} __attribute__((aligned(CELL_SPURS_EVENT_FLAG_ALIGN))) CellSpursEventFlag;


#endif /* __SPURS_EVENT_FLAG_TYPES_H__ */
