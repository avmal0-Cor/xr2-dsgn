/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef __SCE_NP_COMMON_H__
#define __SCE_NP_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define	SCE_NET_NP_ONLINEID_MIN_LENGTH		3
#define	SCE_NET_NP_ONLINEID_MAX_LENGTH		16
#define	SCE_NET_NP_ONLINENAME_MAX_LENGTH	48
#define	SCE_NET_NP_AVATAR_URL_MAX_LENGTH	127
#define	SCE_NET_NP_ABOUT_ME_MAX_LENGTH		63
#define SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE_LARGE	(200 * 1024U)
#define SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE_MIDDLE	(100 * 1024U)
#define SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE_SMALL	(10 * 1024U)
#define SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE	SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE_LARGE
#define SCE_NET_NP_TSS_MAX_SIZE				(64  * 1024U)

#define SCE_NP_COMMUNICATION_SIGNATURE_SIZE			(160)
#define SCE_NET_NP_COMMUNICATION_PASSPHRASE_SIZE	(128)
#define SCE_NP_COMMUNICATION_PASSPHRASE_SIZE		SCE_NET_NP_COMMUNICATION_PASSPHRASE_SIZE

#define	SCE_NET_NP_PSHANDLE_MIN_LENGTH		SCE_NET_NP_ONLINEID_MIN_LENGTH
#define	SCE_NET_NP_PSHANDLE_MAX_LENGTH		SCE_NET_NP_ONLINEID_MAX_LENGTH
#define	SCE_NET_NP_SUBHANDLE_MAX_LENGTH		SCE_NET_NP_ONLINENAME_MAX_LENGTH
#define	SCE_NET_NP_ICON_URL_MAX_LENGTH		SCE_NET_NP_AVATAR_URL_MAX_LENGTH

#define SCE_NP_UTIL_NPID_VERSION			1
#define SCE_NP_UTIL_NPLOBBYID_VERSION		1
#define SCE_NP_UTIL_NPROOMID_VERSION		1

/* SCE_NP_LANG_XXX and CELL_SYSUTIL_LANG_XXX are same value */
#define SCE_NP_LANG_JAPANESE	(0)		/*J 日本語 */
#define SCE_NP_LANG_ENGLISH		(1)		/*J 英語 */
#define SCE_NP_LANG_FRENCH		(2)		/*J フランス語 */
#define SCE_NP_LANG_SPANISH		(3)		/*J スペイン語 */
#define SCE_NP_LANG_GERMAN		(4)		/*J ドイツ語 */
#define SCE_NP_LANG_ITALIAN		(5)		/*J イタリア語 */
#define SCE_NP_LANG_DUTCH		(6)		/*J オランダ語 */
#define SCE_NP_LANG_PORTUGUESE	(7)		/*J ポルトガル語 */
#define SCE_NP_LANG_RUSSIAN		(8)		/*J ロシア語 */
#define SCE_NP_LANG_KOREAN		(9)		/*J 韓国語 */
#define SCE_NP_LANG_CHINESE_T	(10)	/*J 中国語(繁体字) */
#define SCE_NP_LANG_CHINESE_S	(11)	/*J 中国語(簡体字) */
#define SCE_NP_LANG_FINNISH		(12)	/*J フィンランド語 */
#define SCE_NP_LANG_SWEDISH 	(13)	/*J スウェーデン語 */
#define SCE_NP_LANG_DANISH 		(14)	/*J デンマーク語 */
#define SCE_NP_LANG_NORWEGIAN  	(15)	/*J ノルウェー語 */
#define SCE_NP_LANG_POLISH		(16)	/*J ポーランド語 */

#define SCE_NP_FRIEND_MAX_NUM	(100)

/* Communication ID */
typedef struct SceNpCommunicationId {
	char data[9];
	char term;
	uint8_t num;
	char dummy;
} SceNpCommunicationId;
typedef SceNpCommunicationId SceNpTitleId;

/* Communication Passphrase */
typedef struct SceNpCommunicationPassphrase {
	uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_SIZE];
} SceNpCommunicationPassphrase;

/* Communication Signature */
typedef struct SceNpCommunicationSignature {
	uint8_t data[SCE_NP_COMMUNICATION_SIGNATURE_SIZE];
} SceNpCommunicationSignature;

/* Online ID */
typedef struct SceNpOnlineId {
	char data[SCE_NET_NP_ONLINEID_MAX_LENGTH];
	char term;
	char dummy[3];
} SceNpOnlineId;
typedef struct SceNpOnlineId SceNpPsHandle; 

/* NP ID */
typedef struct SceNpId {
	SceNpOnlineId handle;
	uint8_t opt[8];
	uint8_t reserved[8];
} SceNpId;

/* NP Online Name */
typedef struct SceNpOnlineName {
	char data[SCE_NET_NP_ONLINENAME_MAX_LENGTH];
	char term;
	char padding[3];
} SceNpOnlineName;
typedef struct SceNpOnlineName SceNpSubHandle;

/* NP Avatar URL */
typedef struct SceNpAvatarUrl {
	char data[SCE_NET_NP_AVATAR_URL_MAX_LENGTH];
	char term;
} SceNpAvatarUrl;
typedef struct SceNpAvatarUrl SceNpUserIcon;

/* NP USER INFO */
typedef struct SceNpUserInfo {
	SceNpId userId;
	SceNpOnlineName name;		/* Online Name */
	SceNpAvatarUrl icon;		/* Avatar URL */
} SceNpUserInfo;

/* NP USER INFO 2 */
typedef struct SceNpUserInfo2 {
	SceNpId npId;
	SceNpOnlineName *onlineName;	/* (pointer) Online Name */
	SceNpAvatarUrl  *avatarUrl;		/* (pointer) Avatar URL */
} SceNpUserInfo2;

/* LOBBY ID */
typedef struct SceNpLobbyId {
	uint8_t opt[28];
	uint8_t reserved[8];
} SceNpLobbyId;

/* ROOM ID */
typedef struct SceNpRoomId {
	uint8_t opt[28];
	uint8_t reserved[8];
} SceNpRoomId;

/* My Languages */
typedef	struct SceNpMyLanguages{
	int32_t	language1;	/* It will always be set */
	int32_t	language2;	/* Negative when not set */
	int32_t	language3;	/* Negative when not set */
	uint8_t padding[4];
} SceNpMyLanguages;

/* NP Avatar Image(for Large Size) */
typedef struct SceNpAvatarImage {
	uint8_t data[SCE_NET_NP_AVATAR_IMAGE_MAX_SIZE];
	size_t	size;
	uint8_t	reserved[12];
} SceNpAvatarImage;

typedef enum {
	SCE_NP_AVATAR_SIZE_LARGE,
	SCE_NP_AVATAR_SIZE_MIDDLE,
	SCE_NP_AVATAR_SIZE_SMALL
} SceNpAvatarSizeType;

typedef	struct SceNpAboutMe{
	char	data[SCE_NET_NP_ABOUT_ME_MAX_LENGTH];
	char	term;
} SceNpAboutMe;

typedef struct SceNpManagerCacheParam {
	size_t size;
	struct SceNpOnlineId onlineId;
	struct SceNpId npId;
	struct SceNpOnlineName onlineName;
	struct SceNpAvatarUrl avatarUrl;
} SceNpManagerCacheParam;


#define SCE_NP_PORT	3658	/* registered port for NP */
#define SCE_NP_MIN_POOL_SIZE	(128*1024U)

int
sceNpInit(
	size_t poolsize,
	void *poolptr
	);

int
sceNpTerm(
	void
	);

int
sceNp2Init(
	size_t poolsize,
	void *poolptr
	);

int
sceNp2Term(
	void
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SCE_NP_COMMON_H__ */
