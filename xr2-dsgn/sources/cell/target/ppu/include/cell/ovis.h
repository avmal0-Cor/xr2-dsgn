/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_OVIS_H__
#define __CELL_OVIS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <cell/ovis/types.h>
#include <cell/ovis/util.h>
#include <cell/ovis/error.h>

int 
cellOvisInitializeOverlayTable(void *ea_ovly_table, const char *elf);
int 
cellOvisGetOverlayTableSize(const char *elf);



#ifdef __cplusplus
}
#endif

#endif /* __CELL_OVIS_H__ */


