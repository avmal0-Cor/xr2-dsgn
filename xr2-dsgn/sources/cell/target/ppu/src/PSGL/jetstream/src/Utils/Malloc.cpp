/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Memory management functions.
 */


#ifdef WIN32
#include <windows.h>
#else

#include <time.h>
#include <stdarg.h>		// For varargs
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for memalign
#ifdef JS_PLATFORM_LINUX
#include <malloc.h>
#elif defined(JS_PLATFORM_WIN32)
#define memalign(a,b) malloc(b)
#endif

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/Malloc.h>
#include <PSGL/private.h>

#ifdef E_MEMTRACE
#if OS_VERSION_NUMERIC < 0x83
#error EMEMTRACE only works on 0x83 or newer
#else
#include <sys/gpio.h>
#endif
#endif

/**
 *
 * Memory management functions
 *
 */

#ifdef E_MEMDEBUG
#include <PSGL/StackTrace.h>
#include <PSGL/Debug.h>

#define TRUE (1)
#define FALSE (0)

const char *EBlueCode = "\033[34m";
const char *ERedCode = "\033[31m";
const char *ENormalColorCode = "\033[0m";

char*	E_MemName = NULL;
int	E_MemVerbose = FALSE;

unsigned int	E_NumOfMemBlocks = 0, E_NumOfMemBlocksAllocated = 0;
EMemEntry*	E_MemBlocks = NULL;

void*		E_MemBlock = NULL;	// Block to track

// add guard data to each block
#define E_MEMDEBUG_GUARDWORDS 4	// number of ints
#define E_MEMDEBUG_GUARDVALUE 0xa4b3c2d1

#endif // E_MEMDEBUG


//----------------------------------------
// Memory management
//----------------------------------------

#ifdef E_MEMDEBUG
//========================================================
// Dump out information on unfreed memory
//========================================================
void EPrintUnfreedMemory()
{
    unsigned int	LC, LN, LTotalSize;

    LTotalSize = 0;
    if (( LN = E_NumOfMemBlocks ) != 0 )
    {
        printf( "Memory not freed:\n" );fflush( stdout );
        for ( LC = 0;LC < LN;LC++ )
        {
            if ( E_MemBlocks[LC].Active )
            {
                printf( " Address: %s%lx%s, Size: %s%7d%s, Name: %s%s%s\n", ERedCode, ( long )( E_MemBlocks[LC].Address ), ENormalColorCode, ERedCode, ( unsigned int )( E_MemBlocks[LC].Size ), ENormalColorCode, EBlueCode, E_MemBlocks[LC].Name, ENormalColorCode );fflush( stdout );
                LTotalSize += E_MemBlocks[LC].Size;
            }
        }
    }
}


//========================================================
// Add an entry to the MemoryTracker array
//========================================================
static void _EMemAddToList( void* LMem, size_t LSize, char* LName )
{
    EMemEntry*	LMemEntry = NULL;
    unsigned int	LC, LN;
    int		LFound;


// Look for Active=FALSE with the same address, because
// malloc might be reusing a previously freed block
//
    LFound = FALSE;
    LN = E_NumOfMemBlocks;

    if ( LN )
    {
        LMemEntry = E_MemBlocks;
        for ( LC = 0;LC < LN;LC++, LMemEntry++ )
        {
            if (( LMemEntry->Address == LMem ) && ( LMemEntry->Active == FALSE ) )
            {
                LFound = TRUE;
                break;
            }
        }
    }

    if ( !LFound )
    {
        JS_ASSERT( E_NumOfMemBlocks <= E_NumOfMemBlocksAllocated );
        if ( E_NumOfMemBlocks == E_NumOfMemBlocksAllocated )
        {
            E_MemBlocks = ( EMemEntry* )realloc( E_MemBlocks, sizeof( EMemEntry ) * ( E_NumOfMemBlocks + EMemALLOC_INCREMENT ) );
            E_NumOfMemBlocksAllocated += EMemALLOC_INCREMENT;
        }

        LMemEntry = E_MemBlocks + E_NumOfMemBlocks;
        E_NumOfMemBlocks += 1;
        LMemEntry->Address = LMem;
    }

    if ( LMemEntry )
    {
        if ( LName )
        {
            strncpy( LMemEntry->Name, LName, sizeof( LMemEntry->Name ) - 1 );
            LMemEntry->Name[sizeof( LMemEntry->Name )-1] = '\0';
        }
        else
            LMemEntry->Name[0] = '\0';

        LMemEntry->Size = LSize;
        LMemEntry->Active = TRUE;
    }
}

void _jsHeapValidate( void )
{
    // loop over active blocks
    for ( int i = 0; i < ( int )E_NumOfMemBlocks; ++i )
    {
        if ( E_MemBlocks[i].Active )
        {
            const void* ptr = E_MemBlocks[i].Address;
            const size_t size = E_MemBlocks[i].Size;

            // test guard bands
            const int* epi = ( const int* )(( char* )ptr + size );
            for ( int j = 0; j < E_MEMDEBUG_GUARDWORDS; ++j )
            {
                if ( epi[-j - 1] != ( int )E_MEMDEBUG_GUARDVALUE )
                {
                    printf( "Heap corrupted: block 0x%p %d epilogue\n",
                            ( char* )ptr + E_MEMDEBUG_GUARDWORDS*sizeof( int ), ( int )size );
                    JS_ASSERT( 0 && "mem block epilogue" );
                }
            }
        }
    }
}

#endif // E_MEMDEBUG


/**
 * Allocate memory
 *
 * This function allocates a block of memory. The size is given in
 * bytes by LSize.
 *
 * @param LSize           Size in bytes
 *
 * @return Pointer to the allocated memory block or NULL in case of an error.
 *
 * @sa jsRealloc, jsFree
 */


#ifdef JS_DPM_V2
extern unsigned int g_uiMallocCounter;
extern unsigned int g_uiMallocSize;
extern unsigned int g_uiMallocCounterPerFrame;
extern unsigned int g_uiMallocSizePerFrame;
#endif

static void* _jsMalloc( size_t LSize )
{
    _JS_PERFORMANCE_START( MallocFreeCounter );

#ifdef JS_DPM_V2
    g_uiMallocCounter++;
    g_uiMallocSize += LSize;
    g_uiMallocCounterPerFrame++;
    g_uiMallocSizePerFrame += LSize;
#endif

#ifdef E_MEMDEBUG
    _jsHeapValidate();
    // add pre and post guard band
    LSize = _jsPad( LSize, sizeof( int ) );
    LSize += E_MEMDEBUG_GUARDWORDS * sizeof( int );
#endif

    void* LMem = malloc( LSize );

#ifdef E_MEMTRACE
    uint64_t value = 0; sys_gpio_get( SYS_GPIO_DIP_SWITCH_DEVICE_ID, &value ); value &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS;
    if (( value & 0x8 ) > 0 ) { fprintf( stderr, "_jsMalloc(%i) returns %p\n", LSize, LMem ); fflush( stderr ); }
#endif

#ifdef E_MEMDEBUG
    _EMemAddToList( LMem, LSize, E_MemName );
    E_MemName = NULL;

    // fill memory with the guard value
    if ( LMem != NULL )
    {
        int* ptr = ( int* )LMem;
        for ( int i = 0; i < ( int )LSize; i += sizeof( int ) )
            * ptr++ = E_MEMDEBUG_GUARDVALUE;
    }

    _jsHeapValidate();
#endif // E_MEMDEBUG

    _JS_REPORT_EXTRA( PSGL_REPORT_MALLOC_MEMORY_ALLOC, "Allocating malloc memory: %d bytes allocated at %08p", LSize, LMem );
    _JS_PERFORMANCE_STOP( MallocFreeCounter );
    return( LMem );
}

/**
 * Allocate memory
 *
 * This function allocates a block of aligned memory. The alignement is given in bytes by align.
 * The size is given in bytes by LSize.
 *
 * @param align           alignment in bytes. Must be a power of 2
 * @param LSize           Size in bytes
 *
 * @return Pointer to the allocated memory block or NULL in case of an error.
 *
 * @sa jsRealloc, jsFree
 */
static void* _jsMemalign( size_t align, size_t LSize )
{
    _JS_PERFORMANCE_START( MallocFreeCounter );

#ifdef E_MEMDEBUG
    _jsHeapValidate();
    // add pre and post guard band
    LSize = _jsPad( LSize, sizeof( int ) );
    LSize += E_MEMDEBUG_GUARDWORDS * sizeof( int );
#endif

    void* LMem = memalign( align, LSize );

#ifdef E_MEMTRACE
    uint64_t value = 0; sys_gpio_get( SYS_GPIO_DIP_SWITCH_DEVICE_ID, &value ); value &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS;
    if (( value & 0x8 ) > 0 ) { fprintf( stderr, "_jsMemalign(%i,%i) returns %p\n", align, LSize, LMem ); fflush( stderr ); }
#endif

#ifdef E_MEMDEBUG
    _EMemAddToList( LMem, LSize, E_MemName );
    E_MemName = NULL;

    // fill memory with the guard value
    if ( LMem != NULL )
    {
        int* ptr = ( int* )LMem;
        for ( int i = 0; i < ( int )LSize; i += sizeof( int ) )
            * ptr++ = E_MEMDEBUG_GUARDVALUE;
    }

    _jsHeapValidate();
#endif // E_MEMDEBUG

    _JS_REPORT_EXTRA( PSGL_REPORT_MALLOC_MEMORY_ALLOC, "Allocating malloc memory (aligned at %d bytes): %d bytes allocated at %08p", align, LSize, LMem );
    _JS_PERFORMANCE_STOP( MallocFreeCounter );
    return( LMem );
}


/**
 * Reallocate memory
 *
 * This function re-allocates a block of memory. The new size is
 * given in bytes by LSize.
 * If the starting address of the new block differs from the old,
 * jsRealloc will copy the overlapping area over to the new address.
 *
 * @param LOldMem          Old buffer
 * @param LNewSize         Size in bytes
 *
 * @return Pointer to the re-allocated memory block or NULL in case of an error.
 *
 * @sa jsMalloc, jsFree
 */
static void* _jsRealloc( void* LOldMem, size_t LNewSize )
{
    _JS_PERFORMANCE_START( MallocFreeCounter );
#ifdef E_MEMDEBUG
    _jsHeapValidate();

    // add pre and post guard band
    LNewSize = _jsPad( LNewSize, sizeof( int ) );
    LNewSize += E_MEMDEBUG_GUARDWORDS * sizeof( int );
    size_t LOldSize = 0;
#endif

    void*	LNewMem;
    LNewMem = realloc( LOldMem, LNewSize );
#ifdef E_MEMTRACE
    uint64_t value = 0; sys_gpio_get( SYS_GPIO_DIP_SWITCH_DEVICE_ID, &value ); value &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS;
    if (( value & 0x8 ) > 0 ) { fprintf( stderr, "_jsRealloc(%p,%i) returns %p\n", LOldMem, LNewSize, LNewMem ); fflush( stderr ); }
#endif

#ifdef E_MEMDEBUG

    if ( LOldMem )
    {
        EMemEntry*	LMemEntry;
        EMemEntry*	LOldMemEntry;
        EMemEntry*	LNewMemEntry;
        unsigned int	LC, LN;

        LN = E_NumOfMemBlocks;


// If the new address differs from the old one, mark the old one freed
//
        LOldMemEntry = NULL;
        LMemEntry = E_MemBlocks;
        for ( LC = 0;LC < LN;LC++, LMemEntry++ )
        {
            if ( LMemEntry->Address == LOldMem )
            {
                LOldMemEntry = LMemEntry;
                break;
            }
        }

        if ( LOldMemEntry == NULL )
        {
// If MemoryTracker was reset by the user, this
// could happen, even if everything is in order!!!
//
            printf( "jsRealloc: This block was not allocated with jsMalloc or jsRealloc: %lx!!!!\n", ( long )LOldMem );fflush( stdout );
//   JS_ASSERT(0);
        }
        else
        {
            if ( !LOldMemEntry->Active )
            {
                printf( "jsRealloc: This block was already freed once: %lx %d [%s]!!!!\n", ( long )( LOldMemEntry->Address ), ( unsigned int )( LOldMemEntry->Size ), LOldMemEntry->Name );fflush( stdout );
                JS_ASSERT( 0 );
            }
            if ( LNewMem != LOldMem )
            {
                if ( E_MemVerbose ) printf( "jsRealloc freed %lx\n", ( long )LOldMemEntry );fflush( stdout );
                LOldMemEntry->Active = FALSE;
            }

            LOldSize = LOldMemEntry->Size;
        }

        LNewMemEntry = NULL;
        LMemEntry = E_MemBlocks;
        for ( LC = 0;LC < LN;LC++, LMemEntry++ )
        {
            if ( LMemEntry->Address == LNewMem )
            {
                LNewMemEntry = LMemEntry;
                break;
            }
        }

        if ( LNewMemEntry )
        {
            if (( LNewMem != LOldMem ) && ( LNewMemEntry->Active ) )
            {
                printf( "jsRealloc: This block is supposedly still in use: %lx %d [%s]!!!!\n", ( long )( LNewMemEntry->Address ), ( unsigned int )( LNewMemEntry->Size ), LNewMemEntry->Name );fflush( stdout );
                JS_ASSERT( 0 );
            }
        }
        else
        {
            JS_ASSERT( E_NumOfMemBlocks <= E_NumOfMemBlocksAllocated );
            if ( E_NumOfMemBlocks == E_NumOfMemBlocksAllocated )
            {
                E_MemBlocks = ( EMemEntry* )realloc( E_MemBlocks, sizeof( EMemEntry ) * ( E_NumOfMemBlocks + EMemALLOC_INCREMENT ) );
                E_NumOfMemBlocksAllocated += EMemALLOC_INCREMENT;
            }

            LNewMemEntry = E_MemBlocks + E_NumOfMemBlocks;
            E_NumOfMemBlocks += 1;
        }
        LNewMemEntry->Size = LNewSize;
        LNewMemEntry->Address = LNewMem;
        LNewMemEntry->Active = TRUE;

        if ( E_MemName )
        {
            strncpy( LNewMemEntry->Name, E_MemName, sizeof( LNewMemEntry->Name ) - 1 );
            LNewMemEntry->Name[sizeof( LNewMemEntry->Name )-1] = '\0';
        }
        else
            LNewMemEntry->Name[0] = '\0';
    }
    else
    {
        _EMemAddToList( LNewMem, LNewSize, E_MemName );
    }
    E_MemName = NULL;

    // fill memory with guard value
    if ( LNewMem != NULL )
    {
        int fromSize = MIN( LOldSize, LNewSize );
        int toSize   = MAX( LOldSize, LNewSize );

        int* ptr = ( int* )(( char* )LNewMem + fromSize );
        for ( int i = fromSize; i < toSize; i += sizeof( int ) )
            * ptr++ = E_MEMDEBUG_GUARDVALUE;
    }

    _jsHeapValidate();
#endif // E_MEMDEBUG
    _JS_REPORT_EXTRA( PSGL_REPORT_MALLOC_MEMORY_REALLOC, "Reallocating malloc memory at %08p: %d bytes allocated at %08p", LOldMem, LNewSize, LNewMem );
    _JS_PERFORMANCE_STOP( MallocFreeCounter );
    return( LNewMem );
}


/**
 * Free allocated memory block
 *
 * This function frees memory allocated by jsMalloc or jsRealloc.
 *
 * @param LMem	           Buffer to free
 *
 * @sa jsMalloc, jsRealloc
 */
static void _jsFree( void* LMem )
{
    _JS_PERFORMANCE_START( MallocFreeCounter );
    if ( LMem == NULL ) return;
#ifdef E_MEMDEBUG
    {
        _jsHeapValidate();

        EMemEntry*	LMemEntry;
        unsigned int	LC, LN;
        int		LFound;


        if ( E_MemBlock ) if ( LMem == E_MemBlock ) JS_ASSERT( 0 );

        LFound = FALSE;
        LN = E_NumOfMemBlocks;
        LMemEntry = E_MemBlocks;

        for ( LC = 0;LC < LN;LC++, LMemEntry++ )
        {
            if ( LMemEntry->Address == LMem )
            {
                if ( LMemEntry->Active )
                {
                    // fill with garbage
                    int* ptr = ( int* )LMemEntry->Address;
                    for ( int i = 0; i < ( int )LMemEntry->Size; i += sizeof( int ) )
                        * ptr++ = E_MEMDEBUG_GUARDVALUE;

                    LMemEntry->Active = FALSE;
                }
                else
                {
                    printf( "jsFree: This block was already freed once: %lx %d [%s]!!!!\n", ( long )( LMem ), ( unsigned int )( LMemEntry->Size ), LMemEntry->Name );fflush( stdout );
                    JS_ASSERT( 0 );
                }
                LFound = TRUE;
                break;
            }
        }
        if ( !LFound )
        {
            printf( "jsFree: This block was not allocated with jsMalloc or jsRealloc: %lx!!!!\n", ( long )LMem );fflush( stdout );
            JS_ASSERT( 0 );
        }

    }

#endif // E_MEMDEBUG
#ifdef E_MEMTRACE
    uint64_t value = 0; sys_gpio_get( SYS_GPIO_DIP_SWITCH_DEVICE_ID, &value ); value &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS;
    if (( value & 0x8 ) > 0 ) { fprintf( stderr, "_jsFree(%p)\n", LMem );  fflush( stderr ); }
#endif

    free( LMem );
    _JS_REPORT_EXTRA( PSGL_REPORT_MALLOC_MEMORY_FREE, "Freeing malloc memory at %08p", LMem );
    _JS_PERFORMANCE_STOP( MallocFreeCounter );
}

char *jsStrdup( const char *string )
{
    char *newString = ( char * )jsMalloc( strlen( string ) + 1 );
    if ( newString ) strcpy( newString, string );
    return newString;
}

///////////////////////////////////////////////////////////////////////////

PSGL_EXPORT PSGLmallocFunc		jsMallocFunc	= _jsMalloc;
PSGL_EXPORT PSGLmemalignFunc	jsMemalignFunc	= _jsMemalign;
PSGL_EXPORT PSGLreallocFunc		jsReallocFunc	= _jsRealloc;
PSGL_EXPORT PSGLfreeFunc		jsFreeFunc		= _jsFree;

void _jsResetAllocatorFuncs( void )
{
    jsMallocFunc	= _jsMalloc;
    jsMemalignFunc	= _jsMemalign;
    jsReallocFunc	= _jsRealloc;
    jsFreeFunc		= _jsFree;
}

#ifdef E_MEMDEBUG
static char allocTrackedName[1024];
void* _jsMallocTracked( size_t LSize, const char *filename, const char *funcname, int line )
{
    sprintf( allocTrackedName, "%s:%i in function %s", filename, line, funcname );
    char buffer[512];
    _jsGetStackTrace( 2, 8, buffer, sizeof( buffer ) );//start at depth -1 and goes up for 5 functions
    strcat( allocTrackedName, "\n" );
    strcat( allocTrackedName, buffer );
    E_MemName = allocTrackedName;
    return jsMallocFunc( LSize );
}

void* _jsMemalignTracked( size_t align, size_t LSize, const char *filename, const char *funcname, int line )
{
    sprintf( allocTrackedName, "%s:%i in function %s", filename, line, funcname );
    char buffer[512];
    _jsGetStackTrace( 2, 8, buffer, sizeof( buffer ) );//start at depth -1 and goes up for 5 functions
    strcat( allocTrackedName, "\n" );
    strcat( allocTrackedName, buffer );
    E_MemName = allocTrackedName;
    return jsMemalignFunc( align, LSize );;
}

void* _jsReallocTracked( void* LOldMem, size_t LNewSize, const char *filename, const char *funcname, int line )
{
    sprintf( allocTrackedName, "%s:%i in function %s", filename, line, funcname );
    char buffer[512];
    _jsGetStackTrace( 2, 8, buffer, sizeof( buffer ) );//start at depth -1 and goes up for 5 functions
    strcat( allocTrackedName, "\n" );
    strcat( allocTrackedName, buffer );
    E_MemName = allocTrackedName;
    return jsReallocFunc( LOldMem, LNewSize );
}
#endif

// make sure every allocation goes through jsMalloc and jsFree

// we define those globally, but we will "localize" the symbols at link time, so that we don't pollute other people's new

// regular new
void * operator new( size_t size )
{
    return jsMalloc( size );
}

void * operator new[]( size_t size )
{
    return operator new( size );
}

void operator delete( void *ptr )
{
    jsFree( ptr );
}

void operator delete[]( void *ptr )
{
    return operator delete( ptr );
}

// aligned new
void * operator new( size_t size, size_t align )
{
    return jsMemalign( align, size );
}

void * operator new[]( size_t size, size_t align )
{
    return operator new( size,align );
}

void operator delete( void *ptr, size_t align )
{
    return operator delete( ptr );
}

void operator delete[]( void *ptr, size_t align )
{
    return operator delete( ptr,align );
}

/* end of Memory Management  */
