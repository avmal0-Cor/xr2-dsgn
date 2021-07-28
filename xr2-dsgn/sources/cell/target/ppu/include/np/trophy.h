/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_TROPHY_H_
#define _SCE_NP_TROPHY_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/* type */
#define SCE_NP_TROPHY_GRADE_UNKNOWN						(0)
#define SCE_NP_TROPHY_GRADE_PLATINUM					(1)
#define SCE_NP_TROPHY_GRADE_GOLD						(2)
#define SCE_NP_TROPHY_GRADE_SILVER						(3)
#define SCE_NP_TROPHY_GRADE_BRONZE						(4)

/* status */
#define SCE_NP_TROPHY_STATUS_UNKNOWN					(0)
#define SCE_NP_TROPHY_STATUS_NOT_INSTALLED				(1)
#define SCE_NP_TROPHY_STATUS_DATA_CORRUPT				(2)
#define SCE_NP_TROPHY_STATUS_INSTALLED					(3)
#define SCE_NP_TROPHY_STATUS_REQUIRES_UPDATE			(4)

#define SCE_NP_TROPHY_STATUS_PROCESSING_SETUP			(5)
#define SCE_NP_TROPHY_STATUS_PROCESSING_PROGRESS		(6)
#define SCE_NP_TROPHY_STATUS_PROCESSING_FINALIZE		(7)

#define SCE_NP_TROPHY_STATUS_PROCESSING_COMPLETE		(8)

#define SCE_NP_TROPHY_STATUS_CHANGES_DETECTED			(9)

/* sizes */
#define SCE_NP_TROPHY_TITLE_MAX_SIZE					(128)
#define SCE_NP_TROPHY_GAME_DESCR_MAX_SIZE				(1024)
#define SCE_NP_TROPHY_NAME_MAX_SIZE						(128)
#define SCE_NP_TROPHY_DESCR_MAX_SIZE					(1024)

#define SCE_NP_TROPHY_INVALID_CONTEXT					(0)
#define SCE_NP_TROPHY_INVALID_HANDLE					(0)
#define SCE_NP_TROPHY_INVALID_TROPHY_ID					(-1)

/* options */
#define SCE_NP_TROPHY_OPTIONS_CREATE_CONTEXT_READ_ONLY	(0x0000000000000001)

#define SCE_NP_TROPHY_OPTIONS_REGISTER_CONTEXT_SHOW_ERROR_EXIT (0x0000000000000001)

/* trophy flag array */
#define SCE_NP_TROPHY_FLAG_SETSIZE			(128)
#define SCE_NP_TROPHY_FLAG_BITS				(sizeof(SceNpTrophyFlagMask) * 8)
#define SCE_NP_TROPHY_FLAG_BITS_ALL			((SceNpTrophyFlagMask)-1)
#define SCE_NP_TROPHY_FLAG_BITS_SHIFT		(5)
#define SCE_NP_TROPHY_FLAG_BITS_MASK		(SCE_NP_TROPHY_FLAG_BITS - 1)
#define SCE_NP_TROPHY_FLAG_BITS_MAX			(SCE_NP_TROPHY_FLAG_SETSIZE - 1)

#define SCE_NP_TROPHY_FLAG_SET(n, p) \
	((p)->flag_bits[(n) >> SCE_NP_TROPHY_FLAG_BITS_SHIFT] |= (1 << ((n) & SCE_NP_TROPHY_FLAG_BITS_MASK)))
#define SCE_NP_TROPHY_FLAG_CLR(n, p) \
	((p)->flag_bits[(n) >> SCE_NP_TROPHY_FLAG_BITS_SHIFT] &= ~(1 << ((n) & SCE_NP_TROPHY_FLAG_BITS_MASK)))
#define SCE_NP_TROPHY_FLAG_ISSET(n, p) \
	((p)->flag_bits[(n) >> SCE_NP_TROPHY_FLAG_BITS_SHIFT] & (1 << ((n) & SCE_NP_TROPHY_FLAG_BITS_MASK)))
#define SCE_NP_TROPHY_FLAG_ZERO(p) do { \
	SceNpTrophyFlagArray *__fa = (p); \
	uint32_t __i; \
	for(__i = 0; __i < (SCE_NP_TROPHY_FLAG_SETSIZE >> SCE_NP_TROPHY_FLAG_BITS_SHIFT); __i++) \
		__fa->flag_bits[__i] = 0; \
	} while(0)
#define SCE_NP_TROPHY_FLAG_SET_ALL(p) do { \
	SceNpTrophyFlagArray *__fa = (p); \
	uint32_t __i; \
	for(__i = 0; __i < (SCE_NP_TROPHY_FLAG_SETSIZE >> SCE_NP_TROPHY_FLAG_BITS_SHIFT); __i++) \
		__fa->flag_bits[__i] = SCE_NP_TROPHY_FLAG_BITS_ALL; \
	} while(0)

typedef uint32_t SceNpTrophyFlagMask;

typedef struct SceNpTrophyFlagArray {
	SceNpTrophyFlagMask flag_bits[SCE_NP_TROPHY_FLAG_SETSIZE >> SCE_NP_TROPHY_FLAG_BITS_SHIFT];
} SceNpTrophyFlagArray;

typedef int32_t SceNpTrophyId;
typedef uint32_t SceNpTrophyContext;
typedef uint32_t SceNpTrophyHandle;
typedef uint32_t SceNpTrophyStatus;
typedef uint32_t SceNpTrophyGrade;

typedef struct SceNpTrophyGameDetails {
	size_t numTrophies;
	size_t numPlatinum;
	size_t numGold;
	size_t numSilver;
	size_t numBronze;
	char title[SCE_NP_TROPHY_TITLE_MAX_SIZE];
	char description[SCE_NP_TROPHY_GAME_DESCR_MAX_SIZE];
	uint8_t reserved[4];
} SceNpTrophyGameDetails;

typedef struct SceNpTrophyGameData {
	size_t unlockedTrophies;
	size_t unlockedPlatinum;
	size_t unlockedGold;
	size_t unlockedSilver;
	size_t unlockedBronze;
} SceNpTrophyGameData;

typedef struct SceNpTrophyDetails {
	SceNpTrophyId trophyId;
	SceNpTrophyGrade trophyGrade;
	char name[SCE_NP_TROPHY_NAME_MAX_SIZE];
	char description[SCE_NP_TROPHY_DESCR_MAX_SIZE];
	bool hidden;
	uint8_t reserved[3];
} SceNpTrophyDetails;

typedef struct SceNpTrophyData {
	CellRtcTick timestamp;
	SceNpTrophyId trophyId;
	bool unlocked;
	uint8_t reserved[3];
} SceNpTrophyData;

typedef int (*SceNpTrophyStatusCallback)(
	SceNpTrophyContext context,
	SceNpTrophyStatus status,
	int completed,
	int total,
	void *arg
	);

/* library functions */
int sceNpTrophyInit(
	void *pool,
	size_t poolSize,
	sys_memory_container_t containerId,
	uint64_t options
	);

int sceNpTrophyTerm(
	void
	);

int sceNpTrophyCreateHandle(
	SceNpTrophyHandle *handle
	);

int sceNpTrophyDestroyHandle(
	SceNpTrophyHandle handle
	);

int sceNpTrophyAbortHandle(
	SceNpTrophyHandle handle
	);

int sceNpTrophyCreateContext(
	SceNpTrophyContext *context,
	const SceNpCommunicationId *commId,
	const SceNpCommunicationSignature *commSign,
	uint64_t options
	);

int sceNpTrophyGetRequiredDiskSpace(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	uint64_t *reqspace,
	uint64_t options
	);

int sceNpTrophyRegisterContext(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyStatusCallback statusCb,
	void *arg,
	uint64_t options
	);

int sceNpTrophyDestroyContext(
	SceNpTrophyContext context
	);

int sceNpTrophyGetGameInfo(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyGameDetails *details,
	SceNpTrophyGameData *data
	);

int sceNpTrophyGetTrophyUnlockState(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyFlagArray *flags,
	size_t *count
	);

int sceNpTrophyGetTrophyInfo(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyId trophyId,
	SceNpTrophyDetails *details,
	SceNpTrophyData *data
	);

int sceNpTrophyGetGameIcon(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	void *buffer,
	size_t *size
	);

int sceNpTrophyGetTrophyIcon(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyId trophyId,
	void *buffer,
	size_t *size
	);

int sceNpTrophySetSoundLevel(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	uint32_t level,
	uint64_t options
	);

int sceNpTrophyUnlockTrophy(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	SceNpTrophyId trophyId,
	SceNpTrophyId *platinumId
	);

int sceNpTrophyGetGameProgress(
	SceNpTrophyContext context,
	SceNpTrophyHandle handle,
	int32_t *percentage
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_TROPHY_H_ */
