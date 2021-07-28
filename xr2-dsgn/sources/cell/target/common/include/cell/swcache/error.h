/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_ERROR_H__
#define __CELL_SWCACHE_ERROR_H__

#include <cellstatus.h>
#include <cell/error.h>

#define CELL_ERROR_MINOR_FACILITY_SWCACHE                         0xb 

#define CELL_ERROR_MAKE_SWCACHE_ERROR(id) \
	(CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_SWCACHE          << 8) | (id)))

#define CELL_SWCACHE_ERROR_AGAIN                                  CELL_ERROR_CAST(0x80410b01)
#define CELL_SWCACHE_ERROR_INVAL                                  CELL_ERROR_CAST(0x80410b02)
#define CELL_SWCACHE_ERROR_NOSYS                                  CELL_ERROR_CAST(0x80410b03)
#define CELL_SWCACHE_ERROR_NOMEM                                  CELL_ERROR_CAST(0x80410b04)
#define CELL_SWCACHE_ERROR_SRCH                                   CELL_ERROR_CAST(0x80410b05)
#define CELL_SWCACHE_ERROR_NOENT                                  CELL_ERROR_CAST(0x80410b06)
#define CELL_SWCACHE_ERROR_NOEXEC                                 CELL_ERROR_CAST(0x80410b07)
#define CELL_SWCACHE_ERROR_DEADLK                                 CELL_ERROR_CAST(0x80410b08)
#define CELL_SWCACHE_ERROR_PERM                                   CELL_ERROR_CAST(0x80410b09)
#define CELL_SWCACHE_ERROR_BUSY                                   CELL_ERROR_CAST(0x80410b0A)
#define CELL_SWCACHE_ERROR_ABORT                                  CELL_ERROR_CAST(0x80410b0C)
#define CELL_SWCACHE_ERROR_FAULT                                  CELL_ERROR_CAST(0x80410b0D)
#define CELL_SWCACHE_ERROR_CHILD                                  CELL_ERROR_CAST(0x80410b0E)
#define CELL_SWCACHE_ERROR_STAT                                   CELL_ERROR_CAST(0x80410b0F)
#define CELL_SWCACHE_ERROR_ALIGN                                  CELL_ERROR_CAST(0x80410b10)
#define CELL_SWCACHE_ERROR_NULL_POINTER                           CELL_ERROR_CAST(0x80410b11)
#define CELL_SWCACHE_ERROR_NON_CACHED_POINTER                     CELL_ERROR_CAST(0x80410b12)
#define CELL_SWCACHE_ERROR_NO_HTAB                                CELL_ERROR_CAST(0x80410b13)
#define CELL_SWCACHE_ERROR_MEM_LEAKED                             CELL_ERROR_CAST(0x80410b14)
#define CELL_SWCACHE_ERROR_VTAB                                   CELL_ERROR_CAST(0x80410b15)

#endif /* __CELL_SWCACHE_ERROR_H__ */
