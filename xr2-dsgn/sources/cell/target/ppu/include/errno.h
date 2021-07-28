/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
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

#ifdef __PME__
/* PME XXX */
#undef ERANGE
#undef EINTR

#define EBADF         (-1)
#define EAGAIN        (-2)
#define ENOENT        (-3)
#define EISDIR        (-4)
#define EMFILE        (-5)
#define ENOTDIR       (-6)
#define ENAMETOOLONG  (-7)
#define EACCES        (-8)
#define ENOMEM        (-9) // XXX in SUS, it means something different.
#define ESPIPE        (-10)
#define EINVAL        (-11)
#define ENOTSOCK      (-12)
#define EINTR         (-13)
#define EADDRNOTAVAIL (-14)
#define EADDRINUSE    (-15)
#define ENOEXEC       (-16)
#define ESRCH         (-17)
#define ENOBUFS       (-18)
#define EAFNOSUPPORT  (-19)
#define EPROTOTYPE    (-20)
#define EPROTONOSUPPORT (-21)
#define ECHILD        (-22)
#define ECONNRESET    (-23)
#define EDESTADDRREQ  (-24)
#define ECONNREFUSED  (-25)
#define EISCONN       (-26)
#define ENOTCONN      (-27) 
#define EOVERFLOW     (-28)
#define EEXIST        (-29)
#define ENOSPC        (-30)
#define EBUSY         (-31)
#define ENODEV        (-32)
#define ERANGE        (-33)
#define E2BIG         (-34)
#define EIO           (-35)
#define EFBIG         (-36)
#define EPERM         (-37)

// not SUS, but Linux
#define EFAULT (-256)

// for Lv-2?
#define EFPOS  (-257)
#define EILSEQ (-258)
#define EDOM   (-259)
#endif

		/* DECLARATIONS */
_C_LIB_DECL

extern _TLS_QUAL int _Errno;

#ifdef __PME__
#define errno (_CSTD _Errno) // for special vsh
#else
int *_Geterrno(void); // for special vsh
#define errno (*_CSTD _Geterrno()) // for special vsh
#endif

_END_C_LIB_DECL
_C_STD_END
#endif /* _ERRNO */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
