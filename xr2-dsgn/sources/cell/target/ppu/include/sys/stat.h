/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef _SYS_STAT_H_
#define	_SYS_STAT_H_

#ifndef _YVALS
#include <yvals.h>
#endif /* _YVALS */

#include <time.h>
#include <types.h>
#include <sys/fs.h>

/*
 * st_mode(mode_t) has the following bits
 */
#define S_IFMT CELL_FS_S_IFMT	/* type of file */
#define        S_IFDIR  CELL_FS_S_IFDIR	/* directory */
#define        S_IFREG  CELL_FS_S_IFREG	/* regular */
#define        S_IFLNK  CELL_FS_S_IFLNK	/* symbolic link */
#define        S_IFWHT  CELL_FS_S_IFWHT	/* whiteout */

#define S_IRWXU CELL_FS_S_IRWXU	/* RWX mask for owner */
#define         S_IRUSR CELL_FS_S_IRUSR	/* R for owner */
#define         S_IWUSR CELL_FS_S_IWUSR	/* W for owner */
#define         S_IXUSR CELL_FS_S_IXUSR	/* X for owner */

#define S_IRWXG CELL_FS_S_IRWXG	/* RWX mask for group */
#define         S_IRGRP CELL_FS_S_IRGRP	/* R for group */
#define         S_IWGRP CELL_FS_S_IWGRP	/* W for group */
#define         S_IXGRP CELL_FS_S_IXGRP	/* X for group */

#define S_IRWXO CELL_FS_S_IRWXO	/* RWX mask for other */
#define         S_IROTH CELL_FS_S_IROTH	/* R for other */
#define         S_IWOTH CELL_FS_S_IWOTH	/* W for other */
#define         S_IXOTH CELL_FS_S_IXOTH	/* X for other */

_C_STD_BEGIN struct stat {
	/*
	 * POSIX defines that at least the following members should be
	 * in stat structure
	 */
	dev_t st_dev;				/* ID of the device */
	ino_t st_ino;				/* File serial number */
	mode_t st_mode;				/* Mode of file */
	nlink_t st_nlink;			/* Number of links */
#if 0
	uid_t st_uid;				/* User ID */
	gid_t st_gid;				/* Group ID */
#else
	int32_t st_uid;				/* User ID */
	int32_t st_gid;				/* Group ID */
#endif
	dev_t st_rdev;				/* Device ID */
	off64_t st_size;			/* File size in bytes */
	time_t st_atime;			/* Last access time */
	time_t st_mtime;			/* Last data modiflcation time */
	time_t st_ctime;			/* Last status chage time */
	blksize_t st_blksize;		/* I/O block size */
	blkcnt_t st_blocks;			/* Number of blocks */
	/*
	 * User defined members
	 */
};
_C_LIB_DECL int mkdir(const char *, mode_t);
int chmod(const char *, mode_t);
_END_C_LIB_DECL _C_STD_END
#if defined(_STD_USING)
	using _CSTD mkdir;
using _CSTD chmod;
using _CSTD stat;
#endif

#endif /* !_SYS_STAT_H_ */
