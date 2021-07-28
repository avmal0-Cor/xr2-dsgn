/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_SYS_SELECT_H__)
#define __SYS_NET_SYS_SELECT_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define FD_SETSIZE	1024
	typedef unsigned int fd_mask;
#define NFDBITS		(sizeof(fd_mask) * 8)
#define NFDBITS_SHIFT	5
#define NFDBITS_MASK	(NFDBITS - 1)

	typedef struct fd_set {
		fd_mask fds_bits[FD_SETSIZE >> NFDBITS_SHIFT];
	} fd_set;

#define FD_SET(n, p) \
	((p)->fds_bits[(n) >> NFDBITS_SHIFT] |= (1 << ((n) & NFDBITS_MASK)))
#define FD_CLR(n, p) \
	((p)->fds_bits[(n) >> NFDBITS_SHIFT] &= ~(1 << ((n) & NFDBITS_MASK)))
#define FD_ISSET(n, p) \
	((p)->fds_bits[(n) >> NFDBITS_SHIFT] & (1 << ((n) & NFDBITS_MASK)))
#define FD_ZERO(p) do { \
	fd_set *__fds = (p); \
	unsigned int __i; \
	for(__i = 0; __i < (FD_SETSIZE >> NFDBITS_SHIFT); __i++) \
		__fds->fds_bits[__i] = 0; \
} while(0)

	int socketselect(int nfds, fd_set * readfds, fd_set * writefds,
					 fd_set * exceptfds, struct timeval *timeout);

#if defined(__cplusplus)
}
#endif
#endif							/* !__SYS_NET_SYS_SELECT_H__ */
