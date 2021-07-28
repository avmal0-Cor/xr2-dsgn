/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - State Assignment
 */

#include <PSGL/psgl.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/State.h>
#include <PSGLFX/StateAssignment.h>
#include <PSGLFX/Pass.h>
#include <PSGLFX/Parameter.h>
#include <string.h>

namespace PSGLFX
{
StateAssignment::StateAssignment( Pass *p, State *s ):
        Base(),
        parentPass( p ),
        parentParameter( 0 ),
        next( 0 ),
        state( s ),
        value( s->getType(), s->getElemCount() ),
        arrayIndex( 0 )
{
    id = GLContextExtension::getCurrent()->addStateAssignment( this );
}

StateAssignment::StateAssignment( CgRuntimeParameter *p, State *s ):
        Base(),
        parentPass( 0 ),
        parentParameter( p ),
        next( 0 ),
        state( s ),
        value( s->getType(), s->getElemCount() ),
        arrayIndex( 0 )
{
    id = GLContextExtension::getCurrent()->addStateAssignment( this );
}

StateAssignment::~StateAssignment()
{
    if ( next ) delete next;
    GLContextExtension::getCurrent()->delStateAssignment( id );
}

void StateAssignment::append( StateAssignment *other )
{
    if ( next ) next->append( other );
    else next = other;
}

void StateAssignment::invalidate()
{
    validated = false;
    if ( parentPass ) parentPass->invalidate();
}

}

using namespace PSGLFX;

static inline AnyValue *getValueFromStateAssignmentCheckType( CGstateassignment handle, AnyValue::Type type )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    AnyValue &value = sa->getValue();
    if ( value.getType() != type )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return 0;
    }
    return &value;
}

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates a new state assignment in a pass

cgCreateStateAssignment() creates a new state assignment and adds it to a pass. The pass
will keep the state assignment creation order when iterating through the state
assignments with cgSetPassState() and cgResetPassState().
If a state assignment acts on a state that exists in multiple instances, it must be created
by cgCreateStateAssignmentIndex().

@param passHandle   Specifies the pass that the new state assignment will be added to.
@param stateHandle  The state the newly created state assignment will act on

@return Returns the handle to the new state assignment.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>passHandle</c></I> is not a valid pass.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>stateHandle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgCreateState, cgCreateStateAssignmentIndex, cgGetFirstStateAssignment, cgIsStateAssignment, cgDestroyEffect, cgCreateSamplerStateAssignment
 */
CGFX_API CGstateassignment cgCreateStateAssignment( CGpass passHandle, CGstate stateHandle )
{
    if ( !CG_IS_PASS( passHandle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    if ( !CG_IS_STATE( stateHandle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    Pass *pass = CG_GET_PASS( passHandle );
    State *state = CG_GET_STATE( stateHandle );

    // XXX check that state is not a sampler state ???

    StateAssignment *stateAssignment = new StateAssignment( pass, state );
    pass->appendStateAssignment( stateAssignment );
    return stateAssignment->getId();
}

/**
@short Creates a new state assignment with the specified instance index in a pass

cgCreateStateAssignmentIndex() creates a new state assignment and adds it to a pass. The pass
will keep the state assignment creation order when iterating through the state
assignments with cgSetPassState() and cgResetPassState().

This call is to be used for creation of state assignments that act on a state that exists
in multiple instances (for example light states or texture unit states).
For each instance, there is only one <code>CGstate</code> representation, but the index of the instance is referenced
by the state assignment.

@param passHandle   Specifies the pass that the new state assignment will be added to.
@param stateHandle  The state on which the newly created state assignment will act
@param index  Specifies the state instance index

@return Returns the handle to the new state assignment.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>passHandle</c></I> is not a valid pass.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>stateHandle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreatePass, cgCreateState, cgCreateStateAssignment, cgGetFirstStateAssignment, cgIsStateAssignment, cgDestroyEffect, cgCreateSamplerStateAssignment
 */
CGFX_API CGstateassignment cgCreateStateAssignmentIndex( CGpass passHandle, CGstate stateHandle, int index )
{
    // slow but acceptable for now
    CGstateassignment sa = cgCreateStateAssignment( passHandle, stateHandle );
    // we just created it.  it MUST be a state assignment
    JS_ASSERT( CG_IS_STATE_ASSIGNMENT( sa ) );
    StateAssignment *stateAssignment = CG_GET_STATE_ASSIGNMENT( sa );
    stateAssignment->setArrayIndex( index );
    return stateAssignment->getId();
}

/**
@short Retrieve the first state assignment in a pass

cgGetFirstStateAssignment() returns the first state assignment in a pass. Together with
cgGetNextStateAssignment(), this is used to iterate through all the state assignments in a pass.

@param handle  The pass to look up state assignments from.

@return Returns the first state assignment in the pass.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa cgCreateStateAssignment, cgGetNextStateAssignment
 */
CGFX_API CGstateassignment cgGetFirstStateAssignment( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    StateAssignment *stateAssignment = CG_GET_PASS( handle )->getFirstStateAssignment();
    return stateAssignment ? stateAssignment->getId() : 0;
}

/**
@short Retrieve the next state assignment

cgGetNextStateAssignment() returns the state assignment following a given state assignment. Together with cgGetFirstStateAssignment() and cgGetFirstSamplerStateAssignment(), this is used to iterate through all the state assignments in a pass or a sampler parameter.

@param handle  The state assignment from which to get the successor

@return Returns the next state assignment in the pass list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa       cgGetFirstStateAssignment, cgGetFirstSamplerStateAssignment
 */
CGFX_API CGstateassignment cgGetNextStateAssignment( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    StateAssignment *stateAssignment = CG_GET_STATE_ASSIGNMENT( handle )->getNext();
    return stateAssignment ? stateAssignment->getId() : 0;
}

/**
@short Searches for a state assignment by state name

cgGetNamedStateAssignment() looks for a state assignment by state name, in a given pass.

@param handle The pass to look into
@param name   The name of the state set by the state assignment to look for

@return Returns a handle to a state assignment if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa       cgCreateStateAssignment
 */
CGFX_API CGstateassignment cgGetNamedStateAssignment( CGpass handle, const char *name )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    Pass *pass = CG_GET_PASS( handle );

    for ( StateAssignment *stateAssignment = pass->getFirstStateAssignment(); stateAssignment; stateAssignment = stateAssignment->getNext() )
    {
        if ( strcmp( stateAssignment->getState()->getName(), name ) == 0 ) return stateAssignment->getId();
    }
    return NULL;
}

/**
@short Tests if a handle represents a valid state assignment

cgIsStateAssignment() tests whether or not a <c>CGstate</c> assignment handle is a valid state assignment.

@param handle  The state assignment handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid state assignment, and <CODE>CG_FALSE</CODE> otherwise.

@sa       cgCreateStateAssignment
 */
CGFX_API CGbool cgIsStateAssignment( CGstateassignment handle )
{
    return CG_IS_STATE_ASSIGNMENT( handle );
}

/**
@short Retrieve the pass a state assignment belongs to

cgGetStateAssignmentPass() returns the "parent" pass of a state assignment, the one that was specified in cgCreateStateAssignment().

@param handle  The state assignment from which to get the parent pass

@return Returns the parent pass.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa       cgCreateStateAssignment
 */
CGFX_API CGpass cgGetStateAssignmentPass( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    Pass *parent = CG_GET_STATE_ASSIGNMENT( handle )->getParentPass();
    if ( parent ) return parent->getId();
    else
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
}

/**
@short Retrieve the sampler parameter a sampler state assignment belongs to

cgGetSamplerStateAssignmentParameter() returns the "parent" parameter of a sampler state assignment, the one that was specified in cgCreateSamplerStateAssignment().

@param handle  The sampler state assignment from which to get the parent parameter

@return Returns the parent parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid sampler state assignment.</TD>
</TR>
</TABLE>

@sa       cgCreateSamplerStateAssignment
 */
CGFX_API CGparameter cgGetSamplerStateAssignmentParameter( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    CgRuntimeParameter *parent = CG_GET_STATE_ASSIGNMENT( handle )->getParentParameter();
    if ( parent ) return parent->id;
    else
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
}

/**
@short Retrieves the state of a state assignment

cgGetStateAssignmentState() returns the state of the state assignment, as specified by cgCreateStateAssignment().

@param handle The state assignment to retrieve the name from

@return Returns the handle to the state of the state assignment if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa       cgCreateStateAssignment
 */
CGFX_API CGstate cgGetStateAssignmentState( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_STATE_ASSIGNMENT( handle )->getState()->getId();
}

/**
@short Retrieves the state of a sampler state assignment

cgGetSamplerStateAssignmentState() returns the state of the state assignment, as specified by cgCreateSamplerStateAssignment().

@param handle The state assignment to retrieve the name from

@return Returns the handle to the state of the state assignment if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa       cgCreateSamplerStateAssignment
 */
CGFX_API CGstate cgGetSamplerStateAssignmentState( CGstateassignment handle )
{
    return cgGetStateAssignmentState( handle );
}

/**
@short Calls the &lsquo;set&rsquo; callback

cgCallStateSetCallback() calls the state <I>set</I> callback on a state assignment,
setting the state to the state assignment value.
The technique the state assignment is part of needs to be validated before calling
cgCallStateSetCallback().

@param handle The state assignment whose state to set

@return Returns the return value of the state <I>set</I> callback if success, <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa cgSetStateCallbacks, cgSetPassState, cgValidateTechnique
 */
CGFX_API CGbool cgCallStateSetCallback( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return CG_FALSE;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    if ( !sa->isValidated() ) return CG_FALSE;
    return sa->getState()->set( sa );
}

/**
@short Calls the &lsquo;reset&rsquo; callback

cgCallStateResetCallback() calls the state <I>reset</I> callback on a state assignment,
resetting the state to the default value.
The technique the state assignment is part of needs to be validated before calling
cgCallStateResetCallback().

@param handle The state assignment whose state to reset

@return Returns the return value of the state <I>reset</I> callback if success, <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa cgSetStateCallbacks, cgResetPassState, cgValidateTechnique
 */
CGFX_API CGbool cgCallStateResetCallback( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return CG_FALSE;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    if ( !sa->isValidated() ) return CG_FALSE;
    return sa->getState()->reset( sa );
}

/**
@short Calls the &lsquo;validate&rsquo; callback

cgCallStateValidateCallback() calls the state <I>validate</I> callback on a state assignment,
checking if the state assignment values are valid.

@param handle The state assignment whose state to reset

@return Returns the return value of the state <I>validate</I> callback if success, <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa cgSetStateCallbacks, cgValidateTechnique
 */
CGFX_API CGbool cgCallStateValidateCallback( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return CG_FALSE;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    return sa->validate();
}

/**
@short Sets a float value for a state assignment

cgSetFloatStateAssignment() sets the value of a state assignment as a single float. The
state of the state assignment needs to be of type <c>CG_FLOAT</c>, and a scalar.

@param handle The state assignment to be set
@param f      The float value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_FLOAT</c>, or is an array state.</TD>
</TR>
</TABLE>

@sa  cgGetFloatStateAssignmentValues
 */
CGFX_API CGbool cgSetFloatStateAssignment( CGstateassignment handle, float f )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::FLOAT );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    value->setFloat( f );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a int value for a state assignment

cgSetIntStateAssignment() sets the value of a state assignment as a single int. The
state of the state assignment needs to be of type <c>CG_INT</c>, and a scalar.

@param handle The state assignment to be set
@param i      The int value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_INT</c>, or is an array state.</TD>
</TR>
</TABLE>

@sa  cgGetIntStateAssignmentValues
 */
CGFX_API CGbool cgSetIntStateAssignment( CGstateassignment handle, int i )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::INT );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    value->setInt( i );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a boolean value for a state assignment

cgSetBoolStateAssignment() sets the value of a state assignment as a single boolean. The
state of the state assignment needs to be of type <c>CG_BOOL</c>, and a scalar.

@param handle The state assignment to be set
@param b      The boolean value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_BOOL</c>, or is an array state.</TD>
</TR>
</TABLE>

@sa  cgGetBoolStateAssignmentValues
 */
CGFX_API CGbool cgSetBoolStateAssignment( CGstateassignment handle, CGbool b )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::BOOL );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    value->setBool( b != 0 );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a string value for a state assignment

cgSetStringStateAssignment() sets the value of a state assignment as a single string. The
state of the state assignment needs to be of type <c>CG_STRING</c>, and a scalar.
The string passed in parameter is copied internally, so the user may free its
memory right after the call returns.

@param handle The state assignment to be set
@param s      The string to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_STRING</c>, or is an array state.</TD>
</TR>
</TABLE>

@sa  cgGetStringStateAssignmentValue
 */
CGFX_API CGbool cgSetStringStateAssignment( CGstateassignment handle, const char *s )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::STRING );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    JS_ASSERT( !value->isArray() );
    value->setString( s );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a program value for a state assignment

cgSetProgramStateAssignment() sets the value of a state assignment as a single program. The
state of the state assignment needs to be of type <c>CG_PROGRAM</c>, and a scalar.

@param handle  The state assignment to be set
@param program The program handle to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is not a valid program.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_PROGRAM</c>, or is an array state.</TD>
</TR>
</TABLE>

@sa  cgGetProgramStateAssignmentValue, cgCreateProgram
 */
CGFX_API CGbool cgSetProgramStateAssignment( CGstateassignment handle, CGprogram program )
{
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_FALSE;
    }
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::PROGRAM );
    if ( !value ) return CG_FALSE;
    JS_ASSERT( !value->isArray() );
    value->setProgram( _cgGetProgPtr( program ) );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a sampler value for a state assignment

cgSetSamplerStateAssignment() sets the value of a state assignment as a single sampler. The
state of the state assignment needs to be of type <c>CG_SAMPLER2D</c>, <c>CG_SAMPLER3D</c>, or
<c>CG_SAMPLERCUBE</c>, and a scalar.

@param handle  The state assignment to be set
@param sampler The sampler parameter handle to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>sampler</c></I> is not a valid program handle.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The type of <I><c>sampler</c></I> is not the same type as the state.</TD>
</TR>
</TABLE>

 */
CGFX_API CGbool cgSetSamplerStateAssignment( CGstateassignment handle, CGparameter sampler )
{
    if ( !CG_IS_PARAMETER( sampler ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_FALSE;
    }
    CgRuntimeParameter* param = ( CgRuntimeParameter* )_jsCgGLTestParameter( sampler );
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return CG_FALSE;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    AnyValue &value = sa->getValue();

    if (
        ( !AnyValue::isSamplerType( value.getType() ) ) ||
        ( _jsGetParameterCGtype( param->program, param->parameterEntry ) != AnyValue::getSamplerCGtypeFromType( value.getType() ) )
    )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    JS_ASSERT( !value.isArray() );
    value.setSampler( param );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a float array for a state assignment

cgSetFloatArrayStateAssignment() sets the value of a state assignment as an array of floats. The
state of the state assignment needs to be of type <c>CG_FLOAT</c>, and an array. The number of elements
in the array passed as a parameter needs to be the same as that of the state.
The values passed in parameter are copied internally, so the user may free their
memory right after the call returns.

@param handle The state assignment to be set
@param f      The float array containing the values to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_FLOAT</c>, or is not an array state.</TD>
</TR>
</TABLE>

@sa  cgGetFloatStateAssignmentValues
 */
CGFX_API CGbool cgSetFloatArrayStateAssignment( CGstateassignment handle, const float *f )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::FLOAT );
    if ( !value ) return CG_FALSE;
    if ( !value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    // XXX size ???
    memcpy( value->getArrayData(), f, value->getArraySize() );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets an int array for a state assignment

cgSetIntArrayStateAssignment() sets the value of a state assignment as an array of ints. The
state of the state assignment needs to be of type <c>CG_INT</c>, and an array. The number of elements
in the array passed as a parameter needs to be the same as that of the state.
The values passed in parameter are copied internally, so the user may free their
memory right after the call returns.

@param handle The state assignment to be set
@param i      The int array containing the values to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_INT</c>, or is not an array state.</TD>
</TR>
</TABLE>

@sa  cgGetIntStateAssignmentValues
 */
CGFX_API CGbool cgSetIntArrayStateAssignment( CGstateassignment handle, const int *i )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::INT );
    if ( !value ) return CG_FALSE;
    if ( !value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    // XXX size ???
    memcpy( value->getArrayData(), i, value->getArraySize() );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Sets a boolean array for a state assignment

cgSetBoolArrayStateAssignment() sets the value of a state assignment as an array of booleans. The
state of the state assignment needs to be of type <c>CG_BOOL</c>, and an array. The number of elements
in the array passed as a parameter needs to be the same as that of the state.
The values passed in parameter are copied internally, so the user may free their
memory right after the call returns.

@param handle The state assignment to be set
@param b      The boolean array containing the values to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_BOOL</c>, or is not an array state.</TD>
</TR>
</TABLE>

@sa  cgGetBoolStateAssignmentValues
 */
CGFX_API CGbool cgSetBoolArrayStateAssignment( CGstateassignment handle, const CGbool *b )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::BOOL );
    if ( !value ) return CG_FALSE;
    if ( !value->isArray() )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return CG_FALSE;
    }
    CGbool *array = static_cast<CGbool *>( value->getArrayData() );
    // XXX size ???
    for ( int i = 0;i < value->getElemCount();++i ) array[i] = ( b[i] != 0 );
    CG_GET_STATE_ASSIGNMENT( handle )->invalidate();
    return CG_TRUE;
}

/**
@short Retrieves the float values in a state assignment

cgGetFloatStateAssignmentValues() retrieves the values in a state assignment as an array of floats.
The state of the state assignment needs to be of type <c>CG_FLOAT</c>, but can be either a scalar or an array.
The pointer returned reference internal data, the user does not need to free it.

@param handle The state assignment to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the float values. if <I>nVals</I> is not <CODE>NULL</CODE>, the variable referenced holds the number of elements. In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> is not modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_FLOAT</c>.</TD>
</TR>
</TABLE>

@sa  cgSetFloatArrayStateAssignment, cgSetFloatStateAssignment
 */
CGFX_API const float *cgGetFloatStateAssignmentValues( CGstateassignment handle, int *nVals )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::FLOAT );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const float *>( value->getData() );
}

/**
@short Retrieves the int values in a state assignment

cgGetIntStateAssignmentValues() retrieves the values in a state assignment as an array of ints.
The state of the state assignment needs to be of type <c>CG_INT</c>, but can be either a scalar or an array.
The pointer returned reference internal data, the user does not need to free it.

@param handle The state assignment to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the int values. if <I><c>nVals</c></I> is not <CODE>NULL</CODE>, the variable referenced holds the number of elements. In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> is not modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_INT</c>.</TD>
</TR>
</TABLE>

@sa  cgSetIntArrayStateAssignment, cgSetIntStateAssignment
 */
CGFX_API const int *cgGetIntStateAssignmentValues( CGstateassignment handle, int *nVals )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::INT );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const int *>( value->getData() );
}

/**
@short Retrieves the boolean values in a state assignment

cgGetBoolStateAssignmentValues() retrieves the values in a state assignment as an array of booleans.
The state of the state assignment needs to be of type <c>CG_BOOL</c>, but can be either a scalar or an array.
The pointer returned reference internal data, the user does not need to free it.

@param handle The state assignment to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the boolean values. if <I><c>nVals</c></I> is not <CODE>NULL</CODE>, the variable referenced holds the number of elements. In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> is not modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_BOOL</c>.</TD>
</TR>
</TABLE>

@sa  cgSetBoolArrayStateAssignment, cgSetBoolStateAssignment
 */
CGFX_API const CGbool *cgGetBoolStateAssignmentValues( CGstateassignment handle, int *nVals )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::BOOL );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const CGbool *>( value->getData() );
}

/**
@short Retrieves the string value in a state assignment

cgGetStringStateAssignmentValue() retrieves the value in a state assignment as a C string.
The state of the state assignment needs to be of type <c>CG_STRING</c>.
The pointer returned reference internal data, the user does not need to free it.

@param handle The state assignment to retrieve the value from

@return In case of success, the function returns the string value. In case of failure, <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_STRING</c>.</TD>
</TR>
</TABLE>

@sa  cgSetStringStateAssignment
 */
CGFX_API const char *cgGetStringStateAssignmentValue( CGstateassignment handle )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::STRING );
    if ( !value ) return 0;
    return value->getString();
}

/**
@short Retrieves the program value in a state assignment

cgGetProgramStateAssignmentValue() retrieves the value in a state assignment as a program handle.
The state of the state assignment needs to be of type <c>CG_PROGRAM</c>.

@param handle The state assignment to retrieve the value from

@return In case of success, the function returns the program handle. In case of failure, <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_PROGRAM</c>.</TD>
</TR>
</TABLE>

@sa  cgSetProgramStateAssignment
 */
CGFX_API CGprogram cgGetProgramStateAssignmentValue( CGstateassignment handle )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::PROGRAM );
    if ( !value ) return 0;
    _CGprogram *program = value->getProgram();
    if ( program ) return program->id; else return 0;
}

/**
@short Retrieves the texture value in a state assignment

cgGetTextureStateAssignmentValue() retrieves the value in a state assignment as a texture handle.
The state of the state assignment needs to be of type <c>CG_TEXTURE</c>.

@param handle The state assignment to retrieve the value from

@return In case of success, the function returns the texture handle. In case of failure, <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The state of the state assignment is not of type <c>CG_TEXTURE</c>.</TD>
</TR>
</TABLE>

 */
CGFX_API CGparameter cgGetTextureStateAssignmentValue( CGstateassignment handle )
{
    AnyValue *value = getValueFromStateAssignmentCheckType( handle, AnyValue::TEXTURE );
    if ( !value ) return 0;
    CgRuntimeParameter *texture = value->getTexture();
    if ( texture ) return texture->id; else return 0;
}

/**
@short Retrieves the sampler value in a state assignment

cgGetSamplerStateAssignmentValue() retrieves the value in a state assignment as a sampler handle.
The state of the state assignment needs to be of type <c>CG_SAMPLER2D</c>, <c>CG_SAMPLER3D</c>, or
<c>CG_SAMPLERCUBE</c>.

@param handle The state assignment to retrieve the value from

@return In case of success, the function returns the sampler parameter handle. In case of failure, <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR</CODE></TD>
<TD>The state of the state assignment is not of one of the <c>CG_SAMPLER</c> types above.</TD>
</TR>
</TABLE>

@sa  cgSetSamplerStateAssignment
 */
CGFX_API CGparameter cgGetSamplerStateAssignmentValue( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    StateAssignment *sa = CG_GET_STATE_ASSIGNMENT( handle );
    AnyValue &value = sa->getValue();
    if ( !AnyValue::isSamplerType( value.getType() ) )
    {
        _jsCgRaiseError( CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR );
        return 0;
    }
    CgRuntimeParameter *sampler = value.getSampler();
    if ( sampler ) return sampler->id; else return 0;
}

/**
@short Retrieves the instance index in a state assignment

Some states can exist with multiple instances (for example light states, texture unit states, ...).
For each instance, there is only one CGstate representation, but the index of the instance is referenced
by the state assignment.
cgGetStateAssignmentIndex() retrieves the index of the state instance to act upon.

@param handle  Specifies the state assignment to retrieve the index from

@return Returns the index of the state instance if successful; returns
0 if an error occurred.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa cgCreateStateAssignment, cgCreateStateAssignmentIndex, cgCreateState, cgValidateTechnique
 */
CGFX_API int cgGetStateAssignmentIndex( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_STATE_ASSIGNMENT( handle )->getArrayIndex();
}

/**
@short Retrieves the number of parameters a state assignment depends on

The values of a state assignment can depend on parameter values, although this is not
available in current implementation.
cgGetNumDependentStateAssignmentParameters() retrieves the number of such parameters.
Together with cgGetDependentStateAssignmentParameter(), this is used to iterate
through all these parameters.

@param handle The state assignment to retrieve dependent parameters from

@return Returns the number of dependent parameters if success, 0 otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
</TABLE>

@sa  cgGetDependentStateAssignmentParameter
 */
CGFX_API int cgGetNumDependentStateAssignmentParameters( CGstateassignment handle )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    return 0;
}

/**
@short Retrieves a parameter a state assignment depends on, by index

A state assignment values can depend on parameter values, although this is not
available in current implementation.
cgGetDependentStateAssignmentParameter() retrieves a handle to one such parameters.
Together with cgGetNumDependentStateAssignmentParameters(), this is used to iterate
through all these parameters.

@param handle The state assignment to retrieve dependent parameters from
@param index  The index of the parameter in the dependent parameter list

@return Returns a handle to the dependent parameter if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state assignment.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not within the range [0..n-1]
where <I>n</I> is the return value of cgGetNumDependentStateAssignmentParameters().</TD>
</TR>
</TABLE>

@sa  cgGetNumDependentStateAssignmentParameters
 */
CGFX_API CGparameter cgGetDependentStateAssignmentParameter( CGstateassignment handle, int index )
{
    if ( !CG_IS_STATE_ASSIGNMENT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR );
        return 0;
    }
    _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
    return 0;
}
/**
 * @}
 */
// ------------------------------------------------------
