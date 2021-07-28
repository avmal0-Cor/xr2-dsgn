/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *
 *      Copyright (C) 2007 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 */

#ifndef __SCE_NP_MATCHING2_H__
#define __SCE_NP_MATCHING2_H__

#include <sys/memory.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/*J
 *******************************************************************************
 * サイズ
 *******************************************************************************
 */

#define SCE_NP_MATCHING2_ATTR_BIN_MAX_SIZE                            256
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_BIN_ATTR_EXTERNAL_MAX_SIZE   64
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_EXTERNAL_MAX_SIZE              256
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_INTERNAL_MAX_SIZE              256
#define SCE_NP_MATCHING2_ROOMMEMBER_BIN_ATTR_INTERNAL_MAX_SIZE        64
#define SCE_NP_MATCHING2_LOBBY_BIN_ATTR_INTERNAL_MAX_SIZE             256
#define SCE_NP_MATCHING2_LOBBYMEMBER_BIN_ATTR_INTERNAL_MAX_SIZE       64
#define SCE_NP_MATCHING2_USER_BIN_ATTR_MAX_SIZE                       128
#define SCE_NP_MATCHING2_CHAT_MSG_MAX_SIZE                            1024
#define SCE_NP_MATCHING2_BIN_MSG_MAX_SIZE                             1024
#define SCE_NP_MATCHING2_PRESENCE_OPTION_DATA_MAX_SIZE                16
#define SCE_NP_MATCHING2_SESSION_NAME_MAX_SIZE                        64

#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetServerInfo                         4
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetWorldInfoList                      3848
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetUserInfoList                       17604
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetRoomMemberDataExternalList         15624
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetRoomDataExternalList               25768
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_CreateJoinRoom                        1304
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_JoinRoom                              24488
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_SearchRoom                            25776
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_SendRoomChatMessage                   1
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetRoomDataInternal                   24488
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetRoomMemberDataInternal             372
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetLobbyInfoList                      1296
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_JoinLobby                             1124
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_SendLobbyChatMessage                  1
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetLobbyMemberDataInternal            672
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetLobbyMemberDataInternalList        42760
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_GetLobbyMemberDataInternalList_NoEx   20488
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_SignalingGetPingInfo                  40

#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomMemberUpdateInfo                  396
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomOwnerUpdateInfo                   40
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomUpdateInfo                        28
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomDataInternalUpdateInfo            25404
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomMemberDataInternalUpdateInfo      493
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomMessageInfo                       1407
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_LobbyMemberUpdateInfo                 696
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_LobbyUpdateInfo                       8
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_LobbyMemberDataInternalUpdateInfo     472
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_LobbyMessageInfo                      1790
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_LobbyInvitationInfo                   870
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_SignalingOptParamUpdateInfo           8

//E Some Struct Max Sizes
/*E
 * In many circumstances using the MAX_SIZE of each event data is quite wasteful.
 * For example the JoinRoom and GetRoomDataInternal MAX_SIZEs were calculated
 * based on the max number of member slots that the library supports. To make a
 * better assessment of how much buffer space will be needed you can substract
 * the number of slots that won't be used. For example, if only 4 players are
 * supported in the game then (buffer size needed for JoinRoom and GetRoomDataInternal) =
 * _EVENT_DATA_MAX_SIZE_JoinRoom - ((SCE_NP_MATCHING2_ROOM_MAX_SLOT - 4) *
 * _MAX_SIZE_RoomMemberDataInternal).
 * The same goes for SearchRoom, GetLobbyMemberDataInternalList and GetUserInfo.
 */
#define SCE_NP_MATCHING2_MAX_SIZE_RoomDataExternal                                 1288
#define SCE_NP_MATCHING2_MAX_SIZE_RoomMemberDataInternal                           368
#define SCE_NP_MATCHING2_MAX_SIZE_LobbyMemberDataInternal                          668
#define SCE_NP_MATCHING2_MAX_SIZE_LobbyMemberDataInternal_NoExtendedData           80
#define SCE_NP_MATCHING2_MAX_SIZE_UserInfo                                         704

/*J
 *******************************************************************************
 * ID および雑多な構造体
 *******************************************************************************
 */

/*J ユーティリティ初期化パラメータ */
typedef struct SceNpMatching2UtilityInitParam {
	sys_memory_container_t containerId;
	uint32_t requestCbQueueLen;
	uint32_t sessionEventCbQueueLen;
	uint32_t sessionMsgCbQueueLen;
	uint8_t reserved[16];
} SceNpMatching2UtilityInitParam;

/*J メモリ情報 */
typedef struct SceNpMatching2MemoryInfo {
	size_t totalMemSize;    /*J メモリサイズ */
	size_t curMemUsage;     /*J 現在のメモリ消費量 */
	size_t maxMemUsage;     /*J 最大メモリ消費量 */
	uint8_t reserved[12];
} SceNpMatching2MemoryInfo;

/*J システムキュー情報 */
typedef struct SceNpMatching2CbQueueInfo {
	uint32_t requestCbQueueLen;
	uint32_t curRequestCbQueueLen;
	uint32_t maxRequestCbQueueLen;
	uint32_t sessionEventCbQueueLen;
	uint32_t curSessionEventCbQueueLen;
	uint32_t maxSessionEventCbQueueLen;
	uint32_t sessionMsgCbQueueLen;
	uint32_t curSessionMsgCbQueueLen;
	uint32_t maxSessionMsgCbQueueLen;
	uint8_t reserved[12];
} SceNpMatching2CbQueueInfo;

/*J サーバ ID */
typedef uint16_t SceNpMatching2ServerId;

/*J ワールド ID、ワールド番号 */
typedef uint32_t SceNpMatching2WorldId;
typedef uint16_t SceNpMatching2WorldNumber;
#define SCE_NP_MATCHING2_GET_WORLD_NUMBER(worldId) \
	( (SceNpMatching2WorldNumber)((worldId) & 0x0000ffffU) )

/*J ロビーID、ロビー番号 */
typedef uint64_t SceNpMatching2LobbyId;
typedef uint16_t SceNpMatching2LobbyNumber;
#define SCE_NP_MATCHING2_GET_LOBBY_NUMBER(lobbyId) \
	( (SceNpMatching2LobbyNumber)(((lobbyId) & 0x00000000ffff0000ULL)>>16) )

/*J ルーム ID、ルーム番号 */
typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2RoomNumber;
#define SCE_NP_MATCHING2_GET_ROOM_NUMBER(roomId) \
	( (SceNpMatching2RoomNumber)((roomId) & 0x000000000000ffffULL) )

/*J コンテキスト ID */
typedef uint16_t SceNpMatching2ContextId;

/*J リクエスト ID */
typedef uint32_t SceNpMatching2RequestId;

/*J タイトル用パスフレーズ for compatibility */
#define SCE_NP_MATCHING2_TITLE_PASSPHRASE_SIZE 128
typedef SceNpCommunicationPassphrase SceNpMatching2TitlePassphrase;

/*J NAT タイプ */
typedef uint8_t SceNpMatching2NatType;

/*J 検索条件 */
#define SCE_NP_MATCHING2_OPERATOR_EQ   1
#define SCE_NP_MATCHING2_OPERATOR_NE   2
#define SCE_NP_MATCHING2_OPERATOR_LT   3
#define SCE_NP_MATCHING2_OPERATOR_LE   4
#define SCE_NP_MATCHING2_OPERATOR_GT   5
#define SCE_NP_MATCHING2_OPERATOR_GE   6
typedef uint8_t SceNpMatching2Operator;

/*J メッセージ送信タイプ */
#define SCE_NP_MATCHING2_CASTTYPE_BROADCAST 		1
#define SCE_NP_MATCHING2_CASTTYPE_UNICAST   		2
#define SCE_NP_MATCHING2_CASTTYPE_MULTICAST 		3
#define SCE_NP_MATCHING2_CASTTYPE_MULTICAST_TEAM    4
typedef uint8_t SceNpMatching2CastType;

/*J セッションパスワード */
#define SCE_NP_MATCHING2_SESSION_PASSWORD_SIZE 8
typedef struct SceNpMatching2SessionPassword {
	uint8_t data[SCE_NP_MATCHING2_SESSION_PASSWORD_SIZE];
} SceNpMatching2SessionPassword;

/*J セッションタイプ */
#define SCE_NP_MATCHING2_SESSION_TYPE_LOBBY       1
#define SCE_NP_MATCHING2_SESSION_TYPE_ROOM        2
typedef uint8_t SceNpMatching2SessionType;

/*J イベント要因 */
#define SCE_NP_MATCHING2_EVENT_CAUSE_LEAVE_ACTION        1
#define SCE_NP_MATCHING2_EVENT_CAUSE_KICKOUT_ACTION      2
#define SCE_NP_MATCHING2_EVENT_CAUSE_GRANT_OWNER_ACTION  3
#define SCE_NP_MATCHING2_EVENT_CAUSE_SERVER_OPERATION    4
#define SCE_NP_MATCHING2_EVENT_CAUSE_MEMBER_DISAPPEARED  5
#define SCE_NP_MATCHING2_EVENT_CAUSE_SERVER_INTERNAL     6
#define SCE_NP_MATCHING2_EVENT_CAUSE_CONNECTION_ERROR    7
#define SCE_NP_MATCHING2_EVENT_CAUSE_NP_SIGNED_OUT       8
#define SCE_NP_MATCHING2_EVENT_CAUSE_SYSTEM_ERROR        9
#define SCE_NP_MATCHING2_EVENT_CAUSE_CONTEXT_ERROR       10
#define SCE_NP_MATCHING2_EVENT_CAUSE_CONTEXT_ACTION      11
typedef uint8_t SceNpMatching2EventCause;

/*J ルームもしくはロビーに参加、退出などをする際に付随可能なデータ */
#define SCE_NP_MATCHING2_PRESENCE_OPTION_DATA_SIZE 16
typedef struct SceNpMatching2PresenceOptionData {
	uint8_t data[SCE_NP_MATCHING2_PRESENCE_OPTION_DATA_SIZE];
	size_t len;
} SceNpMatching2PresenceOptionData;


/*J
 *******************************************************************************
 * 属性、検索条件
 *******************************************************************************
 */

/*J 属性 ID */
typedef uint16_t SceNpMatching2AttributeId;

/*J
 * フラグ属性
 */

/*J ロビーフラグ属性 */
#define SCE_NP_MATCHING2_LOBBY_FLAG_ATTR_PERMANENT               0x80000000
#define SCE_NP_MATCHING2_LOBBY_FLAG_ATTR_CLAN                    0x40000000
#define SCE_NP_MATCHING2_LOBBY_FLAG_ATTR_MEMBER_NOTIFICATION     0x20000000

/*J ルームフラグ属性 */
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_OWNER_AUTO_GRANT         0x80000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_CLOSED                   0x40000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_FULL                     0x20000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_HIDDEN                   0x10000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_GLOBAL_SEARCH_AVAILABLE  0x08000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_NAT_TYPE_RESTRICTION     0x04000000
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_PROHIBITIVE_MODE         0x02000000

/*J ロビーメンバフラグ属性 */
#define SCE_NP_MATCHING2_LOBBYMEMBER_FLAG_ATTR_OWNER             0x80000000

/*J ルームメンバフラグ属性 */
#define SCE_NP_MATCHING2_ROOMMEMBER_FLAG_ATTR_OWNER              0x80000000
typedef uint32_t SceNpMatching2FlagAttr;

/*J
 * 数値属性
 */

/*J ルーム数値属性 */
/*J ルーム外部検索数値属性(8つ) */
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_NUM   8
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_1_ID  0x004c
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_2_ID  0x004d
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_3_ID  0x004e
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_4_ID  0x004f
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_5_ID  0x0050
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_6_ID  0x0051
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_7_ID  0x0052
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_INT_ATTR_EXTERNAL_8_ID  0x0053

/*J 数値属性構造体 */
typedef struct SceNpMatching2IntAttr {
	SceNpMatching2AttributeId id;
	uint8_t  padding[2];
	uint32_t num;
} SceNpMatching2IntAttr;

/*J
 * バイナリ属性
 */
/*J ロビー内部バイナリ属性(2つ) */
#define SCE_NP_MATCHING2_LOBBY_BIN_ATTR_INTERNAL_NUM             2
#define SCE_NP_MATCHING2_LOBBY_BIN_ATTR_INTERNAL_1_ID            0x0037
#define SCE_NP_MATCHING2_LOBBY_BIN_ATTR_INTERNAL_2_ID            0x0038

/*J ロビー内部ロビーメンババイナリ属性(1つ) */
#define SCE_NP_MATCHING2_LOBBYMEMBER_BIN_ATTR_INTERNAL_NUM       1
#define SCE_NP_MATCHING2_LOBBYMEMBER_BIN_ATTR_INTERNAL_1_ID      0x0039

/*J ルーム内部バイナリ属性(2つ) */
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_INTERNAL_NUM              2
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_INTERNAL_1_ID             0x0057
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_INTERNAL_2_ID             0x0058

/*J ルーム内部ルームメンババイナリ属性(1つ) */
#define SCE_NP_MATCHING2_ROOMMEMBER_BIN_ATTR_INTERNAL_NUM        1
#define SCE_NP_MATCHING2_ROOMMEMBER_BIN_ATTR_INTERNAL_1_ID       0x0059

/*J ルーム外部検索バイナリ属性(1つ) */
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_BIN_ATTR_EXTERNAL_NUM   1
#define SCE_NP_MATCHING2_ROOM_SEARCHABLE_BIN_ATTR_EXTERNAL_1_ID  0x0054

/*J ルーム外部バイナリ属性(2つ) */
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_EXTERNAL_NUM              2
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_EXTERNAL_1_ID             0x0055
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_EXTERNAL_2_ID             0x0056

/*J ユーザバイナリ属性(1つ) */
#define SCE_NP_MATCHING2_USER_BIN_ATTR_NUM                       1
#define SCE_NP_MATCHING2_USER_BIN_ATTR_1_ID                      0x005f

/*J バイナリ属性構造体 */
typedef struct SceNpMatching2BinAttr {
	SceNpMatching2AttributeId id;
	uint8_t padding[2];
	void   *ptr;
	size_t  size;
} SceNpMatching2BinAttr;

/*J
 * 検索条件
 */

/*J 検索対象範囲 */
typedef struct SceNpMatching2RangeFilter {
	unsigned int startIndex;
	unsigned int max;
} SceNpMatching2RangeFilter;
/*J StartIndexの最小値 */
#define SCE_NP_MATCHING2_RANGE_FILTER_START_INDEX_MIN	1
/*J 一度に取得できるルームリストの最大数 */
#define SCE_NP_MATCHING2_RANGE_FILTER_MAX				20

/*J 数値検索条件 */
typedef struct SceNpMatching2IntSearchFilter {
	SceNpMatching2Operator searchOperator;
	uint8_t padding[3];
	SceNpMatching2IntAttr attr;
} SceNpMatching2IntSearchFilter;

/*J バイナリ検索条件 */
typedef struct SceNpMatching2BinSearchFilter {
	SceNpMatching2Operator searchOperator;
	uint8_t padding[3];
	SceNpMatching2BinAttr attr;
} SceNpMatching2BinSearchFilter;

/*J 検索結果の範囲 */
typedef struct SceNpMatching2Range {
	unsigned int startIndex;
	unsigned int total;
	unsigned int size;
} SceNpMatching2Range;

/*J
 *******************************************************************************
 * ユーザ情報
 *******************************************************************************
 */

/*J ユーザが参加中のセッション情報 */
typedef struct SceNpMatching2JoinedSessionInfo {
	SceNpMatching2SessionType sessionType;
	uint8_t padding1[1];
	SceNpMatching2ServerId serverId;	
	SceNpMatching2WorldId worldId;
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2RoomId  roomId;
	CellRtcTick joinDate;
} SceNpMatching2JoinedSessionInfo;

/*J ユーザ情報構造体 */
typedef struct SceNpMatching2UserInfo {
	struct SceNpMatching2UserInfo *next;
	SceNpUserInfo2 userInfo;
	SceNpMatching2BinAttr *userBinAttr;
	unsigned int userBinAttrNum;
	SceNpMatching2JoinedSessionInfo *joinedSessionInfo;
	unsigned int joinedSessionInfoNum;
} SceNpMatching2UserInfo;

/*J
 *******************************************************************************
 * サーバ情報
 *******************************************************************************
 */

/*J サーバステータス */
#define SCE_NP_MATCHING2_SERVER_STATUS_AVAILABLE       1
#define SCE_NP_MATCHING2_SERVER_STATUS_UNAVAILABLE     2
#define SCE_NP_MATCHING2_SERVER_STATUS_BUSY            3
#define SCE_NP_MATCHING2_SERVER_STATUS_MAINTENANCE     4
typedef uint8_t SceNpMatching2ServerStatus;

/*J サーバ構造体 */
typedef struct SceNpMatching2Server {
	SceNpMatching2ServerId     serverId;
	SceNpMatching2ServerStatus status;
	uint8_t padding[1];
} SceNpMatching2Server;


/*J
 *******************************************************************************
 * ワールド情報
 *******************************************************************************
 */

/*J ワールド構造体 */
typedef struct SceNpMatching2World {
	SceNpMatching2WorldId worldId;
	unsigned int numOfLobby;
	unsigned int maxNumOfTotalLobbyMember;
	unsigned int curNumOfTotalLobbyMember;
	unsigned int curNumOfRoom;
	unsigned int curNumOfTotalRoomMember;
	bool withEntitlementId;
	SceNpEntitlementId entitlementId;	
	uint8_t padding[3];
} SceNpMatching2World;


/*J
 *******************************************************************************
 * ロビー情報
 *******************************************************************************
 */

/*J ロビーの最大参加人数 */
#define SCE_NP_MATCHING2_LOBBY_MAX_SLOT      256

/*J ロビーメンバ ID */
typedef uint16_t SceNpMatching2LobbyMemberId;

/*J ロビー内部ロビーメンババイナリ属性 */
typedef struct SceNpMatching2LobbyMemberBinAttrInternal {
	CellRtcTick updateDate;
	SceNpMatching2BinAttr data;
	uint8_t padding[4];
} SceNpMatching2LobbyMemberBinAttrInternal;

/*J ロビー内部から取得するロビーメンバ情報 */
typedef struct SceNpMatching2LobbyMemberDataInternal {
	struct SceNpMatching2LobbyMemberDataInternal *next;
	SceNpUserInfo2 userInfo;

	CellRtcTick joinDate;
	SceNpMatching2LobbyMemberId memberId;
	uint8_t padding[2];
	
	SceNpMatching2FlagAttr flagAttr; /*J 未使用 */

	SceNpMatching2JoinedSessionInfo *joinedSessionInfo;
	unsigned int joinedSessionInfoNum;
	SceNpMatching2LobbyMemberBinAttrInternal *lobbyMemberBinAttrInternal;
	unsigned int lobbyMemberBinAttrInternalNum;
} SceNpMatching2LobbyMemberDataInternal;

/*J ロビー内部用ロビーメンバIDリスト */
typedef struct SceNpMatching2LobbyMemberIdList {
	SceNpMatching2LobbyMemberId *memberId;
	unsigned int memberIdNum;
	SceNpMatching2LobbyMemberId me;
	uint8_t padding[6];
} SceNpMatching2LobbyMemberIdList;

/*J ロビー内部バイナリ属性 */
typedef struct SceNpMatching2LobbyBinAttrInternal {
	CellRtcTick updateDate;
	SceNpMatching2LobbyMemberId updateMemberId;	
	uint8_t padding[2];
	SceNpMatching2BinAttr data;
} SceNpMatching2LobbyBinAttrInternal;

/*J ロビー外部から取得するロビー情報 */
typedef struct SceNpMatching2LobbyDataExternal {
	struct SceNpMatching2LobbyDataExternal *next;
	SceNpMatching2ServerId serverId;
	uint8_t padding1[2];
	SceNpMatching2WorldId worldId;
	uint8_t padding2[4];
	SceNpMatching2LobbyId lobbyId;
	unsigned int maxSlot;
	unsigned int curMemberNum;
	SceNpMatching2FlagAttr flagAttr;
	/*J 未使用 */
	SceNpMatching2IntAttr *lobbySearchableIntAttrExternal;
	unsigned int lobbySearchableIntAttrExternalNum;
	SceNpMatching2BinAttr *lobbySearchableBinAttrExternal;
	unsigned int lobbySearchableBinAttrExternalNum;
	SceNpMatching2BinAttr *lobbyBinAttrExternal;
	unsigned int lobbyBinAttrExternalNum;
	uint8_t padding3[4];
} SceNpMatching2LobbyDataExternal;

/*J ロビー内部から取得するロビー情報 */
typedef struct SceNpMatching2LobbyDataInternal {
	SceNpMatching2ServerId serverId;
	uint8_t padding1[2];
	SceNpMatching2WorldId worldId;
	SceNpMatching2LobbyId lobbyId;
	
	unsigned int maxSlot;	
	SceNpMatching2LobbyMemberIdList memberIdList;
	SceNpMatching2FlagAttr flagAttr;
	
	/*J 未使用 */
	SceNpMatching2LobbyBinAttrInternal *lobbyBinAttrInternal;
	unsigned int lobbyBinAttrInternalNum;	
} SceNpMatching2LobbyDataInternal;

/*J ロビーメッセージの送信先指定 */
typedef union SceNpMatching2LobbyMessageDestination {
	SceNpMatching2LobbyMemberId unicastTarget;
	struct {
		SceNpMatching2LobbyMemberId *memberId;
		unsigned int memberIdNum;
	} multicastTarget;
} SceNpMatching2LobbyMessageDestination;

/*J
 *******************************************************************************
 * ルーム情報
 *******************************************************************************
 */

/*J ルームの最大参加人数 */
#define SCE_NP_MATCHING2_ROOM_MAX_SLOT      64

/*J ルームグループ ID */
#define SCE_NP_MATCHING2_ROOM_GROUP_ID_MAX 15
typedef uint8_t SceNpMatching2RoomGroupId;

/*J ルームメンバ ID */
typedef uint16_t SceNpMatching2RoomMemberId;

/*J チーム ID */
typedef uint8_t SceNpMatching2TeamId;

/*J ルーム内での権限 */
#define SCE_NP_MATCHING2_ROLE_MEMBER   1
#define SCE_NP_MATCHING2_ROLE_OWNER    2
typedef uint8_t SceNpMatching2Role;

/*J キックアウトしたメンバの再参加に関する設定 */
#define SCE_NP_MATCHING2_BLOCKKICKFLAG_OK   0
#define SCE_NP_MATCHING2_BLOCKKICKFLAG_NG   1
typedef uint8_t SceNpMatching2BlockKickFlag;

/*J グループラベル */
#define SCE_NP_MATCHING2_GROUP_LABEL_SIZE 8
typedef struct SceNpMatching2GroupLabel {
	uint8_t data[SCE_NP_MATCHING2_GROUP_LABEL_SIZE];
} SceNpMatching2GroupLabel;

/*J ルームパスワード用スロットマスク */
typedef uint64_t SceNpMatching2RoomPasswordSlotMask;

/*J ルームのグループ設定 */
typedef struct SceNpMatching2RoomGroupConfig {
	unsigned int slotNum;
	bool withLabel;
	SceNpMatching2GroupLabel label;
	bool withPassword;
	uint8_t padding[2];
} SceNpMatching2RoomGroupConfig;

/*J ルームグループのパスワード使用設定用構造体 */
typedef struct SceNpMatching2RoomGroupPasswordConfig {
	SceNpMatching2RoomGroupId groupId;	
	bool withPassword;
	uint8_t padding[1];
} SceNpMatching2RoomGroupPasswordConfig;

/*J ルーム内部ルームメンババイナリデータ */
typedef struct SceNpMatching2RoomMemberBinAttrInternal {
	CellRtcTick updateDate;
	SceNpMatching2BinAttr data;
	uint8_t padding[4];
} SceNpMatching2RoomMemberBinAttrInternal;

/*J ルームグループ */
typedef struct SceNpMatching2RoomGroup {
	SceNpMatching2RoomGroupId groupId;	
	bool withPassword;
	bool withLabel;
	uint8_t padding[1];
	SceNpMatching2GroupLabel label;
	unsigned int slotNum;
	unsigned int curGroupMemberNum;
} SceNpMatching2RoomGroup;

/*J ルーム外部から取得するルームメンバ情報 */
typedef struct SceNpMatching2RoomMemberDataExternal {
	struct SceNpMatching2RoomMemberDataExternal *next;
	SceNpUserInfo2 userInfo;
	CellRtcTick joinDate;
	SceNpMatching2Role role;
	uint8_t padding[7];
} SceNpMatching2RoomMemberDataExternal;

/*J ルーム内部から取得するルームメンバ情報 */
typedef struct SceNpMatching2RoomMemberDataInternal {
	struct SceNpMatching2RoomMemberDataInternal *next;
	SceNpUserInfo2 userInfo;

	CellRtcTick joinDate;
	SceNpMatching2RoomMemberId memberId;
	SceNpMatching2TeamId teamId;
	uint8_t padding1[1];
	
	SceNpMatching2RoomGroup *roomGroup;

	SceNpMatching2NatType natType;
	uint8_t padding2[3];
	SceNpMatching2FlagAttr flagAttr;
	SceNpMatching2RoomMemberBinAttrInternal *roomMemberBinAttrInternal;
	unsigned int roomMemberBinAttrInternalNum;
} SceNpMatching2RoomMemberDataInternal;

/*J ルーム内部用ルームメンバリスト */
typedef struct SceNpMatching2RoomMemberDataInternalList {
	SceNpMatching2RoomMemberDataInternal *members;
	unsigned int membersNum;
	SceNpMatching2RoomMemberDataInternal *me;
	SceNpMatching2RoomMemberDataInternal *owner;
} SceNpMatching2RoomMemberDataInternalList;

/*J ソートメソッド */
#define SCE_NP_MATCHING2_SORT_METHOD_JOIN_DATE			(0)
#define SCE_NP_MATCHING2_SORT_METHOD_SLOT_NUMBER		(1)

/*J ルーム内部バイナリ属性 */
typedef struct SceNpMatching2RoomBinAttrInternal {
	CellRtcTick updateDate;
	SceNpMatching2RoomMemberId updateMemberId;
	uint8_t padding[2];
	SceNpMatching2BinAttr data;
} SceNpMatching2RoomBinAttrInternal;

/*J ルーム外部から取得するルーム情報 */
typedef struct SceNpMatching2RoomDataExternal {
	struct SceNpMatching2RoomDataExternal *next;
	
	SceNpMatching2ServerId serverId;
	uint8_t padding1[2];
	SceNpMatching2WorldId worldId;
	uint8_t padding2[4];
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2RoomId roomId;

	unsigned int maxSlot;
	unsigned int curMemberNum;
	SceNpMatching2RoomPasswordSlotMask passwordSlotMask;

	/*J ルームオーナー情報 */
	SceNpUserInfo2 *owner;

	/*J ルームグループ情報 */
	SceNpMatching2RoomGroup *roomGroup;
	unsigned int roomGroupNum;

	SceNpMatching2FlagAttr flagAttr;

	/*J 検索数値属性 */
	SceNpMatching2IntAttr *roomSearchableIntAttrExternal;
	unsigned int roomSearchableIntAttrExternalNum;

	/*J 検索バイナリ属性 */
	SceNpMatching2BinAttr *roomSearchableBinAttrExternal;
	unsigned int roomSearchableBinAttrExternalNum;

	/*J バイナリ属性 */
	SceNpMatching2BinAttr *roomBinAttrExternal;
	unsigned int roomBinAttrExternalNum;
} SceNpMatching2RoomDataExternal;

/*J ルーム内部から取得するルーム情報 */
typedef struct SceNpMatching2RoomDataInternal {
	SceNpMatching2ServerId serverId;
	uint8_t padding1[2];
	SceNpMatching2WorldId worldId;
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2RoomId roomId;

	SceNpMatching2RoomPasswordSlotMask passwordSlotMask;
	unsigned int maxSlot;

	/*J ルームメンバ */
	SceNpMatching2RoomMemberDataInternalList memberList;

	/*J ルームグループ情報 */
	SceNpMatching2RoomGroup *roomGroup;
	unsigned int roomGroupNum;

	SceNpMatching2FlagAttr flagAttr;
	SceNpMatching2RoomBinAttrInternal *roomBinAttrInternal;
	unsigned int roomBinAttrInternalNum;
} SceNpMatching2RoomDataInternal;

/*J ルームメッセージの送信先指定 */
typedef union SceNpMatching2RoomMessageDestination {
	SceNpMatching2RoomMemberId unicastTarget;
	struct {
		SceNpMatching2RoomMemberId *memberId;
		unsigned int memberIdNum;
	} multicastTarget;
	SceNpMatching2TeamId multicastTargetTeamId;
} SceNpMatching2RoomMessageDestination;

/*J sceNpMatching2CreateContext() で指定するオプション */
#define SCE_NP_MATCHING2_CONTEXT_OPTION_USE_ONLINENAME  (0x01)
#define SCE_NP_MATCHING2_CONTEXT_OPTION_USE_AVATARURL   (0x02)

/*J sceNpMatching2GetUserInfoList() で指定するオプション */
#define SCE_NP_MATCHING2_GET_USER_INFO_LIST_OPTION_WITH_ONLINENAME (0x01)
#define SCE_NP_MATCHING2_GET_USER_INFO_LIST_OPTION_WITH_AVATARURL  (0x02)

/*J sceNpMatching2SearchRoom() で指定するオプション */
#define SCE_NP_MATCHING2_SEARCH_ROOM_OPTION_WITH_NPID        (0x01)
#define SCE_NP_MATCHING2_SEARCH_ROOM_OPTION_WITH_ONLINENAME  (0x02)
#define SCE_NP_MATCHING2_SEARCH_ROOM_OPTION_WITH_AVATARURL   (0x04)
#define SCE_NP_MATCHING2_SEARCH_ROOM_OPTION_NAT_TYPE_FILTER  (0x08)
#define SCE_NP_MATCHING2_SEARCH_ROOM_OPTION_RANDOM           (0x10)

/*J
 * sceNpMatching2SendRoomChatMessage()
 * sceNpMatching2SendRoomMessage()
 * sceNpMatching2SendLobbyChatMessage()
 * sceNpMatching2SendLobbyInvitation()
 * で指定するオプション
 */
#define SCE_NP_MATCHING2_SEND_MSG_OPTION_WITH_NPID        (0x01)
#define SCE_NP_MATCHING2_SEND_MSG_OPTION_WITH_ONLINENAME  (0x02)
#define SCE_NP_MATCHING2_SEND_MSG_OPTION_WITH_AVATARURL   (0x04)

/*J ルーム入室許可ユーザ最大数 */
#define SCE_NP_MATCHING2_ROOM_ALLOWED_USER_MAX	100
/*J ルーム入室拒否ユーザ最大数 */
#define SCE_NP_MATCHING2_ROOM_BLOCKED_USER_MAX	100

/*J
 *******************************************************************************
 * 招待データ
 *******************************************************************************
 */
typedef struct SceNpMatching2InvitationData {
	SceNpMatching2JoinedSessionInfo *targetSession;
	unsigned int targetSessionNum;
	void *optData;
	size_t optDataLen;
} SceNpMatching2InvitationData;

/*J 招待先セッション最大数 */
#define SCE_NP_MATCHING2_INVITATION_TARGET_SESSION_MAX   (2)
/*J 招待情報オプションデータ最大サイズ */
#define SCE_NP_MATCHING2_INVITATION_OPTION_DATA_MAX_SIZE (32)

/*J
 *******************************************************************************
 * コールバック関数
 *******************************************************************************
 */

/*J コールバック関数イベント */
typedef uint16_t SceNpMatching2Event;
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetServerInfo                    (0x0001)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetWorldInfoList                 (0x0002)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetRoomMemberDataExternalList    (0x0003)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetRoomDataExternal              (0x0004)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetRoomDataExternalList          (0x0005)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetLobbyInfoList                 (0x0006)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetUserInfo                      (0x0007)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetUserInfoList                  (0x0008)
#define SCE_NP_MATCHING2_REQUEST_EVENT_CreateServerContext              (0x0009)
#define SCE_NP_MATCHING2_REQUEST_EVENT_DeleteServerContext              (0x000a)
#define SCE_NP_MATCHING2_REQUEST_EVENT_CreateJoinRoom                   (0x0101)
#define SCE_NP_MATCHING2_REQUEST_EVENT_JoinRoom                         (0x0102)
#define SCE_NP_MATCHING2_REQUEST_EVENT_LeaveRoom                        (0x0103)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GrantRoomOwner                   (0x0104)
#define SCE_NP_MATCHING2_REQUEST_EVENT_KickoutRoomMember                (0x0105)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SearchRoom                       (0x0106)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SendRoomChatMessage              (0x0107)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SendRoomMessage                  (0x0108)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetRoomDataInternal              (0x0109)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetRoomDataInternal              (0x010a)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetRoomMemberDataInternal        (0x010b)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetRoomMemberDataInternal        (0x010c)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetSignalingOptParam             (0x010d)
#define SCE_NP_MATCHING2_REQUEST_EVENT_JoinLobby                        (0x0201)
#define SCE_NP_MATCHING2_REQUEST_EVENT_LeaveLobby                       (0x0202)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SendLobbyChatMessage             (0x0203)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SendLobbyInvitation              (0x0204)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetLobbyMemberDataInternal       (0x0205)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetLobbyMemberDataInternal       (0x0206)
#define SCE_NP_MATCHING2_REQUEST_EVENT_GetLobbyMemberDataInternalList   (0x0207)
#define SCE_NP_MATCHING2_REQUEST_EVENT_SignalingGetPingInfo             (0x0e01)
#define SCE_NP_MATCHING2_ROOM_EVENT_MemberJoined                        (0x1101)
#define SCE_NP_MATCHING2_ROOM_EVENT_MemberLeft                          (0x1102)
#define SCE_NP_MATCHING2_ROOM_EVENT_Kickedout                           (0x1103)
#define SCE_NP_MATCHING2_ROOM_EVENT_RoomDestroyed                       (0x1104)
#define SCE_NP_MATCHING2_ROOM_EVENT_RoomOwnerChanged                    (0x1105)
#define SCE_NP_MATCHING2_ROOM_EVENT_UpdatedRoomDataInternal             (0x1106)
#define SCE_NP_MATCHING2_ROOM_EVENT_UpdatedRoomMemberDataInternal       (0x1107)
#define SCE_NP_MATCHING2_ROOM_EVENT_UpdatedSignalingOptParam            (0x1108)
#define SCE_NP_MATCHING2_ROOM_MSG_EVENT_ChatMessage                     (0x2101)
#define SCE_NP_MATCHING2_ROOM_MSG_EVENT_Message                         (0x2102)
#define SCE_NP_MATCHING2_LOBBY_EVENT_MemberJoined                       (0x3201)
#define SCE_NP_MATCHING2_LOBBY_EVENT_MemberLeft                         (0x3202)
#define SCE_NP_MATCHING2_LOBBY_EVENT_LobbyDestroyed                     (0x3203)
#define SCE_NP_MATCHING2_LOBBY_EVENT_UpdatedLobbyMemberDataInternal     (0x3204)
#define SCE_NP_MATCHING2_LOBBY_MSG_EVENT_ChatMessage                    (0x4201)
#define SCE_NP_MATCHING2_LOBBY_MSG_EVENT_Invitation                     (0x4202)
#define SCE_NP_MATCHING2_SIGNALING_EVENT_Dead                           (0x5101)
#define SCE_NP_MATCHING2_SIGNALING_EVENT_Established                    (0x5102)
#define SCE_NP_MATCHING2_CONTEXT_EVENT_StartOver                        (0x6f01)
#define SCE_NP_MATCHING2_CONTEXT_EVENT_Start                            (0x6f02)
#define SCE_NP_MATCHING2_CONTEXT_EVENT_Stop                             (0x6f03)

/*J イベントデータ取得キー */
typedef uint32_t SceNpMatching2EventKey;

/* リクエストコールバック */
typedef void (*SceNpMatching2RequestCallback)(
	SceNpMatching2ContextId        ctxId,
	SceNpMatching2RequestId        reqId,
	SceNpMatching2Event            event,
	SceNpMatching2EventKey         eventKey,
	int     errorCode,
	size_t  dataSize,
	void   *arg
	);

/*J ロビーイベントコールバック */
typedef void (*SceNpMatching2LobbyEventCallback)(
	SceNpMatching2ContextId         ctxId,	
	SceNpMatching2LobbyId           lobbyId,
	SceNpMatching2Event             event,
	SceNpMatching2EventKey          eventKey,
	int     errorCode,
	size_t  dataSize,
	void   *arg
	);

/*J ルームイベントコールバック */
typedef void (*SceNpMatching2RoomEventCallback)(
	SceNpMatching2ContextId         ctxId,	
	SceNpMatching2RoomId            roomId,
	SceNpMatching2Event             event,
	SceNpMatching2EventKey          eventKey,
	int     errorCode,
	size_t  dataSize,
	void   *arg
	);

/*J ロビーメッセージコールバック */
typedef void (*SceNpMatching2LobbyMessageCallback)(
	SceNpMatching2ContextId        ctxId,	
	SceNpMatching2LobbyId          lobbyId,
	SceNpMatching2LobbyMemberId    srcMemberId,
	SceNpMatching2Event            event,
	SceNpMatching2EventKey         eventKey,
	int     errorCode,
	size_t  dataSize,
	void   *arg
	);

/*J ルームメッセージコールバック */
typedef void (*SceNpMatching2RoomMessageCallback)(
	SceNpMatching2ContextId        ctxId,	
	SceNpMatching2RoomId           roomId,
	SceNpMatching2RoomMemberId     srcMemberId,
	SceNpMatching2Event            event,
	SceNpMatching2EventKey         eventKey,
	int     errorCode,
	size_t  dataSize,
	void   *arg
	);

/*J シグナリングコールバック */
typedef void (*SceNpMatching2SignalingCallback)(
	SceNpMatching2ContextId        ctxId,	
	SceNpMatching2RoomId           roomId,
	SceNpMatching2RoomMemberId     peerMemberId,
	SceNpMatching2Event            event,
	int     errorCode,
	void   *arg
	);

/*J コンテキストコールバック */
typedef void (*SceNpMatching2ContextCallback)(
    SceNpMatching2ContextId  ctxId,
    SceNpMatching2Event      event,
    SceNpMatching2EventCause eventCause,
    int   errorCode,
	void *arg
    );

/*J
 *******************************************************************************
 * API 毎のリクエスト、レスポンス構造体
 *******************************************************************************
 */

/*J 共通のオプション構造体 */
typedef struct SceNpMatching2RequestOptParam {
	SceNpMatching2RequestCallback cbFunc;
	void *cbFuncArg;
	uint32_t timeout;
	uint16_t appReqId;
	uint8_t padding[2];
} SceNpMatching2RequestOptParam;

/*
 * sceNpMatching2CreateServerContext()
 */
typedef struct SceNpMatching2CreateServerContextRequest {
	SceNpMatching2ServerId serverId;
} SceNpMatching2CreateServerContextRequest;

/*
 * sceNpMatching2DeleteServerContext()
 */
typedef struct SceNpMatching2DeleteServerContextRequest {
	SceNpMatching2ServerId serverId;
} SceNpMatching2DeleteServerContextRequest;

/*
 * sceNpMatching2GetServerInfo()
 */
typedef struct SceNpMatching2GetServerInfoRequest {
	SceNpMatching2ServerId serverId;
} SceNpMatching2GetServerInfoRequest;

typedef struct SceNpMatching2GetServerInfoResponse {
	SceNpMatching2Server server;
} SceNpMatching2GetServerInfoResponse;

/*
 * sceNpMatching2GetWorldInfoList()
 */
typedef struct SceNpMatching2GetWorldInfoListRequest {
	SceNpMatching2ServerId serverId;
} SceNpMatching2GetWorldInfoListRequest;

typedef struct SceNpMatching2GetWorldInfoListResponse {
	SceNpMatching2World *world;
	unsigned int worldNum;
} SceNpMatching2GetWorldInfoListResponse;

/*
 * sceNpMatching2SetUserInfo()
 */
typedef struct SceNpMatching2SetUserInfoRequest{
	SceNpMatching2ServerId serverId;
	uint8_t padding[2];
	SceNpMatching2BinAttr *userBinAttr;
	unsigned int userBinAttrNum;
} SceNpMatching2SetUserInfoRequest;

/*
 * sceNpMatching2GetUserInfoList()
 */
#define SCE_NP_MATCHING2_GET_USER_INFO_LIST_NPID_NUM_MAX      25
typedef struct SceNpMatching2GetUserInfoListRequest {
	SceNpMatching2ServerId serverId;
	uint8_t padding[2];
	SceNpId *npId;
	unsigned int npIdNum;
	SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
	int option;
} SceNpMatching2GetUserInfoListRequest;

typedef struct SceNpMatching2GetUserInfoListResponse {
	SceNpMatching2UserInfo *userInfo;
	unsigned int userInfoNum;
} SceNpMatching2GetUserInfoListResponse;

/*
 * sceNpMatching2GetRoomMemberDataExternalList()
 */
typedef struct SceNpMatching2GetRoomMemberDataExternalListRequest {
	SceNpMatching2RoomId roomId;
} SceNpMatching2GetRoomMemberDataExternalListRequest;

typedef struct SceNpMatching2GetRoomMemberDataExternalListResponse {
	SceNpMatching2RoomMemberDataExternal *roomMemberDataExternal;
	unsigned int roomMemberDataExternalNum;
} SceNpMatching2GetRoomMemberDataExternalListResponse;

/*
 * sceNpMatching2SetRoomDataExternal()
 */
typedef struct SceNpMatching2SetRoomDataExternalRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2IntAttr *roomSearchableIntAttrExternal;
	unsigned int roomSearchableIntAttrExternalNum;
	SceNpMatching2BinAttr *roomSearchableBinAttrExternal;
	unsigned int roomSearchableBinAttrExternalNum;
	SceNpMatching2BinAttr *roomBinAttrExternal;
	unsigned int roomBinAttrExternalNum;
} SceNpMatching2SetRoomDataExternalRequest;

/*
 * sceNpMatching2GetRoomDataExternalList()
 */
#define SCE_NP_MATCHING2_GET_ROOM_DATA_EXTERNAL_LIST_ROOM_NUM_MAX           20
typedef struct SceNpMatching2GetRoomDataExternalListRequest {
	SceNpMatching2RoomId *roomId;
	unsigned int roomIdNum;
	const SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
} SceNpMatching2GetRoomDataExternalListRequest;

typedef struct SceNpMatching2GetRoomDataExternalListResponse {
	SceNpMatching2RoomDataExternal *roomDataExternal;
	unsigned int roomDataExternalNum;
} SceNpMatching2GetRoomDataExternalListResponse;

/*
 * sceNpMatching2CreateJoinRoom()
 */

/*J ルームパスワードスロットマスクにスロット番号をを追加する */
#define SCE_NP_MATCHING2_ADD_SLOTNUM_TO_ROOM_PASSWORD_SLOT_MASK(mask, slotNumber) \
	( (mask) |= (uint64_t)(0x1ULL<<(64-(slotNumber))) )

/*J シグナリングオプションパラメータ */
typedef uint8_t SceNpMatching2SignalingType;
#define SCE_NP_MATCHING2_SIGNALING_TYPE_NONE      0
#define SCE_NP_MATCHING2_SIGNALING_TYPE_MESH      1
#define SCE_NP_MATCHING2_SIGNALING_TYPE_STAR      2
typedef uint8_t SceNpMatching2SignalingFlag;
#define SCE_NP_MATCHING2_SIGNALING_FLAG_MANUAL_MODE     0x01
typedef struct SceNpMatching2SignalingOptParam {
	SceNpMatching2SignalingType type;
	SceNpMatching2SignalingFlag flag;
	SceNpMatching2RoomMemberId hubMemberId;
	uint8_t reserved2[4];
} SceNpMatching2SignalingOptParam;

typedef struct SceNpMatching2CreateJoinRoomRequest {
	SceNpMatching2WorldId  worldId;
	uint8_t padding1[4];
	SceNpMatching2LobbyId  lobbyId;
	/*J ルーム作成パラメータ */
	unsigned int           maxSlot;
	SceNpMatching2FlagAttr flagAttr;
	SceNpMatching2BinAttr *roomBinAttrInternal;
	unsigned int           roomBinAttrInternalNum;
	SceNpMatching2IntAttr *roomSearchableIntAttrExternal;
	unsigned int           roomSearchableIntAttrExternalNum;
	SceNpMatching2BinAttr *roomSearchableBinAttrExternal;
	unsigned int           roomSearchableBinAttrExternalNum;
	SceNpMatching2BinAttr *roomBinAttrExternal;
	unsigned int           roomBinAttrExternalNum;
	SceNpMatching2SessionPassword      *roomPassword;
	SceNpMatching2RoomGroupConfig      *groupConfig;
	unsigned int                        groupConfigNum;	
	SceNpMatching2RoomPasswordSlotMask *passwordSlotMask;
	SceNpId      *allowedUser;
	unsigned int  allowedUserNum;
	SceNpId      *blockedUser;
	unsigned int  blockedUserNum;
	/*J ルーム参加パラメータ */
	SceNpMatching2GroupLabel *joinRoomGroupLabel;
	SceNpMatching2BinAttr    *roomMemberBinAttrInternal;
	unsigned int              roomMemberBinAttrInternalNum;
	SceNpMatching2TeamId      teamId;
	uint8_t padding2[3];
	/*J シグナリングパラメータ */
	SceNpMatching2SignalingOptParam *sigOptParam;
	uint8_t padding3[4];
} SceNpMatching2CreateJoinRoomRequest;

typedef struct SceNpMatching2CreateJoinRoomResponse {
	SceNpMatching2RoomDataInternal *roomDataInternal;
} SceNpMatching2CreateJoinRoomResponse;

/*
 * sceNpMatching2JoinRoom()
 */
typedef struct SceNpMatching2JoinRoomRequest {
	SceNpMatching2RoomId             roomId;
	SceNpMatching2SessionPassword   *roomPassword;
	SceNpMatching2GroupLabel        *joinRoomGroupLabel;	
	SceNpMatching2BinAttr           *roomMemberBinAttrInternal;
	unsigned int                     roomMemberBinAttrInternalNum;
	SceNpMatching2PresenceOptionData optData;
	SceNpMatching2TeamId             teamId;
	uint8_t padding[3];
} SceNpMatching2JoinRoomRequest;

/*
 * sceNpMatching2JoinProhibitiveRoom()
 */
typedef struct SceNpMatching2JoinProhibitiveRoomRequest {
	SceNpMatching2JoinRoomRequest    joinParam;
	SceNpId                         *blockedUser;
	unsigned int                     blockedUserNum;
} SceNpMatching2JoinProhibitiveRoomRequest;

typedef struct SceNpMatching2JoinRoomResponse {
	SceNpMatching2RoomDataInternal *roomDataInternal;
} SceNpMatching2JoinRoomResponse;

/*
 * sceNpMatching2LeaveRoom()
 */
typedef struct SceNpMatching2LeaveRoomRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2PresenceOptionData optData;
	uint8_t padding[4];
} SceNpMatching2LeaveRoomRequest;

/*
 * sceNpMatching2GrantRoomOwner()
 */
typedef struct SceNpMatching2GrantRoomOwnerRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2RoomMemberId newOwner;
	uint8_t padding[2];
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2GrantRoomOwnerRequest;

/*
 * sceNpMatching2KickoutRoomMember()
 */
typedef struct SceNpMatching2KickoutRoomMemberRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2RoomMemberId target;
	SceNpMatching2BlockKickFlag blockKickFlag;
	uint8_t padding[1];
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2KickoutRoomMemberRequest;

/*
 * sceNpMatching2SearchRoom()
 */
typedef struct SceNpMatching2SearchRoomRequest {
	int option;
	SceNpMatching2WorldId          worldId;	
	SceNpMatching2LobbyId          lobbyId;
	SceNpMatching2RangeFilter      rangeFilter;
	SceNpMatching2FlagAttr         flagFilter;
	SceNpMatching2FlagAttr         flagAttr;
	SceNpMatching2IntSearchFilter *intFilter;
	unsigned int                   intFilterNum;
	SceNpMatching2BinSearchFilter *binFilter;
	unsigned int                   binFilterNum;
	SceNpMatching2AttributeId     *attrId;
	unsigned int                   attrIdNum;
} SceNpMatching2SearchRoomRequest;

typedef struct SceNpMatching2SearchRoomResponse {
	SceNpMatching2Range range;
	SceNpMatching2RoomDataExternal *roomDataExternal;
} SceNpMatching2SearchRoomResponse;

/*
 * sceNpMatching2SendRoomMessage()
 */
typedef struct SceNpMatching2SendRoomMessageRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2CastType castType;
	uint8_t padding[3];
	SceNpMatching2RoomMessageDestination dst;
	const void *msg;
	size_t msgLen;
	int option;
} SceNpMatching2SendRoomMessageRequest;

/*
 * sceNpMatching2SendRoomChatMessage()
 */
typedef struct SceNpMatching2SendRoomChatMessageRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2CastType castType;
	uint8_t padding[3];
	SceNpMatching2RoomMessageDestination dst;
	const void *msg;
	size_t msgLen;
	int option;
} SceNpMatching2SendRoomChatMessageRequest;

typedef struct SceNpMatching2SendRoomChatMessageResponse {
	bool filtered;
} SceNpMatching2SendRoomChatMessageResponse;

/*
 * sceNpMatching2SetRoomDataInternal()
 */
typedef struct SceNpMatching2SetRoomDataInternalRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2FlagAttr flagFilter;
	SceNpMatching2FlagAttr flagAttr;
	SceNpMatching2BinAttr *roomBinAttrInternal;
	unsigned int           roomBinAttrInternalNum;
	SceNpMatching2RoomGroupPasswordConfig *passwordConfig;
	unsigned int passwordConfigNum;
	SceNpMatching2RoomPasswordSlotMask *passwordSlotMask;
	SceNpMatching2RoomMemberId *ownerPrivilegeRank;
	unsigned int ownerPrivilegeRankNum;
	uint8_t padding[4];
} SceNpMatching2SetRoomDataInternalRequest;

/*
 * sceNpMatching2GetRoomDataInternal()
 */
typedef struct SceNpMatching2GetRoomDataInternalRequest {
	SceNpMatching2RoomId roomId;
	const SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
} SceNpMatching2GetRoomDataInternalRequest;

typedef struct SceNpMatching2GetRoomDataInternalResponse {
	SceNpMatching2RoomDataInternal *roomDataInternal;
} SceNpMatching2GetRoomDataInternalResponse;

/*
 * sceNpMatching2SetRoomMemberDataInternal()
 */
typedef struct SceNpMatching2SetRoomMemberDataInternalRequest {
	SceNpMatching2RoomId       roomId;
	SceNpMatching2RoomMemberId memberId;
	SceNpMatching2TeamId       teamId;
	uint8_t padding[5];
	SceNpMatching2FlagAttr     flagFilter;
	SceNpMatching2FlagAttr     flagAttr;	
	SceNpMatching2BinAttr     *roomMemberBinAttrInternal;
	unsigned int               roomMemberBinAttrInternalNum;
} SceNpMatching2SetRoomMemberDataInternalRequest;

/*
 * sceNpMatching2GetRoomMemberDataInternal()
 */
typedef struct SceNpMatching2GetRoomMemberDataInternalRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2RoomMemberId memberId;
	uint8_t padding[6];
	const SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
} SceNpMatching2GetRoomMemberDataInternalRequest;

typedef struct SceNpMatching2GetRoomMemberDataInternalResponse {
	SceNpMatching2RoomMemberDataInternal *roomMemberDataInternal;
} SceNpMatching2GetRoomMemberDataInternalResponse;

/*
 * sceNpMatching2SetSignalingOptParam()
 */
typedef struct SceNpMatching2SetSignalingOptParamRequest {
	SceNpMatching2RoomId roomId;
	SceNpMatching2SignalingOptParam sigOptParam;
} SceNpMatching2SetSignalingOptParamRequest;

/*
 * sceNpMatching2GetLobbyInfoList()
 */
typedef struct SceNpMatching2GetLobbyInfoListRequest {
	SceNpMatching2WorldId      worldId;
	SceNpMatching2RangeFilter  rangeFilter;
	SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
} SceNpMatching2GetLobbyInfoListRequest;

typedef struct SceNpMatching2GetLobbyInfoListResponse {
	SceNpMatching2Range range;
	SceNpMatching2LobbyDataExternal *lobbyDataExternal;
} SceNpMatching2GetLobbyInfoListResponse;

/*
 * sceNpMatching2JoinLobby()
 */
typedef struct SceNpMatching2JoinLobbyRequest {
	SceNpMatching2LobbyId            lobbyId;
	SceNpMatching2JoinedSessionInfo *joinedSessionInfo;
	unsigned int joinedSessionInfoNum;
	SceNpMatching2BinAttr           *lobbyMemberBinAttrInternal;
	unsigned int                     lobbyMemberBinAttrInternalNum;
	SceNpMatching2PresenceOptionData optData;
	uint8_t padding[4];
} SceNpMatching2JoinLobbyRequest;

typedef struct SceNpMatching2JoinLobbyResponse {
	SceNpMatching2LobbyDataInternal *lobbyDataInternal;
} SceNpMatching2JoinLobbyResponse;

/*
 * sceNpMatching2LeaveLobby()
 */
typedef struct SceNpMatching2LeaveLobbyRequest {
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2PresenceOptionData optData;
	uint8_t padding[4];
} SceNpMatching2LeaveLobbyRequest;

/*
 * sceNpMatching2SetLobbyMemberDataInternal()
 */
typedef struct SceNpMatching2SetLobbyMemberDataInternalRequest {
	SceNpMatching2LobbyId       lobbyId;
	SceNpMatching2LobbyMemberId memberId;
	uint8_t padding1[2];
	SceNpMatching2FlagAttr      flagFilter;	/*J 未使用 */
	SceNpMatching2FlagAttr      flagAttr;	/*J 未使用 */
	SceNpMatching2JoinedSessionInfo *joinedSessionInfo;
	unsigned int joinedSessionInfoNum;
	SceNpMatching2BinAttr      *lobbyMemberBinAttrInternal;
	unsigned int                lobbyMemberBinAttrInternalNum;
	uint8_t padding2[4];
} SceNpMatching2SetLobbyMemberDataInternalRequest;

/*
 * sceNpMatching2GetLobbyMemberDataInternal()
 */
typedef struct SceNpMatching2GetLobbyMemberDataInternalRequest {
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2LobbyMemberId memberId;
	uint8_t padding[6];
	const SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
} SceNpMatching2GetLobbyMemberDataInternalRequest;

typedef struct SceNpMatching2GetLobbyMemberDataInternalResponse {
	SceNpMatching2LobbyMemberDataInternal *lobbyMemberDataInternal;
} SceNpMatching2GetLobbyMemberDataInternalResponse;

#define SCE_NP_MATCHING2_LOBBY_MEMBER_DATA_INTERNAL_LIST_MAX			256
#define SCE_NP_MATCHING2_LOBBY_MEMBER_DATA_INTERNAL_EXTENDED_DATA_LIST_MAX	64

/*
 * sceNpMatching2GetLobbyMemberDataInternalList()
 */
typedef struct SceNpMatching2GetLobbyMemberDataInternalListRequest {
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2LobbyMemberId* memberId;
	unsigned int memberIdNum;
	const SceNpMatching2AttributeId *attrId;
	unsigned int attrIdNum;
	bool extendedData;
	uint8_t padding[7];
} SceNpMatching2GetLobbyMemberDataInternalListRequest;

typedef struct SceNpMatching2GetLobbyMemberDataInternalListResponse {
	SceNpMatching2LobbyMemberDataInternal *lobbyMemberDataInternal;
	unsigned int lobbyMemberDataInternalNum;
} SceNpMatching2GetLobbyMemberDataInternalListResponse;

/*
 * sceNpMatching2SendLobbyChatMessage()
 */
typedef struct SceNpMatching2SendLobbyChatMessageRequest {
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2CastType castType;
	uint8_t padding[3];
	SceNpMatching2LobbyMessageDestination dst;
	const void *msg;
	size_t msgLen;
	int option;
} SceNpMatching2SendLobbyChatMessageRequest;

typedef struct SceNpMatching2SendLobbyChatMessageResponse {
	bool filtered;
} SceNpMatching2SendLobbyChatMessageResponse;

/*
 * sceNpMatching2SendLobbyInvitation()
 */
typedef struct SceNpMatching2SendLobbyInvitationRequest {
	SceNpMatching2LobbyId lobbyId;
	SceNpMatching2CastType castType;
	uint8_t padding[3];
	SceNpMatching2LobbyMessageDestination dst;
	SceNpMatching2InvitationData invitationData;
	int option;
} SceNpMatching2SendLobbyInvitationRequest;

/*
 * sceNpMatching2SignalingGetPingInfo()
 */
typedef struct SceNpMatching2SignalingGetPingInfoRequest {
	SceNpMatching2RoomId roomId;
	uint8_t reserved[16];
} SceNpMatching2SignalingGetPingInfoRequest;
	
typedef struct SceNpMatching2SignalingGetPingInfoResponse {
	SceNpMatching2ServerId serverId;
	uint8_t padding1[2];
	SceNpMatching2WorldId worldId;
	SceNpMatching2RoomId roomId;
	uint32_t rtt;
	uint8_t reserved[20];
} SceNpMatching2SignalingGetPingInfoResponse;

/*J
 *******************************************************************************
 * ルームイベントコールバックで通知される構造体
 *******************************************************************************
 */

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_MemberJoined
 * SCE_NP_MATCHING2_ROOM_EVENT_MemberLeft
 */
typedef struct SceNpMatching2RoomMemberUpdateInfo {
	SceNpMatching2RoomMemberDataInternal *roomMemberDataInternal;
	SceNpMatching2EventCause eventCause;
	uint8_t padding[3];
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2RoomMemberUpdateInfo;

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_RoomOwnerChanged
 */
typedef struct SceNpMatching2RoomOwnerUpdateInfo {
	SceNpMatching2RoomMemberId prevOwner;
	SceNpMatching2RoomMemberId newOwner;
	SceNpMatching2EventCause eventCause;
	uint8_t padding[3];
	SceNpMatching2SessionPassword *roomPassword;
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2RoomOwnerUpdateInfo;

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_Kickedout
 * SCE_NP_MATCHING2_ROOM_EVENT_RoomDestroyed
 */
typedef struct SceNpMatching2RoomUpdateInfo {
	SceNpMatching2EventCause eventCause;
	uint8_t padding[3];
	int errorCode;
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2RoomUpdateInfo;

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_UpdatedRoomDataInternal
 */
typedef struct SceNpMatching2RoomDataInternalUpdateInfo {
	SceNpMatching2RoomDataInternal      *newRoomDataInternal;
	SceNpMatching2FlagAttr              *newFlagAttr;
	SceNpMatching2FlagAttr              *prevFlagAttr;
	SceNpMatching2RoomPasswordSlotMask  *newRoomPasswordSlotMask;
	SceNpMatching2RoomPasswordSlotMask  *prevRoomPasswordSlotMask;
	SceNpMatching2RoomGroup            **newRoomGroup;
	unsigned int                         newRoomGroupNum;
	SceNpMatching2RoomBinAttrInternal  **newRoomBinAttrInternal;
	unsigned int                         newRoomBinAttrInternalNum;
} SceNpMatching2RoomDataInternalUpdateInfo;

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_UpdatedRoomMemberDataInternal
 */
typedef struct SceNpMatching2RoomMemberDataInternalUpdateInfo {
	SceNpMatching2RoomMemberDataInternal     *newRoomMemberDataInternal;
	SceNpMatching2FlagAttr                   *newFlagAttr;
	SceNpMatching2FlagAttr                   *prevFlagAttr;
	SceNpMatching2TeamId                     *newTeamId;
	SceNpMatching2RoomMemberBinAttrInternal **newRoomMemberBinAttrInternal;
	unsigned int                              newRoomMemberBinAttrInternalNum;
} SceNpMatching2RoomMemberDataInternalUpdateInfo;

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_UpdatedSignalingOptParam
 */
typedef struct SceNpMatching2SignalingOptParamUpdateInfo {
	SceNpMatching2SignalingOptParam newSignalingOptParam;
} SceNpMatching2SignalingOptParamUpdateInfo;

/*J
 *******************************************************************************
 * ルームメッセージコールバックで通知される構造体
 *******************************************************************************
 */

/*
 * SCE_NP_MATCHING2_ROOM_EVENT_ChatMessage
 * SCE_NP_MATCHING2_ROOM_EVENT_Message
 */
typedef struct SceNpMatching2RoomMessageInfo {
	bool filtered;
	SceNpMatching2CastType castType;
	uint8_t padding[2];
	SceNpMatching2RoomMessageDestination *dst;
	SceNpUserInfo2 *srcMember;
	const void *msg;
	size_t msgLen;	
} SceNpMatching2RoomMessageInfo;

/*J
 *******************************************************************************
 * ロビーイベントコールバックで通知される構造体
 *******************************************************************************
 */

/*
 * SCE_NP_MATCHING2_LOBBY_EVENT_MemberJoined
 * SCE_NP_MATCHING2_LOBBY_EVENT_MemberLeft
 */
typedef struct SceNpMatching2LobbyMemberUpdateInfo {
	SceNpMatching2LobbyMemberDataInternal *lobbyMemberDataInternal;
	SceNpMatching2EventCause eventCause;
	uint8_t padding[3];
	SceNpMatching2PresenceOptionData optData;
} SceNpMatching2LobbyMemberUpdateInfo;

/*
 * SCE_NP_MATCHING2_LOBBY_EVENT_LobbyDestroyed
 */
typedef struct SceNpMatching2LobbyUpdateInfo {
	SceNpMatching2EventCause eventCause;
	uint8_t padding[3];
	int errorCode;
} SceNpMatching2LobbyUpdateInfo;

/*
 * SCE_NP_MATCHING2_LOBBY_EVENT_UpdatedLobbyMemberDataInternal
 */
typedef struct SceNpMatching2LobbyMemberDataInternalUpdateInfo {
	SceNpMatching2LobbyMemberId memberId;
	uint8_t padding[2];
	SceNpId npId;
	SceNpMatching2FlagAttr                    flagFilter;
	SceNpMatching2FlagAttr                    newFlagAttr;
	SceNpMatching2JoinedSessionInfo          *newJoinedSessionInfo;
	unsigned int                              newJoinedSessionInfoNum;
	SceNpMatching2LobbyMemberBinAttrInternal *newLobbyMemberBinAttrInternal;
	unsigned int                              newLobbyMemberBinAttrInternalNum;
} SceNpMatching2LobbyMemberDataInternalUpdateInfo;

/*J
 *******************************************************************************
 * ロビーメッセージコールバックで通知される構造体
 *******************************************************************************
 */

/*
 * SCE_NP_MATCHING2_LOBBY_EVENT_ChatMessage
 */
typedef struct SceNpMatching2LobbyMessageInfo {
	bool filtered;
	SceNpMatching2CastType castType;
	uint8_t padding[2];
	SceNpMatching2LobbyMessageDestination *dst;
	SceNpUserInfo2 *srcMember;
	const void *msg;
	size_t msgLen;
} SceNpMatching2LobbyMessageInfo;

/*
 * SCE_NP_MATCHING2_LOBBY_EVENT_Invitation
 */
typedef struct SceNpMatching2LobbyInvitationInfo {
	SceNpMatching2CastType castType;
	uint8_t padding[3];
	SceNpMatching2LobbyMessageDestination *dst;
	SceNpUserInfo2 *srcMember;
	SceNpMatching2InvitationData invitationData;
} SceNpMatching2LobbyInvitationInfo;

/*J
 *******************************************************************************
 * API 宣言
 *******************************************************************************
 */
int
sceNpMatching2Init(
	size_t stackSize,
	int priority
	);

int
sceNpMatching2Term(
	void
	);

int
sceNpMatching2Init2(
	size_t stackSize,
	int priority,
	SceNpMatching2UtilityInitParam *param
	);

int
sceNpMatching2Term2(
	void
	);

int
sceNpMatching2GetEventData(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2EventKey  eventKey,
	void   *buf,
	size_t  bufLen
	);

int
sceNpMatching2ClearEventData(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2EventKey  eventKey
	);

int
sceNpMatching2CreateContext(
	const SceNpId *npId,
	const SceNpCommunicationId *commId,
	const SceNpCommunicationPassphrase *passPhrase,
	SceNpMatching2ContextId *ctxId,
	int option
	);

int
sceNpMatching2DestroyContext(
	SceNpMatching2ContextId ctxId
	);

int
sceNpMatching2ContextStart(
	SceNpMatching2ContextId ctxId
	);

int
sceNpMatching2ContextStartAsync(
	SceNpMatching2ContextId ctxId,
	uint32_t timeout
	);

int
sceNpMatching2AbortContextStart(
	SceNpMatching2ContextId ctxId
	);

int
sceNpMatching2ContextStop(
	SceNpMatching2ContextId ctxId
	);

int
sceNpMatching2SetDefaultRequestOptParam(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2RequestOptParam *optParam
	);

int
sceNpMatching2RegisterRoomEventCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomEventCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2RegisterRoomMessageCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomMessageCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2RegisterLobbyEventCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2LobbyEventCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2RegisterLobbyMessageCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2LobbyMessageCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2RegisterSignalingCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2SignalingCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2RegisterContextCallback(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2ContextCallback cbFunc,
	void *cbFuncArg
	);

int
sceNpMatching2GetClanLobbyId(
	SceNpMatching2ContextId ctxId,
	SceNpClanId clanId,
	SceNpMatching2LobbyId *lobbyId
	);

int
sceNpMatching2AbortRequest(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RequestId reqId
	);

int
sceNpMatching2GetMemoryInfo(
	SceNpMatching2MemoryInfo *memInfo
	);

int
sceNpMatching2GetCbQueueInfo(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2CbQueueInfo *queueInfo
	);

int
sceNpMatching2GetServerIdListLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2ServerId *serverId,
	uint32_t serverIdNum
	);

int
sceNpMatching2GetServerInfo(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetServerInfoRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2CreateServerContext(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2CreateServerContextRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2DeleteServerContext(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2DeleteServerContextRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetWorldInfoList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetWorldInfoListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetUserInfo(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetUserInfoRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetUserInfoList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetUserInfoListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2CreateJoinRoom(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2CreateJoinRoomRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SearchRoom(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SearchRoomRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2JoinRoom(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2JoinRoomRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2JoinProhibitiveRoom(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2JoinProhibitiveRoomRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2LeaveRoom(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2LeaveRoomRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetRoomDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetRoomDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetRoomDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetRoomDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetSignalingOptParam(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetSignalingOptParamRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetRoomPasswordLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	bool *withPassword,
	SceNpMatching2SessionPassword *roomPassword
	);

int
sceNpMatching2GetLobbyMemberIdListLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2LobbyId lobbyId,
	SceNpMatching2LobbyMemberId *memberId,
	uint32_t memberIdNum,
	SceNpMatching2LobbyMemberId *me
	);

int
sceNpMatching2GetSignalingOptParamLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	SceNpMatching2SignalingOptParam *signalingOptParam
	);

int
sceNpMatching2GetRoomDataExternalList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetRoomDataExternalListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetRoomDataExternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetRoomDataExternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetRoomMemberDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetRoomMemberDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetRoomMemberDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetRoomMemberDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetRoomMemberIdListLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	int sortMethod,
	SceNpMatching2RoomMemberId *memberId,
	uint32_t memberIdNum
	);

int
sceNpMatching2GetRoomMemberDataInternalLocal(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	SceNpMatching2RoomMemberId memberId,
	SceNpMatching2AttributeId *attrId,
	uint32_t attrIdNum,
	SceNpMatching2RoomMemberDataInternal *member,
	char   *buf,
	size_t  bufLen
	);

int
sceNpMatching2GetRoomMemberDataExternalList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetRoomMemberDataExternalListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2KickoutRoomMember(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2KickoutRoomMemberRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GrantRoomOwner(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GrantRoomOwnerRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SendRoomChatMessage(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SendRoomChatMessageRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SendRoomMessage(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SendRoomMessageRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetLobbyInfoList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetLobbyInfoListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2JoinLobby(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2JoinLobbyRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2LeaveLobby(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2LeaveLobbyRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SetLobbyMemberDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SetLobbyMemberDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetLobbyMemberDataInternal(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetLobbyMemberDataInternalRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2GetLobbyMemberDataInternalList(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2GetLobbyMemberDataInternalListRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SendLobbyChatMessage(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SendLobbyChatMessageRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SendLobbyInvitation(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SendLobbyInvitationRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SignalingGetPingInfo(
	SceNpMatching2ContextId ctxId,
	const SceNpMatching2SignalingGetPingInfoRequest *reqParam,
	const SceNpMatching2RequestOptParam *optParam,
	SceNpMatching2RequestId *assignedReqId
	);

int
sceNpMatching2SignalingGetConnectionStatus(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	SceNpMatching2RoomMemberId memberId,
	int *connStatus,
	struct in_addr *peerAddr,
	in_port_t *peerPort
	);

int
sceNpMatching2SignalingGetConnectionInfo(
	SceNpMatching2ContextId ctxId,
	SceNpMatching2RoomId roomId,
	SceNpMatching2RoomMemberId memberId,
	int code,
	SceNpSignalingConnectionInfo *connInfo
	);

int
sceNpMatching2SignalingSetCtxOpt(
	SceNpMatching2ContextId ctxId,
	int optname,
	int optval
	);

int
sceNpMatching2SignalingGetCtxOpt(
	SceNpMatching2ContextId ctxId,
	int optname,
	int *optval
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SCE_NP_MATCHING2_H__ */
