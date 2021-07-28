/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPUDLL_H__
#define __CELL_SPUDLL_H__

#include <cell/spudll/spudll_types.h>
#include <cell/spudll/error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int cellSpudllGetImageSize(uint32_t *psize, const void *so_elf, const CellSpudllHandleConfig *config);
int cellSpudllHandleConfigSetDefaultValues(CellSpudllHandleConfig *config);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __CELL_SPUDLL_H__ */
