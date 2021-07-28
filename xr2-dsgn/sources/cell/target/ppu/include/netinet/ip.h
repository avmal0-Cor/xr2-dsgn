/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETINET_IP_H__)
#define __SYS_NET_NETINET_IP_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct ip_hdr {
	union {
		struct ip_ver_hl {
			unsigned char ver:4;
			unsigned char hl:4;
		} ip_ver_hl;
		unsigned char ver_hl;
	} un;
	unsigned char ip_tos;
	unsigned short ip_len;
	unsigned short ip_id;
	unsigned short ip_off;
	unsigned char ip_ttl;
	unsigned char ip_p;
	unsigned short ip_sum;
	struct in_addr ip_src;
	struct in_addr ip_dst;
	unsigned char opts[0];
#define ip_hl	un.ip_ver_hl.hl
#define ip_v	un.ip_ver_hl.ver
#define ip_get_ver(p)	(((p)->ver_hl >> 4) & 0xf)
#define ip_get_hl(p)	(((p)->ver_hl >> 0) & 0xf)
#define ip_set_ver_hl(p, ver, hl)	((p)->ver_hl = ((ver) << 4) | (hl))
} IP_HDR;

#define IPVERSION	4

#define IP_RF		0x8000
#define IP_DF		0x4000
#define IP_MF		0x2000
#define IP_OFFMASK	0x1fff

#define IP_MAXPACKET	65535

#ifdef __cplusplus
}
#endif

#endif /* !__SYS_NET_NETINET_IP_H__ */

