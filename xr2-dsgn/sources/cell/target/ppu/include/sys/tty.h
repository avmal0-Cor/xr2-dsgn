/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup tty TTY
 * \brief tty system calls defined in sys/tty.h
 *
 * Interfaces defined in sys/tty.h 
 *
 */
/*@{*/

#ifndef	__SYS_SYS_TTY_H__
#define	__SYS_SYS_TTY_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#define SYS_TTYP_MAX  (0x10)

#define SYS_TTYP0     (0)
#define SYS_TTYP1     (1)
#define SYS_TTYP2     (2)
#define SYS_TTYP3     (3)
#define SYS_TTYP4     (4)
#define SYS_TTYP5     (5)
#define SYS_TTYP6     (6)
#define SYS_TTYP7     (7)
#define SYS_TTYP8     (8)
#define SYS_TTYP9     (9)
#define SYS_TTYP10   (10)
#define SYS_TTYP11   (11)
#define SYS_TTYP12   (12)
#define SYS_TTYP13   (13)
#define SYS_TTYP14   (14)
#define SYS_TTYP15   (15)

#define  SYS_TTYP_PPU_STDIN    (SYS_TTYP0)
#define  SYS_TTYP_PPU_STDOUT   (SYS_TTYP0)
#define  SYS_TTYP_PPU_STDERR   (SYS_TTYP1)
#define  SYS_TTYP_SPU_STDOUT   (SYS_TTYP2)

CDECL_BEGIN

extern inline int sys_tty_write(unsigned int ch, const void *buf,
								unsigned int len, unsigned int *pwritelen);

extern inline int sys_tty_write(unsigned int ch, const void *buf,
								unsigned int len, unsigned int *pwritelen)
{
	system_call_4(SYS_TTY_WRITE, ch, (uint32_t) buf, len, (uint32_t) pwritelen);

#ifdef REG_PASS_SYS_TTY_WRITE
	REG_PASS_SYS_TTY_WRITE;
#endif
	return_to_user_prog(int);
}


extern inline int sys_tty_read(unsigned int ch, void *buf, unsigned int len,
							   unsigned int *preadlen);

extern inline int sys_tty_read(unsigned int ch, void *buf, unsigned int len,
							   unsigned int *preadlen)
{
	system_call_4(SYS_TTY_READ, ch, (uint32_t) buf, len, (uint32_t) preadlen);

#ifdef REG_PASS_SYS_TTY_READ
	REG_PASS_SYS_TTY_READ;
#endif
	return_to_user_prog(int);
}


CDECL_END

#endif /* __SYS_SYS_TTY_H__ */

/*@}*/
