/*  SCE CONFIDENTIAL                                      */
/*  PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*  Copyright (C) 2007 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                                  */
#ifndef _SCE_NP_SIGNALING_H_
#define _SCE_NP_SIGNALING_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/*
 * Np Signaling Context
 */
#define SCE_NP_SIGNALING_CTX_MAX						8

/*
 * Np Signaling Event
 */
#define SCE_NP_SIGNALING_EVENT_DEAD						0
#define SCE_NP_SIGNALING_EVENT_ESTABLISHED				1
#define SCE_NP_SIGNALING_EVENT_NETINFO_ERROR			2
#define SCE_NP_SIGNALING_EVENT_NETINFO_RESULT			3

/*
 * Np Signaling Event for Extended Handnler Only
 */
#define SCE_NP_SIGNALING_EVENT_EXT_PEER_ACTIVATED		10
#define SCE_NP_SIGNALING_EVENT_EXT_PEER_DEACTIVATED		11
#define SCE_NP_SIGNALING_EVENT_EXT_MUTUAL_ACTIVATED		12
/* obsolete */
#define SCE_NP_SIGNALING_EVENT_EXT_ACTIVATED			10

/*
 * Np Connection Status
 */
#define SCE_NP_SIGNALING_CONN_STATUS_INACTIVE			0
#define SCE_NP_SIGNALING_CONN_STATUS_PENDING			1
#define SCE_NP_SIGNALING_CONN_STATUS_ACTIVE				2

/*
 * Np Connection Info
 */
#define SCE_NP_SIGNALING_CONN_INFO_RTT					1
#define SCE_NP_SIGNALING_CONN_INFO_BANDWIDTH			2
#define SCE_NP_SIGNALING_CONN_INFO_PEER_NPID			3
#define SCE_NP_SIGNALING_CONN_INFO_PEER_ADDRESS			4
#define SCE_NP_SIGNALING_CONN_INFO_MAPPED_ADDRESS		5
#define SCE_NP_SIGNALING_CONN_INFO_PACKET_LOSS			6

/*
 * Np Signaling Context Option
 */
#define SCE_NP_SIGNALING_CTX_OPT_BANDWIDTH_PROBE		1

#define SCE_NP_SIGNALING_CTX_OPT_BANDWIDTH_PROBE_ENABLE		1
#define SCE_NP_SIGNALING_CTX_OPT_BANDWIDTH_PROBE_DISABLE	0

typedef union SceNpSignalingConnectionInfo {
	uint32_t rtt;
	uint32_t bandwidth;
	SceNpId npId;
	struct {
		struct in_addr addr;
		in_port_t port;
	} address;
	uint32_t packet_loss;
} SceNpSignalingConnectionInfo;

/*
 * Np Signaling Handler
 */
typedef void (*SceNpSignalingHandler)(
	uint32_t ctx_id,
	uint32_t subject_id,
	int event,
	int error_code,
	void *arg
	);

static inline int sceNpSignalingInit(void) {return(0);}
static inline int sceNpSignalingTerm(void) {return(0);}

/*
 * Np Signaling API
 */
int
sceNpSignalingCreateCtx(
	const SceNpId *npId,
	SceNpSignalingHandler handler,
	void *arg,
	uint32_t *ctx_id
	);

int
sceNpSignalingDestroyCtx(
	uint32_t ctx_id
	);

int
sceNpSignalingActivateConnection(
	uint32_t ctx_id,
	const SceNpId *npId,
	uint32_t *conn_id
	);

int
sceNpSignalingDeactivateConnection(
	uint32_t ctx_id,
	uint32_t conn_id
	);

int
sceNpSignalingTerminateConnection(
	uint32_t ctx_id,
	uint32_t conn_id
	);

int
sceNpSignalingGetConnectionStatus(
	uint32_t ctx_id,
	uint32_t conn_id,
	int *conn_status,
	struct in_addr *peer_addr,
	in_port_t *peer_port
	);

int
sceNpSignalingGetConnectionInfo(
	uint32_t ctx_id,
	uint32_t conn_id,
	int code,
	SceNpSignalingConnectionInfo *info
	);

int
sceNpSignalingAddExtendedHandler(
	uint32_t ctx_id,
	SceNpSignalingHandler handler,
	void *arg
	);

int
sceNpSignalingGetConnectionFromNpId(
	uint32_t ctx_id,
	const SceNpId *npId,
	uint32_t *conn_id
	);

int
sceNpSignalingGetConnectionFromPeerAddress(
	uint32_t ctx_id,
	struct in_addr peer_addr,
	in_port_t peer_port,
	uint32_t *conn_id
	);

int
sceNpSignalingSetCtxOpt(
	uint32_t ctx_id,
	int optname,
	int optval
	);

int
sceNpSignalingGetCtxOpt(
	uint32_t ctx_id,
	int optname,
	int *optval
	);

/*
 * Np Signaling NetInfo
 */

typedef struct SceNpSignalingNetInfo {
	size_t size;
	struct in_addr local_addr;
	struct in_addr mapped_addr;
	int nat_status;
	int upnp_status;
	int npport_status;
	uint16_t npport;
} SceNpSignalingNetInfo;

/* nat staus */
#define SCE_NP_SIGNALING_NETINFO_NAT_STATUS_UNKNOWN		0
#define SCE_NP_SIGNALING_NETINFO_NAT_STATUS_TYPE1		1
#define SCE_NP_SIGNALING_NETINFO_NAT_STATUS_TYPE2		2
#define SCE_NP_SIGNALING_NETINFO_NAT_STATUS_TYPE3		3
/* upnp status */
#define SCE_NP_SIGNALING_NETINFO_UPNP_STATUS_UNKNOWN	0
#define SCE_NP_SIGNALING_NETINFO_UPNP_STATUS_INVALID	1
#define SCE_NP_SIGNALING_NETINFO_UPNP_STATUS_VALID		2
/* npport status */
#define SCE_NP_SIGNALING_NETINFO_NPPORT_STATUS_CLOSED	0
#define SCE_NP_SIGNALING_NETINFO_NPPORT_STATUS_OPEN		1

/*
 * Np Signaling NetInfo API
 */

int
sceNpSignalingGetLocalNetInfo(
	uint32_t ctx_id,
	SceNpSignalingNetInfo *netinfo
	);

int
sceNpSignalingGetPeerNetInfo(
	uint32_t ctx_id,
	const SceNpId *npId,
	uint32_t *req_id
	);

int
sceNpSignalingCancelPeerNetInfo(
	uint32_t ctx_id,
	uint32_t req_id
	);

int
sceNpSignalingGetPeerNetInfoResult(
	uint32_t ctx_id,
	uint32_t req_id,
	SceNpSignalingNetInfo *netinfo
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCE_NP_SIGNALING_H_ */
