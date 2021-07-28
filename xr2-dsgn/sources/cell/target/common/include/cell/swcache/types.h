/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SWCACHE_TYPES_H__
#define __CELL_SWCACHE_TYPES_H__

#include <cell/swcache/define.h>

__CELL_SWCACHE_BEGIN

const static unsigned int IS_LOADING_SA  = 18;
const static unsigned int IS_WRITABLE_SA = 19;
const static unsigned int REFCOUNT_SA    = 20;
const static unsigned int WRITEBACK_KICKED_SA = 5;

//! Cache query result about object
enum ObjectAttr
{
	CACHED           = 1,     //!< Object is cached
	NOT_CACHED       = 2,     //!< Object is not cached
	DMA_COMPLETED    = 3,     //!< Object fetch DMA is just completed
	DMA_RUNNING      = 4,     //!< Object fetch DMA is still running
	WRITEBACK_KICKED = 1 << WRITEBACK_KICKED_SA, //!< Object writeback DMA is just kicked
	IS_LOADING       = 1 << IS_LOADING_SA,       //!< Object is now loading from main memory
	IS_WRITABLE      = 1 << IS_WRITABLE_SA,      //!< Object is flagged as writable
};

enum PatchMode
{
	RO_MODE, //!< read-only
	RW_MODE  //!< writable
};

enum ClassAttribute
{
	NO_VTABLE, //!< Class without VTABLE
	HAS_VTABLE //!< Class with VTABLE
};

__CELL_SWCACHE_END

#endif /* __CELL_SWCACHE_CACHE_TYPES_H__ */
