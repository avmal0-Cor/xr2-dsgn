/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETDB_H__)
#define __SYS_NET_NETDB_H__

#include <sys/ansi.h>

#if defined(__cplusplus)
extern "C" {
#endif

int *_sys_net_h_errno_loc(void);	/* for multi-thread */

#define sys_net_h_errno		(*_sys_net_h_errno_loc())

struct hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
#define	h_addr h_addr_list[0]
};

/* error codes of gethostbyname() and gethostbyaddr() */
#define	NETDB_INTERNAL		-1
#define	NETDB_SUCCESS		0
#define	HOST_NOT_FOUND		1
#define	TRY_AGAIN		2
#define	NO_RECOVERY		3
#define	NO_DATA			4
#define	NO_ADDRESS		NO_DATA


struct hostent *gethostbyaddr(const char *addr,
	socklen_t len,
	int type);

struct hostent *gethostbyname(const char *name);

#ifdef __cplusplus
}
#endif

#endif	/* !__SYS_NET_NETDB_H__ */

