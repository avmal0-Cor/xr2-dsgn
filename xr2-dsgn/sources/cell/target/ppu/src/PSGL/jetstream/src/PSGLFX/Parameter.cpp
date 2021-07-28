/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Parameter FX extensions
 */


#include <PSGL/private.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Parameter.h>
#include <PSGLFX/StateAssignment.h>
#include <PSGLFX/Annotation.h>
#include <PSGLFX/AnyValue.h>
#include <PSGLFX/State.h>
#include <Cg/CgCommon.h>
#include <string.h>


namespace PSGLFX
{

CgParameterExtension::CgParameterExtension():
        annotations( 0 ),
        stateAssignments( 0 )
{}

CgParameterExtension::CgParameterExtension( const CgParameterExtension &other ):
        annotations( 0 ),
        stateAssignments( 0 )
{
    for ( Annotation *annotation = other.getFirstAnnotation();annotation;annotation = annotation->getNext() )
    {
        appendAnnotation( new Annotation( *annotation ) );
    }
    for ( StateAssignment *stateAssignment = other.getFirstStateAssignment();stateAssignment;stateAssignment = stateAssignment->getNext() )
    {
        appendStateAssignment( new StateAssignment( *stateAssignment ) );
    }
}

CgParameterExtension::~CgParameterExtension()
{
    if ( stateAssignments ) delete stateAssignments;
    if ( annotations ) delete annotations;
}

void CgParameterExtension::appendStateAssignment( StateAssignment *stateAssignment )
{
    if ( stateAssignments ) stateAssignments->append( stateAssignment );
    else stateAssignments = stateAssignment;
}

void CgParameterExtension::appendAnnotation( Annotation *annotation )
{
    if ( annotations ) annotations->append( annotation );
    else annotations = annotation;
}

void CgParameterExtension::setState()
{
    for ( StateAssignment *s = stateAssignments;s;s = s->getNext() ) s->getState()->set( s );
}

}

using namespace PSGLFX;

static inline bool isSamplerParameter( CgRuntimeParameter *param )
{
    CGtype paramtype = _jsGetParameterCGtype( param->program, param->parameterEntry );
    switch ( paramtype )
    {
        case CG_SAMPLER1D:
        case CG_SAMPLER2D:
        case CG_SAMPLER3D:
        case CG_SAMPLERRECT:
        case CG_SAMPLERCUBE:
            return true;
        default:
            return false;
    }
}

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Adds a scalar annotation to a parameter

cgCreateParameterAnnotation() creates a new annotation and adds it to a parameter.
The newly created annotation is of a scalar type, and its value can be specified
using one of the cgSet*Annotation functions.

@param handle The parameter to add an annotation to
@param name   The name of the annotation to add
@param type   The type of the annotation. Must be <CODE>CG_FLOAT</CODE>, <CODE>CG_INT</CODE>, 
<CODE>CG_BOOL</CODE> or <CODE>CG_STRING</CODE>

@return Returns a handle to the created annotation if it was successfully created, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is not one of the valid types specified above.</TD>
</TR>
</TABLE>

@sa cgCreateParameter, cgSetFloatAnnotation, cgSetIntAnnotation,
    cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues, cgGetIntAnnotationValues,
    cgGetBoolAnnotationValues, cgGetStringAnnotationValue
 */
CGFX_API CGannotation cgCreateParameterAnnotation( CGparameter handle, const char *name, CGtype type )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }
    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    AnyValue::Type internalType = AnyValue::getAnnotationTypeFromCGtype( type );
    if ( internalType == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }

    CgParameterExtension *extension = CgParameterExtension::fromParameterSafe( param );
    Annotation *annotation = new Annotation( name, internalType );
    extension->appendAnnotation( annotation );
    return annotation->getId();
}

/**
@short Creates a new state assignment in a sampler parameter

cgCreateStateAssignment() creates a new state assignment and adds it to a sampler parameter.
The sampler will keep the state assignment creation order when iterating through the state
assignments with cgSetSamplerState().

@param parameterHandle  Specifies the sampler parameter the new state assignment will be added to.
@param stateHandle      The state the newly created state assignment will act on

@return Returns the handle to the new state assignment.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>parameterHandle</c></I> is not a valid parameter, or is not of one of the <c>CG_SAMPLER</c> types.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_STATE_HANDLE_ERROR</CODE></TD>
<TD><I><c>stateHandle</c></I> is not a valid state.</TD>
</TR>
</TABLE>

@sa cgCreateState, cgCreateStateAssignmentIndex, cgGetFirstSamplerStateAssignment, cgIsStateAssignment
 */
CGFX_API CGstateassignment cgCreateSamplerStateAssignment( CGparameter parameterHandle, CGstate stateHandle )
{
    if ( !CG_IS_PARAMETER( parameterHandle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }

    if ( !CG_IS_STATE( stateHandle ) )
    {
        _jsCgRaiseError( CG_INVALID_STATE_HANDLE_ERROR );
        return 0;
    }

    // XXX check that state is a sampler state ???

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( parameterHandle );
    if ( !isSamplerParameter( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }
    State *state = CG_GET_STATE( stateHandle );
    CgParameterExtension *extension = CgParameterExtension::fromParameterSafe( param );
    StateAssignment *stateAssignment = new StateAssignment( param, state );
    extension->appendStateAssignment( stateAssignment );
    return stateAssignment->getId();
}

/**
@short Retrieve the first state assignment in a sampler parameter

cgGetFirstSamplerStateAssignment() returns the first state assignment in a sampler parameter.
Together with cgGetNextStateAssignment(), this is used to iterate through all the state
assignments in a sampler.

@param handle  The sampler parameter to look up state assignments from.

@return Returns the first state assignment in the sampler.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter,  or is not of one of the <CODE>CG_SAMPLER</CODE> types.</TD>
</TR>
</TABLE>

@sa cgCreateSamplerStateAssignment, cgGetNextStateAssignment
 */
CGFX_API CGstateassignment cgGetFirstSamplerStateAssignment( CGparameter handle )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    CgParameterExtension *extension = CgParameterExtension::fromParameter( param );
    if ( !extension ) return 0;

    JS_ASSERT( isSamplerParameter( param ) );

    StateAssignment *stateAssignment = extension->getFirstStateAssignment();
    return stateAssignment ? stateAssignment->getId() : 0;
}

/**
@short Searches for a sampler state assignment by state name

cgGetNamedSamplerStateAssignment() looks for a state assignment by state name, in a given sampler parameter.

@param handle The sampler to look into
@param name   The name of the state set by the state assignment to look for

@return Returns a handle to a state assignment if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter,  or is not of one of the <CODE>CG_SAMPLER</CODE> types.</TD>
</TR>
</TABLE>

@sa cgCreateSamplerStateAssignment
 */
CGFX_API CGstateassignment cgGetNamedSamplerStateAssignment( CGparameter handle, const char *name )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    CgParameterExtension *extension = CgParameterExtension::fromParameter( param );
    if ( !extension ) return 0;

    JS_ASSERT( isSamplerParameter( param ) );

    for ( StateAssignment *stateAssignment = extension->getFirstStateAssignment(); stateAssignment; stateAssignment = stateAssignment->getNext() )
    {
        if ( strcmp( stateAssignment->getState()->getName(), name ) == 0 ) return stateAssignment->getId();
    }
    return 0;
}

/**
@short Sets all the states in a sampler to their assigned values

cgSetSamplerState() iterates through all the states assignments in a sampler parameter, in
creation order, and sets each corresponding state to their assigned value, by
calling the <I>set</I> callback of that state.
For PSGL, there is no distinction between a texture and a sampler. As a result,
the PSGL texture name that the sampler represents needs to be bound to the current texture unit
before cgSetSamplerState() is called.

@param handle  The sampler parameter from which the states have to be set

@note
All of the states set by cgSetSamplerState() are kept in the texture object.
After you call cgSetSamplerState() at initialization, you do not need to call it
every time you draw with the texture unless you modify the state assignment values.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter,  or is not of one of the <CODE>CG_SAMPLER</CODE> types.</TD>
</TR>
</TABLE>

@sa cgCreateStateAssignment, cgSetStateCallbacks, glBindTexture, glActiveTexture, cgGLEnableTextureParameter
 */
CGFX_API void cgSetSamplerState( CGparameter handle )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return;
    }

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    CgParameterExtension *extension = CgParameterExtension::fromParameter( param );
    if ( !extension ) return;

    JS_ASSERT( isSamplerParameter( param ) );

    extension->setState();
}
/**
 * @}
 */
// ------------------------------------------------------
