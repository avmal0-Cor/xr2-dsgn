////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.11.2008
//	Author		: Lain
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VFS_PACK_MEMORY_H_INCLUDED
#define VFS_PACK_MEMORY_H_INCLUDED

#include <xray/buffer_vector.h>
#include <xray/fixed_vector.h>
#include <xray/associative_vector.h>
#include <xray/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

namespace vfs_pack {

extern	xray::memory::doug_lea_allocator_type	g_vfs_pack_allocator;

#define USER_ALLOCATOR							g_vfs_pack_allocator
#include <xray/std_containers.h>
#undef USER_ALLOCATOR

} // namespace vfs_pack 

#define NEW( type )								XRAY_NEW_IMPL(		g_vfs_pack_allocator, type )
#define DELETE( pointer )						XRAY_DELETE_IMPL(	g_vfs_pack_allocator, pointer )
#define MALLOC( size, description )				XRAY_MALLOC_IMPL(	g_vfs_pack_allocator, size, description )
#define REALLOC( pointer, size, description )	XRAY_REALLOC_IMPL(	g_vfs_pack_allocator, pointer, size, description )
#define FREE( pointer )							XRAY_FREE_IMPL(		g_vfs_pack_allocator, pointer )
#define ALLOC( type, count )					XRAY_ALLOC_IMPL(	g_vfs_pack_allocator, type, count )

#endif // #ifndef vfs_pack_MEMORY_H_INCLUDED