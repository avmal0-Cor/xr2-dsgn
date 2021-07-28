/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _UNISTD_H
#define _UNISTD_H

#include <yvals.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>

_C_STD_BEGIN
_C_LIB_DECL
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
off_t   lseek(int, off_t, int);
off64_t lseek64(int, off64_t, int);
int     close(int);
int     unlink(const char *path);
int     link(const char *, const char *);
int     __rename(const char *from, const char *to);
int     stat(const char *path, struct stat *buf);
int     fstat(int fd, struct stat *buf);
int     truncate(const char *path, off_t length);
int     truncate64(const char *path, off64_t length);
int     ftruncate(int fd, off_t length);
int     ftruncate64(int fd, off64_t length);
int     rmdir(const char *path);
int     fsync(int fd);
int     getargc(void);
char    **getargv(void);

typedef struct {
	const char     magic[5];	/*  5 bytes : '\177'GUID              */
	const uint8_t  id_revision;	/*  1 byte  : PPU GUID revision (0x1) */
	const uint16_t reserved1;	/*  2 bytes : Reserved (0x0)          */
	const uint32_t ppu_guid[5];	/* 20 bytes : PPU GUID                */
	const uint32_t reserved2;	/*  4 bytes : Reserved (0x0)          */
} ppu_guid_info_t;

_END_C_LIB_DECL
_C_STD_END

#if defined(_STD_USING)
using _CSTD read;
using _CSTD write;
using _CSTD lseek;
using _CSTD lseek64;
using _CSTD close;
using _CSTD unlink;
using _CSTD link;
using _CSTD __rename;
using _CSTD stat;
using _CSTD fstat;
using _CSTD truncate;
using _CSTD truncate64;
using _CSTD ftruncate;
using _CSTD ftruncate64;
using _CSTD rmdir;
using _CSTD fsync;
using _CSTD getargc;
using _CSTD getargv;
using _CSTD ppu_guid_info_t;
#endif

#endif /* _UNISTD_H */
