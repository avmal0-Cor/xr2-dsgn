/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef	_DIRENT_H
#define	_DIRENT_H

#ifndef _YVALS
#include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h>
#include <sys/synchronization.h>
#include <sys/fs.h>

/*
 * d_type
 */
#define DT_UNKNOWN CELL_FS_TYPE_UNKNOWN
#define DT_DIR     CELL_FS_TYPE_DIRECTORY
#define DT_REG     CELL_FS_TYPE_REGULAR
#define DT_LNK     CELL_FS_TYPE_SYMLINK

struct dirent  {
  /*
   ino_t is not defined in this system 
  ino_t          d_ino;
  */
  int            d_ino ;
  off_t          d_off ;
  unsigned short d_reclen ;
  unsigned char  d_type ;
  char           d_name[256] ;
};

struct dstreamx {
  int           fd;
  struct dirent *dir; 
  long          loc;
  long          count;
  sys_mutex_t   mutex;
};
typedef struct dstreamx DIR; /* Opaque to the user */

_C_STD_BEGIN
_C_LIB_DECL
DIR           *opendir (const char * name) ;
int           closedir (DIR * dirp);
struct dirent *readdir (DIR *dirp);
_END_C_LIB_DECL
_C_STD_END

#if defined(_STD_USING)
using _CSTD opendir;
using _CSTD closedir;
using _CSTD readdir;
#endif


#endif
