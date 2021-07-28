/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Various rasterization related functions, including driver initialization
 * and exit functions, and our main drawing function.
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Thread.h>
#include <PSGL/Utils.h>
#include <PSGL/PerfCounters.h>

#include <Cg/cg.h>
#include <Cg/CgCommon.h>
#include <Cg/cgBinary.h>

#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmFifo.h>
#include <JSGCM/JSGcmShader.h>
#include <JSGCM/ShaderPool.h>
#include <JSGCM/JSGcmDriver.h>
#include <JSGCM/JSGcmVertexAttribPointer.h>
#include <JSGCM/JSGcmDrawElements.h>

#include <PSGL/PerfCounters.h>
#include <string.h>

#include <cell/gcm.h>
using namespace cell::Gcm;


#define JS_ATTRIB_BUFFER_ALIGNMENT 16

// maximum size for drawing data
#define JSGCM_MAX_VERTEX_BUFFER_SIZE (2 << 20)
#define JSGCM_MAX_INDEX_BUFFER_SIZE (1 << 20)

char vp20FixedPipeline[] =
    {
#include "vpFixedPipeline_40.h"
    };

// Initialize the driver and setup the fixed function pipeline 
// shader and needed connections between GL state and the shader
void *_jsPlatformRasterInit()
{

    _jsGcmFifoGlFinish();
    jsGcmDriver *driver = ( jsGcmDriver * )jsMalloc( sizeof( jsGcmDriver ) );
#ifndef JS_NO_ERROR_CHECK
    if ( !driver )
    {
        JS_ASSERT( 0 );
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
#endif
    memset( driver, 0, sizeof( jsGcmDriver ) );

    driver->rt.aaSamples = 1;
    driver->rt.swizzled = JSGCM_FALSE;
    driver->rt.yInverted = JSGCM_TRUE;

    driver->invalidateVertexCache = GL_FALSE;

    driver->flushBufferCount = 0;
    driver->colorBufferMask = 0x1;

    driver->vsFixedPipeline = ( CGprogram )vp20FixedPipeline;
    _jsGcmInitializeBinaryCgProgram( driver->vsFixedPipeline );

    // fixed function parameter handles
#define _JS_CGPARAM(NAME) \
	driver->NAME =_jsGcmFifoCgGetNamedParameter(driver->vsFixedPipeline,#NAME)

    _JS_CGPARAM( viewXform );
    _JS_CGPARAM( projXform );
    _JS_CGPARAM( normXform );
    _JS_CGPARAM( flags );
    _JS_CGPARAM( normalScale );
    _JS_CGPARAM( material.Ka );
    _JS_CGPARAM( material.Kd );
    _JS_CGPARAM( material.Ks );
    _JS_CGPARAM( material.Ke );
    _JS_CGPARAM( material.shininess );

    _JS_CGPARAM( globalAmbient );

    _JS_CGPARAM( useTexMatrix );

    _JS_CGPARAM( nLights );

#define _JS_CGLIGHTPARAMX(NAME,ELEM) \
	driver->light[ELEM].NAME =_jsGcmFifoCgGetNamedParameter(driver->vsFixedPipeline,"light[" #ELEM "]." #NAME )
#define _JS_CGLIGHTPARAM(INDEX) \
	_JS_CGLIGHTPARAMX(position,INDEX);		\
	_JS_CGLIGHTPARAMX(ambient,INDEX);		\
	_JS_CGLIGHTPARAMX(diffuse,INDEX);		\
	_JS_CGLIGHTPARAMX(specular,INDEX);		\
	_JS_CGLIGHTPARAMX(spotDirection,INDEX);	\
	_JS_CGLIGHTPARAMX(spotParams,INDEX);	\
	_JS_CGLIGHTPARAMX(attenuation,INDEX)

    _JS_CGLIGHTPARAM( 0 );
    _JS_CGLIGHTPARAM( 1 );
    _JS_CGLIGHTPARAM( 2 );
    _JS_CGLIGHTPARAM( 3 );
    _JS_CGLIGHTPARAM( 4 );
    _JS_CGLIGHTPARAM( 5 );
    _JS_CGLIGHTPARAM( 6 );
    _JS_CGLIGHTPARAM( 7 );
#undef _JS_CGLIGHTPARAM
#undef _JS_CGLIGHTPARAMX

// tex coord support
#define _JS_CGTEXUNITPARAMX(NAME,ELEM) \
	driver->texunit[ELEM].NAME =_jsGcmFifoCgGetNamedParameter(driver->vsFixedPipeline,"texunit[" #ELEM "]." #NAME )
#define _JS_CGTEXUNITPARAM(INDEX) \
	_JS_CGTEXUNITPARAMX(genMode,INDEX); 		\
	_JS_CGTEXUNITPARAMX(genPlane,INDEX); 		\
	_JS_CGTEXUNITPARAMX(matrix,INDEX)

    _JS_CGTEXUNITPARAM( 0 );
    _JS_CGTEXUNITPARAM( 1 );
    _JS_CGTEXUNITPARAM( 2 );
    _JS_CGTEXUNITPARAM( 3 );
#undef _JS_CGTEXUNITPARAM
#undef _JS_CGTEXUNITPARAMX

// user clip planes
    _JS_CGPARAM( clipPlaneActive );
    _JS_CGPARAM( clipPlane[0] );
    _JS_CGPARAM( clipPlane[1] );
    _JS_CGPARAM( clipPlane[2] );
    _JS_CGPARAM( clipPlane[3] );
    _JS_CGPARAM( clipPlane[4] );
    _JS_CGPARAM( clipPlane[5] );
#undef _JS_CGPARAM

    char *ucode;
    unsigned int ucodeSize;
    _jsGcmGetUCode( driver->vsFixedPipeline, &ucode, &ucodeSize );
    // vp programs do not need to be loaded to video memory.
    _jsGcmSetNativeCgVertexProgram( driver->vsFixedPipeline, ucode );

	// [YLIN] Make it 16 byte align
	driver->sharedVPConstants = ( char * )jsMemalign( 16, 4 * sizeof( float ) * JS_MAX_VP_SHARED_CONSTANTS );

    JS_ASSERT( driver->sharedVPConstants );
    _jsAllocSetHint( _JS_ALLOCTYPE_SHARED_FP_CONSTANTS, 0 );
    driver->sharedFPConstantsId = _jsGcmAllocateMemory( 4 * sizeof( float ) * JS_MAX_FP_SHARED_CONSTANTS );
    _jsAllocClearHint();
    JS_ASSERT( driver->sharedFPConstantsId != GMM_ERROR );


    // fp shader manager
    _jsGcmFPShaderManagerInit( &driver->shaderManager );
    jsCgShaderPool *defaultShaders = _jsGcmInitDefaultShaders();
    _jsGcmFPAddShadersFromPool( &driver->shaderManager, defaultShaders );
    _JS_LIST_INIT( driver->pools, jsCgShaderPool, 4 );

    return driver;
}

// Destroy the driver, and free all its used memory
void _jsPlatformRasterExit( void *drv )
{
    jsGcmDriver *driver = ( jsGcmDriver * )drv;
    _jsGcmFPShaderManagerDestroy( &driver->shaderManager );
    int i;
    for ( i = 0;i < driver->poolsCount;++i )
    {
        _jsGcmFPShaderPoolFree( driver->pools + i );
    }
    _JS_LIST_DESTROY( driver->pools, jsCgShaderPool );

    _jsGcmFreeMemory( driver->sharedFPConstantsId );
    jsFree( driver->sharedVPConstants );

    _jsGcmUninitDefaultShaders();

    if ( driver )
    {
        jsFree( driver );
    }
}

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Loads a file that contains an already-compiled vertex or fragment shader program
 *
 * psglLoadShaderLibrary() loads a file with the name <I><c>filename</c></I>.
 * The function expects the file to have the binary code of a
 * compiled vertex or fragment shader program as its contents.
 * If the file contains valid code, psglLoadShaderLibrary()
 * adds the code to a pool of shader programs.
 *
 * @param filename name of a file that contains the compiled binary code for a vertex or fragment shader program

 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>psglLoadShaderLibrary() fails to load the file successfully.</TD>
</TR>
</TABLE>
 */

void psglLoadShaderLibrary( const char *filename )
{
    jsGcmDriver *driver = _jsGetGcmDriver();
    jsCgShaderPool pool;
    _jsGcmFPShaderPoolCreate( &pool, 0 );
    int result = _jsGcmFPShaderPoolLoad( &pool, filename );
#ifndef JS_NO_ERROR_CHECK
    if ( result )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#else
    (( void )result );		// unused variable is ok
#endif
    _JS_LIST_APPEND( driver->pools, jsCgShaderPool, pool );
    _jsGcmFPAddShadersFromPool( &driver->shaderManager, &pool );
}

/**
 *  @}
 */

///////////////////////////////////////////////////////////////////////////
// compile the AttribSet state into its static command buffer.
static jsBitfield _jsPlatformCompileAttribSet( jsAttribSet* attribSet, jsDrawParams* dparams )
{
    const GLuint cmdsPerAttribute = 16;	// wild guess of max #cmds to allocate per attribute
    const int cmdBufferSize = ( _JS_MAX_VERTEX_ATTRIBS + 1 ) * sizeof( jsFifoData ) * cmdsPerAttribute;

    attribSet->beenUpdatedMask = 0;
    attribSet->cmdNumWords = 0;

    if ( ! attribSet->cmdBuffer )
        attribSet->cmdBuffer = jsMemalign( 16, cmdBufferSize );
#ifndef JS_NO_ERROR_CHECK
    if ( ! attribSet->cmdBuffer )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return 0;
    }
#endif

    // pin all allocations in AttribSet compilation
    gmmPinAllocations();

    // generate NV commands for each attribute
    GLvoid* cPtr = attribSet->cmdBuffer;
    for ( GLuint i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        // attributes using client-side arrays are not precompiled
        if ( dparams->attribXferSize[i] ) continue;

        jsAttribute* attrib = attribSet->attribs.attrib + i;
        if ( JSBIT_GET( attribSet->attribs.EnabledMask, i ) )
        {
            JS_ASSERT( ! JSBIT_GET( attribSet->attribs.NeedsConversionMask, i ) );
            GLuint gpuId = GMM_ERROR;
            GLuint gpuOffset;

            gpuId = _jsGcmGetBufferObjectOrigin( attrib->arrayBuffer );

            // attribute data in VBO, clientData is offset.
            gpuOffset = gmmIdToOffset(gpuId)
                      + (( const GLubyte* )attrib->clientData - ( const GLubyte* )NULL );

            cPtr = _jsGcmFifoGlVertexAttribPointerBuffered( cPtr,
                    i, attrib->clientSize,
                    ( jsGcmEnum )attrib->clientType, attrib->normalized,
                    attrib->clientStride, attrib->frequency, gmmIdIsMain(gpuId), gpuOffset );
        }
        else
        {
            // attribute is disabled
            cPtr = _jsGcmFifoGlVertexAttribPointerBuffered( cPtr, i, 0, JSGCM_FLOAT, 0, 0, 0, 0, 0 );
            cPtr = _jsGcmFifoGlVertexAttrib4fvBuffered( cPtr, i, attrib->value );
        }
        JSBIT_TRUE( attribSet->beenUpdatedMask, i );
        JS_ASSERT(( char * )cPtr - ( char * )attribSet->cmdBuffer <  cmdBufferSize );
    }

    cPtr = _jsGcmFifoGlVertexFrequencyOperationBuffered( cPtr, attribSet->attribs.ModuloMask );

    if ( attribSet->attribs.PrimitiveRestart ) cPtr = _jsGcmFifoGlEnableBuffered( cPtr, JSGCM_PRIMITIVE_RESTART );
    else cPtr = _jsGcmFifoGlDisableBuffered( cPtr, JSGCM_PRIMITIVE_RESTART );
    cPtr = _jsGcmFifoGlPrimitiveRestartIndexBuffered( cPtr, attribSet->attribs.RestartIndex );

    JS_ASSERT(( char * )cPtr - ( char * )attribSet->cmdBuffer < cmdBufferSize );
    attribSet->cmdNumWords = (( char * )cPtr - ( char * )attribSet->cmdBuffer ) / sizeof( jsFifoData );
    attribSet->dirty = GL_FALSE;

    // unpin all allocations in AttribSet compilation
    gmmUnpinAllocations();

    return attribSet->beenUpdatedMask;
}

///////////////////////////////////////////////////////////////////////////
void _jsDumpFifo( char * name );
extern bool _cellRSXFifoDisassembleToFileMask;

// Fast rendering path called by several glDraw calls:
//   glDrawElements, glDrawRangeElements, glDrawArrays
// Slow rendering calls this function also, though it must also perform various
// memory setup operations first
void _jsPlatformDraw( jsDrawParams* dparams )
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );
	
	jsGcmDriver *driver = _jsGetGcmDriver();
    if ( JS_UNLIKELY( !driver->rtValid ) ) return;

    _JS_PERFORMANCE_START( RasterDrawCounter );

    // check for any writable mapped buffers
    if ( driver->flushBufferCount != 0 )
        driver->invalidateVertexCache = GL_TRUE;

    GLboolean isMain = 0;
	GLuint gpuOffset = GMM_ERROR;
	
	// check all conditions for fast pass attribute validation
	bool attribSetDirty = false;
    if ( LContext->attribSetDirty && LContext->attribSetName )
	{
    	jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
		attribSetDirty = attribSet->dirty;
	}
	uint32_t totalXfer = 0;
	for ( GLuint i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
		totalXfer += dparams->attribXferSize[i];

	if( dparams->indexType && dparams->xferTotalSize == 0 && !attribSetDirty && 
		totalXfer == 0 && dparams->indexXferSize == 0 )
	{
		gpuOffset = _jsValidateAttributes( dparams->indices, &isMain );
	}
	else
	{
		gpuOffset = _jsValidateAttributesSlow( dparams, &isMain );
	}

#ifndef JS_NO_ERROR_CHECK
    if ( gpuOffset == GMM_ERROR )
    {
        return;
    }
#endif
    
    if ( driver->invalidateVertexCache )
    {
        driver->invalidateVertexCache = GL_FALSE;
        _jsGcmFifoGlInvalidateVertexCache();
    }

	// if fpLoadProgramId is in main memory need set the CELL_GCM_LOCATION_MAIN
	if ( gmmIdIsMain(driver->fpLoadProgramId) )
	{
		GCM_FUNC( cellGcmSetUpdateFragmentProgramParameterLocation, 
			gmmIdToOffset( driver->fpLoadProgramId ) + driver->fpLoadProgramOffset, CELL_GCM_LOCATION_MAIN );
	}
	else
	{
		GCM_FUNC( cellGcmSetUpdateFragmentProgramParameter, 
            gmmIdToOffset( driver->fpLoadProgramId ) + driver->fpLoadProgramOffset );
	}


    if ( dparams->indexType )
    {
        // glDraw*Elements()
        _JS_PERFORMANCE_START( NVDrawCounter );
        _jsGcmFifoGlDrawElements(( jsGcmEnum )dparams->mode, dparams->indexCount, ( jsGcmEnum )dparams->indexType, isMain, gpuOffset );
        _JS_PERFORMANCE_STOP( NVDrawCounter );
    }
    else
    {
        // glDrawArrays()
        _JS_PERFORMANCE_START( NVDrawCounter );
        _jsGcmFifoGlDrawArrays(( jsGcmEnum )dparams->mode, dparams->firstVertex, dparams->vertexCount );
        _JS_PERFORMANCE_STOP( NVDrawCounter );
    }

    _JS_PERFORMANCE_STOP( RasterDrawCounter );
}


// Set the current clip planes on the hardware
void _jsValidateClipPlanes()
{
    JS_ASSERT( _JS_MAX_CLIP_PLANES == 6 );

    PSGLcontext* LContext = _CurrentContext;
    jsGcmDriver *driver = _jsGetGcmDriver();

    // set bitfields for clip plane control
    //  6 fields of 4 bits packed into a word.
    //  0x0	clipping disabled
    //  0x1 pass if < 0
    //  0x2 pass if >= 0
    GLuint clipControl = 0;
    GLuint clipMask = 0;
    GLuint clipControls[_JS_MAX_CLIP_PLANES];
    for ( int i = 0; i < _JS_MAX_CLIP_PLANES; ++i )
    {
        if ( LContext->ClipPlane[i].Active )
        {
            clipControl |= ( 0x2 << ( 4 * i ) );	// 0x2 is ENABLE_GE
            clipMask |= ( 1 << i );
            clipControls[i] = 0x2; // 0x2 is ENABLE_GE
        }
        else
            clipControls[i] = 0;

        // pass plane equation to fixed function
        if ( !LContext->VertexProgram || LContext->BoundVertexProgram == NULL )
            _jsGcmFifoGlProgramParameterfv(
                driver->vsFixedPipeline,
                driver->clipPlane[i],
                LContext->ClipPlane[i].Coeff,
                NULL );
    }

    _jsGcmFifoGlClipPlanes( clipMask );

    // send to hardware
    GCM_FUNC( cellGcmSetUserClipPlaneControl, clipControls[0], clipControls[1], clipControls[2], clipControls[3], clipControls[4], clipControls[5] ); 
}

#ifdef JS_DPM
int __jsNumFPSharedConstantsValidated;
#endif
// Send fragment program shared constants (uniforms) to hardware
void _jsValidateFragmentProgramSharedConstants()
{
    _JS_PERFORMANCE_START( ValidateFPSharedConstantsCounter );
    PSGLcontext*	LContext = _CurrentContext;

    JS_ASSERT( LContext->FragmentProgram && LContext->BoundFragmentProgram != NULL );
#ifdef JS_DPM
	__jsNumFPSharedConstantsValidated++;
#endif

    _CGprogram * cgprog = LContext->BoundFragmentProgram;
    if (( cgprog->constantPushBuffer ) && ( LContext->LastFPConstantModification != cgprog->lastConstantUpdate ) )
    {
        // build shared constants push buffers, which is dependent on the load address
        uint32_t *jsGcmCurrent = ( uint32_t * )cgprog->constantPushBuffer;

        GCM_FUNC_BUFFERED( cellGcmSetTransferDataMode, jsGcmCurrent, CELL_GCM_TRANSFER_LOCAL_TO_LOCAL);
        GCM_FUNC_BUFFERED( cellGcmSetTransferDataFormat, jsGcmCurrent, 16, 16, 16, 1, 1, 1);

        _jsPrepareFPSharedPushBuffer( cgprog, jsGcmCurrent );

        JS_ASSERT( jsGcmCurrent <= ( jsFifoData* )( cgprog->constantPushBuffer + cgprog->constantPushBufferWordSize ) );

        int nopCount = ( uint32_t* )( cgprog->constantPushBuffer + cgprog->constantPushBufferWordSize ) - jsGcmCurrent;
        JS_ASSERT( nopCount >= 0 );
        GCM_FUNC_BUFFERED( cellGcmSetNopCommand, jsGcmCurrent, nopCount ); 

        _jsGcmPushProgramPushBuffer( cgprog );
        cgprog->lastConstantUpdate = LContext->LastFPConstantModification;
    }

    LContext->needValidate &= ~PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    _JS_PERFORMANCE_STOP( ValidateFPSharedConstantsCounter );
}

#ifdef JS_DPM
int __jsNumFragmentProgramsValidated;
#endif
// Set up the current fragment program on hardware
void _jsValidateFragmentProgram()
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifdef JS_DPM
    __jsNumFragmentProgramsValidated++;
#endif
    jsGcmDriver *driver = _jsGetGcmDriver();
    _CGprogram *program = LContext->BoundFragmentProgram;

    // params are set directly in the GPU memory, so there is nothing to be done here.
    _jsSetNativeCgFragmentProgram( program );
    driver->fpLoadProgramId = program->loadProgramId;
    driver->fpLoadProgramOffset = program->loadProgramOffset;
}

// Set up the fixed function fragment program on hardware
void _jsValidateFFXFragmentProgram()
{
    jsGcmDriver *driver = _jsGetGcmDriver();

    // set all the parameters that can change on the ffx.
    for ( int unit = 0; unit < _JS_MAX_TEXTURE_UNITS; ++unit )
    {
        jsTextureImageUnit *tu = _CurrentContext->TextureImageUnits + unit;
        GLenum mode = _jsGetEnabledTextureMode( tu );
        if ( mode )
        {
            jsTexture* texture = tu->currentTexture;
            JS_ASSERT( texture == _jsGetCurrentTexture( tu, mode ) );
            if ( JS_LIKELY( _jsTextureIsValid( texture ) ) )
            {
                _jsGcmFPShaderManagerSetTextureEnvMode( &driver->shaderManager, unit, tu->envMode, ( float * )&tu->envColor );
                _jsGcmFPShaderManagerSetTextureFormat( &driver->shaderManager, unit, texture->images[0]->format );
                _jsGcmFPShaderManagerSetTextureMode( &driver->shaderManager, unit, mode );
            }
            else
            {
                _jsGcmFPShaderManagerSetTextureMode( &driver->shaderManager, unit, 0 );
            }
        }
        else
        {
            _jsGcmFPShaderManagerSetTextureMode( &driver->shaderManager, unit, 0 );
        }
    }

    _jsValidateFog();

    _jsGcmFPLoadCurrentShader( &driver->shaderManager );
}


// must always call this before _jsPlatformDraw() to setup jsDrawParams
GLboolean _jsPlatformRequiresSlowPath( jsDrawParams* dparams, const GLenum indexType, uint32_t indexCount)
{
    PSGLcontext* LContext = _CurrentContext;
    jsAttributeState* as = LContext->attribs;
    GLuint maxElements;

    if ( JS_UNLIKELY( as->NeedsConversionMask & as->EnabledMask ) )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because an attribute array needs conversion" );
        return GL_TRUE;
    }


    if ( indexType )	// indexed draw
    {
        JS_ASSERT( indexType == GL_UNSIGNED_BYTE || indexType == GL_UNSIGNED_SHORT || indexType == GL_UNSIGNED_INT );
		JS_ASSERT( indexCount );

        if ( JS_UNLIKELY( indexType == GL_UNSIGNED_BYTE ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because the index array needs conversion" );
            return GL_TRUE;
        }
		if ( JS_UNLIKELY( indexCount > 0xfffff ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because too many indices (1048576 max)" );
            return GL_TRUE;
        }

        // if client-side indices, determine transfer buffer requirements
        if ( JS_UNLIKELY( as->ElementArrayBuffer == 0 ) )
        {
            _JS_PERFORMANCE_START( IndicesCounter );
            const GLuint typeSize = ( indexType == GL_UNSIGNED_INT ) ? sizeof( GLuint ) : sizeof( GLushort );

            GLuint numBytes = typeSize * indexCount;
            dparams->indexXferSize = numBytes;
            dparams->xferTotalSize += _jsPad( numBytes, 128 );
            _JS_PERFORMANCE_STOP( IndicesCounter );

            JS_ASSERT( dparams->indexXferSize != 0 );
            if ( JS_UNLIKELY( dparams->indexXferSize > JSGCM_MAX_INDEX_BUFFER_SIZE ) )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because index array doesn't fit in temporary buffer" );
                return GL_TRUE;
            }
        }

        maxElements = dparams->indexMax + 1;	// may later override glDrawElements
    }
    else	// glDrawArrays
    {
        JS_ASSERT( dparams->firstVertex >= 0 );
        JS_ASSERT( dparams->vertexCount > 0 );

        maxElements = dparams->firstVertex + dparams->vertexCount;
        if ( JS_UNLIKELY( maxElements > 0xfffff ) )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because too many vertices (1048576 max)" );
            return GL_TRUE;
        }
    }

    // are any enabled attributes on the client-side?
    const GLuint clientSideMask = as->EnabledMask & ~as->HasVBOMask;
    if ( JS_UNLIKELY( clientSideMask ) )
    {
        if ( indexType && dparams->indexMax == 0 )	// glDrawElements
        {
            if ( as->ElementArrayBuffer )		// if indices VBO
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because glDrawElements must read back index VBO" );
                return GL_TRUE;
            }

            // glDrawElements indices are on client-side;
            // determine min/max to use glDrawRangeElements instead
            _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "glDrawElements has non-VBO attributes, converting to glDrawRangeElements" );
            GLuint min = 0, max = 0;
            _jsComputeMinMaxIndices( LContext, &min, &max, dparams->indices, indexType, dparams->indexCount );
            dparams->indexMin = 0;
            dparams->indexMax = max;
            maxElements = dparams->indexMax + 1;
        }

        // determine transfer buffer requirements for client-side attributes
        _JS_PERFORMANCE_START( AttributesCounter );
        for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
        {
            if ( clientSideMask & ( 1 << i ) )
            {
                jsAttribute* attrib = as->attrib + i;
                const GLuint freq = attrib->frequency;
                GLuint count;
                if ( JSBIT_GET( as->ModuloMask, i ) )
                    count = maxElements > freq ? freq : maxElements;
                else
                    count = ( maxElements + freq - 1 ) / freq;

                const GLuint numBytes = attrib->clientStride * count;
                JS_ASSERT( numBytes != 0 );
                dparams->attribXferOffset[i] = dparams->xferTotalSize;
                dparams->attribXferSize[i] = numBytes;

                const GLuint numBytesPadded = _jsPad( numBytes, 128 );
                dparams->xferTotalSize += numBytesPadded;
                dparams->attribXferTotalSize += numBytesPadded;
            }
            else
            {
                dparams->attribXferOffset[i] = 0;
                dparams->attribXferSize[i] = 0;
            }
        }
        _JS_PERFORMANCE_STOP( AttributesCounter );

        JS_ASSERT( dparams->attribXferTotalSize != 0 );
        if ( dparams->attribXferTotalSize > JSGCM_MAX_VERTEX_BUFFER_SIZE )
        {
            _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Using slow vertex path because attribute arrays exceed the temporary buffer" );
            return GL_TRUE;
        }
    }

    return GL_FALSE;	// we are finally qualified for the fast path
}

// Determine if a vertex attribute will cause slow path rendering, 
// and if so output a warning.
GLboolean _jsPlatformNeedsConversion( const jsAttributeState* as, GLuint index )
{
    const jsAttribute* attrib = as->attrib + index;
    // Maximum stride is 255
    if ( attrib->clientStride >= 256 )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Attribute %d has a stride of %d which is bigger than the maximum 255. Slow path ahead !", index, attrib->clientStride );
        return GL_TRUE;
    }
    switch ( attrib->clientType )
    {
        case GL_SHORT:
        case GL_HALF_FLOAT_ARB:
        case GL_FLOAT:
        case GL_FIXED_11_11_10_SCE:
            // no conversion required
            return GL_FALSE;
        case GL_UNSIGNED_BYTE:
            if ( attrib->normalized ||
                 attrib->clientSize == 4 ) 
                 return GL_FALSE; 
            // _jsGcmFifoGlVertexAttribPointer accepts either size of 4 or normalized attrib
            break;
        default:
            break;
    }
    _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_DATA_WARNING, "Attribute %d of type %s needs conversion. Slow path ahead !", index, _jsGetGLEnumName( attrib->clientType ) );
    return GL_TRUE;
}
// Determine a system contraint value
void _jsPlatformRasterGetIntegerv( GLenum pname, GLint* params )
{
    switch ( pname )
    {
        case GL_MAX_ELEMENTS_VERTICES:
            *params = 0x100000;
            break;
        case GL_MAX_ELEMENTS_INDICES:
            *params = 0x100000;
            break;
        case GL_MAX_DRAW_BUFFERS_ATI:
		case GL_MAX_COLOR_ATTACHMENTS_OES:
            *params = JSGCM_SETRENDERTARGET_MAXCOUNT;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
}

// Return the current JSGcmDriver
jsGcmDriver* _jsGetJSGcmDriver()
{


    JS_ASSERT( _CurrentDevice );
    JS_ASSERT( _CurrentDevice->rasterDriver );
    return ( jsGcmDriver * )( _CurrentDevice->rasterDriver );
}

void _jsPlatformRasterFlush()
{
	// if just recording the commands we don't need to do the RSX flush ( cellGcmFlush() ) 
	if ( !_jsGcmState.recordFifoMode )
		_jsGcmFifoGlFlush();
}

void _jsPlatformRasterFinish()
{
    _jsGcmFifoGlFinish();
}

// Send the current cull face state to hardware
void _jsValidateCullFace()
{
    PSGLcontext*	LContext = _CurrentContext;

    GCM_FUNC( cellGcmSetCullFaceEnable, LContext->FaceCulling );
    GCM_FUNC( cellGcmSetCullFace, LContext->CullFaceMode );
    GCM_FUNC( cellGcmSetFrontFace, LContext->FrontFace );

    _CurrentContext->needValidate &= ~PSGL_VALIDATE_FACE_CULL;
}


// Send the current point size state to hardware
void _jsValidatePointRaster()
{
    PSGLcontext*	LContext = _CurrentContext;
    GCM_FUNC( cellGcmSetPointSize, LContext->PointSize );
    _CurrentContext->needValidate &= ~PSGL_VALIDATE_POINT_RASTER;
}

// Send the current line width state to hardware
void _jsValidateLineRaster()
{
    PSGLcontext*	LContext = _CurrentContext;
    uint32_t w = ( uint32_t )( LContext->LineWidth * 8.0f ); // converting float to 6.3 fixed pt by multiplying by 2^3
    GCM_FUNC( cellGcmSetLineWidth, w );
    _CurrentContext->needValidate &= ~PSGL_VALIDATE_LINE_RASTER;
}

// Send the current polygon rendering mode to hardware
void _jsValidatePolygonMode()
{
    PSGLcontext*	LContext = _CurrentContext;
    GCM_FUNC( cellGcmSetFrontPolygonMode, LContext->FrontPolygonMode );
    GCM_FUNC( cellGcmSetBackPolygonMode, LContext->BackPolygonMode );
    _CurrentContext->needValidate &= ~PSGL_VALIDATE_POLYGON_MODE;
}

// Send the current shading model to hardware
void _jsValidateShadeModel()
{
    PSGLcontext*	LContext = _CurrentContext;
    GCM_FUNC( cellGcmSetShadeMode, LContext->ShadeModel );
    _CurrentContext->needValidate &= ~PSGL_VALIDATE_SHADE_MODEL;
}

void _jsValidateShaderSRGBRemap()
{
    PSGLcontext*	LContext = _CurrentContext;
    // not available
    LContext->needValidate &= ~PSGL_VALIDATE_SHADER_SRGB_REMAP;
}

void _jsValidatePrimitiveRestart()// JSGCM_PORT_RSTENSON Raster ToDo !!!
{
    // on GL, primitive restart will go through slow path.
    PSGLcontext*	LContext = _CurrentContext;
    LContext->needValidate &= ~PSGL_VALIDATE_PRIMITIVE_RESTART;
}

void _jsValidatePointSprite() // JSGCM_PORT_RSTENSON Raster ToDo !!!
{
    PSGLcontext*	LContext = _CurrentContext;
    LContext->needValidate &= ~PSGL_VALIDATE_POINT_SPRITE;
}


void printInfo( GLvoid* cmdBuffer )
{
    printf( " cmdBuffer data %d \n", *( GLuint * )cmdBuffer );
}

// invalidate attributes
void _jsInvalidateAttributes()
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );

	jsAttributeState* as = LContext->attribs;

	as->DirtyMask = ( 1 << _JS_MAX_VERTEX_ATTRIBS ) - 1; // mark all dirty
}

// Fast pass validation of all attributes
GLuint _jsValidateAttributes(const GLvoid* indices, GLboolean *isMain)
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );
	jsGcmDriver *driver = _jsGetGcmDriver();
	jsAttributeState* as = LContext->attribs;
	JS_ASSERT( _jsCheckAttribCaches( as ) );

	GLuint VBOId = GMM_ERROR;
	GLuint gpuOffset = GMM_ERROR;

    // which attributes are known to need updating?
    // (due to being dirty or enabled client-side arrays)
    jsBitfield needsUpdateMask = ( as->DirtyMask | ( as->EnabledMask & ~as->HasVBOMask ) );

    // if an AttribSet is bound or updated, apply its attribute updates
    if ( LContext->attribSetDirty && LContext->attribSetName )
    {
        jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
        ////if ( attribSet->dirty )
        ////    _jsPlatformCompileAttribSet( attribSet, dparams );

        // copy precompiled command buffer to pushbuffer
        if ( attribSet->cmdNumWords > 0 )
        {
            JS_ASSERT( attribSet->beenUpdatedMask );

            // need to make sure we have space for the attrib set or wait until we do.
            _jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo, attribSet->cmdNumWords );
            //fast_memcpy( nvCurrent, attribSet->cmdBuffer, sizeof(__GLNVdata)*attribSet->cmdNumWords );
            __builtin_memcpy( _jsGcmState.fifo.current, attribSet->cmdBuffer, sizeof( jsFifoData )*attribSet->cmdNumWords );
            _jsGcmState.fifo.current += attribSet->cmdNumWords;

            // for attributes updated, clear their bits from needsUpdateMask
            needsUpdateMask &= ~( attribSet->beenUpdatedMask );
            driver->invalidateVertexCache = GL_TRUE;
        }
    }
    LContext->attribSetDirty = GL_FALSE;


    // for any remaining attributes that need updating, do it now.
    _JS_PERFORMANCE_START( AttributesCounter );
    if ( needsUpdateMask )
    {
        for ( GLuint i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
        {
            // skip this attribute if not needing update
            if ( ! JSBIT_GET( needsUpdateMask, i ) ) continue;

            jsAttribute* attrib = as->attrib + i;
            if ( JSBIT_GET( as->EnabledMask, i ) )
            {
                JS_ASSERT( ! JSBIT_GET( as->NeedsConversionMask, i ) );
                const GLsizei stride = attrib->clientStride;
                const GLuint freq = attrib->frequency;

				{
                    // attribute data in VBO, clientData is offset.
                    VBOId = _jsGcmGetBufferObjectOrigin( attrib->arrayBuffer );
                    *isMain = gmmIdIsMain(VBOId);
                    gpuOffset = gmmIdToOffset(VBOId)
                                + (( const GLubyte* )attrib->clientData - ( const GLubyte* )NULL );
                }

                _jsGcmFifoGlVertexAttribPointer( i, attrib->clientSize,
                                                 ( jsGcmEnum )attrib->clientType, attrib->normalized,
                                                 stride, freq, *isMain, gpuOffset );
            }
            else
            {
                // attribute is disabled
                _jsGcmFifoGlVertexAttribPointer( i, 0, JSGCM_FLOAT, 0, 0, 0, 0, 0 );
                _jsGcmFifoGlVertexAttrib4fv( i, attrib->value );
            }
        }
        _jsGcmFifoGlVertexFrequencyOperation( as->ModuloMask );
        driver->invalidateVertexCache = GL_TRUE;
    }
    as->DirtyMask = 0;	// all attributes are now clean
    _JS_PERFORMANCE_STOP( AttributesCounter );

	// validate index buffer
	GLuint indexOffset = 0;

	{
        // glDraw*Elements()
        _JS_PERFORMANCE_START( IndicesCounter );
        
		{
            // index VBO
            VBOId = _jsGcmGetBufferObjectOrigin( as->ElementArrayBuffer );
            *isMain = gmmIdIsMain(VBOId);
            indexOffset = gmmIdToOffset(VBOId)
                        + (( const GLubyte* )indices - ( const GLubyte* )NULL );
        }
        _JS_PERFORMANCE_STOP( IndicesCounter );
    }

	JS_ASSERT( _jsCheckAttribCaches( as ) );

	return indexOffset;

}

// validates attributes for specified draw paramaters
// returns pointer to index buffer
GLuint _jsValidateAttributesSlow( jsDrawParams *dparams, GLboolean *isMain )
{
	PSGLcontext*	LContext = _CurrentContext;
	JS_ASSERT( LContext );
	jsGcmDriver *driver = _jsGetGcmDriver();
	jsAttributeState* as = LContext->attribs;
	JS_ASSERT( _jsCheckAttribCaches( as ) );

    // allocate upload transfer buffer if necessary
    //  The higher level bounce buffer allocator is used, which means that
    //  the buffer will automatically be freed after all JSGCM calls up to
    //  the next allocation have finished.
    JS_ASSERT((( as->EnabledMask & ~as->HasVBOMask ) != 0 ) == ( dparams->attribXferTotalSize != 0 ) );
    void* xferBuffer = NULL;
    GLuint xferId = GMM_ERROR;
    GLuint VBOId = GMM_ERROR;
    GLuint gpuOffset;
    if ( JS_UNLIKELY( dparams->xferTotalSize ) )
    {
        _jsAllocSetHint( _JS_ALLOCTYPE_GEOMETRY_TRANSFER, 0 );
        xferId = _jsGcmAllocateMemory( dparams->xferTotalSize );
        xferBuffer = gmmIdToAddress(xferId);
        _jsAllocClearHint();
#ifndef JS_NO_ERROR_CHECK
        if ( xferId == GMM_ERROR )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return GMM_ERROR;
        }
#endif
    }


    // which attributes are known to need updating?
    // (due to being dirty or enabled client-side arrays)
    jsBitfield needsUpdateMask = ( as->DirtyMask | ( as->EnabledMask & ~as->HasVBOMask ) );

    // if an AttribSet is bound or updated, apply its attribute updates
    if ( LContext->attribSetDirty && LContext->attribSetName )
    {
        jsAttribSet* attribSet = _jsGetAttribSet( LContext->attribSetName );
        if ( attribSet->dirty )
            _jsPlatformCompileAttribSet( attribSet, dparams );

        // copy precompiled command buffer to pushbuffer
        if ( attribSet->cmdNumWords > 0 )
        {
            JS_ASSERT( attribSet->beenUpdatedMask );

            // need to make sure we have space for the attrib set or wait until we do.
            _jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo, attribSet->cmdNumWords );
            //fast_memcpy( nvCurrent, attribSet->cmdBuffer, sizeof(__GLNVdata)*attribSet->cmdNumWords );
            __builtin_memcpy( _jsGcmState.fifo.current, attribSet->cmdBuffer, sizeof( jsFifoData )*attribSet->cmdNumWords );
            _jsGcmState.fifo.current += attribSet->cmdNumWords;

            // for attributes updated, clear their bits from needsUpdateMask
            needsUpdateMask &= ~( attribSet->beenUpdatedMask );
            driver->invalidateVertexCache = GL_TRUE;
        }
    }
    LContext->attribSetDirty = GL_FALSE;


    // for any remaining attributes that need updating, do it now.
    _JS_PERFORMANCE_START( AttributesCounter );
    if ( needsUpdateMask )
    {
        for ( GLuint i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
        {
            // skip this attribute if not needing update
            if ( ! JSBIT_GET( needsUpdateMask, i ) ) continue;

            jsAttribute* attrib = as->attrib + i;
            if ( JSBIT_GET( as->EnabledMask, i ) )
            {
                JS_ASSERT( ! JSBIT_GET( as->NeedsConversionMask, i ) );
                const GLsizei stride = attrib->clientStride;
                const GLuint freq = attrib->frequency;

                if ( JS_UNLIKELY( dparams->attribXferSize[i] ) )
                {
                    // attribute data is client side, need to transfer
                    GLuint maxElements = 0;					// glDrawElements
                    if ( dparams->indexType == 0 )
                        maxElements = dparams->firstVertex + dparams->vertexCount;	// glDrawArray
                    else if ( dparams->indexMax )
                        maxElements = dparams->indexMax + 1;	// glDrawRangeElements

                    // don't transfer data that is not going to be used, from 0 to first*stride
                    GLuint offset;
                    if ( JSBIT_GET( as->ModuloMask, i ) )
                        offset = ( maxElements > freq ) ? 0 : dparams->firstVertex * stride;
                    else
                        offset = ( dparams->firstVertex / freq ) * stride;

                    char * b = ( char * )xferBuffer + dparams->attribXferOffset[i];
                    _JS_PERFORMANCE_START( RasterMemcpyCounter );
                    fast_memcpy( b + offset,
                                 ( char * )attrib->clientData + offset,
                                 dparams->attribXferSize[i] - offset );
                    _JS_PERFORMANCE_STOP( RasterMemcpyCounter );

                    // draw directly from bounce buffer
                    *isMain = gmmIdIsMain(xferId);
                    gpuOffset = gmmIdToOffset(xferId) + (b - ( char * )xferBuffer);

                }
                else
                {
                    // attribute data in VBO, clientData is offset.
                    VBOId = _jsGcmGetBufferObjectOrigin( attrib->arrayBuffer );
                    *isMain = gmmIdIsMain(VBOId);
                    gpuOffset = gmmIdToOffset(VBOId)
                                + (( const GLubyte* )attrib->clientData - ( const GLubyte* )NULL );
                }

                _jsGcmFifoGlVertexAttribPointer( i, attrib->clientSize,
                                                 ( jsGcmEnum )attrib->clientType, attrib->normalized,
                                                 stride, freq, *isMain, gpuOffset );
            }
            else
            {
                // attribute is disabled
                _jsGcmFifoGlVertexAttribPointer( i, 0, JSGCM_FLOAT, 0, 0, 0, 0, 0 );
                _jsGcmFifoGlVertexAttrib4fv( i, attrib->value );
            }
        }
        _jsGcmFifoGlVertexFrequencyOperation( as->ModuloMask );
        driver->invalidateVertexCache = GL_TRUE;
    }
    as->DirtyMask = 0;	// all attributes are now clean
    _JS_PERFORMANCE_STOP( AttributesCounter );

	// validate index buffer
	GLuint indexOffset = 0;

	if( dparams->indexType )
	{
        // glDraw*Elements()
        _JS_PERFORMANCE_START( IndicesCounter );
        if ( JS_UNLIKELY( dparams->indexXferSize ) )
        {
            // client-side index array
            char* b = ( char * )xferBuffer + dparams->indexXferOffset;
            _JS_PERFORMANCE_START( RasterMemcpyCounter );
            fast_memcpy( b, dparams->indices, dparams->indexXferSize );
            _JS_PERFORMANCE_STOP( RasterMemcpyCounter );

            // draw directly from bounce buffer
            *isMain = gmmIdIsMain(xferId);
            indexOffset = gmmIdToOffset(xferId) + (b - ( char * )xferBuffer);
        }
        else
        {
            // index VBO
            VBOId = _jsGcmGetBufferObjectOrigin( as->ElementArrayBuffer );
            *isMain = gmmIdIsMain(VBOId);
            indexOffset = gmmIdToOffset(VBOId)
                        + (( const GLubyte* )dparams->indices - ( const GLubyte* )NULL );
        }
        _JS_PERFORMANCE_STOP( IndicesCounter );
    }

    if ( xferId != GMM_ERROR )
    {
        _jsGcmFreeMemory( xferId );
    }

	JS_ASSERT( _jsCheckAttribCaches( as ) );

	return indexOffset;
}
