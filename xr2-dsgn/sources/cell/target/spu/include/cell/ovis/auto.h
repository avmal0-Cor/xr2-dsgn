/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_OVIS_AUTO_H__
#define __CELL_OVIS_AUTO_H__

#include <stdint.h>
#include <cell/ovis/error.h>
extern unsigned int gCellOvisTag;

extern uint64_t gCellOvisTable;

static inline 
int cellOvisInitializeAutoMapping(uint64_t eaOvlyTable, uint32_t tag)
{
	if(tag >= 32){
		return CELL_OVIS_ERROR_INVAL;
	}
	if((eaOvlyTable % 128) != 0){
		return CELL_OVIS_ERROR_ALIGN;
	}
	gCellOvisTag = tag;
	gCellOvisTable = eaOvlyTable;

	return CELL_OK;
}


#endif /* __CELL_OVIS_AUTO_H__ */
