/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - States
 */

#include <PSGL/psgl.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/State.h>
#include <PSGLFX/StateAssignment.h>
#include <string.h>

namespace PSGLFX
{
State::State( _CGcontext *context, const char *n, AnyValue::Type t, int nelem ):
        Named( n ),
        parent( context ),
        next( 0 ),
        setCallback( 0 ),
        resetCallback( 0 ),
        validateCallback( 0 ),
        type( t ),
        elemCount( nelem ),
        setFunction( &State::defaultNoopFunction ),
        resetFunction( &State::defaultNoopFunction ),
        validateFunction( &State::defaultNoopFunction )
{
    id = GLContextExtension::getCurrent()->addState( this );
}

State::~State()
{
    GLContextExtension::getCurrent()->delState( id );
}

void State::append( State *other )
{
    if ( next ) next->append( other );
    else next = other;
}

void State::remove( State *other )
{
    JS_ASSERT( other );
    if ( next == other )
    {
        next = other->next;
        other->next = 0;
        delete other;
    }
    else if ( next ) next->remove( other );
}

void State::removeAll()
{
    if ( next )
    {
        next->removeAll();
        delete next;
        next = 0;
    }
}

bool State::defaultNoopFunction( StateAssignment *sa )
{
    return true;
}

bool State::defaultValidateFunction( StateAssignment *sa )
{
    return sa->getState()->validateCallback( sa->getId() );
}

bool State::defaultSetFunction( StateAssignment *sa )
{
    return sa->getState()->setCallback( sa->getId() );
}

bool State::defaultResetFunction( StateAssignment *sa )
{
    return sa->getState()->resetCallback( sa->getId() );
}
}

using namespace PSGLFX;


// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates a new scalar pass state.

cgCreateState() creates a new empty scalar state in a context. The newly created
state is empty, but the user can add enumerant mappings with cgAddStateEnumerant(),
and set state callbacks with cgSetStateCallbacks(). This state can be used in a pass.
The state is destroyed along with the Cg context.

@param context Specifies the context that the new state will be added to.
@param name    The name of the newly created state
@param cgType  The type of the newly created state. 
Must be one of <c>CG_FLOAT</c>, <c>CG_INT</c>, <c>CG_BOOL</c>, <c>CG_PROGRAM_TYPE</c>, <c>CG_TEXTURE</c>, 
<c>CG_SAMPLER2D</c>, <c>CG_SAMPLER3D</c>, or <c>CG_SAMPLERCUBE</c>

@return Returns the handle to the new state.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>cgType</c></I> is not one of the above types.</TD>
</TR>
</TABLE>

@sa cgCreateArrayState, cgSetStateCallbacks, cgAddStateEnumerant, cgGetFirstState,
cgIsState, cgCreateSamplerState, cgCreateStateAssignment
 */
CGFX_API CGstate cgCreateState( CGcontext context, const char *name, CGtype cgType )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    AnyValue::Type type = AnyValue::getStateTypeFromCGtype( cgType );
    if ( type == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( context );
    State *state = new State( ctx, name, type );
    CgContextExtension::fromContext( ctx )->appendState( state );
    return state->getId();
}

/**
@short Creates a new array pass state

cgCreateState() creates a new empty array state in a context. The newly created
state is empty, but the user can add enumerant mappings with cgAddStateEnumerant(),
and set state callbacks with cgSetStateCallbacks(). This state can be used in a pass.
The state is destroyed along with the Cg context.

@param context Specifies the context that the new state will be added to.
@param name    The name of the newly created state
@param cgType  The type of the newly created state. Must be one of <c>CG_FLOAT</c>, <c>CG_INT</c>, or <c>CG_BOOL</c>
@param nelems  The number of elements in the state

@return Returns the handle to the new state.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>cgType</c></I> is not one of the above types.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgSetStateCallbacks, cgAddStateEnumerant, cgGetFirstState,
cgIsState, cgCreateArraySamplerState, cgCreateStateAssignment
 */
CGFX_API CGstate cgCreateArrayState( CGcontext context, const char *name, CGtype cgType, int nelems )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    AnyValue::Type type = AnyValue::getStateTypeFromCGtype( cgType );
    switch ( type )
    {
        case AnyValue::FLOAT:
        case AnyValue::INT:
        case AnyValue::BOOL:
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
            return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( context );
    State *state = new State( ctx, name, type, nelems );
    CgContextExtension::fromContext( ctx )->appendState( state );
    return state->getId();
}

/**
@short Tests if a handle represents a valid state

cgIsState() tests whether or not a <c>CGstate</c> handle is a valid state.

@param handle  The state handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid state, and <CODE>CG_FALSE</CODE> otherwise.

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState
 */
CGFX_API CGbool cgIsState( CGstate handle )
{
    return CG_IS_STATE( handle );
}

/**
@short Retrieve the first pass state in a context

cgGetFirstState() returns the first pass state in a context. Together with
cgGetNextState(), this is used to iterate through all the pass states in a context.

@param handle  The context to look up states from.

@return Returns the first pass state in the context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid context.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgGetNextState
 */
CGFX_API CGstate cgGetFirstState( CGcontext handle )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( handle );
    State *state = CgContextExtension::fromContext( ctx )->getFirstState();
    return state ? state->getId() : 0;
}

/**
@short Retrieve the next state in a context

cgGetNextState() returns the state following a given state (or sampler state), in a context. Together with cgGetFirstState() and cgGetFirstSamplerState(), this is used to iterate through all the states and sampler states in a context. There are two separate lists in the context, one for pass states, one for sampler states.

@param handle  The state (or sampler state) from which to get the successor

@return Returns the next state in the context list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgGetFirstState, cgGetFirstSamplerState
 */
CGFX_API CGstate cgGetNextState( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    State *state = CG_GET_STATE( handle )->getNext();
    return state ? state->getId() : 0;
}

/**
@short Retrieve the context a state belongs to

cgGetStateContext() returns the "parent" context of a state, meaning the one that was specified in cgCreateState()

@param handle  The state from which to get the parent context

@return Returns the parent context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa       cgCreateState
 */
CGFX_API CGcontext cgGetStateContext( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_STATE( handle )->getParent()->id;
}

/**
@short Retrieves the name of a state

cgGetStateName() returns the name of the state, as specified at creation time.

@param handle The state to retrieve the name from

@return Returns the name of the state if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState
 */
CGFX_API const char *cgGetStateName( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_STATE( handle )->getName();
}

/**
@short Searches for a pass state by name

cgGetNamedState() looks for a pass state by name, in a given context.

@param handle The context to look into
@param name   The name of the state to look for

@return Returns a handle to a state if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid context.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState
 */
CGFX_API CGstate cgGetNamedState( CGcontext handle, const char *name )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    CgContextExtension *context = CgContextExtension::fromContext( _cgGetContextPtr( handle ) );

    for ( State *state = context->getFirstState(); state; state = state->getNext() )
    {
        if ( strcmp( state->getName(), name ) == 0 ) return state->getId();
    }
    return NULL;
}

/**
@short Retrieves the type of a state

cgGetStateType() returns the type of the state, as specified at creation time.

@param handle The state to retrieve the name from

@return Returns the type of the state if success, <CODE>CG_UNKNOWN_TYPE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState
 */
CGFX_API CGtype cgGetStateType( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return CG_UNKNOWN_TYPE;
    }
    return AnyValue::getCGtypeFromType( CG_GET_STATE( handle )->getType() );
}

/**
@short Creates a new scalar sampler state

cgCreateState() creates a new empty scalar state in a context. The newly created
state is empty, but the user can add enumerant mappings with cgAddStateEnumerant(),
and set state callbacks with cgSetStateCallbacks(). This state can be used in a sampler parameter.
The state is destroyed along with the Cg context.

@param context Specifies the context that the new state will be added to.
@param name    The name of the newly created state
@param cgType  The type of the newly created state. Must be one of <c>CG_FLOAT</c>, <c>CG_INT</c>, <c>CG_BOOL</c>, <c>CG_PROGRAM_TYPE</c>,
<c>CG_TEXTURE</c>, <c>CG_SAMPLER2D</c>, <c>CG_SAMPLER3D</c>, or <c>CG_SAMPLERCUBE</c>

@return Returns the handle to the new state.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>cgType</c></I> is not one of the above types.</TD>
</TR>
</TABLE>

@sa cgCreateArraySamplerState, cgSetStateCallbacks, cgAddStateEnumerant, cgGetFirstState,
cgIsState, cgCreateState, cgCreateSamplerStateAssignment
 */
CGFX_API CGstate cgCreateSamplerState( CGcontext context, const char *name, CGtype cgType )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    AnyValue::Type type = AnyValue::getStateTypeFromCGtype( cgType );
    if ( type == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( context );
    State *state = new State( ctx, name, type );
    CgContextExtension::fromContext( ctx )->appendSamplerState( state );
    return state->getId();
}

/**
@short Creates a new array sampler state

cgCreateArraySamplerState() creates a new empty array state in a context. The newly created
state is empty, but the user can add enumerant mappings with cgAddStateEnumerant(),
and set state callbacks with cgSetStateCallbacks(). This state can be used in a sampler parameter.
The state is destroyed along with the Cg context.

@param context Specifies the context that the new state will be added to.
@param name    The name of the newly created state
@param cgType  The type of the newly created state. Must be one of <c>CG_FLOAT</c>, <c>CG_INT</c>, or <c>CG_BOOL</c>
@param nelems  The number of elements in the state

@return Returns the handle to the new state.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is not a valid context.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>cgType</c></I> is not one of the above types.</TD>
</TR>
</TABLE>

@sa cgCreateSamplerState, cgSetStateCallbacks, cgAddStateEnumerant, cgGetFirstState,
cgIsState, cgCreateArrayState, cgCreateSamplerStateAssignment
 */
CGFX_API CGstate cgCreateArraySamplerState( CGcontext context, const char *name, CGtype cgType, int nelems )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    AnyValue::Type type = AnyValue::getStateTypeFromCGtype( cgType );
    switch ( type )
    {
        case AnyValue::FLOAT:
        case AnyValue::INT:
        case AnyValue::BOOL:
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
            return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( context );
    State *state = new State( ctx, name, type, nelems );
    CgContextExtension::fromContext( ctx )->appendSamplerState( state );
    return state->getId();
}

/**
@short Retrieve the first sampler state in a context

cgGetFirstSamplerState() returns the first state in a context. Together with
cgGetNextState(), this is used to iterate through all the states in a context.

@param handle  The context to look up states from.

@return Returns the first state in the context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid context.</TD>
</TR>
</TABLE>

@sa cgCreateSamplerState, cgGetNextState
 */
CGFX_API CGstate cgGetFirstSamplerState( CGcontext handle )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( handle );
    State *state = CgContextExtension::fromContext( ctx )->getFirstSamplerState();
    return state ? state->getId() : 0;
}

/**
@short Searches for a sampler state by name

cgGetNamedState() looks for a sampler state by name, in a given context.

@param handle The context to look into
@param name   The name of the state to look for

@return Returns a handle to a state if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid context.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState
 */
CGFX_API CGstate cgGetNamedSamplerState( CGcontext handle, const char *name )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    CgContextExtension *context = CgContextExtension::fromContext( _cgGetContextPtr( handle ) );

    for ( State *state = context->getFirstSamplerState(); state; state = state->getNext() )
    {
        if ( strcmp( state->getName(), name ) == 0 ) return state->getId();
    }
    return NULL;
}

/**
@short Sets the callback functions on a state

cgSetStateCallbacks() sets the different callback functions on a state. There
are 3 kinds of callback functions:
<ul>
<li>a <I><c>set</c></I> callback, that is called from cgSetPassState()</li>
<li>a <I><c>reset</c></I> callback, that is called from cgResetPassState()</li>
<li>a <I><c>validate</c></I> callback, that is called from cgValidateTechnique()</li>
</ul>

@param handle   The state on which to change the callbacks
@param set      The <I>set</I> callback function
@param reset    The <I>reset</I> callback function
@param validate The <I>validate</I> callback function

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgSetPassState, cgResetPassState, cgValidateTechnique, cgGetStateResetCallback,  cgGetStateSetCallback, cgGetStateValidateCallback
 */
CGFX_API void cgSetStateCallbacks( CGstate handle, CGstatecallback set, CGstatecallback reset, CGstatecallback validate )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return;
    }
    State *state = CG_GET_STATE( handle );
    state->setCallbacks( set, reset, validate );
}


/**
@short  Gets the reset callback function for a state

cgGetStateResetCallback() returns the reset callback function for a state.

@param handle   The state on which to read the callbacks

@return Returns a pointer to the registered callback function.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgSetPassState, cgResetPassState, cgValidateTechnique, cgGetStateSetCallback, cgGetStateValidateCallback
 */
CGFX_API CGstatecallback cgGetStateResetCallback( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return NULL;
    }
    State *state = CG_GET_STATE( handle );
    return state->getResetCallback();
}

/**
@short Gets the set callback function for a state

cgGetStateSetCallback() returns the set callback function for a state.

@param handle   The state on which to read the callback function

@return Returns a pointer to the registered callback function.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgSetPassState, cgResetPassState, cgValidateTechnique, cgGetStateResetCallback, cgGetStateValidateCallback
 */
CGFX_API CGstatecallback cgGetStateSetCallback( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return NULL;
    }
    State *state = CG_GET_STATE( handle );
    return state->getSetCallback();
}


/**
@short Gets the validate callback function for a state

cgGetStateValidateCallback() returns the validate callback function for a state.

@param handle   The state on which to read the callback function

@return Returns a pointer to the registered callback function.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgSetPassState, cgResetPassState, cgValidateTechnique, cgGetStateSetCallback, cgGetStateResetCallback
 */
CGFX_API CGstatecallback cgGetStateValidateCallback( CGstate handle )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return NULL;
    }
    State *state = CG_GET_STATE( handle );
    return state->getValidateCallback();
}



/**
@short Adds an enumerant mapping in a state

cgAddStateEnumerant() adds a state-specific binding (name,value) for enumerants,
to be able to look them up by name or value. This is useful to create state
assignments from textual symbolic constants instead of numeric values

@param handle   The state to associate the name and value with.
@param name     The name of the enumerant
@param value    The enumerant numeric value

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgGetStateEnumerantName, cgGetStateEnumerantValue, cgSetIntStateAssignment
 */
CGFX_API void cgAddStateEnumerant( CGstate handle, const char *name, int value )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return;
    }
    State *state = CG_GET_STATE( handle );
    state->addEnum( value, name );
}

/**
@short Looks up an enumerant mapping by value

cgGetStateEnumerantName() looks for an enumerant by value, and returns its name.

@param handle   The state on which to find the enumerant
@param value    The enumerant numeric value to look for

@return Returns the name of the enumerant if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>value</c></I> doesn&rsquo;t map to an enumerant name.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgAddStateEnumerant, cgGetStateEnumerantValue
 */
CGFX_API const char *cgGetStateEnumerantName( CGstate handle, int value )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    State *state = CG_GET_STATE( handle );
    const char *name = 0;
    bool r = state->findEnumNameByValue( value, name );
    if ( !r ) _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
    return name;
}

/**
@short Looks up an enumerant value

cgGetStateEnumerantValue() looks for an enumerant by name, and returns its value.

@param handle   The state on which to find the enumerant
@param name     The enumerant name to look for

@return Returns the value of the enumerant if success, 0 otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid state.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>value</c></I> doesn&rsquo;t map to an enumerant name.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateArrayState, cgCreateSamplerState, cgCreateArraySamplerState,
cgAddStateEnumerant, cgGetStateEnumerantName, cgSetIntStateAssignment
 */
CGFX_API int cgGetStateEnumerantValue( CGstate handle, const char *name )
{
    if ( !CG_IS_STATE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }
    State *state = CG_GET_STATE( handle );
    GLenum value = 0;
    bool r = state->findEnumValueByName( value, name );
    if ( !r ) _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
    return value;
}
/**
 * @}
 */
// ------------------------------------------------------
