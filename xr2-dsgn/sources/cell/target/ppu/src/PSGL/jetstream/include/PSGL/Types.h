/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef	_JSTypes_h
#define	_JSTypes_h

#include <stdlib.h>
#include <float.h>

#include <PSGL/psgl.h>
#include <PSGL/Base.h>

#include <Cg/cg.h>

struct jsFramebufferAttachment
{
    GLenum type;	// renderbuffer or texture
    GLuint name;

    // only valid for texture attachment
    GLenum textureTarget;
//    GLint textureLevel;	// not needed for OES
//    GLint textureZOffset;
    jsFramebufferAttachment(): type( GL_NONE ), name( 0 ), textureTarget( GL_NONE )
    {};
};

#define _JS_MAX_COLOR_ATTACHMENTS 4

struct jsFramebuffer
{
    jsFramebufferAttachment color[_JS_MAX_COLOR_ATTACHMENTS];
    jsFramebufferAttachment depth;
    jsFramebufferAttachment stencil;
    GLenum multisampling;
    GLboolean needValidate;
    jsFramebuffer(): multisampling( GL_MULTISAMPLING_NONE_SCE ), needValidate( GL_TRUE )
    {};
    virtual ~jsFramebuffer()
    {};
};


#ifdef __cplusplus
extern "C"
{
#endif

// Module debugging flags
//
#ifdef __JS_DEBUG_ALL
#define __JS_DEBUG_MATRIX_STACK	1
#define __JS_DEBUG_TRANSFORM		1
#define __JS_DEBUG_LIGTHING		1
#define __JS_DEBUG_RASTERIZER		1
#define __JS_DEBUG_TEXTURE		1
#define __JS_DEBUG_TEXGEN		1
#else
#define __JS_DEBUG_MATRIX_STACK	0	// These could come from the build target
#define __JS_DEBUG_TRANSFORM		0
#define __JS_DEBUG_LIGTHING		0
#define __JS_DEBUG_RASTERIZER		0
#define __JS_DEBUG_TEXTURE		0
#define __JS_DEBUG_TEXGEN		0
#endif	// __JS_DEBUG_ALL


// Useful constants
//
#define	jsRADIANS_TO_DEGREES	57.295779513082320877
#define jsDEGREES_TO_RADIANS	0.017453292519943295769

#define JSPI	3.14159265358979323846264
#define JSPIf	3.1415926536f
#define JSINV_PI	(1.0/JSPI)
#define JSINV_PIf	(1.f/JSPIf)

    typedef float		JSFColorComponent;
    typedef float		JSCoordinate;

// bitfield operations
    typedef unsigned int jsBitfield;
#define JSBIT_GET(f,n)			((f) & (1<<(n)))
#define JSBIT_TRUE(f,n)			((f) |= (1<<(n)))
#define JSBIT_FALSE(f,n)		((f) &= ~(1<<(n)))
#define JSBIT_ASSIGN(f,n,val)	do { if(val) JSBIT_TRUE(f,n); else JSBIT_FALSE(f,n); } while(0)


#ifndef MSVC

#define ALIGN16 __attribute__((aligned (16)))
#define _JS_RESTRICT __restrict

#if defined(_WIN64)
    typedef __int64 JSintptr;
    typedef __int64 JSsizeiptr;
#elif defined(__ia64__) || defined(__x86_64__)
    typedef long int JSintptr;
    typedef long int JSsizeiptr;
#else
    typedef long JSintptr;
    typedef long JSsizeiptr;
#endif

#else

#define ALIGN16
#define _JS_RESTRICT
#pragma warning( push )
#pragma warning ( disable : 4200 )
    typedef intptr_t JSintptr;
    typedef size_t JSsizeiptr;
#endif

    typedef struct
    {
        GLfloat	X, Y , Z, W;
    }
    jsPositionXYZW;

    typedef struct
    {
        GLfloat	X, Y , Z;
    }
    jsPositionXYZ;

    typedef struct
    {
        GLfloat	R, G, B, A;
    }
    jsColorRGBAf;

    typedef struct
    {
        int		Type;
        jsColorRGBAf	Ambient;
        jsColorRGBAf	Diffuse;
        jsColorRGBAf	Specular;
        jsColorRGBAf	Emission;
        float		Shininess;
    }
    jsMaterial;


// Used internally for quick identification of shading algorithm
//
#define JSMTL_CONSTANT	0x01
#define JSMTL_LAMBERT	0x02
#define JSMTL_PHONG	0x03


#define JS_LIGHT_SPOT 		0x1
#define JS_LIGHT_DIRECTIONAL	0x2
#define JS_LIGHT_POSITIONAL	0x4

    typedef struct
    {
        jsPositionXYZW	ViewerPosition;		// Position of light source in viewer space
        jsPositionXYZW	Position;
        jsColorRGBAf	Ambient;
        jsColorRGBAf	Diffuse;
        jsColorRGBAf	Specular;

        jsPositionXYZ	SpotDirection;
        jsPositionXYZ	SpotViewerDirection;	// Direction of spot light source in viewer space
        GLfloat 	SpotCutOff;
        GLfloat 	SpotCosCutOff;		// The Cosine of the cutoff of the angle for faster computation
        GLfloat 	SpotExponent;		// Spot Exponent

        GLfloat	QuadraticAttenuation,
        ConstantAttenuation,
        LinearAttenuation;

        GLuint 	Flags;
        int		Active;
    }
    ALIGN16 jsLight;

    typedef struct
    {
        GLfloat Coeff[4];		// plane coefficients
        GLboolean Active;
    }
    jsClipPlane;

    typedef GLfloat *jsMatrixf;

    typedef struct
    {
        jsMatrixf	MatrixStackf;			// Double-precision version of the stack
        int		NumOfMatricesAllocated;		// We have an unlimited Matrix stack, for long IK chains etc.
        int		MatrixStackPtr;
        GLboolean dirty;
    }
    jsMatrixStack;



    typedef struct
    {
        int	X, Y, XSize, YSize;
    }
    jsViewPort;

#define _JS_IMAGE_STORAGE_RASTER	0
#define _JS_IMAGE_STORAGE_BLOCK		1

// image location flags
//  These are flag bits that indicate where the valid image data is.  Data
//  can be valid nowhere, on the host, on the GPU, or in both places.
    enum {
        _JS_IMAGE_DATASTATE_UNSET = 0x0,	// not a flag, just a meaningful 0
        _JS_IMAGE_DATASTATE_HOST  = 0x1,
        _JS_IMAGE_DATASTATE_GPU   = 0x2
    };

// Image data structure
    typedef struct jsImage_
    {
        // isSet indicates whether a gl*TexImage* call has been made on that image,
        // to know whether calling gl*TexSubImage* is valid or not.
        GLboolean isSet;

        GLenum internalFormat;
        GLenum format;
        GLenum type;
        GLsizei width;
        GLsizei height;
        GLsizei depth;
        GLsizei	alignment;

        // image storage
        //  Two types of storage are supported, raster and block.  For raster
        //  storage, the platform driver sets strides (in bytes) between
        //  lines and layers and the library takes care of the rest.  For
        //  block storage, the platform driver sets the block size and
        //  functions for reading and writing blocks to the image.

        //  These values are initially zero, but may be set by the platform
        //  _jsPlatformChooseInternalStorage to specify custom storage
        //  (compressed, swizzled, etc.).  They should be considered
        //  read-only except by the platform driver.
        GLenum storageType;
        GLsizei storageSize;				// minimum allocation
        GLsizei xstride, ystride, zstride;	// strides
        GLuint xblk, yblk, zblk;				// block storage size

        char *data;
        char *mallocData;
        GLsizei mallocStorageSize;
        GLenum dataState;	// valid data location (see enum above)
    }
    jsImage;

// Raster data structure
// This struct is used internally to define 3D raster data for writing
// to or reading from a jsImage.  The GL-level interface for pixel/texel
// level operations always uses a raster, even though the underlying
// platform-specific storage may not be a raster (e.g. compressed
// blocks).  The internal routines _jsRasterToImage and _jsImageToRaster
// convert between the two.
//
// A clean alternative would have been to use jsImage for everything and
// implement a single _jsImageToImage copying function.  However, given
// that one side will always be a raster, the implementation cost was
// not seen as worth the generality.
    typedef struct
    {
        GLenum format;
        GLenum type;
        GLsizei width;
        GLsizei height;
        GLsizei depth;
        GLsizei xstride;
        GLsizei ystride;
        GLsizei zstride;
        void* data;
    }
    jsRaster;

#define _JS_TEXTURE_REVALIDATE_LAYOUT   	0x01
#define _JS_TEXTURE_REVALIDATE_IMAGES		0x02
#define _JS_TEXTURE_REVALIDATE_PARAMETERS	0x04

    typedef struct jsBufferObject jsBufferObject;

// Texture data structure
    typedef struct
    {
        GLuint revalidate;
        GLuint target;
#ifdef JS_REPORT
        GLuint name; // keep the name for reports
#endif

        GLuint minFilter;
        GLuint magFilter;
        GLfloat minLod;
        GLfloat maxLod;
        GLuint baseLevel;
        GLuint maxLevel;
        GLuint wrapS;
        GLuint wrapT;
        GLuint wrapR;
        GLfloat lodBias;
        GLfloat maxAnisotropy;
        GLenum depthMode;
        GLenum compareMode;
        GLenum compareFunc;
        GLuint gammaRemap;
        GLenum usage;
        GLenum aaUsage;

        jsColorRGBAf borderColor;

        GLboolean vertexEnable;
        GLboolean isRenderTarget;
        GLboolean	hasMipmaps;
        // this is valid when the revalidate bits do not have any resource bit set.
        // the validation of the resources update the bit.
        GLboolean   isComplete;

        jsBufferObject *referenceBuffer;
        GLintptr	offset;

        PSGL::Vector<jsFramebuffer *> framebuffers;

        GLuint		imageCount;
        GLuint		faceCount;
        jsImage*	images[6];
        void * platformTexture[]; // C99 flexible array member
    }
    jsTexture;

    typedef struct
    {
        GLfloat 	objectPlane[4];
        GLfloat 	eyePlane[4];
        GLint		enabled;
        GLint		mode;
    }
    ALIGN16 jsTexGen;

    typedef struct
    {
        GLboolean isSet;
//    GLenum condition;	// always GL_ALL_COMPLETED_NV
        void* platformFenceObject[];
    }
    jsFenceObject;

    typedef struct
    {
        GLuint dummy;
        void* platformEventObject[];
    }
    jsEventObject;


#define _JS_TEXTURE_UNIT_REVALIDATE_TEX_GEN			0x01

// For now, we'll use a static array for lights
//
#define _JS_MAX_LIGHTNUM	8
#define _JS_MAX_TEXTURE_COORDS	8
#define _JS_MAX_TEXTURE_COORD_INTERPOLANTS	10
#define _JS_MAX_TEXTURE_IMAGE_UNITS	16
#define _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS	4

#define _JS_MAX_TEXTURE_UNITS 4
#define _JS_MAX_TEXTURE_UNITS_MAX (_JS_MAX_TEXTURE_COORDS>_JS_MAX_TEXTURE_IMAGE_UNITS?_JS_MAX_TEXTURE_COORDS:_JS_MAX_TEXTURE_IMAGE_UNITS)

#define _JS_MAX_TEXTURE_SIZE_LOG2	12
#define _JS_MAX_TEXTURE_SIZE	(1<<_JS_MAX_TEXTURE_SIZE_LOG2)
#define _JS_MAX_TEXTURE_SIZE_3D_LOG2	9
#define _JS_MAX_TEXTURE_SIZE_3D	(1<<_JS_MAX_TEXTURE_SIZE_3D_LOG2)
#define _JS_MAX_MODELVIEW_STACK_DEPTH 16
#define _JS_MAX_PROJECTION_STACK_DEPTH 2
#define _JS_MAX_TEXTURE_STACK_DEPTH 2
#define _JS_MAX_TEXTURE_GEN_COORDS	4

#define _JS_MAX_VERTEX_ATTRIBS	16

#define _JS_MAX_CLIP_PLANES	6

// Texture image unit data structure
    typedef struct
    {
        GLuint		bound2D;
        GLuint		bound3D;
        GLuint		boundCube;

        jsTexture*	default2D;
        jsTexture*	default3D;
        jsTexture*	defaultCube;

        GLboolean	enable2D;
        GLboolean	enable3D;
        GLboolean	enableCube;

        // the current fragment program's target for this unit, if in use.
        // this is invalid otherwise
        GLenum      fragmentTarget;

        GLenum		envMode;
        jsColorRGBAf	envColor;
        GLfloat		lodBias;

        jsTexture* currentTexture;
    }
    jsTextureImageUnit;

// Texture coordinates unit data structure
    typedef struct
    {
        GLuint		revalidate;
        jsTexGen	texGen[_JS_MAX_TEXTURE_GEN_COORDS];

        jsMatrixStack	TextureMatrixStack;

        // computed values
        GLuint		texGenSize;
        GLboolean	enableTextureMatrix;
    }
    jsTextureCoordsUnit;

//typedef struct
//{
//    GLint surfaceID;
//    GLint width;
//    GLint height;
//    GLenum type_format;
//    GLenum scan_format;
//    GLuint flags;
//    GLint bpp;
//    GLboolean setup;
//    GLboolean dataValid;
//    GLboolean dirty;
//    void *data;
//    GLuint size;
//    void* platformSurface[];
//} JsSurface;

    enum
    {
        _JS_FRAMEBUFFER_ATTACHMENT_NONE,
        _JS_FRAMEBUFFER_ATTACHMENT_RENDERBUFFER,
        _JS_FRAMEBUFFER_ATTACHMENT_TEXTURE,
    };

    typedef struct
    {
        GLuint result;
        GLenum queryType;
        void *dpmQuery;
        char platformQuery[];
    }
    jsQuery;

    struct PSGLdevice
    {
        PSGLdeviceParameters deviceParameters;

        GLvoid*			rasterDriver;
        char			platformDevice[]; // C99 flexible array member
    };

    typedef struct
    {
        jsColorRGBAf	Ambient;
        GLboolean 		Light2Sided;
    }
    jsLightModel;

    typedef struct
    {
        // parameters to glDraw* calls
        GLenum	mode;
        GLint	firstVertex;
        GLsizei	vertexCount;
        GLuint	indexMin;
        GLuint	indexMax;		// 0==glDrawElements; 0!=glDrawRangeElements
        GLsizei	indexCount;
        GLenum	indexType;		// 0 == non-indexed (ie glDrawArrays)
        const GLvoid*	indices;

        // internal draw parameters (set by _jsPlatformRequiresSlowPath):
        // client-side array transfer buffer params
        GLuint	xferTotalSize;
        GLuint	indexXferOffset;
        GLuint	indexXferSize;
        GLuint	attribXferTotalSize;
        GLuint	attribXferOffset[_JS_MAX_VERTEX_ATTRIBS];
        GLuint	attribXferSize[_JS_MAX_VERTEX_ATTRIBS];
    }
    jsDrawParams;

// define mapping of vertex semantics to attributes
//  These indices specify the aliasing of vertex attributes with
//  conventional per-vertex parameters.  This mapping is the same as
//  specified in the NV_vertex_program extension.
#define _JS_ATTRIB_POSITION_INDEX			0
#define _JS_ATTRIB_WEIGHT_INDEX				1
#define _JS_ATTRIB_NORMAL_INDEX				2
#define _JS_ATTRIB_PRIMARY_COLOR_INDEX		3
#define _JS_ATTRIB_SECONDARY_COLOR_INDEX	4
#define _JS_ATTRIB_FOG_COORD_INDEX			5
#define _JS_ATTRIB_POINT_SIZE_INDEX			6
#define _JS_ATTRIB_BLEND_INDICES_INDEX		7
#define _JS_ATTRIB_TEX_COORD0_INDEX			8
#define _JS_ATTRIB_TEX_COORD1_INDEX			9
#define _JS_ATTRIB_TEX_COORD2_INDEX			10
#define _JS_ATTRIB_TEX_COORD3_INDEX			11
#define _JS_ATTRIB_TEX_COORD4_INDEX			12
#define _JS_ATTRIB_TEX_COORD5_INDEX			13
#define _JS_ATTRIB_TEX_COORD6_INDEX			14
#define _JS_ATTRIB_TEX_COORD7_INDEX			15

// per-attribute descriptor and data
    typedef struct
    {
        // GL state
        GLvoid*	clientData;   // client-side array pointer or VBO offset
        GLuint	clientSize;   // number of components 1-4
        GLenum	clientType;   // GL_SHORT, GL_INT, GL_FLOAT
        GLsizei	clientStride; // element-to-element distance [bytes]
        GLuint  arrayBuffer;  // name of buffer object; 0==none (ie client-side)
        GLfloat	value[4];     // constant attribute value
        GLuint  frequency;    // instancing divisor
        GLboolean normalized;
    }
    ALIGN16 jsAttribute;

// state for the entire set of vertex attributes, plus
// other AttribSet-encapsulated state.
// (this is the block of state applied en mass during glBindAttribSetSCE())
    typedef struct
    {
        //  Vertex attribute descriptors and data are stored in this array.
        //  The fixed function attributes are aliased to the array via the
        //  indices defined by _JS_ATTRIB_*_INDEX.
        jsAttribute		attrib[_JS_MAX_VERTEX_ATTRIBS];

        // bitfields corresponding to the attrib[] array elements:
        jsBitfield DirtyMask; // 1 == attribute has changed & needs updating
        jsBitfield EnabledMask; // 1 == attribute is enabled for drawing
        jsBitfield NeedsConversionMask; // 1 == attribute needs conversion to float
        jsBitfield HasVBOMask; // 1 == attribute is in a VBO (ie server-side)
        jsBitfield ModuloMask; // 1 == attribute has modulo instancing

        // other AttribSet-encapsulated state
        GLboolean		PrimitiveRestart;
        GLuint			RestartIndex;

        GLuint			ElementArrayBuffer;
    }
    ALIGN16 jsAttributeState;

// contents of the glAttribSetSCE object
    typedef struct
    {
        jsAttributeState attribs;		// to be applied on glBindAttribSetSCE()

        GLboolean	dirty;				// does cmdBuffer need to be regenerated?
        jsBitfield	beenUpdatedMask;	// which attribs are updated by cmdBuffer?
        GLvoid*		cmdBuffer;			// NV curie command buffer
        GLuint		cmdNumWords;		// # of command words in cmdBuffer
    }
    ALIGN16 jsAttribSet;

    struct jsBufferObject
    {
        GLuint refCount;
        GLsizeiptr size;
        GLenum usage;
        GLboolean mapped;
        GLboolean isSurface;
        GLenum internalFormat;
        GLuint width;
        GLuint height;
        GLenum multisampling;
        PSGL::Vector<jsTexture *> textureReferences;
        PSGL::Vector<jsAttribSet *> attribSets;
#ifdef JS_ALLOCINFO
        GLenum target;
#endif
        void *platformBufferObject[];
    };


#define _JS_CONTEXT_RED_MASK				0x01
#define _JS_CONTEXT_GREEN_MASK				0x02
#define _JS_CONTEXT_BLUE_MASK				0x04
#define _JS_CONTEXT_ALPHA_MASK				0x08
#define _JS_CONTEXT_DEPTH_MASK				0x10
#define _JS_CONTEXT_COLOR_MASK				0x0F


    enum
    {
        _JS_CONTEXT_ACTIVE_SURFACE_COLOR0,
        _JS_CONTEXT_ACTIVE_SURFACE_COLOR1,
        _JS_CONTEXT_ACTIVE_SURFACE_COLOR2,
        _JS_CONTEXT_ACTIVE_SURFACE_COLOR3,
        _JS_CONTEXT_ACTIVE_SURFACE_DEPTH,
        _JS_CONTEXT_ACTIVE_SURFACE_STENCIL,
        _JS_CONTEXT_ACTIVE_SURFACES
    };

#define jsELEMENTS_IN_MATRIX	16	// 4x4

    typedef struct jsNameSpace
    {
        void** data;
        void** firstFree;
        unsigned long capacity;
    }
    jsNameSpace;

    typedef void *( *jsTexNameSpaceCreateFunction )( void );
    typedef void( *jsTexNameSpaceDestroyFunction )( void * );

    typedef struct jsTexNameSpace
    {
        void** data;
        GLuint capacity;
        jsTexNameSpaceCreateFunction create;
        jsTexNameSpaceDestroyFunction destroy;
    }
    jsTexNameSpace;

// There are 6 clock domains, each with a maximum of 4 experiments, plus 4 elapsed exp.
#define _JS_MAX_DPM_QUERIES       (4 * 6 + 4)

    struct PSGLcontext
    {
        GLenum			error;
        int			MatrixMode;

        jsMatrixStack	ModelViewMatrixStack;
        jsMatrixStack	ProjectionMatrixStack;

        GLfloat			LocalToScreenMatrixf[jsELEMENTS_IN_MATRIX];
        GLfloat 		InverseModelViewMatrixf[jsELEMENTS_IN_MATRIX];
        GLboolean		InverseModelViewValid;
        GLboolean		RescaleNormals;
        GLboolean		Normalize;
        GLfloat			ScalingFactor;

        jsViewPort		ViewPort;
        GLclampf		DepthNear;
        GLclampf		DepthFar;
        GLboolean		DepthClamp;

        GLboolean		ScissorTest;
        jsViewPort		ScissorBox;
        jsViewPort		RealScissorBox;

        GLboolean		ColorMaterial;
        GLboolean		FaceCulling;
        GLenum			CullFaceMode;
        GLenum			PerspectiveCorrectHint;
        GLenum			FrontFace;
        GLfloat			PointSize;
        GLboolean		PointSmooth;
        GLenum			PointSmoothHint;
        GLfloat			LineWidth;
        GLboolean		LineSmooth;
        GLenum			LineSmoothHint;
        GLfloat			PolygonOffsetFactor;
        GLfloat			PolygonOffsetUnits;
        GLboolean   PolygonOffsetFill;
        GLboolean   PolygonOffsetLine;
        GLenum			ShadeModel;

        jsMaterial		FrontMaterial,			// The current Materials
        BackMaterial;

        jsLightModel	LightModel; 			// Light Model Sturct // contains ambient and Lighting-2-sided flag
        jsLight			Lights[_JS_MAX_LIGHTNUM];

        jsLight*		ActiveLights[_JS_MAX_LIGHTNUM];
        unsigned int	NumOfActiveLights;

        jsClipPlane		ClipPlane[_JS_MAX_CLIP_PLANES];

        jsAttributeState defaultAttribs0;	// a default jsAttributeState, for bind = 0
        jsAttributeState *attribs;			// ptr to current jsAttributeState
        jsTexNameSpace	attribSetNameSpace;
        GLuint			attribSetName;		// name of currently bound AttribSet, 0=none
        GLboolean		attribSetDirty;		// has AttribSet binding or its contents changed since last draw

        // Frame buffer-related fields
        //
        GLenum			DrawBuffer, ReadBuffer;

        jsColorRGBAf	ClearColor;
        GLfloat			ClearDepth;
        GLint			ClearStencil;

        jsColorRGBAf	AccumClearColor;

        GLboolean		Fog;
        GLenum			FogHint;
        GLenum			FogMode;
        jsColorRGBAf	FogColor;
        GLfloat			FogDensity;
        GLfloat			FogStart;
        GLfloat			FogEnd;

        GLboolean		ShaderSRGBRemap;

        GLboolean		DepthTest;
        GLenum			DepthFunc;

        GLboolean		AlphaTest;
        GLenum			AlphaFunc;
        GLclampf		AlphaRef;

        GLboolean		StencilTest;
        GLenum			StencilFunc;
        GLint			StencilRef;
        GLuint			StencilFuncMask;
        GLuint			StencilOpFail;
        GLuint			StencilOpZFail;
        GLuint			StencilOpZPass;

        // backface stencil state
        GLenum			ActiveStencilFace;
        GLboolean		StencilTestTwoSide;
        GLenum			StencilBackfaceFunc;
        GLint			StencilBackfaceRef;
        GLuint			StencilBackfaceFuncMask;
        GLuint			StencilBackfaceOpFail;
        GLuint			StencilBackfaceOpZFail;
        GLuint			StencilBackfaceOpZPass;

        GLboolean		Blending;       // enable for mrt color target 0
        GLboolean		BlendingMrt[3]; // separate blending enable per MRT color target 1, 2, and 3
        GLenum			BlendEquationRGB;
        GLenum			BlendEquationAlpha;
        GLenum			BlendFactorSrcRGB;
        GLenum			BlendFactorDestRGB;
        GLenum			BlendFactorSrcAlpha;
        GLenum			BlendFactorDestAlpha;
        jsColorRGBAf	BlendColor;

        GLboolean		ColorLogicOp;
        GLenum			LogicOp;

        GLboolean		Dithering;
        GLboolean		ReduceDstColor;

        GLboolean		Multisample;
        GLboolean		SampleCoverage;
        GLboolean		SampleAlphaToCoverage;
        GLboolean		SampleAlphaToOne;
        GLboolean		SampleCoverageInvert;
        GLfloat		SampleCoverageValue;

        GLuint			StencilWriteMask;
        GLuint			StencilBackfaceWriteMask;
        GLuint			WriteMask;


        GLenum			FrontPolygonMode, BackPolygonMode;	// Polygon drawing mode

        GLboolean		Lighting;

        GLuint	TexCoordReplaceMask;
        GLboolean	PointSprite;
        GLboolean	VertexProgramPointSize;

        jsTexNameSpace textureNameSpace;
        GLuint			ActiveTexture;
        GLuint			CS_ActiveTexture;
        jsTextureImageUnit	TextureImageUnits[_JS_MAX_TEXTURE_IMAGE_UNITS];
        jsTextureImageUnit* CurrentImageUnit;
        jsTextureCoordsUnit	TextureCoordsUnits[_JS_MAX_TEXTURE_COORDS];
        jsTextureCoordsUnit* CurrentCoordsUnit;

        jsTexture *VertexTextureImages[_JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS];

        GLsizei		packAlignment;
        GLsizei		unpackAlignment;

        jsTexNameSpace	bufferObjectNameSpace;
        GLuint	ArrayBuffer;
        GLuint	PixelPackBuffer;
        GLuint	PixelUnpackBuffer;
        GLuint	TextureBuffer;

        // synchronization objects
        jsTexNameSpace	fenceObjectNameSpace;
        jsTexNameSpace	eventObjectNameSpace;

        // framebuffer objects
        GLuint			framebuffer;	// GL_FRAMEBUFFER_OES binding
        GLuint			renderbuffer;	// GL_RENDERBUFFER_OES binding
        jsTexNameSpace	framebufferNameSpace;
        jsTexNameSpace	renderbufferNameSpace;

        // occlusion queries
        jsTexNameSpace	queryNameSpace;
        GLuint samplesPassedQuery;
        GLuint pmQuery;
        GLuint conditionalRenderQuery;

        GLboolean		VertexProgram;
        struct _CGprogram*	BoundVertexProgram;
        struct _CGprogram*	ShellVertexProgram; //used when the user wants to bypass Cg

        GLboolean		FragmentProgram;
        struct _CGprogram*	BoundFragmentProgram;
        struct _CGprogram*	ShellFragmentProgram;  //used when the user wants to bypass Cg
        unsigned int	LastFPConstantModification;

		GLboolean		AllowTXPDemotion;
        GLboolean		VSync;
        GLboolean       SkipFirstVSync;

        GLuint			needValidate;
        GLboolean		everAttached;

        CGerror JScgLastError;
        CGerrorCallbackFunc JScgErrorCallbackFunction;
        // Cg containers
        CGcontext JScgContextHead;
        jsNameSpace  cgContextNameSpace;
        jsNameSpace  cgProgramNameSpace;
        jsNameSpace  cgParameterNameSpace;
        void* psglFXContextExtension;
    };

//========================================================
// Get the current MatrixStack of the given context
//========================================================
#define jsContextCurrentMatrixStack(mContext, mMatrixStack)\
jsContextGetMatrixStack(mContext, (mContext)->MatrixMode, mMatrixStack)


//========================================================
// Get the given MatrixStack of the given context
//========================================================
#define jsContextGetMatrixStack(mContext, mMatrixMode, mMatrixStack) do \
{\
	switch(mMatrixMode)\
	{\
		case GL_MODELVIEW:\
			mMatrixStack = &((mContext)->ModelViewMatrixStack);\
			break;\
		case GL_PROJECTION:\
			mMatrixStack = &((mContext)->ProjectionMatrixStack);\
			break;\
		case GL_TEXTURE:\
			if ((mContext)->CurrentCoordsUnit) mMatrixStack = &((mContext)->CurrentCoordsUnit->TextureMatrixStack);\
			else mMatrixStack=NULL; \
			break;\
		default: \
			JS_ASSERT(0); \
			break; \
	}\
} while(0)


//================================================
// Get the given Matrix of the given context
//================================================
#define jsContextGetMatrixf(mContext, mMatrixMode, mMatrixStack, mMatrix) do \
{\
	jsContextGetMatrixStack(mContext, mMatrixMode, mMatrixStack);\
	if (mMatrixStack) mMatrix = (mMatrixStack)->MatrixStackf+(mMatrixStack)->MatrixStackPtr*jsELEMENTS_IN_MATRIX;\
} while (0)



//================================================
// Get the current Matrix of the given context
//================================================
#define jsContextCurrentMatrixf(mContext, mMatrixStack, mMatrix) do \
{\
	jsContextGetMatrixStack(mContext, (mContext)->MatrixMode, mMatrixStack);\
	if (mMatrixStack) mMatrix = (mMatrixStack)->MatrixStackf+(mMatrixStack)->MatrixStackPtr*jsELEMENTS_IN_MATRIX;\
} while(0)


#if defined(MSVC)
#pragma warning ( pop )
#endif

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration which encloses file.
#endif


#endif	// _JSTypes_h
