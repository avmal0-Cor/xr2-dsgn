/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * PSGLFX - Program FX extensions
 */

#include <PSGL/private.h>
#include <PSGLFX/Context.h>
#include <PSGLFX/Program.h>
#include <PSGLFX/Annotation.h>
#include <Cg/CgCommon.h>
#include <string.h>


namespace PSGLFX
{
CgProgramExtension::CgProgramExtension():
        annotations( 0 )
{}

CgProgramExtension::CgProgramExtension( const CgProgramExtension &other ):
        annotations( 0 )
{
    for ( Annotation *annotation = other.getFirstAnnotation();annotation;annotation = annotation->getNext() )
    {
        appendAnnotation( new Annotation( *annotation ) );
    }
}

CgProgramExtension::~CgProgramExtension()
{
    if ( annotations ) delete annotations;
}

void CgProgramExtension::appendAnnotation( Annotation *annotation )
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
@short Programmatically generate a texture image from a cg program.

cgEvaluateProgram() evaluates a fragment program over a series of pixels to
programatically generate a texture image.
This function is currently not implemented.

@param program The program to evaluate
@param result  A float array containing the resulting texture
@param ncomps  The numbers of color components that need to be generated
@param nx      The width of the resulting image
@param ny      The height of the resulting image
@param nz      The depth of the resulting image

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid program.</TD>
</TR>
</TABLE>
 */
CGFX_API void cgEvaluateProgram( CGprogram program, float *result, int ncomps, int nx, int ny, int nz )
{
    // XXX NYI ....
}

/**
@short Adds a scalar annotation to a program.

cgCreateProgramAnnotation() creates a new annotation and adds it to a program.
The newly created annotation is of a scalar type, and its value can be specified
using one of the cgSet*Annotation functions.

@param handle The program to add an annotation to
@param name   The name of the annotation to add
@param type   The type of the annotation. Must be <CODE>CG_FLOAT</CODE>, <CODE>CG_INT</CODE>, <CODE>CG_BOOL</CODE> or <CODE>CG_STRING</CODE>

@return Returns a handle to the created annotation if it was successfully created, <CODE>NULL</CODE> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>handle</c></I> is not a valid program.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_TYPE_ERROR</CODE></TD>
<TD><I><c>type</c></I> is not one of the valid types specified above.</TD>
</TR>
</TABLE>

@sa cgCreateProgram, cgSetFloatAnnotation, cgSetIntAnnotation,
    cgSetBoolAnnotation, cgSetStringAnnotation, cgGetFloatAnnotationValues, cgGetIntAnnotationValues,
    cgGetBoolAnnotationValues, cgGetStringAnnotationValue
 */
CGFX_API CGannotation cgCreateProgramAnnotation( CGprogram handle, const char *name, CGtype type )
{
    if ( !CG_IS_PROGRAM( handle ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return 0;
    }
    _CGprogram *program = _cgGetProgPtr( handle );
    CgProgramExtension *extension = CgProgramExtension::fromProgramSafe( program );

    AnyValue::Type internalType = AnyValue::getAnnotationTypeFromCGtype( type );
    if ( internalType == AnyValue::NONE )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_TYPE_ERROR );
        return 0;
    }

    Annotation *annotation = new Annotation( name, internalType );
    extension->appendAnnotation( annotation );
    return annotation->getId();
}

/**
 * @}
 */
// ------------------------------------------------------
