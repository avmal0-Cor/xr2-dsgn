/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETEX_SOCKINFO_H__)
#define __SYS_NET_NETEX_SOCKINFO_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct sys_net_sockinfo {
	int s;
	int proto;
	int recv_queue_length;
	int send_queue_length;
	struct in_addr local_adr;
	int local_port;
	struct in_addr remote_adr;
	int remote_port;
	int state;
} sys_net_sockinfo_t;

typedef struct sys_net_sockinfo_ex {
	int s;
	int proto;
	int recv_queue_length;
	int send_queue_length;
	struct in_addr local_adr;
	int local_port;
	struct in_addr remote_adr;
	int remote_port;
	int state;
	int socket_type;
	int local_vport;
	int remote_vport;
	int reserved[8];
} sys_net_sockinfo_ex_t;

/* state for sys_net_get_sockinfo and sys_net_get_sockinfo_ex */
#define SYS_NET_STATE_UNKNOWN		0
#define SYS_NET_STATE_CLOSED		1
#define SYS_NET_STATE_CREATED		2
#define SYS_NET_STATE_OPENED		3
#define SYS_NET_STATE_LISTEN		4
#define SYS_NET_STATE_SYN_SENT		5
#define SYS_NET_STATE_SYN_RECEIVED	6
#define SYS_NET_STATE_ESTABLISHED	7
#define SYS_NET_STATE_FIN_WAIT_1	8
#define SYS_NET_STATE_FIN_WAIT_2	9
#define SYS_NET_STATE_CLOSE_WAIT	10
#define SYS_NET_STATE_CLOSING		11
#define SYS_NET_STATE_LAST_ACK		12
#define SYS_NET_STATE_TIME_WAIT		13

/* flags for sys_net_get_sockinfo_ex */
#define SYS_NET_SOCKINFO_EX_USER_ONLY	0x00000001
#define SYS_NET_SOCKINFO_EX_PCBTABLES	0x00000002

int sys_net_get_sockinfo(int s, sys_net_sockinfo_t *p, int n);
int sys_net_get_sockinfo_ex(int s, sys_net_sockinfo_ex_t *p, int n, int flags);

#if defined(__cplusplus)
}
#endif

#endif	/* !__SYS_NET_NETEX_SOCKINFO_H__ */

