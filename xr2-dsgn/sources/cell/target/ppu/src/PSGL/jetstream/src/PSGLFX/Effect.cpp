/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Effects
 */


#include <PSGL/psgl.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Effect.h>
#include <PSGLFX/Technique.h>
#include <PSGLFX/Annotation.h>
#include <PSGLFX/Parameter.h>
#include <Cg/CgCommon.h>

namespace PSGLFX
{
Effect::Effect( _CGcontext *context ):
        Named( 0 ),
        parent( context ),
        next( 0 ),
        techniques( 0 ),
        annotations( 0 )
{
    id = GLContextExtension::getCurrent()->addEffect( this );
    memset( &defaultProgram, 0, sizeof defaultProgram );
    defaultProgram.parentEffect = this;
    defaultProgram.parentContext = context;
}

Effect::~Effect()
{
    if ( techniques ) delete techniques;
    if ( annotations ) delete annotations;

    _jsCgProgramErase( &defaultProgram );

    JS_ASSERT( defaultProgram.fxParameterExtensions.size() == 0 );
    GLContextExtension::getCurrent()->delEffect( id );
}

void Effect::append( Effect *other )
{
    if ( next ) next->append( other );
    else next = other;
}

void Effect::remove( Effect *other )
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

void Effect::removeAll()
{
    if ( next )
    {
        next->removeAll();
        delete next;
        next = 0;
    }
}

void Effect::appendTechnique( Technique *technique )
{
    if ( techniques ) techniques->append( technique );
    else techniques = technique;
}

void Effect::appendAnnotation( Annotation *annotation )
{
    if ( annotations ) annotations->append( annotation );
    else annotations = annotation;
}
}

using namespace PSGLFX;

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */
/**
@short Creates an effect

cgCreateEffect() creates a new effect from scratch. In this implementation, you
cannot create an effect from cgfx code, you must fill in the effect using the
different creation functions

@param context Specifies the context that the new effect will be added to.
@param code    The code used to create this effect. This is ignored in the
               current implementation.
@param args    The arguments used to create this effect. This is ignored in the
               current implementation.

@return Returns the handle to the new effect.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgDestroyEffect, cgSetEffectName, cgIsEffect, cgCreateTechnique, cgCreateEffectAnnotation
 */
CGFX_API CGeffect cgCreateEffect( CGcontext context, const char *code, const char **args )
{
    if ( !CG_IS_CONTEXT( context ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( context );
    Effect *effect = new Effect( ctx );
    CgContextExtension::fromContext( ctx )->appendEffect( effect );
    return effect->getId();
}

/**
@short Creates an effect from a file

cgCreateEffectFromFile() creates a new effect from a file. In this implementation, you
cannot create an effect from cgfx code, so this code will always fail.

@param context  Specifies the context that the new effect will be added to.
@param filename The name of the file used to create this effect. This is ignored in the
                current implementation.
@param args     The arguments used to create this effect. This is ignored in the
                current implementation.

@return Returns the handle to the new effect.  Currently always returns 0.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>context</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateEffect, cgDestroyEffect, cgSetEffectName, cgIsEffect, cgCreateTechnique
 */
CGFX_API CGeffect cgCreateEffectFromFile( CGcontext context, const char *filename, const char **args )
{
    return 0;
}

/**
@short Creates and compile a program from an effect (Not implemented)

cgCreateProgramFromEffect() creates a new program by compiling CgFX code. This function is not implemented.

@param effect   The effect to create a program from
@param profile  The profile to use to compile the program
@param entry    The name of the entry point to compile
@param args     The arguments of the entry point

@return Returns the handle to the new program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>effect</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateProgram
 */
CGFX_API CGprogram cgCreateProgramFromEffect( CGeffect effect, CGprofile profile, const char *entry, const char **args )
{
    return 0;
}

/**
@short Destroys an effect

cgDestroyEffect() destroy an effect that was created, and all associated resources: techniques, passes, state assignments, parameters, and their annotations.

@param handle  The effect to destroy

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>effect</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateEffect, cgIsEffect
 */
CGFX_API void cgDestroyEffect( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return;
    }
    Effect *effect = CG_GET_EFFECT( handle );
    CgContextExtension::fromContext( effect->getParent() )->removeEffect( effect );
}

/**
@short Tests if a handle represents a valid effect

cgIsEffect() tests whether or not a <c>CGeffect</c> handle is a valid effect

@param handle  The effect handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid effect, and <CODE>CG_FALSE</CODE> otherwise.

@sa       cgCreateEffect, cgDestroyEffect
 */
CGFX_API CGbool cgIsEffect( CGeffect handle )
{
    return CG_IS_EFFECT( handle );
}

/**
@short Retrieve the first effect in a context

cgGetFirstEffect() returns the first effect in a context. Together with cgGetNextEffect(), this is used to iterate through all the effects in a context.

@param handle  The context to look up effects from.

@return Returns the first effect in the context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetNextEffect
 */
CGFX_API CGeffect cgGetFirstEffect( CGcontext handle )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    _CGcontext* ctx = _cgGetContextPtr( handle );
    Effect *effect = CgContextExtension::fromContext( ctx )->getFirstEffect();
    return effect ? effect->getId() : 0;
}

/**
@short Retrieve the next effect in a context

cgGetNextEffect() returns the effect following a given effect, in a context. Together with cgGetFirstEffect(), this is used to iterate through all the effects in a context.

@param handle  The effect from which to get the successor

@return Returns the next effect in the context list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetFirstEffect
 */
CGFX_API CGeffect cgGetNextEffect( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Effect *effect = CG_GET_EFFECT( handle )->getNext();
    return effect ? effect->getId() : 0;
}

/**
@short Retrieve the context an effect belongs to

cgGetEffectContext() returns the "parent" context of an effect, the one that was specified in cgCreateEffect().

@param handle  The effect from which to get the parent context

@return Returns the parent context.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateEffect
 */
CGFX_API CGcontext cgGetEffectContext( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_EFFECT( handle )->getParent()->id;
}

/**
@short Sets the name of an effect

cgSetEffectName() can be used at effect construction time, to specify the effect name.
The name can be used to look-up an effect by name, in a context, using cgGetNamedEffect().

@param handle The effect to set the name on
@param name   The name of the effect

@return Returns <CODE>CG_TRUE</CODE> if the name was successfully set, <CODE>CG_FALSE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgCreateEffect, cgGetEffectName, cgGetNamedEffect
 */
CGFX_API CGbool cgSetEffectName( CGeffect handle, const char *name )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return CG_FALSE;
    }
    CG_GET_EFFECT( handle )->setName( name );
    return CG_TRUE;
}

/**
@short Retrieves the name of an effect

cgGetEffectName() returns the name of the effect, as specified by cgSetEffectName().

@param handle The effect to retrieve the name from

@return Returns the name of the effect if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgSetEffectName
 */
CGFX_API const char *cgGetEffectName( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_EFFECT( handle )->getName();
}

/**
@short Searches for an effect by name

cgGetNamedEffect() looks for an effect by name, in a given context.

@param handle The context to look into
@param name   The name of the effect to look for

@return Returns a handle to an effect if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid context.</TD>
</TR>
</TABLE>

@sa       cgCreateEffect, cgSetEffectName
 */
CGFX_API CGeffect cgGetNamedEffect( CGcontext handle, const char *name )
{
    if ( !CG_IS_CONTEXT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return 0;
    }
    CgContextExtension *context = CgContextExtension::fromContext( _cgGetContextPtr( handle ) );

    for ( Effect *effect = context->getFirstEffect(); effect; effect = effect->getNext() )
    {
        if ( strcmp( effect->getName(), name ) == 0 ) return effect->getId();
    }
    return NULL;
}

/**
@short Adds a scalar annotation to an effect

cgCreateEffectAnnotation() creates a new annotation and adds it to an effect.
The newly created annotation is of a scalar type, and its value can be specified
using one of the cgSet*Annotation functions.

@param handle The effect to add an annotation to
@param name   The name of the annotation to add
@param type   The type of the annotation. Must be <CODE>CG_FLOAT</CODE>, <CODE>CG_INT</CODE>, <CODE>CG_BOOL</CODE> or <CODE>CG_STRING</CODE>

@return Returns a handle to the created annotation if it was successfully created, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is not one of the valid types specified above.</TD>
</TR>
</TABLE>

@sa cgCreateEffect, cgSetFloatAnnotation, cgSetIntAnnotation,
    cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues, cgGetIntAnnotationValues,
    cgGetBoolAnnotationValues, cgGetStringAnnotationValue
 */
CGFX_API CGannotation cgCreateEffectAnnotation( CGeffect handle, const char *name, CGtype type )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    AnyValue::Type internalType = AnyValue::getAnnotationTypeFromCGtype( type );
    if ( internalType == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }

    Effect *effect = CG_GET_EFFECT( handle );
    Annotation *annotation = new Annotation( name, internalType );
    effect->appendAnnotation( annotation );
    return annotation->getId();
}


/**
@short Creates a parameter in an effect

cgCreateEffectParameter() creates an effect level shared parameter. These
parameters are primarily used by connecting them to one or more program
parameters with cgConnectParameter(). This is very similar to cgCreateParameter(),
except that the parameter is owned by the effect and deleted with it.

@param handle  Specifies the effect that the new parameter will be added to.
@param name   The name of the new parameter
@param type    The type of the new parameter.

@return Returns the handle to the new parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>effect</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameter
 */
CGFX_API CGparameter cgCreateEffectParameter( CGeffect handle, const char *name, CGtype type )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    Effect *effect = CG_GET_EFFECT( handle );
    _CGprogram * program = effect->getDefaultProgram();
    CgRuntimeParameter *param = _jsCgCreateParameterInternal( program, name, type );

    CgParameterFX newParamFX;
    newParamFX.param = param;
    newParamFX.fx = new CgParameterExtension();
    program->fxParameterExtensions.push_back( newParamFX );

    return param ? param->id : NULL;
}


/**
@short Creates an array parameter in an effect (not yet implemented)

This function has not yet been implemented for PSGL.

cgCreateEffectParameterArray() creates a one-dimensional array of effect level shared
parameters.

@param effect  Specifies the effect that the new parameter will be added to.
@param name    The name of the new parameter
@param type    The type of the new parameter
@param length  The number of elements in this array

@return Returns the handle to the new parameter of type <code>CG_ARRAY</code>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>effect</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameter, cgCreateEffectParameter
 */
CGFX_API CGparameter cgCreateEffectParameterArray( CGeffect effect, const char *name,
        CGtype type, int length )
{
    if ( !CG_IS_EFFECT( effect ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    // If type is unrecognised, CG_INVALID_VALUE_TYPE_ERROR
    // if length <=0, CG_INVALID_SIZE_ERROR
    // if name==NULL, CG_INVALID_POINTER_ERROR
    // if name already used, CG_DUPLICATE_NAME_ERROR

    // TODO: *********************
    return ( CGparameter )NULL;
}


/**
@short Creates a multi-dimensional array parameter in an effect (not yet implemented)

This function has not yet been implemented for PSGL.

cgCreateEffectParameterMultiDimArray() creates an array of effect level shared
parameters. This array can have multiple dimensions, each with a different
size.

@param effect   Specifies the effect that the new parameter will be added to.
@param name     The name of the new parameter
@param type     The type of the new parameter
@param dim      The number of dimensions in this array
@param lengths  Pointer to an array of integers, one for each dimension

@return Returns the handle to the new parameter of type <code>CG_ARRAY</code>

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>effect</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgCreateParameter, cgCreateEffectParameter
 */
CGFX_API CGparameter cgCreateEffectParameterMultiDimArray( CGeffect effect, const char *name,
        CGtype type, int dim,
        const int *lengths )
{
    if ( !CG_IS_EFFECT( effect ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    // If type is unrecognised, CG_INVALID_VALUE_TYPE_ERROR
    // if length <=0, CG_INVALID_SIZE_ERROR
    // if name==NULL, CG_INVALID_POINTER_ERROR
    // if name already used, CG_DUPLICATE_NAME_ERROR

    // TODO: *********************
    return ( CGparameter )NULL;
}


/**
@short Retrieve the first parameter in an effect

cgGetFirstEffectParameter() returns the first parameter in an effect. Together
with cgGetNextParameter(), this is used to iterate through all the parameters in an effect.

@param handle  The effect to look up parameters from.

@return Returns the first parameter in the effect.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetNextParameter, cgCreateEffectParameter
 */
CGFX_API CGparameter cgGetFirstEffectParameter( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    Effect *effect = CG_GET_EFFECT( handle );
    _CGprogram * program = effect->getDefaultProgram();

    if ( program->fxParameterExtensions.size() == 0 )
        return NULL;

    CgRuntimeParameter *param = program->fxParameterExtensions[0].param;
    return param ? param->id : NULL;
}


/**
@short Searches for a parameter by name in an effect

cgGetNamedEffectParameter() looks for a parameter by name, in a given effect.

@param handle The effect to look into
@param name   The name of the parameter to look for

@return Returns a handle to a parameter if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgCreateEffectParameter

@note This function is not fully implemented, and is currently unsupported.
 */
CGFX_API CGparameter cgGetNamedEffectParameter( CGeffect handle, const char  *name )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    Effect *effect = CG_GET_EFFECT( handle );
    _CGprogram * program = effect->getDefaultProgram();
    return cgGetNamedProgramParameter( program, CG_GLOBAL, name );
}

/**
@short Returns the first leaf parameter in an effect

cgGetFirstLeafEffectParameter() returns the first leaf parameter in an effect.
In the current implementation, since there is no way to create a leaf parameter
in an effect, this always returns <c>NULL</c>.

@param handle The effect to look into

@return Returns the handle to the first leaf parameter if any, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgCreateEffectParameter
 */
CGFX_API CGparameter cgGetFirstLeafEffectParameter( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    return NULL;
}

#include <stdio.h>

/**
@short Searches for a parameter by semantic in an effect

cgGetEffectParameterBySemantic() looks for a parameter by semantic, in a given effect.

@param handle The effect to look into
@param semantic   The name of the parameter semantic to look for

@return Returns a handle to a parameter if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa       cgCreateEffectParameter
 */
CGFX_API CGparameter cgGetEffectParameterBySemantic( CGeffect handle, const char *semantic )
{
    if ( semantic == NULL )
        return NULL;

    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }

    Effect *effect = CG_GET_EFFECT( handle );
    _CGprogram * progPtr = effect->getDefaultProgram();

    // all effect semantics are runtime allocated
    STL_NAMESPACE vector<CgRuntimeSemantic>::iterator semIter = progPtr->parameterSemantics.begin();
    while ( semIter != progPtr->parameterSemantics.end() )
    {
        if ( !strcmp( &semIter->semantic[0], semantic ) )
            return semIter->param->id;
        semIter++;
    }

    return NULL;
}


/**
@short Get the parent effect of a parameter 

cgGetParameterEffect() allows the application to retrieve a handle to
the effect that a given parameter belongs to.

@param       param   Specifies the parameter.

@return Returns a <I>CGeffect</I> handle to the parent effect, if one exists.
In the event of an error <CODE>NULL</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> does not refer to a valid parameter object.</TD>
</TR>
</TABLE>

@sa       cgGetParameterProgram, cgGetParameterContext
*/
CGFX_API CGeffect cgGetParameterEffect( CGparameter param )
{
    // We return efect for parameter created using cgCreateEffectParameter()

    // check parameter handle
    if ( !CG_IS_PARAMETER( param ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return ( CGeffect )NULL;
    }

    CgRuntimeParameter* ptr = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    _CGprogram *program = ptr->program;
    if ( program->parentEffect )
    {
        return (( Effect* )( program->parentEffect ) )->getId();
    }

    return ( CGeffect )NULL;
}



/**
 * @}
 */
// ------------------------------------------------------
