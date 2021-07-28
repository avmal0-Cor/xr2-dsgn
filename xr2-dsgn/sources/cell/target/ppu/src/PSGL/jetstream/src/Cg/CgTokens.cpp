/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION: Constant tables and Cg Runtime functions for type, resource, enum, profile and error functions.

#define CG_LIB
#include <Cg/cg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PSGL/Types.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>

#include "Cg/CgCommon.h"

// structs ------------------------------

typedef struct JScgProfileMapType
{
    CGprofile id;
    char* string;
    int is_vertex_program;
}
JScgProfileMapType;

typedef struct JScgTypeMapType
{
    CGtype id;
    char* string;
    CGparameterclass parameter_class;
}
JScgTypeMapType;

// string tables ------------------------
// string tables map enum values to printable strings

static const JScgTypeMapType JScgTypeMap[] =
    {
        { CG_UNKNOWN_TYPE, "unknown", CG_PARAMETERCLASS_UNKNOWN},
        { CG_STRUCT, "struct", CG_PARAMETERCLASS_STRUCT},
        { CG_ARRAY, "array", CG_PARAMETERCLASS_ARRAY},

        //NOTE: string is compiler friendly lower-case version of type name
#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
    { enum_name, #compiler_name, classname },
#include <Cg/cg_datatypes.h>

        // note: assumes CG_UNKNOWN_TYPE == 0
        { CGtype( 0 ), "", CG_PARAMETERCLASS_UNKNOWN }
    };

static const JSenumMap JScgResourceMap[] =
    {

#define CG_BINDLOCATION_MACRO(name,enum_name,compiler_name,enum_int,addressable,param_type) \
    { enum_name, compiler_name },
#include <Cg/cg_bindlocations.h>

        { CG_UNDEFINED, "undefined" },
        { 0, "" }
    };

static const JSenumMap JScgEnumMap[] =
    {

#define CG_ENUM_MACRO(enum_name, enum_val) \
    { enum_val, #enum_name },
#include <Cg/cg_enums.h>

        { 0, "" }
    };

static const JScgProfileMapType JScgProfileMap[] =
    {
        {( CGprofile )6144, "CG_PROFILE_START", 1 },
        {( CGprofile )6145, "unknown", 1 },

#define CG_PROFILE_MACRO(name, compiler_id, compiler_id_caps, compiler_opt,int_id,vertex_profile) \
    {CG_PROFILE_ ## compiler_id_caps, compiler_opt, vertex_profile},
        //#include <Cg/cgJS_profiles.h>
#include <Cg/cg_profiles.h>

        {( CGprofile )0, "", 0 }
    };

static const JSenumMap JScgErrorMap[] =
    {

#define CG_ERROR_MACRO(code, enum_name, message) \
    { enum_name, message },
#include <Cg/cg_errors.h>

        { 0, "" }
    };




// templated map lookup functions  ---------


template<typename T, size_t N>
const T*_jsLookupEnum( const T( &map )[N], const int key )
{
    const unsigned int count = sizeof( map ) / sizeof( map[0] );
    for ( unsigned int i = 0; i < count; ++i )
    {
        if ( map[i].id == key )
        {
            return map + i;
        }
    }
    return 0;
}


template<typename T, size_t N>
const T* _jsLookupString( const T( &map )[N], const char *key )
{
    const unsigned int count = sizeof( map ) / sizeof( map[0] );
    if ( NULL != key )
    {
        for ( unsigned int i = 0; i < count; ++i )
        {
            if ( 0 == strcmp( map[i].string, key ) )
            {
                return map + i;
            }
        }
    }
    return 0;
}






// API type functions -----------------------
/** @addtogroup Cg
 *@{
 */

/**  @short  Get the type name associated with a type enumerant

cgGetTypeString() returns the type name associated with a type enumerant.

@param       type    The type enumerant.

@return Returns the type string of the enumerant <I><c>type</c></I>.

@sa cgGetType, cgGetParameterType
 */
CG_API const char* cgGetTypeString( CGtype type )
{
    JS_APIENTRY_TIMER( cgGetTypeString );
    const JScgTypeMapType *ptr = _jsLookupEnum( JScgTypeMap, type );
    if ( 0 != ptr )
    {
        return const_cast<const char*>( ptr->string );
    }
    return "";
}

/**  @short  Get the type enumerant assigned to a type name

cgGetType() returns the enumerant assigned to a type name.
The following is an example of how cgGetType() might be used.
@code
CGtype Float4Type = cgGetType("float4");
if (cgGetParameterType(myparam) == Float4Type)
   {
      ...
   }
@endcode

@param type_string A string containing the type name.  The name is case-sensitive.

@return Returns the type enumerant of <i><c>type_string</c></i>.  If no such type exists
<CODE>CG_UNKNOWN_TYPE</CODE> will be returned.

@sa cgGetTypeString, cgGetParameterType
*/
CG_API CGtype cgGetType( const char* type_string )
{
    JS_APIENTRY_TIMER( cgGetType );
    const JScgTypeMapType *ptr = _jsLookupString( JScgTypeMap, type_string );
    if ( 0 != ptr )
    {
        return static_cast<CGtype>( ptr->id );
    }
    return CG_UNKNOWN_TYPE;
}


// User type functions ----------------------

/**
@short  Get the type enumerant assigned to a user type name

cgGetNamedUserType() returns the enumerant assigned to a user defined
type name in <I><c>handle</c></I>.

@param       handle The handle the type is defined in.

@param       name    A string containing the type name.  The name is case-sensitive.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns the type with the named <I><c>name</c></I>.  If no such type exists
<CODE>CG_UNKNOWN_TYPE</CODE> will be returned.

@sa       cgGetUserType, cgGetType
*/
CG_API CGtype cgGetNamedUserType( CGhandle handle, const char* name )
{
    JS_APIENTRY_TIMER( cgGetNamedUserType );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetNamedUserType, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             program, sizeof( CGhandle ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             name, strlen( name ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    // TODO **************
    return CG_UNKNOWN_TYPE;
}

/**
@short  Gets the number of user types of a program

cgGetNumUserTypes() returns the number of user defined types in the handle <I><c>handle</c></I>.

@param handle The handle the types are defined in.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns the number of user defined types in <I><c>program</c></I>.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is invalid.</TD>
</TR>
</TABLE>

@sa       cgGetUserType, cgGetNamedUserType
*/
CG_API int cgGetNumUserTypes( CGhandle handle )
{
    JS_APIENTRY_TIMER( cgGetNumUserTypes );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNumUserTypes, JSDPM_CALL_TYPE_STATE_CG, 0, 1, program, sizeof( CGhandle ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // TODO **************
    return 0;
}

/**
@short  Gets a user defined type from a handle

cgGetUserType() returns the user defined type with the index <I><c>index</c></I> in <I><c>handle</c></I>.

@param       handle The handle the type is defined in.
@param       index   The index of the user defined type.  <I><c>index</c></I> must be greater than
or equal to 0 and less than N, where N is the value returned by
cgGetNumUserTypes().

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns the type if the call is successful.  <CODE>CG_UNKNOWN_TYPE</CODE> if
the returned type is a built-in type or an error is thrown.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PROGRAM_HANDLE_ERROR</CODE></TD>
<TD><I><c>program</c></I> is invalid.</TD>
</TR>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is outside the proper range.</TD>
</TR>
</TABLE>

@sa       cgGetNumUserTypes, cgGetNamedUserType
 */
CG_API CGtype cgGetUserType( CGhandle handle, int index )
{
    JS_APIENTRY_TIMER( cgGetUserType );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetUserType, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             program, sizeof( CGhandle ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             index, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    // TODO **************
    return CG_UNKNOWN_TYPE;
}

/**
@short  Gets the number of parent types of a given type

cgGetNumParentTypes() returns the number of parents the child type <I><c>type</c></I>
inherits from.

@param       type    Specifies the child type.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns the number of parent types.  0 is returned if there are no parents.

@sa       cgGetParentType
*/
CG_API int cgGetNumParentTypes( CGtype type )
{
    JS_APIENTRY_TIMER( cgGetNumParentTypes );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetNumParentTypes, JSDPM_CALL_TYPE_STATE_CG, 0, 1, type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    // TODO **************
    return 0;
}

/**
@short  Gets a parent type of a child type

cgGetParentType() returns a parent type of <I><c>type</c></I>.

@param       type    Specifies the child type.

@param       index   The index of the parent type.  <I><c>index</c></I> must be greater than or
equal to 0 and less than N where N is the value returned by
cgGetNumParentTypes().

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns the parent type of the Nth position, where N is indicated by <I><c>index</c></I>.
Returns <CODE>CG_UNKNOWN_TYPE</CODE> if <I><c>type</c></I> is a built-in type or an error is thrown.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_OUT_OF_ARRAY_BOUNDS_ERROR</CODE></TD>
<TD><I><c>index</c></I> is outside the
proper range.</TD>
</TR>
</TABLE>

@sa       cgGetNumParentTypes
*/
CG_API CGtype cgGetParentType( CGtype type, int index )
{
    JS_APIENTRY_TIMER( cgGetParentType );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgGetParentType, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             index, sizeof( int ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    // TODO **************
    return CG_UNKNOWN_TYPE;
}

/**
@short  Determine if a type is a parent of another type

cgIsParentType() returns a <CODE>CGbool</CODE> indicating if <I><c>parent</c></I> is a parent type of <I><c>child</c></I>.

@return Returns <CODE>CG_TRUE</CODE> if <I><c>parent</c></I> is a parent type of <I><c>child</c></I>.
Returns <CODE>CG_FALSE</CODE> otherwise.

@param       parent  Specifies the parent type.
@param       child   Specifies the child type.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@sa       cgGetParentType
*/
CG_API CGbool cgIsParentType( CGtype parent, CGtype child )
{
    JS_APIENTRY_TIMER( cgIsParentType );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( cgIsParentType, JSDPM_CALL_TYPE_STATE_CG, 0, 2,
                             parent, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             child, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }
#endif
    // TODO **************
    return CG_FALSE;
}

/**
@short  Determine if a type is an interface

cgIsInterfaceType() returns a <CODE>CGbool</CODE> indicating if <I><c>type</c></I> is an interface.

@param       type    Specifies the type being evaluated.

@note
Use of this function is discouraged, as the
advanced features it supports are not available.

@return Returns  <CODE>CG_TRUE</CODE> if <I><c>type</c></I> is an interface (not just a struct), <CODE>CG_FALSE</CODE> otherwise.

@sa       cgGetType
 */
CG_API CGbool cgIsInterfaceType( CGtype type )
{
    JS_APIENTRY_TIMER( cgIsInterfaceType );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgIsInterfaceType, JSDPM_CALL_TYPE_STATE_CG, 0, 1, type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif

    // TODO **************
    return CG_FALSE;
}


// Resource Functions -----------------------

/**
@short  Gets the resource name associated with a resource enumerant.

cgGetResourceString() returns the resource name associated with a
resource enumerant.

@param       resource   The resource enumerant.

@return Returns the resource string of the enumerant <I><c>resource</c></I>.

@sa cgGetResource, cgGetParameterResource
 */
CG_API const char* cgGetResourceString( CGresource resource )
{
    JS_APIENTRY_TIMER( cgGetResourceString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetResourceString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, resource, sizeof( CGresource ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    return _JS_MAP_LOOKUP_ENUM( JScgResourceMap, resource );
}

/**
@short  Get the resource enumerant assigned to a resource name

cgGetResource() returns the enumerant assigned to a resource name.

@par Example
@code
CGresource PositionResource = cgGetResource("POSITION");
if (cgGetParameterResource(myparam) == PositionResource)
   {
      ...
   }
@endcode

@param       resource_string        A string containing the resource name.

@return Returns the resource enumerant of <I><c>resource_string</c></I>.  If no such resource
exists <CODE>CG_UNDEFINED</CODE> will be returned.

@sa cgGetResourceString, cgGetParameterResource
*/
CG_API CGresource cgGetResource( const char* resource_string )
{
    JS_APIENTRY_TIMER( cgGetResource );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetResource, JSDPM_CALL_TYPE_STATE_CG, 0, 1, resource_string, strlen( resource_string ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    GLenum r = _JS_MAP_LOOKUP_STRING( JScgResourceMap, resource_string );
    return ( r == -1U ) ? CG_UNDEFINED : ( CGresource )r;
}


// Enum Functions ----------------------------

/**
@short  Get the name string associated with an enumerant

cgGetEnumString() returns the name string associated with the enumerant
<I><c>en</c></I>.   It&rsquo;s primarily useful for printing out debugging information.

@param       en      The enumerant.

@return Returns the enum string of the enumerant <I><c>en</c></I>.
<CODE>NULL</CODE> is returned in the event of an error.

@par Examples
The following example will print "CG_UNIFORM" to the stdout.
@code
const char *EnumString = cgGetEnumString(CG_UNIFORM);
printf("%s\n", EnumString);
@endcode
@sa       cgGetEnum
 */
CG_API const char* cgGetEnumString( CGenum en )
{
    JS_APIENTRY_TIMER( cgGetEnumString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetEnumString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, en, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif

    return _JS_MAP_LOOKUP_ENUM( JScgEnumMap, en );
}

/**
@short  Get the enumerant assigned with the given string name

cgGetEnum() returns the enumerant assigned to an enumerant name.

The following is an example of how cgGetEnum() might be used.
@code
CGenum VaryingEnum = cgGetEnum("CG_VARYING");
@endcode

@param enum_string
A string containing the enum name.  The name is case-sensitive.

@return Returns the enumerant of <I><c>enum_string</c></I>.  If no such enumerant exists
<CODE>CG_UNKNOWN</CODE> is returned.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_PARAMETER_ERROR</CODE></TD>
<TD><I><c>enum_string</c></I> is <CODE>NULL</CODE>.</TD>
</TR>
</TABLE>

@sa       cgGetEnumString
 */
CG_API CGenum cgGetEnum( const char* enum_string )
{
    JS_APIENTRY_TIMER( cgGetEnum );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetEnum, JSDPM_CALL_TYPE_STATE_CG, 0, 1, enum_string, strlen( enum_string ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    if ( !enum_string ) { _jsCgRaiseError( CG_INVALID_PARAMETER_ERROR ); return CG_UNKNOWN; }
    GLenum r = _JS_MAP_LOOKUP_STRING( JScgEnumMap, enum_string );
    return ( r == -1U ) ? CG_UNKNOWN : ( CGenum )r;
}


// Profile functions -------------------------


// profiles are the only tokens not stored in a JSenumMap. Instead
// they use a JScgProfileMap which contains extra information about
// whether the profile is a vertex or a fragment program.

/**
@short  Get the profile name associated with a profile enumerant

cgGetProfileString() returns the profile named associated with a profile
enumerant.

@param       profile The profile enumerant.

@return Returns the profile string of the enumerant <I><c>profile</c></I>.

@sa cgGetProfile, cgGetProgramProfile
 */
CG_API const char* cgGetProfileString( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGetProfileString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetProfileString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    const size_t arraysize = sizeof( JScgProfileMap ) / sizeof( JScgProfileMapType );
    unsigned int i = 0;
    while ( i < arraysize )
    {
        if ( profile == JScgProfileMap[i].id )
        {
            // id found.
            return JScgProfileMap[i].string;
        }
        ++i;
    }
    // id not found, return an empty string
    return "";
}

/**
@short  Get the profile enumerant from a profile name

cgGetProfile() returns the enumerant assigned to a profile name.

@par Example
@code
CGprofile ARBVP1Profile = cgGetProfile("arbvp1");
if (cgGetProgramProfile(myprog) == ARBVP1Profile)
   {
      ...
   }
@endcode

@param   profile_string  A string containing the profile name.  The name is case-sensitive.

@return Returns the profile enumerant of <I><c>profile_string</c></I>.
If no such profile exists <CODE>CG_PROFILE_UNKNOWN</CODE> will be returned.

@sa cgGetProfileString, cgGetProgramProfile
*/
CG_API CGprofile cgGetProfile( const char* profile_string )
{
    JS_APIENTRY_TIMER( cgGetProfile );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetProfile, JSDPM_CALL_TYPE_STATE_CG, CG_PROFILE_UNKNOWN, 1, profile_string, strlen( profile_string ) + 1, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    size_t arraysize = sizeof( JScgProfileMap ) / sizeof( JScgProfileMapType );
    unsigned int i = 0;
    while ( i < arraysize )
    {
        if ( 0 == strcmp( JScgProfileMap[i].string, profile_string ) )
        {
            // string found.
            return JScgProfileMap[i].id;
        }
        ++i;
    }
    // string not found, return fail code.
    return CG_PROFILE_UNKNOWN;
}


// ErrorFunctions ----------------------------
/**
@short  Get the current error condition

cgGetError() returns a <I>CGerror</I> type struct corresponding to a detected error condition.
If 0 is returned, no error has occurred.

@return Returns the current error condition. An error condition of zero means that no error has occurred.

@note
Cg and OpenGL have completely separate error reporting systems.  The error statuses stored for the
two systems are different.  Thus, cgGetError() and glGetError() refer to different error status codes.

@sa cgGetErrorString
*/
CG_API CGerror cgGetError( void )
{
    JS_APIENTRY_TIMER( cgGetError );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( cgGetError, JSDPM_CALL_TYPE_STATE_CG, 0 ); }
#endif
    CGerror err = _CurrentContext->JScgLastError;
    _CurrentContext->JScgLastError = CG_NO_ERROR;
    return err;
}

/**
@short  Get the string corresponding to an error condition

cgGetErrorString() returns a human readable string corresponding to a detected error condition.

@param  error   Specifies the error condition.

@return Returns a human readable error string for the given error condition.

@sa cgGetError
*/
CG_API const char* cgGetErrorString( CGerror error )
{
    JS_APIENTRY_TIMER( cgGetErrorString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetErrorString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, error, sizeof( CGerror ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE ); }
#endif
    return _JS_MAP_LOOKUP_ENUM( JScgErrorMap, error );
}

/**
@short  Get the current error condition

cgGetLastErrorString() returns the current error condition and error condition string.
It&rsquo;s similar to calling cgGetErrorString() with the
result of cgGetError().  However in certain cases the error string
may contain more information about the specific error that occurred last
than what cgGetErrorString() would return.

@param   error   A pointer to a <I>CGerror</I> variable for returning the last error code.

@return Returns the last error string.  Returns <CODE>NULL</CODE> if there was no error.
@par
If error is not NULL, it will return the last error code (the same
value cgGetError() would return in the location specified by <I><c>error</c></I>.

@sa cgGetError

*/
CG_API const char* cgGetLastErrorString( CGerror* error )
{
    JS_APIENTRY_TIMER( cgGetLastErrorString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetLastErrorString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, error, sizeof( CGerror ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER ); }
#endif
    // return both the error id and string.
    *error = _CurrentContext->JScgLastError;
    _CurrentContext->JScgLastError = CG_NO_ERROR;
    const char * result = _JS_MAP_LOOKUP_ENUM( JScgErrorMap, *error );
    return result;
}

/**
@short  Set the error callback

cgSetErrorCallback() sets a callback function that will be called every
time an error occurs.  The callback function is not passed any parameters.
It is assumed that the callback function will call cgGetError() to
obtain the current error.  To disable the callback function, cgSetErrorCallback()
may be called with <CODE>NULL</CODE>.

@par Example
@code
void myErrorCallback( void ) {
   CGerror cgerror = cgGetError();
   fprintf(stderr, "ERROR : %s\n", cgGetErrorString(cgerror));
}

void main(int argc, char *argv[]) {
   cgSetErrorCallback(myErrorCallback);
      ...
}
@endcode

@param func    A function pointer to the callback function.

@sa cgGetError, cgGetErrorString, cgGetErrorCallback

*/
CG_API void cgSetErrorCallback( CGerrorCallbackFunc func )
{
    JS_APIENTRY_TIMER( cgSetErrorCallback );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgSetErrorCallback, JSDPM_CALL_TYPE_STATE_CG, , 1, func, sizeof( CGerrorCallbackFunc ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER ); }
#endif
    _CurrentContext->JScgErrorCallbackFunction = func;
}


/**
@short  Get the current error callback

cgGetErrorCallback() returns a <I>CGerrorCallbackFunc</I> pointer to the callback function.

@return Returns the currently set callback function.
<CODE>NULL</CODE> will be returned if no callback function is set.

@sa cgSetErrorCallback
*/
CG_API CGerrorCallbackFunc cgGetErrorCallback( void )
{
    JS_APIENTRY_TIMER( cgGetErrorCallback );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( cgGetErrorCallback, JSDPM_CALL_TYPE_STATE_CG, NULL ); }
#endif
    return _CurrentContext->JScgErrorCallbackFunction;
}


// Misc Functions -----------------------------

/**
@short  Gets a special string

cgGetString() returns an informative string depending on the <I><c>sname</c></I>.
Currently there is only one valid enumerant that may be passed in.

<TABLE rules=all>
<TR>
<TD><CODE>CG_VERSION</CODE></TD>
<TD>
Returns the version string of the Cg runtime and compiler.
</TD>
</TR>
</TABLE>

@param sname   An enumerant describing the string to be returned.

@return Returns the string depending on <I><c>sname</c></I>.
Returns <CODE>NULL</CODE> in the event of an error.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_INVALID_ENUMERANT_ERROR</CODE></TD>
<TD><I><c>sname</c></I> is an invalid enumerant.</TD>
</TR>
</TABLE>

*/
CG_API const char* cgGetString( CGenum sname )
{
    JS_APIENTRY_TIMER( cgGetString );
#ifndef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( cgGetString, JSDPM_CALL_TYPE_STATE_CG, NULL, 1, sname, sizeof( CGenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }
#endif

    if ( sname == CG_VERSION )
    {
        // this should return the version of the runtime or perhaps more meaninfully, the runtime compiler
        static char versionstring[8];
        sprintf( versionstring, "%d", CG_VERSION_NUM );
        return versionstring;
    }

    _jsCgRaiseError( CG_INVALID_ENUMERANT_ERROR );
    return NULL;
}



/**
@short Given a profile, return which domain it covers.

cgGetProfileDomain() returns the domain covered by a particular profile.
For example, the <code>CG_PROFILE_SCE_FP_RSX</code> profile specifies programs that
execute in the <code>CG_FRAGMENT_PROGRAM</code> domain.

@param       profile The profile enumerant.

@return Returns the domain enumerant (<code>CG_VERTEX_DOMAIN</code>, <code>CG_FRAGMENT_DOMAIN</code>
or <code>CG_GEOMETRY_DOMAIN</code>) associated with the <i><c>profile</c></i> if the profile
is recognized, <code>CG_UNKNOWN_DOMAIN</code> otherwise.

@par Errors
Use cgGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>CG_UNKNOWN_PROFILE_ERROR</CODE></TD>
<TD><I><c>profile</c></I> is an invalid profile enumerant.</TD>
</TR>
</TABLE>

@sa cgGetProfile, cgGetProgramProfile
*/
CG_API CGdomain cgGetProfileDomain( CGprofile profile )
{
    JS_APIENTRY_TIMER( cgGetProfileDomain );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGetProfileDomain, JSDPM_CALL_TYPE_STATE_CG, NULL, 1,
                             profile, sizeof( CGprofile ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    const size_t arraysize = sizeof( JScgProfileMap ) / sizeof( JScgProfileMapType );
    unsigned int i = 0;
    while ( i < arraysize )
    {
        if ( profile == JScgProfileMap[i].id )
        {
            // id found, check whether this is a vertex or fragment program
            if ( JScgProfileMap[i].is_vertex_program )
                return CG_VERTEX_DOMAIN;
            else
                return CG_FRAGMENT_DOMAIN;
        }
        ++i;
    }
    // id not found, return an unknown domain
    _jsCgRaiseError( CG_UNKNOWN_PROFILE_ERROR );
    return CG_UNKNOWN_DOMAIN;
}



/**
@short Return the <code>CGparameterclass</code> of a given <code>CGtype</code>

Given a <code>CGtype</code> enumeration, cgGetTypeClass() returns the parameter
class of that type. For example, a <code>CG_FLOAT4</code> type belongs to the
<code>CG_PARAMETERCLASS_VECTOR</code> class, <code>CG_FLOAT4x4</code> belongs to the
<code>CG_PARAMETERCLASS_MATRIX</code> class, and <code>CG_SAMPLER2D</code> belongs to the
<code>CG_PARAMETERCLASS_SAMPLER</code> class.

@param       type The <code>CGtype</code> to be queried.

@return A value from the <code>Cgparameterclass</code> enumeration if the <code>CGtype</code> is
recognized, <code>CG_PARAMETERCLASS_UNKNOWN</code> otherwise.

@sa cgGetTypeBase
*/
CG_API CGparameterclass cgGetTypeClass( CGtype type )
{
    JS_APIENTRY_TIMER( cgGetTypeClass );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGetTypeClass, JSDPM_CALL_TYPE_STATE_CG, NULL, 1,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif

    const JScgTypeMapType *ptr = _jsLookupEnum( JScgTypeMap, type );
    if ( 0 == ptr )
    {
        return CG_PARAMETERCLASS_UNKNOWN;
    }
    return ptr->parameter_class;
}

/**
@short Return the base type of a given CGtype

User types are defined as implementations of interface base types. Given a usertype,
cgGetTypeBase() returns the base type used in the definition of that usertype.  For example, "struct MySpotlight"
is a concrete instance of the abstract "interface MyLight". The base type is the same
result found if you were to call cgGetParentType() repeatedly until there are no more
parents.

@param       type The <code>CGtype</code> to be queried.

@return The enumeration is an interface type that is the parent base class of the argument.

@sa cgGetType, cgGetParentType
*/
CG_API CGtype cgGetTypeBase( CGtype type )
{
    JS_APIENTRY_TIMER( cgGetTypeBase );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_1( cgGetTypeBase, JSDPM_CALL_TYPE_STATE_CG, NULL, 1,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM );
    }
#endif
    // get the base type of a usertype without having to create
    // a parameter of that type.

    // TODO: usertypes not supported in Jetstream
    return CG_UNKNOWN_TYPE;
}


/**
@short Returns the number of rows and columns in a <code>CGtype</code>

cgGetTypeSizes() returns the number of rows and columns in a <code>CGtype</code>

cgGetTypeSizes() returns 1 for <i><c>ncols</c></i> if the data type is not a matrix type.

@param       type   The <code>CGtype</code> to be queried.
@param       nrows  A pointer to an integer where the number of rows will be written.
@param       ncols  A pointer to an integer where the number of columns will be written.

@return Returns <code>CG_TRUE</code> if the type is a matrix type, <code>CG_FALSE</code> otherwise.

@sa cgGetMatrixSize
*/
CG_API CGbool cgGetTypeSizes( CGtype type, int *nrows, int *ncols )
{
    JS_APIENTRY_TIMER( cgGetTypeSizes );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGetTypeSizes, JSDPM_CALL_TYPE_STATE_CG, NULL, 3,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             nrows, sizeof( int* ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                             ncols, sizeof( int* ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    // TODO *****************
    return _cgMatrixDimensions( type, ( unsigned int* )nrows, ( unsigned int* )ncols );
    
}

/**
@short Returns the number of rows and columns in a matrix.

Given a matrix type, cgGetMatrixSize() returns the number of rows and columns in that matrix.
cgGetMatrixSize() returns 0 for <i><c>ncols</c></i> if the data type is not a matrix type.

@param       type   The <code>CGtype</code> to be queried.
@param       nrows  A pointer to an integer where the number of rows will be written.
@param       ncols  A pointer to an integer where the number of columns will be written.

@sa cgGetTypeSizes
*/
CG_API void cgGetMatrixSize( CGtype type, int *nrows, int *ncols )
{
    JS_APIENTRY_TIMER( cgGetMatrixSize );
#ifndef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( cgGetMatrixSize, JSDPM_CALL_TYPE_STATE_CG, NULL, 3,
                             type, sizeof( CGtype ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             nrows, sizeof( int* ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER,
                             ncols, sizeof( int* ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
#endif

    if (( 0 == nrows ) || ( 0 == ncols ) )
    {
        _jsCgRaiseError( CG_INVALID_POINTER_ERROR );
        return;
    }

    if (( type >= CG_FLOAT1x1 && type <= CG_FLOAT4x4 ) ||
	( type >= CG_HALF1x1 && type <= CG_HALF4x4 ) ||
	( type >= CG_INT1x1 && type <= CG_INT4x4 ) ||
	( type >= CG_BOOL1x1 && type <= CG_BOOL4x4 ) ||
	( type >= CG_FIXED1x1 && type <= CG_FIXED4x4 ))
	{
		_cgMatrixDimensions( type, ( unsigned int* )nrows, ( unsigned int* )ncols );
		return; 
	}

    // type supplied is not a recognised matrix type
    _jsCgRaiseError( CG_NOT_MATRIX_PARAM_ERROR );
    // NOTE: but fall through to return 0,0...
    *nrows = 0;
    *ncols = 0;

}


/**
 * @}
 */
