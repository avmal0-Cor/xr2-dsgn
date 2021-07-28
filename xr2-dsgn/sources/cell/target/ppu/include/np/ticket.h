/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2007 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_TICKET_H_
#define _SCE_NP_TICKET_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/*E
 * ----------------------------------------------------------------
 * NP Ticket and Ticket parameter sizes
 * ----------------------------------------------------------------
 */
/*E maximum size of NP Ticket */
#define SCE_NP_TICKET_MAX_SIZE			(65536)

/*E size of NP Ticket Serial ID */
#define SCE_NP_TICKET_SERIAL_ID_SIZE	(20)

/*E size of Subject Region */
#define SCE_NP_SUBJECT_REGION_SIZE		(4)

/*E size of Subject Domain */
#define SCE_NP_SUBJECT_DOMAIN_SIZE		(4)

/*E size of Service ID */
#define SCE_NP_SERVICE_ID_SIZE			(24)

/*E size of Entitlement ID */
#define SCE_NP_ENTITLEMENT_ID_SIZE		(32)

/*E maximum size of Cookie Information */
#define SCE_NP_COOKIE_MAX_SIZE			(1024)

/*E NP Ticket version */
typedef struct SceNpTicketVersion {
	uint16_t major;
	uint16_t minor;
} SceNpTicketVersion;

int
sceNpManagerRequestTicket(
	const SceNpId *npId,
	const char *serviceId,
	const void *cookie,
	size_t cookieSize,
	const char *entitlementId,
	unsigned int consumedCount
	);

int
sceNpManagerRequestTicket2(
	const SceNpId *npId,
	const SceNpTicketVersion *version,
	const char *serviceId,
	const void *cookie,
	size_t cookieSize,
	const char *entitlementId,
	unsigned int consumedCount
	);

int
sceNpManagerGetTicket(
	void *buffer,
	size_t *bufferSize
	);

/*E
 * ----------------------------------------------------------------
 * NP Ticket parameter types and functions
 * ----------------------------------------------------------------
 */
#define SCE_NP_TICKET_PARAM_DATA_LEN	(256)

typedef int64_t SceNpTime;	/*E time in milliseconds */

typedef struct SceNpDate {
	uint16_t year;
	uint8_t month;
	uint8_t day;
} SceNpDate;

union SceNpTicketParam {
	int32_t		i32;
	int64_t		i64;
	uint32_t	u32;
	uint64_t	u64;
	SceNpDate	date;
	uint8_t		data[SCE_NP_TICKET_PARAM_DATA_LEN];
};

typedef enum {
	SCE_NP_TICKET_PARAM_SERIAL_ID = 0,       /* Binary */
	SCE_NP_TICKET_PARAM_ISSUER_ID,           /* uint32 */
	SCE_NP_TICKET_PARAM_ISSUED_DATE,         /* SceNpTime */
	SCE_NP_TICKET_PARAM_EXPIRE_DATE,         /* SceNpTime */
	SCE_NP_TICKET_PARAM_SUBJECT_ACCOUNT_ID,  /* uint64 */
	SCE_NP_TICKET_PARAM_SUBJECT_ONLINE_ID,   /* String */
	SCE_NP_TICKET_PARAM_SUBJECT_REGION,      /* Binary */
	SCE_NP_TICKET_PARAM_SUBJECT_DOMAIN,      /* String */
	SCE_NP_TICKET_PARAM_SERVICE_ID,          /* String */
	SCE_NP_TICKET_PARAM_SUBJECT_STATUS,      /* uint32 */
	SCE_NP_TICKET_PARAM_STATUS_DURATION,     /* SceNpTime */
	SCE_NP_TICKET_PARAM_SUBJECT_DOB,         /* SceNpDate */

	SCE_NP_TICKET_PARAM_MAX
} SceNpTicketParamId;

int
sceNpManagerGetTicketParam(
	int paramId,
	union SceNpTicketParam *param
	);

#define SCE_NP_SUBJECT_ONLINE_ID_GET_ONLINE_ID(raw, onlineid) \
	memcpy((onlineid)->data, (raw), SCE_NET_NP_ONLINEID_MAX_LENGTH); \
	(onlineid)->term = '\0'

#define SCE_NP_SUBJECT_STATUS_GET_AGE(u32)			(((u32)>>24)&0x7f)
#define SCE_NP_SUBJECT_STATUS_IS_SUSPENDED(u32)		((u32)&0x80)
#define SCE_NP_SUBJECT_STATUS_IS_CHAT_DISABLED(u32)	((u32)&0x100)
#define SCE_NP_SUBJECT_STATUS_CONTENT_RATING(u32)	((u32)&0x200)

#define SCE_NP_SUBJECT_REGION_GET_COUNTRY_CODE(raw, cc) \
	(cc)->data[0] = *((char *)(raw)); \
	(cc)->data[1] = *((char *)(raw)+1); \
	(cc)->term = '\0'
#define SCE_NP_SUBJECT_REGION_GET_LANGUAGE_CODE(raw) \
	(*((uint8_t *)(raw)+3))


/*E NP country code (ISO 3166-1 two-letter system) */
typedef struct SceNpCountryCode {
	char data[2];
	char term;
	char padding[1];
} SceNpCountryCode;


/*E
 * ----------------------------------------------------------------
 * NP Entitlement types and functions
 * ----------------------------------------------------------------
 */
typedef struct SceNpEntitlementId {
	unsigned char data[SCE_NP_ENTITLEMENT_ID_SIZE];
} SceNpEntitlementId;

typedef enum {
	SCE_NP_ENTITLEMENT_TYPE_NON_CONSUMABLE = 0,
	SCE_NP_ENTITLEMENT_TYPE_CONSUMABLE
} SceNpEntitlementType;

typedef struct SceNpEntitlement {
	SceNpEntitlementId	id;
	SceNpTime	created_date;
	SceNpTime	expire_date;
	uint32_t	type;
	int32_t		remaining_count;  /*E may be negative */
	uint32_t	consumed_count;
	uint8_t		padding[4];
} SceNpEntitlement;

int
sceNpManagerGetEntitlementIdList(
	SceNpEntitlementId *entIdList,
	size_t entIdListNum
	);

int 
sceNpManagerGetEntitlementById(
	const char *entId,
	SceNpEntitlement *ent
	);

#define sceNpManagerCheckEntitlementById(entId) \
	sceNpManagerGetEntitlementById((entId), NULL)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_TICKET_H_ */
