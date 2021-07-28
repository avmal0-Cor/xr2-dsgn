/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/Debug.h>
#include <PSGL/Types.h>
#include <PSGL/Utils.h>
#include <PSGL/private.h>
#include <Cg/CgCommon.h>

#include <string.h>
/*
 * General  comments:
 * This data structure holds a map of handles (aka names) to pointers in memory (aka values).
 * It has a constant time creation (except when reallocation is needed), free, test and lookup of name.
 * There is no search mechanism for a value.
 * The data structure is an array of pointers, indexed by name.
 * The free elements are stored as a linked list of pointers in place of the values (ie when a name is free, its value is a pointer
 * to the next free name's value.
 * An element is known to be free if its value points to inside the array of pointers.
 * There are a couple of gotchas:
 * - the values have to be pointers to memory. This is because the fact is used to figure out whether a
 *   specific name is free without walking the linked list.
 * - all names are offset by 1, so that the name 0 is never returned.
 *   Each name API handles that offset first thing
 * - The free list is a linked list of pointers. This might seem scary considering they point to a block that can be reallocated.
 *   But the reallocation only happens when no free element is left, hence when no pointer inside the block exists anymore.
 * - The free linked list is guarded by a NULL pointer
 */
void _jsInitNameSpace( jsNameSpace * name )
{
    JS_ASSERT( name != NULL );
    name->data = NULL;
    name->firstFree = NULL;
    name->capacity = 0;
}

void _jsFreeNameSpace( jsNameSpace * ns )
{
    JS_ASSERT( ns != NULL );
    // XXX should we verify all names were freed ?

    if ( ns->data ) { jsFree( ns->data ); };
    ns->data = NULL;
    ns->capacity = 0;
    ns->firstFree = NULL;
}

static const int NAME_INCREMENT = 4;
jsName _jsCreateName( jsNameSpace * ns, void* object )
{
    JS_ASSERT( ns != NULL );
    // NULL is reserved for the guard of the linked list.
    JS_ASSERT( object != NULL );
    if ( NULL == ns->firstFree )
    {
        // need to allocate more pointer space
        int newCapacity = ns->capacity + NAME_INCREMENT;

        // realloc the block of pointers
        void** newData = ( void** )jsMalloc( newCapacity * sizeof( void* ) );
        if ( newData == NULL )
        {
            // XXX what should we generally do here ?
            _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
            return 0;
        }
        memcpy( newData, ns->data, ns->capacity * sizeof( void* ) );
        if ( ns->data != NULL ) jsFree( ns->data );
        ns->data = newData;
        // initialize the pointers to the next free elements.
        // (effectively build a linked list of free elements in place)
        // treat the last item differently, by linking it to NULL
        for ( int index = ns->capacity; index < newCapacity - 1; ++index )
        {
            ns->data[index] = ns->data + index + 1;
        }
        ns->data[newCapacity - 1] = NULL;
        // update the first free element to the new data pointer.
        ns->firstFree = ns->data + ns->capacity;
        // update the new capacity.
        ns->capacity = newCapacity;
    }
    // firstFree is a pointer, compute the index of it
    jsName result = ns->firstFree - ns->data;

    // update the first free to the next free element.
    ns->firstFree = ( void** ) * ns->firstFree;

    // store the object in data.
    ns->data[result] = object;

    // offset the index by 1 to avoid the name 0
    return result + 1;
}

unsigned int _jsIsName( jsNameSpace* ns, jsName name )
{
    // there should always be a namesepace
    JS_ASSERT( NULL != ns );

    // 0 is never valid.
    if ( JS_UNLIKELY( 0 == name ) )
    {
        return 0;
    }

    // names start numbering from 1, so convert from a name to an index
    --name;

    // test whether it is in the namespace range
    if ( JS_UNLIKELY( name >= ns->capacity ) )
    {
        return 0;
    }

    // test whether the pointer is inside the data block.
    // if so, it means it is free.
    // if it points to NULL, it means it is the last free name in the linked list.
    void** value = ( void** )ns->data[name];

    if ( JS_UNLIKELY( NULL == value ||
                      ( value >= ns->data && value < ns->data + ns->capacity ) ) )
    {
        return 0;
    }

    // The pointer is not free and allocated, so name is a real name.
    return 1;
}

void _jsEraseName( jsNameSpace* ns, jsName name )
{
    JS_ASSERT( ns != NULL );
    if ( _jsIsName( ns, name ) )
    {
        --name;
        ns->data[name] = ns->firstFree;
        ns->firstFree = ns->data + name;
    }
}
