/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __LV2_FS_LOWFS_FS_EXTERNAL_H__
#define __LV2_FS_LOWFS_FS_EXTERNAL_H__

CDECL_BEGIN
#pragma pack(4)

/*
 * max path length except for mount point length
 */
#define CELL_FS_MAX_FS_PATH_LENGTH (1024)
/*
 * max file name length
 */
#define CELL_FS_MAX_FS_FILE_NAME_LENGTH (255)
/*
 * max mount point length
 */
#define CELL_FS_MAX_MP_LENGTH      (31)

/*
 * CellFsMode
 */
typedef int CellFsMode;
#define CELL_FS_S_IFMT 0170000	/* type of file */
#define        CELL_FS_S_IFDIR 0040000	/* directory */
#define        CELL_FS_S_IFREG 0100000	/* regular */
#define        CELL_FS_S_IFLNK 0120000	/* symbolic link */
#define        CELL_FS_S_IFWHT 0160000	/* unknown */
#define CELL_FS_S_IRWXU 0000700	/* RWX mask for owner */
#define        CELL_FS_S_IRUSR 0000400	/* R for owner */
#define        CELL_FS_S_IWUSR 0000200	/* W for owner */
#define        CELL_FS_S_IXUSR 0000100	/* X for owner */
#define CELL_FS_S_IRWXG 0000070	/* RWX mask for group */
#define        CELL_FS_S_IRGRP 0000040	/* R for group */
#define        CELL_FS_S_IWGRP 0000020	/* W for group */
#define        CELL_FS_S_IXGRP 0000010	/* X for group */
#define CELL_FS_S_IRWXO 0000007	/* RWX mask for other */
#define        CELL_FS_S_IROTH 0000004	/* R for other */
#define        CELL_FS_S_IWOTH 0000002	/* W for other */
#define        CELL_FS_S_IXOTH 0000001	/* X for other */

/* 006 */
#define CELL_FS_OTH_CREATE_MODE_RW  (CELL_FS_S_IRWXO&~CELL_FS_S_IXOTH)
/* 005 */
#define CELL_FS_OTH_CREATE_MODE_RX  (CELL_FS_S_IRWXO&~CELL_FS_S_IWOTH)
/* 700 */
#define CELL_FS_DEFAULT_CREATE_MODE_1 CELL_FS_S_IRWXU
/* 600 */
#define CELL_FS_DEFAULT_CREATE_MODE_2 (CELL_FS_S_IRUSR|CELL_FS_S_IWUSR)
/* 500 */
#define CELL_FS_DEFAULT_CREATE_MODE_3 (CELL_FS_S_IRUSR|CELL_FS_S_IXUSR)
/* 400 */
#define CELL_FS_DEFAULT_CREATE_MODE_4 CELL_FS_S_IRUSR
/* 707 */
#define CELL_FS_DEFAULT_CREATE_MODE_5 (CELL_FS_S_IRWXU|CELL_FS_S_IRWXO)
/* 606 */
#define CELL_FS_DEFAULT_CREATE_MODE_6 (CELL_FS_DEFAULT_CREATE_MODE_2|CELL_FS_OTH_CREATE_MODE_RW)
/* 505 */
#define CELL_FS_DEFAULT_CREATE_MODE_7 (CELL_FS_DEFAULT_CREATE_MODE_3|CELL_FS_OTH_CREATE_MODE_RX)
/* 404 */
#define CELL_FS_DEFAULT_CREATE_MODE_8 (CELL_FS_S_IRUSR|CELL_FS_S_IROTH)

/* CellFsDirent.d_type */
#define CELL_FS_TYPE_UNKNOWN   0
#define CELL_FS_TYPE_DIRECTORY 1
#define CELL_FS_TYPE_REGULAR   2
#define CELL_FS_TYPE_SYMLINK   3

/*
 * CellFsDirent
 */
typedef struct CellFsDirent {
	uint8_t d_type;				/* file type                  */
	uint8_t d_namlen;			/* length of string in d_name */
	char d_name[CELL_FS_MAX_FS_FILE_NAME_LENGTH + 1];
} CellFsDirent;

/* CellFsStat.st_uid */
#define NO_UID (-1)
#define SYSTEM_UID (0)

/* CellFsStat.st_gid */
#define NO_GID (-1)

/*
 * CellFsStat
 */
typedef struct CellFsStat {
	CellFsMode st_mode;			/* mode                     */
	int st_uid;					/* user  id -> not used now */
	int st_gid;					/* group id -> not used now */
	time_t st_atime;			/* last access time         */
	time_t st_mtime;			/* last modified time       */
	time_t st_ctime;			/* create time              */
	uint64_t st_size;			/* file size                */
	uint64_t st_blksize;		/* block size               */
} CellFsStat;

/*
 * CellFsUtimbuf
 */
typedef struct CellFsUtimbuf {
	time_t actime;
	time_t modtime;
} CellFsUtimbuf;

/*
 * flags for cellFsOpen
 */
#define CELL_FS_O_CREAT         000100
#define CELL_FS_O_EXCL          000200
#define CELL_FS_O_TRUNC         001000
#define CELL_FS_O_APPEND        002000
#define CELL_FS_O_ACCMODE       000003
#define        CELL_FS_O_RDONLY 000000
#define        CELL_FS_O_RDWR   000002
#define        CELL_FS_O_WRONLY 000001
#define CELL_FS_O_MSELF		010000

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif
/*
 * whence for cellFsLseek(original definition are in stdio.h)
 */
#define CELL_FS_SEEK_SET SEEK_SET
#define CELL_FS_SEEK_CUR SEEK_CUR
#define CELL_FS_SEEK_END SEEK_END

/*
 * error code
 */
typedef int CellFsErrno;

#define CELL_FS_OK                  CELL_OK
#define CELL_FS_ERROR_EDOM          EDOM
#define CELL_FS_ERROR_EFAULT        EFAULT
#define CELL_FS_ERROR_EFBIG         EFBIG
#define CELL_FS_ERROR_EFPOS         EFPOS
#define CELL_FS_ERROR_EMLINK        EMLINK
#define CELL_FS_ERROR_ENFILE        ENFILE
#define CELL_FS_ERROR_ENOENT        ENOENT
#define CELL_FS_ERROR_ENOSPC        ENOSPC
#define CELL_FS_ERROR_ENOTTY        ENOTTY
#define CELL_FS_ERROR_EPIPE         EPIPE
#define CELL_FS_ERROR_ERANGE        ERANGE
#define CELL_FS_ERROR_EROFS         EROFS
#define CELL_FS_ERROR_ESPIPE        ESPIPE
#define CELL_FS_ERROR_E2BIG         E2BIG
#define CELL_FS_ERROR_EACCES        EACCES
#define CELL_FS_ERROR_EAGAIN        EAGAIN
#define CELL_FS_ERROR_EBADF         EBADF
#define CELL_FS_ERROR_EBUSY         EBUSY
#define CELL_FS_ERROR_ECHILD        ECHILD
#define CELL_FS_ERROR_EEXIST        EEXIST
#define CELL_FS_ERROR_EINTR         EINTR
#define CELL_FS_ERROR_EINVAL        EINVAL
#define CELL_FS_ERROR_EIO           EIO
#define CELL_FS_ERROR_EISDIR        EISDIR
#define CELL_FS_ERROR_EMFILE        EMFILE
#define CELL_FS_ERROR_ENODEV        ENODEV
#define CELL_FS_ERROR_ENOEXEC       ENOEXEC
#define CELL_FS_ERROR_ENOMEM        ENOMEM
#define CELL_FS_ERROR_ENOTDIR       ENOTDIR
#define CELL_FS_ERROR_ENXIO         ENXIO
#define CELL_FS_ERROR_EPERM         EPERM
#define CELL_FS_ERROR_ESRCH         ESRCH
#define CELL_FS_ERROR_EXDEV         EXDEV
#define CELL_FS_ERROR_EBADMSG       EBADMSG
#define CELL_FS_ERROR_ECANCELED     ECANCELED
#define CELL_FS_ERROR_EDEADLK       EDEADLK
#define CELL_FS_ERROR_EILSEQ        EILSEQ
#define CELL_FS_ERROR_EINPROGRESS   EINPROGRESS
#define CELL_FS_ERROR_EMSGSIZE      EMSGSIZE
#define CELL_FS_ERROR_ENAMETOOLONG  ENAMETOOLONG
#define CELL_FS_ERROR_ENOLCK        ENOLCK
#define CELL_FS_ERROR_ENOSYS        ENOSYS
#define CELL_FS_ERROR_ENOTEMPTY     ENOTEMPTY
#define CELL_FS_ERROR_ENOTSUP       ENOTSUP
#define CELL_FS_ERROR_ETIMEDOUT     ETIMEDOUT
#define CELL_FS_ERROR_EFSSPECIFIC   EFSSPECIFIC
#define CELL_FS_ERROR_EOVERFLOW     EOVERFLOW
#define CELL_FS_ERROR_ENOTMOUNTED   ENOTMOUNTED
#define CELL_FS_ERROR_ENOTMSELF     ENOTMSELF
#define CELL_FS_ERROR_ENOTSDATA     ENOTSDATA
#define CELL_FS_ERROR_EAUTHFATAL    EAUTHFATAL

#define CELL_FS_SUCCEEDED     CELL_FS_OK
#define CELL_FS_EDOM          CELL_FS_ERROR_EDOM
#define CELL_FS_EFAULT        CELL_FS_ERROR_EFAULT
#define CELL_FS_EFBIG         CELL_FS_ERROR_EFBIG
#define CELL_FS_EFPOS         CELL_FS_ERROR_EFPOS
#define CELL_FS_EMLINK        CELL_FS_ERROR_EMLINK
#define CELL_FS_ENFILE        CELL_FS_ERROR_ENFILE
#define CELL_FS_ENOENT        CELL_FS_ERROR_ENOENT
#define CELL_FS_ENOSPC        CELL_FS_ERROR_ENOSPC
#define CELL_FS_ENOTTY        CELL_FS_ERROR_ENOTTY
#define CELL_FS_EPIPE         CELL_FS_ERROR_EPIPE
#define CELL_FS_ERANGE        CELL_FS_ERROR_ERANGE
#define CELL_FS_EROFS         CELL_FS_ERROR_EROFS
#define CELL_FS_ESPIPE        CELL_FS_ERROR_ESPIPE
#define CELL_FS_E2BIG         CELL_FS_ERROR_E2BIG
#define CELL_FS_EACCES        CELL_FS_ERROR_EACCES
#define CELL_FS_EAGAIN        CELL_FS_ERROR_EAGAIN
#define CELL_FS_EBADF         CELL_FS_ERROR_EBADF
#define CELL_FS_EBUSY         CELL_FS_ERROR_EBUSY
#define CELL_FS_ECHILD        CELL_FS_ERROR_ECHILD
#define CELL_FS_EEXIST        CELL_FS_ERROR_EEXIST
#define CELL_FS_EINTR         CELL_FS_ERROR_EINTR
#define CELL_FS_EINVAL        CELL_FS_ERROR_EINVAL
#define CELL_FS_EIO           CELL_FS_ERROR_EIO
#define CELL_FS_EISDIR        CELL_FS_ERROR_EISDIR
#define CELL_FS_EMFILE        CELL_FS_ERROR_EMFILE
#define CELL_FS_ENODEV        CELL_FS_ERROR_ENODEV
#define CELL_FS_ENOEXEC       CELL_FS_ERROR_ENOEXEC
#define CELL_FS_ENOMEM        CELL_FS_ERROR_ENOMEM
#define CELL_FS_ENOTDIR       CELL_FS_ERROR_ENOTDIR
#define CELL_FS_ENXIO         CELL_FS_ERROR_ENXIO
#define CELL_FS_EPERM         CELL_FS_ERROR_EPERM
#define CELL_FS_ESRCH         CELL_FS_ERROR_ESRCH
#define CELL_FS_EXDEV         CELL_FS_ERROR_EXDEV
#define CELL_FS_EBADMSG       CELL_FS_ERROR_EBADMSG
#define CELL_FS_ECANCELED     CELL_FS_ERROR_ECANCELED
#define CELL_FS_EDEADLK       CELL_FS_ERROR_EDEADLK
#define CELL_FS_EILSEQ        CELL_FS_ERROR_EILSEQ
#define CELL_FS_EINPROGRESS   CELL_FS_ERROR_EINPROGRESS
#define CELL_FS_EMSGSIZE      CELL_FS_ERROR_EMSGSIZE
#define CELL_FS_ENAMETOOLONG  CELL_FS_ERROR_ENAMETOOLONG
#define CELL_FS_ENOLCK        CELL_FS_ERROR_ENOLCK
#define CELL_FS_ENOSYS        CELL_FS_ERROR_ENOSYS
#define CELL_FS_ENOTEMPTY     CELL_FS_ERROR_ENOTEMPTY
#define CELL_FS_ENOTSUP       CELL_FS_ERROR_ENOTSUP
#define CELL_FS_ETIMEDOUT     CELL_FS_ERROR_ETIMEDOUT
#define CELL_FS_EFSSPECIFIC   CELL_FS_ERROR_EFSSPECIFIC
#define CELL_FS_EOVERFLOW     CELL_FS_ERROR_EOVERFLOW
#define CELL_FS_ENOTMOUNTED   CELL_FS_ERROR_ENOTMOUNTED
#define CELL_FS_ENOTMSELF     CELL_FS_ERROR_ENOTMSELF
#define CELL_FS_ENOTSDATA     CELL_FS_ERROR_ENOTSDATA
#define CELL_FS_EAUTHFATAL    CELL_FS_ERROR_EAUTHFATAL

#pragma pack()
CDECL_END

#endif /* __LV2_FS_LOWFS_FS_EXTERNAL_H__ */
