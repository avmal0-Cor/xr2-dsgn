/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/**
 * \ingroup all_syscall
 * \defgroup console_syscall Console I/O
 * \brief console system calls defined in sys/console.h
 *
 * Interfaces defined in sys/console.h 
 *
 * These system calls are provided for special and temporary purposes.
 * These interfaces will be disappearred in the future releases.
 */
/*@{*/

#ifndef	__SYS_SYS_CONSOLE_H__
#define	__SYS_SYS_CONSOLE_H__

#include <sys/cdefs.h>

CDECL_BEGIN

void console_putc(int c);
int console_write(char *data, unsigned long len);
int console_getc(void);

CDECL_END

#endif /* __SYS_SYS_CONSOLE_H__ */

/*@}*/
