/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * jsGcmDriver struct definition and related function declarations
 */

#ifndef JSGCM_DRIVER_H
#define JSGCM_DRIVER_H

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Thread.h>
#include <PSGL/Utils.h>
#include <Cg/CgCommon.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmDevice.h>
#include <JSGCM/JSGcmShader.h>
#include <JSGCM/JSGcmFPShader.h>
#include <JSGCM/JSGcmFPShaderMan.h>
#include <JSGCM/ShaderPool.h>

extern void _jsVMXCopyWithoutPrealloc( char* dst, const char* src, size_t size );

#define _JSGCM_MAX_COLOR_SURFACES 4

typedef struct _jsGcmDriver_
{

    jsGcmRenderTargetEx rt;
    GLuint colorBufferMask;
    GLboolean rtValid;
    GLboolean invalidateVertexCache;
    int xSuperSampling; // supersampling factor in X
    int ySuperSampling; // supersampling factor in Y

    GLuint flushBufferCount;	// # of mapped buffer objects in bounce buffer

    GLuint fpLoadProgramId; // address of the currently bound fragment program
    GLuint fpLoadProgramOffset;

    GLuint queryCount;
    GLboolean query[JSGCM_MAX_USER_QUERIES];
    GLuint queryFence[JSGCM_MAX_USER_QUERIES];

    CGprogram vsFixedPipeline;

    CGparameter viewXform;
    CGparameter projXform;
    CGparameter normXform;

    CGparameter flags;
    CGparameter normalScale;

    CGparameter useTexMatrix;

    struct
    {
        CGparameter Ka;
        CGparameter Kd;
        CGparameter Ks;
        CGparameter Ke;
        CGparameter shininess;
    }
    material;

    CGparameter globalAmbient;

    CGparameter nLights;
    struct
    {
        CGparameter position;
        CGparameter ambient;
        CGparameter diffuse;
        CGparameter specular;
        CGparameter spotDirection;
        CGparameter spotParams;
        CGparameter attenuation;
    }
    light[_JS_MAX_LIGHTNUM];

    struct
    {
        CGparameter genMode;	// texgen mode
        CGparameter genPlane;	// either object or linear
        CGparameter matrix;		// texture matrix
    }
    texunit[_JS_MAX_TEXTURE_UNITS];

    CGparameter clipPlaneActive;
    CGparameter clipPlane[_JS_MAX_CLIP_PLANES];

    GLuint sharedFPConstantsId;
    char *sharedVPConstants;

    jsGcmFPShaderManager shaderManager;
    _JS_LIST_DEFINE( pools, jsCgShaderPool )
}
jsGcmDriver;

inline static jsGcmDriver *_jsGetGcmDriver()
{
    JS_ASSERT( _CurrentDevice );
    JS_ASSERT( _CurrentDevice->rasterDriver );
    return ( jsGcmDriver * )( _CurrentDevice->rasterDriver );
}

// methods from the 3d class we are setting per texture image unit.
// 2 methods are not cached, because we do not expose them:
// - CONTROL2 (for brilinear/aniso)
// - BORDER_COLOR
typedef struct
{
    GLuint SET_TEXTURE_CONTROL3; // pitch and depth
    GLuint SET_TEXTURE_OFFSET; // gpu addr (from dma ctx)
    GLuint SET_TEXTURE_FORMAT; // which dma ctx, [123]D, border source, mem layout, mip levels
    GLuint SET_TEXTURE_ADDRESS; // wrap, signed and unsigned remap control, gamma, zfunc
    GLuint SET_TEXTURE_CONTROL0; // enable, lod clamp, aniso, image field, alpha kill, colorkey
    GLuint SET_TEXTURE_CONTROL1; // remap and crossbar setup.
    GLuint SET_TEXTURE_FILTER; // lod bias, convol filter, min/mag filter, component signedness
    GLuint SET_TEXTURE_IMAGE_RECT; // texture width/height
}
jsGcmTextureMethods;

// Gcm Specific function parameter mappings.
// for cellGcmSetControl
struct jsGcmTextureControl0
{
    GLuint minLOD;
    GLuint maxLOD;
    GLuint maxAniso;
};

// for cellGcmSetAddress
struct jsGcmTextureAddress
{
    GLuint wrapS;
    GLuint wrapT;
    GLuint wrapR;
    GLuint unsignedRemap;
    GLuint zfunc;
    GLuint gamma;
};

// for cellGcmSetTextureFilter
struct jsGcmTextureFilter
{
    GLuint 	min;
    GLuint 	mag;
    GLuint 	conv;
    GLint  bias;
};

// Structure to contain Gcm Function Parameters for setting later.
// Control1 and Control3 will be set by cellGcmSetTexture
struct jsGcmTextureMethodParams
{
    jsGcmTextureControl0 control0;
    jsGcmTextureAddress  address;
	jsGcmTextureFilter   filter;
	GLuint borderColor; // texture border color

};


typedef struct
{
    // These are enough to describe the GPU format
    GLuint 		faces;
    GLuint 		levels;
    GLuint 		baseWidth;
    GLuint 		baseHeight;
    GLuint 		baseDepth;
    jsGcmEnum 	internalFormat;
    GLuint 		pixelBits;
    GLuint 		pitch;
}
jsGcmTextureLayout;

// GCM texture data structure
typedef struct
{
    GLenum pool;

    jsGcmTextureMethods 		methods; // [RSTENSON] soon to be legacy

    jsGcmTextureMethodParams 	gcmMethods;
    CellGcmTexture		  		gcmTexture;

    GLuint 						gpuAddressId;
    GLuint                      gpuAddressIdOffset;
    GLuint 						gpuSize;

    jsGcmTextureLayout 			gpuLayout;

    // pixel buffer object
    //  If this member is non-NULL, then gpuAddress points to memory within
    //  this PBO.
    jsBufferObject* pbo;
}
jsGcmTexture;

int _jsGcmLoadFPShader( jsGcmShader *shader );
void _jsGcmUnloadFPShader( jsGcmShader *shader );
void _jsGcmSetParameter4fv( jsGcmShader *shader, CgBinaryParameter *p, const GLfloat *params );
void _jsGcmSetParameterHost4fv( jsGcmShader *shader, CgBinaryParameter *p, const GLfloat *params );

void _jsPlatformDirtySurfaces();

void _jsVertexTexImage2D( GLuint unit, GLuint levels, GLuint width, GLuint height, GLenum internalFormat, GLuint pitch, GLboolean isMain, GLuint gpuOffset );
void _jsVertexTexParameters( GLuint unit, GLboolean enable, GLenum wrapS, GLenum wrapT, GLfloat lodBias, GLfloat minLod, GLfloat maxLod );

void _jsPlatformValidateTextureResources( jsTexture *texture );

GLuint _jsGetGcmImageOffset( jsGcmTextureLayout *layout, GLuint face, GLuint level );

int _jsGcmGetFramebufferSurface( GLenum format, jsGcmSurface* Surface );

// Push a CG program onto the current command buffer
static inline void _jsGcmPushProgramPushBuffer( _CGprogram * cgprog )
{
	 // make sure there is space for the pushbuffer + any nops we need to add for alignment  
    _jsGcmFifoWaitForFreeSpace( &_jsGcmState.fifo,  cgprog->constantPushBufferWordSize + 4 + 32); 
	// first add nops to get us the next alligned position in the fifo 
	// [YLIN] Use VMX register to copy
	uint32_t padding_in_word = ( ( 0x10-(((uint32_t)_jsGcmState.fifo.current)&0xf))&0xf )>>2;
	uint32_t padded_size = ( ((cgprog->constantPushBufferWordSize)<<2) + 0xf )&~0xf;
	GCM_FUNC( cellGcmSetNopCommandUnsafe, padding_in_word );
	memcpy16(_jsGcmState.fifo.current, cgprog->constantPushBuffer, padded_size);
	_jsGcmState.fifo.current+=cgprog->constantPushBufferWordSize;
	
}

// Look up the memory location of a buffer object (VBO, PBO)
static inline GLuint _jsGcmGetBufferObjectOrigin( GLuint buffer )
{
    jsBufferObject *bufferObject = _jsGetBufferObject( _CurrentContext, buffer );
    JS_ASSERT( bufferObject );
    jsGcmBufferObject *gcmBuffer = ( jsGcmBufferObject * ) & bufferObject->platformBufferObject;
    return gcmBuffer->bufferId;
}

// This function is strictly for fifo debugging purposes
inline void _jsGcmPushNop( int n )
{
    cellGcmSetNopCommand( n );
}


#endif
