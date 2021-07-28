/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETINET_ICMP_H__)
#define __SYS_NET_NETINET_ICMP_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct icmp_hdr {
	unsigned char icmp_type;
	unsigned char icmp_code;
	unsigned short icmp_cksum;
	union {
		struct echo {
			unsigned short id;
			unsigned short sequence;
		} echo;
		unsigned int gateway;
		struct frag {
			unsigned short unused;
			unsigned short mtu;
		} frag;
	} un;
#define icmp_id	un.echo.id
#define icmp_seq	un.echo.sequence
#define icmp_gwaddr	gateway
#define icmp_unused	un.frag.unused
#define icmp_nextmtu	un.frag.mtu
} ICMP_HDR;

#define ICMP_TYPE_ECHO_REPLY		0
#define ICMP_TYPE_DEST_UNREACH		3
#define	ICMP_TYPE_SOURCE_QUENCH		4
#define	ICMP_TYPE_REDIRECT		5
#define ICMP_TYPE_ECHO_REQUEST		8
#define ICMP_TYPE_TIME_EXCEEDED		11
#define	ICMP_TYPE_PARAMETER_PROBLEM	12
#define	ICMP_TYPE_TIMESTAMP_REQUEST	13
#define	ICMP_TYPE_TIMESTAMP_REPLY	14
#define	ICMP_TYPE_INFORMATION_REQUEST	15
#define	ICMP_TYPE_INFORMATION_REPLY	16
#define	ICMP_TYPE_ADDRESS_MASK_REQUEST	17
#define	ICMP_TYPE_ADDRESS_MASK_REPLY	18

/* DEST_UNREACH */
#define ICMP_CODE_NET_UNREACH		0
#define ICMP_CODE_HOST_UNREACH		1
#define ICMP_CODE_PROTO_UNREACH		2
#define ICMP_CODE_PORT_UNREACH		3
#define ICMP_CODE_FRAG_AND_DF		4
#define ICMP_CODE_SRC_HOST_FAILED	5
#define ICMP_CODE_DST_NET_UNKNOWN	6
#define ICMP_CODE_DST_HOST_UNKNOWN	7
#define ICMP_CODE_SRC_HOST_ISOLATED	8
#define ICMP_CODE_NET_ADMIN_PROHIBITED	9
#define ICMP_CODE_NET_HOST_PROHIBITED	10
#define ICMP_CODE_NET_TOS		11
#define ICMP_CODE_HOST_TOS		12

/* TIME_EXCEEDED */
#define ICMP_CODE_TTL_EXCEEDED		0
#define ICMP_CODE_FRT_EXCEEDED		1

#ifdef __cplusplus
}
#endif

#endif /* !__SYS_NET_NETINET_ICMP_H__ */

