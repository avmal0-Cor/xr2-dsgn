/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETEX_NS_H__)
#define __SYS_NET_NETEX_NS_H__

#if defined(__cplusplus)
extern "C" {
#endif

int sys_net_get_lib_name_server(struct in_addr *primary,
	struct in_addr *secondary);
int sys_net_set_lib_name_server(struct in_addr *primary,
	struct in_addr *secondary);

#if defined(__cplusplus)
}
#endif

#endif	/* !__SYS_NET_NETEX_NS_H__ */

