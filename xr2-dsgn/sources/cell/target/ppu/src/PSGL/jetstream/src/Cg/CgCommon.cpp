/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: Common shared functions for implementing Jetstream Cg Runtime

#include <stdio.h>
#include <Cg/cg.h>
#include <Cg/CgCommon.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>
// externs ----------------------------------

#ifdef JS_DEBUG
extern void _jsCgContextDumpList( CGcontext ptr, unsigned int indent );
#endif

// functions --------------------------------

void _jsCgRaiseError( CGerror error )
{
    // Not in OpenGL style, reports on last error that occurred.
    _CurrentContext->JScgLastError = error;


    _JS_REPORT_EXTRA( PSGL_REPORT_CG_ERROR, "Cg error:%s", cgGetErrorString( error ) );

    if ( _CurrentContext->JScgErrorCallbackFunction )
    {
        _CurrentContext->JScgErrorCallbackFunction();
    }
}

unsigned int _jsCountFloatsInCgType( CGtype type )
{
    int size = 0;
    switch ( type )
    {
        case CG_FLOAT:
        case CG_FLOAT1:
        case CG_FLOAT1x1:
            size = 1;
            break;
        case CG_FLOAT2:
        case CG_FLOAT2x1:
        case CG_FLOAT1x2:
            size = 2;
            break;
        case CG_FLOAT3:
        case CG_FLOAT3x1:
        case CG_FLOAT1x3:
            size = 3;
            break;
        case CG_FLOAT4:
        case CG_FLOAT4x1:
        case CG_FLOAT1x4:
        case CG_FLOAT2x2:
            size = 4;
            break;
        case CG_FLOAT2x3:
        case CG_FLOAT3x2:
            size = 6;
            break;
        case CG_FLOAT2x4:
        case CG_FLOAT4x2:
            size = 8;
            break;
        case CG_FLOAT3x3:
            size = 9;
            break;
        case CG_FLOAT3x4:
        case CG_FLOAT4x3:
            size = 12;
            break;
        case CG_FLOAT4x4:
            size = 16;
            break;
        case CG_SAMPLER1D:
        case CG_SAMPLER2D:
        case CG_SAMPLER3D:
        case CG_SAMPLERRECT:
        case CG_SAMPLERCUBE:
            size = 1;
            break;
        case CG_BOOL:
            size = 1;
            break;
        case CG_HALF:
        case CG_HALF1:
        case CG_HALF1x1:
            size = 1;
            break;
        case CG_HALF2:
        case CG_HALF2x1:
        case CG_HALF1x2:
            size = 2;
            break;
        case CG_HALF3:
        case CG_HALF3x1:
        case CG_HALF1x3:
            size = 3;
            break;
        case CG_HALF4:
        case CG_HALF4x1:
        case CG_HALF1x4:
        case CG_HALF2x2:
            size = 4;
            break;
        case CG_HALF2x3:
        case CG_HALF3x2:
            size = 6;
            break;
        case CG_HALF2x4:
        case CG_HALF4x2:
            size = 8;
            break;
        case CG_HALF3x3:
            size = 9;
            break;
        case CG_HALF3x4:
        case CG_HALF4x3:
            size = 12;
            break;
        case CG_HALF4x4:
            size = 16;
            break;
        case CG_INT:
        case CG_INT1:
        case CG_INT1x1:
            size = 1;
            break;
        case CG_INT2:
        case CG_INT2x1:
        case CG_INT1x2:
            size = 2;
            break;
        case CG_INT3:
        case CG_INT3x1:
        case CG_INT1x3:
            size = 3;
            break;
        case CG_INT4:
        case CG_INT4x1:
        case CG_INT1x4:
        case CG_INT2x2:
            size = 4;
            break;
        case CG_INT2x3:
        case CG_INT3x2:
            size = 6;
            break;
        case CG_INT2x4:
        case CG_INT4x2:
            size = 8;
            break;
        case CG_INT3x3:
            size = 9;
            break;
        case CG_INT3x4:
        case CG_INT4x3:
            size = 12;
            break;
        case CG_INT4x4:
            size = 16;
            break;
	//case CG_BOOL:  bool on it's own is more useful so comes earlier
        case CG_BOOL1:
        case CG_BOOL1x1:
            size = 1;
            break;
        case CG_BOOL2:
        case CG_BOOL2x1:
        case CG_BOOL1x2:
            size = 2;
            break;
        case CG_BOOL3:
        case CG_BOOL3x1:
        case CG_BOOL1x3:
            size = 3;
            break;
        case CG_BOOL4:
        case CG_BOOL4x1:
        case CG_BOOL1x4:
        case CG_BOOL2x2:
            size = 4;
            break;
        case CG_BOOL2x3:
        case CG_BOOL3x2:
            size = 6;
            break;
        case CG_BOOL2x4:
        case CG_BOOL4x2:
            size = 8;
            break;
        case CG_BOOL3x3:
            size = 9;
            break;
        case CG_BOOL3x4:
        case CG_BOOL4x3:
            size = 12;
            break;
        case CG_BOOL4x4:
            size = 16;
            break;
        case CG_FIXED:
        case CG_FIXED1:
        case CG_FIXED1x1:
            size = 1;
            break;
        case CG_FIXED2:
        case CG_FIXED2x1:
        case CG_FIXED1x2:
            size = 2;
            break;
        case CG_FIXED3:
        case CG_FIXED3x1:
        case CG_FIXED1x3:
            size = 3;
            break;
        case CG_FIXED4:
        case CG_FIXED4x1:
        case CG_FIXED1x4:
        case CG_FIXED2x2:
            size = 4;
            break;
        case CG_FIXED2x3:
        case CG_FIXED3x2:
            size = 6;
            break;
        case CG_FIXED2x4:
        case CG_FIXED4x2:
            size = 8;
            break;
        case CG_FIXED3x3:
            size = 9;
            break;
        case CG_FIXED3x4:
        case CG_FIXED4x3:
            size = 12;
            break;
        case CG_FIXED4x4:
            size = 16;
            break;
        default:
            JS_ASSERT( 0 );
            size = 0;
            break;
    }
    return size;
}

#ifdef JS_DEBUG

void _jsCgPrintSpaces( unsigned int n )
{
    for ( unsigned int i = 0; i < n; ++i )
        putchar( ' ' );
}

void _jsCgDumpState()
{
    // write out all Cg data structures to stdout
    printf( "Cg State ------\n" );
    _jsCgContextDumpList( _CurrentContext->JScgContextHead, 3 );
    printf( "End -----------\n\n" );
}

#endif

// here are default setters
void _cgRaiseInvalidParam( CgRuntimeParameter*p, const void*v )
{
    _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
}
void _cgRaiseInvalidParamIndex( CgRuntimeParameter*p, const void*v, const int index )
{
    _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
}

void _cgRaiseNotMatrixParam( CgRuntimeParameter*p, const void*v )
{
    _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
}
void _cgRaiseNotMatrixParamIndex( CgRuntimeParameter*p, const void*v, const int index )
{
    _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
}

void _cgIgnoreSetParam( CgRuntimeParameter*p, const void*v )
{
    // nothing
}
void _cgIgnoreSetParamIndex( CgRuntimeParameter*p, const void*v, const int index )
{
    // nothing
}

CGbool _cgMatrixDimensions( CGtype type, unsigned int* nrows, unsigned int* ncols )
{
    JS_ASSERT( nrows != NULL );
    JS_ASSERT( ncols != NULL );

	CGbool isMatrixType = CG_TRUE;
    switch ( type )
    {
		// Scalar type support 
		case CG_FLOAT:   *nrows = 1; *ncols = 1;	isMatrixType = CG_FALSE; break;
		case CG_FLOAT1:   *nrows = 1; *ncols = 1;	isMatrixType = CG_FALSE; break;		
		case CG_HALF:   *nrows = 1; *ncols = 1;		isMatrixType = CG_FALSE; break;
		case CG_HALF1:   *nrows = 1; *ncols = 1;	isMatrixType = CG_FALSE; break;
		case CG_INT:   *nrows = 1; *ncols = 1;		isMatrixType = CG_FALSE; break;
		case CG_INT1:   *nrows = 1; *ncols = 1;		isMatrixType = CG_FALSE; break;
		case CG_BOOL:   *nrows = 1; *ncols = 1;		isMatrixType = CG_FALSE; break;
		case CG_BOOL1:   *nrows = 1; *ncols = 1;	isMatrixType = CG_FALSE; break;
		case CG_FIXED:   *nrows = 1; *ncols = 1;	isMatrixType = CG_FALSE; break;
        case CG_FIXED1:   *nrows = 1; *ncols = 2;	isMatrixType = CG_FALSE; break;   


		// Vector type support 
        case CG_FLOAT1x1:   *nrows = 1; *ncols = 1; break;
        case CG_FLOAT1x2:   *nrows = 1; *ncols = 2; break;
        case CG_FLOAT1x3:   *nrows = 1; *ncols = 3; break;
        case CG_FLOAT1x4:   *nrows = 1; *ncols = 4; break;
        case CG_FLOAT2x1:   *nrows = 2; *ncols = 1; break;
        case CG_FLOAT2x2:   *nrows = 2; *ncols = 2; break;
        case CG_FLOAT2x3:   *nrows = 2; *ncols = 3; break;
        case CG_FLOAT2x4:   *nrows = 2; *ncols = 4; break;
        case CG_FLOAT3x1:   *nrows = 3; *ncols = 1; break;
        case CG_FLOAT3x2:   *nrows = 3; *ncols = 2; break;
        case CG_FLOAT3x3:   *nrows = 3; *ncols = 3; break;
        case CG_FLOAT3x4:   *nrows = 3; *ncols = 4; break;
        case CG_FLOAT4x1:   *nrows = 4; *ncols = 1; break;
        case CG_FLOAT4x2:   *nrows = 4; *ncols = 2; break;
        case CG_FLOAT4x3:   *nrows = 4; *ncols = 3; break;
        case CG_FLOAT4x4:   *nrows = 4; *ncols = 4; break;
        case CG_HALF1x1:   *nrows = 1; *ncols = 1; break;
        case CG_HALF1x2:   *nrows = 1; *ncols = 2; break;
        case CG_HALF1x3:   *nrows = 1; *ncols = 3; break;
        case CG_HALF1x4:   *nrows = 1; *ncols = 4; break;
        case CG_HALF2x1:   *nrows = 2; *ncols = 1; break;
        case CG_HALF2x2:   *nrows = 2; *ncols = 2; break;
        case CG_HALF2x3:   *nrows = 2; *ncols = 3; break;
        case CG_HALF2x4:   *nrows = 2; *ncols = 4; break;
        case CG_HALF3x1:   *nrows = 3; *ncols = 1; break;
        case CG_HALF3x2:   *nrows = 3; *ncols = 2; break;
        case CG_HALF3x3:   *nrows = 3; *ncols = 3; break;
        case CG_HALF3x4:   *nrows = 3; *ncols = 4; break;
        case CG_HALF4x1:   *nrows = 4; *ncols = 1; break;
        case CG_HALF4x2:   *nrows = 4; *ncols = 2; break;
        case CG_HALF4x3:   *nrows = 4; *ncols = 3; break;
        case CG_HALF4x4:   *nrows = 4; *ncols = 4; break;
        case CG_INT1x1:   *nrows = 1; *ncols = 1; break;
        case CG_INT1x2:   *nrows = 1; *ncols = 2; break;
        case CG_INT1x3:   *nrows = 1; *ncols = 3; break;
        case CG_INT1x4:   *nrows = 1; *ncols = 4; break;
        case CG_INT2x1:   *nrows = 2; *ncols = 1; break;
        case CG_INT2x2:   *nrows = 2; *ncols = 2; break;
        case CG_INT2x3:   *nrows = 2; *ncols = 3; break;
        case CG_INT2x4:   *nrows = 2; *ncols = 4; break;
        case CG_INT3x1:   *nrows = 3; *ncols = 1; break;
        case CG_INT3x2:   *nrows = 3; *ncols = 2; break;
        case CG_INT3x3:   *nrows = 3; *ncols = 3; break;
        case CG_INT3x4:   *nrows = 3; *ncols = 4; break;
        case CG_INT4x1:   *nrows = 4; *ncols = 1; break;
        case CG_INT4x2:   *nrows = 4; *ncols = 2; break;
        case CG_INT4x3:   *nrows = 4; *ncols = 3; break;
        case CG_INT4x4:   *nrows = 4; *ncols = 4; break;
        case CG_BOOL1x1:   *nrows = 1; *ncols = 1; break;
        case CG_BOOL1x2:   *nrows = 1; *ncols = 2; break;
        case CG_BOOL1x3:   *nrows = 1; *ncols = 3; break;
        case CG_BOOL1x4:   *nrows = 1; *ncols = 4; break;
        case CG_BOOL2x1:   *nrows = 2; *ncols = 1; break;
        case CG_BOOL2x2:   *nrows = 2; *ncols = 2; break;
        case CG_BOOL2x3:   *nrows = 2; *ncols = 3; break;
        case CG_BOOL2x4:   *nrows = 2; *ncols = 4; break;
        case CG_BOOL3x1:   *nrows = 3; *ncols = 1; break;
        case CG_BOOL3x2:   *nrows = 3; *ncols = 2; break;
        case CG_BOOL3x3:   *nrows = 3; *ncols = 3; break;
        case CG_BOOL3x4:   *nrows = 3; *ncols = 4; break;
        case CG_BOOL4x1:   *nrows = 4; *ncols = 1; break;
        case CG_BOOL4x2:   *nrows = 4; *ncols = 2; break;
        case CG_BOOL4x3:   *nrows = 4; *ncols = 3; break;
        case CG_BOOL4x4:   *nrows = 4; *ncols = 4; break;
        case CG_FIXED1x1:   *nrows = 1; *ncols = 1; break;
        case CG_FIXED1x2:   *nrows = 1; *ncols = 2; break;
        case CG_FIXED1x3:   *nrows = 1; *ncols = 3; break;
        case CG_FIXED1x4:   *nrows = 1; *ncols = 4; break;
        case CG_FIXED2x1:   *nrows = 2; *ncols = 1; break;
        case CG_FIXED2x2:   *nrows = 2; *ncols = 2; break;
        case CG_FIXED2x3:   *nrows = 2; *ncols = 3; break;
        case CG_FIXED2x4:   *nrows = 2; *ncols = 4; break;
        case CG_FIXED3x1:   *nrows = 3; *ncols = 1; break;
        case CG_FIXED3x2:   *nrows = 3; *ncols = 2; break;
        case CG_FIXED3x3:   *nrows = 3; *ncols = 3; break;
        case CG_FIXED3x4:   *nrows = 3; *ncols = 4; break;
        case CG_FIXED4x1:   *nrows = 4; *ncols = 1; break;
        case CG_FIXED4x2:   *nrows = 4; *ncols = 2; break;
        case CG_FIXED4x3:   *nrows = 4; *ncols = 3; break;
        case CG_FIXED4x4:   *nrows = 4; *ncols = 4; break;
        default:            *nrows = 0; *ncols = 0; break;
    }

	return isMatrixType; 
}


// This helper function validates array access.
CgRuntimeParameter* _cgGLTestArrayParameter( CGparameter paramIn, long offset, long nelements )
{
    // check that it is a param at all.
    CgRuntimeParameter* param = _jsCgGLTestParameter( paramIn );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !param ) ) return NULL;

    // validate whether an array parameter
    const CgParameterEntry *parameterEntry = param->parameterEntry;
    if ( JS_UNLIKELY( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) ) )
    {
        // SPECIAL CASE FOR ARRAY_OF_SAMPLERS
        if ( !( parameterEntry->flags & CGP_STRUCTURE ) &&
                isSampler( _jsGetParameterCGtype( param->program, param->parameterEntry ) ) )
        {
            int length = strlen( param->program->stringTable + parameterEntry->nameOffset ) - 3;
            if ( length > 0 &&
                    !strcmp( "[0]", param->program->stringTable + parameterEntry->nameOffset + length ) )
                return param;
        }
        // END SPECIAL CASE FOR ARRAY_OF_SAMPLERS

        _jsCgRaiseError( CG_ARRAY_PARAM_ERROR );
        return NULL;
    }

    if ( JS_UNLIKELY( parameterEntry->flags & CGP_RTCREATED ) )
    {
        JS_ASSERT( 0 && "Runtime array parameters can not be created.  This should not happen." );
    }

    // validate bounds. Last case is for corner case offset=size, nelements = 0
    const CgParameterArray *parameterArray = _jsGetParameterArray( param->program, parameterEntry );
    int arraySize = ( int )_jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
    if ( JS_UNLIKELY( offset < 0 || nelements < 0 || offset + nelements >  arraySize || offset >= arraySize ) )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return NULL;
    }
#endif

    return param;
}

// This helper function validates a texture parameter.
CgRuntimeParameter* _cgGLTestTextureParameter( CGparameter param )
{
    CgRuntimeParameter* ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return NULL;

    // validate texture type
    CGtype parameterType = _jsGetParameterCGtype( ptr->program, ptr->parameterEntry );
    switch ( parameterType )
    {
            // 1D texture not supported by OpenGL/ES
            /*    case CG_SAMPLER1D:*/
        case CG_SAMPLER2D:
        case CG_SAMPLER3D:
            /*    case CG_SAMPLERRECT:*/
        case CG_SAMPLERCUBE:
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            return NULL;
    }
#endif
    return ptr;
}


#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
	nrows ,
static int _typesRowCount[] =
    {
#include "Cg/cg_datatypes.h"
    };

#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
	ncols ,
static int _typesColCount[] =
    {
#include "Cg/cg_datatypes.h"
    };

unsigned int _jsGetTypeRowCount( CGtype parameterType )
{
    int typeIndex = parameterType - 1 - CG_TYPE_START_ENUM;
    return _typesRowCount[typeIndex];
}

unsigned int _jsGetTypeColCount( CGtype parameterType )
{
    int typeIndex = parameterType - 1 - CG_TYPE_START_ENUM;
    return _typesColCount[typeIndex];
}



// ------------------------------------------
