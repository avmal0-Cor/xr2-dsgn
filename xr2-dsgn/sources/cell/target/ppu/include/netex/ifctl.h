/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETEX_IFCTL_H__)
#define __SYS_NET_NETEX_IFCTL_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define SYS_NET_CC_GET_MEMORY_FREE_CURRENT	0x00000280
#define SYS_NET_CC_GET_MEMORY_FREE_MINIMUM	0x00000281
#define SYS_NET_CC_GET_LIB_MEMORY_FREE_CURRENT	0x00000282
#define SYS_NET_CC_GET_LIB_MEMORY_FREE_MINIMUM	0x00000283
#define SYS_NET_CC_CLEAR_DNS_CACHE	0x00000290

int sys_net_if_ctl(int if_id, int code, void *ptr, int len);

#if defined(__cplusplus)
}
#endif

#endif	/* !__SYS_NET_NETEX_IFCTL_H__ */

