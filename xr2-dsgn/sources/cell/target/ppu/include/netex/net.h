/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NETEX_NET_H__)
#define __SYS_NET_NETEX_NET_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef long long sys_net_thread_id_t;

typedef struct sys_net_initialize_parameter {
	void *memory;
	int memory_size;
	int flags;
} sys_net_initialize_parameter_t;

typedef struct sys_net_test_param_ops {
	unsigned short drop_rate;	/* 0-100 % (1%) */
	unsigned short drop_duration;	/* 0-8000 ms (1ms) */
	unsigned short pass_duration;	/* 0-8000 ms (1ms) */
	unsigned short delay_time;	/* 0-8000 ms (1ms) */
	unsigned short delay_jitter;	/* 0-delay_time ms (1ms) */
	unsigned short order_rate;	/* 0-100 % (1%) */
	unsigned short order_delay_time;	/* 0-8000 ms (1ms) */
	unsigned short duplication_rate;	/* 0-100 % (1%) */
	unsigned int bps_limit;	/* 0, 1024-1G bps (1bps) */
	unsigned short lower_size_limit;	/* 0-upper_size_limit bytes */
	unsigned short upper_size_limit;	/* lower_size_limit-1500 */
	unsigned long long policy_pattern;	/* 0-31: game, 32-63: system */
	unsigned short policy_flags[64];	/* each policy */
	unsigned char reserved[64];
} sys_net_test_param_ops_t;	/* sizeof(sys_net_test_param_ops_t) = 224 */

typedef struct sys_net_test_param {
	unsigned short version;	/* specified by user (IN) */
	unsigned short option_number;	/* 0 or 1-n (IN) */
	unsigned short current_version;	/* current version (OUT) */
	unsigned short result;	/* result (OUT) */
	unsigned int flags;	/* (IN) */
	unsigned int reserved2;
	sys_net_test_param_ops_t send;	/* (IN) */
	sys_net_test_param_ops_t recv;	/* (IN) */
	unsigned int seed;
	unsigned char reserved[44];
} sys_net_test_param_t;	/* sizeof(sys_net_test_param_t) = 512 */

#define sys_net_initialize_network() ({ \
	static char __libnet_memory[128 * 1024]; \
	sys_net_initialize_parameter_t __libnet_param; \
	__libnet_param.memory = __libnet_memory; \
	__libnet_param.memory_size = sizeof(__libnet_memory); \
	__libnet_param.flags = 0; \
	sys_net_initialize_network_ex(&__libnet_param); \
})

int sys_net_initialize_network_ex(sys_net_initialize_parameter_t *param);
int sys_net_finalize_network(void);

int sys_net_abort_socket(int sockfd, int flags);
int sys_net_abort_resolver(sys_net_thread_id_t tid, int flags);

int sys_net_open_dump(int len, int flags);
int sys_net_read_dump(int id, void *buf, int len, int *pflags);
int sys_net_close_dump(int id, int *pflags);

int sys_net_set_resolver_configurations(int retrans, int retry, int flags);

int sys_net_free_thread_context(sys_net_thread_id_t tid, int flags);

int sys_net_show_ifconfig(void);
int sys_net_show_nameserver(void);
int sys_net_show_route(void);

int sys_net_set_netemu_test_param(sys_net_test_param_t *param);
int sys_net_get_netemu_test_param(sys_net_test_param_t *param);

/* flags for sys_net_initialize_parameter_t */
#define SYS_NET_INIT_ERROR_CHECK	0x0001

/* flags for sys_net_abort_{resolver,socket} */
#define SYS_NET_ABORT_STRICT_CHECK	0x0001

/* flags for sys_net_{open,read,close}_dump */
#define SYS_NET_DUMP_TCPDUMP	0x0000	/* open(set) */
#define SYS_NET_DUMP_SYSLOG	0x0001	/* open(set) */
#define SYS_NET_DUMP_PEEK	0x0010	/* read(set) */
#define SYS_NET_DUMP_DONTWAIT	0x0020	/* read(set) */
#define SYS_NET_DUMP_OVERFLOW	0x0040	/* read(get) */

/* flags for sys_net_free_thread_context */
#define SYS_NET_THREAD_SELF	0x0001
#define SYS_NET_THREAD_ALL	0x0002
#define SYS_NET_THREAD_ABORT_RESOLVER	0x0004

#if defined(__cplusplus)
}
#endif

#endif	/* !__SYS_NET_NETEX_NET_H__ */

