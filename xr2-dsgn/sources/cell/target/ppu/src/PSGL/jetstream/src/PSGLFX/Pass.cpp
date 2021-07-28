/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Techniques
 */

#include <PSGL/psgl.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Technique.h>
#include <PSGLFX/Effect.h>
#include <PSGLFX/Pass.h>
#include <PSGLFX/StateAssignment.h>
#include <PSGLFX/State.h>
#include <PSGLFX/Annotation.h>
#include <string.h>

namespace PSGLFX
{
Pass::Pass( Technique *technique, const char *n ):
        Named( n ),
        parent( technique ),
        next( 0 ),
        stateAssignments( 0 ),
        annotations( 0 )
{
    id = GLContextExtension::getCurrent()->addPass( this );
}

Pass::~Pass()
{
    if ( next ) delete next;
    if ( stateAssignments ) delete stateAssignments;
    if ( annotations ) delete annotations;
    GLContextExtension::getCurrent()->delPass( id );
}

void Pass::append( Pass *other )
{
    if ( next ) next->append( other );
    else next = other;
}

void Pass::appendStateAssignment( StateAssignment *stateAssignment )
{
    if ( stateAssignments ) stateAssignments->append( stateAssignment );
    else stateAssignments = stateAssignment;
    invalidate();
}

void Pass::appendAnnotation( Annotation *annotation )
{
    if ( annotations ) annotations->append( annotation );
    else annotations = annotation;
}

bool Pass::validate()
{
    validated = true;
    for ( StateAssignment *s = stateAssignments;s;s = s->getNext() ) validated &= s->validate();
    return validated;
}

void Pass::invalidate()
{
    validated = false;
    if ( parent ) parent->invalidate();
}

bool Pass::isValidated()
{
    return validated;
}

void Pass::setState()
{
    JS_ASSERT( validated );
    for ( StateAssignment *s = stateAssignments;s;s = s->getNext() )
    {
        JS_ASSERT( s->isValidated() );
        s->getState()->set( s );
    }
}

void Pass::resetState()
{
    JS_ASSERT( validated );
    for ( StateAssignment *s = stateAssignments;s;s = s->getNext() )
    {
        JS_ASSERT( s->isValidated() );
        s->getState()->reset( s );
    }
}
}

using namespace PSGLFX;

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates a new pass in a technique

cgCreatePass() creates a new empty pass and adds it to a technique. The technique
will keep the pass creation order when iterating through the passes with cgGetFirstPass()
and cgGetNextPass().

@param handle  Specifies the technique that the new pass will be added to.
@param name    The name of the newly created pass

@return Returns the handle to the new pass.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgCreateTechnique, cgGetFirstPass, cgIsPass, cgDestroyEffect, cgCreatePassAnnotation
 */
CGFX_API CGpass cgCreatePass( CGtechnique handle, const char *name )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Technique* technique = CG_GET_TECHNIQUE( handle );
    Pass *pass = new Pass( technique, name );
    technique->appendPass( pass );
    return pass->getId();
}

/**
@short Retrieve the first pass in a technique

cgGetFirstPass() returns the first pass in a technique. Together with
cgGetNextPass(), this is used to iterate through all the passes in a technique.

@param handle  The technique to look up passes from.

@return Returns the first pass in the technique.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgGetNextPass
 */
CGFX_API CGpass cgGetFirstPass( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Pass *pass = CG_GET_TECHNIQUE( handle )->getFirstPass();
    return pass ? pass->getId() : 0;
}

/**
@short Retrieve the next pass in a technique

cgGetNextPass() returns the pass following a given pass, in a technique. Together with cgGetFirstPass(), this is used to iterate through all the passes in a technique.

@param handle  The pass from which to get the successor

@return Returns the next pass in the technique list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa       cgGetFirstPass
 */
CGFX_API CGpass cgGetNextPass( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    Pass *pass = CG_GET_PASS( handle )->getNext();
    return pass ? pass->getId() : 0;
}

/**
@short Searches for a pass by name

cgGetNamedPass() looks for a pass by name, in a given technique.

@param handle The technique to look into
@param name   The name of the pass to look for

@return Returns a handle to a pass if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa       cgCreatePass
 */
CGFX_API CGpass cgGetNamedPass( CGtechnique handle, const char *name )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Technique *technique = CG_GET_TECHNIQUE( handle );

    for ( Pass *pass = technique->getFirstPass(); pass; pass = pass->getNext() )
    {
        if ( strcmp( pass->getName(), name ) == 0 ) return pass->getId();
    }
    return NULL;
}

/**
@short Retrieves the name of a pass

cgGetPassName() returns the name of the pass, as specified by cgCreatePass().

@param handle The pass to retrieve the name from

@return Returns the name of the pass if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa       cgCreatePass
 */
CGFX_API const char *cgGetPassName( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_PASS( handle )->getName();
}

/**
@short Tests if a handle represents a valid pass

cgIsPass() tests whether or not a <c>CGpass</c> handle is a valid pass

@param handle  The pass handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid pass, and <CODE>CG_FALSE</CODE> otherwise.

@sa       cgCreatePass
 */
CGFX_API CGbool cgIsPass( CGpass handle )
{
    return CG_IS_PASS( handle );
}

/**
@short Retrieve the technique a pass belongs to

cgGetPassTechnique() returns the "parent" technique of a pass, the one that was specified in cgCreatePass().

@param handle  The pass from which to get the parent technique

@return Returns the parent technique.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa       cgCreatePass
 */
CGFX_API CGtechnique cgGetPassTechnique( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_PASS( handle )->getParent()->getId();
}

/**
@short Sets all the states in a pass to their assigned values

cgSetPassState() iterates through all the states assignments in a pass, in
creation order, and sets each corresponding state to their assigned value, by
calling the <I>set</I> callback of that state.
The technique containing this pass has to be validated (by calling cgValidateTechnique())
before cgSetPassState() is called.

@param handle  The pass from which the states have to be set

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgCreateStateAssignment, cgValidateTechnique, cgSetStateCallbacks
 */
CGFX_API void cgSetPassState( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return;
    }
    Pass *pass = CG_GET_PASS( handle );
    if ( pass->isValidated() ) pass->setState();
    else
    {
        // XXX raise error ?
    }
}

/**
@short Resets all the states in a pass to their default values

cgResetPassState() iterates through all the states assignments in a pass, in
creation order, and resets each corresponding state to their default value, by
calling the <I>reset</I> callback of that state.
The technique containing this pass has to be validated (by calling cgValidateTechnique())
before cgResetPassState() is called.

@param handle  The pass from which the states have to be reset

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgCreateStateAssignment, cgValidateTechnique, cgSetStateCallbacks
 */
CGFX_API void cgResetPassState( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return;
    }
    Pass *pass = CG_GET_PASS( handle );
    if ( pass->isValidated() ) pass->resetState();
    else
    {
        // XXX raise error ?
    }
}

/**
@short Adds a scalar annotation to a pass.

cgCreatePassAnnotation() creates a new annotation and adds it to a pass.
The newly created annotation is of a scalar type, and its value can be specified
using one of the cgSet*Annotation functions.

@param handle The pass to add an annotation to
@param name   The name of the annotation to add
@param type   The type of the annotation. Must be <CODE>CG_FLOAT</CODE>, <CODE>CG_INT</CODE>, <CODE>CG_BOOL</CODE> or <CODE>CG_STRING</CODE>

@return Returns a handle to the created annotation if it was successfully created, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is not one of the valid types specified above.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgSetFloatAnnotation, cgSetIntAnnotation,
    cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues, cgGetIntAnnotationValues,
    cgGetBoolAnnotationValues, cgGetStringAnnotationValue
 */
CGFX_API CGannotation cgCreatePassAnnotation( CGpass handle, const char *name, CGtype type )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }

    AnyValue::Type internalType = AnyValue::getAnnotationTypeFromCGtype( type );
    if ( internalType == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }

    Pass *pass = CG_GET_PASS( handle );
    Annotation *annotation = new Annotation( name, internalType );
    pass->appendAnnotation( annotation );
    return annotation->getId();
}

/**
 * @}
 */
// ------------------------------------------------------
