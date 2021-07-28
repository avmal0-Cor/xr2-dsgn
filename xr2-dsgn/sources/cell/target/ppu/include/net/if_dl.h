/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if !defined(__SYS_NET_NET_IF_DL_H__)
#define __SYS_NET_NET_IF_DL_H__

#include <sys/ansi.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct sockaddr_dl {
	unsigned char sdl_len;
	sa_family_t sdl_family;
	unsigned short sdl_index;
	unsigned char sdl_type;
	unsigned char sdl_nlen;
	unsigned char sdl_alen;
	unsigned char sdl_slen;
	char sdl_data[12];
};

#ifdef __cplusplus
}
#endif

#endif	/* !__SYS_NET_IF_DL_H__ */

