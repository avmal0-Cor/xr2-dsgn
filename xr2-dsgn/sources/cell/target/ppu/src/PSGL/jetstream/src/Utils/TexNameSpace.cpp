/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Texture object namespace API implementation
 * Manage jsTexNameSpace data structure which has pointer to the data 
 * witch is associated with name by array index.
 * For example, ns->data[i] is a pointer to the data with name i
 */

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/Types.h>
#include <PSGL/Utils.h>
#include <PSGL/private.h>

static const unsigned int capacityIncr = 16;

// Initialize texture namespace ns with creation and destruction functions
void _jsTexNameSpaceInit( jsTexNameSpace *ns, jsTexNameSpaceCreateFunction create, jsTexNameSpaceDestroyFunction destroy )
{
    JS_ASSERT( ns );
    ns->capacity = capacityIncr;
    ns->data = ( void ** )jsMalloc( ns->capacity * sizeof( void* ) );
#ifndef JS_NO_ERROR_CHECK
    if ( !ns->data )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }
#endif
    memset( ns->data, 0, ns->capacity*sizeof( void* ) );
    ns->create = create;
    ns->destroy = destroy;
}

// Free texture namespace ns
void _jsTexNameSpaceFree( jsTexNameSpace *ns )
{
    JS_ASSERT( ns );
    JS_ASSERT( ns->data );
    for ( GLuint i = 1;i < ns->capacity;++i )
    {
        if ( ns->data[i] ) ns->destroy( ns->data[i] );
    }
    jsFree( ns->data );
    ns->data = NULL;
}

// Reset all names in namespace ns to NULL
void _jsTexNameSpaceResetNames( jsTexNameSpace *ns )
{
    JS_ASSERT( ns );
    JS_ASSERT( ns->data );
    for ( GLuint i = 1;i < ns->capacity;++i )
    {
        if ( ns->data[i] )
        {
            ns->destroy( ns->data[i] );
            ns->data[i] = NULL;
        }
    }
}

// Get an index of the first free name in namespace ns
GLuint _jsTexNameSpaceGetFree( jsTexNameSpace *ns )
{
    JS_ASSERT( ns );
    JS_ASSERT( ns->data );
    GLuint i;
    for ( i = 1;i < ns->capacity;++i )
    {
        if ( !ns->data[i] ) break;
    }
    return i;
}

// Add name to namespace by increasing capacity and calling creation call back function
// Return GL_TRUE for success, GL_FALSE for failure
GLboolean _jsTexNameSpaceCreateNameLazy( jsTexNameSpace *ns, GLuint name )
{
    JS_ASSERT( ns );
    JS_ASSERT( ns->data );
    if ( name >= ns->capacity )
    {
        int newCapacity = name >= ns->capacity + capacityIncr ? name + 1 : ns->capacity + capacityIncr;
        void **newData = ( void ** )jsRealloc( ns->data, newCapacity * sizeof( void * ) );
#ifndef JS_NO_ERROR_CHECK
        if ( !newData )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return GL_FALSE;
        }
#endif
        memset( newData + ns->capacity, 0, ( newCapacity - ns->capacity )*sizeof( void * ) );
        ns->data = newData;
        ns->capacity = newCapacity;
    }
    if ( !ns->data[name] )
    {
        ns->data[name] = ns->create();
        if ( ns->data[name] ) return GL_TRUE;
    }
    return GL_FALSE;
}

// Check if name is a valid name in namespace ns
// Return GL_TRUE if so, GL_FALSE otherwise
GLboolean _jsTexNameSpaceIsName( jsTexNameSpace *ns, GLuint name )
{
    JS_ASSERT( ns );
    JS_ASSERT( ns->data );
    if (( name > 0 ) && ( name < ns->capacity ) ) return( ns->data[name] != 0 );
    else return GL_FALSE;
}

// Generate new n names in namespace ns
void _jsTexNameSpaceGenNames( jsTexNameSpace *ns, GLsizei n, GLuint *names )
{
    JS_ASSERT( ns );
    JS_ASSERT( names );
#ifndef JS_NO_ERROR_CHECK
    if ( n < 0 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif
    for ( int i = 0;i < n;++i )
    {
        GLuint name = _jsTexNameSpaceGetFree( ns );
        names[i] = name;
        if ( name ) _jsTexNameSpaceCreateNameLazy( ns, name );
    }
}

// Delete n names from namespace ns
void _jsTexNameSpaceDeleteNames( jsTexNameSpace *ns, GLsizei n, const GLuint *names )
{
    JS_ASSERT( ns );
    JS_ASSERT( names );
#ifndef JS_NO_ERROR_CHECK
    if ( n < 0 )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif
    for ( int i = 0;i < n;++i )
    {
        GLuint name = names[i];
        if ( !_jsTexNameSpaceIsName( ns, name ) ) continue;
        ns->destroy( ns->data[name] );
        ns->data[name] = NULL;
    }
}

