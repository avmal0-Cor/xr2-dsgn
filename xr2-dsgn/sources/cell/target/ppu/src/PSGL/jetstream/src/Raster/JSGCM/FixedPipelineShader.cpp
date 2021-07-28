/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdio.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/ShaderPool.h>
#include <JSGCM/JSGcmFPShaderMan.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmShader.h>


#include <PSGL/Utils.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <Cg/cg.h>
#include <Cg/cgBinary.h>
#include <string.h>


#define NV_FP_PROFILE CG_PROFILE_SCE_FP_TYPEB

#define DUMP_MISSING_SHADERS

void _jsGcmFPShaderManagerInit( jsGcmFPShaderManager *manager )
{
    int i;
    for ( i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        manager->currentState.loadType[i] = _FP_STATE_LOAD_NONE;
        manager->currentState.texEnv[i] = _FP_STATE_TEXENV_MODULATE;
        manager->currentState.texFormat[i] = _FP_STATE_FORMAT_RGBA;
    }
    manager->currentState.fog = _FP_STATE_FOG_NONE;
    manager->dirty = GL_TRUE;
    manager->lastShader = NULL;
#ifdef DUMP_MISSING_SHADERS
    _JS_LIST_INIT( manager->missingStates, jsGcmFPShaderState, 0 );
#endif
}

#ifdef DUMP_MISSING_SHADERS
#define STRING_SIZE 256

static void _jsGcmFPShaderManagerDumpState( jsGcmFPShaderState *state )
{
    char tempString[STRING_SIZE];
    int l = 0;
    l += snprintf( tempString + l, STRING_SIZE - l, "STATE" );
    JS_ASSERT( l < STRING_SIZE );
    int i;
    for ( i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        l += snprintf( tempString + l, STRING_SIZE - l, ":%d:%d:%d", state->loadType[i], state->texEnv[i], state->texFormat[i] );
        JS_ASSERT( l < STRING_SIZE );
    }
    l += snprintf( tempString + l, STRING_SIZE - l, ":%d\n", state->fog );
    JS_ASSERT( l < STRING_SIZE );
    _jsReportExtra( PSGL_REPORT_MISSING_STATE, "%s", tempString );
}

#undef STRING_SIZE
#endif


void _jsGcmFPShaderManagerDestroy( jsGcmFPShaderManager *manager )
{
    int i;
    for ( i = 0;i < HASH_SIZE;++i )
    {
        jsGcmFPShader *shader = manager->shaders[i];
        while ( shader )
        {
            // XXX The default jsGcmFPShader instances are actually shared among
            // managers/drivers/devices (shaders loaded dynamically are not shared).
            // This can cause a problem when multiple devices are in use,
            // particularly when they are destroyed.  We work around this by
            // reference counting jsGcmFPShader objects.
            //
            // The linked list pointers for jsGcmFPShaderManager are inside the
            // object, which is ugly because they are simultaneously in separate
            // lists (each manager has its own array of lists).  We rely on the fact
            // that the default shader pool is always loaded first, so the list
            // pointers are always the same among jsGcmFPShaderManager instances
            // *and* additions to the list are always prepended so that the pointers
            // will not be disturbed.
            if ( --shader->refCount )
                break;

            _jsGcmUnloadFPShader( shader->nvShader );
            jsGcmFPShader *next = shader->next;
            shader->next = NULL;
            shader = next;
        }
        manager->shaders[i] = NULL;
    }
#ifdef DUMP_MISSING_SHADERS
    _JS_LIST_DESTROY( manager->missingStates, jsGcmFPShaderState );
#endif
}

void _jsGcmFPAddShadersFromPool( jsGcmFPShaderManager *manager, jsCgShaderPool *pool )
{
    int i;
    for ( i = 0;i < pool->shadersCount;++i )
    {
        jsGcmFPShader *shader = pool->shaders + i;

        // XXX Don't remove the old shader.  The new shader is prepended to
        // the hash list, so we will find it first.  This avoids the issue
        // when the old shader is a default shader (actually when the shader
        // that precedes it in the list is *also* a default shader) and
        // changing the linked list pointers is visible by all shader
        // managers.
        jsGcmFPShader *old_shader = _jsGcmFPFindShader( manager, &shader->state );
        if ( old_shader )
            _jsGcmFPRemoveShader( manager, old_shader );
        _jsGcmFPAddShader( manager, shader );
    }
}

static GLuint jsGcmHashState( const jsGcmFPShaderState *state )
{
    GLuint hash = 0;
    int i;
    for ( i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        GLuint texState = (( state->texFormat[i] << 4 ) + ( state->texEnv[i] ) ) * state->loadType[i];
        hash = (( hash * 11 ) + texState ) % HASH_SIZE;
    }
    hash = (( hash * 11 ) + state->fog ) % HASH_SIZE;
    return hash;
}

static GLboolean jsGcmCompareState( const jsGcmFPShaderState *state1, const jsGcmFPShaderState *state2 )
{
    int i;
    for ( i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        if ( state1->loadType[i] != state2->loadType[i] ) return GL_FALSE;
        if ( state1->loadType[i] == _FP_STATE_LOAD_NONE ) continue; // if none, we don't care about env/format
        if ( state1->texEnv[i] != state2->texEnv[i] ) return GL_FALSE;
        if ( state1->texFormat[i] != state2->texFormat[i] ) return GL_FALSE;
    }
    if ( state1->fog != state2->fog ) return GL_FALSE;
    return GL_TRUE;
}

void _jsGcmFPAddShader( jsGcmFPShaderManager *manager, jsGcmFPShader *shader )
{
    GLuint hash = jsGcmHashState( &shader->state );
    JS_ASSERT( hash < HASH_SIZE );
    JS_ASSERT( !shader->next );
    shader->next = manager->shaders[hash];
    manager->shaders[hash] = shader;

    // The +2 below is because at this point 0 and 1 are already used for dummyShader and oneTextureShader
    _jsAllocSetHint( _JS_ALLOCTYPE_FIXED_PIPELINE_PROGRAM, hash + 2 );
    _jsGcmLoadFPShader( shader->nvShader );
    _jsAllocClearHint();

    // XXX The default jsGcmFPShader instances are actually shared among
    // managers/drivers/devices (shaders loaded dynamically are not shared).
    // This can cause a problem when multiple devices are in use,
    // particularly when they are destroyed.  We work around this by
    // reference counting jsGcmFPShader objects.
    //
    // The linked list pointers for jsGcmFPShaderManager are inside the
    // object, which is ugly because they are simultaneously in separate
    // lists (each manager has its own array of lists).  We rely on the fact
    // that the default shader pool is always loaded first, so the list
    // pointers are always the same among jsGcmFPShaderManager instances
    // *and* additions to the list are always prepended so that the pointers
    // will not be disturbed.
    ++shader->refCount;
}

void _jsGcmFPRemoveShader( jsGcmFPShaderManager *manager, jsGcmFPShader *shader )
{
    JS_ASSERT( shader );
    GLuint hash = jsGcmHashState( &shader->state );
    JS_ASSERT( hash < HASH_SIZE );
    jsGcmFPShader *s = manager->shaders[hash];
    if ( shader == s )
    {
        manager->shaders[hash] = s->next;
        shader->next = NULL;
    }
    else for ( ;s->next;s = s->next )
        {
            if ( s->next == shader )
            {
                s->next = shader->next;
                shader->next = NULL;
                break;
            }
        }

    // don't unload from GPU unless no other references
    // XXX The default jsGcmFPShader instances are actually shared among
    // managers/drivers/devices (shaders loaded dynamically are not shared).
    // This can cause a problem when multiple devices are in use,
    // particularly when they are destroyed.  We work around this by
    // reference counting jsGcmFPShader objects.
    if ( --shader->refCount == 0 )
        _jsGcmUnloadFPShader( shader->nvShader );
}

jsGcmFPShader *_jsGcmFPFindShader( jsGcmFPShaderManager *manager, const jsGcmFPShaderState *state )
{
    GLuint hash = jsGcmHashState( state );
    JS_ASSERT( hash < HASH_SIZE );
    for ( jsGcmFPShader *shader = manager->shaders[hash];shader;shader = shader->next )
    {
        if ( jsGcmCompareState( &shader->state, state ) ) return shader;
    }
    return NULL;
}

jsGcmFPShader *_jsGcmFPFindCurrentShader( jsGcmFPShaderManager *manager )
{
    if ( manager->dirty )
    {
        manager->lastShader = _jsGcmFPFindShader( manager, &manager->currentState );
        manager->dirty = GL_FALSE;
    }
#ifdef DUMP_MISSING_SHADERS
    if ( !manager->lastShader )
    {
        // we didn't find the shader associated with the given state.
        // add it to the list of missing shaders.
        int i;
        for ( i = 0;i < manager->missingStatesCount;++i )
        {
            if ( jsGcmCompareState( manager->missingStates + i, &manager->currentState ) ) break;
        }
        if ( i >= manager->missingStatesCount )
        {
            _jsGcmFPShaderManagerDumpState( &manager->currentState );
            _JS_LIST_APPEND( manager->missingStates, jsGcmFPShaderState, manager->currentState );
        }
    }
#endif
    return manager->lastShader;
}

GLboolean _jsGcmFPLoadCurrentShader( jsGcmFPShaderManager *manager )
{
    jsGcmFPShader *shader = _jsGcmFPFindCurrentShader( manager );
#ifndef JS_NO_ERROR_CHECK
    if ( !shader )
    {
        _jsSetError( GL_INVALID_VALUE );
        return GL_FALSE;
    }
#endif

    jsGcmShader *nvShader = shader->nvShader;
    for ( int i = 0;i < _JS_MAX_TEXTURE_UNITS;++i )
    {
        if ( manager->currentState.texEnv[i] != _FP_STATE_TEXENV_BLEND ) continue;
        if ( manager->currentState.loadType[i] == _FP_STATE_LOAD_NONE ) continue;
        if ( !shader->ccParam[i] ) continue;
        _jsGcmSetParameter4fv( nvShader, shader->ccParam[i], manager->envColor[i] );
    }
    if ( manager->currentState.fog != _FP_STATE_FOG_NONE )
    {
        if ( shader->fogParam ) _jsGcmSetParameter4fv( nvShader, shader->fogParam, manager->fogParam );
        if ( shader->fogColor ) _jsGcmSetParameter4fv( nvShader, shader->fogColor, manager->fogColor );
    }
    JS_ASSERT(nvShader->loadAddressId != GMM_ERROR);
    _jsGcmSetNativeCgFragmentProgram(&(nvShader->program), nvShader->loadAddressId);
    _jsGetGcmDriver()->fpLoadProgramId = nvShader->loadAddressId;
    _jsGetGcmDriver()->fpLoadProgramOffset = 0;
    return GL_TRUE;
}

void _jsGcmFPShaderManagerSetTextureMode( jsGcmFPShaderManager *manager, GLuint unit, GLenum mode )
{
    JS_ASSERT( unit < _JS_MAX_TEXTURE_UNITS );
    GLubyte loadType;
    switch ( mode )
    {
        case GL_TEXTURE_CUBE_MAP:
            loadType = _FP_STATE_LOAD_CUBE;
            break;
        case GL_TEXTURE_3D:
            loadType = _FP_STATE_LOAD_3D;
            break;
        case GL_TEXTURE_2D:
            loadType = _FP_STATE_LOAD_2D;
            break;
        default:
            loadType = _FP_STATE_LOAD_NONE;
            break;
    }
    manager->currentState.loadType[unit] = loadType;
    manager->dirty = GL_TRUE;
}

void _jsGcmFPShaderManagerSetTextureEnvMode( jsGcmFPShaderManager *manager, GLuint unit, GLenum envMode, GLfloat *color )
{
    JS_ASSERT( unit < _JS_MAX_TEXTURE_UNITS );
    GLubyte mode;
    switch ( envMode )
    {
        case GL_REPLACE:
            mode = _FP_STATE_TEXENV_REPLACE;
            break;
        case GL_DECAL:
            mode = _FP_STATE_TEXENV_DECAL;
            break;
        case GL_BLEND:
            mode = _FP_STATE_TEXENV_BLEND;
            break;
        case GL_ADD:
            mode = _FP_STATE_TEXENV_ADD;
            break;
        default:
            mode = _FP_STATE_TEXENV_MODULATE;
            break;
    }
    manager->currentState.texEnv[unit] = mode;
    manager->envColor[unit][0] = color[0];
    manager->envColor[unit][1] = color[1];
    manager->envColor[unit][2] = color[2];
    manager->envColor[unit][3] = color[3];
    manager->dirty = GL_TRUE;
}

void _jsGcmFPShaderManagerSetTextureFormat( jsGcmFPShaderManager *manager, GLuint unit, GLenum format )
{
    JS_ASSERT( unit < _JS_MAX_TEXTURE_UNITS );
    GLubyte texFormat;
    switch ( format )
    {
        case GL_RGB:
            texFormat = _FP_STATE_FORMAT_RGB;
            break;
        case GL_LUMINANCE:
            texFormat = _FP_STATE_FORMAT_L;
            break;
        case GL_LUMINANCE_ALPHA:
            texFormat = _FP_STATE_FORMAT_LA;
            break;
        case GL_ALPHA:
            texFormat = _FP_STATE_FORMAT_A;
            break;
        case GL_INTENSITY:
            texFormat = _FP_STATE_FORMAT_I;
            break;
        default:
            texFormat = _FP_STATE_FORMAT_RGBA;
            break;
    }
    manager->currentState.texFormat[unit] = texFormat;
    manager->dirty = GL_TRUE;
}

void _jsGcmFPShaderManagerSetFog( jsGcmFPShaderManager *manager, GLenum mode, GLfloat *color, GLfloat start, GLfloat end, GLfloat density )
{
    GLubyte fog;
    switch ( mode )
    {
        case GL_LINEAR:
            fog = _FP_STATE_FOG_LINEAR;
            manager->fogParam[0] = 1.f / ( start - end );
            manager->fogParam[1] = end / ( end - start );
            break;
        case GL_EXP:
            fog = _FP_STATE_FOG_EXP;
            manager->fogParam[0] = density;
            break;
        case GL_EXP2:
            fog = _FP_STATE_FOG_EXP2;
            manager->fogParam[0] = density;
            break;
        default:
            fog = _FP_STATE_FOG_NONE;
            break;
    }
    manager->currentState.fog = fog;
    manager->fogColor[0] = color[0];
    manager->fogColor[1] = color[1];
    manager->fogColor[2] = color[2];
    manager->fogColor[3] = color[3];
    manager->dirty = GL_TRUE;
}

static char fp30psColorOnly[] =
    {
#include "fpColorOnly_40.h"
    };

static char fp30psTextureReplace[] =
    {
#include "fpOneTexture_40.h"
    };

static jsGcmFPShader shaderDefs[] =
    {
        {
            0,		// reference count
            NULL,
            {
                {
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                },
                {
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                },
                {
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                },
                _FP_STATE_FOG_NONE
            },
            NULL,
            { NULL, NULL, NULL, NULL, }, NULL, NULL,
        },
        {
            0,		// reference count
            NULL,
            {
                {
                    _FP_STATE_LOAD_2D,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                },
                {
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                },
                {
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                },
                _FP_STATE_FOG_NONE
            },
            NULL,
            { NULL, NULL, NULL, NULL, }, NULL, NULL,
        },
        {
            0,		// reference count
            NULL,
            {
                {
                    _FP_STATE_LOAD_2D,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                    _FP_STATE_LOAD_NONE,
                },
                {
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                    _FP_STATE_TEXENV_MODULATE,
                },
                {
                    _FP_STATE_FORMAT_L,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                    _FP_STATE_FORMAT_RGBA,
                },
                _FP_STATE_FOG_NONE
            },
            NULL,
            { NULL, NULL, NULL, NULL, }, NULL, NULL,
        },
    };

static jsCgShaderPool defaultShaders =
    {
        shaderDefs,
        sizeof( shaderDefs ) / sizeof( shaderDefs[0] ),
        sizeof( shaderDefs ) / sizeof( shaderDefs[0] ),
    };

static GLboolean initDone = GL_FALSE;

static jsGcmShader * dummyShader;
static jsGcmShader * oneTextureShader;
jsCgShaderPool *_jsGcmInitDefaultShaders()
{
    if ( !initDone )
    {
        // init the shaders with the static data.
        shaderDefs[0].nvShader = ( jsGcmShader * )jsMemalign( 16, sizeof( jsGcmShader ) - sizeof( CgBinaryProgram ) + sizeof( fp30psColorOnly ) );
        dummyShader = ( jsGcmShader * )jsMemalign( 16, sizeof( jsGcmShader ) - sizeof( CgBinaryProgram ) + sizeof( fp30psColorOnly ) );
        shaderDefs[1].nvShader = ( jsGcmShader * )jsMemalign( 16, sizeof( jsGcmShader ) - sizeof( CgBinaryProgram ) + sizeof( fp30psTextureReplace ) );
        oneTextureShader = ( jsGcmShader * )jsMemalign( 16, sizeof( jsGcmShader ) - sizeof( CgBinaryProgram ) + sizeof( fp30psTextureReplace ) );

        memcpy( &shaderDefs[0].nvShader->program, fp30psColorOnly, sizeof( fp30psColorOnly ) );
        memcpy( &dummyShader->program, fp30psColorOnly, sizeof( fp30psColorOnly ) );
        memcpy( &shaderDefs[1].nvShader->program, fp30psTextureReplace, sizeof( fp30psTextureReplace ) );
        memcpy( &oneTextureShader->program, fp30psTextureReplace, sizeof( fp30psTextureReplace ) );

        _jsGcmInitializeBinaryCgProgram(( CGprogram )&shaderDefs[0].nvShader->program );
        _jsGcmInitializeBinaryCgProgram(( CGprogram )&dummyShader->program );
        _jsGcmInitializeBinaryCgProgram(( CGprogram )&shaderDefs[1].nvShader->program );
        _jsGcmInitializeBinaryCgProgram(( CGprogram )&oneTextureShader->program );

        shaderDefs[0].nvShader->loadAddressId = GMM_ERROR;
        dummyShader->loadAddressId = GMM_ERROR;
        shaderDefs[1].nvShader->loadAddressId = GMM_ERROR;
        shaderDefs[2].nvShader = shaderDefs[1].nvShader;
        oneTextureShader->loadAddressId = GMM_ERROR;

        _jsGcmFPPrepareShader( shaderDefs );
        _jsGcmFPPrepareShader( shaderDefs + 1 );
        _jsGcmFPPrepareShader( shaderDefs + 2 );
        _jsAllocSetHint( _JS_ALLOCTYPE_FIXED_PIPELINE_PROGRAM, 0 );
        _jsGcmLoadFPShader( dummyShader ); 
        _jsAllocClearHint();
        _jsAllocSetHint( _JS_ALLOCTYPE_FIXED_PIPELINE_PROGRAM, 1 );
        _jsGcmLoadFPShader( oneTextureShader ); 
        _jsAllocClearHint();
        initDone = GL_TRUE;
    }
    return &defaultShaders;
}

jsGcmShader *_jsGcmFPGetDummyShader()
{
    JS_ASSERT( dummyShader );
    return dummyShader;
}

jsGcmShader *_jsGcmFPGetOneTextureShader()
{
    JS_ASSERT( oneTextureShader );
    return oneTextureShader;
}

void _jsGcmUninitDefaultShaders()
{
    if ( initDone )
    {
        jsFree( shaderDefs[0].nvShader );
        jsFree( dummyShader );
        jsFree( shaderDefs[1].nvShader );
        jsFree( oneTextureShader );
    }
    initDone = GL_FALSE;
}

