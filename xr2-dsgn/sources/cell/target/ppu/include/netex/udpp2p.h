/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETEX_UDPP2P_H__)
#define __SYS_NET_NETEX_UDPP2P_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct sys_net_udpp2p_test_param {
	unsigned int	drop_rate;		/* 0 - 100 % */
	unsigned int	drop_duration;	/* msec */
	unsigned int	delay_time;			/* msec */
	unsigned int	delay_qlen;			/* pkts */
} sys_net_udpp2p_test_param_t;

int sys_net_set_udpp2p_test_param(sys_net_udpp2p_test_param_t *param);
int sys_net_get_udpp2p_test_param(sys_net_udpp2p_test_param_t *param);

#if defined(__cplusplus)
}
#endif

#endif	/* !__SYS_NET_NETEX_UDPP2P_H__ */

