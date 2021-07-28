/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _FCNTL_H
#define _FCNTL_H

#include <yvals.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/fs.h>

#define O_CREAT         CELL_FS_O_CREAT
#define O_EXCL          CELL_FS_O_EXCL
#define O_TRUNC         CELL_FS_O_TRUNC
#define O_APPEND        CELL_FS_O_APPEND
#define O_ACCMODE       CELL_FS_O_ACCMODE
#define   O_RDONLY      CELL_FS_O_RDONLY
#define   O_RDWR        CELL_FS_O_RDWR
#define   O_WRONLY      CELL_FS_O_WRONLY

_C_STD_BEGIN
_C_LIB_DECL
int open(const char *, int, ...);
_END_C_LIB_DECL
_C_STD_END

#if defined(_STD_USING)
using _CSTD open;
#endif

#endif /* _FCNTL_H */
