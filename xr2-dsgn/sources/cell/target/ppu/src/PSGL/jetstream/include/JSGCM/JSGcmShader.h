/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef JSGCM_SHADER_H
#define JSGCM_SHADER_H

#include <PSGL/psgl.h>
#include <Cg/cg.h>
#include <Cg/cgBinary.h>
#include <Cg/CgCommon.h>
#include <JSGCM/JSGcmGlEnums.h>
#include <JSGCM/GmmAlloc.h>

typedef struct _jsGcmShader_
{
    GLuint loadAddressId;
    CgBinaryProgram __attribute__(( aligned( 16 ) ) ) program;
}
jsGcmShader;

#if JS_ENDIAN == JS_BIG_ENDIAN
#define ENDIAN_32(X, F) ((F) ? endianSwapWord(X) : (X))
#else
#define ENDIAN_32(X, F) (X)
#endif

extern int _parameterAlloc; 
extern int _ucodeAlloc; 

int _jsLoadFPShader( _CGprogram *program );

void _jsCreatePushBuffer( _CGprogram *program );
void _jsSetDefaultValuesFP( _CGprogram *program );
void _jsSetDefaultValuesVP( _CGprogram *program );

int _jsGetTypeResource( _CGprogram* program, unsigned short typeIndex, short *resourceIndex );
int _jsGetTypeResourceID( _CGprogram* program, unsigned short typeIndex );
int _jsGetTypeResourceRegisterCountVP( _CGprogram* program, short resourceIndex, int resourceCount, unsigned short *resource );


/*
**
** note:
**  - matrices are expected in column-major/OpenGL order
*/
void static inline _jsGcmFifoGlProgramParameterfv( CGprogram prog, CGparameter param, const GLfloat *value, GLuint ucodeId )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* ) prog;
    CgBinaryParameter *p = ( CgBinaryParameter* ) param;
    
    switch ( prg->profile )
    {
        case CG_PROFILE_VP40:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            JS_ASSERT(( p->var == CG_CONSTANT ) || ( p->var == CG_UNIFORM ) );
            if ( p->res != CG_UNDEFINED ) // unused argument
            {
                JS_ASSERT( p->res == CG_C );
                JS_ASSERT( p->resIndex < JSGCM_VTXPRG_MAX_CONST );
                switch ( p->type )
                {
                    case CG_FLOAT:
                    case CG_FLOAT1:
                    case CG_FLOAT2:
                    case CG_FLOAT3:
                    case CG_FLOAT4:
                        JS_ASSERT( p->resIndex < JSGCM_VTXPRG_MAX_CONST );                        
                        GCM_FUNC( cellGcmSetVertexProgramParameterBlock, p->resIndex, 1, value ); 
                        break;
                    case CG_FLOAT4x4:
                        // set 4 consts
                        JS_ASSERT(( p + 1 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 2 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 3 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 4 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        
                        {
                            GLfloat v2[16];
                            v2[0] = value[0];v2[1] = value[4];v2[2] = value[8];v2[3] = value[12];
                            v2[4] = value[1];v2[5] = value[5];v2[6] = value[9];v2[7] = value[13];
                            v2[8] = value[2];v2[9] = value[6];v2[10] = value[10];v2[11] = value[14];
                            v2[12] = value[3];v2[13] = value[7];v2[14] = value[11];v2[15] = value[15];
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 1 )->resIndex, 1, v2 + 0 ); // GCM_PORT_TESTED [Cedric]
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 2 )->resIndex, 1, v2 + 4 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 3 )->resIndex, 1, v2 + 8 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 4 )->resIndex, 1, v2 + 12 );
                        }
                        break;
                    case CG_FLOAT3x3:
                        // set 3 consts
                        JS_ASSERT(( p + 1 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 2 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 3 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        {
                            GLfloat v2[12];
                            v2[0] = value[0];v2[1] = value[3];v2[2] = value[6];v2[3] = 0;
                            v2[4] = value[1];v2[5] = value[4];v2[6] = value[7];v2[7] = 0;
                            v2[8] = value[2];v2[9] = value[5];v2[10] = value[8];v2[11] = 0;
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 1 )->resIndex, 1, v2 + 0 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 2 )->resIndex, 1, v2 + 4 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 3 )->resIndex, 1, v2 + 8 );
                        }
                        break;
                    case CG_HALF:
                    case CG_HALF1:
                    case CG_HALF2:
                    case CG_HALF3:
                    case CG_HALF4:
                    case CG_INT:
                    case CG_INT1:
                    case CG_INT2:
                    case CG_INT3:
                    case CG_INT4:
                    case CG_BOOL:
                    case CG_BOOL1:
                    case CG_BOOL2:
                    case CG_BOOL3:
                    case CG_BOOL4:
                    case CG_FIXED:
                    case CG_FIXED1:
                    case CG_FIXED2:
                    case CG_FIXED3:
                    case CG_FIXED4:
                        JS_ASSERT( p->resIndex < JSGCM_VTXPRG_MAX_CONST );                        
                        GCM_FUNC( cellGcmSetVertexProgramParameterBlock, p->resIndex, 1, value ); 
                        break;
                    case CG_HALF4x4:
                    case CG_INT4x4:
                    case CG_BOOL4x4:
                    case CG_FIXED4x4:
                        // set 4 consts
                        JS_ASSERT(( p + 1 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 2 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 3 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 4 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        
                        {
                            GLfloat v2[16];
                            v2[0] = value[0];v2[1] = value[4];v2[2] = value[8];v2[3] = value[12];
                            v2[4] = value[1];v2[5] = value[5];v2[6] = value[9];v2[7] = value[13];
                            v2[8] = value[2];v2[9] = value[6];v2[10] = value[10];v2[11] = value[14];
                            v2[12] = value[3];v2[13] = value[7];v2[14] = value[11];v2[15] = value[15];
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 1 )->resIndex, 1, v2 + 0 ); // GCM_PORT_TESTED [Cedric]
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 2 )->resIndex, 1, v2 + 4 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 3 )->resIndex, 1, v2 + 8 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 4 )->resIndex, 1, v2 + 12 );
                        }
                        break;
                    case CG_HALF3x3:
                    case CG_INT3x3:
                    case CG_BOOL3x3:
                    case CG_FIXED3x3:
                        // set 3 consts
                        JS_ASSERT(( p + 1 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 2 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        JS_ASSERT(( p + 3 )->resIndex < JSGCM_VTXPRG_MAX_CONST );
                        {
                            GLfloat v2[12];
                            v2[0] = value[0];v2[1] = value[3];v2[2] = value[6];v2[3] = 0;
                            v2[4] = value[1];v2[5] = value[4];v2[6] = value[7];v2[7] = 0;
                            v2[8] = value[2];v2[9] = value[5];v2[10] = value[8];v2[11] = 0;
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 1 )->resIndex, 1, v2 + 0 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 2 )->resIndex, 1, v2 + 4 );
                            GCM_FUNC( cellGcmSetVertexProgramParameterBlock, ( p + 3 )->resIndex, 1, v2 + 8 );
                        }
                        break;
                    default:
                        JS_ASSERT( 0 );
                        break;
                }
            }
            break;


        case CG_PROFILE_FP40:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            JS_ASSERT( p->var == CG_UNIFORM );
            switch ( p->type )
            {
                case CG_FLOAT:
                case CG_FLOAT1:
                case CG_FLOAT2:
                case CG_FLOAT3:
                case CG_FLOAT4:
                    case CG_HALF:
                    case CG_HALF1:
                    case CG_HALF2:
                    case CG_HALF3:
                    case CG_HALF4:
                    case CG_INT:
                    case CG_INT1:
                    case CG_INT2:
                    case CG_INT3:
                    case CG_INT4:
                    case CG_BOOL:
                    case CG_BOOL1:
                    case CG_BOOL2:
                    case CG_BOOL3:
                    case CG_BOOL4:
                    case CG_FIXED:
                    case CG_FIXED1:
                    case CG_FIXED2:
                    case CG_FIXED3:
                    case CG_FIXED4:
                {
                    // set embedded constants
                    CgBinaryEmbeddedConstantOffset offset = p->embeddedConst;
                    if ( offset && ucodeId != GMM_ERROR )
                    {
                        CgBinaryEmbeddedConstant *ec = ( CgBinaryEmbeddedConstant* )(( char* )prg + offset );
                        unsigned int j;
                        GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );

                        for ( j = 0; j < ec->ucodeCount; j++ )
                        {
                            GLuint dst = gmmIdToOffset(ucodeId) + ec->ucodeOffset[j];
							void *pointer=NULL;
							GCM_FUNC( cellGcmSetInlineTransferPointer, dst, 4, &pointer);
							float *fp = (float*)pointer;
							const float *src = (const float*)value;
							fp[0] = cellGcmSwap16Float32(src[0]);
							fp[1] = cellGcmSwap16Float32(src[1]);
							fp[2] = cellGcmSwap16Float32(src[2]);
							fp[3] = cellGcmSwap16Float32(src[3]);
                        }
                    }
                }
                break;
                default:
                    JS_ASSERT( 0 );
                    break;
            }
            break;

        default:
            JS_ASSERT( 0 );
            break;
    }
}

void static inline _jsFifoGlProgramParameterfvVP( const _CGprogram *program, const CgParameterEntry *parameterEntry, const GLfloat *value )
{
    JS_ASSERT(( parameterEntry->flags & CGPV_MASK ) == CGPV_CONSTANT || ( parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM );
   
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
    JS_ASSERT( parameterResource->resource != ( unsigned short ) - 1 );
    if ( parameterResource->resource != ( unsigned short ) - 1 )
    {
        //JS_ASSERT(parameter->_resource == CG_C);
        JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
        switch ( parameterResource->type )
        {
            case CG_FLOAT:
            case CG_FLOAT1:
            case CG_FLOAT2:
            case CG_FLOAT3:
            case CG_FLOAT4:
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 1, value ); // GCM_PORT_TESTED [Cedric]
                break;
            case CG_FLOAT4x4:
                // set 4 consts
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 1 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 2 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 3 < JSGCM_VTXPRG_MAX_CONST );
                {
                    GLfloat v2[16];
                    v2[0] = value[0];v2[1] = value[4];v2[2] = value[8];v2[3] = value[12];
                    v2[4] = value[1];v2[5] = value[5];v2[6] = value[9];v2[7] = value[13];
                    v2[8] = value[2];v2[9] = value[6];v2[10] = value[10];v2[11] = value[14];
                    v2[12] = value[3];v2[13] = value[7];v2[14] = value[11];v2[15] = value[15];
                    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 4, v2 ); // GCM_PORT_TESTED [Cedric]
                }
                break;
            case CG_FLOAT3x3:
                // set 3 consts
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 1 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 2 < JSGCM_VTXPRG_MAX_CONST );
                {
                    GLfloat v2[12];
                    v2[0] = value[0];v2[1] = value[3];v2[2] = value[6];v2[3] = 0;
                    v2[4] = value[1];v2[5] = value[4];v2[6] = value[7];v2[7] = 0;
                    v2[8] = value[2];v2[9] = value[5];v2[10] = value[8];v2[11] = 0;
                    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 3, v2 );
                }
                break;
                    case CG_HALF:
                    case CG_HALF1:
                    case CG_HALF2:
                    case CG_HALF3:
                    case CG_HALF4:
                    case CG_INT:
                    case CG_INT1:
                    case CG_INT2:
                    case CG_INT3:
                    case CG_INT4:
                    case CG_BOOL:
                    case CG_BOOL1:
                    case CG_BOOL2:
                    case CG_BOOL3:
                    case CG_BOOL4:
                    case CG_FIXED:
                    case CG_FIXED1:
                    case CG_FIXED2:
                    case CG_FIXED3:
                    case CG_FIXED4:
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 1, value ); // GCM_PORT_TESTED [Cedric]
                break;
                    case CG_HALF4x4:
                    case CG_INT4x4:
                    case CG_BOOL4x4:
                    case CG_FIXED4x4:
                // set 4 consts
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 1 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 2 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 3 < JSGCM_VTXPRG_MAX_CONST );
                {
                    GLfloat v2[16];
                    v2[0] = value[0];v2[1] = value[4];v2[2] = value[8];v2[3] = value[12];
                    v2[4] = value[1];v2[5] = value[5];v2[6] = value[9];v2[7] = value[13];
                    v2[8] = value[2];v2[9] = value[6];v2[10] = value[10];v2[11] = value[14];
                    v2[12] = value[3];v2[13] = value[7];v2[14] = value[11];v2[15] = value[15];
                    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 4, v2 ); // GCM_PORT_TESTED [Cedric]
                }
                break;
                    case CG_HALF3x3:
                    case CG_INT3x3:
                    case CG_BOOL3x3:
                    case CG_FIXED3x3:
                // set 3 consts
                JS_ASSERT( parameterResource->resource < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 1 < JSGCM_VTXPRG_MAX_CONST );
                JS_ASSERT( parameterResource->resource + 2 < JSGCM_VTXPRG_MAX_CONST );
                {
                    GLfloat v2[12];
                    v2[0] = value[0];v2[1] = value[3];v2[2] = value[6];v2[3] = 0;
                    v2[4] = value[1];v2[5] = value[4];v2[6] = value[7];v2[7] = 0;
                    v2[8] = value[2];v2[9] = value[5];v2[10] = value[8];v2[11] = 0;
                    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, parameterResource->resource, 3, v2 );
                }
                break;
            default:
                JS_ASSERT( 0 );
                break;
        }
    }
}


void _jsPrepareFPSharedPushBuffer( _CGprogram *program, jsFifoData * &jsGcmCurrent );

#define VERTEX_PROFILE_INDEX 0
#define FRAGMENT_PROFILE_INDEX 1
int _jsGcmGenerateProgram( _CGprogram *program, int profileIndex, const CgProgramHeader *programHeader, const void *ucode,
                           const CgParameterTableHeader *parameterHeader, const CgParameterEntry *parameterEntries,
                           const char *stringTable, const float *defaultValues );

#endif
