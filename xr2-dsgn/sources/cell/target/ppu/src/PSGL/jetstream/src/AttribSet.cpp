/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: AttribSet functions, see GL_SCE_vertex_attribute_set specification

///////////////////////////////////////////////////////////////////////////
// TODO / Questions:
// - what about include/PSGL/GLCall.h or GLCommon.h?
// - should include/PSGL/DPM.h be made to mirror the types in Types.h?
// - add a performance counter to measure Bind cost?
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/PerfCounters.h>
#include <PSGL/DPM2.h>

#if 0 //[
extern jsUint64 _PerformanceStep;
#define GET_COUNTER(_t) static jsUint64 _t=0; asm volatile ("mftb %0" : "=r"(_t));
#define PRINT_TIME_ELAPSED(_t1,_t0,_scale,_units) printf("deltaT: %s - %s = %9.4f%s %llu\n", #_t1, #_t0, (_scale*((double)(_t1-_t0)))/(double)_PerformanceStep, _units, _t1-_t0)
#else
#define GET_COUNTER(_t) ((void)0)
#define PRINT_TIME_ELAPSED(_t1,_t0,_scale,_units) ((void)0)
#endif //]

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
// AttribSet constructor/destructor

// constructor for _jsTexNameSpaceInit()
jsAttribSet* _jsCreateAttribSet( void )
{
    jsAttribSet* attribSet = ( jsAttribSet * )jsMemalign( 16, sizeof( jsAttribSet ) );
#ifndef JS_NO_ERROR_CHECK
    if ( attribSet == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
#endif

    _jsResetAttributeState( &attribSet->attribs );

    attribSet->dirty = GL_TRUE;
    attribSet->beenUpdatedMask = 0;
    attribSet->cmdBuffer = NULL;
    attribSet->cmdNumWords = 0;

    return attribSet;
}


// destructor for _jsTexNameSpaceInit()
void _jsDestroyAttribSet( jsAttribSet* attribSet )
{
    JS_ASSERT( attribSet );
    if ( attribSet->cmdBuffer ) jsFree( attribSet->cmdBuffer );
    jsFree( attribSet );
}


///////////////////////////////////////////////////////////////////////////
// Hooks for GL calls that may, via buffers, indirectly modify AttribSet.
// These indirect modifications may even affect an unbound AttribSet.

// called during glDeleteBuffers()
void _jsAttribSetDeleteBuffer( PSGLcontext *LContext, GLuint buffName )
{
    JS_ASSERT( LContext );
    JS_ASSERT( buffName );
    jsBufferObject *bufferObject = _jsGetBufferObject( LContext, buffName );
    GLuint attrSetCount = bufferObject->attribSets.getCount();
    if ( attrSetCount == 0 ) return;
    for ( unsigned int i = 0;i < attrSetCount;++i )
    {
        jsAttribSet *attribSet = bufferObject->attribSets[i];
        for ( GLuint j = 0; j < _JS_MAX_VERTEX_ATTRIBS; ++j )
        {
            if ( attribSet->attribs.attrib[j].arrayBuffer == buffName ) attribSet->attribs.attrib[j].arrayBuffer = 0;
        }
        attribSet->dirty = GL_TRUE;
    }
    LContext->attribSetDirty = GL_TRUE;
    bufferObject->attribSets.clear();
}


// called during glBufferData()
void _jsAttribSetBufferData( PSGLcontext *LContext, GLuint buffName )
{
    JS_ASSERT( LContext );
    JS_ASSERT( buffName );
    jsBufferObject *bufferObject = _jsGetBufferObject( LContext, buffName );
    GLuint attrSetCount = bufferObject->attribSets.getCount();
    if ( attrSetCount == 0 ) return;
    for ( unsigned int i = 0;i < attrSetCount;++i )
    {
        jsAttribSet *attribSet = bufferObject->attribSets[i];
        attribSet->dirty = GL_TRUE;
    }
    LContext->attribSetDirty = GL_TRUE;
}


// called during glMapBuffer
void _jsAttribSetMapBuffer( PSGLcontext *LContext, GLuint buffName )
{
    JS_ASSERT( LContext );
    JS_ASSERT( buffName );
    jsBufferObject *bufferObject = _jsGetBufferObject( LContext, buffName );
    GLuint attrSetCount = bufferObject->attribSets.getCount();
    if ( attrSetCount == 0 ) return;
    for ( unsigned int i = 0;i < attrSetCount;++i )
    {
        jsAttribSet *attribSet = bufferObject->attribSets[i];
        attribSet->dirty = GL_TRUE;
    }
    LContext->attribSetDirty = GL_TRUE;
}

// if an AttribSet is bound and modified, mark it dirty.
void _jsAttribSetMarkDirty()
{
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );
    JS_ASSERT( LContext->attribSetName );
    jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
    JS_ASSERT( attribSet );
    attribSet->dirty = GL_TRUE;
    LContext->attribSetDirty = GL_TRUE;
}


///////////////////////////////////////////////////////////////////////////
//  API functions
///////////////////////////////////////////////////////////////////////////

/** @addtogroup OpenGL
 *
 * @{
 */

/**
 *  @short Generate attribute set names
 *
glGenAttribSetsSCE() returns <I><c>n</c></I>&nbsp;<CODE>AttribSet</CODE> object names in <I><c>names</c></I> array.
For the purposes of glGenAttribSetsSCE() only, these names are marked as used.
However, they acquire state only when they are bound using glBindAttribSetSCE().

 *  @param n        Specifies the number of <CODE>AttribSet</CODE> objects to be generated.
 *  @param names  Specifies an array in which the generated <CODE>AttribSet</CODE> object names are stored.
 *
 *
 * @note
 * For more information about attribute sets, including sample code, refer to the <i>PlayStation&reg; Graphics
 * Library Programming Guide</i>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glDeleteAttribSetsSCE, glIsAttribSetSCE, glBindAttribSetSCE, glCopyAttribSetSCE
 */

GLAPI void APIENTRY glGenAttribSetsSCE( GLsizei n, GLuint *names )
{
    JS_APIENTRY_TIMER( glGenAttribSetsSCE );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenAttribSetsSCE, JSDPM_CALL_TYPE_STATE_GEOM, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             names, ( n * sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext *LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->attribSetNameSpace, n, names );
}


///////////////////////////////////////////////////////////////////////////

/**
 *  @short Delete named attribute set objects
 *
glDeleteAttribSetsSCE() deletes <I><c>n</c></I>&nbsp;<CODE>AttribSet</CODE> objects named by the elements of the array
<I><c>names</c></I>. After an <CODE>AttribSet</CODE> object is deleted, its name is free for reuse.

glDeleteAttribSetsSCE() ignores the case where <I><c>n</c></I> is zero, or when values in <I><c>names</c></I> do not
correspond to existing <CODE>AttribSet</CODE> objects.

 *  @param n        Specifies the number of <CODE>AttribSet</CODE> objects to be deleted.
 *  @param names  Specifies an array of <CODE>AttribSet</CODE> object names to be deleted.
 *
 *  @note
An error is returned if an attribute set is currently bound when glDeleteAttribSetsSCE() is called. 
@par
For more information about attribute sets, including sample code, refer to the <i>PlayStation&reg; Graphics
 * Library Programming Guide</i>.
 *
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glGenAttribSetsSCE, glIsAttribSetSCE, glBindAttribSetSCE, glCopyAttribSetSCE
 */

GLAPI void APIENTRY glDeleteAttribSetsSCE( GLsizei n, const GLuint *names )
{
    JS_APIENTRY_TIMER( glDeleteAttribSetsSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2(
            glDeleteAttribSetsSCE, JSDPM_CALL_TYPE_STATE_GEOM, , 2,
            n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
            names, ( n*sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    for ( int i = 0; i < n; ++i )
    {
        // if we're trying to delete a bound AttribSet, unbind first!
        if ( LContext->attribSetName == names[i] ) glBindAttribSetSCE( 0 );
    }
    _jsTexNameSpaceDeleteNames( &LContext->attribSetNameSpace, n, names );
}


///////////////////////////////////////////////////////////////////////////

/**
 * @short Test for a named attribute set
 *
 * glIsAttribSetSCE() tests a name to determine if it represents
 * an <CODE>AttribSet</CODE> object.  An attribute set is created when an unused name is
 * bound for the first time.
 *
 * @param       name Specifies the AttribSet name.
 *
 * @return Returns <CODE>GL_TRUE</CODE> if <I><c>name</c></I> is the
 * name of an <CODE>AttribSet</CODE> object and returns <CODE>GL_FALSE</CODE> otherwise.
 *
 * @note
 * For more information about attribute sets, including sample code, refer to the <i>PlayStation&reg; Graphics
 * Library Programming Guide</i>.
 *
 * @sa glGenAttribSetsSCE, glDeleteAttribSetsSCE, glBindAttribSetSCE, glCopyAttribSetSCE
 */

GLAPI GLboolean APIENTRY glIsAttribSetSCE( GLuint name )
{
    JS_APIENTRY_TIMER( glIsAttribSetSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glIsAttribSetSCE, JSDPM_CALL_TYPE_STATE_GEOM, GL_TRUE, 1,
                             name, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    return _jsTexNameSpaceIsName( &LContext->attribSetNameSpace, name );
}


///////////////////////////////////////////////////////////////////////////

/**
 *  @short Bind a named attribute set
 *
glBindAttribSetSCE() creates or uses a named attribute set.
Calling glBindAttribSetSCE() with <I><c>name</c></I> activates the named <CODE>AttribSet</CODE> in the PSGL context.

While an attribute set is bound, PSGL operations which affect attribute parameters are stored in the
bound <CODE>AttribSet</CODE> object.  Note that the GL state is not changed until the first draw after the
attribute set is bound.  glBindAttribSetSCE() does not affect the GL state.

If an unused attribute set name is specified, an <CODE>AttribSet</CODE> object is created.
The resulting default <CODE>AttribSet</CODE> object has no stored attribute parameters.

Attribute set names are unsigned integers.
When an attribute set is bound, any previous attribute set binding is automatically broken.
The name 0 is reserved by PSGL, and breaks any existing attribute set binding.

 *  @param name     Specifies the name of an attribute set.
 *
 * @note
 * For more information about attribute sets, including sample code, refer to the <i>PlayStation&reg; Graphics
 * Library Programming Guide</i>.
 *
 * @sa glGenAttribSetsSCE, glDeleteAttribSetsSCE, glIsAttribSetSCE, glCopyAttribSetSCE
 */

GLAPI void APIENTRY glBindAttribSetSCE( GLuint name )
{
    JS_APIENTRY_TIMER( glBindAttribSetSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( glBindAttribSetSCE, JSDPM_CALL_TYPE_STATE_GEOM, , 1,
                             name, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    if ( name == 0 )
    {
        // Special case: 0 == unbind any AttribSet

        if ( LContext->attribSetName )
        {
            // if an AttribSet is currently bound, then
            // copy its contents back to the default jsAttributeState.
            jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
            JS_ASSERT( attribSet );

            jsAttributeState* __restrict cxAttrs = &LContext->defaultAttribs0;
            jsAttributeState* __restrict asAttrs = &attribSet->attribs;
            *cxAttrs = *asAttrs;
        }

        LContext->attribs = &LContext->defaultAttribs0;
        LContext->attribSetName = 0;
        LContext->attribSetDirty = GL_FALSE;
        return;
    }

    // ensure object exists (in case didn't call glGenAttribSets())
    _jsTexNameSpaceCreateNameLazy( &LContext->attribSetNameSpace, name );
#ifndef JS_NO_ERROR_CHECK
    if ( ! _jsTexNameSpaceIsName( &LContext->attribSetNameSpace, name ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsAttribSet* attribSet = _jsGetAttribSet( name );
    JS_ASSERT( attribSet );

    LContext->attribs = &attribSet->attribs;
    LContext->attribSetName = name;
    LContext->attribSetDirty = GL_TRUE;
}

///////////////////////////////////////////////////////////////////////////

/**
 * @short Copy an attribute set
 *
 * glCopyAttribSetSCE() copies the attribute parameter state of an <CODE>AttribSet</CODE>
 * object to another <CODE>AttribSet</CODE> object.  The target <CODE>AttribSet</CODE> object is created if
 * it does not exist.
 *
 * @param       fromName Specifies the name of the source <CODE>AttribSet</CODE> object.
 * @param       toName Specifies the name of the target <CODE>AttribSet</CODE> object.
 *
 * @note
 * For more information about attribute sets, including sample code, refer to the <i>PlayStation&reg; Graphics
 * Library Programming Guide</i>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The <CODE>AttribSet</CODE> object for <I><c>fromName</c></I> or <I><c>toName</c></I> is currently bound.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>fromName</c></I> and <I><c>toName</c></I> specify the same name.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>fromName</c></I> or <I><c>toName</c></I> is negative.</TD>
</TR>

</TABLE>
 *
 * @sa glGenAttribSetsSCE, glDeleteAttribSetsSCE, glIsAttribSetSCE, glBindAttribSetSCE
 */

GLAPI void APIENTRY glCopyAttribSetSCE( GLuint fromName, GLuint toName )
{
    JS_APIENTRY_TIMER( glCopyAttribSetSCE );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glCopyAttribSetSCE, JSDPM_CALL_TYPE_STATE_GEOM, , 2,
                             fromName, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             toName, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    PSGLcontext* LContext = _CurrentContext;
    JS_ASSERT( LContext );

    // not permitted when AttribSet is bound or copy to self
    if (( LContext->attribSetName == fromName ) || ( LContext->attribSetName == toName ) || ( fromName == toName ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // names must be valid
    if ( ! _jsTexNameSpaceIsName( &LContext->attribSetNameSpace, fromName ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if ( ! _jsTexNameSpaceIsName( &LContext->attribSetNameSpace, toName ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsAttribSet* __restrict from = _jsGetAttribSet( fromName );
    JS_ASSERT( from );

    jsAttribSet* __restrict to = _jsGetAttribSet( toName );
    JS_ASSERT( to );

    jsAttributeState* fromAS = &from->attribs;
    jsAttributeState* toAS = &to->attribs;
    *toAS = *fromAS;

    to->dirty = GL_TRUE;
}

/** @} OpenGL */

/*EOF*/
