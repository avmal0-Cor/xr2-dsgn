/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_FRIENDLIST_H_
#define _SCE_NP_FRIENDLIST_H_

#include <sys/memory.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef uint64_t SceNpFriendlistCustomOptions;

typedef int (*SceNpFriendlistResultHandler)(
	int retCode,
	void *arg
	);

int sceNpFriendlist(
	SceNpFriendlistResultHandler resultHandler,
	void *userArg,
	sys_memory_container_t containerId
	);

int sceNpFriendlistCustom(
	SceNpFriendlistCustomOptions options,
	SceNpFriendlistResultHandler resultHandler,
	void *userArg,
	sys_memory_container_t containerId
	);

int sceNpFriendlistAbortGui(
	void
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_FRIENDLIST_H_ */
