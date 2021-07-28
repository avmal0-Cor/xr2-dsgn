/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/* errno.h standard header */
#ifndef _ERRNO
#define _ERRNO
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#if !defined(__PME__)
/* The Lv-2 error codes. */
#include <sys/return_code.h>
#endif

_C_STD_BEGIN

#if 0
/* Non standard error codes. */
#define EFBIG          0x001B
#define EMLINK         0x001F
#define ENFILE         0x0017
#define ENOSPC         0x001C
#define ENOTTY         0x0019
#define EPIPE          0x0020
#define EROFS          0x001E
#define ESPIPE         0x001D

#define E2BIG         0x0007  
#define EACCES        0x000D  
#define EBADF         0x0009  
#define EEXIST        0x0011  
#define EIO           0x0005  
#define EISDIR        0x0015  
#define EMFILE        0x0018  
#define ENODEV        0x0013  
#define ENOEXEC       0x0008  
#define ENOTDIR       0x0014  
#define ENXIO         0x0006  
#define EPERM         0x0001  
#define EXDEV         0x0012  

#define EBADMSG       0x004D  
#define ECANCELED     0x002F  
#define EDEADLK       0x002D  
#define EINPROGRESS   0x0096  
#define EMSGSIZE      0x0061  
#define ENAMETOOLONG  0x004E  
#define ENOLCK        0x002E  
#define ENOTEMPTY     0x005D  
#define ENOTSUP       0x0030  
#endif

		/* DECLARATIONS */
_C_LIB_DECL

 #if !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE
extern _TLS_QUAL int _Errno;

  #define errno	(_CSTD _Errno)

 #else /* !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE */
int *_Geterrno(void);

  #define errno	(*_CSTD _Geterrno())
 #endif /* !_MULTI_THREAD || _COMPILER_TLS && !_GLOBAL_LOCALE */

_END_C_LIB_DECL
_C_STD_END
#endif /* _ERRNO */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
