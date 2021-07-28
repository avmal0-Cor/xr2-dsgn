/*  SCE CONFIDENTIAL
 *  PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *  Copyright (C) 2008 Sony Computer Entertainment Inc.
 *  All Rights Reserved.
 */

#ifndef __CELL_GCM_CG_EXTENSION_H__
#define __CELL_GCM_CG_EXTENSION_H__

#ifdef __cplusplus
extern "C" {
#endif

int32_t cellGcmCgStripBranchesFromFragmentUCode(
	const void *ucode,
	uint32_t size,
	void *pOutUCode,
	uint32_t *pOutUCodeSize
);

#ifdef __cplusplus
}
#endif

#endif  // __CELL_GCM_CG_EXTENSION_H__
