/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Internal malloc handlers and debug helpers.
 */


#ifndef _JSMalloc_h
#define _JSMalloc_h

#include <string.h>


#include <PSGL/export.h>
#include <PSGL/psgl.h>

#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
extern "C"
{
#endif

//#define E_MEMDEBUG	1	// Do memory profiling

#ifdef E_MEMDEBUG
    extern char*	E_MemName;
    extern int	E_MemVerbose;


#define	EMemALLOC_INCREMENT	4096	// For E_MemBlocks

    typedef struct
    {
        char		Name[256];
        unsigned long	Size;
        void*		Address;
        int		Active;
    }
    EMemEntry;

    extern unsigned int	E_NumOfMemBlocks, E_NumOfMemBlocksAllocated;
    extern EMemEntry*	E_MemBlocks;
    extern void*		E_MemBlock;

    extern void	EPrintUnfreedMemory();

    extern void _jsHeapValidate( void );

    void* _jsMallocTracked( size_t LSize, const char *filename, const char *funcname, int line );
    void* _jsMemalignTracked( size_t align, size_t LSize, const char *filename, const char *funcname, int line );
    void* _jsReallocTracked( void* LOldMem, size_t LNewSize, const char *filename, const char *funcname, int line );

#define jsMalloc(LSize) _jsMallocTracked(LSize,__FILE__,__FUNCTION__,__LINE__)
#define jsMemalign(align,LSize) _jsMemalignTracked(align,LSize,__FILE__,__FUNCTION__,__LINE__)
#define jsRealloc(LOldMem,LNewSize) _jsReallocTracked(LOldMem,LNewSize,__FILE__,__FUNCTION__,__LINE__)
#define jsFree jsFreeFunc

#else // E_MEMDEBUG
    static inline void _jsHeapValidate( void ) {}

#define jsMalloc jsMallocFunc
#define jsMemalign jsMemalignFunc
#define jsRealloc jsReallocFunc
#define jsFree jsFreeFunc

#endif // E_MEMDEBUG

// old js.h - exposed for jstest
    extern PSGL_EXPORT PSGLmallocFunc      jsMallocFunc;
    extern PSGL_EXPORT PSGLmemalignFunc    jsMemalignFunc;
    extern PSGL_EXPORT PSGLreallocFunc     jsReallocFunc;
    extern PSGL_EXPORT PSGLfreeFunc        jsFreeFunc;
    PSGL_EXPORT char* jsStrdup( const char *string );


#if defined (__cplusplus) && (!(defined (__CPLUSLIBS)))
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif	// _JSMalloc_h
