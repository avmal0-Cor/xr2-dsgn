/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Internal functions to manage texture
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>

// Allocate memory for texture object
// Caller has to clean it up by calling _jsFreeTexture
jsTexture *_jsAllocateTexture()
{
    GLuint size = sizeof( jsTexture ) + _jsPlatformTextureSize();
    jsTexture *texture = ( jsTexture * )jsMalloc( size );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
#endif
    memset( texture, 0, size );
    texture->target = 0;
    texture->minFilter = GL_NEAREST_MIPMAP_LINEAR;
    texture->magFilter = GL_LINEAR;
    texture->minLod = -1000.f;
    texture->maxLod = 1000.f;
    texture->baseLevel = 0;
    texture->maxLevel = 1000;
    texture->wrapS = GL_REPEAT;
    texture->wrapT = GL_REPEAT;
    texture->wrapR = GL_REPEAT;
    texture->lodBias = 0.f;
    texture->maxAnisotropy = 1.f;
    texture->depthMode = GL_LUMINANCE;
    texture->compareMode = GL_NONE;
    texture->compareFunc = GL_LEQUAL;
    texture->gammaRemap = 0;
    texture->vertexEnable = GL_FALSE;
    texture->usage = 0;
    texture->aaUsage = GL_MULTISAMPLING_NONE_SCE;
    texture->isRenderTarget = GL_FALSE;
    int face;
    for ( face = 0;face < 6;++face ) texture->images[face] = NULL;
    texture->hasMipmaps = GL_FALSE;
    texture->isComplete = GL_FALSE;
    texture->imageCount = 0;
    texture->faceCount = 1;
    texture->revalidate = 0;
    texture->referenceBuffer = NULL;
    new( &texture->framebuffers ) PSGL::Vector<jsFramebuffer *>();
    _jsPlatformCreateTexture( texture );
    return texture;
}

// Clean up resource used by a texture object
// In case of texture reference, it assumes associated frame buffer is 
// not cleaned yet.
void _jsFreeTexture( jsTexture *texture )
{
    _jsTextureTouchFBOs( texture );
    texture->framebuffers.~Vector<jsFramebuffer *>();
    int face;
    for ( face = 0;face < 6;++face )
    {
        if ( texture->images[face] )
        {
            for ( GLuint i = 0;i < texture->imageCount;++i )
            {
                jsImage *image = texture->images[face] + i;
                _jsImageFreeCPUStorage( image );
            }
            jsFree( texture->images[face] );
        }
    }
    if ( texture->referenceBuffer )
    {
        texture->referenceBuffer->textureReferences.removeElement( texture );
    }
    _jsPlatformDestroyTexture( texture );
    jsFree( texture );
}

// Unbind a texture with a name from the unit in a context
void _jsTextureUnbind( PSGLcontext* context, GLuint name )
{
    int unit;
    for ( unit = 0;unit < _JS_MAX_TEXTURE_IMAGE_UNITS;++unit )
    {
        jsTextureImageUnit *tu = context->TextureImageUnits + unit;
        GLboolean dirty = GL_FALSE;
        if ( tu->bound2D == name )
        {
            tu->bound2D = 0;
            dirty = GL_TRUE;
        }
        if ( tu->bound3D == name )
        {
            tu->bound3D = 0;
            dirty = GL_TRUE;
        }
        if ( tu->boundCube == name )
        {
            tu->boundCube = 0;
            dirty = GL_TRUE;
        }
        if ( dirty )
        {
            _jsUpdateCurrentTextureCache( tu );
            context->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
        }
    }
    // now deal with the vertex textures
    if ( _jsTexNameSpaceIsName( &context->textureNameSpace, name ) )
    {
        jsTexture*texture = ( jsTexture * )context->textureNameSpace.data[name];
        for ( unit = 0;unit < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS; ++unit )
        {
            if ( context->VertexTextureImages[unit] == texture )
            {
                context->VertexTextureImages[unit] = NULL;
                context->needValidate |= PSGL_VALIDATE_VERTEX_TEXTURES_USED;
            }
        }
    }
}

// Iterate thru texture's every level of every face and check images validity.
GLboolean _jsTextureHasValidLevels( const jsTexture *texture, int levels, int width, int height, int depth, GLenum format, GLenum type, GLenum internalFormat )
{
    for ( GLuint face = 0; face < texture->faceCount; ++face )
    {
        int imgWidth = width;
        int imgHeight = height;
        int imgDepth = depth;

        JS_ASSERT( texture->images[face] );
        for ( int ii = 0; ii < levels; ++ii )
        {
            jsImage *image = texture->images[face] + texture->baseLevel + ii;
            // check that
            // - image has been set
            if ( !image->isSet )
                return GL_FALSE;
            // - they all have a consistent size
            if ( imgWidth != image->width )
                return GL_FALSE;
            if ( imgHeight != image->height )
                return GL_FALSE;
            if ( imgDepth != image->depth )
                return GL_FALSE;
            // - they all have a consistent format and type
            if ( format != image->format )
                return GL_FALSE;
            if ( type != image->type )
                return GL_FALSE;
            if ( internalFormat != image->internalFormat )
                return GL_FALSE;
            imgWidth >>= 1;
            if ( imgWidth == 0 )
                imgWidth = 1;
            imgHeight >>= 1;
            if ( imgHeight == 0 )
                imgHeight = 1;
            imgDepth >>= 1;
            if ( imgDepth == 0 )
                imgDepth = 1;
        }
    }
    return GL_TRUE;
}

// Check the validity of texture's format and mipmap
GLboolean _jsTextureIsValid( const jsTexture* texture )
{
    GLboolean mipmap;
    if ( texture->minFilter != GL_LINEAR && texture->minFilter != GL_NEAREST )
        mipmap = GL_TRUE;
    else
        mipmap = GL_FALSE;

    // we need at least the first image
    if ( texture->imageCount < 1 + texture->baseLevel )
        return GL_FALSE;
    if ( !texture->images[0] )
        return GL_FALSE;
    const jsImage* image = texture->images[0] + texture->baseLevel;

    int width = image->width;
    int height = image->height;
    int depth = image->depth;
    GLenum format = image->format;
    GLenum type = image->type;
    GLenum internalFormat = image->internalFormat;
    if (( texture->vertexEnable ) && ( internalFormat != GL_FLOAT_RGBA32 ) && ( internalFormat != GL_RGBA32F_ARB ) && ( internalFormat != GL_LUMINANCE32F_ARB ) )
        return GL_FALSE;
    if (( internalFormat == 0 ) || ( format == 0 ) || ( type == 0 ) )
        return GL_FALSE;
    if (( depth > 1 ) && ( texture->target != GL_TEXTURE_3D ) )
        return GL_FALSE;
    if (( width != height ) && ( texture->target == GL_TEXTURE_CUBE_MAP ) )
        return GL_FALSE;

    // determine expected number of images
    GLuint levels;
    if ( mipmap )
    {
        int dimension = MAX( MAX( width, height ), depth );
        levels = 1 + _jsLog2( dimension );
    }
    else
        levels = 1;

    // don't need more than max level
    levels = MIN( levels, 1 + texture->maxLevel );
    if ( texture->imageCount < levels )
        return GL_FALSE;

    return _jsTextureHasValidLevels( texture, levels, width, height, depth, format, type, internalFormat );
}

// Reallocate images held by a texture
void _jsReallocateImages( jsTexture *texture, GLint level, GLsizei dimension )
{
    GLuint oldCount = texture->imageCount;
    if ( dimension <= 0 ) dimension = 1;
    GLuint n = level + 1 + _jsLog2( dimension );
    n = MAX( n, oldCount );
    for ( GLuint face = 0;face < texture->faceCount;++face )
    {
        jsImage *images = ( jsImage * )jsRealloc( texture->images[face], n * sizeof( jsImage ) );
#ifndef JS_NO_ERROR_CHECK
        if ( !images )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            JS_ASSERT( 0 );
            return;
        }
#endif
        memset( images + oldCount, 0, ( n - oldCount )*sizeof( jsImage ) );
        texture->images[face] = images;
    }
    texture->imageCount = n;
}

// Get an enabled texture mode of a texture image unit
GLenum _jsGetEnabledTextureMode( const jsTextureImageUnit *unit )
{
    // here, if fragment program is enabled and a valid program is set, get the enabled
    // units from the program instead of the texture units.
    if ( _CurrentContext->BoundFragmentProgram != NULL && _CurrentContext->FragmentProgram != GL_FALSE 
		&& _CurrentContext->BoundFragmentProgram != _CurrentContext->ShellFragmentProgram)
    {
        return unit->fragmentTarget;
    }
    if ( unit->enableCube ) return GL_TEXTURE_CUBE_MAP;
    else if ( unit->enable3D ) return GL_TEXTURE_3D;
    else if ( unit->enable2D ) return GL_TEXTURE_2D;
    else return 0;
}

// Get a texture which currently bound to a target mode of an image unit
jsTexture *_jsGetCurrentTexture( const jsTextureImageUnit *unit, GLenum target )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    GLuint name = 0;
    jsTexture *defaultTexture = NULL;
    switch ( target )
    {
        case GL_TEXTURE_2D:
            name = unit->bound2D;
            defaultTexture = unit->default2D;
            break;
        case GL_TEXTURE_3D:
            name = unit->bound3D;
            defaultTexture = unit->default3D;
            break;
        case GL_TEXTURE_CUBE_MAP:
            name = unit->boundCube;
            defaultTexture = unit->defaultCube;
            break;
        default:
            return NULL;
    }
    if ( name )
    {
        JS_ASSERT( LContext->textureNameSpace.capacity > name );
        JS_ASSERT( LContext->textureNameSpace.data[name] );
        return ( jsTexture * )LContext->textureNameSpace.data[name];
    }
    else return defaultTexture;
}

// Update a current texture of a unit
void _jsUpdateCurrentTextureCache( jsTextureImageUnit *unit )
{
    GLenum target = _jsGetEnabledTextureMode( unit );
    unit->currentTexture = _jsGetCurrentTexture( unit, target );
}

// Get a texture and image which bound to target mode of the current unit 
// level is used to determine if reallocation is necessary or not.
// Results are stored in **texture and **image
// Return value is 0 for success and -1 for failure
int _jsGetImage( GLenum target, GLint level, jsTexture **texture, jsImage **image, GLsizei reallocateSize )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    JS_ASSERT( unit );

    GLuint index = 0;
    GLenum expectedTarget = 0;
    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = target - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            expectedTarget = GL_TEXTURE_CUBE_MAP;
            break;
        case GL_TEXTURE_2D:
            expectedTarget = GL_TEXTURE_2D;
            break;
        case GL_TEXTURE_3D:
            expectedTarget = GL_TEXTURE_3D;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return -1;
    }

    jsTexture *tex = _jsGetCurrentTexture( unit, expectedTarget );
    JS_ASSERT( tex );
    JS_ASSERT( tex->target == expectedTarget );

#ifndef JS_NO_ERROR_CHECK
    if (( level < 0 ) || ( level > _JS_MAX_TEXTURE_SIZE_LOG2 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return -1;
    }
#endif

    if ( level >= ( int )tex->imageCount )
    {
        _jsReallocateImages( tex, level, reallocateSize );
#ifndef JS_NO_ERROR_CHECK
        if ( level >= ( int )tex->imageCount )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return -1;
        }
#endif
    }

    JS_ASSERT( tex->images[index] );
    *image = tex->images[index] + level;
    *texture = tex;
    return 0;
}

// Bind a texure with a name to the target mode of the unit
void _jsBindTextureInternal( jsTextureImageUnit *unit, GLuint name, GLenum target )
{
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsTexture *texture = NULL;
#ifndef JS_NO_ERROR_CHECK
    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP:
            break;
        case GL_TEXTURE_3D:
            break;
        case GL_TEXTURE_2D:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif
    if ( name )
    {
        _jsTexNameSpaceCreateNameLazy( &LContext->textureNameSpace, name );
        texture = ( jsTexture * )LContext->textureNameSpace.data[name];
#ifndef JS_NO_ERROR_CHECK
        if ( !texture )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            return;
        }
#endif

        if ( texture->target )
        {
#ifndef JS_NO_ERROR_CHECK
            if ( texture->target != target )
            {
                _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_INCOMPLETE, 
                    "Error: Trying to bind texture object %d(%s) to target %s", 
                    name, _jsGetGLEnumName( texture->target ), _jsGetGLEnumName( target ) );
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
        }
        else
        {
            texture->target = target;
            texture->faceCount = ( target == GL_TEXTURE_CUBE_MAP ) ? 6 : 1;
        }
#ifdef JS_REPORT
        texture->name = name;
#endif
    }

    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP:
            unit->boundCube = name;
            break;
        case GL_TEXTURE_3D:
            unit->bound3D = name;
            break;
        case GL_TEXTURE_2D:
            unit->bound2D = name;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }
    _jsUpdateCurrentTextureCache( unit );
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
}

// Bind vertex texture with name to the vetex texture image unit
void _jsBindVertexTextureInternal( GLuint unit, GLuint name )
{
    JS_ASSERT( unit < _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS );
    PSGLcontext *LContext = _CurrentContext;
    jsTexture *texture = NULL;
    if ( name )
    {
        if ( name < LContext->textureNameSpace.capacity ) texture = ( jsTexture * )LContext->textureNameSpace.data[name];
#ifndef JS_NO_ERROR_CHECK
        if (( !texture ) || ( texture->target != GL_TEXTURE_2D ) )
        {
            _jsSetError( GL_INVALID_OPERATION );
            return;
        }
#endif
    }
    LContext->VertexTextureImages[unit] = texture;
    LContext->needValidate |= PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

// This helper function describes a palette format.
//  Nonzero is returned in the event of an unsupported format.
static int _jsPaletteGetDesc(
    GLenum format,
    GLuint* indexBits,
    GLuint* paletteFormat,
    GLuint* paletteType )
{
    switch ( format )
    {
        case GL_PALETTE4_RGB8_OES:
            *indexBits = 4;
            *paletteFormat = GL_RGB;
            *paletteType = GL_UNSIGNED_BYTE;
            break;
        case GL_PALETTE4_RGBA8_OES:
            *indexBits = 4;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_BYTE;
            break;
        case GL_PALETTE4_R5_G6_B5_OES:
            *indexBits = 4;
            *paletteFormat = GL_RGB;
            *paletteType = GL_UNSIGNED_SHORT_5_6_5;
            break;
        case GL_PALETTE4_RGB5_A1_OES:
            *indexBits = 4;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
        case GL_PALETTE4_RGBA4_OES:
            *indexBits = 4;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
        case GL_PALETTE8_RGB8_OES:
            *indexBits = 8;
            *paletteFormat = GL_RGB;
            *paletteType = GL_UNSIGNED_BYTE;
            break;
        case GL_PALETTE8_RGBA8_OES:
            *indexBits = 8;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_BYTE;
            break;
        case GL_PALETTE8_R5_G6_B5_OES:
            *indexBits = 8;
            *paletteFormat = GL_RGB;
            *paletteType = GL_UNSIGNED_SHORT_5_6_5;
            break;
        case GL_PALETTE8_RGB5_A1_OES:
            *indexBits = 8;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
        case GL_PALETTE8_RGBA4_OES:
            *indexBits = 8;
            *paletteFormat = GL_RGBA;
            *paletteType = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return -1;
    }

    return 0;
}

static const GLint _jsCompressedTextureFormats[] =
    {
        GL_PALETTE4_RGB8_OES,
        GL_PALETTE4_RGBA8_OES,
        GL_PALETTE4_R5_G6_B5_OES,
        GL_PALETTE4_RGBA4_OES,
        GL_PALETTE4_RGB5_A1_OES,
        GL_PALETTE8_RGB8_OES,
        GL_PALETTE8_RGBA8_OES,
        GL_PALETTE8_R5_G6_B5_OES,
        GL_PALETTE8_RGBA4_OES,
        GL_PALETTE8_RGB5_A1_OES,
    };

// Get a texture state variable
void _jsGetTextureIntegerv( GLenum pname, GLint* params )
{
    switch ( pname )
    {
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
            params[0] = sizeof( _jsCompressedTextureFormats ) / sizeof( _jsCompressedTextureFormats[0] );
            break;
        case GL_COMPRESSED_TEXTURE_FORMATS:
            memcpy( params, _jsCompressedTextureFormats, sizeof( _jsCompressedTextureFormats ) );
            break;
        case GL_MAX_TEXTURE_SIZE:
            params[0] = _JS_MAX_TEXTURE_SIZE;
            break;
        case GL_MAX_3D_TEXTURE_SIZE:
            params[0] = _JS_MAX_TEXTURE_SIZE_3D;
            break;
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            params[0] = 16;
            break;
        default:
            JS_ASSERT( 0 );
            return;
    }
}

// common implementation for glCompressedTexImage[23]D
//  This helper routine is common code for both the 2D and 3D
//  entrypoints for paletted textures.
//
//  The API for indexed textures is rather ugly because it redefines the
//  level argument to be 1 - nLevels (yes, a nonpositive value) and
//  the data holds the palette followed by all the mipmap levels.  This
//  is semantically different from the way glTexImage[23]D works.
GLboolean _jsPaletteTexImage(
    GLenum target,
    GLint level,	// 1 - numLevels (0 if no mipmapping)
    GLenum internalFormat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLsizei imageSize,
    const GLvoid* data )
{
    jsTexture* texture = NULL;
    jsImage* image = NULL;
#ifndef JS_NO_ERROR_CHECK
    if (( width < 0 ) || ( height < 0 ) || ( depth < 0 ) || ( level > 0 ) || ( border != 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return GL_FALSE;
    }
    _jsGetImage( target, 0, &texture, &image, MAX( width, height ) );
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return GL_FALSE;
    }
#endif

    // determine palette configuration
    GLuint indexBits = 8;
    GLuint paletteFormat = 0;
    GLuint paletteType = 0;
    _jsPaletteGetDesc( internalFormat, &indexBits, &paletteFormat, &paletteType );

    const GLuint paletteEntries = 1 << indexBits;
    const GLuint paletteEntrySize = _jsGetPixelSize( paletteFormat, paletteType );
    const GLuint mask = (( 1 << indexBits ) - 1 );

    // check size
    unsigned int needSize = paletteEntries * paletteEntrySize;
    GLsizei w = width;
    GLsizei h = height;
    GLsizei d = depth;
    for ( int i = 0; i < 1 - level; ++i )
    {
        needSize += _jsPad( w * h * d * indexBits, 8 ) / 8;
        w = ( w > 1 ) ? ( w / 2 ) : 1;
        h = ( h > 1 ) ? ( h / 2 ) : 1;
        d = ( d > 1 ) ? ( d / 2 ) : 1;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( imageSize != ( int )needSize )
    {
        _jsSetError( GL_INVALID_VALUE );
        return GL_FALSE;
    }
#endif


    // allocate deindexing buffer
    GLvoid* buffer = jsMalloc( width * height * depth * paletteEntrySize );
#ifndef JS_NO_ERROR_CHECK
    if ( buffer == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return GL_FALSE;
    }
#endif

    // iterate over the levels
    const char* pixels = ( const char* )data + paletteEntries * paletteEntrySize;
    w = width;
    h = height;
    d = depth;
    for ( int i = 0; i < 1 - level; ++i )
    {
        // get the image
        _jsGetImage( target, i, &texture, &image, MAX( width, height ) );


        if ( data != NULL )
        {
            // deindex the image and call the unindexed _jsSetImage()
            char* dptr = ( char * )buffer;
            for ( int i = 0; i < w*h*d; ++i )
            {
                // determine the lookup index
                const GLuint srcOffset = indexBits * i;
                const GLuint srcBit = srcOffset % 8;
                const GLuint srcByte = srcOffset / 8;

                const GLuint paletteEntry = ( pixels[srcByte] >> ( 8 - srcBit - indexBits ) ) & mask;

                // deindex into the temporary buffer
                memcpy(
                    dptr,
                    ( const char* )data + paletteEntry*paletteEntrySize,
                    paletteEntrySize );

                dptr += paletteEntrySize;
            }

            // call non-paletted function
            _jsSetImage(
                image,
                paletteFormat,
                w, h, d,
                1,		// alignment
                paletteFormat,
                paletteType,
                buffer );
        }
        else
            _jsSetImage(
                image,
                paletteFormat,
                w, h, d,
                1,		// alignment
                paletteFormat,
                paletteType,
                NULL );

        // update pointer
        pixels += _jsPad( w * h * d * indexBits, 8 ) / 8;

        // update size for next level
        w = ( w > 1 ) ? ( w / 2 ) : 1;
        h = ( h > 1 ) ? ( h / 2 ) : 1;
        d = ( d > 1 ) ? ( d / 2 ) : 1;
    }

    // deallocate deindexing buffer
    jsFree( buffer );

    if ( level < 0 ) texture->hasMipmaps = GL_TRUE;
    return GL_TRUE;
}
