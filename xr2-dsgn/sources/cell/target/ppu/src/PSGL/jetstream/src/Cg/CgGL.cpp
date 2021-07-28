/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <string.h>		// for memcpy
#include <PSGL/psgl.h>
#include <Cg/cgGL.h>
#include <Cg/CgCommon.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>	// many routines in this file (CgGL.c) are not annotated because they only call into other cg or gl routines

#ifndef JS_NO_ERROR_CHECK
#define JS_CHECK_PROGRAM { \
	if (JS_UNLIKELY(!CG_IS_PROGRAM(program))) \
	{ \
	_jsCgRaiseError(CG_INVALID_PROGRAM_HANDLE_ERROR); \
	return; \
	} \
}
#define JS_CHECK_PROGRAM_RET(ret) { \
	if (JS_UNLIKELY(!CG_IS_PROGRAM(program))) \
	{ \
		_jsCgRaiseError(CG_INVALID_PROGRAM_HANDLE_ERROR); \
		return ret; \
	} \
}
#else
#define JS_CHECK_PROGRAM
#define JS_CHECK_PROGRAM_RET(ret)
#endif


// TODO: stubs are present but no valid code
// cgGLSetTextureParameter
// cgGLGetTextureParameter

/******************************************************************************
 *** Internal API functions
 *****************************************************************************/

inline static float *_jsGetUniformValuePtr( CGparameter param, CgRuntimeParameter *rtParameter )
{
    float* value = ( float* )( rtParameter->pushBufferPointer );

    // check in bounds to know if you should even bother checking that it is in an array
    if ( rtParameter > rtParameter->program->runtimeParameters )
    {
        CgRuntimeParameter *rtInArrayCheckParameter = rtParameter - 1;
        // check is array
        if ( rtInArrayCheckParameter->parameterEntry->flags & CGP_ARRAY )
        {
            value = *(( float** )( rtParameter->pushBufferPointer ) + CG_GETINDEX( param ) );
        }
    }
    return value;
}


// endian swapping of the fragment uniforms, if necessary
#if JS_ENDIAN == JS_BIG_ENDIAN
#define SWAP_IF_BE(arg) endianSwapWordByHalf(arg)
#else
#define SWAP_IF_BE(arg) arg
#endif


/******************************************************************************
 *** Profile Functions
 *****************************************************************************/

/**
 * @addtogroup CgGL
 * @{
 */
/**
@short Determines if a given profile is supported by the runtime

cgGLIsProfileSupported() returns <CODE>CG_TRUE</CODE> if the profile indicated
by the <I><c>profile</c></I> parameter is supported by the runtime library.
Otherwise it returns <CODE>CG_FALSE</CODE>.

@param       profile The profile enumerant.

@return      Boolean indicating support or lack of support for a profile.

@sa cgGLEnableProfile, cgGLDisableProfile
 */
CGGL_API CGbool cgGLIsProfileSupported( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGLIsProfileSupported );

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

#ifdef JS_DPM
if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLIsProfileSupported, JSDPM_CALL_TYPE_STATE_CG, 0, 1, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    switch ( profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            return ( CGbool ) _jsPlatformSupportsVertexProgram( profile );
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            return ( CGbool ) _jsPlatformSupportsFragmentProgram( profile );
        default:
            return CG_FALSE;
    }
}

/**
@short Enable a profile within OpenGL

cgGLEnableProfile() enables a given profile.

@param       profile The profile enumerant.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa cgGLDisableProfile
 */
CGGL_API void cgGLEnableProfile( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGLEnableProfile );

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

#ifdef JS_DPM
if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLEnableProfile, JSDPM_CALL_TYPE_STATE_CG, , 1, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // this is a logical extension to glEnable
    PSGLcontext* LContext = _CurrentContext;
    switch ( profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            LContext->VertexProgram = GL_TRUE;
            LContext->needValidate |= PSGL_VALIDATE_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
            break;

        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            _jsLeaveFFXFP( LContext );
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PROFILE_ERROR );
            break;
    }
}

/**
@short Disable a profile within OpenGL

cgGLDisableProfile() disables a given profile.

@param       profile The profile enumerant.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGLEnableProfile
 */
CGGL_API void cgGLDisableProfile( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGLDisableProfile );

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

#ifdef JS_DPM
if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLDisableProfile, JSDPM_CALL_TYPE_STATE_CG, , 1, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // this is a logical extension to glDisable
    PSGLcontext* LContext = _CurrentContext;
    switch ( profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            LContext->VertexProgram = GL_FALSE;
            // when disabling a vertex program, we need to revalidate all ffx state.
            // no need to invalidate textures because they are only available on programmable pipe.
            LContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_PROGRAM ;
            // revalidate texgen on all units
            int unit;
            for ( unit = 0;unit < _JS_MAX_TEXTURE_UNITS;unit++ )
            {
                LContext->TextureCoordsUnits[unit].revalidate = _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN;
            }
            break;
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            LContext->FragmentProgram = GL_FALSE;
            _jsEnterFFXFP( LContext );
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PROFILE_ERROR );
            break;
    }
}


/**
@short Returns the best available profile for a particular processing unit

cgGLGetLatestProfile() returns the best available profile of a given class.

cgGLGetLatestProfile() may be used in conjunction with cgCreateProgram() to
ensure that more optimal profiles are used as they are made available
even though they might not be available at compile time or with a given
version of the runtime.

@param       profile_type
The class of profile that will be returned.  <I><c>profile_type</c></I> may be one of:
\arg <CODE>CG_GL_VERTEX: </CODE> For the latest vertex profile.
\arg <CODE>CG_GL_FRAGMENT: </CODE> For the latest fragment profile.

@return Returns a profile enumerant for the latest profile of the given class.
If no appropriate profile is available or an error occurs <CODE>CG_PROFILE_UNKNOWN</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>profile_type</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa cgCreateProgram
 */
CGGL_API CGprofile cgGLGetLatestProfile( CGGLenum profile_type )
{
    JS_APIENTRY_TIMER( cgGLGetLatestProfile );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLGetLatestProfile, JSDPM_CALL_TYPE_STATE_CG, CG_PROFILE_UNKNOWN, 1, profile_type, sizeof( CGGLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif
    switch ( profile_type )
    {
        case CG_GL_VERTEX:
        case CG_GL_FRAGMENT:
            return _jsPlatformGetLatestProfile( profile_type );
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return CG_PROFILE_UNKNOWN;
    }
}

/**
 * @short Sets implicit optimization runtime compiler options
for a profile (not implemented)
 *
 * cgGLSetOptimalOptions() is not implemented.
 *
 * cgGLSetOptimalOptions() sets implicit compiler arguments
that are appended to the argument list passed to cgCreateProgram(). The arguments are
chosen based on the available compiler arguments, RSX&trade;, and driver.

@param profile the profile enumerant

@sa cgCreateProgram

 */
CGGL_API void cgGLSetOptimalOptions( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGLSetOptimalOptions );
    // This does nothing because we don't compile at run-time.
}

/******************************************************************************
 *** Program Managment Functions
 *****************************************************************************/

/**
 * @short Prepares a program for binding (not implemented)
 *
cgGLLoadProgram() is not implemented.
 *
cgGLLoadProgram() prepares a program for binding.
All programs must be loaded before they can be bound to the current state.
See cgGLBindProgram() for more information about binding programs.

In this implementation, a CGprogram is automatically loaded upon creation, so this
function does nothing.

@param program specifies the program

@sa cgGLBindProgram
 */
CGGL_API void cgGLLoadProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGLLoadProgram );
    // noop since there is integration of cg in js.
}

/**
 * @short Determines if a program is loaded (not implemented)
 *
 * cgGLIsProgramLoaded() returns <CODE>CG_TRUE</CODE>
if program is loaded with cgGLLoadProgram() and <CODE>CG_FALSE</CODE> otherwise.
In this implementation, a <CODE>CGProgram</CODE> is always loaded, so this function always
returns <CODE>CG_TRUE</CODE>.

@param program the program handle

@return Boolean value to indicate if program is loaded.  On this implementation,
<CODE>CG_TRUE</CODE> is always returned.

@sa cgGLLoadProgram

 */
CGGL_API CGbool cgGLIsProgramLoaded( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGLIsProgramLoaded );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLIsProgramLoaded, JSDPM_CALL_TYPE_STATE_CG, 0, 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !CG_IS_PROGRAM( program ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return CG_FALSE;
    }
#endif

    // TODO: for now a valid program is always loaded.
    return CG_TRUE;
}


/**
@short Binds a program to the current state.

cgGLBindProgram() binds a program to the current state.
The profile of the program must be enabled for the binding to affect rendering.
This may be done with the cgGLEnableProfile() function.

@param program    Specifies the program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>program</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is not a valid program.</TD>
</TR>
<TR>
<TD><CODE>CG_PROGRAM_BIND_ERROR</CODE></TD>
<TD>The program failed to bind for some reason.</TD>
</TR>
</TABLE>

@sa cgCreateProgram
 */
#ifdef JS_DPM
int __jsNumVPBinds;
int __jsNumFPBinds;
#endif
CGGL_API void cgGLBindProgram( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGLBindProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLBindProgram, JSDPM_CALL_TYPE_STATE_CG, , 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
#ifndef JS_NO_ERROR_CHECK
    // check the program id
    if ( JS_UNLIKELY( !CG_IS_PROGRAM( program ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
#endif
    _CGprogram* ptr = _cgGetProgPtr( program );
    JS_ASSERT( ptr );

    // now do the binding.
    switch ( ptr->header.profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //hack to counter removal of TypeC during beta
        case 7005:
        case CG_PROFILE_SCE_VP_RSX:
#ifdef JS_DPM
            __jsNumVPBinds++;
#endif
            _cgGLBindVertexProgram( ptr );
            break;

        case CG_PROFILE_SCE_FP_TYPEB:
            //hack to counter removal of TypeC during beta
        case 7006:
        case CG_PROFILE_SCE_FP_RSX:
#ifdef JS_DPM
            __jsNumFPBinds++;
#endif
            _cgGLBindFragmentProgram( ptr );
            break;

        default:
            _jsCgRaiseError( CG_INVALID_PROFILE_ERROR );
            return;
    }

}

/**
@short Unbinds the program bound in a profile

cgGLUnbindProgram() unbinds the program bound in the profile specified by <I><c>profile</c></I>.

@param profile Specifies the profile.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD>The program's profile is not a supported OpenGL profile.</TD>
</TR>
</TABLE>

@sa       cgGLSetManageTextureParameters, cgGLBindProgram
 */
CGGL_API void cgGLUnbindProgram( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGLUnbindProgram );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLUnbindProgram, JSDPM_CALL_TYPE_STATE_CG, , 1, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // TODO: pop texture state
    //  Needs to be done per profile. Can't use glPopAttrib.

    switch ( profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            //hack to counter removal of TypeC during beta
        case 7005:
            _cgGLUnbindVertexProgram();
            // no need to invalidate textures because they are only available on programmable pipe.
            break;
        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            //hack to counter removal of TypeC during beta
        case 7006:
            _cgGLUnbindFragmentProgram();

            break;
        default:
            _jsCgRaiseError( CG_INVALID_PROFILE_ERROR );
            return;
    }

}


// this API exposes internal implementation of Cg.
// Since we do not rely on program objects, always return 0.
/**
 * @short On other Cg implementations, returns the OpenGL program ID
associated with the CGprogram object.

 * On other Cg implementations, cgGLGetProgramID() returns the identifier to
the program GL object associated with the given Cg program.
cgGLGetProgramID() should not be called before cgGLLoadProgram() is called.

In our implementation, cgGLGetProgramID() always does nothing and returns 0.

@note
Since this implementation of the Cg runtime does not rely on
a lower level interface, a Cg program does not have a specific ID.
In this implementation, cgGLGetProgramID() does not return a useful value.

@return Returns a profile enumerant for the latest profile of the given class.
If no appropriate profile is available or an error occurs <CODE>CG_PROFILE_UNKNOWN</CODE>
is returned.

@param   program   Specifies the program.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is not a valid program.</TD>
</TR>
</TABLE>

@sa cgGLLoadProgram, cgGLBindProgram
 */
CGGL_API GLuint cgGLGetProgramID( CGprogram program )
{
    JS_APIENTRY_TIMER( cgGLGetProgramID );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLGetProgramID, JSDPM_CALL_TYPE_STATE_CG, 0, 1, program, sizeof( CGprogram ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return 0;
    }
#endif
    return 0;
}

/**
@short Enable all profiles associated with a GLSL combined program (Not implemented)

cgGLEnableProgramProfiles() is not supported under PSGL.

cgGLEnableProgramProfiles() enables all profiles associated with a GLSL combined program.

@param program The program containing profiles that need to be enabled.
*/
CGGL_API void cgGLEnableProgramProfiles( CGprogram program )
{
#ifndef JS_NO_ERROR_CHECK
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
#endif

    // TODO: unsupported in Jetstream
    return;
}

/**
@short Disables all profiles associated with a GLSL-combined program. (Not implemented)

cgGLDisableProgramProfiles is not supported under PSGL.

Disables all profiles associated with a GLSL-combined program.

@param program The program containing profiles that need to be disabled.
*/
CGGL_API void cgGLDisableProgramProfiles( CGprogram program )
{
#ifndef JS_NO_ERROR_CHECK
    if ( !CG_IS_PROGRAM( program ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
#endif

    // TODO: unsupported in Jetstream
    return;
}


/******************************************************************************
 *** Parameter Managment Functions
 *****************************************************************************/

/**
@short Sets the value of scalar parameters

cgGLSetParameter1f() sets the value of a given scalar or vector parameter.
It differs from cgGLSetParameter4f() only in the number of values that it takes.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   x       The values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter1f( CGparameter param, float x )
{
    JS_APIENTRY_TIMER( cgGLSetParameter1f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter1f, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif

    // check to see if the parameter is a good one
    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    // otherwise apply the values to the parameter
    float v[4] = {x, x, x, x};
    ptr->setterIndex( ptr, v, CG_GETINDEX( param ) );
}

/**
@short Sets the values of scalar parameters

cgGLSetParameter2f() sets the value of a given scalar or vector parameter.
It differs from cgGLSetParameter4f() only in the number of values that it takes.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   x,y       The values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter2f( CGparameter param, float x, float y )
{
    JS_APIENTRY_TIMER( cgGLSetParameter2f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetParameter2f, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             y, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    // otherwise apply the values to the parameter
    float v[4] = {x, y, y, y};
    ptr->setterIndex( ptr, v, CG_GETINDEX( param ) );
}

/**
@short Sets the values of scalar parameters

cgGLSetParameter3f() sets the value of a given scalar or vector parameter.
It differs from cgGLSetParameter4f() only in the number of values that it takes.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   x,y,z       The values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter3f( CGparameter param, float x, float y, float z )
{
    JS_APIENTRY_TIMER( cgGLSetParameter3f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameter3f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             y, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             z, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    // otherwise apply the values to the parameter
    float v[4] = {x, y, z, z};
    ptr->setterIndex( ptr, v, CG_GETINDEX( param ) );
}

/**

@short Sets the value of scalar and vector parameters

The <c>cgGLSetParameter*</c> functions set the value of a given scalar or vector parameter.
The functions are available in various combinations.

Each function takes either 1, 2, 3, or 4 values depending on the function
that is used.  If more values are passed in than the parameter
requires, the extra values will be ignored.  If less values are passed
in than the parameter requires, the last value will be smeared.

There are versions of each function that take either float or double
values signified by the <I>f</I> or <I>d</I> in the function name.

The functions with the <I>v</I> at the end of their names take an array of
values instead of explicit parameters.

The <c>cgGLSetParameter</c> functions may be called with either uniform or
varying parameters.  When called with a varying parameter, the appropriate immediate mode OpenGL 
entry point will be called.

@par Function Prototype Variants
@code
void cgGLSetParameter1{fd}(CGparameter param, type x);
void cgGLSetParameter2{fd}(CGparameter param, type x, type y);
void cgGLSetParameter3{fd}(CGparameter param, type x, type y, type z);
void cgGLSetParameter4{fd}(CGparameter param, type x, type y, type z, type w);
void cgGLSetParameter{1234}{fd}v(CGparameter param, const type *v);
@endcode

@param       param   Specifies the parameter that will be set.

@param x,y,z,w       The values to set the parameter to.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter failed to set for any other reason.</TD>
</TR>
</TABLE>

@note
Some <c>cgSetParameter</c> functions can still be used in PSGL.  These functions are macro-replaced with their
<c>cgGLSetParameter</c> equivalents.  See the <i>PlayStation&reg; Graphics Library Programming Guide</i> for details about which 
<c>cgSetParameter</c> functions can be used.


@sa cgGLGetParameter4f, cgGLSetParameterArray4f, cgGLSetMatrixParameterfr,
    cgGLSetMatrixParameterArrayfr, cgGLSetTextureParameter
 */
CGGL_API void cgGLSetParameter4f( CGparameter param, float x, float y, float z, float w )
{
    JS_APIENTRY_TIMER( cgGLSetParameter4f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_5( cgGLSetParameter4f, JSDPM_CALL_TYPE_STATE_CG, , 5,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             y, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             z, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE,
                             w, sizeof( float ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }
#endif
    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    // otherwise apply the values to the parameter
    float v[4] = {x, y, z, w};
    ptr->setterIndex( ptr, v, CG_GETINDEX( param ) );
}

/**
@short Sets the values of vector parameters

cgGLSetParameter1fv() sets the value of a vector parameter.
It differs from cgGLSetParameter4f() in the number of values taken and in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the value to set the parameter to.

@sa       cgGLSetParameter4f

*/
CGGL_API void cgGLSetParameter1fv( CGparameter param, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter1fv );
#ifndef JS_NO_ERROR_CHECK
    if ( NULL == v )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter1fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 1*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    float v2[4] = { v[0], v[0], v[0], v[0]};
    ptr->setterIndex( ptr, v2, CG_GETINDEX( param ) );
}

/**
@short Sets the values of vector parameters

cgGLSetParameter2fv() sets the value of vector parameters.
It differs from cgGLSetParameter4f() in the number of values taken and in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter2fv( CGparameter param, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter2fv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter2fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 2*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    float v2[4] = { v[0], v[1], v[1], v[1]};
    ptr->setterIndex( ptr, v2, CG_GETINDEX( param ) );
}

/**
@short Sets the values of vector parameters

cgGLSetParameter3fv() sets the value of vector parameters.
It differs from cgGLSetParameter4f() in the number of values taken and in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter3fv( CGparameter param, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter3fv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter3fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 3*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    float v2[4] = { v[0], v[1], v[2], v[2]};
    ptr->setterIndex( ptr, v2, CG_GETINDEX( param ) );
}

/**
@short Sets the values of vector parameters

cgGLSetParameter4fv() sets the value of vector parameters.
It differs from cgGLSetParameter4f() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter4fv( CGparameter param, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter4fv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter4fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 4*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == ptr ) ) return;
#endif

    float v2[4] = { v[0], v[1], v[2], v[3]};
    ptr->setterIndex( ptr, v2, CG_GETINDEX( param ) );
}

/**
 * @}  CgGL
 */

/**
 * @addtogroup CgDouble
 * @{
 */

/**
@ingroup CgDouble

@short Sets the values of scalar parameters (Not recommended for use)

cgGLSetParameter1d() sets the value of scalar parameters.
It differs from cgGLSetParameter1f() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param       param   Specifies the parameter that will be set.

@param x       The values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter1f() instead.

@sa       cgGLSetParameter1f, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter1d( CGparameter param, double x )
{
    JS_APIENTRY_TIMER( cgGLSetParameter1d );
    cgGLSetParameter1f( param, ( float )x );
}

/**
@ingroup CgDouble

@short Sets the values of scalar parameters (Not recommended for use)

cgGLSetParameter2d() sets the value of scalar parameters.
It differs from cgGLSetParameter2f() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param       param   Specifies the parameter that will be set.

@param x,y       The values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter2f() instead.

@sa       cgGLSetParameter2f, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter2d( CGparameter param, double x, double y )
{
    JS_APIENTRY_TIMER( cgGLSetParameter2d );
    // this is not on the fast path, so keep it simple.
    cgGLSetParameter2f( param, ( float )x, ( float )y );
}

/**
@ingroup CgDouble

@short Sets the values of scalar parameters (Not recommended for use)

cgGLSetParameter3d() sets the value of scalar parameters.
It differs from cgGLSetParameter3f() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param       param   Specifies the parameter that will be set.

@param x,y,z       The values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter3f() instead.

@sa       cgGLSetParameter3f, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter3d( CGparameter param, double x, double y, double z )
{
    JS_APIENTRY_TIMER( cgGLSetParameter3d );
    cgGLSetParameter3f( param, ( float )x, ( float )y, ( float )z );
}

/**
@ingroup CgDouble

@short Sets the values of scalar parameters (Not recommended for use)

cgGLSetParameter4d() sets the value of scalar parameters.
It differs from cgGLSetParameter4f() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param       param   Specifies the parameter that will be set.

@param x,y,z,w       The values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter4f() instead.

@sa       cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter4d( CGparameter param, double x, double y, double z, double w )
{
    JS_APIENTRY_TIMER( cgGLSetParameter4d );
    cgGLSetParameter4f( param, ( float )x, ( float )y, ( float )z, ( float )w );
}

/**
@ingroup CgDouble

@short Sets the values of vector parameters (Not recommended for use)

cgGLSetParameter1dv() sets the value of vector parameters.
It differs from cgGLSetParameter1fv() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter1fv() instead.

@sa       cgGLSetParameter1fv, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter1dv( CGparameter param, const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter1dv );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    cgGLSetParameter1f( param, ( float )v[0] );
}

/**
@ingroup CgDouble

@short Sets the values of vector parameters (Not recommended for use)

cgGLSetParameter2dv() sets the value of vector parameters.
It differs from cgGLSetParameter2fv() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter2fv() instead.

@sa       cgGLSetParameter2fv, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter2dv( CGparameter param, const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter2dv );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    cgGLSetParameter2f( param, ( float )v[0], ( float )v[1] );
}

/**
@ingroup CgDouble

@short Sets the values of vector parameters (Not recommended for use)

cgGLSetParameter3dv() sets the value of vector parameters.
It differs from cgGLSetParameter3fv() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter3fv() instead.

@sa       cgGLSetParameter3fv, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter3dv( CGparameter param, const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter3dv );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    cgGLSetParameter3f( param, ( float )v[0], ( float )v[1], ( float )v[2] );
}

/**
@ingroup CgDouble

@short Sets the values of vector parameters (Not recommended for use)

cgGLSetParameter4dv() sets the value of vector parameters.
It differs from cgGLSetParameter4fv() in argument type.
Refer to cgGLSetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v   For the array versions of the set functions, a pointer to the values to set the parameter to.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameter4fv() instead.

@sa       cgGLSetParameter4fv, cgGLSetParameter4f
*/
CGGL_API void cgGLSetParameter4dv( CGparameter param, const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter4dv );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( NULL == v ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    cgGLSetParameter4f( param, ( float )v[0], ( float )v[1], ( float )v[2], ( float )v[3] );
}

/**
 * @}  CgDouble
 */

/**
 * @addtogroup CgGL
 * @{
 */

/**
@short Gets the value of scalar parameters

cgGLGetParameter1f() returns the value of the specified parameter.  It differs from cgGLGetParameter4f() only in
the number of values returned.  Refer to cgGLGetParameter4f() for more information.

@param   param   Specifies the parameter to be retrieved.

@param   v       A pointer to the buffer to return the values in.

@sa       cgGetParameterValues, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter1f( CGparameter param, float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter1f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetParameter1f, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 1*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( v != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = _jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    const CgParameterResource *parameterResource = _jsGetParameterResource( rtParameter->program, parameterEntry );

    switch ( parameterResource->type )
    {
        case CG_FLOAT4:
        case CG_FLOAT3:
        case CG_FLOAT2:
        case CG_FLOAT:
        case CG_HALF4:
        case CG_HALF3:
        case CG_HALF2:
        case CG_HALF:
        case CG_INT4:
        case CG_INT3:
        case CG_INT2:
        case CG_INT:
        case CG_BOOL4:
        case CG_BOOL3:
        case CG_BOOL2:
        case CG_BOOL:
        case CG_FIXED4:
        case CG_FIXED3:
        case CG_FIXED2:
        case CG_FIXED:
            *v = *value; // fall through...
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
    }
}

/**
@short Gets the value of scalar parameters

cgGLGetParameter2f() returns the value of the specified parameter.  It differs from cgGLGetParameter4f() only in
the number of values returned.  Refer to cgGLGetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v       A pointer to the buffer to return the values in.

@sa       cgGetParameterValues, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter2f( CGparameter param, float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter2f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetParameter2f, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 2*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( v != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = _jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // peek into image for value
    const CgParameterResource *parameterResource = _jsGetParameterResource( rtParameter->program, parameterEntry );
    switch ( parameterResource->type )
    {
        case CG_FLOAT2:
        case CG_FLOAT3:
        case CG_FLOAT4:
        case CG_HALF2:
        case CG_HALF3:
        case CG_HALF4:
        case CG_INT2:
        case CG_INT3:
        case CG_INT4:
        case CG_BOOL2:
        case CG_BOOL3:
        case CG_BOOL4:
        case CG_FIXED2:
        case CG_FIXED3:
        case CG_FIXED4:
            v[0] = value[0];
            v[1] = value[1];
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            break;
    }
}

/**
@short Gets the value of scalar parameters

cgGLGetParameter3f() returns the value of the specified parameter.  It differs from cgGLGetParameter4f() only in
the number of values returned.  Refer to cgGLGetParameter4f() for more information.

@param   param   Specifies the parameter that will be set.

@param   v       A pointer to the buffer to return the values in.

@sa       cgGetParameterValues, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter3f( CGparameter param, float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter3f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetParameter3f, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 3*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( v != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = _jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // peek into image for value
    const CgParameterResource *parameterResource = _jsGetParameterResource( rtParameter->program, parameterEntry );
    switch ( parameterResource->type )
    {
        case CG_FLOAT3:
        case CG_FLOAT4:
        case CG_HALF3:
        case CG_HALF4:
        case CG_INT3:
        case CG_INT4:
        case CG_BOOL3:
        case CG_BOOL4:
        case CG_FIXED3:
        case CG_FIXED4:
            v[0] = value[0];
            v[1] = value[1];
            v[2] = value[2];
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            break;
    }
}

/**
@short Gets the value of scalar and vector parameters

The <c>cgGLGetParameter*</c> functions extract the values set by
cgGLSetParameter4f() functions.  The functions are available in various combinations.

Each function may return either 1, 2, 3, or 4 values.

There are versions of each function that take either float or double
values signified by the <I>f</I> or <I>d</I> in the function name.
The <c>cgGLGetParameter*</c> functions can only be called with uniform
numeric parameters.

@par Function Prototype Variants
@code
cgGLGetParameter{1234}{fd}(CGparameter param, type *v);
@endcode

@param param   Specifies the parameter.
@param v       A pointer to the buffer to return the values in.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to get for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetParameter4f, cgGLGetParameterArray4f
 */
CGGL_API void cgGLGetParameter4f( CGparameter param, float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter4f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetParameter4f, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 4*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( v != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = _jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // peek into image for value
    const CgParameterResource *parameterResource = _jsGetParameterResource( rtParameter->program, parameterEntry );
    switch ( parameterResource->type )
    {
        case CG_FLOAT4:
        case CG_HALF4:
        case CG_INT4:
        case CG_BOOL4:
        case CG_FIXED4:
            v[0] = value[0];
            v[1] = value[1];
            v[2] = value[2];
            v[3] = value[3];
            break;
        default:
            _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
            break;
    }
}

/**
 * @}  CgGL
 */

/**
 * @addtogroup CgDouble
 * @{
 */

/**
@ingroup CgDouble

@short Gets the value of scalar parameters (Not recommended for use)

cgGLGetParameter1d() returns the value of the specified parameter.  It differs from cgGLGetParameter1f() only in
the data type of values returned and in performance.

@param param   Specifies the parameter.
@param v       A pointer to the buffer to return the values in.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameter1f() instead.

@sa       cgGLGetParameter1f, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter1d( CGparameter param, double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter1d );
    JS_ASSERT( v != NULL );
    float f = 0.f;
    cgGLGetParameter1f( param, &f );
    v[0] = ( double ) f;
}

/**
@ingroup CgDouble

@short Gets the value of scalar parameters (Not recommended for use)

cgGLGetParameter2d() returns the value of the specified parameter.  It differs from cgGLGetParameter2f() only in
the data type of values returned and in performance.

@param param   Specifies the parameter.
@param v       A pointer to the buffer to return the values in.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameter2f() instead.

@sa       cgGLGetParameter2f, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter2d( CGparameter param, double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter2d );
    JS_ASSERT( v != NULL );
    float f[2] = {0.f, 0.f};
    cgGLGetParameter2f( param, f );
    v[0] = ( double ) f[0];
    v[1] = ( double ) f[1];
}

/**
@short Gets the value of scalar parameters (Not recommended for use)

cgGLGetParameter3d() returns the value of the specified parameter.  It differs from cgGLGetParameter3f() only in
the data type of values returned and in performance.

@param param   Specifies the parameter.
@param v       A pointer to the buffer to return the values in.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameter3f() instead.

@sa       cgGLGetParameter3f, cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter3d( CGparameter param, double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter3d );
    JS_ASSERT( v != NULL );
    float f[3] = {0.f, 0.f, 0.f};
    cgGLGetParameter3f( param, f );
    v[0] = ( double ) f[0];
    v[1] = ( double ) f[1];
    v[2] = ( double ) f[2];
}

/**
@short Gets the value of scalar parameters (Not recommended for use)

cgGLGetParameter4d() returns the value of the specified parameter.  It differs from cgGLGetParameter4f() only in
the data type of the returned values and in performance.

@param param   Specifies the parameter.
@param v       A pointer to the buffer to return the values in.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameter4f() instead.

@sa       cgGLGetParameter4f, cgGLSetParameter4f
 */
CGGL_API void cgGLGetParameter4d( CGparameter param, double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameter4d );
    JS_ASSERT( v != NULL );
    float f[4] = {0.f, 0.f, 0.f, 0.f};
    cgGLGetParameter4f( param, f );
    v[0] = ( double ) f[0];
    v[1] = ( double ) f[1];
    v[2] = ( double ) f[2];
    v[3] = ( double ) f[3];
}

/**
 * @}  CgDouble
 */

/**
 * @addtogroup CgGL
 * @{
 */
/**
@short Sets the value of scalar or vector array parameters

cgGLSetParameterArray1f() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray4f() only in the number of values returned.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@sa       cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray1f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray1f );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray1f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    ptr++;

    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        ptr->setterIndex( ptr, v + i, i + offset );
    }
}

/**
@short Sets the value of scalar or vector array parameters

cgGLSetParameterArray2f() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray4f() only in the number of values returned.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@sa       cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray2f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray2f );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray2f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*2*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );

    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    ptr++;

    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        ptr->setterIndex( ptr, v + 2 * i, i + offset );
    }
}

/**
@short Sets the value of scalar or vector array parameters

cgGLSetParameterArray3f() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray4f() only in the number of values returned.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@sa       cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray3f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray3f );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray3f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*3*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );

    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif
    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    ptr++;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        ptr->setterIndex( ptr, v + 3 * i, i + offset );
    }
}

/**
@short Sets an array of scalar or vector parameters

       The <c>cgGLSetArrayParameter*</c> functions set the value of a given scalar or
       vector array parameter.  The functions are available in various combinations.

       Each function will set either 1, 2, 3, or 4 values per array element
       depending on the function that is used.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

@par Function Prototype Variants
@code
void cgGLSetParameterArray{1234}{fd}(CGparameter param, long offset, long nelements, const type *v);
@endcode

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The <I><c>offset</c></I> and/or the <I><c>nelements</c></I> parameter are out of the array bounds.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetParameter4f, cgGLGetParameterArray4f
 */
CGGL_API void cgGLSetParameterArray4f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray4f );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray4f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*4*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif


    JS_ASSERT( v != NULL );

    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif
    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    ptr++;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        ptr->setterIndex( ptr, v + 4 * i, i + offset );
    }
}
/**
 * @}  CgGL
 */

/**
 * @addtogroup CgDouble
 * @{
 */
/**
@ingroup CgDouble

@short Sets the value of scalar or vector array parameters (Not recommended for use)

cgGLSetParameterArray1d() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray1f() in
the data type of the values returned and in performance.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameterArray1f() instead.

@sa       cgGLSetParameterArray1f, cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray1d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray1d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray1d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter * ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( ptr )
    {
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // double array API
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLSetParameter1dv( p, v + i );
        }
        _JS_DPM_TRACE_ON
    }
}

/**
@ingroup CgDouble

@short Sets the value of scalar or vector array parameters (Not recommended for use)

cgGLSetParameterArray2d() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray2f() in
the data type of the values returned and in performance.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameterArray2f() instead.

@sa       cgGLSetParameterArray2f, cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray2d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray2d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray2d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*2*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter * ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( ptr )
    {
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // double array API
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLSetParameter2dv( p, v + 2*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@short Sets the value of scalar or vector array parameters (Not recommended for use)

cgGLSetParameterArray3d() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray3f() in
the data type of the values returned and in performance.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameterArray3f() instead.

@sa       cgGLSetParameterArray3f, cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray3d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray3d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray3d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*3*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter * ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( ptr )
    {
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // double array API
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLSetParameter3dv( p, v + 3*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Sets the value of scalar or vector array parameters (Not recommended for use)

cgGLSetParameterArray4d() sets the value of a given scalar or vector array parameter.
It differs from cgGLSetParameterArray4f() in
the  data type of the values returned and in performance.
Refer to cgGLSetParameterArray4f() for more information.

@param param   Specifies the array parameter that will be set.
@param offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.

@param nelements The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.

@param v       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the vector
               size indicated by the number in the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetParameterArray4f() instead.

@sa       cgGLSetParameter4f
 */
CGGL_API void cgGLSetParameterArray4d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       const double *v )
{
    JS_APIENTRY_TIMER( cgGLSetParameterArray4d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetParameterArray4d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*4*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter * ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( ptr )
    {
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( ptr->program, ptr->parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // double array API
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLSetParameter4dv( p, v + 4*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
 * @}  CgDouble
 */

/**
 * @addtogroup CgGL
 * @{
 */
/**
@short Gets the values of scalar or vector array parameters

cgGLGetParameterArray1f() retrieves an array of values from a give scalar or vector array parameter.
Refer to cgGLGetParameterArray4f() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@sa cgGLGetParameter4f, cgGetParameterValues
 *
 */
CGGL_API void cgGLGetParameterArray1f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray1f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray1f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _cgGLTestArrayParameter( param, offset, nelements ) == NULL ) ) return;
#endif
    _JS_DPM_TRACE_OFF // get array API
    if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLGetParameter1f( p, v + i );
    }
    _JS_DPM_TRACE_ON
}

/**
@short Gets the values of scalar or vector array parameters

cgGLGetParameterArray2f() retrieves an array of values from a give
scalar or vector array parameter.  Refer to cgGLGetParameterArray4f()
for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@sa cgGLGetParameter4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray2f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray2f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray2f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*2*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _cgGLTestArrayParameter( param, offset, nelements ) == NULL ) ) return;
#endif
    _JS_DPM_TRACE_OFF // get array API
    if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLGetParameter2f( p, v + 2*i );
    }
    _JS_DPM_TRACE_ON
}

/**
@short Gets the values of scalar or vector array parameters

cgGLGetParameterArray3f() retrieves an array of values from a give
scalar or vector array parameter.  Refer to cgGLGetParameterArray4f()
for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@sa cgGLGetParameter4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray3f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray3f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray3f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*3*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _cgGLTestArrayParameter( param, offset, nelements ) == NULL ) ) return;
#endif
    _JS_DPM_TRACE_OFF // get array API

    if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLGetParameter3f( p, v + 3*i );
    }
    _JS_DPM_TRACE_ON
}

/**
@short Gets the values of scalar or vector array parameters

The <c>cgGLGetParameterArray*</c> functions retrieve an array of values from a give scalar or vector array parameter.
The functions are available in various combinations.

       Each function will retrieve either 1, 2, 3, or 4 values per array 
       element depending on the function that is used.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

@par Function Prototype Variants
@code
void cgGLGetParameterArray{1234}{fd}(CGparameter param, long offset, long nelements, const type *v);
@endcode

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The <I><c>offset</c></I> and/or the <I><c>nelements</c></I> parameter are out of the array bounds.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLGetParameter4f, cgGLSetParameter4f, cgGLSetParameterArray4f
 */
CGGL_API void cgGLGetParameterArray4f( CGparameter param,
                                       long offset,
                                       long nelements,
                                       float *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray4f );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray4f, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*4*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _cgGLTestArrayParameter( param, offset, nelements ) == NULL ) ) return;
#endif
    _JS_DPM_TRACE_OFF // get array API

    if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLGetParameter4f( p, v + 4*i );
    }
    _JS_DPM_TRACE_ON
}

/**
 * @}  CgGL
 */

/**
 * @addtogroup CgDouble
 * @{
 */

/**
@ingroup CgDouble

@short Gets the values of scalar or vector array parameters (Not recommended for use)

cgGLGetParameterArray1d() retrieves an array of values from a given
scalar or vector array parameter.  It differs from cgGLGetParameterArray1f() in the data type of the values
returned and in performance.
Refer to cgGLGetParameterArray4f() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameterArray1f() instead.

@sa cgGLGetParameterArray1f, cgGLGetParameter4f, cgGLGetParameterArray4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray1d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray1d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray1d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( ptr != NULL ) )
    {
        _JS_DPM_TRACE_OFF // get array API
        if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
        // loop over array elements
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLGetParameter1d( p, v + i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Gets the values of scalar or vector array parameters (Not recommended for use)

cgGLGetParameterArray2d() differs from cgGLGetParameterArray2f() in
the number and data type of the values returned, and in performance.
Refer to cgGLGetParameterArray4f() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameterArray2f() instead.

@sa cgGLGetParameterArray2f, cgGLGetParameter4f, cgGLGetParameterArray4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray2d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray2d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray2d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*2*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( ptr != NULL ) )
    {
        _JS_DPM_TRACE_OFF // get array API
        if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
        // loop over array elements
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLGetParameter2d( p, v + 2*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Gets the values of scalar or vector array parameters (Not recommended for use)

cgGLGetParameterArray3d() differs from cgGLGetParameterArray3f() in
the number and data type of the values returned, and in performance.
Refer to cgGLGetParameterArray4f() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameterArray3f() instead.

@sa cgGLGetParameterArray3f, cgGLGetParameter4f, cgGLGetParameterArray4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray3d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray3d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray3d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*3*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter * ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( ptr != NULL ) )
    {
        _JS_DPM_TRACE_OFF // get array API
        if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
        // loop over array elements
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLGetParameter3d( p, v + 3*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Gets the values of scalar or vector array parameters (Not recommended for use)

cgGLGetParameterArray4d() differs from cgGLGetParameterArray4f() in
the data type of the values returned and in performance.
Refer to cgGLGetParameterArray4f() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements
               The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param v       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the vector size indicated by the number in
               the function name.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetParameterArray4f() instead.

@sa cgGLGetParameter4f, cgGLGetParameterArray4f, cgGetParameterValues
*/
CGGL_API void cgGLGetParameterArray4d( CGparameter param,
                                       long offset,
                                       long nelements,
                                       double *v )
{
    JS_APIENTRY_TIMER( cgGLGetParameterArray4d );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetParameterArray4d, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, nelements*4*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( v != NULL );
    CgRuntimeParameter* ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( ptr != NULL ) )
    {
        _JS_DPM_TRACE_OFF // get array API
        if ( nelements == 0 )  nelements = cgGetArraySize( param, 0 ) - offset;
        // loop over array elements
        for ( int i = 0; i < nelements; ++i )
        {
            CGparameter p = cgGetArrayParameter( param, i + offset );
            cgGLGetParameter4d( p, v + 4*i );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
 * @}  CgDouble
 */

/**
 * @addtogroup CgGL
 * @{
 */

/**
@short Sets a varying parameter with an attribute array

cgGLSetParameterPointer() sets a varying parameter to a given vertex
array in the typical OpenGL style.  See the OpenGL documentation on the
various vertex array functions (e.g. glVertexPointer(), glNormalPointer(),
etc...) for more information.

@param       param   Specifies the parameter that will be set.
@param       fsize   The number of coordinates per vertex.
@param       type    The data type of each coordinate.  Possible values are
<CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_SHORT</CODE>, <CODE>GL_INT</CODE>, <CODE>GL_FLOAT</CODE>,
<CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_DOUBLE</CODE>, and <CODE>GL_FIXED_11_11_10_SCE</CODE>.
@param       stride  Specifies the byte offset between consecutive vertices.  If
               <CODE>stride</CODE> is 0 the array is assumed to be tightly packed.
@param       pointer Specifies the pointer to the first coordinate in the vertex array.

@note
The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
The component placement within the word is:
<TABLE>
<TR>
<TD align="center">Component #</TD>
<TD align="center">Bits</TD>
<TD align="center">Shift</TD>
</TR>
<TR>
<TD>0</TD><TD>11</TD><TD>0</TD>
</TR>
<TR>
<TD>1</TD><TD>11</TD><TD>11</TD>
</TR>
<TR>
<TD>2</TD><TD>10</TD><TD>22</TD>
</TR>
</TABLE>
The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
@par
When <I><c>type</c></I> is set to <CODE>GL_FIXED_11_11_10_SCE</CODE>, <I><c>fsize</c></I> must be 3.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_UNSUPPORTED_GL_EXTENSION_ERROR</CODE></TD>
<TD><I><c>param</c></I> requires a GL extension that is not available.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetParameter4f, cgGLSetParameterElementFunc
 */
CGGL_API void cgGLSetParameterPointer( CGparameter param,
                                       GLint fsize,
                                       GLenum type,
                                       GLsizei stride,
                                       const GLvoid *pointer )
{
    JS_APIENTRY_TIMER( cgGLSetParameterPointer );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_5( cgGLSetParameterPointer, JSDPM_CALL_TYPE_STATE_CG, , 5,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             fsize, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }
#endif

    CgRuntimeParameter *_ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif

    const CgParameterResource *parameterResource = _jsGetParameterResource( _ptr->program, _ptr->parameterEntry );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY(( _ptr->parameterEntry->flags & CGPV_MASK ) != CGPV_VARYING || parameterResource == 0 || _jsGetBaseResource(( CGresource )parameterResource->resource ) != CG_ATTR0 ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    GLuint index = ( GLuint )( parameterResource->resource - CG_ATTR0 );
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );

    _jsVertexAttribPointerNV(
        index,    // attribute index
        fsize,                  // element size
        type,                   // GL_FLOAT
        ( _ptr->parameterEntry->flags & CGP_NORMALIZE ) ? 1 : 0,  // from the semantic of the param
        stride,                 // element to element in bytes
        pointer );              // data pointer
}

/**
@short Sets an attribute pointer

cgGLAttribPointer() sets a varying parameter to a given vertex array. This
is very similar to cgGLSetParameterPointer(), except the parameter is
specified by index, and one can specify whether or not the type is
normalized.

@param       index      An integer between 0 and 15, which specifies the index of the attribute to be set.
The actual resource is <CODE>CG_ATTR0</CODE> + <I><c>index</c></I>.
@param       fsize      The number of coordinates per vertex.
@param       type       The data type of each coordinate.  Possible values are
<CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_FLOAT</CODE>,
<CODE>GL_HALF_FLOAT_ARB</CODE>, and <CODE>GL_FIXED_11_11_10_SCE</CODE>.
@param       normalize  Whether or not to normalize fixed point types. If true, unsigned types (<CODE>GL_UNSIGNED_BYTE</CODE>)
will be re-mapped to [0..1], and signed types (<CODE>GL_SHORT</CODE> and <CODE>GL_INT</CODE>) will be re-mapped to [-1..1].
If false, the values are not re-mapped and keep their original values.
@param       stride     Specifies the byte offset between consecutive vertices.  If
               <CODE>stride</CODE> is 0 the array is assumed to be tightly packed.
@param       pointer    Specifies the pointer to the first coordinate in the vertex array.

@note
The format for <CODE>GL_FIXED_11_11_10_SCE</CODE> is 3 signed components packed into 32 bits.
The component placement within the word is:
<TABLE>
<TR>
<TD align="center">Component #</TD>
<TD align="center">Bits</TD>
<TD align="center">Shift</TD>
</TR>
<TR>
<TD>0</TD><TD>11</TD><TD>0</TD>
</TR>
<TR>
<TD>1</TD><TD>11</TD><TD>11</TD>
</TR>
<TR>
<TD>2</TD><TD>10</TD><TD>22</TD>
</TR>
</TABLE>
The interpretation of each component in the vertex program is normalized from [-1.0, 1.0].
@par
When <I><c>type</c></I> is set to <CODE>GL_FIXED_11_11_10_SCE</CODE>, <I><c>fsize</c></I> must be 3.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not an integer value from 0 to 15.</TD>
</TR>
</TABLE>

@sa cgGLSetParameterPointer, cgGLAttribElementFunc, cgGLEnableAttrib, cgGLDisableAttrib, cgGLAttribValues
 */
CGGL_API void cgGLAttribPointer( GLuint index, GLint fsize, GLenum type, GLboolean normalize, GLsizei stride, const GLvoid *pointer )
{
    JS_APIENTRY_TIMER( cgGLAttribPointer );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_6( cgGLAttribPointer, JSDPM_CALL_TYPE_STATE_CG, , 6,
                             index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             fsize, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             normalize, sizeof( GLboolean ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             stride, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             pointer, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );	// just save the pointer value (not the contents) for deferred evaluation
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( index >= _JS_MAX_VERTEX_ATTRIBS ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    _jsVertexAttribPointerNV(
        index,    // attribute index
        fsize,                  // element size
        type,                   // GL_FLOAT
        normalize,
        stride,                 // element to element in bytes
        pointer );              // data pointer
}
/**
@short Specifies a mathematical operation on vertex array indices that are referenced by a Cg parameter

A function like glDrawArrays() refers to array indices to access vertex array data.
The function cgGLSetParameterElementFunc() requires a Cg parameter that refers
to an array of data. It performs a mathematical operation (either
a division or modulo operation) on the array index number to generate a new array index reference. cgGLSetParameterElementFunc() may be useful to generate a larger
set of multiple instances of data by
using different combinations of smaller sets of array data.

If you do not specify a func or frequency value, the function uses the last used values. 
If no values were previously set, the default behavior uses <I><CODE>func</CODE></I> as <CODE>GL_DIVIDE_SCE</CODE>, 
and <I><CODE>frequency</CODE></I> as 1. Of course, dividing by 1 does not change the index value. 


If cgGLSetParameterElementFunc() has been called with <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> values 
other than the defaults, these settings will persist until reset with another call to the 
function with explicitly set <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> values. Therefore to 
ensure the behavior of cgGLSetParameterElementFunc() is predictable, specify <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> 
values in every cgGLSetParameterElementFunc() call. Alternatively, after a draw call using non-default 
values for <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I>, you can reset the <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> 
values to their defaults with another call to cgGLSetParameterElementFunc().

@param       param   Specifies the parameter that will be modified.
@param       func    The function to perform on the array index, either <CODE>GL_DIVIDE_SCE</CODE> or <CODE>GL_MODULO_SCE</CODE>.
@param       frequency   Defines the second operand of the division (divisor) or modulo operation. For
the <code>GL_MODULO_SCE</code> case, the RSX&trade; modulo value is 16 bits internally, so this value must be
below 2^16. A warning report will be generated if this value exceeded.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>Accessing the parameter failed.</TD>
</TR>
</TABLE>

@sa cgGLSetParameterPointer, glDrawArrays
 */
CGGL_API void cgGLSetParameterElementFunc( CGparameter param, GLenum func, GLuint frequency )
{
    if ( func == GL_MODULO_SCE && frequency > 0xffff ) _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "cgGLSetParameterElementFunc GL_MODULO_SCE frequency value > 0xffff (16 bits)" );
    JS_APIENTRY_TIMER( cgGLSetParameterElementFunc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetParameterElementFunc, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             func, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             frequency, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    CgRuntimeParameter *_ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif

    const CgParameterResource *parameterResource = _jsGetParameterResource( _ptr->program, _ptr->parameterEntry );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY(( _ptr->parameterEntry->flags & CGPV_MASK ) != CGPV_VARYING || parameterResource == 0 || _jsGetBaseResource(( CGresource )parameterResource->resource ) != CG_ATTR0 ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    GLuint index = ( GLuint )( parameterResource->resource - CG_ATTR0 );
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );
    _jsVertexAttribElementFunc( index, func, frequency );
}

/**
@short Specifies a mathematical operation on vertex array indices that are referenced by Cg vertex attributes

A function like glDrawArrays() refers to array indices to access vertex array data.
The function cgGLAttribElementFunc() references arrays of data through Cg semantics (<CODE>GL_ATTR0</CODE> to <CODE>GL_ATTR15</CODE>). It performs a mathematical operation
(either a division or modulo operation) on the array index number to generate
a new array index reference.
cgGLAttribElementFunc() may be useful to generate a larger set of multiple instances of
data by using different combinations of smaller sets of array data.

If you do not specify a func or frequency value, the function uses the last used values. 
If no values were previously set, the default behavior uses <I><CODE>func</CODE></I> as <CODE>GL_DIVIDE_SCE</CODE>, 
and <I><CODE>frequency</CODE></I> as 1. Of course, dividing by 1 does not change the array index value. 

If cgGLAttribElementFunc() has been called with <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> values 
other than the defaults, these settings will persist until reset with another call to the 
function with explicitly set <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> values. Therefore to 
ensure the behavior of cgGLAttribElementFunc() is predictable, specify <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> 
values in every cgGLAttribElementFunc() call. Alternatively, after a draw call using non-default 
values for <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I>, you can reset the <I><CODE>func</CODE></I> and <I><CODE>frequency</CODE></I> 
values to their defaults with another call to cgGLAttribElementFunc().

@param       index      An integer between 0 and 15, which specifies the index of the attribute to be set.
The actual resource is <CODE>CG_ATTR0</CODE> + <I><c>index</c></I>.
@param       func    The function to perform on the array index, either <CODE>GL_DIVIDE_SCE</CODE> or <CODE>GL_MODULO_SCE</CODE>.
@param       frequency   Defines the second operand of the division (divisor) or modulo operation.
For the <code>GL_MODULO_SCE</code> case, the RSX&trade; modulo value is 16 bits internally, so this value must be
below 2^16. A warning report will be generated if this value exceeded.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not an integer value from 0 to 15.</TD>
</TR>
</TABLE>

@sa cgGLSetParameterPointer, glDrawArrays
 */
CGGL_API void cgGLAttribElementFunc( GLuint index, GLenum func, GLuint frequency )
{
    if ( func == GL_MODULO_SCE && frequency > 0xffff ) _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "cgGLAttribElementFunc GL_MODULO_SCE frequency value > 0xffff (16 bits)" );
    JS_APIENTRY_TIMER( cgGLAttribElementFunc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLAttribElementFunc, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             func, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             frequency, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( index >= _JS_MAX_VERTEX_ATTRIBS ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    _jsVertexAttribElementFunc( index, func, frequency );
}


/**
@short Sets an attribute vertex values

cgGLAttribValues() sets a varying parameter to given values.
This allows greater flexibility than cgGLAttribPointer(),
particularly for indexed vertex data, because the user has more
freedom than just specifying the mathematical operation to
apply to the indices of an array of data.



@param       index      An integer between 0 and 15, which specifies the index of the attribute to be set.
The actual resource is <CODE>CG_ATTR0</CODE> + <I><c>index</c></I>.
@param       x          The values to set the attribute x component to
@param       y          The values to set the attribute y component to
@param       z          The values to set the attribute z component to
@param       w          The values to set the attribute w component to

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not an integer value from 0 to 15.</TD>
</TR>
</TABLE>

@sa cgGLEnableAttrib, cgGLDisableAttrib, cgGLAttribPointer,
 */
CGGL_API void cgGLAttribValues( GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    JS_APIENTRY_TIMER( cgGLAttribValues );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_5( cgGLAttribValues, JSDPM_CALL_TYPE_STATE_CG, , 5,
                             index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             y, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             z, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             w, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( index >= _JS_MAX_VERTEX_ATTRIBS ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    _jsVertexAttrib4fNV( index, x, y, z, w );
}

/**
@short Enables a vertex attribute in the OpenGL state

cgGLEnableAttrib() enables the specified vertex attribute

@param       index      An integer between 0 and 15, which specifies the index of the attribute index to enable.
The actual resource is <CODE>CG_ATTR0</CODE> + <I><c>index</c></I>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not an integer value from 0 to 15.</TD>
</TR>
</TABLE>

@sa cgGLDisableAttrib, cgGLAttribPointer
 */
CGGL_API void cgGLEnableAttrib( GLuint index )
{
    JS_APIENTRY_TIMER( cgGLEnableAttrib );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLEnableAttrib, JSDPM_CALL_TYPE_STATE_CG, , 1, index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( index >= _JS_MAX_VERTEX_ATTRIBS ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    _jsEnableVertexAttribArrayNV( index );
}

/**
@short Disables a vertex attribute in the OpenGL state

cgGLDisableAttrib() disables the specified vertex attribute

@param       index      An integer between 0 and 15, which specifies the index of the attribute index to disable.
The actual resource is <CODE>CG_ATTR0</CODE> + <I><c>index</c></I>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not an integer value from 0 to 15.</TD>
</TR>
</TABLE>

@sa cgGLEnableAttrib, cgGLAttribPointer
 */
CGGL_API void cgGLDisableAttrib( GLuint index )
{
    JS_APIENTRY_TIMER( cgGLDisableAttrib );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLDisableAttrib, JSDPM_CALL_TYPE_STATE_CG, , 1, index, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( index >= _JS_MAX_VERTEX_ATTRIBS ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    _jsDisableVertexAttribArrayNV( index );
}

/**
@short Enables a vertex attribute in the OpenGL state

cgGLEnableClientState() enables the vertex attribute
associated with the given varying parameter.

@param       param   Specifies the parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a varying parameter.</TD>
</TR>
</TABLE>

@sa cgGLDisableClientState
 */
CGGL_API void cgGLEnableClientState( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLEnableClientState );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLEnableClientState, JSDPM_CALL_TYPE_STATE_CG, , 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    CgRuntimeParameter *_ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif

    const CgParameterResource *parameterResource = _jsGetParameterResource( _ptr->program, _ptr->parameterEntry );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY(( _ptr->parameterEntry->flags & CGPV_MASK ) != CGPV_VARYING || parameterResource == 0 || _jsGetBaseResource(( CGresource )parameterResource->resource ) != CG_ATTR0 ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif
    GLuint index = ( GLuint )( parameterResource->resource - CG_ATTR0 );
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );
    _jsEnableVertexAttribArrayNV( index );
}

/**
@short Disables a vertex attribute in the OpenGL state

cgGLDisableClientState() disables the vertex attribute
associated with the given varying parameter.

@param       param   Specifies the parameter.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a varying parameter.</TD>
</TR>
</TABLE>

@sa cgGLEnableClientState
 */
CGGL_API void cgGLDisableClientState( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLDisableClientState );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLDisableClientState, JSDPM_CALL_TYPE_STATE_CG, , 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    CgRuntimeParameter *_ptr = _jsCgGLTestParameter( param );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif

    const CgParameterResource *parameterResource = _jsGetParameterResource( _ptr->program, _ptr->parameterEntry );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY(( _ptr->parameterEntry->flags & CGPV_MASK ) != CGPV_VARYING || parameterResource == 0 || _jsGetBaseResource(( CGresource )parameterResource->resource ) != CG_ATTR0 ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    GLuint index = ( GLuint )( parameterResource->resource - CG_ATTR0 );
    JS_ASSERT( index < _JS_MAX_VERTEX_ATTRIBS );
    _jsDisableVertexAttribArrayNV( index );
}

/******************************************************************************
 *** Matrix Parameter Managment Functions
 *****************************************************************************/
/**
@ingroup CgDouble

@short Sets the value of matrix parameters (Not recommended for use)

cgGLSetMatrixParameterdc() sets the value of a given matrix parameter.
It differs from cgGLSetMatrixParameterfc() only in argument type and performance.

@param       param   Specifies the parameter that will be set.

@param       matrix  An array of values to set the matrix parameter to.
The array must be the number of rows times the number of columns in size.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetMatrixParameterfc() instead.

@sa cgGLGetMatrixParameterfr, cgGLSetParameter4f, cgGLSetMatrixParameterfc
*/
CGGL_API void cgGLSetMatrixParameterdc( CGparameter param, const double*matrix )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterdc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetMatrixParameterdc, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    CgRuntimeParameter* _ptr = _jsCgGLTestParameter( param );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int rows = _jsGetTypeRowCount( parameterType );
        unsigned int cols = _jsGetTypeColCount( parameterType );

        // get dimensions
        if ( JS_LIKELY( rows != 0 ) )
        {
            // convert to float matrix
            GLfloat m[16];
            for ( GLuint i = 0; i < rows; ++i )
                for ( GLuint j = 0; j < cols; ++j )
                    m[i*cols + j] = ( float )matrix[i*cols + j];
            _ptr->settercIndex( _ptr, m, CG_GETINDEX( param ) );
        }
        else
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        }
    }
}

/**
@short Sets the value of matrix parameters

cgGLSetMatrixParameterfc() sets the value of a given matrix parameter.
It differs from cgGLSetMatrixParameterfr() only in that the values are assumed to be in column
order.  Refer to cgGLSetMatrixParameterfr() for more information.

@param       param   Specifies the parameter that will be set.

@param       matrix  An array of values to set the matrix parameter to.
The array must be the number of rows times the number of columns in size.

@sa cgGLGetMatrixParameterfr, cgGLSetParameter4f
*/
CGGL_API void cgGLSetMatrixParameterfc( CGparameter param, const float *matrix )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterfc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetMatrixParameterfc, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    CgRuntimeParameter* ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif
    ptr->settercIndex( ptr, matrix, CG_GETINDEX( param ) );
}

/**
@ingroup CgDouble

@short Sets the value of matrix parameters (Not recommended for use)

cgGLSetMatrixParameterdr() sets the value of a given matrix parameter.
It differs from cgGLSetMatrixParameterfr() only in argument type and performance.

@param       param   Specifies the parameter that will be set.

@param       matrix  An array of values to set the matrix parameter to.
The array must be the number of rows times the number of columns in size.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetMatrixParameterfr() instead.

@sa cgGLGetMatrixParameterfr, cgGLSetParameter4f, cgGLSetMatrixParameterfr
*/
CGGL_API void cgGLSetMatrixParameterdr( CGparameter param, const double*matrix )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterdr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetMatrixParameterdc, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    CgRuntimeParameter* _ptr = _jsCgGLTestParameter( param );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int rows = _jsGetTypeRowCount( parameterType );
        unsigned int cols = _jsGetTypeColCount( parameterType );

        // test matrix
        if ( JS_LIKELY( rows != 0 ) )
        {
            // convert to float matrix
            GLfloat m[16];
            for ( GLuint i = 0; i < rows; ++i )
                for ( GLuint j = 0; j < cols; ++j )
                    m[i*cols + j] = ( float )matrix[i*cols + j];
            _ptr->setterrIndex( _ptr, m, CG_GETINDEX( param ) );
        }
        else
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        }
    }
}

/**
@short Sets the value of matrix parameters

       The <c>cgGLSetMatrixParameter*</c> functions set the value of a given matrix
       parameter.  The functions are available in various combinations.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

       There are versions of each function that assume the array of values are
       laid out in either row or column order signified by the <I>r</I> or <I>c</I> in the
       function name respectively.

       The <c>cgGLSetMatrixParameter*</c> functions may only be called with uniform
       parameters.

@par Function Prototype Variants
@code
void cgGLSetMatrixParameter{fd}{rc}(CGparameter param, const type *matrix);
@endcode
@param       param   Specifies the parameter that will be set.

@param       matrix  An array of values to set the matrix parameter to.
The array must be the number of rows times the number of columns in size.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_NOT_MATRIX_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a matrix parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to retrieve for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLGetMatrixParameterfr, cgGLSetMatrixParameterArrayfr, cgGLSetParameter4f

*/
CGGL_API void cgGLSetMatrixParameterfr( CGparameter param, const float *matrix )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterfr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetMatrixParameterfr, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    CgRuntimeParameter *ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif
    ptr->setterrIndex( ptr, matrix, CG_GETINDEX( param ) );
}

/**
@ingroup CgDouble

@short Gets the value of matrix parameters (Not recommended for use)

cgGLGetMatrixParameterdr() retrieves the value of a given
matrix parameter.  It differs from cgGLGetMatrixParameterfr() only in argument type.
Refer to cgGLGetMatrixParameterfr() for more information.

@param       param   Specifies the parameter that will be retrieved.
@param       matrix  An array to retrieve the matrix parameter to.  The array must
               be the number of rows times the number of columns in size.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetMatrixParameterfr() instead.


@sa cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterfr, cgGLSetParameter4f
*/
CGGL_API void cgGLGetMatrixParameterdr( CGparameter param, double *matrix )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterdr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetMatrixParameterdr, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );

    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    CGtype parameterType = _jsGetParameterCGtype( rtParameter->program, parameterEntry );
    unsigned int rows = _jsGetTypeRowCount( parameterType );
    unsigned int cols = _jsGetTypeColCount( parameterType );

    if ( rows == 0 )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return;
    }

    /* peek into argument block */
    for ( GLuint i = 0; i < rows; ++i )
        for ( GLuint j = 0; j < cols; ++j )
            matrix[i*cols + j] = ( double )value[i * 4 + j];
}

/**
@short Retrieves the value of matrix parameters

       The <c>cgGLGetMatrixParameter*</c> functions retrieve the value of a given
       matrix parameter.  The functions are available in various combinations.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

       There are versions of each function that assume the array of values are
       laid out in either row or column order signified by the <I>r</I> or <I>c</I> in the
       function name respectively.

       The <c>cgGLGetMatrixParameter*</c> functions may only be called with uniform
       parameters.

@par Function Prototype Variants
@code
void cgGLGetMatrixParameter{fd}{rc}(CGparameter param, const type *matrix);
@endcode
@param       param   Specifies the parameter that will be retrieved.
@param       matrix  An array to retrieve the matrix parameter to.  The array must
               be the number of rows times the number of columns in size.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_NOT_MATRIX_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a matrix parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to retrieve for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterfr, cgGLSetParameter4f
 */
CGGL_API void cgGLGetMatrixParameterfr( CGparameter param, float *matrix )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterfr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetMatrixParameterfr, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    CGtype parameterType = _jsGetParameterCGtype( rtParameter->program, parameterEntry );
    unsigned int rows = _jsGetTypeRowCount( parameterType );
    unsigned int cols = _jsGetTypeColCount( parameterType );

    if ( rows == 0 )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return;
    }

    /* peek into argument block */
    for ( GLuint i = 0; i < rows; ++i )
        for ( GLuint j = 0; j < cols; ++j )
            matrix[i*cols + j] = value[i * 4 + j];
}

/**
@ingroup CgDouble

@short Gets the value of matrix parameters (Not recommended for use)

cgGLGetMatrixParameterdc() retrieves the value of a given
matrix parameter.  It differs from cgGLGetMatrixParameterfc() only in argument type.
Refer to cgGLGetMatrixParameterfc() for more information.

@param       param   Specifies the parameter that will be retrieved.
@param       matrix  An array to retrieve the matrix parameter to.  The array must
               be the number of rows times the number of columns in size.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetMatrixParameterfc() instead.


@sa cgGLGetMatrixParameterfc, cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterfr, cgGLSetParameter4f
*/
CGGL_API void cgGLGetMatrixParameterdc( CGparameter param, double *matrix )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterdc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetMatrixParameterdc, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );


    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    // uniforms only
    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    CGtype parameterType = _jsGetParameterCGtype( rtParameter->program, parameterEntry );
    unsigned int rows = _jsGetTypeRowCount( parameterType );
    unsigned int cols = _jsGetTypeColCount( parameterType );

    if ( rows == 0 )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return;
    }

    /* peek into argument block */
    for ( GLuint i = 0; i < rows; ++i )
        for ( GLuint j = 0; j < cols; ++j )
            matrix[i*cols + j] = ( double )value[i + j * 4];
}

/**
@short Gets the value of matrix parameters

cgGLGetMatrixParameterfc() retrieves the value of a given
matrix parameter.  It differs from cgGLGetMatrixParameterfr() only that the values are considered to
be in column order. Refer to cgGLGetMatrixParameterfr() for more information.

@param       param   Specifies the parameter that will be retrieved.
@param       matrix  An array to retrieve the matrix parameter to.
The array must be the number of rows times the number of columns in size.

@sa cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterfr, cgGLSetParameter4f
*/
CGGL_API void cgGLGetMatrixParameterfc( CGparameter param, float *matrix )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterfc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLGetMatrixParameterfc, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, 16*sizeof( float ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrix != NULL );

    //check parameter handle
    CgRuntimeParameter *rtParameter = ( CgRuntimeParameter* )_jsCgGLTestParameter( param );
    if ( !rtParameter )
        return;

    const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
    if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
            (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    float* value = _jsGetUniformValuePtr( param, rtParameter );
    if ( !value )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }

    CGtype parameterType = _jsGetParameterCGtype( rtParameter->program, parameterEntry );
    unsigned int rows = _jsGetTypeRowCount( parameterType );
    unsigned int cols = _jsGetTypeColCount( parameterType );

    if ( rows == 0 )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return;
    }

    /* peek into argument block */
    for ( GLuint i = 0; i < rows; ++i )
        for ( GLuint j = 0; j < cols; ++j )
            matrix[i*cols + j] = value[i + j * 4];
}

/**
@short Sets the value of a matrix parameter to a matrix in the OpenGL state

       cgGLSetStateMatrixParameter() retrieves matrices from the OpenGL state
       and sets a matrix parameter to the matrix.  The matrix may optionally
       be transformed before it is set via the transform parameter.

       cgGLSetStateMatrixParameter() may only be called with any uniform matrix
       parameter.  If the size of the matrix is less than 4x4, the upper left
       corner of the matrix that fits into the given matrix parameter will be set.

@param param   Specifies the parameter that will be set.
@param matrix  An enumerant indicating which matrix should be retrieved from
               the OpenGL state.  It may be any one of:
\arg <CODE>CG_GL_MODELVIEW_MATRIX:</CODE>  The current modelview matrix.
\arg <CODE>CG_GL_PROJECTION_MATRIX: </CODE>  The current projection matrix.
\arg <CODE>CG_GL_TEXTURE_MATRIX: </CODE> The current texture matrix.
\arg <CODE>CG_GL_MODELVIEW_PROJECTION_MATRIX</CODE> The concatenated modelview and projection matrices.

@param transform  An optional transformation that may be applied to the OpenGL
state matrix before it is set.  The enumerants may be any one of:
\arg <CODE>CG_GL_MATRIX_IDENTITY: </CODE> Doesn't apply any transform leaving the matrix as is.
\arg <CODE>CG_GL_MATRIX_TRANSPOSE:</CODE>  Transposes the matrix.
\arg <CODE>CG_GL_MATRIX_INVERSE</CODE> Inverts the matrix.
\arg <CODE>CG_GL_MATRIX_INVERSE_TRANSPOSE: </CODE> Transforms and inverts the matrix.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_NOT_MATRIX_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a matrix parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD>One of more of the enumerant parameters to cgGLSetStateMatrixParameter are invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetMatrixParameterfr, cgGLGetMatrixParameterfr

 */
CGGL_API void cgGLSetStateMatrixParameter( CGparameter param,
        CGGLenum matrix,
        CGGLenum transform )
{
    JS_APIENTRY_TIMER( cgGLSetStateMatrixParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetStateMatrixParameter, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrix, sizeof( CGGLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             transform, sizeof( CGGLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
    _JS_DPM_TRACE_OFF // gl get matrix
#endif
    // retrieve matrix from GL
    float m[4][4];
    switch ( matrix )
    {
        case CG_GL_MODELVIEW_MATRIX:
            glGetFloatv( GL_MODELVIEW_MATRIX, ( float* )m );
            break;
        case CG_GL_PROJECTION_MATRIX:
            glGetFloatv( GL_PROJECTION_MATRIX, ( float* )m );
            break;
        case CG_GL_TEXTURE_MATRIX:
            glGetFloatv( GL_TEXTURE_MATRIX, ( float* )m );
            break;
        case CG_GL_MODELVIEW_PROJECTION_MATRIX:
        {
            float mv[4][4], p[4][4];
            glGetFloatv( GL_MODELVIEW_MATRIX, ( float* )mv );
            glGetFloatv( GL_PROJECTION_MATRIX, ( float* )p );

#define M(I,J) \
			m[I][J] = mv[I][0]*p[0][J] + mv[I][1]*p[1][J] + mv[I][2]*p[2][J] + mv[I][3]*p[3][J]
            M( 0, 0 ); M( 0, 1 ); M( 0, 2 ); M( 0, 3 );
            M( 1, 0 ); M( 1, 1 ); M( 1, 2 ); M( 1, 3 );
            M( 2, 0 ); M( 2, 1 ); M( 2, 2 ); M( 2, 3 );
            M( 3, 0 ); M( 3, 1 ); M( 3, 2 ); M( 3, 3 );
#undef M
        }
        break;
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            _JS_DPM_TRACE_ON
            return;
    }
    _JS_DPM_TRACE_ON // end gl get matrix

    // apply transform
    switch ( transform )
    {
        case CG_GL_MATRIX_IDENTITY:
            break;
        case CG_GL_MATRIX_TRANSPOSE:
#define SWAP(A,B) do { float tmp; tmp = A; A = B; B = tmp; } while (0)
            SWAP( m[0][1], m[1][0] );
            SWAP( m[0][2], m[2][0] );
            SWAP( m[0][3], m[3][0] );
            SWAP( m[1][2], m[2][1] );
            SWAP( m[1][3], m[3][1] );
            SWAP( m[2][3], m[3][2] );
#undef SWAP
            break;
        case CG_GL_MATRIX_INVERSE:
            jsMatrixInverse(( float * )m, ( float * )m );
            break;
        case CG_GL_MATRIX_INVERSE_TRANSPOSE:
#define SWAP(A,B) do { float tmp; tmp = A; A = B; B = tmp; } while (0)
            SWAP( m[0][1], m[1][0] );
            SWAP( m[0][2], m[2][0] );
            SWAP( m[0][3], m[3][0] );
            SWAP( m[1][2], m[2][1] );
            SWAP( m[1][3], m[3][1] );
            SWAP( m[2][3], m[3][2] );
#undef SWAP
            jsMatrixInverse(( float * )m, ( float * )m );
            break;
        default:
            _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
            return;
    }

    // set parameter to matrix
    //  Because the parameter may not be a 4x4 matrix and the set
    //  functions take packed data, we have to make another copy.
    //  Maybe there should be a fastpath for 4x4.
    CgRuntimeParameter* ptr = _jsCgGLTestParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
    // uniforms only
    if ( JS_UNLIKELY( !( ptr->parameterEntry->flags & CGPV_UNIFORM ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    CGtype parameterType = _jsGetParameterCGtype( ptr->program, ptr->parameterEntry );
    if ( JS_LIKELY( parameterType == CG_FLOAT4x4 ) )
    {
        ptr->settercIndex( ptr, m, CG_GETINDEX( param ) );
    }
    else
    {
        /* pack matrix , use for all non float types as well */
        float packedmatrix[16];
        unsigned int rows = _jsGetTypeRowCount( parameterType );
        unsigned int cols = _jsGetTypeColCount( parameterType );
        for ( GLuint row = 0; row < rows; ++row )
            for ( GLuint j = 0; j < cols; ++j )
                packedmatrix[row*cols + j] = m[j][row];
        ptr->setterrIndex( ptr, packedmatrix, CG_GETINDEX( param ) );
    }
}

/**
@short Sets an array of values for a given matrix parameter

cgGLSetMatrixParameterArrayfc() sets the value of a given scalar or vector array parameter.  It
differs from cgGLSetMatrixParameterArrayfr() only in that the values are assumed to be in column
order.  Refer to cgGLSetMatrixParameterArrayfr() for more information.

@param       param   Specifies the array parameter that will be set.
@param       offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.
@param       nelements
               The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param       matrices       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the number
               of elements in the matrix.

@sa   cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterfr
 */
CGGL_API void cgGLSetMatrixParameterArrayfc( CGparameter param,
        long offset,
        long nelements,
        const float *matrices )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterArrayfc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetMatrixParameterArrayfc, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );

    CgRuntimeParameter* _ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif
    const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) ) )
    {
        JS_ASSERT( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) );
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
    }
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }
    //we have an array here, the parameterEntry of the type is the next one
    _ptr++;
    //get the type stride
    CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
    unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !melems ) )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return ;
    }
#endif
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        _ptr->settercIndex( _ptr, matrices + i * melems, i + offset );
    }
}
/**
@short Sets an array of values for a given matrix parameter

       The <c>cgGLSetMatrixParameterArray*</c> functions set the value of a given
       scalar or vector array parameter.  The functions are available in various combinations.

       There are versions of each function that assume the array of values are
       laid out in either row or column order signified by the <I>r</I> or <I>c</I> in the
       function name respectively.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

@par Function Prototype Variants
@code
void cgGLSetMatrixParameterArray{fd}{rc}(CGparameter param, long offset, long nelements, const type *v);
@endcode
@param       param   Specifies the array parameter that will be set.
@param       offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.
@param       nelements
               The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param       matrices       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the number
               of elements in the matrix.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_NOT_MATRIX_PARAM_ERROR</CODE></TD>
<TD>The elements of the array indicated by <I><c>param</c></I> are not matrix parameters.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The <I><c>offset</c></I> and/or the <I><c>nelements</c></I> parameter are out of the array bounds.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetMatrixParameterfr, cgGLGetMatrixParameterArrayfr

*/
CGGL_API void cgGLSetMatrixParameterArrayfr( CGparameter param,
        long offset,
        long nelements,
        const float *matrices )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterArrayfr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetMatrixParameterArrayfr, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );

    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif
    const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) ) )
    {
        JS_ASSERT( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) );
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
    }
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }
    //we have an array here, the parameterEntry of the type is the next one
    _ptr++;
    //get the type stride
    CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
    unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !melems ) )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return ;
    }
#endif
    // loop over array elements
    for ( int i = 0; i < nelements; ++i )
    {
        _ptr->setterrIndex( _ptr, matrices + i * melems, i + offset );
    }
}

/**
@ingroup CgDouble

@short Sets an array of values for a given matrix parameter (Not recommended for use)

cgGLSetMatrixParameterArraydc() sets the value of a given scalar or vector array parameter.  It
differs from cgGLSetMatrixParameterArrayfc() only in the argument type and in performance.

@param       param   Specifies the array parameter that will be set.
@param       offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.
@param       nelements
               The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param       matrices       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the number
               of elements in the matrix.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetMatrixParameterArrayfc()
instead.

@sa   cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterArrayfc, cgGLSetMatrixParameterfr
 */
CGGL_API void cgGLSetMatrixParameterArraydc( CGparameter param,
        long offset,
        long nelements,
        const double *matrices )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterArraydc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetMatrixParameterArraydc, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );

    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif
    const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) ) )
    {
        JS_ASSERT( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) );
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
    }
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    _ptr++;

    //get the type stride
    CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
    unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !melems ) )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return ;
    }
#endif

    // loop over array elements
    _JS_DPM_TRACE_OFF // set double array API
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLSetMatrixParameterdc( p, matrices + i*melems );
    }
    _JS_DPM_TRACE_ON
}

/**
@ingroup CgDouble

@short Sets an array of values for a given matrix parameter (Not recommended for use)

cgGLSetMatrixParameterArraydr() sets the value of a given scalar or vector array parameter.  It
differs from cgGLSetMatrixParameterArrayfr() only in the argument type and in performance.

@param       param   Specifies the array parameter that will be set.
@param       offset  An offset into the array parameter to start setting from.  A
               value of 0 will start setting from the first element of the
               array.
@param       nelements
               The number of elements to set.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param       matrices       The array of values to set the parameter to.  This must be a
               contiguous set of values that total <I><c>nelements</c></I> times the number
               of elements in the matrix.

@note For optimal PSGL performance, do not use this function.  Use cgGLSetMatrixParameterArrayfr()
instead.

@sa   cgGLGetMatrixParameterArrayfr, cgGLSetMatrixParameterArrayfr, cgGLSetMatrixParameterfr
 */
CGGL_API void cgGLSetMatrixParameterArraydr( CGparameter param,
        long offset,
        long nelements,
        const double *matrices )
{
    JS_APIENTRY_TIMER( cgGLSetMatrixParameterArraydr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLSetMatrixParameterArraydr, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( double ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );

    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( _ptr == NULL ) ) return;
#endif
    const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) ) )
    {
        JS_ASSERT( !( parameterEntry->flags & CGP_ARRAY ) && !( parameterEntry->flags & CGP_UNROLLED ) );
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
    }
#endif

    if ( nelements == 0 )
    {
        const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
        nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
    }

    //we have an array here, the parameterEntry of the type is the next one
    _ptr++;

    //get the type stride
    CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
    unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( !melems ) )
    {
        _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
        return ;
    }
#endif

    // loop over array elements
    _JS_DPM_TRACE_OFF // set double array API
    for ( int i = 0; i < nelements; ++i )
    {
        CGparameter p = cgGetArrayParameter( param, i + offset );
        cgGLSetMatrixParameterdr( p, matrices + i*melems );
    }
    _JS_DPM_TRACE_ON

}

/**
@short Retrieves an array of values for a given matrix array parameter

cgGLGetMatrixParameterArrayfc() retrieves an array of values
from a given matrix array parameter.  It differs from cgGLGetMatrixParameterArrayfr() only in that the
array of values is laid out in column order.  Refer to cgGLGetMatrixParameterArrayfr() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements  The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param matrices       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the number of elements in the matrix.

@sa   cgGLSetMatrixParameterArrayfr, cgGLGetMatrixParameterfr, cgGLGetParameter4f
*/
CGGL_API void cgGLGetMatrixParameterArrayfc( CGparameter param,
        long offset,
        long nelements,
        float *matrices )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterArrayfc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetMatrixParameterArrayfc, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );
    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }
        //we have an array here, the parameterEntry of the type is the next one
        _ptr++;

        parameterEntry = _ptr->parameterEntry;
        if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
                (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return;
        }

        int arrayID = ( int )_ptr->id;

        //get the type stride
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );
        if ( !melems )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return ;
        }
        // loop over array elements
        _JS_DPM_TRACE_OFF // get matrix array API
        for ( int i = 0; i < nelements; ++i )
        {
            int arrayParameter = arrayID | (( i + offset ) << CG_PARAMETERSIZE );
            cgGLGetMatrixParameterfc(( CGparameter )arrayParameter, matrices + i*melems );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@short Retrieves an array of values for a given matrix array parameter

       The <c>cgGLGetMatrixParameterArray*</c> functions retrieve an array of values
       from a given matrix array parameter.  The functions are available in
       various combinations.

       There are versions of each function that take either float or double
       values signified by the <I>f</I> or <I>d</I> in the function name.

       There are versions of each function that assume the array of values are
       laid out in either row or column order signified by the <I>r</I> or <I>c</I> in the
       function name respectively.

@par Function Prototype Variants
@code
void cgGLGetMatrixParameterArray{fd}{rc}(CGparameter param, long offset, long nelements, const type *v);
@endcode
@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements  The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param matrices       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the number of elements in the matrix.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_ARRAY_PARAM_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not an array parameter.</TD>
</TR>
<TR>
<TD><CODE>CG_NOT_MATRIX_PARAM_ERROR</CODE></TD>
<TD>The elements of the array indicated by <I><c>param</c></I> are not matrix parameters.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The <I><c>offset</c></I> and/or the <I><c>nelements</c></I> parameter are out of the array bounds.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD>The parameter fails to retrieve for any other reason.</TD>
</TR>
</TABLE>

@sa   cgGLGetParameter4f, cgGLSetMatrixParameterArrayfr, cgGLSetMatrixParameterfr
 */
CGGL_API void cgGLGetMatrixParameterArrayfr( CGparameter param,
        long offset,
        long nelements,
        float *matrices )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterArrayfr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetMatrixParameterArrayfr, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( GLfloat ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );
    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }
        //we have an array here, the parameterEntry of the type is the next one
        _ptr++;
        parameterEntry = _ptr->parameterEntry;
        if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
                (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return;
        }

        int arrayID = ( int )_ptr->id;

        //get the type stride
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );
        if ( !melems )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return ;
        }
        // loop over array elements
        _JS_DPM_TRACE_OFF // get matrix array API
        for ( int i = 0; i < nelements; ++i )
        {
            int arrayParameter = arrayID | (( i + offset ) << CG_PARAMETERSIZE );
            cgGLGetMatrixParameterfr(( CGparameter )arrayParameter, matrices + i*melems );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Retrieves an array of values for a given matrix array parameter (Not recommended for use)

cgGLGetMatrixParameterArraydc() retrieves an array of values
from a given matrix array parameter.  It differs from cgGLGetMatrixParameterArrayfc() only in the data type of
the arguments.  Refer to cgGLGetMatrixParameterArrayfc() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements  The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param matrices       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the number of elements in the matrix.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetMatrixParameterArrayfc() instead.

@sa   cgGLGetMatrixParameterArrayfc, cgGLSetMatrixParameterArrayfr, cgGLGetMatrixParameterfr, cgGLGetParameter4f
*/
CGGL_API void cgGLGetMatrixParameterArraydc( CGparameter param,
        long offset,
        long nelements,
        double *matrices )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterArraydc );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetMatrixParameterArraydc, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif
    JS_ASSERT( matrices != NULL );
    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        //we have an array here, the parameterEntry of the type is the next one
        _ptr++;

        parameterEntry = _ptr->parameterEntry;
        if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
                (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return;
        }

        int arrayID = ( int )_ptr->id;

        //get the type stride
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );
        if ( !melems )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return ;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // get matrix array API
        for ( int i = 0; i < nelements; ++i )
        {
            int arrayParameter = arrayID | (( i + offset ) << CG_PARAMETERSIZE );
            cgGLGetMatrixParameterdc(( CGparameter )arrayParameter, matrices + i*melems );
        }
        _JS_DPM_TRACE_ON
    }

}

/**
@ingroup CgDouble

@short Retrieves an array of values for a given matrix array parameter (Not recommended for use)

cgGLGetMatrixParameterArraydr() retrieves an array of values
from a given matrix array parameter.  It differs from cgGLGetMatrixParameterArrayfr() only in the data type of
the arguments.  Refer to cgGLGetMatrixParameterArrayfr() for more information.

@param param   Specifies the array parameter.
@param offset  An offset into the array parameter to start getting from.  A
               value of 0 will start getting from the first element of the
               array.
@param nelements  The number of elements to get.  A value of 0 will default to
               the number of elements in the array minus the <I><c>offset</c></I> value.
@param matrices       The array to retrieve the values into.  The size of the array must
               be <I><c>nelements</c></I> times the number of elements in the matrix.

@note For optimal PSGL performance, do not use this function.  Use cgGLGetMatrixParameterArrayfr() instead.

@sa   cgGLSetMatrixParameterArrayfr, cgGLGetMatrixParameterfr, cgGLGetParameter4f

*/
CGGL_API void cgGLGetMatrixParameterArraydr( CGparameter param,
        long offset,
        long nelements,
        double *matrices )
{
    JS_APIENTRY_TIMER( cgGLGetMatrixParameterArraydr );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_4( cgGLGetMatrixParameterArraydr, JSDPM_CALL_TYPE_STATE_CG, , 4,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             nelements, sizeof( long ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             matrices, nelements*16*sizeof( double ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }
#endif

    JS_ASSERT( matrices != NULL );
    CgRuntimeParameter *_ptr = _cgGLTestArrayParameter( param, offset, nelements );
    if ( JS_LIKELY( _ptr != NULL ) )
    {
        const CgParameterEntry *parameterEntry = _ptr->parameterEntry;
        if ( nelements == 0 )
        {
            const CgParameterArray *parameterArray = _jsGetParameterArray( _ptr->program, parameterEntry );
            nelements = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount ) - offset;
        }

        //we have an array here, the parameterEntry of the type is the next one
        _ptr++;

        parameterEntry = _ptr->parameterEntry;
        if (( parameterEntry->flags & CGP_TYPE_MASK ) != CGP_INTRINSIC ||
                (( parameterEntry->flags & CGPV_MASK ) != CGPV_UNIFORM && ( parameterEntry->flags & CGPV_MASK ) != CGPV_CONSTANT ) )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return;
        }

        int arrayID = ( int )_ptr->id;

        //get the type stride
        CGtype parameterType = _jsGetParameterCGtype( _ptr->program, _ptr->parameterEntry );
        unsigned int melems = _jsGetTypeRowCount( parameterType ) * _jsGetTypeColCount( parameterType );
        if ( !melems )
        {
            _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
            return ;
        }

        // loop over array elements
        _JS_DPM_TRACE_OFF // get matrix array API
        for ( int i = 0; i < nelements; ++i )
        {
            int arrayParameter = arrayID | (( i + offset ) << CG_PARAMETERSIZE );
            cgGLGetMatrixParameterdr(( CGparameter )arrayParameter, matrices + i*melems );
        }
        _JS_DPM_TRACE_ON
    }

}

/******************************************************************************
 *** Texture Parameter Management Functions
 *****************************************************************************/
/**
@short Sets the value of texture parameters

cgGLSetTextureParameter() sets the value of a given texture parameter to
a OpenGL texture object.

Note that cgGLEnableTextureParameter() must be called after cgGLSetTextureParameter()
and before the geometry is drawn in order to use the texture.

@param       param   Specifies the parameter that will be set.
@param       texobj  An OpenGL texture object name.  This is the value the parameter
               will be set to.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLGetTextureParameter, cgGLSetParameter4f
 */
CGGL_API void cgGLSetTextureParameter( CGparameter param, GLuint texobj )
{
    JS_APIENTRY_TIMER( cgGLSetTextureParameter );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetTextureParameter, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             texobj, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    // According to the cg implementation from nvidia, set just stores the obj.
    // Enable does the actual work.
    CgRuntimeParameter* ptr = _cgGLTestTextureParameter( param );
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif

    ptr->samplerSetter( ptr, &texobj, 0 );
}



/*

CGGL_API void CGGLENTRY cgGLSetupSampler(CGparameter param, GLuint texobj)
{
	return;
}

*/


/**
@short Retrieves the value of texture parameters

cgGLGetTextureParameter() retrieves the value of a texture parameter.

@param       param   Specifies the texture parameter.

@return Returns the name of the OpenGL object that the texture was set to.
If the parameter has not been set, 0 will be returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to get for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetTextureParameter, cgGLGetParameter4f
 */
CGGL_API GLuint cgGLGetTextureParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLGetTextureParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLGetTextureParameter, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    CgRuntimeParameter* ptr = _cgGLTestTextureParameter( param );
    if ( ptr == NULL ) return 0;
if ( !( ptr->parameterEntry->flags & CGPF_REFERENCED ) ) { _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR ); return 0; }
    return *( GLuint* )ptr->pushBufferPointer;
    return 0;
}

/**
@short Enables the texture unit associated with the given texture parameter

       cgGLEnableTextureParameter() binds and enables the texture object that
       was associated with the parameter <I><c>param</c></I>.  It must be called after
       cgGLSetTextureParameter() is called but before the geometry is
       drawn.

       cgGLDisableTextureParameter() should be called once all of the geometry
       is drawn to avoid applying the texture to the wrong geometry and
       shaders.

@param       param   Specifies the texture parameter that has a texture object associated with it.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to get for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLDisableTextureParameter, cgGLSetTextureParameter
 */
CGGL_API void cgGLEnableTextureParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLEnableTextureParameter );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLEnableTextureParameter, JSDPM_CALL_TYPE_STATE_CG, , 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    CgRuntimeParameter* ptr = _cgGLTestTextureParameter( param );
    // do not use TestTexture, as the setSampler is set accordingly
#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
#endif
    ptr->samplerSetter( ptr, NULL, 0 );
}

/**
@short Disables the texture unit associated with the given texture parameter

       cgGLDisableTextureParameter() unbinds and disables the texture object
       that was associated with the parameter <I><c>param</c></I>.

       See the cgGLEnableTextureParameter() manpage for more information.

@param       param   Specifies the texture parameter that has a texture object associated with it.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROFILE_ERROR</CODE></TD>
<TD><I><c>param</c></I>'s profile is not a supported OpenGL profile.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to get for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLEnableTextureParameter, cgGLSetTextureParameter
 */
CGGL_API void cgGLDisableTextureParameter( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLDisableTextureParameter );
    if ( _cgGLTestTextureParameter( param ) )
    {
        // this does not do anything on nvidia's implementation
    }
}

/**
@short Returns the GL enumerant for the texture unit associated with a texture parameter

cgGLGetTextureEnum() returns the OpenGL enumerant for the texture unit assigned
to <I><c>param</c></I>. The enumerant has the form <CODE>GL_TEXTURE<i>n</i>_ARB</CODE>
where <i>n</i> is the texture unit number.

@param  param   Specifies the texture parameter.

@return Returns  a <CODE>GLenum</CODE> of the form <CODE>GL_TEXTURE<i>n</i>_ARB</CODE>.
If <I><c>param</c></I> is not a texture parameter <CODE>GL_INVALID_OPERATION</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetTextureParameter
 */
CGGL_API GLenum cgGLGetTextureEnum( CGparameter param )
{
    JS_APIENTRY_TIMER( cgGLGetTextureEnum );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLGetTextureEnum, JSDPM_CALL_TYPE_STATE_CG, 0, 1, param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // The returned value is the texture unit assigned to this parameter
    // by the Cg compiler.
    CgRuntimeParameter* ptr = _cgGLTestTextureParameter( param );
    if ( ptr == NULL ) return GL_INVALID_OPERATION;

    if ( ptr->parameterEntry->flags & CGP_RTCREATED )
    {
        // runtime created texture parameters do not have allocated texture units
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return GL_INVALID_OPERATION;
    }

    // XXX  what about the vertex texture enums !?
    if (( ptr->program->header.profile == CG_PROFILE_SCE_VP_TYPEB )
            //|| (ptr->program->header.profile==CG_PROFILE_SCE_VP_TYPEC)
            || ( ptr->program->header.profile == CG_PROFILE_SCE_VP_RSX ) )
    {
        return GL_INVALID_OPERATION;
    }

    if ( !( ptr->parameterEntry->flags & CGPF_REFERENCED ) || !(( ptr->parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM ) ) { _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR ); return GL_INVALID_OPERATION; }
    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    JS_ASSERT( parameterResource->resource >= CG_TEXUNIT0 && parameterResource->resource <  CG_TEXUNIT0 + _JS_MAX_TEXTURE_IMAGE_UNITS );
    return GL_TEXTURE0 + parameterResource->resource - CG_TEXUNIT0;
}

/**
@short Sets the manage texture parameters flag for a Cg context

       By default, cgGL does not manage any texture state in OpenGL.  It is up
       to the user to enable and disable textures with cgGLEnableTextureParameter() and
       cgGLDisableTextureParameter() respectively.  This behavior is the default in order
       to avoid conflicts with texture state on geometry that's rendered with
       the fixed function pipeline or without cgGL.

       If automatic texture management is desired, cgGLSetManageTextureParameters()
	   may be called with flag set to <CODE>CG_TRUE</CODE> before cgGLBindProgram() is
       called.  To reset the texture state cgGLUnbindProgram() may be used.

       Calling cgGLSetManageTextureParameters() with flag set to <CODE>CG_FALSE</CODE> will
       disable automatic texture management.

@param       ctx     Specifies the context.
@param       flag    The flag to set the manage textures flag to.

@sa       cgGLGetManageTextureParameters, cgGLBindProgram, cgGLUnbindProgram
 */
CGGL_API void cgGLSetManageTextureParameters( CGcontext ctx, CGbool flag )
{
    JS_APIENTRY_TIMER( cgGLSetManageTextureParameters );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetManageTextureParameters, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             flag, sizeof( CGbool ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( !cgIsContext( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return;
    }
#endif
    _cgGetContextPtr( ctx )->GLmanageTextures = flag;
}

/**
@short Gets the manage texture parameters flag from a context

Returns the manage texture management flag.
See cgGLSetManageTextureParameters() for more information.

@param  ctx     Specifies the context.

@return Returns the manage texture management flag in the context <I><c>ctx</c></I>.

@sa       cgGLSetManageTextureParameters, cgGLBindProgram, cgGLUnbindProgram
 */
CGGL_API CGbool cgGLGetManageTextureParameters( CGcontext ctx )
{
    JS_APIENTRY_TIMER( cgGLGetManageTextureParameters );
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGLGetManageTextureParameters, JSDPM_CALL_TYPE_STATE_CG, 0, 1, ctx, sizeof( CGcontext ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( !cgIsContext( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return CG_FALSE;
    }
#endif
    return _cgGetContextPtr( ctx )->GLmanageTextures;
}

/**
@short Sets the value of a vertex program uniform register

The cgGLSetVertexRegister4fv() function sets the value of a given uniform
register for vertex programs. A uniform register consists of 4 floats, and is
referenced by index. Uniforms in vertex programs can be associated with a given
register by binding it to one of the <i>Cxxx</i> semantics, where <i>xxx</i> is an index
from 0 to 255.

@param		index	The index of the register to set
@param		v		A pointer to the 4 float values to load into the register

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not within the 0-255 range.</TD>
</TR>
</TABLE>

@sa cgGLSetParameter4fv, cgGLSetVertexRegisterBlock, cgGLSetFragmentRegister4fv
 */
CGGL_API void cgGLSetVertexRegister4fv( unsigned int index, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetVertexRegister4fv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetVertexRegister4fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             index, sizeof( index ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 4*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( index >= JS_MAX_VP_SHARED_CONSTANTS )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return;
    }
#endif
    _jsPlatformSetVertexRegister4fv( index, v );
}

/**
@short Sets the values of a block of vertex program uniform registers

The cgGLSetVertexRegisterBlock() function sets the value of a given block of
uniform register for vertex programs. The block is defined by the index of the
first register, and a register count.

A uniform register consists of 4 floats, and is referenced by index. Uniforms
in vertex programs can be associated with a given register by binding it to
one of the <i>Cxxx</i> semantics, where <i>xxx</i> is an index from 0 to 255.

@param		index	The index of the first register to set
@param		count	The number of the registers to set
@param		v		A pointer to the block of 4 float values to load into the
					register block. This must point to a 4 &times; <I><c>count</c></I> block of floats

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The range <I><c>index</c></I> - <I><c>index</c></I>+<I><c>count</c></I>-1 is not included within the 0-255 range.</TD>
</TR>
</TABLE>

@sa cgGLSetParameterArray4f, cgGLSetVertexRegister4fv, cgGLSetFragmentRegisterBlock
 */
CGGL_API void cgGLSetVertexRegisterBlock( unsigned int index, unsigned int count, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetVertexRegisterBlock );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetVertexRegisterBlock, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             index, sizeof( index ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             count, sizeof( count ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, count*16, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if (( index >= JS_MAX_VP_SHARED_CONSTANTS ) || ( index + count > JS_MAX_VP_SHARED_CONSTANTS ) )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return;
    }
#endif
    _jsPlatformSetVertexRegisterBlock( index, count, v );
}

/**
@short Sets the value of a fragment program uniform register

The cgGLSetFragmentRegister4fv() function sets the value of a given uniform
register for fragment programs. A uniform register consists of 4 floats, and is
referenced by index. Uniforms in fragment programs can be associated with a given
register by binding it to one of the <i>Cxxx</i> semantics, where <i>xxx</i> is an index
from 0 to 1023.

@param		index	The index of the register to set
@param		v		A pointer to the 4 float values to load into the register

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is not within the 0-1023 range.</TD>
</TR>
</TABLE>

@sa cgGLSetParameter4fv, cgGLSetFragmentRegisterBlock, cgGLSetVertexRegister4fv
 */
CGGL_API void cgGLSetFragmentRegister4fv( unsigned int index, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetFragmentRegister4fv );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetFragmentRegister4fv, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             index, sizeof( index ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, 4*sizeof( float ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if ( index >= JS_MAX_FP_SHARED_CONSTANTS )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return;
    }
#endif
    _jsPlatformSetFragmentRegister4fv( index, v );
}

/**
@short Sets the values of a block of fragment program uniform registers

The cgGLSetFragmentRegisterBlock() function sets the value of a given block of
uniform register for fragment programs. The block is defined by the index of the
first register, and a register count.

A uniform register consists of 4 floats, and is referenced by index. Uniforms
in fragment programs can be associated with a given register by binding it to
one of the <i>Cxxx</i> semantics, where <i>xxx</i> is an index from 0 to 1023.

@param		index	The index of the first register to set
@param		count	The number of the registers to set
@param		v		A pointer to the block of 4 float values to load into the
					register block. This must point to a 4 &times; <I><c>count</c></I> block of floats

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The range <I><c>index</c></I> - <I><c>index</c></I>+<I><c>count</c></I>-1 is not included within the 0-1023 range.</TD>
</TR>
</TABLE>

@sa cgGLSetParameterArray4f, cgGLSetFragmentRegister4fv, cgGLSetVertexRegisterBlock
 */
CGGL_API void cgGLSetFragmentRegisterBlock( unsigned int index, unsigned int count, const float *v )
{
    JS_APIENTRY_TIMER( cgGLSetFragmentRegisterBlock );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetFragmentRegisterBlock, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             index, sizeof( index ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             count, sizeof( count ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, count*16, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    if (( index >= JS_MAX_FP_SHARED_CONSTANTS ) || ( index + count > JS_MAX_FP_SHARED_CONSTANTS ) )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return;
    }
#endif
    _jsPlatformSetFragmentRegisterBlock( index, count, v );
}

/**
@short Sets the values of a boolean parameter

The boolean register consists of 32 bits, referenced by index.
Uniforms in vertex programs can be associated with a given register by binding it
to one of the <i>Bxx</i> semantics, where <i>xx</i> is an index from 0 to 31.

If the parameter is not shared, the value is applied when the parameter's parent program is bound.
If it is shared, the value is stored in the context and will be pushed immediately
when there is a currently bound program.

Note that parameter connections to boolean uniforms for static
control flow are not yet supported.

@param		param	The handle for the parameter
@param		v	The boolean value with which to set the register

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a valid parameter handle.</TD>
</TR>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>param</c></I> is not a texture parameter or if the parameter fails to set for any other reason.</TD>
</TR>
</TABLE>

@sa cgGLSetProgramBoolVertexRegisters, cgGLSetProgramBoolVertexRegistersBlock, cgGLSetBoolVertexRegisters, cgGLSetBoolVertexRegistersSharingMask
 */
void cgGLSetParameter1b( CGparameter param, CGbool v )
{
    JS_APIENTRY_TIMER( cgGLSetParameter1b );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGLSetParameter1b, JSDPM_CALL_TYPE_STATE_CG, , 2,
                             param, sizeof( CGparameter ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, sizeof( bool ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    CgRuntimeParameter* ptr = _jsCgGLTestParameter( param );
    if ( !ptr->parameterEntry->flags & CGPF_REFERENCED ) //ignore
        return;

    _CGprogram* program = ptr->program;

#ifndef JS_NO_ERROR_CHECK
    if ( JS_UNLIKELY( ptr == NULL ) ) return;
    if ( !( ptr->parameterEntry->flags & CGPV_UNIFORM ) )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
#ifndef JS_NO_ERROR_CHECK
    if ( parameterResource->type != CGP_SCF_BOOL )
    {
        _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR );
        return;
    }
#endif

    // not sure what to do if no program like for global effect params that can be connected
    // maybe actually do the propagation?!
    if ( program )
    {
        unsigned short resource = parameterResource->resource;
        JS_ASSERT( parameterResource->resource != 0xffff );
        JS_ASSERT( resource < JS_BOOLEAN_REGISTERS_COUNT );
        JS_ASSERT( program->parentContext );

        unsigned int isShared = ( 1 << resource ) & ( program->parentContext->controlFlowBoolsSharedMask );
        unsigned int mask = ( 1 << resource );

        // if the parameter is shared, initialize bools from context, otherwise from program
        unsigned int bools = isShared ? program->parentContext->controlFlowBoolsShared : program->controlFlowBools;

        // set or unset the appropriate bit
        if ( v )
        {
            bools |= mask;
        }
        else
        {
            mask = ~mask;
            bools &= mask;
        }

        if ( isShared )
        {
            program->parentContext->controlFlowBoolsShared = bools;
        }
        else
        {
            program->controlFlowBools = bools;
        }

        _CurrentContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
    }
}

/**
@short Sets the values of non-shared boolean registers

cgGLSetProgramBoolVertexRegisters() sets the non-shared booleans in the programs register and applies
them when the specified program is bound.  The values persist after the program
is unbound.  The shared values are ignored, as this call only affects the
values for the specified program.

@param		prog	The handle for the program
@param		values	The values for boolean registers

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> is not a valid program.</TD>
</TR>
</TABLE>

@note
The boolean register consists of 32 bits, referenced by index.
Uniforms in vertex programs can be associated with a given register by binding it
to one of the <i>Bxx</i> semantics, where <i>xx</i> is an index from 0 to 31.

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@sa cgGLSetParameter1b, cgGLSetProgramBoolVertexRegistersBlock, cgGLSetBoolVertexRegisters, cgGLSetBoolVertexRegistersSharingMask
 */
void cgGLSetProgramBoolVertexRegisters( CGprogram prog, unsigned int values )
{
    JS_APIENTRY_TIMER( cgGLSetProgramBoolVertexRegisters );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGLSetBoolVertexRegisters, JSDPM_CALL_TYPE_STATE_CG, , 1,
                             values, sizeof( values ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    // check the program id
    if ( JS_UNLIKELY( !CG_IS_PROGRAM( prog ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
#endif
    _CGprogram* ptr = _cgGetProgPtr( prog );

    JS_ASSERT( ptr );
    JS_ASSERT( ptr->parentContext );

    // use the shared mask to avoid overwriting the values of shared registers
    unsigned int mask = ~( ptr->parentContext->controlFlowBoolsSharedMask );
    // these will get pushed to the gpu when the program is bound
    ptr->controlFlowBools = values & mask;

    _CurrentContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}

/**
@short Sets the value of a block of boolean registers

cgGLSetProgramBoolVertexRegistersBlock() sets the value of a given block of
boolean registers. The block is defined by the index of the first register and a register count.

The specified values are applied when the specified program is bound.  The values persist after the
program is unbound.  The shared values are ignored as this call only affects the
values of the program.

@param		prog	The handle for the program
@param		index	The number of the first register to set
@param		count	The number of the registers to set
@param		v		A pointer to the block of <CODE>bool</CODE> values to load into the registers

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>prog</c></I> is not a valid program.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD>The range <I><c>index</c></I> - <I><c>index</c></I>+<I><c>count</c></I>-1 is not included within the 0-31 range.</TD>
</TR>
</TABLE>

@note
The boolean register consists of 32 bits, referenced by index.
Uniforms in vertex programs can be associated with a given register by binding it
to one of the <i>Bxx</i> semantics, where <i>xx</i> is an index from 0 to 31.

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@sa cgGLSetParameter1b, cgGLSetProgramBoolVertexRegisters, cgGLSetBoolVertexRegisters, cgGLSetBoolVertexRegistersSharingMask
 */
void cgGLSetProgramBoolVertexRegistersBlock( CGprogram prog, unsigned int index, unsigned int count, const CGbool *v )
{
    JS_APIENTRY_TIMER( cgGLSetProgramBoolVertexRegistersBlock );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGLSetProgramBoolVertexRegistersBlock, JSDPM_CALL_TYPE_STATE_CG, , 3,
                             index, sizeof( index ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             count, sizeof( count ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             v, count*sizeof( bool ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
#ifndef JS_NO_ERROR_CHECK
    // check the program id
    if ( JS_UNLIKELY( !CG_IS_PROGRAM( prog ) ) )
    {
        _jsCgRaiseError( CG_INVALID_PROGRAM_HANDLE_ERROR );
        return;
    }
    if (( index >= JS_BOOLEAN_REGISTERS_COUNT ) || ( index + count > JS_BOOLEAN_REGISTERS_COUNT ) )
    {
        _jsCgRaiseError( CG_OUT_OF_ARRAY_BOUNDS_ERROR );
        return;
    }
#endif
    _CGprogram* ptr = _cgGetProgPtr( prog );
    JS_ASSERT( ptr );
    JS_ASSERT( ptr->parentContext );

    unsigned int bools = ptr->controlFlowBools;
    unsigned int mask = 0;

    // initialize mask with bool values
    for ( unsigned int i = 0; i < count; i++ )
    {
        mask |= ( v[i] << index + i );
    }

    // use the shared mask to avoid overwriting the values of shared registers
    // shared values currently get ignored, maybe they should be saved to context
    mask &= ~( ptr->parentContext->controlFlowBoolsSharedMask );

    // set or unset the appropriate bit
    bools = ~bools;
    bools ^= mask;
    bools = ~bools;

    ptr->controlFlowBools = bools;

    _CurrentContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}

/**
@short Sets the values for all boolean registers

cgGLSetBoolVertexRegisters() sets the registers directly, applying them to the currently bound program.
The non-shared values are stored in the program so that they persist after the program
is unbound.  The shared values are stored in the context of the bound program.

If there is no currently bound program, the register is still set and no error is
raised.  However, the values will be overwritten the next time a program is bound.

@param		values	The values for all 32 boolean registers

@par Errors

@note
The boolean register consists of 32 bits, referenced by index.
Uniforms in vertex programs can be associated with a given register by binding it
to one of the <i>Bxx</i> semantics, where <i>xx</i> is an index from 0 to 31.

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@sa cgGLSetParameter1b, cgGLSetProgramBoolVertexRegisters, cgGLSetProgramBoolVertexRegistersBlock, cgGLSetBoolVertexRegistersSharingMask
 */
void cgGLSetBoolVertexRegisters( unsigned int values )
{
    JS_APIENTRY_TIMER( cgGLSetBoolVertexRegisters );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGLSetBoolVertexRegisters, JSDPM_CALL_TYPE_STATE_CG, , 1,
                             values, sizeof( values ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif

    // keep the shared values in the context and the unshared ones in the current program
    if ( _CurrentContext->BoundVertexProgram )
    {
        JS_ASSERT( _CurrentContext->BoundVertexProgram->parentContext );

        _CurrentContext->BoundVertexProgram->controlFlowBools = values & ~( _CurrentContext->BoundVertexProgram->parentContext->controlFlowBoolsSharedMask );
        _CurrentContext->BoundVertexProgram->parentContext->controlFlowBoolsShared = values & _CurrentContext->BoundVertexProgram->parentContext->controlFlowBoolsSharedMask;
    }

    _jsPlatformSetBoolVertexRegisters( values );
}

/**
@short Sets the shared register bit mask

cgGLSetBoolVertexRegistersSharingMask() sets the bit mask that is used to determine which boolean parameters are shared
by the programs of this context.

@param		ctx	The handle of the context
@param		mask	Bit mask - true specifies a shared <CODE>bool</CODE> register

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_CONTEXT_HANDLE_ERROR</CODE></TD>
<TD>The context handle specified is not a valid cg context.
</TR>
</TABLE>

@note
By default, static control flow is enabled for uniform boolean variables with
explicit bindings.  To try to bind all uniform boolean variables for static
control flow, you must compile your Cg program with the <c>-scf all</c> option.

@sa cgGLSetParameter1b, cgGLSetProgramBoolVertexRegisters, cgGLSetProgramBoolVertexRegistersBlock, cgGLSetBoolVertexRegisters
 */
void cgGLSetBoolVertexRegistersSharingMask( CGcontext ctx, unsigned int mask )
{
    JS_APIENTRY_TIMER( cgGLSetBoolVertexRegistersSharingMask );
#ifndef JS_NO_ERROR_CHECK
    if ( !cgIsContext( ctx ) )
    {
        _jsCgRaiseError( CG_INVALID_CONTEXT_HANDLE_ERROR );
        return;
    }
#endif
    _CGcontext* context = _cgGetContextPtr( ctx );
    context->controlFlowBoolsSharedMask = mask;
}


/******************************************************************************
 *** Misc Functions
 *****************************************************************************/


/**
@short Toggles debug mode for GL operations (Not implemented)

cgGLSetDebugMode() is not supported under PSGL.

cgGLSetDebugMode() toggles debug mode for GL operations

@param		debug	CG_TRUE or CG_FALSE
 */
CGGL_API void cgGLSetDebugMode( CGbool debug )
{
    // TODO: *****************88
    return;
}



/**
@short Get the number of registers to be used in the shader program

cgGLGetRegisterCount() gets the number of registers to be used in the shader program

@param		program	the shader program

@return  Returns the number of registers to be used in <i><c>program</c></i>

@sa cgGLSetRegisterCount
*/
unsigned int cgGLGetRegisterCount( CGprogram program )
{
    JS_CHECK_PROGRAM_RET( 0 );
    _CGprogram* ptr = _cgGetProgPtr( program );
    return _jsGetProgramProfileIndex(( CGprofile )ptr->header.profile ) == FRAGMENT_PROFILE_INDEX ?
           ptr->header.fragmentProgram.registerCount : ptr->header.vertexProgram.registerCount;
}


/**
@short Set the number of register to be used in the shader program

cgGLSetRegisterCount() sets the number of registers to be used in the shader program.

@note
Use cgGLGetRegisterCount() to retrieve the original register count.  Do not set the number of
registers to be below the original register count.

@param		program	the shader program
@param		regCount the register count

@sa cgGLGetRegisterCount
*/
void cgGLSetRegisterCount( CGprogram program, const unsigned int regCount )
{
    JS_CHECK_PROGRAM;
    _CGprogram* ptr = _cgGetProgPtr( program );
    if ( _jsGetProgramProfileIndex(( CGprofile )ptr->header.profile ) == FRAGMENT_PROFILE_INDEX )
    {
        ptr->header.fragmentProgram.registerCount = regCount;
        if ( _CurrentContext->BoundFragmentProgram == program )
            _CurrentContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM;
    }
    // don't do anything for vertex
    /*else if (_jsGetProgramProfileIndex(ptr->header->profile) == VERTEX_PROFILE_INDEX)
    	ptr->header->vertexProgram->registerCount;*/
}

/**
 * @}  CgGL
 */
