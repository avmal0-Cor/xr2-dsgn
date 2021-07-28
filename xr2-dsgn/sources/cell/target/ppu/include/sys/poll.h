/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_SYS_POLL_H__)
#define __SYS_NET_SYS_POLL_H__

#if defined(__cplusplus)
extern "C" {
#endif

	typedef unsigned int nfds_t;

	struct pollfd {
		int fd;
		short events;
		short revents;
	};

#define POLLIN		0x0001
#define POLLPRI		0x0002
#define POLLOUT		0x0004
#define POLLERR		0x0008		/* revent only */
#define POLLHUP		0x0010		/* revent only */
#define POLLNVAL	0x0020		/* revent only */
#define POLLRDNORM	0x0040
#define POLLWRNORM	POLLOUT
#define POLLRDBAND	0x0080
#define POLLWRBAND	0x0100

	int socketpoll(struct pollfd *fds, nfds_t nfds, int timeout);

#if defined(__cplusplus)
}
#endif
#endif							/* !__SYS_NET_SYS_POLL_H__ */
