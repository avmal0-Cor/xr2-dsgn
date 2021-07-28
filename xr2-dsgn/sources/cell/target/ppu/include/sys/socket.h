/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_SYS_SOCKET_H__)
#define __SYS_NET_SYS_SOCKET_H__

#include <sys/ansi.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* socket types */
#define SOCK_STREAM	1
#define SOCK_DGRAM	2
#define SOCK_RAW	3
#define SOCK_DGRAM_P2P	6
#define SOCK_STREAM_P2P	10

/* option flags */
#define SO_REUSEADDR	0x0004
#define SO_KEEPALIVE	0x0008
#define SO_BROADCAST	0x0020
#define SO_LINGER	0x0080
#define SO_OOBINLINE	0x0100
#define SO_REUSEPORT	0x0200
#define SO_ONESBCAST	0x0800
#define SO_USECRYPTO	0x1000
#define SO_USESIGNATURE	0x2000

/* additional options */
#define SO_SNDBUF	0x1001
#define SO_RCVBUF	0x1002
#define SO_SNDLOWAT	0x1003
#define SO_RCVLOWAT	0x1004
#define SO_SNDTIMEO	0x1005
#define SO_RCVTIMEO	0x1006
#define SO_ERROR	0x1007
#define SO_TYPE		0x1008

#define SO_NBIO		0x1100
#define SO_TPPOLICY	0x1101

	struct linger {
		int l_onoff;			/* on/off */
		int l_linger;			/* linger time [sec] */
	};

#define	SOL_SOCKET	0xffff		/* options for socket level */

/* address families */
#define AF_UNSPEC		0
#define AF_LOCAL		1
#define AF_UNIX			AF_LOCAL
#define AF_INET			2
#define AF_IMPLINK		3
#define AF_PUP			4
#define AF_CHAOS		5
#define AF_NS			6
#define AF_ISO			7
#define AF_OSI			AF_ISO
#define AF_ECMA			8
#define AF_DATAKIT		9
#define AF_CCITT		10
#define AF_SNA			11
#define AF_DECnet		12
#define AF_DLI			13
#define AF_LAT			14
#define AF_HYLINK		15
#define AF_APPLETALK		16
#define AF_ROUTE		17
#define AF_LINK			18
#define pseudo_AF_XTP		19
#define AF_COIP			20
#define AF_CNT			21
#define pseudo_AF_RTIP		22
#define AF_IPX			23
#define AF_INET6		24
#define pseudo_AF_PIP		25
#define AF_ISDN			26
#define AF_E164			AF_ISDN
#define AF_NATM			27
#define AF_ARP			28
#define pseudo_AF_KEY		29
#define pseudo_HDRCMPLT		30
#define AF_MAX			31

	struct sockaddr {
		unsigned char sa_len;
		sa_family_t sa_family;
		char sa_data[14];
	};

	struct iovec {
		int zero1;				/* system use, big endian */
		void *iov_base;
		int zero2;				/* system use, big endian */
		size_t iov_len;
	};

	struct msghdr {
		int zero1;				/* system use, big endian */
		void *msg_name;
		socklen_t msg_namelen;
		int padding1;			/* system use, 32bit */
		int zero2;				/* system use, big endian */
		struct iovec *msg_iov;
		int msg_iovlen;
		int padding2;			/* system use, 32bit */
		int zero3;				/* system use, big endian */
		void *msg_control;
		socklen_t msg_controllen;
		int msg_flags;
	};

#define	MSG_OOB		0x1
#define	MSG_PEEK	0x2
#define MSG_DONTROUTE	0x4
#define MSG_EOR		0x8
#define MSG_TRUNC	0x10
#define MSG_CTRUNC	0x20
#define	MSG_WAITALL	0x40
#define	MSG_DONTWAIT	0x80
#define MSG_BCAST	0x100
#define MSG_MCAST	0x200
#define MSG_USECRYPTO	0x400
#define MSG_USESIGNATURE	0x800

#define SHUT_RD		0
#define SHUT_WR		1
#define SHUT_RDWR	2

	int accept(int s, struct sockaddr *addr, socklen_t * addrlen);
	int bind(int s, const struct sockaddr *addr, socklen_t addrlen);
	int connect(int s, const struct sockaddr *name, socklen_t namelen);
	int getpeername(int s, struct sockaddr *name, socklen_t * namelen);
	int getsockname(int s, struct sockaddr *name, socklen_t * namelen);
	int getsockopt(int s, int level, int optname, void *optval,
				   socklen_t * optlen);
	int listen(int s, int backlog);
	ssize_t recv(int s, void *buf, size_t len, int flags);
	ssize_t recvfrom(int s, void *buf, size_t len,
					 int flags, struct sockaddr *from, socklen_t * fromlen);
	ssize_t recvmsg(int s, struct msghdr *msg, int flags);
	ssize_t send(int s, const void *msg, size_t len, int flags);
	ssize_t sendto(int s, const void *msg, size_t len,
				   int flags, const struct sockaddr *to, socklen_t tolen);
	ssize_t sendmsg(int s, const struct msghdr *msg, int flags);
	int setsockopt(int s, int level, int optname, const void *optval,
				   socklen_t optlen);
	int shutdown(int s, int how);
	int socket(int domain, int type, int protocol);
	int socketclose(int s);

#ifdef __cplusplus
}
#endif
#endif							/* !__SYS_NET_SYS_SOCKET_H__ */
