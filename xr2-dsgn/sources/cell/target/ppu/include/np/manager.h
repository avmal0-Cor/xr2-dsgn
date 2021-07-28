/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_MANAGER_H_
#define _SCE_NP_MANAGER_H_

#include <sysutil/sysutil_common.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define SCE_NP_MANAGER_SUB_SIGNIN_MAX				3

#define SCE_NP_MANAGER_SUB_SIGNIN_RESULT_OK			0
#define SCE_NP_MANAGER_SUB_SIGNIN_RESULT_CANCEL		1
typedef void (*SceNpManagerSubSigninCallback)(int result, SceNpId *npId, void *cb_arg);

static inline int sceNpManagerInit(void) {return(0);}
static inline int sceNpManagerTerm(void) {return(0);}

/*
 * Np Manager Status
 */
#define SCE_NP_MANAGER_STATUS_OFFLINE				(-1)
#define SCE_NP_MANAGER_STATUS_GETTING_TICKET		(0)
#define SCE_NP_MANAGER_STATUS_GETTING_PROFILE		(1)
#define SCE_NP_MANAGER_STATUS_LOGGING_IN			(2)
#define SCE_NP_MANAGER_STATUS_ONLINE				(3)

#define SCE_NP_MANAGER_EVENT_GOT_TICKET			(255)

/*
 * Np Manager API
 */
int
sceNpManagerGetStatus(
	int *status
	);

int
sceNpManagerGetOnlineId(
	SceNpOnlineId *onlineId
	);

int
sceNpManagerGetNpId(
	SceNpId *npId
	);

/*
 * NP manager callback
 */
typedef void (*SceNpManagerCallback)(
	int event,
	int result,
	void *arg);

int
sceNpManagerRegisterCallback(
	SceNpManagerCallback callback,
	void *arg
	);

int
sceNpManagerUnregisterCallback(
	void
	);

int
sceNpManagerGetOnlineName(
	SceNpOnlineName *onlineName
	);

int
sceNpManagerGetAvatarUrl(
	SceNpAvatarUrl *avatarUrl
	);

int
sceNpManagerGetMyLanguages(
	SceNpMyLanguages *myLang
	);

int
sceNpManagerGetNetworkTime(
	CellRtcTick *pTick
	);

int
sceNpManagerGetAccountRegion(
	SceNpCountryCode *countryCode,
	int *languageCode
	);

int
sceNpManagerGetAccountAge(
	int *age
	);

int
sceNpManagerGetContentRatingFlag(
	int *isRestricted,
	int *age
	);

int
sceNpManagerGetChatRestrictionFlag(
	int *isRestricted
	);

int
sceNpManagerSubSignin(
	CellSysutilUserId userId,
	SceNpManagerSubSigninCallback cb_func,
	void *cb_arg,
	int flag
	);

int
sceNpManagerSubSignout(
	SceNpId *npId
	);

int
sceNpManagerSubSigninAbortGui(
	void
	);


int
sceNpManagerGetCachedInfo(
	CellSysutilUserId userID, 
	struct SceNpManagerCacheParam *param
	);

/* obsolete */
int
sceNpManagerGetPsHandle(
	SceNpPsHandle *psHandle
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_MANAGER_H_ */
