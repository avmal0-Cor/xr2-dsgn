/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _cg_common_h
#define _cg_common_h

#include <Cg/cg.h>

#include <PSGL/psgl.h>
#include <PSGL/private.h>

#include <vector>
#include <string>

#include "Cg/CgInternal.h"
#include "Cg/CgProgramGroup.h"

#ifndef STL_NAMESPACE
#define STL_NAMESPACE std::
#endif

// [YLIN]
#include <string.h> // for memcpy16
#if (CELL_SDK_VERSION<=0x210001)
using _CSTD memcpy16; // fixed at SDK 220
#endif // (CELL_SDK_VERSION<=0x210001)

#ifdef __cplusplus
extern "C"
{
#endif


#define JS_MAX_VP_SHARED_CONSTANTS 256
#define JS_MAX_FP_SHARED_CONSTANTS 1024
#define JS_BOOLEAN_REGISTERS_COUNT 32

// parameter setter, prototype of functions called when a uniform is set.
    typedef void( *_cgSetFunction )( struct CgRuntimeParameter* _JS_RESTRICT, const void* _JS_RESTRICT );

    typedef void( *_cgSetArrayIndexFunction )( struct CgRuntimeParameter* _JS_RESTRICT, const void* _JS_RESTRICT, const int index );

    typedef struct _CgUniform
    {
        void *pushBufferPtr;
    }
    _CgUniform;

    typedef struct _CGprogram _CGprogram;

    typedef struct CgRuntimeParameter
    {
        // sampler
        _cgSetArrayIndexFunction samplerSetter;
        /*cgSetFunction*/

        //vector
        _cgSetArrayIndexFunction  setterIndex;
        // matrices
        _cgSetArrayIndexFunction settercIndex;
        _cgSetArrayIndexFunction setterrIndex;

        void *pushBufferPointer;
        const CgParameterEntry *parameterEntry;
        _CGprogram *program;
        int glType; //tmp...
        CGparameter id;//tmp
    }
    CgRuntimeParameter;

    typedef struct
    {
        CgRuntimeParameter* child;
        CgRuntimeParameter* parent;
        CgRuntimeParameter* top;
        _cgSetArrayIndexFunction childOnBindSetter;
    }
    CgParameterConnection;

    typedef struct
    {
        CgRuntimeParameter* param;
        void* fx;
    }
    CgParameterFX;

    typedef struct
    {
        CgRuntimeParameter* param;
        STL_NAMESPACE vector<char> semantic;
    }
    CgRuntimeSemantic;

    struct _CGprogram
    {
        struct _CGprogram*   next;          // link to next in NULL-terminated singly linked list of programs
        CGprogram            id;            // numerical id for this program object
        struct _CGcontext*   parentContext; // parent context for this program
        void*                parentEffect;  // parent effect for this program (only used for default program of an effect, containing effect parameters)
		bool				 inLocalMemory; // only pertains to fragment programs which can be in location local, the default, or location main via cgb interfaces 
        // parameters in the CG_PROGRAM namespace

        unsigned int         constantPushBufferWordSize;
        unsigned int*        constantPushBuffer;
        unsigned int         lastConstantUpdate;

        // executable program image
        void*     platformProgram;

        // entire binary program image from compiler
        void*     platformProgramBinary;

        // extra information to ease the coding of the js runtime
        // the following are used to know which parameters are samplers, so that we can walk them in a quick way.
        unsigned int         samplerCount;
        unsigned int *       samplerIndices;
        unsigned int *       samplerUnits;

        unsigned int         controlFlowBools;

        void *			FXprogramExtension; // on-demand allocated by FX

        //binary format additions
        //info previously contained in platformProgram ( loadAddress + nvBinary )
        CgProgramHeader header;
        const char *name;
        const void *ucode;

        GLuint loadProgramId;
        // offset into the allocation id above, normally zero for internal use.
        // But for psglSetFragmentProgramConfiguration it's possible for 
        // users to manage sub-heap in allocation and put multiple 
        // program in each allocation.
        GLuint loadProgramOffset; 

        int version; //contained a boolean indicating if the structure pointers have been patched or not

        char *parameterResources;
        int rtParametersCount;
        CgRuntimeParameter *runtimeParameters;
        const CgParameterEntry *parametersEntries;

        unsigned short *resources;
        unsigned short *pushBufferPointers;

        int defaultValuesIndexCount;
        const CgParameterDefaultValue *defaultValuesIndices;

        int semanticCount;
        const CgParameterSemantic *semanticIndices;

        int defaultValueCount;
        const float *defaultValues;

        const char *stringTable;

        unsigned int **constantPushBufferPointers;

        //tmp
        unsigned int *samplerValuesLocation;
        void *memoryBlock;

        _CGprogramGroup *programGroup;
        int programIndexInGroup;

        // supports runtime created parameters
        STL_NAMESPACE vector<CgRuntimeParameter*> runtimeCreatedParameters;

        // supports parameter connections
        STL_NAMESPACE vector<CgParameterConnection> connectionTable;

        // supports runtime created parameters
        STL_NAMESPACE vector<CgParameterFX> fxParameterExtensions;

        // supports runtime allocation of semantics
        STL_NAMESPACE vector<CgRuntimeSemantic> parameterSemantics;

        //runtime compilation / conversion
        void *runtimeElf;
    };

    typedef struct _CGcontext
    {
        struct _CGcontext* next;            // for global linked list of CGcontexts
        CGcontext          id;              // numerical handle for this object
        unsigned int       programCount;    // number of programs in the list
        struct _CGprogram* programList;     // head of singly linked list of programs

        CGenum             compileType;     // compile manual, immediate or lazy (unused so far)

        // implementation data for GL
        CGbool             GLmanageTextures;

        unsigned int         controlFlowBoolsSharedMask;
        unsigned int         controlFlowBoolsShared;

        // default program, fake owner of the context parameters
        _CGprogram        defaultProgram;
        void *             FXcontextExtension;

        //groups
        CGprogramGroup groupList;

        //"static" variable used to store the values of the last parameter for which getParameterValue has been called
        double currentParameterValue[16];
        char currentParameterName[128];
    }
    _CGcontext;


// prototypes --------------------------------

// internal error handling
    PSGL_EXPORT void _jsCgRaiseError( CGerror error );
// interface between object types
    extern void _jsCgProgramDestroyAll( _CGcontext* c );
    extern void _jsCgDestroyContextParam( CgRuntimeParameter* p );
    PSGL_EXPORT CgRuntimeParameter*_jsCgCreateParameterInternal( _CGprogram *program, const char* name, CGtype type );
    PSGL_EXPORT void _jsCgProgramErase( _CGprogram* prog );

// default setters
    void _cgRaiseInvalidParam( CgRuntimeParameter*p, const void*v );
    void _cgRaiseNotMatrixParam( CgRuntimeParameter*p, const void*v );
    void _cgIgnoreSetParam( CgRuntimeParameter*p, const void*v );
    void _cgRaiseInvalidParamIndex( CgRuntimeParameter*p, const void*v, const int index );
    void _cgRaiseNotMatrixParamIndex( CgRuntimeParameter*p, const void*v, const int index );
    void _cgIgnoreSetParamIndex( CgRuntimeParameter*p, const void*v, const int index );

// cg helpers

// Is macros
#define CG_IS_CONTEXT(_ctx) _jsIsName(&_CurrentContext->cgContextNameSpace, (jsName)_ctx)
#define CG_IS_PROGRAM(_program) _jsIsName(&_CurrentContext->cgProgramNameSpace, (jsName)_program)
#define CG_IS_PARAMETER(_param) _jsIsName(&_CurrentContext->cgParameterNameSpace, (jsName)(((unsigned int)_param)&CG_PARAMETERMASK))

//array indices
#define CG_PARAMETERSIZE 22 //22 bits == 4 millions parameters
#define CG_PARAMETERMASK ((1<<CG_PARAMETERSIZE)-1)
#define CG_GETINDEX(param) (int)((unsigned int)(param)>>CG_PARAMETERSIZE)


    inline static bool isMatrix( CGtype type )
    {
        if (( type >= CG_FLOAT1x1 && type <= CG_FLOAT4x4 ) ||
	    ( type >= CG_HALF1x1 && type <= CG_HALF4x4 ) ||
	    ( type >= CG_INT1x1 && type <= CG_INT4x4 ) ||
	    ( type >= CG_BOOL1x1 && type <= CG_BOOL4x4 ) ||
	    ( type >= CG_FIXED1x1 && type <= CG_FIXED4x4 ))
	    return true;
	return false;
    }

    inline static bool isSampler( CGtype type )
    {
        return ( type >= CG_SAMPLER1D && type <= CG_SAMPLERCUBE );
    }


    unsigned int _jsCountFloatsInCgType( CGtype type );
    CGbool _cgMatrixDimensions( CGtype type, unsigned int* nrows, unsigned int* ncols );

    unsigned int _jsGetTypeRowCount( CGtype parameterType );
    unsigned int _jsGetTypeColCount( CGtype parameterType );

// the internal cg conversions
    inline static CgRuntimeParameter* _cgGetParamPtr( CGparameter p )
    {
        //return (_CGparameter*)_jsGetNamedValue(&_CurrentContext->cgParameterNameSpace, (jsName)p);
        //that's 4 millions of parameters, it means the max array size is 1024, which might be a little low for RSX computing application ?
        return ( CgRuntimeParameter* )_jsGetNamedValue( &_CurrentContext->cgParameterNameSpace, ( jsName )((( unsigned int )p )&CG_PARAMETERMASK ) );
    }

    inline static _CGprogram* _cgGetProgPtr( CGprogram p )
    {
        return ( _CGprogram* )_jsGetNamedValue( &_CurrentContext->cgProgramNameSpace, ( jsName )p );
    }

    inline static _CGcontext* _cgGetContextPtr( CGcontext c )
    {
        return ( _CGcontext* )_jsGetNamedValue( &_CurrentContext->cgContextNameSpace, ( jsName )c );
    }

    inline static CgRuntimeParameter* _jsCgGLTestParameter( CGparameter param )
    {
#ifndef JS_NO_ERROR_CHECK
        if ( JS_UNLIKELY( !CG_IS_PARAMETER( param ) ) )
        {
            _jsCgRaiseError( CG_INVALID_PARAM_HANDLE_ERROR );
            return NULL;
        }
#endif
        return _cgGetParamPtr( param );
    }

    CgRuntimeParameter* _cgGLTestArrayParameter( CGparameter paramIn, long offset, long nelements );
    CgRuntimeParameter* _cgGLTestTextureParameter( CGparameter param );

#ifdef JS_DEBUG
    void _jsCgDumpState();
    void _jsCgPrintSpaces( unsigned int n );
#endif

    inline static int _jsGetSizeofSubArray( const unsigned short *dimensions, unsigned short count )
    {
        int res = 1;
        for ( int i = 0;i < count;i++ )
            res *= ( int )( *( dimensions++ ) );
        return res;
    }

    inline static CGresource _jsGetBaseResource( CGresource resource )
    {
        switch ( resource )
        {
case CG_ATTR0: case CG_ATTR1: case CG_ATTR2: case CG_ATTR3:
case CG_ATTR4: case CG_ATTR5: case CG_ATTR6: case CG_ATTR7:
case CG_ATTR8: case CG_ATTR9: case CG_ATTR10: case CG_ATTR11:
case CG_ATTR12: case CG_ATTR13: case CG_ATTR14: case CG_ATTR15:
                return CG_ATTR0;
            case CG_HPOS:
                return CG_HPOS;
case CG_COL0: case CG_COL1: case CG_COL2: case CG_COL3:
                return CG_COL0;
case CG_TEXCOORD0: case CG_TEXCOORD1: case CG_TEXCOORD2: case CG_TEXCOORD3:
case CG_TEXCOORD4: case CG_TEXCOORD5: case CG_TEXCOORD6: case CG_TEXCOORD7:
        case CG_TEXCOORD8: case CG_TEXCOORD9:
                return CG_TEXCOORD0;
case CG_TEXUNIT0: case CG_TEXUNIT1: case CG_TEXUNIT2: case CG_TEXUNIT3:
case CG_TEXUNIT4: case CG_TEXUNIT5: case CG_TEXUNIT6: case CG_TEXUNIT7:
case CG_TEXUNIT8: case CG_TEXUNIT9: case CG_TEXUNIT10: case CG_TEXUNIT11:
case CG_TEXUNIT12: case CG_TEXUNIT13: case CG_TEXUNIT14: case CG_TEXUNIT15:
                return CG_TEXUNIT0;
            case CG_FOGCOORD:
                return CG_FOGCOORD;
            case CG_PSIZ:
                return CG_PSIZ;
            case CG_WPOS:
                return CG_WPOS;
case CG_COLOR0: case CG_COLOR1: case CG_COLOR2: case CG_COLOR3:
                return CG_COLOR0;
            case CG_DEPTH0:
                return CG_DEPTH0;
            case CG_C:
                return CG_C;
            case CG_B:
                return CG_B;
case CG_CLP0: case CG_CLP1: case CG_CLP2: case CG_CLP3: case CG_CLP4: case CG_CLP5:
                return CG_CLP0;
            case CG_UNDEFINED:
                return CG_UNDEFINED;
            default:
#ifndef MSVC
                _JS_REPORT_EXTRA( PSGL_REPORT_ASSERT, "resource 0x%d is unknown here", resource );
#endif
                JS_ASSERT( !"unknown resource" );
                return CG_UNDEFINED;
        }
    }

// platform API
    CGprofile _jsPlatformGetLatestProfile( CGGLenum profile_type );
    int _jsPlatformCopyProgram( _CGprogram* source, _CGprogram* destination );

    void _jsPlatformProgramErase( void* platformProgram );

    int _jsPlatformGenerateVertexProgram( _CGprogram *program, const CgProgramHeader *programHeader, const void *ucode, const CgParameterTableHeader *parameterHeader, const char *stringTable, const float *defaultValues );
    CGbool _jsPlatformSupportsVertexProgram( CGprofile p );

    int _jsPlatformGenerateFragmentProgram( _CGprogram *program, const CgProgramHeader *programHeader, const void *ucode, const CgParameterTableHeader *parameterHeader, const char *stringTable, const float *defaultValues );
    CGbool _jsPlatformSupportsFragmentProgram( CGprofile p );



    void _jsPlatformSetVertexRegister4fv( unsigned int reg, const float * _JS_RESTRICT v );
    void _jsPlatformSetVertexRegisterBlock( unsigned int reg, unsigned int count, const float * _JS_RESTRICT v );
    void _jsPlatformSetFragmentRegister4fv( unsigned int reg, const float * _JS_RESTRICT v );
    void _jsPlatformSetFragmentRegisterBlock( unsigned int reg, unsigned int count, const float * _JS_RESTRICT v );

    void _jsPlatformSetBoolVertexRegisters( unsigned int values );

// names API
    PSGL_EXPORT unsigned int _cgHashString( const char *str );

    inline static void _pullConnectedParameterValues( _CGprogram* ptr )
    {
        // we now use a pull method to get the data into the children parameters
        // when their program is bound they pull the data from their parents
        STL_NAMESPACE vector<CgParameterConnection>::iterator connectionIter = ptr->connectionTable.begin();
        while ( connectionIter != ptr->connectionTable.end() )
        {
            // pull data into connectionIter->child from connectionIter->top
            connectionIter->childOnBindSetter( connectionIter->child, connectionIter->top->pushBufferPointer, 0 );
            connectionIter++;
        }
    }

    inline static void _cgGLBindVertexProgram( _CGprogram* program )
    {
        // the program is a vertex program, just update the GL state
        _CurrentContext->BoundVertexProgram = program;

        // and inform the GL state to re-upload the vertex program
        _CurrentContext->needValidate |= PSGL_VALIDATE_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_TEXTURES_USED;

        // This must happen before the sampler setters so texture parameters have the correct value in their push buffers for that routine
        _pullConnectedParameterValues( program );

        CGbool is_managed = program->parentContext->GLmanageTextures;
        // enable texture parameters if the managed flag is set.
        if ( is_managed )
        {
            for ( GLuint index = 0; index < program->samplerCount; ++index )
            {
                // walk the array of sampler parameters
                CgRuntimeParameter *rtParameter = program->runtimeParameters + program->samplerIndices[index];

                JS_ASSERT((( CgParameterResource * )( program->parameterResources + rtParameter->parameterEntry->typeIndex ) )->type == CG_SAMPLER2D );
                // find out which texture unit this parameter has been assigned to
                JS_ASSERT(((( CgParameterResource * )( program->parameterResources + rtParameter->parameterEntry->typeIndex ) )->resource - CG_TEXUNIT0 ) < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS );
                rtParameter->samplerSetter( rtParameter, NULL, 0 );
            }
        }
    }

    inline static void _cgGLBindFragmentProgram( _CGprogram* program )
    {
        _CurrentContext->BoundFragmentProgram = program;

        //support for mix combined usage of the Cg runtime and the low level shader API
        _CurrentContext->ShellVertexProgram->header.vertexProgram.attributeOutputMask = program->header.attributeInputMask;

        // need to revalidate the textures in order to update which targets to fetch from
        _CurrentContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;

        // This must happen before the sampler setters so texture parameters have the correct value in their push buffers for that routine
        _pullConnectedParameterValues( program );

        // TODO: push texture state
        //  Needs to be done per profile. Can't use glPushAttrib.

        CGbool is_managed = program->parentContext->GLmanageTextures;

        // deal with the texture parameters now.
        for ( GLuint index = 0; index < program->samplerCount; ++index )
        {
            // walk the array of sampler parameters
            CgRuntimeParameter *rtParameter = program->runtimeParameters + program->samplerIndices[index];
            CgParameterResource *parameter = ( CgParameterResource * )( program->parameterResources + rtParameter->parameterEntry->typeIndex );
            JS_ASSERT( parameter->type >= CG_SAMPLER1D && parameter->type <= CG_SAMPLERCUBE );
            // find out which texture unit this parameter has been assigned to
            unsigned int unit = parameter->resource - CG_TEXUNIT0;

            JS_ASSERT( unit < _JS_MAX_TEXTURE_IMAGE_UNITS );
            _CurrentContext->TextureImageUnits[unit].fragmentTarget = rtParameter->glType;

            // enable texture parameters if the managed flag is set.
            if ( is_managed )
            {
                //tmp
                rtParameter->samplerSetter( rtParameter, NULL, 0 );
            }
            else
            {
                _jsUpdateCurrentTextureCache( &_CurrentContext->TextureImageUnits[unit] );
            }
        }
    }

    inline static void _cgGLUnbindVertexProgram()
    {
        _CurrentContext->BoundVertexProgram = NULL;
        _CurrentContext->needValidate |= PSGL_VALIDATE_FFX_VERTEX_PROGRAM | PSGL_VALIDATE_VERTEX_PROGRAM;
    }

// XXX this should not be here.
    inline static void _jsLeaveFFXFP( PSGLcontext*LContext )
    {
        LContext->FragmentProgram = GL_TRUE;
        struct _CGprogram* current = LContext->BoundFragmentProgram;
        if ( current )
        {
            for ( GLuint i = 0; i < current->samplerCount; ++i )
            {
                int unit = current->samplerUnits[i];
                _jsUpdateCurrentTextureCache( &_CurrentContext->TextureImageUnits[unit] );
            }
        }
        LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    }

    inline static void _jsEnterFFXFP( PSGLcontext*LContext )
    {
        // the currenttexture cache was specific to the current program,
        // and the unused units were out of sync, so we need to revalidate all thats needed for ffx.
        for ( GLuint unit = 0; unit < _JS_MAX_TEXTURE_UNITS; ++unit )
        {
            _jsUpdateCurrentTextureCache( &LContext->TextureImageUnits[unit] );
        }

        LContext->needValidate |= PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM | PSGL_VALIDATE_TEXTURES_USED;
    }

    inline static void _cgGLUnbindFragmentProgram()
    {
        _CurrentContext->BoundFragmentProgram = NULL;
        _jsEnterFFXFP( _CurrentContext );
    }

    static inline GLenum _jsCgGetSamplerGLTypeFromCgType( CGtype type )
    {
        switch ( type )
        {
            case CG_SAMPLER1D:
            case CG_SAMPLER2D:
            case CG_SAMPLERRECT:
                return GL_TEXTURE_2D;
            case CG_SAMPLER3D:
                return GL_TEXTURE_3D;
            case CG_SAMPLERCUBE:
                return GL_TEXTURE_CUBE_MAP;
            default:
                JS_ASSERT( 0 );
                return 0;
        }
    }

    static inline int is_created_param( CgRuntimeParameter* ptr )
    {
        JS_ASSERT( ptr->parameterEntry != NULL );
        if ( ptr->parameterEntry->flags & CGP_RTCREATED )
        {
            return 1;
        }
        return 0;
    }

    struct jsNameSpace;

#define VERTEX_PROFILE_INDEX 0
#define FRAGMENT_PROFILE_INDEX 1
    int _jsNVGenerateProgram( _CGprogram *program, int profileIndex, const CgProgramHeader *programHeader, const void *ucode,
                              const CgParameterTableHeader *parameterHeader, const CgParameterEntry *parameterEntries,
                              const char *stringTable, const float *defaultValues );

//
// these functions return the statically allocated table of function pointers originally
// written for NV unshared vertex parameter setters, but now also used by runtime
// created parameters cause these setters just do straight copies into the pushbuffer memory
//
    _cgSetArrayIndexFunction getVectorTypeIndexSetterFunction( unsigned short a, unsigned short b, unsigned short c, unsigned short d );
    _cgSetArrayIndexFunction getMatrixTypeIndexSetterFunction( unsigned short a, unsigned short b, unsigned short c, unsigned short d, unsigned short e, unsigned short f );

// -------------------------------------------

    typedef void( * CgcontextHookFunction )( _CGcontext *context );
    extern PSGL_EXPORT CgcontextHookFunction _cgContextCreateHook;
    extern PSGL_EXPORT CgcontextHookFunction _cgContextDestroyHook;

    typedef void( * CgparameterHookFunction )( CgRuntimeParameter *parameter );
    extern PSGL_EXPORT CgparameterHookFunction _cgParameterCreateHook;
    extern PSGL_EXPORT CgparameterHookFunction _cgParameterDestroyHook;

    typedef void( * CgprogramHookFunction )( _CGprogram *program );
    typedef void( * CgprogramCopyHookFunction )( _CGprogram *newprogram, _CGprogram *oldprogram );
    extern PSGL_EXPORT CgprogramHookFunction _cgProgramCreateHook;
    extern PSGL_EXPORT CgprogramHookFunction _cgProgramDestroyHook;
    extern PSGL_EXPORT CgprogramCopyHookFunction _cgProgramCopyHook;

    typedef int( * cgRTCgcCompileHookFunction )( const char*, const char *, const char*, const char**, char** );
    typedef void( * cgRTCgcFreeHookFunction )( char* );
    extern PSGL_EXPORT cgRTCgcCompileHookFunction _cgRTCgcCompileProgramHook;
    extern PSGL_EXPORT cgRTCgcFreeHookFunction _cgRTCgcFreeCompiledProgramHook;


//-----------------------------------------------
//inlined helper functions
    static inline int _jsGetParameterType( const CGprogram *program, const CgParameterEntry *entry )
    {
        return ( entry->flags & CGP_TYPE_MASK );
    }

    static inline const CgParameterResource *_jsGetParameterResource( const _CGprogram *program, const CgParameterEntry *entry )
    {
        JS_ASSERT( !( entry->flags & ( CGP_STRUCTURE | CGP_ARRAY ) ) && !( entry->flags & CGP_RTCREATED ) );
        return ( CgParameterResource * )( program->parameterResources + entry->typeIndex );
    }

    static inline CGtype _jsGetParameterCGtype( const _CGprogram *program, const CgParameterEntry *entry )
    {
        JS_ASSERT( !( entry->flags & ( CGP_STRUCTURE | CGP_ARRAY ) ) );
        if ( entry->flags & CGP_RTCREATED )
        {
            return ( CGtype )entry->typeIndex;
        }
        else
        {
            const CgParameterResource *parameterResource = _jsGetParameterResource( program, entry );
            if ( parameterResource )
            {
                return ( CGtype )parameterResource->type;
            }
        }
        return CG_UNKNOWN_TYPE;
    }

    static inline const CgParameterArray *_jsGetParameterArray( const _CGprogram *program, const CgParameterEntry *entry )
    {
        JS_ASSERT( entry->flags & CGP_ARRAY );
        return ( CgParameterArray* )( program->parameterResources + entry->typeIndex );
    }

    static inline const CgParameterStructure *_jsGetParameterStructure( const _CGprogram *program, const CgParameterEntry *entry )
    {
        JS_ASSERT( entry->flags & CGP_STRUCTURE );
        return ( CgParameterStructure* )( program->parameterResources + entry->typeIndex );
    }

    inline int _jsGetProgramProfileIndex( CGprofile profile )
    {
        if ( profile == CG_PROFILE_SCE_FP_TYPEB || profile == CG_PROFILE_SCE_FP_TYPEC || profile == CG_PROFILE_SCE_FP_RSX )
            return FRAGMENT_PROFILE_INDEX;
        else if ( profile == CG_PROFILE_SCE_VP_TYPEB || profile == CG_PROFILE_SCE_VP_TYPEC || profile == CG_PROFILE_SCE_VP_RSX )
            return VERTEX_PROFILE_INDEX;
        else
            return -1;
    }

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration that encloses file.
#endif

#endif
