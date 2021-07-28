/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SHEAP_TYPES_H__
#define __CELL_SHEAP_TYPES_H__


typedef uint32_t CellSheapKey;
#define CELL_SHEAP_NUM_KEY_ENTRY 256


typedef struct _CellKeySheapBarrier {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
} CellKeySheapBarrier;

typedef struct _CellKeySheapBuffer {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
	uint64_t        size;
} CellKeySheapBuffer;

typedef struct _CellKeySheapMutex {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
} CellKeySheapMutex;

typedef struct _CellKeySheapQueue {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
} CellKeySheapQueue;


typedef struct _CellKeySheapRwm {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
} CellKeySheapRwm;

typedef struct _CellKeySheapSemaphore {
	uint64_t  ea_ksheap;
	CellSheapKey key;
	uint64_t  ea;
} CellKeySheapSemaphore;


#endif /* __CELL_SHEAP_TYPES_H__ */
