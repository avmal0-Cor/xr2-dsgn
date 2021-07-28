/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2007 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_DRM_INLINE_H_
#define _SCE_NP_DRM_INLINE_H_

#include <cell/cell_fs.h>
#include <np/drm.h>
#include <np/error.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

static __inline__ int sceNpDrmOpen(
	const SceNpDrmKey *k_licensee,
	const char *path,
	int flags,
	int *fd,
	void * dummy_arg,
	uint64_t dummy_size)
{
	int ret;
	SceNpDrmOpenArg arg;
	CellFsStat sb;

	(void)dummy_arg;
	(void)dummy_size;

	/* file existance check (time saving) */
	ret = cellFsStat(path, &sb);
	if (ret == CELL_FS_ERROR_ENOENT) { return ret; }

	/* check DRM */
	ret = sceNpDrmIsAvailable(k_licensee, path);
	if (ret != CELL_OK) { return ret; }

	/* open as DRM EDATA */
	arg.flag = SCE_NP_DRM_OPEN_FLAG;
	ret = cellFsOpen(path, flags, fd, &arg, sizeof(arg));

	return ret;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_DRM_INLINE_H_ */
