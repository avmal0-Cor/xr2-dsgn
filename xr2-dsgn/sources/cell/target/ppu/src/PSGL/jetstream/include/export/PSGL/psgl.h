/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef _PSGL_h
#define _PSGL_h

#include <stdlib.h>
#include <PSGL/export.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <PSGL/report.h>

#include <cell/cgb/cgb_struct.h>
#include <cell/resc.h>

#include <sdk_version.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct PSGLdevice PSGLdevice;
typedef struct PSGLcontext PSGLcontext;

typedef enum PSGLtvStandard
{
    PSGL_TV_STANDARD_NONE,
    PSGL_TV_STANDARD_NTSC_M,
    PSGL_TV_STANDARD_NTSC_J,
    PSGL_TV_STANDARD_PAL_M,
    PSGL_TV_STANDARD_PAL_B,
    PSGL_TV_STANDARD_PAL_D,
    PSGL_TV_STANDARD_PAL_G,
    PSGL_TV_STANDARD_PAL_H,
    PSGL_TV_STANDARD_PAL_I,
    PSGL_TV_STANDARD_PAL_N,
    PSGL_TV_STANDARD_PAL_NC,
    PSGL_TV_STANDARD_HD480I,
    PSGL_TV_STANDARD_HD480P,
    PSGL_TV_STANDARD_HD576I,
    PSGL_TV_STANDARD_HD576P,
    PSGL_TV_STANDARD_HD720P,
    PSGL_TV_STANDARD_HD1080I,
    PSGL_TV_STANDARD_HD1080P,
    PSGL_TV_STANDARD_1280x720_ON_VESA_1280x768 = 128,
    PSGL_TV_STANDARD_1280x720_ON_VESA_1280x1024,
    PSGL_TV_STANDARD_1920x1080_ON_VESA_1920x1200,
} PSGLtvStandard;

typedef enum PSGLbufferingMode
{
    PSGL_BUFFERING_MODE_SINGLE = 1,
    PSGL_BUFFERING_MODE_DOUBLE = 2,
    PSGL_BUFFERING_MODE_TRIPLE = 3,
} PSGLbufferingMode;

typedef enum PSGLdeviceConnector
{
    PSGL_DEVICE_CONNECTOR_NONE,
    PSGL_DEVICE_CONNECTOR_VGA,
    PSGL_DEVICE_CONNECTOR_DVI,
    PSGL_DEVICE_CONNECTOR_HDMI,
    PSGL_DEVICE_CONNECTOR_COMPOSITE,
    PSGL_DEVICE_CONNECTOR_SVIDEO,
    PSGL_DEVICE_CONNECTOR_COMPONENT,
} PSGLdeviceConnector;

typedef enum RescRatioMode
{
    RESC_RATIO_MODE_FULLSCREEN,
    RESC_RATIO_MODE_LETTERBOX,  // default
    RESC_RATIO_MODE_PANSCAN,
} RescRatioMode;

typedef enum RescPalTemporalMode
{
    RESC_PAL_TEMPORAL_MODE_50_NONE,  // default - no conversion
    RESC_PAL_TEMPORAL_MODE_60_DROP,
    RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE,
    RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_30_DROP,
    RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_DROP_FLEXIBLE,
} RescPalTemporalMode;

typedef enum RescInterlaceMode
{
    RESC_INTERLACE_MODE_NORMAL_BILINEAR,
    RESC_INTERLACE_MODE_INTERLACE_FILTER,
} RescInterlaceMode;

#define PSGL_DEVICE_PARAMETERS_COLOR_FORMAT             0x0001
#define PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT             0x0002
#define PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE       0x0004
#define PSGL_DEVICE_PARAMETERS_TV_STANDARD              0x0008
#define PSGL_DEVICE_PARAMETERS_CONNECTOR                0x0010
#define PSGL_DEVICE_PARAMETERS_BUFFERING_MODE           0x0020
#define PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT             0x0040
#define PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT 0x0080
#define PSGL_DEVICE_PARAMETERS_RESC_RATIO_MODE          0x0100
#define PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE   0x0200
#define PSGL_DEVICE_PARAMETERS_RESC_INTERLACE_MODE      0x0400
#define PSGL_DEVICE_PARAMETERS_RESC_ADJUST_ASPECT_RATIO 0x0800

// mask for validation
#define PSGL_VALIDATE_NONE                         0x00000000 
#define PSGL_VALIDATE_FRAMEBUFFER                  0x00000001             
#define PSGL_VALIDATE_TEXTURES_USED                0x00000002 
#define PSGL_VALIDATE_VERTEX_PROGRAM               0x00000004 
#define PSGL_VALIDATE_VERTEX_CONSTANTS 			   0x00000008
#define PSGL_VALIDATE_VERTEX_TEXTURES_USED 		   0x00000010   
#define PSGL_VALIDATE_FFX_VERTEX_PROGRAM 		   0x00000020
#define PSGL_VALIDATE_FRAGMENT_PROGRAM 			   0x00000040
#define PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM 		   0x00000080
#define PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS    0x00000100
#define PSGL_VALIDATE_VIEWPORT 					   0x00000200
#define PSGL_VALIDATE_ALPHA_TEST 				   0x00000400
#define PSGL_VALIDATE_DEPTH_TEST 				   0x00000800
#define PSGL_VALIDATE_WRITE_MASK 				   0x00001000
#define PSGL_VALIDATE_STENCIL_TEST 				   0x00002000
#define PSGL_VALIDATE_STENCIL_OP_AND_MASK 		   0x00004000
#define PSGL_VALIDATE_SCISSOR_BOX 				   0x00008000
#define PSGL_VALIDATE_FACE_CULL 				   0x00010000
#define PSGL_VALIDATE_BLENDING 					   0x00020000
#define PSGL_VALIDATE_POINT_RASTER 				   0x00040000
#define PSGL_VALIDATE_LINE_RASTER 				   0x00080000
#define PSGL_VALIDATE_POLYGON_OFFSET 			   0x00100000
#define PSGL_VALIDATE_SHADE_MODEL 				   0x00200000
#define PSGL_VALIDATE_LOGIC_OP 					   0x00400000
#define PSGL_VALIDATE_MULTISAMPLING 			   0x00800000
#define PSGL_VALIDATE_POLYGON_MODE 				   0x01000000
#define PSGL_VALIDATE_PRIMITIVE_RESTART 		   0x02000000
#define PSGL_VALIDATE_CLIP_PLANES 				   0x04000000
#define PSGL_VALIDATE_SHADER_SRGB_REMAP 		   0x08000000
#define PSGL_VALIDATE_POINT_SPRITE 				   0x10000000
#define PSGL_VALIDATE_TWO_SIDE_COLOR 			   0x20000000
#define PSGL_VALIDATE_ALL 						   0x3FFFFFFF
	
typedef struct
{
    GLuint enable;
    GLenum colorFormat;
    GLenum depthFormat;
    GLenum multisamplingMode;
    PSGLtvStandard TVStandard;
    PSGLdeviceConnector connector;
    PSGLbufferingMode bufferingMode;
    GLuint width;   // dimensions of display device (scanout buffer)
    GLuint height;

    // dimensions of render buffer. Only set explicitly if the render target buffer
    // needs to be different size than display scanout buffer (resolution scaling required).
    // These can only be set if PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT is set in the "enable" mask,
    // otherwise, render buffer dimensions are set to device dimensions (width/height).
    GLuint renderWidth;
    GLuint renderHeight;

    RescRatioMode rescRatioMode;             // RESC aspect ratio rescaling mode: full screen, letterbox, or pan & scan
    RescPalTemporalMode rescPalTemporalMode; // RESC pal frame rate conversion mode: none, drop frame, interpolate
    RescInterlaceMode rescInterlaceMode;     // RESC interlace filter mode: normal bilinear or use the anti-flicker interlace filter

    // horizontal and vertical scaling to adjust for the difference in overscan rates for each SD/HD mode or TV
    GLfloat horizontalScale;
    GLfloat verticalScale;
}
PSGLdeviceParameters;

#define	PSGL_INIT_MAX_SPUS				0x0001
#define	PSGL_INIT_INITIALIZE_SPUS			0x0002
#define	PSGL_INIT_PERSISTENT_MEMORY_SIZE	0x0004
#define	PSGL_INIT_TRANSIENT_MEMORY_SIZE	0x0008
#define	PSGL_INIT_ERROR_CONSOLE			0x0010
#define	PSGL_INIT_FIFO_SIZE				0x0020
#define	PSGL_INIT_HOST_MEMORY_SIZE			0x0040
#define PSGL_INIT_USE_PMQUERIES           0x0080

typedef struct PSGLinitOptions
{
    GLuint			enable;	// bitfield of options to set
    GLuint 			maxSPUs;
    GLboolean		initializeSPUs;
    GLuint			persistentMemorySize;
    GLuint			transientMemorySize;
    int				errorConsole;
    GLuint			fifoSize;
    GLuint			hostMemorySize;
}
PSGLinitOptions;

typedef unsigned long long int PSGLuint64;

typedef void*( *PSGLmallocFunc )( size_t LSize );		// expected to return 16-byte aligned
typedef void*( *PSGLmemalignFunc )( size_t align, size_t LSize );
typedef void*( *PSGLreallocFunc )( void* LBlock, size_t LSize );
typedef void( *PSGLfreeFunc )( void* LBlock );

extern PSGL_EXPORT void	psglInit( PSGLinitOptions* options );
extern PSGL_EXPORT void	psglExit();

PSGL_EXPORT PSGLdevice*	psglCreateDeviceAuto( GLenum colorFormat, GLenum depthFormat, GLenum multisamplingMode );
PSGL_EXPORT PSGLdevice*	psglCreateDeviceExtended( const PSGLdeviceParameters *parameters );
PSGL_EXPORT GLfloat psglGetDeviceAspectRatio( const PSGLdevice * device );
PSGL_EXPORT void psglGetDeviceDimensions( const PSGLdevice * device, GLuint *width, GLuint *height );
PSGL_EXPORT void psglGetRenderBufferDimensions( const PSGLdevice * device, GLuint *width, GLuint *height );
PSGL_EXPORT void psglDestroyDevice( PSGLdevice* device );

PSGL_EXPORT void psglMakeCurrent( PSGLcontext* context, PSGLdevice* device );
PSGL_EXPORT PSGLcontext* psglCreateContext();
PSGL_EXPORT void psglDestroyContext( PSGLcontext* LContext );
PSGL_EXPORT void psglResetCurrentContext();
PSGL_EXPORT PSGLcontext* psglGetCurrentContext();
PSGL_EXPORT PSGLdevice* psglGetCurrentDevice();
PSGL_EXPORT void psglSwap( void );

PSGL_EXPORT void psglLoadShaderLibrary( const char *filename );

PSGL_EXPORT PSGLuint64 psglGetSystemTime();
PSGL_EXPORT PSGLuint64 psglGetLastFlipTime();
PSGL_EXPORT void* psglGetSPUInitData( void );
PSGL_EXPORT void psglSetFlipHandler( void( *handler )( const GLuint head ) );

PSGL_EXPORT GLboolean psglSetAllocatorFuncs( PSGLmallocFunc mallocFunc, PSGLmemalignFunc memalignFunc, PSGLreallocFunc reallocFunc, PSGLfreeFunc freeFunc );

PSGL_EXPORT void psglSetBounceBufferSize( GLsizei size );
PSGL_EXPORT GLsizei psglGetBounceBufferSize();

PSGL_EXPORT void psglAddressToOffset(const void *address, GLuint *offset);
PSGL_EXPORT void psglSetVertexProgramRegister(GLuint reg, const void * __restrict value);
PSGL_EXPORT void psglSetVertexProgramRegisterBlock(GLuint reg, GLuint count, const void * __restrict value);
PSGL_EXPORT void psglSetVertexProgramTransformBranchBits(GLuint values);
PSGL_EXPORT void psglSetVertexProgramConfiguration(const CellCgbVertexProgramConfiguration *conf, const void *ucodeStorageAddress);

PSGL_EXPORT void psglSetFragmentProgramEmbeddedConstant(const GLuint offset, const GLfloat *value, const GLuint sizeInWords);
PSGL_EXPORT void psglSetFragmentProgramConfiguration(const CellCgbFragmentProgramConfiguration *conf);

PSGL_EXPORT void psglSetFragmentProgramEmbeddedConstantMemoryLocation(const GLuint offset, const GLfloat *value, const GLuint sizeInWords, bool inLocalMemory);
PSGL_EXPORT void psglSetFragmentProgramConfigurationMemoryLocation(const CellCgbFragmentProgramConfiguration *conf, bool inLocalMemory);

PSGL_EXPORT void psglSetUserClipPlanes( const GLuint userClipControlMask, const GLuint vertexOutputAttributeMask );

#if (CELL_SDK_VERSION >= 0x180000 )
static inline PSGL_EXPORT void psglRescAdjustAspectRatio( const float horizontalScale, const float verticalScale )
    { cellRescAdjustAspectRatio( horizontalScale, verticalScale ); }
#endif

// New PSGL interfaces for recording of the fifo and replaying in the same frame 
// One caviate though.  The buffer passed in must be mapped by gpu in order for this to work. 
// therefore you should always use psglAllocateCommandBuffer which will allocate at 1mb increments 
// and map the memory as well. 
PSGL_EXPORT void psglBeginCommandRecord( void * commandBuffer, GLuint sizeInBytes );
// returns the current command buffer position 
PSGL_EXPORT void * psglGetCommandRecordCurrent();
// returns the current position in the user memory 
PSGL_EXPORT void * psglEndCommandRecord( bool AddReturn );
// returns the current command buffer position 
PSGL_EXPORT void * psglCallCommandBuffer( void * commandBuffer );
PSGL_EXPORT void psglPushCommandBuffer( void * commandBuffer, GLuint sizeInBytes ); 

// utility function that allocates 1 megabyte aligned memory and maps it so the rsx can access it
PSGL_EXPORT void * psglAllocateCommandBuffer( GLuint sizeInBytes, GLuint * offset );
// another utility to delete this buffer  
PSGL_EXPORT void psglFreeCommandBuffer( void * commandBuffer );

// out of space static command buffer call back when fifo runs out of space 
typedef int32_t (*psglStaticCommandBufferCallBack)(struct CellGcmContextData *, uint32_t);
PSGL_EXPORT void psglSetRecordOutOfSpaceCallback( psglStaticCommandBufferCallBack callback ); 
// stall call back for use when the user has static command buffers that they have called vs pushed
typedef bool (*psglStallCallBack)( void * get, void * nextBegin, void * nextEnd);
PSGL_EXPORT void psglSetStallCallback( psglStallCallBack callBack );

#define PSGL_ADD_RETURN true
#define PSGL_DO_NOT_ADD_RETURN false

#define PSGL_STALL true
#define PSGL_DO_NOT_STALL false

// function to make a command buffer hole specific to draw.
// validates states and attributes in the same way as glDrawElements, and generates a command buffer hole 
// in the main command buffer, instead of pushing an actual draw command
PSGL_EXPORT int psglDrawCommandBufferHole( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, 
										  uint32_t *indexOffset, uint32_t *holeEA, uint32_t *holeSizeInWord );
// This function makes a gereral purpose command buffer hole for Jump To Self sync with specified size
PSGL_EXPORT int psglGenerateCommandBufferHole( uint32_t holeSizeInWord, uint32_t *holeEA );

// This funciton validates states specified in mask
//  mask - specify states to be validated
//  returns mask of states which was NOT validated
PSGL_EXPORT GLuint psglValidateStates( GLuint mask );
// This function invalidates states specified in mask
//  mask - specify states to be invalidateted
PSGL_EXPORT void psglInvalidateStates( GLuint mask );

// This funciton validates attributes with index buffer at specified offset
//  indices - specify offset to index buffer object
//  isMain - true will be set if index buffer object is on main memory, false for local memory
//  returns RSX offset of index buffer
PSGL_EXPORT GLuint psglValidateAttributes( const GLvoid* indices, GLboolean *isMain );
// This function invalidates all attributes
PSGL_EXPORT void psglInvalidateAttributes();

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif
