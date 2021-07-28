/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPUDLL_H__
#define __CELL_SPUDLL_H__

#include <stdint.h>
#include <spu_intrinsics.h>
#include <cell/spudll/spudll_types.h>

#ifdef __cplusplus
extern "C" {
#endif




int cellSpudllHandleConfigSetDefaultValues(CellSpudllHandleConfig *config);

int cellSpudllInitialize(CellSpudllHandle *handle, uint64_t ea_so, const CellSpudllHandleConfig* config);

int cellSpudllSym(CellSpudllHandle *handle, const char* symbol, void** value);
int cellSpudllGetImageSize(CellSpudllHandle *handle, uint32_t *size);

int cellSpudllStartLoad(CellSpudllHandle *handle, void *image, CellSpudllHandle **depends, uint32_t numDepends);
int cellSpudllWaitLoad(CellSpudllHandle *handle);
int cellSpudllUnload(CellSpudllHandle *handle);

/*
 *  Resolve undefined references in handle using symbols in handle_array.
 */ 
int cellSpudllLink(CellSpudllHandle *handle, CellSpudllHandle *from_array[], uint32_t numFrom);
int cellSpudllCallInit(CellSpudllHandle *handle);
int cellSpudllCallFini(CellSpudllHandle *handle);

void cellSpudllDump(CellSpudllHandle *handle);


int cellSpudllEnableSpursTrace(int enable, uint32_t dmaTag);

#ifdef __cplusplus
}
#endif


#include <cell/spudll/error.h>


#endif /* __CELL_SPUDLL_H__ */
