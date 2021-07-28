/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#include <stdio.h>
#include <stdlib.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFBOps.h>
#include <JSGCM/JSGcmFifo.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <string.h>
#include <PSGL/DebugChannel.h>

#ifdef JS_PLATFORM_CELL
#include <sys/timer.h>
#define sleep(x) sys_timer_sleep(x)
#endif


#ifdef JS_RSXFIFODUMP
#include <PSGL/JSRSXFifoDump.h>
#endif

#include <PSGL/DebugChannel.h>

#include <cell/gcm.h>
using namespace cell::Gcm;

#ifdef GCM_HUD
#include <cell/gcm_hud.h>
#endif


// the global instance of the jsGcmState structure
jsGcmState _jsGcmState;

/* setup the default opengl state
**
**
*/

void _jsGcmSetOpenGLState( jsGcmState *jsGcmSt )
{
    GLuint i;

    // initialize the default OpenGL state
    _jsGcmFifoGlAlphaFunc( JSGCM_ALWAYS, 0.0f );
    _jsGcmFifoGlBlendColor( 0.0f, 0.0f, 0.0f, 0.0f );
    _jsGcmFifoGlBlendEquation( JSGCM_FUNC_ADD, JSGCM_FUNC_ADD );
    _jsGcmFifoGlBlendFunc( JSGCM_ONE, JSGCM_ZERO, JSGCM_ONE, JSGCM_ZERO );
    _jsGcmFifoGlClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    _jsGcmFifoGlClearDepthf( 1.0f );
    _jsGcmFifoGlClearStencil( 0 );
    _jsGcmFifoGlClipPlanes( 0 );
    _jsGcmFifoGlColorMask( JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE, JSGCM_TRUE );
    _jsGcmFifoGlCullFace( JSGCM_BACK );
    _jsGcmFifoGlDepthBoundsf( 0.0f, 1.0f );
    _jsGcmFifoGlDepthFunc( JSGCM_LESS );
    _jsGcmFifoGlDepthMask( JSGCM_TRUE );
    _jsGcmFifoGlDisable( JSGCM_ALPHA_TEST );
    _jsGcmFifoGlDisable( JSGCM_BLEND );
    _jsGcmFifoGlDisable( JSGCM_COLOR_LOGIC_OP );
    _jsGcmFifoGlDisable( JSGCM_CULL_FACE );
    _jsGcmFifoGlDisable( JSGCM_COLOR_LOGIC_OP );
    _jsGcmFifoGlDisable( JSGCM_DEPTH_BOUNDS_TEST );
    _jsGcmFifoGlDisable( JSGCM_DEPTH_TEST );
    _jsGcmFifoGlDisable( JSGCM_POLYGON_OFFSET_FILL );
    _jsGcmFifoGlDisable( JSGCM_POLYGON_OFFSET_LINE );
    _jsGcmFifoGlDisable( JSGCM_PRIMITIVE_RESTART );
    _jsGcmFifoGlDisable( JSGCM_PSHADER_SRGB_REMAPPING );
    _jsGcmFifoGlDisable( JSGCM_SCISSOR_TEST );
    _jsGcmFifoGlDisable( JSGCM_STENCIL_TEST );
    _jsGcmFifoGlDisable( JSGCM_STENCIL_TEST_TWO_SIDE );
    _jsGcmFifoGlDisable( JSGCM_LIGHT_MODEL_TWO_SIDE );
    _jsGcmFifoGlDisable( JSGCM_POINT_SPRITE );

    _jsGcmFifoGlVertexFrequencyOperation( 0 );

    for ( i = 0; i < JSGCM_ATTRIB_COUNT; i++ )
    {
        _jsGcmFifoGlVertexAttribPointer( i, 0, JSGCM_FLOAT, JSGCM_FALSE, 0, 0, 0, 0 );
    }


    _jsGcmFifoGlEnable( JSGCM_DITHER );
    _jsGcmFifoGlFrontFace( JSGCM_CCW );
    _jsGcmFifoGlLineWidth( 1.0f );
    _jsGcmFifoGlLogicOp( JSGCM_COPY );
    _jsGcmFifoGlPolygonOffset( 0.0f, 0.0f );
    _jsGcmFifoGlPointSize( 1.0f );
    _jsGcmFifoGlPrimitiveRestartIndex( 0 );
    _jsGcmFifoGlScissor( 0, 0, JSGCM_MAX_RT_DIMENSION, JSGCM_MAX_RT_DIMENSION );
    _jsGcmFifoGlShadeModel( JSGCM_SMOOTH );

    _jsGcmFifoGlActiveStencilFace( JSGCM_BACK );
    _jsGcmFifoGlStencilFunc( JSGCM_ALWAYS, 0, 0xff );
    _jsGcmFifoGlStencilOp( JSGCM_KEEP, JSGCM_KEEP, JSGCM_KEEP );
    _jsGcmFifoGlStencilMask( 0xff );
    _jsGcmFifoGlActiveStencilFace( JSGCM_FRONT );
    _jsGcmFifoGlStencilFunc( JSGCM_ALWAYS, 0, 0xff );
    _jsGcmFifoGlStencilOp( JSGCM_KEEP, JSGCM_KEEP, JSGCM_KEEP );
    _jsGcmFifoGlStencilMask( 0xff );
    for ( i = 0; i < JSGCM_MAX_TEXIMAGE_COUNT; i++ )
    {
        static const GLuint borderColor = 0;

        // update the setTextureAddress Portion
        GCM_FUNC( cellGcmSetTextureAddress, i, CELL_GCM_TEXTURE_WRAP, CELL_GCM_TEXTURE_WRAP, CELL_GCM_TEXTURE_CLAMP_TO_EDGE,
                  CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, CELL_GCM_TEXTURE_ZFUNC_NEVER, 0 );

        // update the setTextureFilter Portion
        GCM_FUNC( cellGcmSetTextureFilter, i, 0, CELL_GCM_TEXTURE_NEAREST_LINEAR, CELL_GCM_TEXTURE_LINEAR,
                  CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX );

        // update the texture control to setup antisotropic settings
        GCM_FUNC( cellGcmSetTextureControl, i, CELL_GCM_TRUE, 0, 12 << 8, CELL_GCM_TEXTURE_MAX_ANISO_1 );

        // update border color
        GCM_FUNC( cellGcmSetTextureBorderColor, i, borderColor );
    }

    // Set zNear and zFar to the default 0.0f and 1.0f here
    _jsGcmFifoGlViewport( 0, 0, JSGCM_MAX_RT_DIMENSION, JSGCM_MAX_RT_DIMENSION, 0.0f, 1.0f );

    jsGcmSt->state.queries.currentZPassQueryId = 0xffffffff;
}

PSGL_EXPORT int _psglDebugChannel = 0;

#ifdef JSGCM_DEBUG_CHANNEL_ERROR
void _jsBackTrace( unsigned int );

void _jsGcmDebugFinish()
{
    if ( _psglDebugChannel )
    {
        int v = _psglDebugChannel;
        _psglDebugChannel = 0; // avoid re-entrance
        switch ( v )
        {
            case 1:
                _jsGcmFifoGlFinish();
                break;
            case 2:
                _jsGcmFifoGlFlush();
                break;
            case 3:
            {
                void *dummy;
                _jsGcmFifoGlIncFenceRef( &dummy );
            }
            break;
            case 4:
#ifdef JS_RSXFIFODUMP
                _jsRSXFifoDumpDebugFinish();
#endif
                break;
            default:
                break;
        }
        _psglDebugChannel = v;
    }
}

#endif

/* determines if the gpu hit a error and needs to be restarted
**
** note:
**   - if a error occurs the channel is usually suspended and has to be recreated to
**     to keep things running...
*/
#ifdef JSGCM_DUMP_FIFO
#include <stdio.h>
static int dumpIndex = 0;
void _jsGcmDumpFifo( void )
{
    jsGcmState *jsGcmSt = &_jsGcmState;
    JSGCM_FIFO_FIONA *fifo = &jsGcmSt->fifo;
    char filename[256];
    snprintf( filename, 256, "/app_home/dump%03d.txt", dumpIndex );
    printf( "dumping fifo contents in %s\n", filename );
    filename[255] = 0;
    FILE *f = fopen( filename, "w" );

    GLuint put = fifo->lastPutWritten - fifo->dmaPushBuffer;
    GLuint get = fifo->dmaControl->Get / 4;
    GLuint ref = fifo->dmaControl->Reference;
    GLuint current = fifo->current - fifo->dmaPushBuffer;
    fprintf( f, "Put=0x%08x(w) Get=0x%08x(w) Current=0x%08x(w) Reference=0x%08x(w)\n", put, get, current, ref );
    for ( unsigned int i = 0;i < fifo->dmaPushBufferSizeInWords;++i )
    {
        if (( i % 8 ) == 0 ) fprintf( f, "\n0x%08x:", i );
        fprintf( f, " %08x", fifo->dmaPushBuffer[i].u );
    }
    fprintf( f, "\n" );
    fclose( f );
}
#endif


/* Setup gl enviroment.
**
** note:
**  - the returned host memory is for testing purpose
**    only (glMemCpy* etc)
*/
GLboolean _jsGcmInitFromRM( jsGcmResource *rmResource )
{
    jsGcmState *jsGcmSt = &_jsGcmState;
    memset( jsGcmSt, 0, sizeof( *jsGcmSt ) );

    jsGcmSt->localAddress = rmResource->localAddress;
    jsGcmSt->hostMemoryBase = rmResource->hostMemoryBase;
    jsGcmSt->hostMemorySize = rmResource->hostMemorySize;

    jsGcmSt->hostNotifierBuffer = NULL; //rmResource->hostNotifierBuffer;
    jsGcmSt->semaphores = rmResource->semaphores;

#ifdef JS_RSXFIFODUMP
    _jsRSXFifoDumpInit();
#endif

    // Init the jsGcm fifo
    int _psglDebugChannel_save = _psglDebugChannel;
    _psglDebugChannel = 0; // disable debug channel for initialization

    _jsGcmFifoInit( &jsGcmSt->fifo, rmResource->dmaControl, rmResource->dmaPushBufferOffset, ( jsFifoData* )rmResource->dmaPushBuffer, rmResource->dmaPushBufferSize );

    _jsGcmFifoFinish( &jsGcmSt->fifo );
    
    _psglDebugChannel = _psglDebugChannel_save;

    // Initialize HUD
#ifdef GCM_HUD
    cellGcmHUDInit();
#endif
    
    // Set the GPU to a known state
    _jsGcmSetOpenGLState( jsGcmSt );
    
    // wait for setup to complete
    _jsGcmFifoFinish( &jsGcmSt->fifo );

    return GL_TRUE;
}


void _jsGcmDestroy( void )
{
    jsGcmState *jsGcmSt = &_jsGcmState;
    memset( jsGcmSt, 0, sizeof( *jsGcmSt ) );
}
