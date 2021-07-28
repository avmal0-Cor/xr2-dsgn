/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETINET_IN_H__)
#define __SYS_NET_NETINET_IN_H__

#include <sys/ansi.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define IPPROTO_IP	0
#define	IPPROTO_ICMP	1
#define IPPROTO_IGMP	2
#define	IPPROTO_TCP	6
#define	IPPROTO_UDP	17
#define IPPROTO_ICMPV6	58

struct in_addr {
	in_addr_t s_addr;
};

#define	IN_CLASSA(i)		(((unsigned int)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((unsigned int)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(i)		(((unsigned int)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((unsigned int)(i) & 0xf0000000) == 0xe0000000)
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	INADDR_ANY		(unsigned int)0x00000000
#define	INADDR_LOOPBACK		(unsigned int)0x7f000001
#define	INADDR_BROADCAST	(unsigned int)0xffffffff
#define	INADDR_NONE		0xffffffff

#define	IN_LOOPBACKNET		127

struct sockaddr_in {
	unsigned char sin_len;
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
};

struct sockaddr_in_p2p {
	unsigned char sin_len;
	sa_family_t	sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	in_port_t sin_vport;
	char sin_zero[6];
};

#define INET_ADDRSTRLEN		16

#define IP_HDRINCL			2
#define IP_TOS				3
#define IP_TTL				4
#define IP_MULTICAST_IF			9
#define IP_MULTICAST_TTL		10
#define IP_MULTICAST_LOOP		11
#define IP_ADD_MEMBERSHIP		12
#define IP_DROP_MEMBERSHIP		13
#define IP_TTLCHK			23
#define IP_MAXTTL			24

#define IP_DEFAULT_MULTICAST_TTL	1
#define IP_DEFAULT_MULTICAST_LOOP	1

struct ip_mreq {
	struct in_addr imr_multiaddr;
	struct in_addr imr_interface;
};

#define htonl(x)		(x)
#define htons(x)		(x)
#define ntohl(x)		(x)
#define ntohs(x)		(x)

#ifdef __cplusplus
}
#endif

#endif	/* !__SYS_NET_NETINET_IN_H__ */

