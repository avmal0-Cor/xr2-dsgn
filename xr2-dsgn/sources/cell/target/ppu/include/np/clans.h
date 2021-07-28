/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_CLANS_H_
#define _SCE_NP_CLANS_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/* sizes */
#define SCE_NP_CLANS_ANNOUNCEMENT_MESSAGE_BODY_MAX_LENGTH		(1536)
#define SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH						(64)
#define SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH						(8)
#define SCE_NP_CLANS_CLAN_DESCRIPTION_MAX_LENGTH				(255)
#define SCE_NP_CLANS_CLAN_BINARY_ATTRIBUTE1_MAX_SIZE			(190)
#define SCE_NP_CLANS_CLAN_BINARY_DATA_MAX_SIZE					(10 * 1024)
#define SCE_NP_CLANS_MEMBER_BINARY_ATTRIBUTE1_MAX_SIZE			(16)
#define SCE_NP_CLANS_MEMBER_DESCRIPTION_MAX_LENGTH				(255)
#define SCE_NP_CLANS_MEMBER_BINARY_DATA_MAX_SIZE				(1024)
#define SCE_NP_CLANS_MESSAGE_BODY_MAX_LENGTH					(1536)
#define SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH					(54)
#define SCE_NP_CLANS_MESSAGE_BODY_CHARACTER_MAX					(512)
#define SCE_NP_CLANS_MESSAGE_SUBJECT_CHARACTER_MAX				(18)
#define SCE_NP_CLANS_MESSAGE_BINARY_DATA_MAX_SIZE				(1024)
#define SCE_NP_CLANS_PAGING_REQUEST_START_POSITION_MAX			(1000000)
#define SCE_NP_CLANS_PAGING_REQUEST_PAGE_MAX					(100)

/* roles */
#define SCE_NP_CLANS_ROLE_UNKNOWN								(0)
#define SCE_NP_CLANS_ROLE_NON_MEMBER							(1)
#define SCE_NP_CLANS_ROLE_MEMBER								(2)
#define SCE_NP_CLANS_ROLE_SUB_LEADER							(3)
#define SCE_NP_CLANS_ROLE_LEADER								(4)

/* bit fields */
#define SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_ATTR \
	(SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR1 \
	 | SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR2 \
	 | SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR3 \
	 | SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_BIN_ATTR1)
#define SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_PROFILE \
	(SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_TAG \
	 | SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_NUM_MEMBERS)
#define SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_PROFILE_AND_ATTR \
	(SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_ATTR \
	 | SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_PROFILE)
#define SCE_NP_CLANS_FIELDS_ALL_UPDATABLE_CLAN_INFO \
	(SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_DESCR \
	| SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_BIN_DATA1)
#define SCE_NP_CLANS_FIELDS_ALL_UPDATABLE_CLAN_INFO_AND_ATTR \
	(SCE_NP_CLANS_FIELDS_ALL_SEARCHABLE_ATTR \
	| SCE_NP_CLANS_FIELDS_ALL_UPDATABLE_CLAN_INFO)
#define SCE_NP_CLANS_FIELDS_ALL_UPDATABLE_MEMBER_INFO \
	(SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_DESCR \
	| SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_BIN_ATTR1 \
	| SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_BIN_DATA1 \
	| SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_ALLOW_MSG)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR1			(0x00000001)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR2			(0x00000002)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_INT_ATTR3			(0x00000004)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_ATTR_BIN_ATTR1			(0x00000008)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_TAG				(0x00000010)
#define SCE_NP_CLANS_FIELDS_SEARCHABLE_PROFILE_NUM_MEMBERS		(0x00000020)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_DESCR			(0x00000040)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_CLAN_INFO_BIN_DATA1		(0x00000080)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_DESCR			(0x00000100)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_BIN_ATTR1		(0x00000200)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_BIN_DATA1		(0x00000400)
#define SCE_NP_CLANS_FIELDS_UPDATABLE_MEMBER_INFO_ALLOW_MSG		(0x00000800)

/* search operators */
#define SCE_NP_CLANS_SEARCH_OPERATOR_EQUAL_TO					(0)
#define SCE_NP_CLANS_SEARCH_OPERATOR_NOT_EQUAL_TO				(1)
#define SCE_NP_CLANS_SEARCH_OPERATOR_GREATER_THAN				(2)
#define SCE_NP_CLANS_SEARCH_OPERATOR_GREATER_THAN_OR_EQUAL_TO	(3)
#define SCE_NP_CLANS_SEARCH_OPERATOR_LESS_THAN					(4)
#define SCE_NP_CLANS_SEARCH_OPERATOR_LESS_THAN_OR_EQUAL_TO		(5)
#define SCE_NP_CLANS_SEARCH_OPERATOR_SIMILAR_TO					(6)

/* status */
#define SCE_NP_CLANS_MEMBER_STATUS_UNKNOWN						(0)
#define SCE_NP_CLANS_MEMBER_STATUS_NORMAL						(1)
#define SCE_NP_CLANS_MEMBER_STATUS_INVITED						(2)
#define SCE_NP_CLANS_MEMBER_STATUS_PENDING						(3)

/* message options */
#define SCE_NP_CLANS_MESSAGE_OPTIONS_ALL \
	(SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR)
#define SCE_NP_CLANS_MESSAGE_OPTIONS_NONE						(0x00000000)
#define SCE_NP_CLANS_MESSAGE_OPTIONS_CENSOR						(0x00000001)

#define SCE_NP_CLANS_INVALID_ID									(0)
#define SCE_NP_CLANS_INVALID_REQUEST_HANDLE						(0)

struct SceNpClansRequest;
typedef struct SceNpClansRequest* SceNpClansRequestHandle;
typedef uint32_t SceNpClanId;
typedef uint32_t SceNpClansMessageId;
typedef uint32_t SceNpClansRole;
typedef int32_t SceNpClansMemberStatus;
typedef int32_t SceNpClansSearchOperator;

typedef struct SceNpClansPagingRequest {
	size_t startPos;
	size_t max;
} SceNpClansPagingRequest;

typedef struct SceNpClansPagingResult {
	size_t count;
	size_t total;
} SceNpClansPagingResult;

typedef struct SceNpClansClanBasicInfo {
	SceNpClanId clanId;
	size_t numMembers;
	char name[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH+1];
	char tag[SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH+1];
	uint8_t reserved[2];
} SceNpClansClanBasicInfo;

typedef struct SceNpClansEntry {
	SceNpClansClanBasicInfo info;
	SceNpClansRole role;
	SceNpClansMemberStatus status;
	bool allowMsg;
	uint8_t reserved[3];
} SceNpClansEntry;

typedef struct SceNpClansSearchableAttr {
	uint32_t fields;
	uint32_t intAttr1;
	uint32_t intAttr2;
	uint32_t intAttr3;
	uint8_t binAttr1[SCE_NP_CLANS_CLAN_BINARY_ATTRIBUTE1_MAX_SIZE];
	uint8_t reserved[2];
} SceNpClansSearchableAttr;

typedef struct SceNpClansSearchableProfile {
	SceNpClansSearchableAttr attr;
	uint32_t fields;
	size_t numMembers;
	SceNpClansSearchOperator tagSearchOp;
	SceNpClansSearchOperator numMembersSearchOp;
	SceNpClansSearchOperator intAttr1SearchOp;
	SceNpClansSearchOperator intAttr2SearchOp;
	SceNpClansSearchOperator intAttr3SearchOp;
	SceNpClansSearchOperator binAttr1SearchOp;
	char tag[SCE_NP_CLANS_CLAN_TAG_MAX_LENGTH+1];
	uint8_t reserved[3];
} SceNpClansSearchableProfile;

typedef struct SceNpClansSearchableName {
	SceNpClansSearchOperator nameSearchOp;
	char name[SCE_NP_CLANS_CLAN_NAME_MAX_LENGTH+1];
	uint8_t reserved[3];
} SceNpClansSearchableName;

typedef struct SceNpClansUpdatableClanInfo {
	uint32_t fields;
	char description[SCE_NP_CLANS_CLAN_DESCRIPTION_MAX_LENGTH+1];
	SceNpClansSearchableAttr attr;
	uint8_t *binData1;
	size_t binData1Size;
} SceNpClansUpdatableClanInfo;

typedef struct SceNpClansClanInfo {
	CellRtcTick dateCreated;
	SceNpClansClanBasicInfo info;
	SceNpClansUpdatableClanInfo updatable;
} SceNpClansClanInfo;

typedef struct SceNpClansUpdatableMemberInfo {
	uint32_t fields;
	uint8_t *binData1;
	size_t binData1Size;
	uint8_t binAttr1[SCE_NP_CLANS_MEMBER_BINARY_ATTRIBUTE1_MAX_SIZE];
	char description[SCE_NP_CLANS_MEMBER_DESCRIPTION_MAX_LENGTH+1];
	bool allowMsg;
	uint8_t reserved[3];
} SceNpClansUpdatableMemberInfo;

typedef struct SceNpClansMemberEntry {
	SceNpId npid;
	SceNpClansRole role;
	SceNpClansMemberStatus status;
	SceNpClansUpdatableMemberInfo updatable;
} SceNpClansMemberEntry;

typedef struct SceNpClansMessage {
	char subject[SCE_NP_CLANS_MESSAGE_SUBJECT_MAX_LENGTH+1];
	char body[SCE_NP_CLANS_MESSAGE_BODY_MAX_LENGTH+1];
	uint32_t options;
} SceNpClansMessage;

typedef struct SceNpClansMessageData {
	uint8_t *binData1;
	size_t binData1Size;
} SceNpClansMessageData;

typedef struct SceNpClansMessageEntry {
	CellRtcTick postDate;
	SceNpClansMessageId mId;
	SceNpClansMessage message;
	SceNpClansMessageData data;
	SceNpId npid;
	SceNpClanId postedBy;
} SceNpClansMessageEntry;

typedef struct SceNpClansBlacklistEntry {
	SceNpId entry;
	SceNpId registeredBy;
} SceNpClansBlacklistEntry;

/*
 * Clan Library Functions
 */
int sceNpClansInit(
	const SceNpCommunicationId *commId,
	const SceNpCommunicationPassphrase *passphrase,
	void *pool,
	size_t poolSize,
	uint32_t flags
	);

int sceNpClansTerm(
	void
	);

int sceNpClansCreateRequest(
	SceNpClansRequestHandle *handle,
	uint64_t flags
	);

int sceNpClansDestroyRequest(
	SceNpClansRequestHandle handle
	);

int sceNpClansAbortRequest(
	SceNpClansRequestHandle handle
	);

/*
 * General Clan Functions
 */
int sceNpClansCreateClan(
	SceNpClansRequestHandle handle,
	const char *name,
	const char *tag,
	SceNpClanId *clanId
	);

int sceNpClansDisbandClan(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId
	);

int sceNpClansGetClanList(
	SceNpClansRequestHandle handle,
	const SceNpClansPagingRequest *paging,
	SceNpClansEntry *clanList,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansGetClanListByNpId(
	SceNpClansRequestHandle handle,
	const SceNpClansPagingRequest *paging,
	const SceNpId *npid,
	SceNpClansEntry *clanList,
	SceNpClansPagingResult *pageResult
	);

/*
 * Clan Search Functions
 */
int sceNpClansSearchByProfile(
	SceNpClansRequestHandle handle,
	const SceNpClansPagingRequest *paging,
	const SceNpClansSearchableProfile *search,
	SceNpClansClanBasicInfo *results,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansSearchByName(
	SceNpClansRequestHandle handle,
	const SceNpClansPagingRequest *paging,
	const SceNpClansSearchableName *search,
	SceNpClansClanBasicInfo *results,
	SceNpClansPagingResult *pageResult
	);

/*
 * Clan Information Functions
 */
int sceNpClansGetClanInfo(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClansClanInfo *info
	);

int sceNpClansUpdateClanInfo(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansUpdatableClanInfo *info
	);

int sceNpClansGetMemberList(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansPagingRequest *paging,
	SceNpClansMemberStatus status,
	SceNpClansMemberEntry *memList,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansGetMemberInfo(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid,
	SceNpClansMemberEntry *memInfo
	);

int sceNpClansUpdateMemberInfo(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansUpdatableMemberInfo *info
	);

int sceNpClansChangeMemberRole(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid,
	SceNpClansRole role
	);

int sceNpClansUpdateAutoAcceptStatus(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	bool enable
	);

int sceNpClansGetAutoAcceptStatus(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	bool *enable
	);

/*
 * Clan Membership Functions
 */
int sceNpClansJoinClan(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId
	);

int sceNpClansLeaveClan(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId
	);

int sceNpClansKickMember(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid,
	const SceNpClansMessage *message
	);

int sceNpClansSendInvitation(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid,
	const SceNpClansMessage *message
	);

int sceNpClansCancelInvitation(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid
	);

int sceNpClansSendInvitationResponse(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansMessage *message,
	bool accept
	);

int sceNpClansSendMembershipRequest(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansMessage *message
	);

int sceNpClansCancelMembershipRequest(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId
	);

int sceNpClansSendMembershipResponse(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid,
	const SceNpClansMessage *message,
	bool allow
	);

/*
 * Clan Blacklist Functions
 */
int sceNpClansGetBlacklist(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansPagingRequest *paging,
	SceNpClansBlacklistEntry *bl,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansAddBlacklistEntry(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid
	);

int sceNpClansRemoveBlacklistEntry(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpId *npid
	);

/*
 * Clan Announcements Functions
 */
int sceNpClansRetrieveAnnouncements(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansPagingRequest *paging,
	SceNpClansMessageEntry *mList,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansPostAnnouncement(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansMessage *message,
	const SceNpClansMessageData *data,
	uint32_t duration,
	SceNpClansMessageId *mId
	);

int sceNpClansRemoveAnnouncement(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClansMessageId mId
	);

/*
 * Clan Challenge Functions
 */
int sceNpClansPostChallenge(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClanId targetClan,
	const SceNpClansMessage *message,
	const SceNpClansMessageData *data,
	uint32_t duration,
	SceNpClansMessageId *mId
	);

int sceNpClansRetrievePostedChallenges(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClanId targetClan,
	const SceNpClansPagingRequest *paging,
	SceNpClansMessageEntry *mList,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansRemovePostedChallenge(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClanId targetClan,
	SceNpClansMessageId mId
	);

int sceNpClansRetrieveChallenges(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	const SceNpClansPagingRequest *paging,
	SceNpClansMessageEntry *mList,
	SceNpClansPagingResult *pageResult
	);

int sceNpClansRemoveChallenge(
	SceNpClansRequestHandle handle,
	SceNpClanId clanId,
	SceNpClansMessageId mId
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_CLANS_H_ */
