/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_COMMON_H__
#define __CELL_SPURS_COMMON_H__ 1

/* The C Standard Library */
#include <stdint.h>
#include <stdbool.h>

#include <spu_intrinsics.h>				/* qword */
#include <cell/spurs/types.h>			/* CellSpursWorkloadId */

#ifdef __cplusplus
extern "C" {
#endif

uint32_t cellSpursGetCurrentSpuId(void);
uint64_t cellSpursGetSpursAddress(void);
uint32_t cellSpursGetTagId(void);
CellSpursWorkloadId cellSpursGetWorkloadId(void);

uint32_t cellSpursGetSpuCount(void);

bool cellSpursPoll(void);
int cellSpursGetSpuGuid(const qword *pSpuGUID, uint64_t *guid);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_COMMON_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
