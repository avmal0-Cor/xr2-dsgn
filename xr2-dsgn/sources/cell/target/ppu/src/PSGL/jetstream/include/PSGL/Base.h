/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _BASE_H_
#define _BASE_H_

#include <PSGL/Debug.h>
#include <PSGL/Malloc.h>
#include <new>

namespace PSGL
{

class Base
{
public:
    void * operator new( size_t size ) { return jsMalloc( size ); }
    void * operator new( size_t /*size*/, void *p ) { return p; }
    void operator delete( void *ptr ) { return jsFree( ptr ); }
    void operator delete( void * /*ptr*/, void * /*p*/ ) { }};

template<class T> class Vector: public Base
{
    T* array;
    unsigned int count;
    unsigned int capacity;
    unsigned int increment;
public:
    Vector(): array( 0 ), count( 0 ), capacity( 0 ), increment( 4 ) {}
    ~Vector() { clear(); reallocArray( 0 ); }

    inline void setIncrement( unsigned int i ) { increment = i; }
    inline unsigned int getCount() { return count; }

    inline void reallocArray( unsigned int newCapacity )
    {
        if ( newCapacity == capacity ) return;
        if ( newCapacity > capacity ) newCapacity = ( newCapacity > capacity + increment ) ? newCapacity : ( capacity + increment );
        JS_ASSERT( newCapacity >= count );
        if ( newCapacity == 0 )
        {
            jsFree( array );
            array = 0;
        }
        else array = static_cast<T*>( jsRealloc( static_cast<void *>( array ), sizeof( T ) * newCapacity ) );
        capacity = newCapacity;
    }

    inline void clear()
    {
        if ( !array ) return;
        for ( unsigned int i = 0;i < count;++i )( array + i )->~T();
        count = 0;
    }

    inline unsigned int pushBack( const T &element )
    {
        if ( count + 1 > capacity ) reallocArray( count + 1 );
        new(( void * )( array + count ) ) T( element );
        return ++count;
    }

    inline T &appendNew()
    {
        if ( count + 1 > capacity ) reallocArray( count + 1 );
        T* newElem = new( array + count ) T();
        ++count;
        return *newElem;
    }

    inline unsigned int appendUnique( const T &element )
    {
        for ( unsigned int i = 0;i < count;++i ) if ( array[i] == element ) return i;
        return pushBack( element );
    }

    inline void removeElement( const T &element )
    {
        for ( unsigned int i = count; i > 0; --i )
        {
            if ( array[i-1] == element )
            {
                remove( i - 1 );
                return;
            }
        }
    }

    inline void remove( unsigned int index )
    {
        JS_ASSERT( index < count );
        ( array + index )->~T();
        --count;
        if ( count > index ) memmove( array + index, array + index + 1, ( count - index )*sizeof( T ) );
    }

inline T *getArray() const { return array; }
    inline T& operator []( int i ) const { return array[i]; }
};

}

#endif
