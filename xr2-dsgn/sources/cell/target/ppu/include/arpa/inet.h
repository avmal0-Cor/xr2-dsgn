/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_ARPA_INET_H__)
#define __SYS_NET_ARPA_INET_H__

#include <sys/ansi.h>

#if defined(__cplusplus)
extern "C" {
#endif

in_addr_t inet_addr(const char *cp);
struct in_addr inet_makeaddr(in_addr_t net, in_addr_t lna);
in_addr_t inet_netof(struct in_addr in);
in_addr_t inet_lnaof(struct in_addr in);
in_addr_t inet_network(const char *cp);
char *inet_ntoa(struct in_addr in);
int inet_aton(const char *cp, struct in_addr *addr);
const char *inet_ntop(int af, const void *src, char *dst,
	socklen_t size);
int inet_pton(int af, const char *src, void *dst);

#ifdef __cplusplus
}
#endif

#endif	/* !__SYS_NET_ARPA_INET_H__ */

