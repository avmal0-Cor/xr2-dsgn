/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_LSGUARD_H__
#define __CELL_LSGUARD_H__

#include <stddef.h>
#include <spu_intrinsics.h>
#include <cell/lsguard/error.h>

#define CELL_LSGUARD_MAX_WRITABLE_REGION	16

#ifdef __cplusplus
extern "C" {
#endif

int cellLsGuardPermitWrite(const void* start, size_t size);
int cellLsGuardCancelWritePermission(const void* start);
int cellLsGuardCheckWriteAccess(const void* start, size_t size);
void cellLsGuardRehash(void);
int cellLsGuardCheckCorruption(void);

#ifdef __cplusplus
} /* __cplusplus */
#endif

#endif /* __CELL_LSGUARD_H__ */
