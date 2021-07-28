/*   SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
*   Copyright (C) 2009 Sony Computer Entertainment Inc.
*   All Rights Reserved.
*/
#ifndef __SCE_NP_TITLE_USER_STORAGE_H__
#define __SCE_NP_TITLE_USER_STORAGE_H__

#include <np.h>

#define SCE_NP_TUS_DATA_INFO_MAX_SIZE		384
#define	SCE_NP_TUS_MAX_CTX_NUM	(32)
#define	SCE_NP_TUS_MAX_SLOT_NUM_PER_TRANS	(64)
#define	SCE_NP_TUS_MAX_USER_NUM_PER_TRANS	(101)


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

typedef	int32_t		SceNpTusSlotId;
typedef SceNpOnlineId SceNpTusVirtualUserId;

#include <np.h>

typedef	struct SceNpTusVariable{
	SceNpId ownerId;
	int32_t hasData;
	CellRtcTick lastChangedDate;
	uint8_t pad[4];
	SceNpId lastChangedAuthorId;
	int64_t variable;
	int64_t oldVariable;
	uint8_t reserved[16];
} SceNpTusVariable;

typedef	struct SceNpTusDataInfo{
	size_t infoSize;
	uint8_t pad[4];
	uint8_t data[SCE_NP_TUS_DATA_INFO_MAX_SIZE];
} SceNpTusDataInfo;

typedef	struct SceNpTusDataStatus{
	SceNpId ownerId;
	int32_t hasData;
	CellRtcTick lastChangedDate;
	SceNpId lastChangedAuthorId;
	void *data;
	size_t dataSize;
	uint8_t pad[4];
	SceNpTusDataInfo info;
} SceNpTusDataStatus;

typedef enum {
	SCE_NP_TUS_OPETYPE_EQUAL = 1,
	SCE_NP_TUS_OPETYPE_NOT_EQUAL,
	SCE_NP_TUS_OPETYPE_GREATER_THAN,
	SCE_NP_TUS_OPETYPE_GREATER_OR_EQUAL,
	SCE_NP_TUS_OPETYPE_LESS_THAN,
	SCE_NP_TUS_OPETYPE_LESS_OR_EQUAL
}SceNpTusOeratorType;

int32_t sceNpTusInit(int32_t prio);
int32_t sceNpTusTerm(void);

int32_t sceNpTusCreateTitleCtx(
	const SceNpCommunicationId *communicationId,
	const SceNpCommunicationPassphrase *passphrase,
	const SceNpId *selfNpId
);

int sceNpTusDestroyTitleCtx(
	int32_t titleCtxId
);

int sceNpTusCreateTransactionCtx(
	int32_t titleCtxId
);

int sceNpTusDestroyTransactionCtx (
	int32_t transId
);

int sceNpTusSetTimeout (
	int32_t ctxId,
	usecond_t timeout
);

int sceNpTusAbortTransaction(
	int32_t transId
);

int sceNpTusWaitAsync (
	int32_t transId,
	int32_t *result
);
int sceNpTusPollAsync (
	int32_t transId,
	int32_t *result
);

int sceNpTusSetMultiSlotVariable (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	const int64_t variableArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusSetMultiSlotVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	const int64_t variableArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusSetMultiSlotVariableAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	const int64_t variableArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusSetMultiSlotVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	const int64_t variableArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotVariable (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotVariableAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserVariable (
	int32_t	transId,
	const SceNpId targetNpIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId targetVirtualUserIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserVariableAsync (
	int32_t	transId,
	const SceNpId targetNpIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId targetVirtualUserIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusVariable variableArray[],
	size_t variableArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusAddAndGetVariable (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	int64_t inVariable,
	SceNpTusVariable *outVariable,
	size_t outVariableSize,
	void *option
);

int sceNpTusAddAndGetVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	int64_t inVariable,
	SceNpTusVariable *outVariable,
	size_t outVariableSize,
	void *option
);

int sceNpTusAddAndGetVariableAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	int64_t inVariable,
	SceNpTusVariable *outVariable,
	size_t outVariableSize,
	void *option
);

int sceNpTusAddAndGetVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	int64_t inVariable,
	SceNpTusVariable *outVariable,
	size_t outVariableSize,
	void *option
);

int sceNpTusTryAndSetVariable (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	int32_t opeType,
	int64_t variable,
	SceNpTusVariable *resultVariable,
	size_t	resultVariableSize,
	void *option
);

int sceNpTusTryAndSetVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	int32_t opeType,
	int64_t variable,
	SceNpTusVariable *resultVariable,
	size_t	resultVariableSize,
	void *option
);

int sceNpTusTryAndSetVariableAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	int32_t opeType,
	int64_t variable,
	SceNpTusVariable *resultVariable,
	size_t	resultVariableSize,
	void *option
);

int sceNpTusTryAndSetVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	int32_t opeType,
	int64_t variable,
	SceNpTusVariable *resultVariable,
	size_t	resultVariableSize,
	void *option
);

int sceNpTusDeleteMultiSlotVariable (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotVariableVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotVariableAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotVariableVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusSetData (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	size_t totalSize,
	size_t sendSize,
	const void *data,
	const SceNpTusDataInfo *info,
	size_t infoStructSize,
	void *option
);

int sceNpTusSetDataVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	size_t totalSize,
	size_t sendSize,
	const void *data,
	const SceNpTusDataInfo *info,
	size_t infoStructSize,
	void *option
);

int sceNpTusSetDataAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	size_t totalSize,
	size_t sendSize,
	const void *data,
	const SceNpTusDataInfo *info,
	size_t infoStructSize,
	void *option
);

int sceNpTusSetDataVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	size_t totalSize,
	size_t sendSize,
	const void *data,
	const SceNpTusDataInfo *info,
	size_t infoStructSize,
	void *option
);

int sceNpTusGetData (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	SceNpTusDataStatus *dataStatus,
	size_t dataStatusSize,
	void *data,
	size_t recvSize,
	void *option
);

int sceNpTusGetDataVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	SceNpTusDataStatus *dataStatus,
	size_t dataStatusSize,
	void *data,
	size_t recvSize,
	void *option
);

int sceNpTusGetDataAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	SceNpTusSlotId slotId,
	SceNpTusDataStatus *dataStatus,
	size_t dataStatusSize,
	void *data,
	size_t recvSize,
	void *option
);

int sceNpTusGetDataVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	SceNpTusSlotId slotId,
	SceNpTusDataStatus *dataStatus,
	size_t dataStatusSize,
	void *data,
	size_t recvSize,
	void *option
);

int sceNpTusGetMultiSlotDataStatus (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotDataStatusVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotDataStatusAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiSlotDataStatusVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserDataStatus (
	int32_t	transId,
	const SceNpId targetNpIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserDataStatusVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId targetVirtualUserIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserDataStatusAsync (
	int32_t	transId,
	const SceNpId targetNpIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusGetMultiUserDataStatusVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId targetVirtualUserIdArray[],
	SceNpTusSlotId slotId,
	SceNpTusDataStatus statusArray[],
	size_t statusArraySize,
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotData (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotDataVUser (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotDataAsync (
	int32_t	transId,
	const SceNpId *targetNpId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

int sceNpTusDeleteMultiSlotDataVUserAsync (
	int32_t	transId,
	const SceNpTusVirtualUserId *targetVirtualUserId,
	const SceNpTusSlotId slotIdArray[],
	int32_t arrayNum,
	void *option
);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /* __SCE_NP_TITLE_USER_STORAGE_H__*/
