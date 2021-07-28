/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * File contains constants, structures, and many function declarations needed 
 * internally by PSGL.  Additionally, functions for validating the value of 
 * various state enums are defined here.
 */

#ifndef _JSPrivate_h
#define _JSPrivate_h


#include <PSGL/psgl.h>
#include <PSGL/Types.h>
#include <PSGL/Utils.h>
#include <PSGL/Debug.h>
#include <PSGL/ReportInternal.h>

#ifndef OS_VERSION_NUMERIC
#define OS_VERSION_NUMERIC 0x160
#endif
#if defined _JS_USE_JSGCM && (OS_VERSION_NUMERIC < 0x160)
#include <PSGL/gcm_ext.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
#define _JS_EXTERN_C extern "C"
#else
#define _JS_EXTERN_C
#endif

    extern PSGL_EXPORT PSGLcontext*	_CurrentContext;
    extern PSGLdevice*	_CurrentDevice;

    extern PSGL_EXPORT char*    jsVersion;


// only for internal purpose
#define GL_UNSIGNED_BYTE_4_4 0x4A00
#define GL_UNSIGNED_BYTE_4_4_REV 0x4A01
#define GL_UNSIGNED_BYTE_6_2 0x4A02
#define GL_UNSIGNED_BYTE_2_6_REV 0x4A03
#define GL_UNSIGNED_SHORT_12_4 0x4A04
#define GL_UNSIGNED_SHORT_4_12_REV 0x4A05
#define GL_UNSIGNED_BYTE_2_2_2_2 0x4A06
#define GL_UNSIGNED_BYTE_2_2_2_2_REV 0x4A07

#define GL_FLOAT_RGBA32 0x888B
//------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------

    PSGL_EXPORT void jsPrintMatrixf( jsMatrixf LMatrix );
    PSGL_EXPORT void jsMatrixScale( GLfloat LXScale, GLfloat LYScale, GLfloat LZScale );
    PSGL_EXPORT void jsMatrixInverse( GLfloat* matIn, GLfloat* matOut );


    typedef void( *JSlogFunction )( int, int, char* );

    PSGL_EXPORT int jsLog( int type, char* message, ... );
    PSGL_EXPORT void jsLogSetDestination( void* destination );
    PSGL_EXPORT void* jsLogGetDestination();
    PSGL_EXPORT void jsLogSetFunction( JSlogFunction function );
    PSGL_EXPORT JSlogFunction	jsLogGetFunction();



//----------------------------------------
// PSGL.c
//----------------------------------------
    typedef void( * JScontextHookFunction )( PSGLcontext *context );
    extern PSGL_EXPORT JScontextHookFunction _jsContextCreateHook;
    extern PSGL_EXPORT JScontextHookFunction _jsContextDestroyHook;

    extern PSGLcontext*	_jsContextCreate();
    extern void		_jsContextFree( PSGLcontext* LContext );
    extern void		_jsSetError( GLenum error );
    extern GLuint	_jsValidateStates( GLuint mask );
    void _jsAttachContext( PSGLdevice *device, PSGLcontext* context );
    void _jsDetachContext( PSGLdevice *device, PSGLcontext* context );
    void _jsInvalidateAllStates( PSGLcontext* context );
#ifdef JS_DPM
    void * _jsAPIENTRYPointer( char * name );
    int _jsSaveCurrentState( void *saveArea, unsigned long int saveAreaSize, unsigned long int * totalSize );
    int _jsRestoreCurrentState( void *saveArea, unsigned long int * totalSize );
    PSGLinitOptions *	_jsShadowInitOptions( void );
    int _getJsInitCompleted();
#endif
    void _jsResetAttributeState( jsAttributeState* as );
    PSGLuint64 _jsGetLastFlipTime();
void _jsSetFlipHandler(void (*handler)(const GLuint head), PSGLdevice *device);

//----------------------------------------
// Light.c
//----------------------------------------
    extern void	_jsMaterialInit( jsMaterial* LMaterial );
    extern void	_jsLightInit( jsLight* LLight, GLenum light );
    PSGL_EXPORT void _jsSetLightPosition( jsLight *LLight, const float *params );
    PSGL_EXPORT void _jsSetLightSpotDirection( jsLight *LLight, const float *params );
    PSGL_EXPORT void _jsSetLightCutoff( jsLight *LLight, float param );
    extern int	_jsCollectActiveLights( PSGLcontext* LContext );
    extern void	_jsIlluminatePointf( const GLfloat* LPoint, const GLfloat* LNormal, const jsMaterial* LMaterial, GLfloat* LIllumination );
    extern void	_jsIlluminatePointLambert( const GLfloat* LPoint, const GLfloat* LNormal, const jsMaterial* LMaterial, GLfloat* LIllumination );
    extern void	_jsIlluminatePointPhong( const GLfloat* LPoint, const GLfloat* LNormal, const jsMaterial* LMaterial, GLfloat* LIllumination );


//----------------------------------------
// Matrix.c
//----------------------------------------
    extern void	_jsContextRefreshLocalToScreenMatrix( PSGLcontext* LContext );

//----------------------------------------
// Texture.c
//----------------------------------------
    jsTexture *_jsAllocateTexture();
    void _jsFreeTexture( jsTexture *texture );
    void _jsTextureUnbind( PSGLcontext* context, GLuint name );
    extern int	_jsTextureInit( PSGLcontext* context, GLuint name );
    extern void	_jsTextureDelete( PSGLcontext* context, GLuint name );
    extern GLboolean _jsTextureHasValidLevels( const jsTexture *texture, int levels, int width, int height, int depth, GLenum format, GLenum type, GLenum internalFormat );
    extern GLboolean _jsTextureIsValid( const jsTexture* texture );
    GLenum _jsGetEnabledTextureMode( const jsTextureImageUnit *unit );
    extern jsTexture *_jsGetCurrentTexture( const jsTextureImageUnit *unit, GLenum target );
    PSGL_EXPORT void _jsUpdateCurrentTextureCache( jsTextureImageUnit *unit );
    void _jsReallocateImages( jsTexture *texture, GLint level, GLsizei dimension );
    extern int _jsGetImage( GLenum target, GLint level, jsTexture **texture, jsImage **image, GLsizei reallocateSize );
    void _jsGetTextureIntegerv( GLenum pname, GLint* params );
    static inline jsTexture* _jsGetTexture( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->textureNameSpace, name ) );
        return ( jsTexture* )LContext->textureNameSpace.data[name];
    }

    static inline jsTexture* _jsGetTextureSafe( PSGLcontext *LContext, GLuint name )
    {
        return _jsTexNameSpaceIsName( &LContext->textureNameSpace, name ) ? ( jsTexture* )LContext->textureNameSpace.data[name] : NULL;
    }

    static inline jsFramebuffer *_jsGetFramebuffer( PSGLcontext *LContext, GLuint name );
    inline static void _jsTextureTouchFBOs( jsTexture *texture )
    {
        PSGLcontext *LContext = _CurrentContext;
        if ( !LContext ) return; // may be called in psglDestroyContext
        // check if bound to any framebuffer
        GLuint fbCount = texture->framebuffers.getCount();
        if ( fbCount > 0 )
        {
            jsFramebuffer *contextFramebuffer = LContext->framebuffer ? _jsGetFramebuffer( LContext, LContext->framebuffer ) : NULL;
            for ( GLuint i = 0;i < fbCount;++i )
            {
                jsFramebuffer* framebuffer = texture->framebuffers[i];
                framebuffer->needValidate = GL_TRUE;
                if ( JS_UNLIKELY( framebuffer == contextFramebuffer ) ) LContext->needValidate |= PSGL_VALIDATE_SCISSOR_BOX | PSGL_VALIDATE_FRAMEBUFFER;
            }
        }
    }

    GLboolean _jsPaletteTexImage( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data );

    PSGL_EXPORT void _jsSetTexGenEyePlane( jsTexGen *texgen, const float *param );


//----------------------------------------
// Image.c
//----------------------------------------
    GLboolean _jsIsType( GLenum type );
    GLboolean _jsIsFormat( GLenum format );
    GLboolean _jsIsValidPair( GLenum format, GLenum type );
    void _jsImageAllocCPUStorage( jsImage *image );
    void _jsImageFreeCPUStorage( jsImage *image );
    extern void	_jsSetImage( jsImage* image, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei alignment, GLenum format, GLenum type, const GLvoid* pixels );
    extern void	_jsSetSubImage( GLenum target, GLint level, jsTexture *texture, jsImage* image, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLsizei alignment, GLenum format, GLenum type, const GLvoid* pixels );
    extern int	_jsGetPixelSize( GLenum format, GLenum type );
    static inline int _jsGetStorageSize( GLenum format, GLenum type, GLsizei width, GLsizei height, GLsizei depth )
    {
        return _jsGetPixelSize( format, type )*width*height*depth;
    }

    extern int _jsGetTypeSize( GLenum type );
    extern int	_jsGetMaxBitSize( GLenum type );
    extern int	_jsGetStorageSize( GLenum format, GLenum type, GLsizei width, GLsizei height, GLsizei depth );
    extern void _jsImageToRaster( const jsImage* image, jsRaster* raster, GLuint x, GLuint y, GLuint z );
    extern void _jsRasterToImage( const jsRaster* raster, jsImage* image, GLuint x, GLuint y, GLuint z );
    extern void _jsRawRasterToImage( const jsRaster* raster, jsImage* image, GLuint x, GLuint y, GLuint z );
    void _jsResampleImage3D( jsImage* src, jsImage* dst );

//----------------------------------------
// Surface.c
//----------------------------------------
    void _jsGenericReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels );

//----------------------------------------
// Buffer.c
//----------------------------------------
    jsBufferObject *_jsCreateBufferObject();
    void _jsFreeBufferObject( jsBufferObject *buffer );
    static inline jsBufferObject *_jsGetBufferObject( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->bufferObjectNameSpace, name ) );
        return ( jsBufferObject * )LContext->bufferObjectNameSpace.data[name];
    }

//----------------------------------------
// Sync.c
//----------------------------------------
    jsFenceObject *_jsCreateFenceObject();
    void _jsFreeFenceObject( jsFenceObject *fence );
    static inline jsFenceObject *_jsGetFenceObject( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->fenceObjectNameSpace, name ) );
        return ( jsFenceObject * )LContext->fenceObjectNameSpace.data[name];
    }

    jsEventObject *_jsCreateEventObject();
    void _jsFreeEventObject( jsEventObject *event );
    static inline jsEventObject *_jsGetEventObject( PSGLcontext *LContext, GLuint name )
    {
        // NULL is returned when the event could not be allocated (either
        // because events are not supported or all available events are being
        // used).  This is checked in calling code so do not assert here.
        return ( jsEventObject * )LContext->eventObjectNameSpace.data[name];
    }

//----------------------------------------
// FramebufferObject.c
//----------------------------------------
    jsFramebuffer *_jsCreateFramebuffer( void );
    void _jsDestroyFramebuffer( jsFramebuffer *framebuffer );
    static inline jsFramebuffer *_jsGetFramebuffer( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->framebufferNameSpace, name ) );
        return ( jsFramebuffer * )LContext->framebufferNameSpace.data[name];
    }

    static inline jsFramebuffer *_jsGetFramebufferSafe( PSGLcontext *LContext, GLuint name )
    {
        return _jsTexNameSpaceIsName( &LContext->framebufferNameSpace, name ) ? ( jsFramebuffer * )LContext->framebufferNameSpace.data[name] : NULL;
    }

	// a renderbuffer is implemented as a texture in a different namespace
    static inline jsTexture* _jsGetRenderbuffer( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->renderbufferNameSpace, name ) );
        return ( jsTexture* )LContext->renderbufferNameSpace.data[name];
    }

    static inline jsTexture* _jsGetRenderbufferSafe( PSGLcontext *LContext, GLuint name )
    {
        return _jsTexNameSpaceIsName( &LContext->renderbufferNameSpace, name ) ? ( jsTexture* )LContext->renderbufferNameSpace.data[name] : NULL;
    }

    void _jsFramebufferGetAttachmentTexture( PSGLcontext* LContext, const jsFramebufferAttachment* attachment, jsTexture** texture, GLuint* face );
    GLenum _jsPlatformFramebufferCheckStatus( jsFramebuffer* framebuffer );
    GLboolean _jsPlatformGenerateMipmap( jsTexture* texture, GLuint face );
    void _jsPlatformFramebufferGetParameteriv( GLenum pname, GLint* params );
    void _jsPlatformResolveFramebuffer();
    extern GLuint _jsPlatformRenderbufferMaxSize( void );
    void _jsGetFramebufferSize( GLuint* width, GLuint* height );

//----------------------------------------
// Query.c
//----------------------------------------
    jsQuery *_jsCreateQuery( void );
    void _jsDestroyQuery( jsQuery *query );
    static inline jsQuery *_jsGetQuery( PSGLcontext *LContext, GLuint name )
    {
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->queryNameSpace, name ) );
        return ( jsQuery* )LContext->queryNameSpace.data[name];
    }
    GLuint _jsPlatformQuerySize( void );
    void _jsPlatformGetQueryConfig( GLenum pname, GLuint* params );
    void _jsPlatformCreateQuery( jsQuery* query );
    void _jsPlatformDestroyQuery( jsQuery* query );
    void _jsPlatformBeginQuery( jsQuery* query );
    void _jsPlatformEndQuery( jsQuery* query );
    void _jsPlatformGetQuery( jsQuery* query, GLenum pname, GLuint* params );
    void _jsPlatformBeginConditionalRendering( jsQuery* query );
    void _jsPlatformEndConditionalRendering( jsQuery* query );

//----------------------------------------
// VertexArray.c
//----------------------------------------
#ifdef JS_DEBUG
    GLboolean _jsCheckAttribCaches( jsAttributeState* attributeState );
#endif
    void _jsVertexAttrib1fNV( GLuint index, GLfloat x );
    void _jsVertexAttrib1fvNV( GLuint index, const GLfloat* v );
    void _jsVertexAttrib2fNV( GLuint index, GLfloat x, GLfloat y );
    void _jsVertexAttrib2fvNV( GLuint index, const GLfloat* v );
    void _jsVertexAttrib3fNV( GLuint index, GLfloat x, GLfloat y, GLfloat z );
    void _jsVertexAttrib3fvNV( GLuint index, const GLfloat* v );
    void _jsVertexAttrib4fNV( GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
    void _jsVertexAttrib4fvNV( GLuint index, const GLfloat* v );
    void _jsVertexAttribPointerNV( GLuint index, GLint fsize, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );
    void _jsVertexAttribElementFunc( GLuint index, GLenum func, GLuint frequency );
    void _jsEnableVertexAttribArrayNV( GLuint index );
    void _jsDisableVertexAttribArrayNV( GLuint index );
    GLint _jsConvertStream( jsAttributeState* asDst, const jsAttributeState* asSrc, GLuint index,
                            GLint skip, GLint first, GLint count,
                            const void* indices, GLenum indexType );
    void _jsComputeMinMaxIndices( PSGLcontext* LContext, GLuint* min, GLuint* max, const void* indices, GLenum indexType, GLsizei count );

//----------------------------------------
// AttribSet.c
//----------------------------------------

    static inline jsAttribSet* _jsGetAttribSet( GLuint name )
    {
        JS_ASSERT( name );
        PSGLcontext* LContext = _CurrentContext;
        JS_ASSERT( LContext );
        JS_ASSERT( _jsTexNameSpaceIsName( &LContext->attribSetNameSpace, name ) );
        return ( jsAttribSet* ) LContext->attribSetNameSpace.data[name];
    }


    jsAttribSet* _jsCreateAttribSet( void );
    void _jsDestroyAttribSet( jsAttribSet* attribSet );

    void _jsAttribSetDeleteBuffer( PSGLcontext *LContext, GLuint buffName );
    void _jsAttribSetBufferData( PSGLcontext *LContext, GLuint buffName );
    void _jsAttribSetMapBuffer( PSGLcontext *LContext, GLuint buffName );

    void _jsAttribSetMarkDirty( void );

//----------------------------------------
// Vertex.c
//----------------------------------------
    PSGL_EXPORT void _jsSetClipPlane( PSGLcontext* LContext, jsClipPlane *plane, const GLfloat *equation );

//----------------------------------------
// Segment.c - Break large draws into a more manageable size.
//----------------------------------------
    void _jsSegmentInit( void );
    void _jsSegmentExit( void );
    void _jsDrawSlow( jsDrawParams* dparams );

//----------------------------------------
// Platform/Init.c
//----------------------------------------
    extern void _jsPlatformInit( PSGLinitOptions* options );
    extern void _jsPlatformExit();

	// memcpy alternatives
	//
	// fast_memcpy chooses a copy function based on alignment and size.  It can
	// be used as a drop-in replacement for memcpy.
	//
	// _jsAsyncCopy is an asynchronous copy requiring 16-byte alignment for both
	// source and destination, and a multiple of 16 size.  It uses an SPU for
	// DMA provided one is available.  _jsAsyncCopyInit must be called before
	// any asynchronous copy function.
	//
	// _jsAsyncCopyNotify is similar to _jsAsyncCopy except that the provided
	// 32-bit value is written to the provided address when the copy has
	// completed.
	//
	// _jsAsyncCopyBarrier ensures that copies before the call are completed
	// before copies after the call begin.  If the notifyAddress argument is not
	// NULL, the notifyValue is written after all previous copies are complete.
	//
	// _jsAsyncCopyFinish blocks until outstanding asynchronous copies to
	// complete.
	//
	// _jsAsyncCopySetSlowRange sets the GPU memory window, which has some usage
	// restrictions.
    void fast_memcpy( void * _JS_RESTRICT to, const void * _JS_RESTRICT from, unsigned int n );

    void _jsAsyncCopyInit( void );
    void _jsAsyncCopySetSlowRange( const void* start, size_t size, unsigned long long* ioif );
    void _jsAsyncCopy( void* dst, const void* src, size_t size );
    void _jsAsyncCopyNotify( void* dst, const void* src, size_t size, int notifyValue, int* notifyAddress );
    void _jsAsyncCopyBarrier( int notifyValue, int* notifyAddress );
    void _jsAsyncCopyFinish( void );


//----------------------------------------
// Device/Device.c
//----------------------------------------
    extern void _jsDeviceInit( PSGLinitOptions* options );
    extern void _jsDeviceExit();
    extern PSGLdeviceParameters * _jsShadowDeviceParameters( void );


//----------------------------------------
// Device/.../PlatformDevice.c
//----------------------------------------
    extern GLboolean		_jsPlatformDeviceInit( PSGLinitOptions* options );
    extern void		_jsPlatformDeviceExit();
    extern int		_jsPlatformDeviceSize();
    extern int		_jsPlatformCreateDevice( PSGLdevice* device );
    extern void		_jsPlatformDestroyDevice( PSGLdevice* device );
    extern void		_jsPlatformMakeCurrent( void* device );
    extern void		_jsPlatformSwapBuffers( PSGLdevice* device );
    extern const GLvoid*	_jsPlatformGetProcAddress( const char *funcName );
    extern void		_jsPlatformClearZCullStats();
#ifdef JS_DPM_V2
    extern int _jsPlatformGetTiledRegionInfo( GLuint address, GLuint *pitch, GLenum *comp, int *zcullStart, int *zcullWidth, int *zcullHeight, unsigned char *zcullAAFormat );
#endif

//----------------------------------------
// Raster/.../PlatformRaster.c
//----------------------------------------
    void*	_jsPlatformRasterInit();
    void	_jsPlatformRasterExit( void* driver );
    void	_jsPlatformRasterDestroyResources();
    void	_jsPlatformDraw( jsDrawParams* dparams );
    GLboolean _jsPlatformNeedsConversion( const jsAttributeState* as, GLuint index );
	// [YLIN] Try to avoid LHS inside this function.
	//   In oringinal implementation, indexType and indexCount will be stored right before this function
	//   and since we will load them right after enter this function, there are LHS.
	GLboolean _jsPlatformRequiresSlowPath( jsDrawParams* dparams, const GLenum indexType, uint32_t indexCount);
    void _jsPlatformRasterGetIntegerv( GLenum pname, GLint* params );
    void	_jsPlatformRasterFlush();
    void	_jsPlatformRasterFinish();
    void	_jsValidateFragmentProgram();
    void	_jsValidateFragmentProgramSharedConstants();
    void	_jsValidateClipPlanes();
	void	_jsInvalidateAttributes();
	GLuint	_jsValidateAttributes( const GLvoid* indices, GLboolean *isMain );
	GLuint	_jsValidateAttributesSlow( jsDrawParams *dparams, GLboolean *isMain );

//----------------------------------------
// Raster/.../PlatformTexture.c
//----------------------------------------
    extern int	_jsPlatformTextureSize();
    extern int	_jsPlatformTextureMaxUnits();
    extern void	_jsPlatformCreateTexture( jsTexture* texture );
    extern void	_jsPlatformDestroyTexture( jsTexture* texture );
    extern void	_jsPlatformValidateTextureStage( int unit, jsTexture*texture );
    void _jsPlatformValidateVertexTextures();
    extern GLenum _jsPlatformChooseInternalStorage( jsImage* image, GLenum internalformat );
    extern GLenum _jsPlatformTranslateTextureFormat( GLenum internalFormat );
    extern void _jsPlatformCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
    GLenum _jsPlatformChooseInternalFormat( GLenum internalformat );
    void _jsPlatformExpandInternalFormat( GLenum internalformat, GLenum *format, GLenum *type );
    void _jsPlatformGetImageData( GLenum target, GLint level, jsTexture *texture, jsImage *image );
    extern void _jsPlatformSetCompressedTexture(
            GLenum target,
            GLint level,
            GLenum internalformat,
            GLsizei width, GLsizei height, GLsizei depth,
            GLint border,
            GLsizei imageSize,
            const GLvoid* data );
    extern void _jsPlatformSetCompressedTextureSub(
            GLenum target,
            GLint level,
            GLint xoffset, GLint yoffset, GLint zoffset,
            GLsizei width, GLsizei height, GLsizei depth,
            GLenum format,
            GLsizei imageSize,
            const GLvoid* data );
    extern GLboolean _jsPlatformTexturePBOImage(
            jsTexture* texture,
            jsImage* image,
            GLint level,
            GLint internalformat,
            GLsizei width, GLsizei height, GLsizei depth,
            GLenum format, GLenum type,
            const GLvoid *offset );
    extern GLboolean _jsPlatformTexturePBOSubImage(
            jsTexture* texture,
            jsImage* image,
            GLint level,
            GLint xoffset, GLint yoffset, GLint zoffset,
            GLsizei width, GLsizei height, GLsizei depth,
            GLenum format, GLenum type,
            const GLvoid *pixels );
    GLboolean _jsPlatformTextureReference( jsTexture *texture, GLuint pitch, jsBufferObject *bufferObject, GLintptr offset );
#ifdef  JS_DPM
    int	_jsSavePlatformTextureAndUpdate( void * platformTexture, jsTexNameSpace * bufferObjectNameSpace, void * saveArea, unsigned long int * offset,
                                         unsigned long int offsetMax );
    int	_jsRestorePlatformTextureAndUpdate( void * platformTexture, jsTexNameSpace * bufferObjectNameSpace, void * saveArea, unsigned long int * offset );
#endif

//----------------------------------------
// Raster/.../PlatformFBops.c
//----------------------------------------
    extern void _jsFBClear( GLbitfield mask );
    extern void _jsValidateFog();
    extern void _jsValidateFramebuffer( void );
    extern void _jsValidateFFXVertexProgram();
    extern void _jsValidateFFXFragmentProgram();
    extern void _jsPlatformReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLboolean flip, GLenum format, GLenum type, GLvoid *pixels );
    extern GLboolean _jsPlatformReadPBOPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLboolean flip, GLenum format, GLenum type, GLvoid *pixels );

//----------------------------------------
// Raster/.../PlatformTNL.c
//----------------------------------------
    void _jsValidateMatrices();
    void _jsValidateMaterial( void );
    void _jsValidateColorMaterial( void );
    void _jsValidateNormalize();
    void _jsValidateLighting();
    void _jsValidateTexGen( int unit );
    void _jsValidateVertexProgram();
    void _jsValidateVertexConstants();

//----------------------------------------
// Raster/.../PlatformBuffer.c
//----------------------------------------
    int _jsPlatformBufferObjectSize();
    GLboolean _jsPlatformCreateBufferObject( jsBufferObject* bufferObject );
    void _jsPlatformDestroyBufferObject( jsBufferObject* bufferObject );
    void _jsPlatformBufferObjectSetData( jsBufferObject* bufferObject, GLintptr offset, GLsizeiptr size, const GLvoid *data, GLboolean tryImmediateCopy );
    GLvoid _jsPlatformBufferObjectCopyData( jsBufferObject* bufferObjectDst, jsBufferObject* bufferObjectSrc );
    // map / unmap buffers. Internally refcounted
    char *_jsPlatformBufferObjectMap( jsBufferObject* bufferObject, GLenum access );
    GLboolean _jsPlatformBufferObjectUnmap( jsBufferObject* bufferObject );
    void _jsPlatformGetBufferParameteriv( jsBufferObject *bufferObject, GLenum pname, int *params );

    // this is shared in glBindTexture and cgGL code
    PSGL_EXPORT void _jsBindTextureInternal( jsTextureImageUnit *unit, GLuint name, GLenum target );
    void _jsBindVertexTextureInternal( GLuint unit, GLuint name );

//----------------------------------------
// Raster/.../PlatformSync.c
//----------------------------------------
    int _jsPlatformFenceObjectSize();
    GLboolean _jsPlatformCreateFenceObject( jsFenceObject* fenceObject );
    void _jsPlatformDestroyFenceObject( jsFenceObject* fenceObject );
    void _jsPlatformSetFence( jsFenceObject* fenceObject );
    GLboolean _jsPlatformTestFence( jsFenceObject* fenceObject );
    void _jsPlatformFinishFence( jsFenceObject* fenceObject );

    int _jsPlatformEventObjectSize();
    GLboolean _jsPlatformCreateEventObject( jsEventObject* eventObject );
    void _jsPlatformDestroyEventObject( jsEventObject* eventObject );
    void _jsPlatformSetEvent( jsEventObject* eventObject );
    void _jsPlatformResetEvent( jsEventObject* eventObject );
    void _jsPlatformWaitEvent( jsEventObject* eventObject, GLboolean autoReset );
    unsigned long long _jsPlatformMapEvent( jsEventObject* eventObject );
    void _jsPlatformSetMappedEvent( unsigned long long mappedEvent );

    int _jsSaveSyncGlobals( void * saveArea, unsigned long int * offset, unsigned long int saveAreaSize );
    int	_jsRestoreSyncGlobals( void * saveArea, unsigned long int * offset );

	// State validation, to share between GL and FX
	// Check whether incoming enum is a valid comparison
    static inline GLboolean _jsIsTestFunc( GLenum func )
    {
        switch ( func )
        {
            case GL_NEVER:
            case GL_LESS:
            case GL_EQUAL:
            case GL_LEQUAL:
            case GL_GREATER:
            case GL_NOTEQUAL:
            case GL_GEQUAL:
            case GL_ALWAYS:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid source blend mode
    static inline GLboolean _jsIsSrcBlendFunc( GLenum sfactor )
    {
        switch ( sfactor )
        {
            case GL_ZERO:
            case GL_ONE:
            case GL_DST_COLOR:
            case GL_ONE_MINUS_DST_COLOR:
            case GL_SRC_ALPHA:
            case GL_ONE_MINUS_SRC_ALPHA:
            case GL_DST_ALPHA:
            case GL_ONE_MINUS_DST_ALPHA:
            case GL_SRC_ALPHA_SATURATE:
            case GL_CONSTANT_COLOR:
            case GL_ONE_MINUS_CONSTANT_COLOR:
            case GL_CONSTANT_ALPHA:
            case GL_ONE_MINUS_CONSTANT_ALPHA:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid destination blend mode
    static inline GLboolean _jsIsDstBlendFunc( GLenum dfactor )
    {
        switch ( dfactor )
        {
            case GL_ZERO:
            case GL_ONE:
            case GL_SRC_COLOR:
            case GL_ONE_MINUS_SRC_COLOR:
            case GL_SRC_ALPHA:
            case GL_ONE_MINUS_SRC_ALPHA:
            case GL_DST_ALPHA:
            case GL_ONE_MINUS_DST_ALPHA:
            case GL_CONSTANT_COLOR:
            case GL_ONE_MINUS_CONSTANT_COLOR:
            case GL_CONSTANT_ALPHA:
            case GL_ONE_MINUS_CONSTANT_ALPHA:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid blend equation
    static inline GLboolean _jsIsBlendEquation( GLenum mode )
    {
        switch ( mode )
        {
            case GL_FUNC_ADD:
            case GL_MIN:
            case GL_MAX:
            case GL_FUNC_SUBTRACT:
            case GL_FUNC_REVERSE_SUBTRACT:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid logic operation
    static inline GLboolean _jsIsLogicOp( GLenum opcode )
    {
        switch ( opcode )
        {
            case GL_CLEAR:
            case GL_SET:
            case GL_COPY:
            case GL_COPY_INVERTED:
            case GL_NOOP:
            case GL_INVERT:
            case GL_AND:
            case GL_NAND:
            case GL_OR:
            case GL_NOR:
            case GL_XOR:
            case GL_EQUIV:
            case GL_AND_REVERSE:
            case GL_AND_INVERTED:
            case GL_OR_REVERSE:
            case GL_OR_INVERTED:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid face mode
    static inline GLboolean _jsIsFaceMode( GLenum mode )
    {
        switch ( mode )
        {
            case GL_FRONT_AND_BACK:
            case GL_FRONT:
            case GL_BACK:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid polygon orientation
    static inline GLboolean _jsIsFaceOrientation( GLenum mode )
    {
        switch ( mode )
        {
            case GL_CW:
            case GL_CCW:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid stencil operation
    static inline GLboolean _jsIsStencilOp( GLenum op )
    {
        switch ( op )
        {
            case GL_KEEP:
            case GL_ZERO:
            case GL_REPLACE:
            case GL_INCR:
            case GL_DECR:
            case GL_INVERT:
            case GL_INCR_WRAP:
            case GL_DECR_WRAP:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid texture generation mode
    static inline GLboolean _jsIsTexGenMode( GLenum param, GLenum target )
    {
        switch ( param )
        {
            case GL_SPHERE_MAP:
                if ( target == GL_R ) return GL_FALSE;
            case GL_NORMAL_MAP: // fall through
            case GL_REFLECTION_MAP:
                if ( target == GL_Q ) return GL_FALSE;
            case GL_OBJECT_LINEAR: // fall through
            case GL_EYE_LINEAR:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid texture environment operation
    static inline GLboolean _jsIsTexEnvMode( GLenum param )
    {
        switch ( param )
        {
            case GL_MODULATE:
            case GL_DECAL:
            case GL_BLEND:
            case GL_REPLACE:
            case GL_ADD:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid polygon rendering mode
    static inline GLboolean _jsIsPolygonMode( GLenum mode )
    {
        switch ( mode )
        {
            case GL_POINT:
            case GL_LINE:
            case GL_FILL:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid shade model
    static inline GLboolean _jsIsShadeModel( GLenum mode )
    {
        switch ( mode )
        {
            case GL_SMOOTH:
            case GL_FLAT:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid fog mode
    static inline GLboolean _jsIsFogMode( GLenum param )
    {
        switch ( param )
        {
            case GL_LINEAR:
            case GL_EXP:
            case GL_EXP2:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid texture wrapping mode
    static inline GLboolean _jsIsWrapMode( GLenum param )
    {
        return param == GL_REPEAT ||
               param == GL_CLAMP ||
               param == GL_CLAMP_TO_EDGE ||
               param == GL_MIRRORED_REPEAT ||
               param == GL_MIRROR_CLAMP_EXT ||
               param == GL_MIRROR_CLAMP_TO_EDGE_EXT ||
               param == GL_MIRROR_CLAMP_TO_BORDER_EXT ||
               param == GL_CLAMP_TO_BORDER;
    }

	// Check whether incoming enum is a valid texture magnification filter
    static inline GLboolean _jsIsMagFilter( GLenum param )
    {
        return ( param == GL_NEAREST || param == GL_LINEAR );
    }

	// Check whether incoming enum is a valid texture minification filter
    static inline GLboolean _jsIsMinFilter( GLenum param )
    {
        return ( param == GL_NEAREST || param == GL_LINEAR ||
                 param == GL_NEAREST_MIPMAP_NEAREST || param == GL_LINEAR_MIPMAP_NEAREST ||
                 param == GL_NEAREST_MIPMAP_LINEAR || param == GL_LINEAR_MIPMAP_LINEAR );
    }

	// Check whether incoming enum is a valid depth texture format
    static inline GLboolean _jsIsDepthTextureMode( GLenum param )
    {
        return ( param == GL_LUMINANCE || param == GL_INTENSITY || param == GL_ALPHA );
    }

	// Check whether incoming enum is a valid depth texture compare mode
    static inline GLboolean _jsIsCompareMode( GLenum param )
    {
        return ( param == GL_NONE || param == GL_COMPARE_R_TO_TEXTURE_ARB );
    }

	// Check whether incoming enum is a valid internal texture format
    static inline GLboolean _jsIsInternalFormat( GLenum format )
    {
        switch ( format )
        {
            case GL_ALPHA:
            case GL_ALPHA4:
            case GL_ALPHA8:
            case GL_ALPHA12:
            case GL_ALPHA16:
            case 1:
            case GL_INTENSITY:
            case GL_INTENSITY4:
            case GL_INTENSITY8:
            case GL_LUMINANCE:
            case GL_LUMINANCE4:
            case GL_LUMINANCE8:
            case GL_INTENSITY12:
            case GL_LUMINANCE12:
            case GL_INTENSITY16:
            case GL_LUMINANCE16:
            case 2:
            case GL_ALPHA_LUMINANCE_SCE:
            case GL_LUMINANCE_ALPHA:
            case GL_LUMINANCE8_ALPHA8:
            case GL_LUMINANCE4_ALPHA4:
            case GL_LUMINANCE6_ALPHA2:
            case GL_LUMINANCE12_ALPHA4:
            case GL_LUMINANCE12_ALPHA12:
            case GL_LUMINANCE16_ALPHA16:
            case GL_DEPTH_COMPONENT:
            case GL_DEPTH_COMPONENT16:
            case GL_DEPTH_COMPONENT24:
            case GL_DEPTH_COMPONENT32:
            case GL_R3_G3_B2:
            case GL_RGB4:
            case GL_RGB5:
            case 3:
            case GL_RGB:
            case GL_RGB8:
            case GL_RGB10:
            case GL_RGB10_A2:
            case GL_RGB12:
            case GL_RGB16:
            case GL_RGBA2:
            case GL_RGBA4:
            case GL_RGB5_A1:
            case 4:
            case GL_RGBA8:
            case GL_RGBA:
            case GL_RGBA12:
            case GL_RGBA16:
            case GL_ALPHA16F_ARB:
            case GL_LUMINANCE16F_ARB:
            case GL_LUMINANCE_ALPHA16F_ARB:
            case GL_RGB16F_ARB:
            case GL_RGBA16F_ARB:
            case GL_ALPHA32F_ARB:
            case GL_LUMINANCE32F_ARB:
            case GL_LUMINANCE_ALPHA32F_ARB:
            case GL_RGB32F_ARB:
            case GL_RGBA32F_ARB:
            case GL_ARGB_SCE:
            case GL_BGRA:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid DXT compressed internal format
    static inline GLboolean _jsIsDXTInternalFormat( GLenum format )
    {
        switch ( format )
        {
            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

	// Check whether incoming enum is a valid multisampling mode
    static inline GLboolean _jsIsMultisampling( GLenum param )
    {
        switch ( param )
        {
            case GL_MULTISAMPLING_NONE_SCE:
            case GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE:
            case GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE:
            case GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE:
                return GL_TRUE;
            default:
                return GL_FALSE;
        }
    }

#ifdef __cplusplus
}	// Close scope of 'extern "C"' declaration that encloses file.
#endif

#endif	// _JSPrivate_h
