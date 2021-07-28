/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmCgUtils.cpp
 */

#include <JSGCM/JSGcmUtils.h>
#include <string.h>

// support automatic endian flipping (makes the driver bigger)

/* changes the endianness of a cg binary program
**
** notes:
** - make sure this stays in sync with CgBinaryProgram!
*/

static void _jsGcmFlipEndianness( CGprogram prog )
{

    CgBinaryProgram *prg = ( CgBinaryProgram* ) prog;
    CGprofile profile = ( CGprofile ) _jsGcmSwapGLuint_8(( GLuint )prg->profile );
    unsigned int i;

    JS_ASSERT(( profile == CG_PROFILE_SCE_FP_TYPEB ) || ( profile == CG_PROFILE_SCE_VP_TYPEB ) ||
              ( profile == CG_PROFILE_SCE_VP_TYPEC ) || ( profile == CG_PROFILE_SCE_FP_TYPEC ) ||
              ( profile == CG_PROFILE_SCE_VP_RSX ) || ( profile == CG_PROFILE_SCE_FP_RSX ) );

    // flip endianness
    prg->profile                = ( CGprofile )_jsGcmSwapGLuint_8( prg->profile );
    prg->binaryFormatRevision   = _jsGcmSwapGLuint_8( prg->binaryFormatRevision );
    prg->totalSize              = _jsGcmSwapGLuint_8( prg->totalSize );
    prg->parameterCount         = _jsGcmSwapGLuint_8( prg->parameterCount );
    prg->parameterArray         = _jsGcmSwapGLuint_8( prg->parameterArray );
    prg->program                = _jsGcmSwapGLuint_8( prg->program );
    prg->ucodeSize              = _jsGcmSwapGLuint_8( prg->ucodeSize );
    prg->ucode                  = _jsGcmSwapGLuint_8( prg->ucode );

    // flip vertex program
    if (( profile == CG_PROFILE_SCE_VP_TYPEB ) ||
            ( profile == CG_PROFILE_SCE_VP_TYPEC ) ||
            ( profile == CG_PROFILE_SCE_VP_RSX ) )
    {
        CgBinaryVertexProgram *vs = ( CgBinaryVertexProgram* )(( char* )prg + prg->program );
        vs->instructionCount      = _jsGcmSwapGLuint_8( vs->instructionCount );
        vs->instructionSlot       = _jsGcmSwapGLuint_8( vs->instructionSlot );
        vs->registerCount         = _jsGcmSwapGLuint_8( vs->registerCount );
        vs->attributeInputMask    = _jsGcmSwapGLuint_8( vs->attributeInputMask );
        vs->attributeOutputMask   = _jsGcmSwapGLuint_8( vs->attributeOutputMask );
        vs->userClipMask          = _jsGcmSwapGLuint_8( vs->userClipMask );

        // flip ucode
        for ( i = 0; i < prg->ucodeSize; i += 16 )
        {
            GLuint *inst = ( GLuint* )(( char* )prg + prg->ucode + i );
            inst[0] = _jsGcmSwapGLuint_8( inst[0] );
            inst[1] = _jsGcmSwapGLuint_8( inst[1] );
            inst[2] = _jsGcmSwapGLuint_8( inst[2] );
            inst[3] = _jsGcmSwapGLuint_8( inst[3] );
        }
    }

    // flip fragment program
    if (( profile == CG_PROFILE_SCE_FP_TYPEB ) ||
            ( profile == CG_PROFILE_SCE_FP_TYPEC ) ||
            ( profile == CG_PROFILE_SCE_FP_RSX ) )
    {
        CgBinaryFragmentProgram *ps = ( CgBinaryFragmentProgram* )(( char* )prg + prg->program );
        ps->instructionCount    = _jsGcmSwapGLuint_8( ps->instructionCount );
        ps->attributeInputMask  = _jsGcmSwapGLuint_8( ps->attributeInputMask );
        ps->partialTexType      = _jsGcmSwapGLuint_8( ps->partialTexType );
        ps->texCoordsInputMask  = _jsGcmSwapGLushort_8( ps->texCoordsInputMask );
        ps->texCoords2D         = _jsGcmSwapGLushort_8( ps->texCoords2D );

        // flip ucode
        for ( i = 0; i < prg->ucodeSize; i += 16 )
        {
            GLuint *inst = ( GLuint* )(( char* )prg + prg->ucode + i );
            inst[0] = _jsGcmSwapGLuint_16( _jsGcmSwapGLuint_8( inst[0] ) );
            inst[1] = _jsGcmSwapGLuint_16( _jsGcmSwapGLuint_8( inst[1] ) );
            inst[2] = _jsGcmSwapGLuint_16( _jsGcmSwapGLuint_8( inst[2] ) );
            inst[3] = _jsGcmSwapGLuint_16( _jsGcmSwapGLuint_8( inst[3] ) );
        }
    }

    // flip each parameter struct
    for ( i = 0; i < prg->parameterCount; i++ )
    {
        CgBinaryParameterOffset arrayOffset = prg->parameterArray;
        CgBinaryParameter *parray = ( CgBinaryParameter* )(( char* )prog + arrayOffset );
        CgBinaryParameter *p = &parray[i];

        p->type             = ( CGtype )_jsGcmSwapGLuint_8( p->type );
        p->res              = ( CGresource )_jsGcmSwapGLuint_8( p->res );
        p->var              = ( CGenum )_jsGcmSwapGLuint_8( p->var );
        p->resIndex         = _jsGcmSwapGLuint_8( p->resIndex );
        p->name             = _jsGcmSwapGLuint_8( p->name );
        p->defaultValue     = _jsGcmSwapGLuint_8( p->defaultValue );
        p->embeddedConst    = _jsGcmSwapGLuint_8( p->embeddedConst );
        p->semantic         = _jsGcmSwapGLuint_8( p->semantic );
        p->direction        = ( CGenum )_jsGcmSwapGLuint_8( p->direction );
        p->paramno          = _jsGcmSwapGLuint_8( p->paramno );
        p->isReferenced     = _jsGcmSwapGLuint_8( p->isReferenced );
        p->isShared         = _jsGcmSwapGLuint_8( p->isShared );

        if ( p->defaultValue )
        {
            CgBinaryFloatOffset offset = p->defaultValue;
            GLuint *defValue = ( GLuint* )(( char* )prg + offset );
            unsigned int j;
            JS_ASSERT( sizeof( float ) == sizeof( GLuint ) );
            for ( j = 0; j < 4; j++ )
            {
                defValue[j] = _jsGcmSwapGLuint_8( defValue[j] );
            }
        }
        if ( p->embeddedConst )
        {
            CgBinaryEmbeddedConstant *ec = ( CgBinaryEmbeddedConstant* )(( char* )prg + p->embeddedConst );
            unsigned int j;
            ec->ucodeCount = _jsGcmSwapGLuint_8( ec->ucodeCount );
            for ( j = 0; j < ec->ucodeCount; j++ )
            {
                ec->ucodeOffset[j] = _jsGcmSwapGLuint_8( ec->ucodeOffset[j] );
            }
        }
    }
}


/* initialize a binary cg program. If the endianness doesn't match all data is
** swapped as needed.
**
** notes:
**  - not very fast, but shader loading is usually a one time job and only
**    if the endianness mismatches (the default should be big endian)
**  - the prog will be modified!
*/
void _jsGcmInitializeBinaryCgProgram( CGprogram prog )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* ) prog;
    CGprofile profile;
    unsigned int i;

    // not matching the current endian mode ?
    profile = prg->profile;
    if (( profile != CG_PROFILE_SCE_FP_TYPEB ) && ( profile != CG_PROFILE_SCE_VP_TYPEB ) &&
            ( profile != CG_PROFILE_SCE_FP_TYPEC ) && ( profile != CG_PROFILE_SCE_VP_TYPEC ) &&
            ( profile != CG_PROFILE_SCE_FP_RSX ) && ( profile != CG_PROFILE_SCE_VP_RSX ) )
    {
        _jsGcmFlipEndianness( prog );
    }

    // must be supported profile
    profile = prg->profile;
    JS_ASSERT(( profile == CG_PROFILE_SCE_FP_TYPEB ) || ( profile == CG_PROFILE_SCE_VP_TYPEB ) ||
              ( profile == CG_PROFILE_SCE_VP_TYPEC ) || ( profile == CG_PROFILE_SCE_FP_TYPEC ) ||
              ( profile == CG_PROFILE_SCE_VP_RSX ) || ( profile == CG_PROFILE_SCE_FP_RSX ) );

    // set embedded constants to default
    for ( i = 0; i < prg->parameterCount; i++ )
    {
        CgBinaryParameter *parray = ( CgBinaryParameter* )(( char* )prg + prg->parameterArray );
        CgBinaryParameter *param = &parray[i];

        // set embedded constants to default
        if ( param->embeddedConst && param->defaultValue && prg->ucode )
        {
            CgBinaryEmbeddedConstant *ec = ( CgBinaryEmbeddedConstant* )(( char* ) prg + param->embeddedConst );
            float *defValue = ( float* )(( char* )prg + param->defaultValue );
            GLuint *ucode    = ( GLuint* )(( char* )prg + prg->ucode );
            unsigned int j;
            for ( j = 0; j < ec->ucodeCount; j++ )
            {
                float *src = defValue;
                GLfloat *dst = ( GLfloat* )(( char* )ucode + ec->ucodeOffset[j] );

#if defined(JSGCM_BIG_ENDIAN)
                dst[0] = _jsGcmSwapGLfloat_16( src[0] );
                dst[1] = _jsGcmSwapGLfloat_16( src[1] );
                dst[2] = _jsGcmSwapGLfloat_16( src[2] );
                dst[3] = _jsGcmSwapGLfloat_16( src[3] );
#else
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
#endif
            }
        }
    }
}

/*
**
**
*/
void _jsGcmGetUCode( CGprogram prog, char **pUCode, unsigned int *pUCodeSize )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* ) prog;
    if ( pUCode )
    {
        char *ucode = ( char* )prg + prg->ucode;
        *pUCode = ucode;
    }
    if ( pUCodeSize )
    {
        *pUCodeSize = prg->ucodeSize;
    }
}

/*
**
*/
CGprofile _jsGcmFifoCgGetProgramProfile( CGprogram prog )
{
    CgBinaryProgram *p = ( CgBinaryProgram* ) prog;
    JS_ASSERT( p != NULL );

    return p->profile;
}

/*
**
** note:
**  - this method is SLOW
*/
CGparameter _jsGcmFifoCgGetNamedParameter( CGprogram prog, const char *name )
{
    CGparameter r = 0;
    CgBinaryProgram *p = ( CgBinaryProgram* ) prog;

    if ( p )
    {
        CgBinaryParameter *parray = ( CgBinaryParameter* )(( char* )prog + p->parameterArray );
        unsigned int i;
        for ( i = 0; i < p->parameterCount; i++, parray++ )
        {
            char *paramName = ( char* )(( char* )prog + parray->name );
            if ( strcmp( name, paramName ) == 0 )
            {
                return ( CGparameter ) parray;
            }
        }
    }
    return r;
}

/*
**
**
*/
CGparameter _jsGcmFifoCgGetFirstLeafParameter( CGprogram prog, CGenum name_space )
{
    CGparameter result = NULL;
    CgBinaryProgram *prg = ( CgBinaryProgram* ) prog;
    JS_ASSERT( name_space == CG_PROGRAM );
    if ( prg && prg->parameterCount )
    {
        CgBinaryParameter *parameterArray = ( CgBinaryParameter* )(( char* )prg + prg->parameterArray );
        result = ( CGparameter ) parameterArray;
    }
    return result;
}

/*
**
**
*/
CGparameter _jsGcmFifoCgGetNextLeafParameter( CGprogram prog, CGparameter current )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )current;
    CgBinaryProgram *prg = ( CgBinaryProgram* )prog;
    CGparameter result = NULL;
    if ( prg && prg->parameterArray )
    {
        // reached end of parameter array ?
        CgBinaryParameter *parameterArray = ( CgBinaryParameter* )(( char* )prg + prg->parameterArray );
        CgBinaryParameter *end = parameterArray + prg->parameterCount;
        JS_ASSERT(( void* )p >= ( void* )parameterArray );
        JS_ASSERT(( void* )p < ( void* )end );
        p++;
        if (( void* )p < ( void* )end )
        {
            result = ( CGparameter )p;
        }
    }
    return result;
}

/*
**
**
*/
CGtype _jsGcmFifoCgGetParameterType( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->type;
}

/*
**
**
*/
CGresource _jsGcmFifoCgGetParameterResource( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->res;
}

/*
**
**
*/
unsigned int _jsGcmFifoCgGetParameterResourceIndex( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->resIndex;
}

/*
**
**
*/
CGenum _jsGcmFifoCgGetParameterVariability( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->var;
}

/*
**
**
*/
CGenum _jsGcmFifoCgGetParameterDirection( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->direction;
}

/*
**
**
*/
const char *_jsGcmFifoCgGetParameterSemantic( CGprogram prog, CGparameter param )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* )prog;
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return ( const char * )prg + p->semantic;
}


/*
**
**
*/
const char *_jsGcmFifoCgGetParameterName( CGprogram prog, CGparameter param )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* )prog;
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    if ( p )
    {
        const char *name = ( const char* )(( char* )prg + p->name );
        return name;
    }
    else
    {
        return NULL;
    }
}

/*
**
**
*/
const float *_jsGcmFifoCgGetParameterValues( CGprogram prog, CGparameter param )
{
    CgBinaryProgram *prg = ( CgBinaryProgram* )prog;
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    if ( p && p->defaultValue )
    {
        const float *defValue = ( const float* )(( char* )prg + p->defaultValue );
        return defValue;
    }
    else
    {
        return NULL;
    }
}

/*
**
**
*/
int  _jsGcmFifoCgGetParameterIndex( CGprogram prog, CGparameter param )
{
    CgBinaryParameter *p = ( CgBinaryParameter* )param;
    JS_ASSERT( param );
    return p->paramno;
}
