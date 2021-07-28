/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_PROFILE_H_
#define _SCE_NP_PROFILE_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef int (*SceNpProfileResultHandler)(
	int result,
	void *arg
	);

int sceNpProfileCallGui(
	const SceNpId *npid,
	SceNpProfileResultHandler handler,
	void *userArg,
	uint64_t options
	);

int sceNpProfileAbortGui(
	void
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_PROFILE_H_ */
