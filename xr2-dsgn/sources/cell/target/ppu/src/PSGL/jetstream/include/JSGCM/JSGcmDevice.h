/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Device specific functions, datatypes and structures used by PSGL
 */

#ifndef _JSGCM_DEVICE_H_
#define _JSGCM_DEVICE_H_

#include <PSGL/psgl.h>
#include <PSGL/Thread.h>
#include <JSGCM/JSGcmUtils.h>

#ifdef __cplusplus
extern "C"
{
#endif

// This is the structure format in which we the mode timing tables
// in this DLL are stored. This should never need to be changed.
    typedef struct  _tagMODESTRUC
    {
        GLushort  wHorizVisible;
        GLushort  wVertVisible;
        GLushort  wInterlacedMode;
        GLushort  wRefresh;
        GLushort  wHorizTotal;
        GLushort  wHorizBlankStart;
        GLushort  wHorizSyncStart;
        GLushort  wHorizSyncEnd;
        GLushort  wHorizBlankEnd;
        GLushort  wVertTotal;
        GLushort  wVertBlankStart;
        GLushort  wVertSyncStart;
        GLushort  wVertSyncEnd;
        GLushort  wVertBlankEnd;
        GLuint      dwDotClock;      // In 10K Hertz
        GLushort  wHSyncPolarity;
        GLushort  wVSyncPolarity;
    }
    MODESTRUC;

#define BUFFER_HSYNC_NEGATIVE           0
#define BUFFER_HSYNC_POSITIVE           1
#define BUFFER_VSYNC_NEGATIVE           0
#define BUFFER_VSYNC_POSITIVE           1

// This is the format that the mode timing parameters are handed back

    enum {
        _JSGCM_SURFACE_SOURCE_TEMPORARY,
        _JSGCM_SURFACE_SOURCE_DEVICE,
        _JSGCM_SURFACE_SOURCE_TEXTURE,
        _JSGCM_SURFACE_SOURCE_RENDERBUFFER,
        _JSGCM_SURFACE_SOURCE_PBO,
    };

    enum {
        _JSGCM_SURFACE_POOL_NONE,
        _JSGCM_SURFACE_POOL_TILED_COLOR,
        _JSGCM_SURFACE_POOL_TILED_DEPTH,
        _JSGCM_SURFACE_POOL_LINEAR,
        _JSGCM_SURFACE_POOL_SYSTEM,		// GPU accessible host memory
        _JSGCM_SURFACE_POOL_PPU,		// generic EA
        _JSGCM_SURFACE_POOL_SYSTEM_TILED_COLOR, // tiled color GPU accessible XDR 
        _JSGCM_SURFACE_POOL_SYSTEM_TILED_DEPTH, // tiled depth GPU accessible XDR 
    };

// descriptor for 2D data
    typedef struct
    {
        GLenum source;	// device, texture, renderbuffer

        GLuint width, height;
        GLuint bpp;		// bytes per pixel, derived from format
        GLuint pitch;	// 0 if swizzled

        GLuint samples;	// antialiasing
        jsGcmEnum format;	// e.g. JSGCM_ARGB8
        GLboolean flip;	// top to bottom if true (set on device source)

        GLenum pool;	// type of memory

        char* ppuData; // generic EA that's not RSX mem

        GLuint dataId;		// id to get address and offset
        GLuint dataIdOffset;
    }
    jsGcmSurface;

#define _JSGCM_DEVICE_SYNC_FENCE 1
#define _JSGCM_DEVICE_SYNC_COND  2

    typedef struct
    {
        // TODO: get rid of this member
        jsGcmRenderTargetEx rt;

        // framebuffers
        jsGcmSurface color[3];
        jsGcmSurface depthStencil;
        jsGcmSurface scanoutColor[3];

        // double/triple buffering management
        GLuint drawBuffer;	// 0, 1, or 2
        GLuint scanBuffer;	// 0, 1, or 2

        // resc buffers (maintain pointers for freeing in _jsPlatformDestroyDevice)
        GLuint RescColorBuffersId;
        GLuint RescVertexArrayId;
        GLuint RescFragmentShaderId;

//    int aa;
//    int width;
//    int height;
//    int bpp;
        const MODESTRUC *ms;
        GLboolean vsync;
        GLboolean skipFirstVsync;
		GLuint    skipFirstVsyncCountDown; // Count down for shut down
        GLenum deviceType;
        GLenum TVStandard;
        GLenum TVFormat;
        GLuint swapFifoRef;
        GLuint swapFifoRef2; // Added for supporting Triple buffering [RSTENSON]
        jsMutex *mutex;
        jsCond *cond;
        GLboolean setOffset;
        GLboolean signal;
        GLuint semaValue;
        unsigned int syncMethod;

        // User flip callback
        void (*flipHandler)(const uint32_t head);
    }
    jsGcmDevice;

    extern void _jsGcmCopyGPUToGPU( const jsGcmSurface* src, GLuint srcX, GLuint srcY,
                                        const jsGcmSurface* dst, GLuint dstX, GLuint dstY, GLuint width, GLuint height );
    extern void _jsGcmCopyGPUToPPU( const jsGcmSurface* src, GLuint srcX, GLuint srcY,
                                           const jsGcmSurface* dst, GLuint dstX, GLuint dstY, GLuint width, GLuint height );
    extern void _jsGcmFlipGPUToGPU( const jsGcmSurface* src, GLuint srcX, GLuint srcY,
                                        const jsGcmSurface* dst, GLuint dstX, GLuint dstY, GLuint width, GLuint height );

// This utility function copies whole 2D regions described with the
// jsGcmSurface struct.
    void _jsGcmCopySurface( const jsGcmSurface* src, GLuint srcX, GLuint srcY,
                            const jsGcmSurface* dst, GLuint dstX, GLuint dstY, GLuint width, GLuint height,
                            GLboolean writeSync );	// don't bypass GPU pipeline

#ifdef __cplusplus
}
#endif
#endif // _JSGCM_DEVICE_H_
