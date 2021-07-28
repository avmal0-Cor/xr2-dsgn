/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
/* mspace.h : prototypes of mspace_xxx() */
#ifndef _MSPACE
#define _MSPACE
#ifndef _YVALS
#include <yvals.h>
#endif /* _YVALS */
#include <sys/types.h>
#include <stdlib.h> /* struct malloc_managed_size */

_C_STD_BEGIN
_C_LIB_DECL

typedef void* mspace;

/* create and destroy mspace */
mspace mspace_create(void *, size_t);
int    mspace_destroy(mspace);

/* allocator and deallocator */
void*  mspace_malloc(mspace, size_t);
void   mspace_free(mspace, void *);
void*  mspace_calloc(mspace, size_t, size_t);
void*  mspace_memalign(mspace, size_t, size_t);
void*  mspace_realloc(mspace, void *, size_t);
void*  mspace_reallocalign(mspace, void *, size_t, size_t);

/* utility */
size_t mspace_malloc_usable_size(void *mem);
int    mspace_malloc_stats(mspace, struct malloc_managed_size*);
int    mspace_is_heap_empty(mspace);

_END_C_LIB_DECL
_C_STD_END
#endif /* _MSPACE */

#if defined(_STD_USING)
using _CSTD mspace;
using _CSTD mspace_create;
using _CSTD mspace_destroy;
using _CSTD mspace_malloc;
using _CSTD mspace_free;
using _CSTD mspace_realloc;
using _CSTD mspace_reallocalign;
using _CSTD mspace_calloc;
using _CSTD mspace_memalign;
using _CSTD mspace_malloc_usable_size;
using _CSTD mspace_malloc_stats;
using _CSTD mspace_is_heap_empty;

#endif /* defined(_STD_USING) */
