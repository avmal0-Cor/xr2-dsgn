/*   SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
*   Copyright (C) 2009 Sony Computer Entertainment Inc.
*   All Rights Reserved.
*/
#ifndef __SCE_NP_LOOK_UP_H__
#define __SCE_NP_LOOK_UP_H__

#include <np.h>

#define	SCE_NP_LOOKUP_MAX_CTX_NUM	(32)

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

int sceNpLookupInit(void);
int	sceNpLookupTerm(void);

int	sceNpLookupCreateTitleCtx(
	const SceNpCommunicationId *titleId,
	const SceNpId *selfNpId
);

int	sceNpLookupDestroyTitleCtx (
	int32_t titleCtxId
);

int	sceNpLookupCreateTransactionCtx(
	int32_t	titleCtxId
);

int	sceNpLookupDestroyTransactionCtx (
	int32_t transId
);

int sceNpLookupAbortTransaction (
	int32_t transId
);

int sceNpLookupSetTimeout(
	int32_t transId,
	usecond_t timeout
);

int	sceNpLookupWaitAsync (
	int32_t transId,
	int32_t *result
);

int	sceNpLookupPollAsync (
	int32_t transId,
	int32_t *result
);

int	sceNpLookupNpId(
	int32_t				transId,
	const SceNpOnlineId	*onlineId,
	SceNpId				*npId,
	void				*option
);

int	sceNpLookupNpIdAsync(
	int32_t				transId,
	const SceNpOnlineId	*onlineId,
	SceNpId				*npId,
	int32_t				prio,
	void				*option
);

int	sceNpLookupUserProfile(
	int32_t				transId,
	const SceNpId		*npId,
	SceNpUserInfo		*userInfo,
	SceNpAboutMe		*aboutMe,
	SceNpMyLanguages	*languages,
	SceNpCountryCode	*countryCode,
	SceNpAvatarImage	*avatarImage,
	void *option
);

int	sceNpLookupUserProfileAsync(
	int32_t				transId,
	const SceNpId		*npId,
	SceNpUserInfo		*userInfo,
	SceNpAboutMe		*aboutMe,
	SceNpMyLanguages	*languages,
	SceNpCountryCode	*countryCode,
	SceNpAvatarImage	*avatarImage,
	int32_t				prio,
	void				*option
);

int	sceNpLookupUserProfileWithAvatarSize(
	int32_t				transId,
	int32_t				avatarSizeType,
	const SceNpId		*npId,
	SceNpUserInfo		*userInfo,
	SceNpAboutMe		*aboutMe,
	SceNpMyLanguages	*languages,
	SceNpCountryCode	*countryCode,
	void				*avatarImageData,
	size_t				avatarImageDataMaxSize,
	size_t				*avatarImageDataSize,
	void *option
);

int	sceNpLookupUserProfileWithAvatarSizeAsync(
	int32_t				transId,
	int32_t				avatarSizeType,
	const SceNpId		*npId,
	SceNpUserInfo		*userInfo,
	SceNpAboutMe		*aboutMe,
	SceNpMyLanguages	*languages,
	SceNpCountryCode	*countryCode,
	void				*avatarImageData,
	size_t				avatarImageDataMaxSize,
	size_t				*avatarImageDataSize,
	int32_t				prio,
	void				*option
);



int	sceNpLookupAvatarImage(
	int32_t					transId,
	const SceNpAvatarUrl	*avatarUrl,
	SceNpAvatarImage		*avatarImage,
	void					*option
);


int	sceNpLookupAvatarImageAsync(
	int32_t					transId,
	const SceNpAvatarUrl	*avatarUrl,
	SceNpAvatarImage		*avatarImage,
	int32_t					prio,
	void					*option
);

int	sceNpLookupTitleSmallStorage(
	int32_t					transId,
	void					*data,
	size_t					maxSize,
	size_t					*contentLength,
	void *option
);

int	sceNpLookupTitleSmallStorageAsync(
	int32_t					transId,
	void					*data,
	size_t					maxSize,
	size_t					*contentLength,
	int32_t					prio,
	void *option
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /* __SCE_NP_LOOK_UP_H__*/


