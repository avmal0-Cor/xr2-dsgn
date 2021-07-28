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
#include <PSGLFX/Effect.h>
#include <PSGLFX/Technique.h>
#include <PSGLFX/Pass.h>
#include <PSGLFX/Annotation.h>
#include <string.h>

namespace PSGLFX
{
Technique::Technique( Effect *effect, const char *n ):
        Named( n ),
        parent( effect ),
        next( 0 ),
        passes( 0 ),
        annotations( 0 ),
        validated( false )
{
    id = GLContextExtension::getCurrent()->addTechnique( this );
}

Technique::~Technique()
{
    if ( next ) delete next;
    if ( passes ) delete passes;
    if ( annotations ) delete annotations;
    GLContextExtension::getCurrent()->delTechnique( id );
}

void Technique::append( Technique *other )
{
    if ( next ) next->append( other );
    else next = other;
}

void Technique::appendPass( Pass *pass )
{
    if ( passes ) passes->append( pass );
    else passes = pass;
    invalidate();
}

void Technique::appendAnnotation( Annotation *annotation )
{
    if ( annotations ) annotations->append( annotation );
    else annotations = annotation;
}

bool Technique::validate()
{
    validated = true;
    for ( Pass *p = passes;p;p = p->getNext() ) validated &= p->validate();
    return validated;
}

void Technique::invalidate()
{
    validated = false;
}

bool Technique::isValidated()
{
    return validated;
}
}

using namespace PSGLFX;

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates a new technique in an effect

cgCreateTechnique() creates a new empty technique and adds it to an effect. The effect
will keep the technique creation order when iterating through the techniques with cgGetFirstTechnique()
and cgGetNextTechnique().

@param handle  Specifies the effect that the new technique will be added to.
@param name    The name of the newly created technique

@return Returns the handle to the new technique.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa cgCreateEffect, cgGetFirstTechnique, cgIsTechnique, cgDestroyEffect, cgCreateTechniqueAnnotation
 */
CGFX_API CGtechnique cgCreateTechnique( CGeffect handle, const char *name )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Effect* effect = CG_GET_EFFECT( handle );
    Technique *technique = new Technique( effect, name );
    effect->appendTechnique( technique );
    return technique->getId();
}

/**
@short Retrieve the first technique in an effect

cgGetFirstTechnique() returns the first technique in an effect. Together with
cgGetNextTechnique(), this is used to iterate through all the techniques in an effect.

@param handle  The effect to look up techniques from.

@return Returns the first technique in the effect.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa cgCreateTechnique, cgGetNextTechnique
 */
CGFX_API CGtechnique cgGetFirstTechnique( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Technique *technique = CG_GET_EFFECT( handle )->getFirstTechnique();
    return technique ? technique->getId() : 0;
}

/**
@short Retrieve the next technique in an effect

cgGetNextTechnique() returns the technique following a given technique, in an effect. Together with cgGetFirstTechnique(), this is used to iterate through all the techniques in an effect.

@param handle  The technique from which to get the successor

@return Returns the next technique in the effect list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa       cgGetFirstTechnique
 */
CGFX_API CGtechnique cgGetNextTechnique( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Technique *technique = CG_GET_TECHNIQUE( handle )->getNext();
    return technique ? technique->getId() : 0;
}

/**
@short Searches for a technique by name

cgGetNamedTechnique() looks for a technique by name, in a given effect.

@param handle The effect to look into
@param name   The name of the technique to look for

@return Returns a handle to a technique if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgCreateTechnique
 */
CGFX_API CGtechnique cgGetNamedTechnique( CGeffect handle, const char *name )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Effect *effect = CG_GET_EFFECT( handle );

    for ( Technique *technique = effect->getFirstTechnique(); technique; technique = technique->getNext() )
    {
        if ( strcmp( technique->getName(), name ) == 0 ) return technique->getId();
    }
    return NULL;
}

/**
@short Retrieves the name of a technique

cgGetTechniqueName() returns the name of the technique, as specified by cgCreateTechnique().

@param handle The technique to retrieve the name from

@return Returns the name of the technique if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa       cgCreateTechnique
 */
CGFX_API const char *cgGetTechniqueName( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_TECHNIQUE( handle )->getName();
}

/**
@short Tests if a handle represents a valid technique

cgIsTechnique() tests whether or not a CGtechnique handle is a valid technique

@param handle  The technique handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid technique, and <CODE>CG_FALSE</CODE> otherwise.

@sa       cgCreateTechnique
 */
CGFX_API CGbool cgIsTechnique( CGtechnique handle )
{
    return CG_IS_TECHNIQUE( handle );
}

/**
@short Retrieve the effect a technique belongs to

cgGetTechniqueEffect() returns the "parent" effect of a technique, the one that was specified in cgCreateTechnique().

@param handle  The technique from which to get the parent effect

@return Returns the parent effect.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa       cgCreateTechnique
 */
CGFX_API CGeffect cgGetTechniqueEffect( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_TECHNIQUE( handle )->getParent()->getId();
}

/**
@short Validates a technique

cgValidateTechnique() checks whether or not a technique contains only valid
state assignments. It iterates over all state assignments in all the passes and
call the <I>validate</I> callback on all the corresponding states, to check
whether all the states are available, and if the values to be set are valid
(valid enum, valid range, etc.).
Any time a component (pass, state assignment) of the technique is added or
changed, the technique is invalidated and the user has to validate it again.
A technique must be validated before cgSetPassState() and cgResetPassState()
can be called on its pass.

@param handle  The technique to validate

@return Returns <CODE>CG_TRUE</CODE> if the technique successfully validated,
and <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgCreateTechnique, cgIsTechniqueValidated, cgSetPassState, cgResetPassState,
cgCreatePass, cgCreateStateAssignment
 */
CGFX_API CGbool cgValidateTechnique( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return CG_FALSE;
    }

    Technique *technique = CG_GET_TECHNIQUE( handle );
    return technique->validate();
}

/**
@short Tests a technique for successful validation

cgIsTechniqueValidated() checks whether or not a technique was previously validated,
and whether or not this validation state is still current.
Any time a component (pass, state assignment) of the technique is added or
changed, the technique is invalidated and the user has to validate it again.
A technique must be validated before cgSetPassState() and cgResetPassState()
can be called on its pass.

@param handle  The technique to test for validation

@return Returns <CODE>CG_TRUE</CODE> if the technique successfully validated,
and <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgCreateTechnique, cgValidateTechnique, cgSetPassState, cgResetPassState,
cgCreatePass, cgCreateStateAssignment
 */
CGFX_API CGbool cgIsTechniqueValidated( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return CG_FALSE;
    }

    Technique *technique = CG_GET_TECHNIQUE( handle );
    return technique->isValidated();
}

/**
@short Adds a scalar annotation to a technique.

cgCreateTechniqueAnnotation() creates a new annotation and adds it to a technique.
The newly created annotation is of a scalar type, and its value can be specified
using one of the cgSet*Annotation functions.

@param handle The technique to add an annotation to
@param name   The name of the annotation to add
@param type   The type of the annotation. Must be <CODE>CG_FLOAT</CODE>, <CODE>CG_INT</CODE>, 
<CODE>CG_BOOL</CODE> or <CODE>CG_STRING</CODE>

@return Returns a handle to the created annotation if it was successfully created, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is not one of the valid types specified above.</TD>
</TR>
</TABLE>

@sa cgCreateTechnique, cgSetFloatAnnotation, cgSetIntAnnotation,
    cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues, cgGetIntAnnotationValues,
    cgGetBoolAnnotationValues, cgGetStringAnnotationValue
 */
CGFX_API CGannotation cgCreateTechniqueAnnotation( CGtechnique handle, const char *name, CGtype type )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }

    AnyValue::Type internalType = AnyValue::getAnnotationTypeFromCGtype( type );
    if ( internalType == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }

    Technique *technique = CG_GET_TECHNIQUE( handle );
    Annotation *annotation = new Annotation( name, internalType );
    technique->appendAnnotation( annotation );
    return annotation->getId();
}


/**
 * @}
 */
// ------------------------------------------------------
