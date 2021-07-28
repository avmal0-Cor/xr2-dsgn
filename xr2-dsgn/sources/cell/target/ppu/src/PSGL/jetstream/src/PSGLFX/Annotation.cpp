/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Annotations
 */

#include <PSGL/psgl.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Annotation.h>
#include <PSGLFX/Effect.h>
#include <PSGLFX/Technique.h>
#include <PSGLFX/Pass.h>
#include <PSGLFX/Parameter.h>
#include <PSGLFX/Program.h>
#include <string.h>

namespace PSGLFX
{
Annotation::Annotation( const char *n, AnyValue::Type type, int nelem ):
        Named( n ),
        next( 0 ),
        value( type, nelem )
{
    id = GLContextExtension::getCurrent()->addAnnotation( this );
}

Annotation::Annotation( const Annotation &other ):
        Named( other.getName() ),
        next( 0 ),
        value( other.value )
{
    id = GLContextExtension::getCurrent()->addAnnotation( this );
}

Annotation::~Annotation()
{
    if ( next ) delete next;
    GLContextExtension::getCurrent()->delAnnotation( id );
}

void Annotation::append( Annotation *other )
{
    if ( next ) next->append( other );
    else next = other;
}
}

using namespace PSGLFX;

static inline CGannotation getNamedAnnotation( Annotation *first, const char *name )
{
    for ( Annotation *annotation = first; annotation; annotation = annotation->getNext() )
    {
        if ( strcmp( annotation->getName(), name ) == 0 ) return annotation->getId();
    }
    return NULL;
}

static inline AnyValue *getValueFromAnnotationCheckType( CGannotation handle, AnyValue::Type type )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_ANNOTATION( handle );
    AnyValue &value = annotation->getValue();
    if ( value.getType() != type )
    {
        _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
        return 0;
    }
    return &value;
}

// API functions ----------------------------------------

/** @addtogroup PSGLFX
 *@{
 */

/**
@short Retrieve the first annotation in an effect

cgGetFirstEffectAnnotation() returns the first annotation in an effect.
Together with cgGetNextAnnotation(), this is used to iterate through all
the annotations in an effect.

@param handle  The effect to look up annotations from.

@return Returns the first annotation in the effect.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa cgGetNextAnnotation, cgCreateEffectAnnotation
 */
CGFX_API CGannotation cgGetFirstEffectAnnotation( CGeffect handle )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_EFFECT( handle )->getFirstAnnotation();
    return annotation ? annotation->getId() : 0;
}

/**
@short Retrieve the first annotation in a technique

cgGetFirstTechniqueAnnotation() returns the first annotation in a technique.
Together with cgGetNextAnnotation(), this is used to iterate through all
the annotations in a technique.

@param handle  The technique to look up annotations from.

@return Returns the first annotation in the technique.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgGetNextAnnotation, cgCreateTechniqueAnnotation
 */
CGFX_API CGannotation cgGetFirstTechniqueAnnotation( CGtechnique handle )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_TECHNIQUE( handle )->getFirstAnnotation();
    return annotation ? annotation->getId() : 0;
}

/**
@short Retrieve the first annotation in a pass

cgGetFirstPassAnnotation() returns the first annotation in a pass.
Together with cgGetNextAnnotation(), this is used to iterate through all
the annotations in a pass.

@param handle  The pass to look up annotations from.

@return Returns the first annotation in the pass.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa cgGetNextAnnotation, cgCreatePassAnnotation
 */
CGFX_API CGannotation cgGetFirstPassAnnotation( CGpass handle )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_PASS( handle )->getFirstAnnotation();
    return annotation ? annotation->getId() : 0;
}

/**
@short Retrieve the first annotation in a parameter

cgGetFirstParameterAnnotation() returns the first annotation in a parameter.
Together with cgGetNextAnnotation(), this is used to iterate through all
the annotations in a parameter.

@param handle  The parameter to look up annotations from.

@return Returns the first annotation in the parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetNextAnnotation, cgCreateParameterAnnotation
 */
CGFX_API CGannotation cgGetFirstParameterAnnotation( CGparameter handle )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    CgParameterExtension *extension = CgParameterExtension::fromParameter( param );
    if ( !extension ) return 0;

    Annotation *annotation = extension->getFirstAnnotation();
    return annotation ? annotation->getId() : 0;
}

/**
@short Retrieve the first annotation in a program

cgGetFirstProgramAnnotation() returns the first annotation in a program.
Together with cgGetNextAnnotation(), this is used to iterate through all
the annotations in a program.

@param handle  The program to look up annotations from.

@return Returns the first annotation in the program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid program.</TD>
</TR>
</TABLE>

@sa cgGetNextAnnotation, cgCreateProgramAnnotation
*/
CGFX_API CGannotation cgGetFirstProgramAnnotation( CGprogram handle )
{
    if ( !CG_IS_PROGRAM( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return 0;
    }

    _CGprogram *prog = _cgGetProgPtr( handle );

    CgProgramExtension *extension = CgProgramExtension::fromProgram( prog );
    if ( !extension ) return 0;

    Annotation *annotation = extension->getFirstAnnotation();
    return annotation ? annotation->getId() : 0;
}

/**
@short Retrieve the next annotation

cgGetNextAnnotation() returns the annotation following a given annotation,
in an effect, technique, pass, program or parameter. Together with
cgGetFirst*Annotation(), this is used to iterate through all the annotations
in one of these objects.

@param handle  The annotation from which to get the successor

@return Returns the next annotation in the list.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
</TABLE>

@sa cgGetFirstEffectAnnotation, cgGetFirstTechniqueAnnotation, cgGetFirstPassAnnotation, cgGetFirstParameterAnnotation, cgGetFirstProgramAnnotation
 */
CGFX_API CGannotation cgGetNextAnnotation( CGannotation handle )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_ANNOTATION( handle )->getNext();
    return annotation ? annotation->getId() : 0;
}

/**
@short Searches for an annotation by name in an effect

cgGetNamedEffectAnnotation() looks for an annotation by name, in a given effect.

@param handle The effect to look into
@param name   The name of the annotation to look for

@return Returns a handle to an annotation if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_EFFECT_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid effect.</TD>
</TR>
</TABLE>

@sa cgGetFirstEffectAnnotation, cgCreateEffectAnnotation
 */
CGFX_API CGannotation cgGetNamedEffectAnnotation( CGeffect handle, const char *name )
{
    if ( !CG_IS_EFFECT( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_EFFECT_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_EFFECT( handle )->getFirstAnnotation();
    return getNamedAnnotation( annotation, name );
}

/**
@short Searches for an annotation by name in a technique

cgGetNamedTechniqueAnnotation() looks for an annotation by name, in a given technique.

@param handle The technique to look into
@param name   The name of the annotation to look for

@return Returns a handle to an annotation if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_TECHNIQUE_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid technique.</TD>
</TR>
</TABLE>

@sa cgGetFirstTechniqueAnnotation, cgCreateTechniqueAnnotation
 */
CGFX_API CGannotation cgGetNamedTechniqueAnnotation( CGtechnique handle, const char *name )
{
    if ( !CG_IS_TECHNIQUE( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_TECHNIQUE_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_TECHNIQUE( handle )->getFirstAnnotation();
    return getNamedAnnotation( annotation, name );
}

/**
@short Searches for an annotation by name in a pass

cgGetNamedPassAnnotation() looks for an annotation by name, in a given pass.

@param handle The pass to look into
@param name   The name of the annotation to look for

@return Returns a handle to an annotation if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PASS_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid pass.</TD>
</TR>
</TABLE>

@sa cgGetFirstPassAnnotation, cgCreatePassAnnotation
 */
CGFX_API CGannotation cgGetNamedPassAnnotation( CGpass handle, const char *name )
{
    if ( !CG_IS_PASS( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PASS_HANDLE_ERROR );
        return 0;
    }
    Annotation *annotation = CG_GET_PASS( handle )->getFirstAnnotation();
    return getNamedAnnotation( annotation, name );
}

/**
@short Searches for an annotation by name in a parameter

cgGetNamedParameterAnnotation() looks for an annotation by name, in a given parameter.

@param handle The parameter to look into
@param name   The name of the annotation to look for

@return Returns a handle to an annotation if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid parameter.</TD>
</TR>
</TABLE>

@sa cgGetFirstParameterAnnotation, cgCreateParameterAnnotation
 */
CGFX_API CGannotation cgGetNamedParameterAnnotation( CGparameter handle, const char *name )
{
    if ( !CG_IS_PARAMETER( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_HANDLE_ERROR );
        return 0;
    }

    CgRuntimeParameter *param = ( CgRuntimeParameter* )_jsCgGLTestParameter( handle );

    CgParameterExtension *extension = CgParameterExtension::fromParameter( param );
    if ( !extension ) return 0;

    Annotation *annotation = extension->getFirstAnnotation();
    return getNamedAnnotation( annotation, name );
}

/**
@short Searches for an annotation by name in a program

cgGetNamedProgramAnnotation() looks for an annotation by name, in a given program.

@param handle The program to look into
@param name   The name of the annotation to look for

@return Returns a handle to an annotation if it was found, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid program.</TD>
</TR>
</TABLE>

@sa cgGetFirstProgramAnnotation, cgCreateProgramAnnotation
 */
CGFX_API CGannotation cgGetNamedProgramAnnotation( CGprogram handle, const char *name )
{
    if ( !CG_IS_PROGRAM( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return 0;
    }

    _CGprogram *prog = _cgGetProgPtr( handle );

    CgProgramExtension *extension = CgProgramExtension::fromProgram( prog );
    if ( !extension ) return 0;

    Annotation *annotation = extension->getFirstAnnotation();
    return getNamedAnnotation( annotation, name );
}

/**
@short Tests if a handle represents a valid annotation

cgIsAnnotation() tests whether or not a <c>CGannotation</c> handle is a valid annotation

@param handle  The annotation handle to test

@return Returns <CODE>CG_TRUE</CODE> if <I><c>handle</c></I> is a valid annotation, and <CODE>CG_FALSE</CODE> otherwise.

@sa cgCreateEffectAnnotation, cgCreateTechniqueAnnotation,
cgCreatePassAnnotation, cgCreateProgramAnnotation,
cgCreateParameterAnnotation
 */
CGFX_API CGbool cgIsAnnotation( CGannotation handle )
{
    return CG_IS_ANNOTATION( handle );
}

/**
@short Retrieves the name of an annotation

cgGetAnnotationName() returns the name of the annotation, as specified when the annotation was created.

@param handle The annotation to retrieve the name from

@return Returns the name of the annotation if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
</TABLE>

@sa cgCreateEffectAnnotation, cgCreateTechniqueAnnotation,
cgCreatePassAnnotation, cgCreateProgramAnnotation,
cgCreateParameterAnnotation
 */
CGFX_API const char *cgGetAnnotationName( CGannotation handle )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return 0;
    }
    return CG_GET_ANNOTATION( handle )->getName();
}

/**
@short Retrieves the type of an annotation

cgGetAnnotationType() returns the type of the annotation, as specified when the annotation was created.

@param handle The annotation to retrieve the type from

@return Returns the type of the annotation if success, <CODE>CG_UNKNOWN_TYPE</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
</TABLE>

@sa cgCreateEffectAnnotation, cgCreateTechniqueAnnotation,
cgCreatePassAnnotation, cgCreateProgramAnnotation,
cgCreateParameterAnnotation
 */
CGFX_API CGtype cgGetAnnotationType( CGannotation handle )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return CG_UNKNOWN_TYPE;
    }
    return AnyValue::getCGtypeFromType( CG_GET_ANNOTATION( handle )->getValue().getType() );
}

/**
@short Sets a float value for an annotation

cgSetFloatAnnotation() sets the value of an annotation as a single float. The
annotation needs to be of type <c>CG_FLOAT</c>, and a scalar.

@param handle The annotation to be set
@param f      The float value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_FLOAT</c>, or is an array.</TD>
</TR>
</TABLE>

@sa  cgSetIntAnnotation, cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues
 */
CGFX_API CGbool cgSetFloatAnnotation( CGannotation handle, float f )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::FLOAT );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
        return CG_FALSE;
    }
    value->setFloat( f );
    return CG_TRUE;
}

/**
@short Sets an int value for an annotation.

cgSetIntAnnotation() sets the value of an annotation as a single int. The
annotation needs to be of type <c>CG_INT</c>, and a scalar.

@param handle The annotation to be set
@param i      The int value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_INT</c>, or is an array.</TD>
</TR>
</TABLE>

@sa  cgSetFloatAnnotation, cgSetBoolAnnotation, cgSetStringAnnotation, cgGetIntAnnotationValues
 */
CGFX_API CGbool cgSetIntAnnotation( CGannotation handle, int i )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::INT );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
        return CG_FALSE;
    }
    value->setInt( i );
    return CG_TRUE;
}

/**
@short Sets a boolean value for an annotation

cgSetBoolAnnotation() sets the value of an annotation as a single boolean. The
annotation needs to be of type <c>CG_BOOL</c>, and a scalar.

@param handle The annotation to be set
@param b      The boolean value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_BOOL</c>, or is an array.</TD>
</TR>
</TABLE>

@sa  cgSetFloatAnnotation, cgSetIntAnnotation, cgSetStringAnnotation, cgGetBoolAnnotationValues
 */
CGFX_API CGbool cgSetBoolAnnotation( CGannotation handle, CGbool b )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::BOOL );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
        return CG_FALSE;
    }
    value->setBool( b );
    return CG_TRUE;
}

/**
@short Sets a string value for an annotation

cgSetStringAnnotation() sets the value of an annotation as a C string. The
annotation needs to be of type <c>CG_STRING</c>, and a scalar.
The string passed in parameter is copied internally, so the user may free its
memory right after the call returns.

@param handle The annotation to be set
@param s      The string value to set

@return Returns <c>CG_TRUE</c> if success, and <c>CG_FALSE</c> otherwise

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_STRING</c>.</TD>
</TR>
</TABLE>

@sa  cgSetFloatAnnotation, cgSetIntAnnotation, cgSetBoolAnnotation, cgGetStringAnnotationValue
 */
CGFX_API CGbool cgSetStringAnnotation( CGannotation handle, const char *s )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::STRING );
    if ( !value ) return CG_FALSE;
    if ( value->isArray() )
    {
        _jsCgRaiseError( CG_INVALID_VALUE_TYPE_ERROR );
        return CG_FALSE;
    }
    JS_ASSERT( !value->isArray() );
    value->setString( s );
    return CG_TRUE;
}

/**
@short Retrieves the float values in an annotation

cgGetFloatAnnotationValues() retrieves the values in an annotation as an array of floats.
The annotation needs to be of type <code>CG_FLOAT</code>, but can be either a scalar or an array.
The pointer returned reference internal data, the user does not need to free it.

@param handle The annotation to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. 
Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the float values. 
If <I><c>nVals</c></I> isn&rsquo;t <CODE>NULL</CODE>, the variable referenced holds the number of elements. 
In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> isn&rsquo;t 
modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_FLOAT</c>.</TD>
</TR>
</TABLE>

@sa  cgSetFloatAnnotation
 */
CGFX_API const float *cgGetFloatAnnotationValues( CGannotation handle, int *nVals )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::FLOAT );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const float *>( value->getData() );
}

/**
@short Retrieves the int values in an annotation

cgGetIntAnnotationValues() retrieves the values in an annotation as an array of ints.
The annotation needs to be of type <c>CG_INT</c>, but can be either a scalar or an array.
The pointer returned reference internal data, the user does not need to free it.

@param handle The annotation to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. 
Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the int values. 
If <I><c>nVals</c></I> isn&rsquo;t <CODE>NULL</CODE>, the variable referenced holds the number of elements. 
In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> isn&rsquo;t 
modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_INT</c>.</TD>
</TR>
</TABLE>

@sa  cgSetIntAnnotation
 */
CGFX_API const int *cgGetIntAnnotationValues( CGannotation handle, int *nVals )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::INT );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const int *>( value->getData() );
}

/**
@short Retrieves the bool values in an annotation

cgGetBoolAnnotationValues() retrieves the values in an annotation as an array of bools.
The annotation needs to be of type <code>CG_BOOL</code>, but can be either a scalar or an array.
The pointer returned references internal data, so the user does not need to free it.

@param handle The annotation to retrieve values from
@param nVals  A reference to a variable that will store the number of elements in the returned array. 
Can be <CODE>NULL</CODE>.

@return In case of success, the function returns a pointer to an array holding the bool values. 
If <I><c>nVals</c></I> isn&rsquo;t <CODE>NULL</CODE>, the variable referenced holds the number of elements. 
In case of failure, <CODE>NULL</CODE> is returned, and the variable pointed by <I><c>nVals</c></I> 
is not modified.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_BOOL</c>.</TD>
</TR>
</TABLE>

@sa  cgSetBoolAnnotation
 */
CGFX_API const CGbool *cgGetBoolAnnotationValues( CGannotation handle, int *nVals )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::BOOL );
    if ( !value ) return 0;
    if ( nVals ) *nVals = value->getElemCount();
    return static_cast<const CGbool *>( value->getData() );
}


/**
@short Retrieves the bool values in an annotation (deprecated)

This function has been deprecated.  Use cgGetBoolAnnotationValues() instead.

@param a  Not implemented
@param nvalues Not implemented

@returns Not implemented

@sa cgGetBoolAnnotationValues
*/
CGFX_API const int * cgGetBooleanAnnotationValues( CGannotation a, int *nvalues )
{
    // NOTE: deprecated, do not use.
    return 0;
}


/**
@short Retrieves the string values in an annotation

cgGetStringAnnotationValue() retrieves the values in an annotation as a string.
The annotation needs to be of type <CODE>CG_STRING</CODE>.
The pointer returned reference internal data, the user does not need to free it.

@param handle The annotation to retrieve the value from

@return In case of success, the function returns a pointer to a C string. 
In case of failure, <CODE>NULL</CODE> is returned.
@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_VALUE_TYPE_ERROR</CODE></TD>
<TD>The annotation is not of type <c>CG_STRING</c>.</TD>
</TR>
</TABLE>

@sa  cgSetStringAnnotation
 */
CGFX_API const char *cgGetStringAnnotationValue( CGannotation handle )
{
    AnyValue *value = getValueFromAnnotationCheckType( handle, AnyValue::STRING );
    if ( !value ) return 0;
    return value->getString();
}

/**
@short Retrieves the number of parameters an annotation depends on

The values of an annotation can depend on parameter values, although this is not
available in current implementation.
cgGetNumDependentAnnotationParameters() retrieves the number of such parameters.
Together with cgGetDependentAnnotationParameter(), this is used to iterate
through all these parameters.

@param handle The annotation to retrieve dependent parameters from

@return Returns the number of dependent parameters if success, 0 otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
</TABLE>

@sa  cgGetDependentAnnotationParameter
 */
CGFX_API int cgGetNumDependentAnnotationParameters( CGannotation handle )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return 0;
    }
    return 0;
}

/**
@short Retrieves a parameter an annotation depends on, by index

An annotation values can depend on parameter values, although this is not
available in current implementation.
cgGetDependentAnnotationParameter() retrieves a handle to one such parameters.
Togeteher with cgGetNumDependentAnnotationParameters(), this is used to iterate
through all these parameters.

@param handle The annotation to retrieve dependent parameters from
@param index  The index of the parameter in the dependent parameter list

@return Returns a handle to the dependent parameter if success, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ANNOTATION_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid annotation.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not within the range [0..n-1]
where <I>n</I> is the return value of cgGetNumDependentAnnotationParameters().</TD>
</TR>
</TABLE>

@sa  cgGetNumDependentAnnotationParameters
 */
CGFX_API CGparameter cgGetDependentAnnotationParameter( CGannotation handle, int index )
{
    if ( !CG_IS_ANNOTATION( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_ANNOTATION_HANDLE_ERROR );
        return 0;
    }
    _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
    return 0;
}

/**
 * @}
 */
// ------------------------------------------------------
