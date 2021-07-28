/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_CGB_STRUCT_H__
#define __CELL_CGB_STRUCT_H__

#if defined(WIN32) || defined(LINUX) || defined(__linux__)
#else
#include <sys/types.h>
#endif

#if defined(SPU)
#define __PACKED__ __attribute__((packed)) 
#else
#define __PACKED__
#endif

typedef struct __PACKED__ CellCgbVertexProgramConfiguration{
		 uint16_t		 instructionSlot;
		 uint16_t		 instructionCount;
		 uint16_t		 attributeInputMask;
		 uint8_t		 		 registerCount;
		 uint8_t		 		 unused0;
}CellCgbVertexProgramConfiguration;

typedef struct __PACKED__ CellCgbFragmentProgramConfiguration{
		 uint32_t		 offset;
		 uint32_t		 attributeInputMask;
		 uint16_t		 texCoordsInputMask;
		 uint16_t		 texCoords2D;
		 uint16_t		 texCoordsCentroid;
		 uint16_t		 unused0;
		 uint32_t		 fragmentControl;
		 uint8_t		 		 registerCount;
		 uint8_t		 		 unused1;
		 uint16_t		 unused2;
}CellCgbFragmentProgramConfiguration;

#endif // __CELL_CGB_STRUCT_H__

