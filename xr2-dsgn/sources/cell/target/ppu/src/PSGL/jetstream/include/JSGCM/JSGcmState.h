/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * JSGcmState to maintain our JSGCM context state and memory info separate from the PSGL Context
 */



#ifndef _JSGCM_STATE_H_
#define _JSGCM_STATE_H_

#include <cell/gcm.h>
#include <JSGCM/JSGcmResource.h>
#include <JSGCM/JSGcmConsts.h>
#include <JSGCM/JSGcmGlEnums.h>
#include <JSGCM/JSGcmFifo.h>

using namespace cell::Gcm;

// GCM can render to 4 color buffers at once.
#define JSGCM_SETRENDERTARGET_MAXCOUNT  4

// the current rendering surface/target
    typedef struct jsGcmRenderTarget jsGcmRenderTarget;
    struct jsGcmRenderTarget
    {
        GLuint  colorFormat;
        GLuint  colorBufferCount;

        GLuint  depthFormat;

        // swizzled surface target
        GLuint  swizzled;

        // (0,0) is in the lower left
        GLuint  yInverted;

        // AntiAliasing option: amount of samples used (1, 2, 4)
        GLuint  aaSamples;

        // by default we swap the dac using SCANOUT_FILTER_TAPS_NO_CHANGE.
        // If we change between AA/non-AA we need to reconfigure the dac
        GLuint  scanOutFilterTap;

        // the color writes enables depends on the render surface format
        GLbyte  writeR, writeG, writeB, writeA;

        // color only rendering implies disabled depth test
        GLuint  depthTest;

        // gcm render target structure [RSTENSON]
        CellGcmSurface  gcmRenderTarget;
    };

// cached state: texture
    typedef struct jsGcmTextureState jsGcmTextureState;
    struct jsGcmTextureState
    {
        // unforunately to many pieces of state have been put into single
        // 32bit registers -- so we need to cache some of them...
        GLuint hwTexAddress;
        GLuint hwTexFilter;
        GLuint hwTexControl0;
        //GLuint hwTexCoordCtrl;

    };

// cached state: viewport
    typedef struct jsGcmViewportState jsGcmViewportState;
    struct jsGcmViewportState
    {
        // user values given as input to glViewport
        GLint x, y, w, h;

        // from glDepthRange
        GLfloat zScale, zCenter;

        // from glViewport
        GLfloat xScale, xCenter;
        GLfloat yScale, yCenter;

        // current frontFace
        GLuint frontFace;
    };

// cached state: scissor
    typedef struct jsGcmScissorState jsGcmScissorState;
    struct jsGcmScissorState
    {
        // user values given as input to glScissor()
        GLint x, y, w, h;

        // for reload: need to know if scissor was enabled
        GLuint enabled;
    };

// cached state: clear
    typedef struct jsGcmClearState jsGcmClearState;
    struct jsGcmClearState
    {
        // current clear color
        GLfloat r, g, b, a;

        // we do not have separate method for setting depth and z
        // see glClearStencil, glClearDepth
        GLuint hwDepthStencil;
    };

// cached state: blend
    typedef struct jsGcmBlendState jsGcmBlendState;
    struct jsGcmBlendState
    {
        // current blend color
        GLfloat r, g, b, a;

        // alpha blend reference
        GLuint alphaFunc;
        GLfloat alphaRef;
    };

// cached state: stencil
    typedef struct jsGcmStencilState jsGcmStencilState;
    struct jsGcmStencilState
    {
        // need to track the current 2sd-stencil face (0=front, 1=back)
        GLuint activeStencilFace;

        // the stencil hint we use on the next depth/stencil clear
        GLuint hwStencilCullHint;
    };

// cached state: stencil
    typedef struct jsGcmQueryState jsGcmQueryState;
    struct jsGcmQueryState
    {
        // GL_SAMPLES_PASSED support
        GLuint currentZPassQueryId;
    };

// cached state: stencil
    typedef struct jsGcmZCullState jsGcmZCullState;
    struct jsGcmZCullState
    {
        // last completed block in the search.
        GLfloat lastCulled;

        // x value
        GLint lastMoveForward;

        // stepsize associated with current block
        GLfloat lastStep;

        // percentage culled by predecessor of "Last"
        GLfloat oldCulled;

        // zcull tweakables...
        GLuint hwControl0;
        GLuint hwControl1;

        GLuint disableClearStats;
    };

    typedef struct jsGcmInterpolantState jsGcmInterpolantState;
    struct jsGcmInterpolantState
    {
        // mask of inputs used by programs
        //  Uses bits from SET_VERTEX_ATTRIB_OUTPUT_MASK.
        GLuint vertexProgramAttribMask;
        GLuint fragmentProgramAttribMask;

        // mask of active clip planes
        //  Uses bits from SET_VERTEX_ATTRIB_OUTPUT_MASK.
        GLuint clipMask;

        // mask for one or two sided lighting
        //  Contains front diffuse and specular bits (for one-sided lighting) or
        //  both front and back diffuse and specular bits (for two-sided
        //  lighting).
        GLuint sideMask;

        // mask for point sprite
        GLuint pointSpriteMask;
    };

// cached state (because no dedecated method exist)
    typedef struct jsGcmCachedState jsGcmCachedState;
    struct jsGcmCachedState
    {
        // our hw<->ogl mapping is ...let's say strange...
        //jsGcmTextureState tex[JSGCM_MAX_TEXIMAGE_COUNT];
        //[RSTENSON] Removing this above.  Texturing is all GCM now.

        // we do not have separate method for setting depth and z
        jsGcmClearState clear;

        // we need to track blending color, too
        jsGcmBlendState blend;

        // there is no stencil_test_enable, need to track scissor values
        jsGcmScissorState scissor;

        // need to cache viewport values, because of yInverted
        jsGcmViewportState viewport;

        // the 2sd-stencil spec says we need to cache the current face :(
        jsGcmStencilState stencil;

        // zcull parameter adjustment
        jsGcmZCullState zcull;

        // all interpolants are enabled/disabled with a single mask
        jsGcmInterpolantState interpolant;

        // query stuff
        jsGcmQueryState queries;
    };

// used for debug purpose only: state we track for hw consistency checks
    typedef struct jsGcmConsistencyTracker
    {
        // pixel shader output precison has to match with the color buffer precison
        GLuint isPShaderPrecisonR;
    }
    jsGcmConsistencyTracker;

// gleSetRenderTarget has enough arguments to define its own struct
    typedef struct jsGcmRenderTargetEx jsGcmRenderTargetEx;
    struct jsGcmRenderTargetEx
    {
        // color buffers
        jsGcmEnum   colorFormat;
        GLuint      colorBufferCount;

        GLuint      colorId[JSGCM_SETRENDERTARGET_MAXCOUNT];
        GLuint      colorIdOffset[JSGCM_SETRENDERTARGET_MAXCOUNT];
        GLuint      colorPitch[JSGCM_SETRENDERTARGET_MAXCOUNT];

        // depth buffer
        jsGcmEnum   depthFormat;
        GLuint      depthId;
        GLuint      depthIdOffset;
        GLuint      depthPitch;

        // target is swizzled
        GLboolean   swizzled;

        // (0,0) is in the lower left
        GLboolean   yInverted;

        // window offset
        GLuint      xOffset;
        GLuint      yOffset;

        // render dimensions
        GLuint      width;
        GLuint      height;

        // aa samples
        GLuint      aaSamples;
    };

	// ** the master instance representing a channel/context **
    struct jsGcmState
    {
        char *localAddress;

        // host memory window the gpu can access
        void 						*hostMemoryBase;
        GLuint 						hostMemorySize;

        // all kind of notifers
        jsGcmHostNotifierMemory    *hostNotifierBuffer;

   
        // semaphores
        jsGcmSemaphoreMemory        *semaphores;

        // -- context state --

        // fifo
        jsGcmFifo               fifo;

		// for record and playback of static command buffers in psgl [RSTENSON]
		GLuint								recordFifoMode;
		GLuint								recordFifoSizeInWords;
		GLuint *							recordPrevBegin;	
		GLuint *							recordPrevEnd;	
		GLuint *							recordPrevCurrent;
		psglStaticCommandBufferCallBack		recordOutOfSpaceCallback; 
		psglStallCallBack					recordStallCallback; 

        // rendering target
        jsGcmRenderTarget       renderTarget;

        // state
        jsGcmCachedState        state;

        // shadow state for consitency checks
        jsGcmConsistencyTracker consistency;

        // Cell Gcm Config
        CellGcmConfig config;

		// [RSTENSON] need query report structure to check inline transfer completion 
		// to sync report information 

        volatile 		GLuint * 		reportCompletionValues;
		GLuint 			reportCompletionOffset; 

		GLboolean			reportsInMainMemory; 
		CellGcmReportData *	reportAddressMain;
		GLuint				totalMainMemoryReports;
		GLuint				totalPSGLMainMemoryReports; 

		// to use as the back end label value when syncing before cellGcmSetTile, SetZCull, and SetInvalidateTile. 
		GLuint			labelValue; 
		
		// used to store zCull values 
        GLuint			zCull[4]; 

		// need a static clear value to dma with the inlineTransfer.  This will due. 
		GLuint 			 reportCompletionValue __attribute__ ((aligned ( 32 ))); ;

	 
    };

	// the _jsGcmState.reportCompletionValue will be initialized to this 
	#define JSGCM_REPORT_COMPLETE_VALUE 12345 
	
	// the global object (faster then passing the argument around all the time)
    extern jsGcmState _jsGcmState;



    GLboolean   _jsGcmInitFromRM( jsGcmResource * resource );
    void        _jsGcmDestroy( void );
    void        _jsGcmInitChannelState( jsGcmState *state );
    void        _jsGcmSetOpenGLState( jsGcmState *state );
    void        _jsGcmInitBlitterContextDMAs();
    void        _jsGcmInit3DContextDMAs();


#endif
