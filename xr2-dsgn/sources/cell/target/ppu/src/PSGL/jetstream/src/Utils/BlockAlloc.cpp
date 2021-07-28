/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Memory management functions.
 */

#include <PSGL/psgl.h>
#include <PSGL/Utils.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define ALLOC_BIT (1U<<31)
#define IS_ALLOC(X) ((X)&ALLOC_BIT)
#define BLOCK_SIZE(X) ((X)&~ALLOC_BIT)

static const GLuint minAlign = sizeof( void * );
static const GLuint defaultBookAllocSize = 16;
static const GLint maxBookAllocIncr = 512;

static inline int _jsMemoryBlockManagerBookCheckAlloc( MemoryBlockManager_t *block, GLint size )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    JS_ASSERT( block->bookAlloc > 0 );
    if ( block->bookAlloc >= size ) return 0;

    GLint newSize = MAX( size, MIN( block->bookAlloc * 2, block->bookAlloc + maxBookAllocIncr ) );

    // try increasing by a reasonably big amount
    void *newbook = jsRealloc( block->book, newSize * sizeof( GLuint ) );

    // if it fails, increase by just the needed amount
    if (( !newbook ) && ( newSize > size ) )
    {
        newSize = size;
        newbook = jsRealloc( block->book, newSize * sizeof( GLuint ) );
    }

    // if it still fails, we won't make it
    if ( !newbook ) return -1;

    block->book = ( GLuint * )newbook;
    block->bookAlloc = newSize;
    return 0;
}

static inline int _jsMemoryBlockManagerBookInsert( MemoryBlockManager_t *block, GLint index, GLuint count )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    JS_ASSERT( index <= block->bookSize );
    int result = _jsMemoryBlockManagerBookCheckAlloc( block, block->bookSize + count );
    if ( result ) return result;
    memmove( block->book + index + count, block->book + index, ( block->bookSize - index )*sizeof( GLuint ) );
    block->bookSize += count;
    return 0;
}

static inline void _jsMemoryBlockManagerBookRemove( MemoryBlockManager_t *block, GLint index, GLuint count )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    JS_ASSERT( index + ( GLint )count <= block->bookSize );
    JS_ASSERT( count >= 0 );
    if ( count == 0 ) return;
    block->bookSize -= count;
    memmove( block->book + index, block->book + index + count, ( block->bookSize - index )*sizeof( GLuint ) );
}

static inline GLint _jsMemoryBlockManagerBookFind( MemoryBlockManager_t *block, void *mem )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    int i;
    char *current = ( char * )block->memory;
    for ( i = 0;i < block->bookSize;++i )
    {
        if ( current == mem ) return i;
        current += BLOCK_SIZE( block->book[i] );
    }
    return -1;
}

static inline void _jsMemoryBlockManagerBookAllocBlock( MemoryBlockManager_t *block, GLint index, GLuint size, GLuint pad )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );

    GLint newIndex = index;
    GLuint data = block->book[index];

    JS_ASSERT( !IS_ALLOC( data ) );
    JS_ASSERT( BLOCK_SIZE( data ) >= size + pad );

    GLuint rightPad = BLOCK_SIZE( data ) - size - pad;

    if ( pad > 0 )
    {
        // warning bug on my compiler version.
        int result = 0;
        result	= _jsMemoryBlockManagerBookInsert( block, newIndex, 1 );
        JS_ASSERT( !result );
        ( void )( result );
        block->book[newIndex] = pad;
        ++newIndex;
    }

    block->book[newIndex] = size | ALLOC_BIT;
    ++newIndex;

    if ( rightPad > 0 )
    {
        // warning bug on my compiler version
        int result = 0;
        result = _jsMemoryBlockManagerBookInsert( block, newIndex, 1 );
        JS_ASSERT( !result );
        ( void )( result );
        block->book[newIndex] = rightPad;
    }
}

static inline void _jsMemoryBlockManagerBookFreeBlock( MemoryBlockManager_t *block, GLint index, GLint *newBlock, GLuint *oldPad )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    JS_ASSERT( IS_ALLOC( block->book[index] ) );
    GLint newIndex = index;
    GLint removeCount = 0;
    GLuint newSize = BLOCK_SIZE( block->book[index] );
    GLuint pad = 0;

    // coalesce
    if (( index > 0 ) && !IS_ALLOC( block->book[index-1] ) )
    {
        --newIndex;
        ++removeCount;
        pad = BLOCK_SIZE( block->book[index-1] );
        newSize += pad;
    }
    if (( index < block->bookSize - 1 ) && !IS_ALLOC( block->book[index+1] ) )
    {
        ++removeCount;
        newSize += BLOCK_SIZE( block->book[index+1] );
    }
    if ( removeCount > 0 ) _jsMemoryBlockManagerBookRemove( block, newIndex + 1, removeCount );

    *oldPad = pad;
    *newBlock = newIndex;
    block->book[newIndex] = newSize;
}

int _jsMemoryBlockManagerInit( MemoryBlockManager_t *block, void * memory, GLuint size, GLuint alignment, GLenum method )
{
    JS_ASSERT( block );
    JS_ASSERT(( alignment&( alignment - 1 ) ) == 0 );
    if ( size >= ALLOC_BIT ) return -1;
    block->book = ( GLuint * )jsMalloc( defaultBookAllocSize * sizeof( GLuint ) );
    if ( !block->book ) return -1;
    block->memory = ( char * )memory;
    block->size = size;
    block->minAlignment = MAX( alignment, minAlign );
    block->method = method;
    block->bookSize = 1;
    block->bookAlloc = defaultBookAllocSize;
    block->book[0] = size;
    return 0;
}

void _jsMemoryBlockManagerWipe( MemoryBlockManager_t *block )
{
    JS_ASSERT( block );
    JS_ASSERT( block->bookAlloc > 0 );
    block->bookSize = 1;
    block->book[0] = block->size;
}

void _jsMemoryBlockManagerDestroy( MemoryBlockManager_t *block )
{
    JS_ASSERT( block );
    jsFree( block->book );
}

void *_jsMemoryBlockManagerAlloc( MemoryBlockManager_t *block, GLuint size )
{
    JS_ASSERT( block );
    return _jsMemoryBlockManagerAllocAligned( block, size, block->minAlignment );
}

void *_jsMemoryBlockManagerAllocAligned( MemoryBlockManager_t *block, GLuint size, GLuint alignment )
{
    JS_ASSERT( block );
    return _jsMemoryBlockManagerReallocAligned( block, NULL, size, alignment );
}

void *_jsMemoryBlockManagerRealloc( MemoryBlockManager_t *block, void *mem, GLuint size )
{
    JS_ASSERT( block );
    return _jsMemoryBlockManagerReallocAligned( block, mem, size, block->minAlignment );
}

static inline GLuint COST( GLuint freeBlockSize, GLuint size, GLuint pad )
{
    const GLuint otherPad = freeBlockSize - size - pad;
    return freeBlockSize*freeBlockSize - pad*pad - otherPad*otherPad;
}


void *_jsMemoryBlockManagerReallocAligned( MemoryBlockManager_t *block, void *mem, GLuint size, GLuint alignment )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );
    JS_ASSERT(( alignment&( alignment - 1 ) ) == 0 );

    int freedBlockIndex = -1;
    GLuint freedBlockPad = 0;
    GLuint oldBlockSize = 0;
    if ( alignment < block->minAlignment ) alignment = block->minAlignment;

    if ( size > 0 )
    {
        // we may need to allocate book-keeping info. Check that we can do it
        // now before we destroy the data. If we can't do it, we are so out of
        // memory that something else is probably going to fail anyway.
        // Gracefully fail the re-allocation
        int result = _jsMemoryBlockManagerBookCheckAlloc( block, block->bookSize + 2 );
        if ( result )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return NULL;
        }
    }

    if ( mem )
    {
        // free the pointer, keeping useful information
        GLint oldBlockIndex = _jsMemoryBlockManagerBookFind( block, mem );
        JS_ASSERT( oldBlockIndex >= 0 );
        JS_ASSERT( IS_ALLOC( block->book[oldBlockIndex] ) );
        oldBlockSize = BLOCK_SIZE( block->book[oldBlockIndex] );
        _jsMemoryBlockManagerBookFreeBlock( block, oldBlockIndex, &freedBlockIndex, &freedBlockPad );
    }
    if ( size <= 0 ) return NULL;

    char *pointer = block->memory;
    int bestFitIndex = -1;
    GLuint bestCost = UINT_MAX;
    char *bestPointer = NULL;
    GLuint bestLeftPad = 0;
    int i;
    for ( i = 0;i < block->bookSize;++i )
    {
        const GLuint blockSize = BLOCK_SIZE( block->book[i] );

        if ( IS_ALLOC( block->book[i] ) )
        {
            pointer += blockSize;
            continue;
        }

        GLuint leftPad = alignment - 1 - ((( long )pointer - 1 ) & ( alignment - 1 ) );

        // the block cannot fit
        if ( leftPad + size > blockSize )
        {
            pointer += blockSize;
            continue;
        }

        if ( block->method == _JS_ALLOC_FIRST_FIT )
        {
            bestFitIndex = i;
            bestLeftPad = leftPad;
            bestPointer = pointer;
            break;
        }

        // try left align
        GLuint cost = COST( blockSize, size, leftPad );
        if ( cost < bestCost )
        {
            bestFitIndex = i;
            bestCost = cost;
            bestLeftPad = leftPad;
            bestPointer = pointer;
        }

        // try right align
        GLuint rightPad = ( blockSize - leftPad - size ) & ( alignment - 1 );
        cost = COST( blockSize, size, rightPad );
        if ( cost < bestCost )
        {
            bestFitIndex = i;
            bestCost = cost;
            bestLeftPad = blockSize - size - rightPad;
            bestPointer = pointer;
        }

        pointer += blockSize;
    }

    if ( bestFitIndex < 0 )
    {
        if ( mem )
        {
            // we need to re-allocate the block we freed before
            _jsMemoryBlockManagerBookAllocBlock( block, freedBlockIndex, oldBlockSize, freedBlockPad );
        }
        return NULL;
    }

    _jsMemoryBlockManagerBookAllocBlock( block, bestFitIndex, size, bestLeftPad );

    char *newPointer = bestPointer + bestLeftPad;
    if ( mem && mem != newPointer )
    {
        GLuint move = oldBlockSize < size ? oldBlockSize : size;
        if ( move > 0 )	memmove( newPointer, mem, move );
    }

    // check to see if we ever return pointer that's out of bounds
    JS_ASSERT( newPointer >= block->memory );
    JS_ASSERT( newPointer + size <= block->memory + block->size );

    return newPointer;
}

void _jsMemoryBlockManagerFree( MemoryBlockManager_t *block, void *mem )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );

    if ( !mem ) return;
    GLint newBlock;
    GLuint pad;
    GLint index = _jsMemoryBlockManagerBookFind( block, mem );
    JS_ASSERT( index >= 0 );
    JS_ASSERT( IS_ALLOC( block->book[index] ) );
    _jsMemoryBlockManagerBookFreeBlock( block, index, &newBlock, &pad );
}

GLboolean _jsMemoryBlockManagerTryRealloc( MemoryBlockManager_t *block, void *mem, GLuint size )
{
    JS_ASSERT( block );
    JS_ASSERT( block->book );

    if ( !mem ) return GL_FALSE;
    GLint index = _jsMemoryBlockManagerBookFind( block, mem );
    JS_ASSERT( index >= 0 );
    JS_ASSERT( IS_ALLOC( block->book[index] ) );
    if ( size > BLOCK_SIZE( block->book[index] ) )
    {
        if (( index == block->bookSize - 1 ) ||
            ( IS_ALLOC( block->book[index+1] ) ))
        {
            // fail if we don't have any book left
            // or if next book is also allocated
            return GL_FALSE;
        }

        if ( BLOCK_SIZE( block->book[index] ) + BLOCK_SIZE( block->book[index+1] ) < size )
        {
            // next book doesn't have enough space
            return GL_FALSE;
        }
        else if ( BLOCK_SIZE( block->book[index] ) + BLOCK_SIZE( block->book[index+1] ) > size )
        {
            // subtract space from the next book
            block->book[index+1] += BLOCK_SIZE( block->book[index] ) - size;
        }
        else
        {
            // next book has just enough space
            _jsMemoryBlockManagerBookRemove( block, index + 1, 1 );
        }
        block->book[index] = ALLOC_BIT | size;
    }
    else if ( size < BLOCK_SIZE( block->book[index] ) )
    {
        if ( size == 0 )
        {
            GLint newBlock;
            GLuint pad;
            _jsMemoryBlockManagerBookFreeBlock( block, index, &newBlock, &pad );
        }
        else
        {
            if (( index == block->bookSize - 1 ) || ( IS_ALLOC( block->book[index+1] ) ) )
            {
                // we have to create a new block
                int result = _jsMemoryBlockManagerBookInsert( block, index + 1, 1 );
                if ( result ) return GL_FALSE;
                block->book[index+1] = BLOCK_SIZE( block->book[index] ) - size;
            }
            else
            {
                block->book[index+1] += BLOCK_SIZE( block->book[index] ) - size;
            }
            block->book[index] = ALLOC_BIT | size;
        }
    }
    return GL_TRUE;
}
