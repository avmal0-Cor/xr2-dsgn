/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdio.h>
#include <PSGL/psgl.h>
#include <JSGCM/JSGcmUtils.h>
#include <PSGL/Utils.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <JSGCM/ShaderPool.h>
#include <Cg/cg.h>
#include <Cg/cgBinary.h>
#include <string.h>

#define ALIGN_8(X) (X)=((X)+7)&~7

#if JS_ENDIAN == JS_BIG_ENDIAN
#define ENDIAN_32_1(X) (X)=endianSwapWord((X))
#else
#define ENDIAN_32_1(X) ((void)(X))
#endif

void _jsGcmFPPrepareShader( jsGcmFPShader *shader )
{
    int i;
    for ( i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        char pname[5];
        snprintf( pname, 5, _CC_PARAM_NAME"%d", i );
        shader->ccParam[i] = ( CgBinaryParameter * )_jsGcmFifoCgGetNamedParameter(( CGprogram ) & shader->nvShader->program, pname );
    }
    shader->fogParam = ( CgBinaryParameter * )_jsGcmFifoCgGetNamedParameter(( CGprogram ) & shader->nvShader->program, _FOG_PARAM_NAME );
    shader->fogColor = ( CgBinaryParameter * )_jsGcmFifoCgGetNamedParameter(( CGprogram ) & shader->nvShader->program, _FOG_COLOR_NAME );
}

void _jsGcmFPShaderPoolCreate( jsCgShaderPool *pool, int shaderCount )
{
    _JS_LIST_INIT( pool->shaders, jsGcmFPShader, shaderCount );
}

void _jsGcmFPShaderPoolFree( jsCgShaderPool *pool )
{
    for ( int i = 0;i < pool->shadersCount;++i )
    {
        jsFree( pool->shaders[i].nvShader );
    }
    _JS_LIST_DESTROY( pool->shaders, jsGcmFPShader );
}

int _jsGcmFPShaderPoolLoad( jsCgShaderPool *pool, const char *fileName )
{
    jsGcmFPShaderSerial *shadersSerial = 0;
    char *cgData = 0;
    FILE *file = fopen( fileName, "rb" );
    if ( !file )
    {
        printf( "could not load shader pool file %s\n", fileName );
        return -1;
    }
    jsCgShaderPoolHeader header;
    int r = fread( &header, sizeof( jsCgShaderPoolHeader ), 1, file );
    int error = 0;
    if ( r < 1 )
    {
        error = -1;
        goto fail1;
    }

    ENDIAN_32_1( header.magic );
    ENDIAN_32_1( header.version );
    ENDIAN_32_1( header.shaderCount );
    ENDIAN_32_1( header.cgSize );

    if (( header.magic != JS_SHADER_POOL_MAGIC ) ||
            ( header.version != JS_SHADER_POOL_VERSION ) ||
            ( header.shaderCount <= 0 ) ||
            ( header.cgSize <= 0 ) )
    {
        error = -2;
        goto fail1;
    }

    cgData = ( char * )jsMalloc( header.cgSize );
    if ( !cgData )
    {
        error = -3;
        goto fail1;
    }

    shadersSerial = ( jsGcmFPShaderSerial * )jsMalloc( header.shaderCount * sizeof( jsGcmFPShaderSerial ) );
    if ( !shadersSerial )
    {
        error = -3;
        goto fail2;
    }

    r = fread( shadersSerial, sizeof( jsGcmFPShaderSerial ), header.shaderCount, file );
    if ( r < ( int )header.shaderCount )
    {
        error = -4;
        goto fail3;
    }

    r = fread( cgData, header.cgSize, 1, file );
    if ( r < 1 )
    {
        error = -4;
        goto fail3;
    }

    pool->shadersCount = header.shaderCount;
    _JS_LIST_CHECK_CAPACITY( pool->shaders, jsGcmFPShader );
    unsigned int i;
    for ( i = 0;i < header.shaderCount;++i )
    {
        jsGcmFPShaderSerial *shaderSerial = shadersSerial + i;
        ENDIAN_32_1( shaderSerial->nvShader.programOffset );
        unsigned int offset = shaderSerial->nvShader.programOffset;
        if ( offset >= header.cgSize )
        {
            error = -4;
            goto fail4;
        }
        jsGcmFPShader* shader = pool->shaders + i;
        memset( shader, 0, sizeof( jsGcmFPShader ) );
        shader->state = shaderSerial->state;

        CgBinaryProgram *program = ( CgBinaryProgram * )( cgData + offset );
        _jsGcmInitializeBinaryCgProgram(( CGprogram )program );
        shader->nvShader = ( jsGcmShader * )jsMemalign( 16, sizeof( jsGcmShader ) + program->totalSize - sizeof( CgBinaryProgram ) );
        if ( !shader->nvShader )
        {
            error = -4;
            goto fail4;
        }
        shader->nvShader->loadAddressId = GMM_ERROR;
        CgBinaryProgram *nvProgram = &shader->nvShader->program;
        memcpy( nvProgram, program, program->totalSize );
        _jsGcmFPPrepareShader( shader );
    }
    jsFree( cgData );
    jsFree( shadersSerial );
    fclose( file );
    return 0;

fail4:
    for ( int i = pool->shadersCount - 1;i >= 0;--i )
    {
        if ( pool->shaders[i].nvShader ) jsFree( pool->shaders[i].nvShader );
        _JS_LIST_REMOVE( pool->shaders, jsGcmFPShader, i );
    }
fail3:
    jsFree( shadersSerial );
    shadersSerial = NULL;
fail2:
    jsFree( cgData );
    cgData = NULL;
fail1:
    fclose( file );
    return error;
}
