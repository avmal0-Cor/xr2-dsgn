/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Memory management functions.
 */


#include <stdio.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>

// block buffer alignment
//  This is *not* the alignment of all returned elements, only the alignment
//  of the first element in each block.  Whether other allocations are
//  aligned depends on the element size.
#define _JS_ALLOC_ALIGN 16

#ifdef TEST_FIXED_ALLOC
// reduce outside dependencies for testing
#include <assert.h>
#include <malloc.h>
#define jsMalloc malloc
#define jsFree free
#define jsRealloc realloc
#define _jsSetError(X)
#undef JS_ASSERT
#define JS_ASSERT assert
#endif

struct jsFixedAllocator
{
    GLuint elementBytes;	// fixed size allocation

    // free list
    GLuint* freeList;	// array of free indices
    GLuint freeCount;	// number of free indices
    GLuint freeListSize;

    // blocks of fixed size elements
    char** block;		// array of elements
    GLuint blockCount;	// number of blocks
    GLuint blockSize;	// number of elements in each block
};

jsFixedAllocator* _jsFixedAllocatorCreate(
    GLuint elementBytes,
    GLuint initialCount )
{
    JS_ASSERT( elementBytes > 0 );
    JS_ASSERT( initialCount > 0 );

    // allocate the allocator struct and the first block
    const GLuint allocSize =
        sizeof( jsFixedAllocator ) + _JS_ALLOC_ALIGN + elementBytes * initialCount;
    jsFixedAllocator* a = ( jsFixedAllocator* )jsMalloc( allocSize );
    if ( a == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }

    a->elementBytes = elementBytes;
    a->freeList = NULL;
    a->freeCount = initialCount;
    a->block = NULL;
    a->blockCount = 1;
    a->blockSize = initialCount;

    // initialize first block
    a->block = ( char** )jsMalloc( sizeof( char* ) );
    if ( a->block == NULL )
    {
        _jsFixedAllocatorDestroy( a );
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
    a->block[0] = _jsPadPtr(( char* )a + sizeof( jsFixedAllocator ), _JS_ALLOC_ALIGN );

    // initialize free list
    a->freeList = ( GLuint * )jsMalloc( initialCount * sizeof( GLuint ) );
    if ( a->freeList == NULL )
    {
        _jsFixedAllocatorDestroy( a );
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
    a->freeListSize = initialCount;
    for ( GLuint i = 0; i < initialCount; ++i )
        a->freeList[i] = initialCount - 1 - i;

    return a;
}

void _jsFixedAllocatorDestroy( jsFixedAllocator* a )
{
    JS_ASSERT( a != NULL );

    // XXX Should we issue a warning if all the elements aren't freed?

    // free blocks
    //  The first block is allocated with the allocator struct.
    for ( GLuint i = 1; i < a->blockCount; ++i )
    {
        if ( a->block[i] != NULL )
            jsFree( a->block[i] );
        a->block[i] = NULL;
    }
    if ( a->block != NULL )
        jsFree( a->block );
    a->block = NULL;

    if ( a->freeList != NULL )
        jsFree( a->freeList );
    jsFree( a );
}


int _jsFixedAllocatorSave( jsFixedAllocator * a, void * saveArea, unsigned long * offset, unsigned long offsetMax )
{
    JS_ASSERT( a );
    JS_ASSERT( saveArea );
    JS_ASSERT( offset );
    JS_ASSERT( *offset < offsetMax );

    // save allocator body

    if ( *offset + sizeof( jsFixedAllocator ) > offsetMax )	return -1;
    memcpy(( char * )saveArea + *offset, a, sizeof( jsFixedAllocator ) );
    *offset	+= sizeof( jsFixedAllocator );

    // save block list

    int	size	= ( a->blockCount - 1 ) * sizeof( char * );
    if ( *offset + size > offsetMax )	return -1;
    memcpy(( char * )saveArea + *offset, a->block + 1, size );
    *offset	+= size;

    // save block contents

    for ( GLuint i = 0; i < a->blockCount; ++i )
    {
        if ( a->block[i] != NULL )
        {
            if ( *offset + a->blockSize > offsetMax )	return -1;
            memcpy(( char * )saveArea + *offset, a->block[i], a->blockSize );
            *offset	+= a->blockSize;
        }
    }	// for

    // save freelist

    size = sizeof( GLuint ) * a->freeListSize;
    if ( *offset + size > offsetMax )	return -1;
    memcpy(( char * )saveArea + *offset, a->freeList, size );
    *offset	+= size;

    return 0;
}


void _jsFixedAllocatorRestore( jsFixedAllocator * a, void * saveArea, unsigned long * offset )
{
    JS_ASSERT( a );
    JS_ASSERT( saveArea );
    JS_ASSERT( offset );

    // assume a is a newly created allocator

    char * block0	= a->block[0];
    jsFree( a->block );
    jsFree( a->freeList );

    // restore the allocator body

    memcpy(( char * )a, ( char * )saveArea + *offset, sizeof( jsFixedAllocator ) );
    *offset	+= sizeof( jsFixedAllocator );

    // restore block list, to be overwritten

    a->block = ( char ** )jsMalloc( a->blockCount * sizeof( char * ) );
    JS_ASSERT( a->block );

    int	size	= ( a->blockCount - 1 ) * sizeof( char * );
    memcpy(( char ** )a->block + 1, ( char * )saveArea + *offset, size );
    *offset	+= size;

    // restore each block

    memcpy( block0, ( char * )saveArea + *offset, a->blockSize );
    *offset += a->blockSize;
    a->block[0] = block0;

    for ( GLuint i = 1; i < a->blockCount; ++i )
    {
        if ( a->block[i] != NULL )
        {
            a->block[i] = ( char * )jsMalloc( a->blockSize );
            JS_ASSERT( a->block[i] );
            memcpy( a->block[i], ( char * )saveArea + *offset, a->blockSize );
            *offset += a->blockSize;
        }
    }	// for

    // restore freelist
    size = sizeof( GLuint ) * a->freeListSize;
    a->freeList	= ( GLuint * )jsMalloc( size );
    JS_ASSERT( a->freeList );
    memcpy(( char * )a->freeList, ( char * )saveArea + *offset, size );
    *offset	+= size;
}



void* _jsFixedAllocatorAlloc( jsFixedAllocator* a )
{
    JS_ASSERT( a != NULL );

    // check if an element is available
    if ( a->freeCount )
    {
        // The (decremented) count can be used to determine the block id.
        const GLuint id = a->freeList[--a->freeCount];

        // The id encodes the block and index of the free element.
        char* p = _jsPadPtr( a->block[id/a->blockSize], _JS_ALLOC_ALIGN ) +
                  ( id % a->blockSize ) * a->elementBytes;
        return p;
    }

    // no element available, need to allocate more
#ifdef JS_DEBUG
    // fprintf(stderr,"perf warning: jsFixedAllocator allocating another block\n");
#endif

    char** newBlockList = ( char** )jsRealloc( a->block, ( a->blockCount + 1 ) * sizeof( char* ) );
    if ( newBlockList == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
    a->block = newBlockList;

    char* newBlock = ( char* )jsMalloc( a->blockSize * a->elementBytes + _JS_ALLOC_ALIGN );
    if ( newBlock == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }

    int newFreeListSize = a->blockCount + 1;
    GLuint* newFreeList = ( GLuint* )jsRealloc(
                              a->freeList,
                              newFreeListSize * a->blockSize * sizeof( GLuint ) );
    if ( newFreeList == NULL )
    {
        jsFree( newBlock );
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }

    // commit changes
    a->freeList = newFreeList;
    const GLuint prevElements = a->blockCount * a->blockSize;
    for ( GLuint i = 0; i < a->blockSize; ++i )
        a->freeList[i] = prevElements + i;
    a->freeCount += a->blockSize;
    a->freeListSize = newFreeListSize;
    a->block[a->blockCount] = newBlock;
    a->blockCount += 1;

    // try again
    return _jsFixedAllocatorAlloc( a );
}


void _jsLocateBlockElement( jsFixedAllocator* a, void* p, int * blockIndexResult, int * elementIndexResult )
{
    int blockIndex;
    int elementIndex = -1;
    for ( blockIndex = 0; blockIndex < ( int )a->blockCount; ++blockIndex )
    {
        const size_t offset = ( GLbyte* )p - ( GLbyte* )_jsPadPtr( a->block[blockIndex], _JS_ALLOC_ALIGN );
        unsigned	maxOffset = a->blockSize * a->elementBytes;
        if ( offset >= 0 && offset < maxOffset )
        {
            elementIndex = offset / a->elementBytes;
            break;
        }
    }
    blockIndex	= ( elementIndex == -1 ) ? -1 : blockIndex;

    *blockIndexResult	= blockIndex;
    *elementIndexResult	= elementIndex;
}


char * _jsPtrToBlockElement( int blockIndex, int elementIndex, jsFixedAllocator * a )
{
    JS_ASSERT( blockIndex != -1 );
    JS_ASSERT( elementIndex != -1 );
    char * result;
    result	= a->block[blockIndex];
    int offset	= elementIndex * a->elementBytes;
    result	+= offset;
    return result;
}


void _jsFixedAllocatorFree( jsFixedAllocator* a, void* p )
{
    JS_ASSERT( p != NULL );

    // determine block and element
    //  This linear search through the blocks is why it is advised to size
    //  the initial allocation large enough so that only one block is used.
    int blockIndex;
    int elementIndex = -1;
    _jsLocateBlockElement( a, p, &blockIndex, &elementIndex );
    JS_ASSERT( elementIndex >= 0 );

    // add to the free list
    const int id = blockIndex * a->blockSize + elementIndex;
    JS_ASSERT( a->freeCount < ( a->freeListSize * a->blockSize ) );
    a->freeList[a->freeCount] = id;
    ++a->freeCount;
}

#ifdef TEST_FIXED_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE	256
#define MAX_NUMBER	64
#define MAX_ALLOCS	1024

int main()
{
    for ( int trial = 0; trial < 64; ++trial )
    {
        const GLuint size = ( rand() % MAX_SIZE ) + 1;
        const GLuint count = ( rand() % MAX_NUMBER ) + 1;
        printf( "%d bytes, prealloc %d...", size, count );

        // create allocator
        jsFixedAllocator* a = _jsFixedAllocatorCreate( size, count );

        void* p[MAX_ALLOCS];
        memset( p, 0, sizeof( p ) );

        // randomly allocate and deallocate
        GLuint curAlloc = 0;
        GLuint maxAlloc = 0;
        for ( int i = 0; i < 32*MAX_ALLOCS; ++i )
        {
            const GLuint index = rand() % MAX_ALLOCS;
            if ( p[index] == NULL )
            {
                // allocate
                p[index] = _jsFixedAllocatorAlloc( a );

                // fill with low byte of index
                memset( p[index], index & 0xff, size );

                ++curAlloc;
                if ( curAlloc > maxAlloc )
                    maxAlloc = curAlloc;

                // verify no duplicate pointers
                for ( int j = 0; j < MAX_ALLOCS; ++j )
                {
                    if ( j != index )
                    {
                        JS_ASSERT( p[index] != p[j] );
                    }
                }
            }
            else
            {
                // deallocate
                _jsFixedAllocatorFree( a, p[index] );
                p[index] = NULL;

                --curAlloc;
            }

            // test contents
            for ( int j = 0; j < MAX_ALLOCS; ++j )
            {
                unsigned char* q = ( unsigned char* )p[j];
                if ( q != NULL )
                {
                    for ( int k = 0; k < size; ++k )
                    {
                        JS_ASSERT( q[k] == ( j & 0xff ) );
                    }
                }
            }
        }

        printf( "max %d allocations\n", maxAlloc );

        // destroy allocator
        _jsFixedAllocatorDestroy( a );
    }
    return 0;
}

#endif // TEST_FIXED_ALLOC
