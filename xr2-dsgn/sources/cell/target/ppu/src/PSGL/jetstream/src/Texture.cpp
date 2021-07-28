/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Texturing API
 */

#include <limits.h>
#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>
#include <PSGL/PerfCounters.h>


/** @addtogroup OpenGL
 *
 * @{
 */

/**
 *  @short Generate texture names
 *
 *  glGenTextures() returns <I><c>n</c></I> texture names in <I><c>textures</c></I>. There is no guarantee that
 *  the names form a contiguous set of integers.  However, it is guaranteed that none of the
 *  returned names was in use immediately before the call to glGenTextures().
 *
 *  The generated textures have no dimensionality; they assume the dimensionality of the
 *  texture target to which they are first bound (see glBindTexture()).
 *
 *  Texture names returned by a call to glGenTextures() are not returned by subsequent calls,
 *  unless they are first deleted with glDeleteTextures().
 *
 *  @param n Specifies the number of texture names to be generated.
 *
 *  @param textures Specifies an array in which the generated texture names are stored.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @sa glBindTexture, glCopyTexImage2D, glDeleteTextures, glTexImage2D, glTexParameterf
 */

GLAPI void APIENTRY glGenTextures( GLsizei n, GLuint *textures )
{
    JS_APIENTRY_TIMER( glGenTextures );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glGenTextures, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             textures, ( n*sizeof( GLuint ) ), INPUT_OUTPUT, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    _jsTexNameSpaceGenNames( &LContext->textureNameSpace, n, textures );
}

/**
 *  @short Delete texture names
 *
 *  glDeleteTextures() deletes <I><c>n</c></I> textures named by the elements of the array <I><c>textures</c></I>.
 *  After a texture is deleted, it has no contents or dimensionality, and its name is free for reuse
 *  (for example by glGenTextures()). If a texture that is currently bound is deleted, the binding
 *  reverts to 0 (the default texture).
 *
 *  glDeleteTextures() silently ignores 0&rsquo;s and names that do not correspond to existing textures.
 *
 *  @param n Specifies the number of texture names to be deleted.
 *
 *  @param textures Specifies an array of textures to be deleted.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>n</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 *  @sa glBindTexture, glGenTextures
 */
GLAPI void APIENTRY glDeleteTextures( GLsizei n, const GLuint *textures )
{
    JS_APIENTRY_TIMER( glDeleteTextures );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glDeleteTextures, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             n, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             textures, ( n*sizeof( GLuint ) ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    for ( int i = 0;i < n;++i )
    {
        if ( textures[i] ) _jsTextureUnbind( LContext, textures[i] );
    }
    _jsTexNameSpaceDeleteNames( &LContext->textureNameSpace, n, textures );
}

/**
 *  @short Set texture parameters
 *
 *  @param target  Specifies the target texture, which must be one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>,
 *  or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param pname   Specifies the symbolic name of a single-valued texture parameter.
 *  <I><c>pname</c></I> can be one of the following:
 *  <CODE>GL_TEXTURE_ALLOCATION_HINT_SCE</CODE>,
 *  <CODE>GL_TEXTURE_MULTISAMPLING_HINT_SCE</CODE>,
 *  <CODE>GL_TEXTURE_MIN_FILTER</CODE>, <CODE>GL_TEXTURE_MAG_FILTER</CODE>,
 *  <CODE>GL_TEXTURE_LOD_BIAS</CODE>, <CODE>GL_TEXTURE_MIN_LOD</CODE>, <CODE>GL_TEXTURE_MAX_LOD</CODE>,
 *  <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, <CODE>GL_TEXTURE_WRAP_R</CODE>.
 *  <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>,
 *  <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE>,
 *  <CODE>GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_R_SCE</CODE>, <CODE>GL_TEXTURE_GAMMA_REMAP_G_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_B_SCE</CODE>, or <CODE>GL_TEXTURE_GAMMA_REMAP_A_SCE</CODE>.
 *
 *  @param param   Specifies the value of <I><c>pname</c></I>.
 *
Texture mapping is a technique that applies an image onto an object&rsquo;s surface as if the image were a decal or cellophane
shrink-wrap. The image is created in texture space, with an (<I>s, t, r, q</I>) coordinate system. A texture is a two- or
three-dimensional image and a set of parameters that determine how samples are derived from the image.
 *
glTexParameterf() assigns the value or values in <I>param</I> to the texture parameter specified as <I><c>pname</c></I>.
glTexParameterf() is one of four recommended variants (glTexParameterf(),
glTexParameteri(), glTexParameterfv(), and glTexParameteriv()) that differ
only by whether the value of <i><c>param</c></i> is represented as a <c>GLfloat</c>, a
<c>GLint</c>, an array of <c>GLfloats</c>, or an array of <c>GLints</c>, respectively.
This description of glTexParameterf() applies to all of the variants.

<I><c>target</c></I> defines the target texture, which is one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>, or
<CODE>GL_TEXTURE_CUBE_MAP</CODE>. The following symbols are accepted in <I><c>pname</c></I>:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_TEXTURE_ALLOCATION_HINT_SCE</CODE></TD>
 * <TD>
 * The texture allocation hint is a request to allocate texture images with a
 * particular memory pool and layout.  Valid values for the parameter are:
 * <UL>
 * <LI><CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE></LI>
 * <LI><CODE>GL_TEXTURE_LINEAR_GPU_SCE</CODE></LI>
 * <LI><CODE>GL_TEXTURE_TILED_GPU_SCE</CODE></LI>
 * <LI><CODE>GL_TEXTURE_LINEAR_SYSTEM_SCE</CODE></LI>
 * <LI><CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE></LI>
 * </UL>
 * The default parameter value is <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE>.
 *
 * The <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> and <CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE>
 * allocations have relatively fast draw performance (as fast or faster than
 * <CODE>GL_TEXTURE_LINEAR_GPU_SCE</CODE> or <CODE>GL_TEXTURE_LINEAR_SYSTEM_SCE</CODE>
 * respectively) and relatively slow render target
 * performance (when attached to a frame buffer object).  Texture images must
 * have all power of two dimensions to be allocated as swizzled.  Texture
 * images that do not have all power of two dimensions but are requested
 * with the <CODE>GL_TEXTURE_SWIZZLED_GPU_SCE</CODE> or <CODE>GL_TEXTURE_SWIZZLED_SYSTEM_SCE</CODE>
 * value will be handled as if
 * they had a <CODE>GL_TEXTURE_LINEAR_GPU_SCE</CODE> or <CODE>GL_TEXTURE_LINEAR_SYSTEM_SCE</CODE>
 * value instead.
 *
 * The <CODE>GL_TEXTURE_LINEAR_GPU_SCE</CODE> allocation has relatively slow
 * draw performance and moderate render target performance.
 *
  * The <CODE>GL_TEXTURE_TILED_GPU_SCE</CODE> allocation has relatively fast
 * draw performance and the fastest render target performance.  Cube maps
 * must not be mipmapped and cube map
 * images must have a base level height and width that is a multiple of 16
 * to be allocated with this value.  Otherwise, the cube map images will
 * be allocated as if <CODE>GL_TEXTURE_LINEAR_GPU_SCE</CODE>
 * were used instead.
 *
 * The RSX&trade; will be faster with local memory than with host memory.
 *
 * For RSX&trade; memory, textures are not guaranteed to be given the requested allocation,
 * depending on the capability and availability of hardware resources. If host memory is requested,
 * then host memory is allocated for the texture.
 </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MULTISAMPLING_HINT_SCE</CODE></TD>
 * <TD>
 * The texture multisampling hint gives information about what type of rendering
 * is expected on the particular texture so that PSGL can choose the best compression format.
 * Valid values for the parameter are:
 * <UL>
 * <LI><CODE>GL_MULTISAMPLING_NONE_SCE</CODE></LI>
 * <LI><CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE></LI>
 * <LI><CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE></LI>
 * <LI><CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE></LI>
 * </UL>
 * The default parameter value is <CODE>GL_MULTISAMPLING_NONE_SCE</CODE>.
 * 
 * In general, you should use the same value as the multisampling parameter set on the frame buffer objects that this texture is attached to.
 * 
 * For more information about multisampling, including an example demonstrating how to use
 * multisampling in a PSGL application, refer to the "Multisampling" section of the
 * <i>PlayStation&reg; Graphics Library Programming Guide</i>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MIN_FILTER</CODE></TD>
 * <TD>
 * The texture minifying function is used whenever the pixel being textured maps to an area greater than one texture element. There are six defined minifying functions. Two of them use the nearest one or nearest four texture elements to compute the texture value. The other four use mipmaps.
 *
  * A mipmap is an ordered set of arrays representing the same image at progressively lower resolutions. If the texture has
 * dimensions 2<sup>n</sup> x 2<sup>m</sup>, there are <I>max(n,m)+1</I> mipmaps. The first mipmap is the original texture, with dimensions 2<sup>n</sup> x 2<sup>m</sup>.
 * Each subsequent mipmap has dimensions 2<sup>(k - 1)</sup> x 2<sup>(l - 1)</sup>, where 2<sup>k</sup> x 2<sup>l</sup> are the dimensions of the previous mipmap, until
 * either k = 0 or l = 0. At that point, subsequent mipmaps have dimension 1 x 2<sup>(l - 1)</sup> or 2<sup>(k - 1)</sup> x 1 until the final mipmap,
 * which has dimension 1 x 1. To define the mipmaps, call glTexImage2D(), glTexImage3D(), or glCopyTexImage2D()
 * with the level argument indicating
 * the order of the mipmaps. Level 0 is the original texture. Level <I>max(n,m)</I> is the final 1 x 1 mipmap.
* </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MAG_FILTER</CODE></TD>
 * <TD>
 * The texture magnification function is used when the pixel being textured maps to an area less than or equal to one texture element. It sets the texture magnification function to either <CODE>GL_NEAREST</CODE> or <CODE>GL_LINEAR</CODE> (see below). <CODE>GL_NEAREST</CODE> is generally faster than <CODE>GL_LINEAR</CODE>, but it can produce textured images with sharper edges because the transition between texture elements is not as smooth. The initial value of <CODE>GL_TEXTURE_MAG_FILTER</CODE> is <CODE>GL_LINEAR</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_LOD_BIAS</CODE></TD>
 * <TD>
 * The value specified is added to the calculated texture level-of-detail parameter that is used when
 * selecting mipmap(s) for minification.  The default value of <CODE>GL_TEXTURE_LOD_BIAS</CODE> is 0,
 * indicating no adjustment.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MIN_LOD</CODE></TD>
 * <TD>
 * Sets the minimum level-of-detail parameter to limit the selection of the highest resolution mipmap.
 * The initial value is -1000.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MAX_LOD</CODE></TD>
 * <TD>
 * Sets the maximum level-of-detail parameter to limit the selection of the lowest resolution mipmap.
 * The initial value is 1000.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_WRAP_S</CODE></TD>
 * <TD>
 * Sets the wrap parameter for texture coordinate s to either <CODE>GL_CLAMP</CODE>, <CODE>GL_CLAMP_TO_EDGE</CODE>,
<CODE>GL_REPEAT</CODE>, <CODE>GL_MIRRORED_REPEAT</CODE>, <CODE>GL_MIRROR_CLAMP_EXT</CODE>,
<CODE>GL_MIRROR_CLAMP_TO_EDGE_EXT</CODE>, <CODE>GL_MIRROR_CLAMP_TO_BORDER_EXT</CODE>, or
<CODE>GL_CLAMP_TO_BORDER</CODE>.

<CODE>GL_CLAMP</CODE> causes s coordinates to be clamped to the range [0, 1]
and is useful for preventing wrapping artifacts when mapping a single image onto an object.

<CODE>GL_CLAMP_TO_EDGE</CODE> causes s coordinates to be clamped to the range [ 1/2N , 1 - 1/2N ], where N is the
size of the texture in the direction of clamping.

<CODE>GL_REPEAT</CODE> causes the integer part of the s coordinate to be ignored; the GL uses only the fractional
part, thereby creating a repeating pattern.

<CODE>GL_MIRRORED_REPEAT</CODE> causes s coordinates to be the fractional part of the texture coordinate when the
integer part is even, and 1 - frac(s) when the integer part is odd, where frac(s) represents the fractional part of s.

<CODE>GL_MIRROR_CLAMP_EXT</CODE> mirrors and clamps the texture coordinate, min(1, max(1/(2 &times; N), abs(s))),
where N is the size of the texture image in the direction of wrapping.  <CODE>GL_MIRROR_CLAMP_TO_EDGE_EXT</CODE>
mirrors and clamps to edge the texture coordinate, min(1 - 1/(2 &times; N), max(1/(2 &times; N), abs(s))), where N is the
size of the texture image in the direction of wrapping. <CODE>GL_MIRROR_CLAMP_TO_BORDER_EXT</CODE> mirrors and clamps
the texture coordinate to min(1 + 1/(2 &times; N), max(1/(2 &times; N), abs(s))), where N is the size of the texture
image in the direction of wrapping.

<CODE>GL_CLAMP_TO_BORDER</CODE> causes s coordinates to be clamped to the range [ -1/2N , 1 + 1/2N ], where N is the
size of the texture image in the direction of clamping.  Border texture elements are accessed only if wrapping is set
to <CODE>GL_CLAMP</CODE>.

Initially, <CODE>GL_TEXTURE_WRAP_S</CODE> is set to <CODE>GL_REPEAT</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_WRAP_T</CODE></TD>
 * <TD>
 * Sets the wrap parameter for texture coordinate t to either <CODE>GL_CLAMP</CODE>, <CODE>GL_CLAMP_TO_EDGE</CODE>,
<CODE>GL_REPEAT</CODE>, <CODE>GL_MIRRORED_REPEAT</CODE>, <CODE>GL_MIRROR_CLAMP_EXT</CODE>,
<CODE>GL_MIRROR_CLAMP_TO_EDGE_EXT</CODE>, <CODE>GL_MIRROR_CLAMP_TO_BORDER_EXT</CODE>, or
<CODE>GL_CLAMP_TO_BORDER</CODE>. See <CODE>GL_TEXTURE_WRAP_S</CODE> for details.

Initially, <CODE>GL_TEXTURE_WRAP_T</CODE> is set to <CODE>GL_REPEAT</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_WRAP_R</CODE></TD>
 * <TD>
 * Sets the wrap parameter for texture coordinate r to either <CODE>GL_CLAMP</CODE>, <CODE>GL_CLAMP_TO_EDGE</CODE>,
<CODE>GL_REPEAT</CODE>, <CODE>GL_MIRRORED_REPEAT</CODE>, <CODE>GL_MIRROR_CLAMP_EXT</CODE>,
<CODE>GL_MIRROR_CLAMP_TO_EDGE_EXT</CODE>, <CODE>GL_MIRROR_CLAMP_TO_BORDER_EXT</CODE>, or
<CODE>GL_CLAMP_TO_BORDER</CODE>. See <CODE>GL_TEXTURE_WRAP_S</CODE> for details.

Initially, <CODE>GL_TEXTURE_WRAP_R</CODE> is set to <CODE>GL_REPEAT</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE></TD>
 * <TD>
 * Specifies the texture comparison mode for currently bound depth textures; that is, a texture whose internal
 * format is <CODE>GL_DEPTH_COMPONENT_*</CODE>. See glTexImage2D() for a list of all depth texture internal formats.
 * Permissible values are <CODE>GL_COMPARE_R_TO_TEXTURE_ARB</CODE> or <CODE>GL_NONE</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE></TD>
 * <TD>
 * Specifies the comparison operator used when <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE> is set to
 * <CODE>GL_COMPARE_R_TO_TEXTURE_ARB</CODE>. Permissible values are <CODE>GL_LEQUAL</CODE> or <CODE>GL_GEQUAL</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE></TD>
 * <TD>
 * Specifies a single symbolic constant indicating depth values should be treated during
 * filtering and texture application. Accepted values are
 * <CODE>GL_LUMINANCE</CODE>, <CODE>GL_INTENSITY</CODE>, and <CODE>GL_ALPHA</CODE>.
 * The initial value is <CODE>GL_LUMINANCE</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE></TD>
 * <TD>
 * Specifies the desired maximum degree of anisotropy used during mipmap filtering.
 * The degree of anisotropy is the ratio of the longer side to the smaller side of the footprint of
 * the texture filter. The anisotropy value must be greater than or equal to 1, and it must be
 * less than the value returned by querying
 * <CODE>GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT</CODE>. The default value is 1.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_MAX_LEVEL</CODE></TD>
 * <TD>Sets the index of the highest defined mipmap level. This is an integer value. The initial value is 1000.</TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE</CODE></TD>
 * <TD>
 * A boolean value that indicates whether or not a Cg vertex program can access texel data
 * from a texture. The initial value is <CODE>GL_FALSE</CODE>. 
 * For a vertex texture, the internal format must be one of <c>GL_FLOAT_RGBA32</c>, 
 * <c>GL_RGBA32F_ARB</c>, or <c>GL_LUMINANCE32F_ARB</c>.
 * </TD>
 * </TR>
 * <TR>
 * <TD>
 * <CODE>GL_TEXTURE_GAMMA_REMAP_R_SCE</CODE>,
 * <CODE>GL_TEXTURE_GAMMA_REMAP_G_SCE</CODE>,
 * <CODE>GL_TEXTURE_GAMMA_REMAP_B_SCE</CODE> or
 * <CODE>GL_TEXTURE_GAMMA_REMAP_A_SCE</CODE>
 * </TD>
 * <TD>
 * Specifies whether or not to enable Gamma Correction to the different channels (R,G,B or A) of the texture. If <I><c>param</c></I> is 0, Gamma Correction is disabled for that channel, otherwise it is enabled.
 * </TD>
 * </TR>
 * </TABLE>
 *
 *  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_MIN_FILTER</CODE>, then <I><c>param</c></I> supplies a function for minifying the texture
 *  as one of the following:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NEAREST</CODE></TD>
 * <TD>
 * Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LINEAR</CODE></TD>
 * <TD>
 * Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured. These can include border texture elements, depending on the values of <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, and <CODE>GL_TEXTURE_WRAP_R</CODE>, and on the exact mapping.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NEAREST_MIPMAP_NEAREST</CODE></TD>
 * <TD>
 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses the <CODE>GL_NEAREST</CODE> criterion (the texture element nearest to the center of the pixel) to produce a texture value.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LINEAR_MIPMAP_NEAREST</CODE></TD>
 * <TD>
 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses the <CODE>GL_LINEAR</CODE> criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NEAREST_MIPMAP_LINEAR</CODE></TD>
 * <TD>
 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the <CODE>GL_NEAREST</CODE> criterion (the texture element nearest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LINEAR_MIPMAP_LINEAR</CODE></TD>
 * <TD>
 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the <CODE>GL_LINEAR</CODE> criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to produce a texture value from each mipmap. The final texture value is a weighted average of those two values.
 * </TD>
 * </TR>
 * </TABLE>
As more texture elements are sampled in the minification process, fewer aliasing artifacts will be apparent. While the <CODE>GL_NEAREST</CODE> and <CODE>GL_LINEAR</CODE> minification functions can be faster than the other four, they sample only one or four texture elements to determine the texture value of the pixel being rendered and can produce moire patterns or ragged transitions. The initial value of <CODE>GL_TEXTURE_MIN_FILTER</CODE> is <CODE>GL_NEAREST_MIPMAP_LINEAR</CODE>.
 *
 *  If <I><c>pname</c></I> is <CODE>GL_TEXTURE_MAG_FILTER</CODE>, then <I><c>param</c></I> supplies a function for magnifying the texture
 *  as one of the following:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_NEAREST</CODE></TD>
 * <TD>
 * Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LINEAR</CODE></TD>
 * <TD>
 * Returns the weighted average of the four texture elements that are closest to the center of the pixel being textured. These can include border texture elements, depending on the values of <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, and <CODE>GL_TEXTURE_WRAP_R</CODE>, and on the exact mapping.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * If <I><c>pname</c></I> is <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE>, then <I><c>param</c></I> specifies the texture comparison mode
 * as one of the following:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_COMPARE_R_TO_TEXTURE_ARB</CODE></TD>
 * <TD>
 * Specifies that the interpolated and clamped <I>r</I> texture coordinate should be compared
 * to the value in the currently bound depth texture. The comparison is evaluated based
 * upon the operator set by <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>.  The result of the comparison
 * is assigned to luminance, intensity, or alpha, as specified by <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_NONE</CODE></TD>
 * <TD>
 * Specifies that the luminance, intensity, or alpha (as specified by
 * <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>) should be assigned the appropriate value from the
 * currently bound depth texture.
 *
 * Besides specifying <CODE>GL_NONE</CODE> from the run-time, you must also use a specific function to load the data, such as
 * <CODE>texDepth2D</CODE>, which uses the same arguments as <CODE>tex2D</CODE> and returns a single float.
 * Other variants of the <CODE>texDepth</CODE> instructions are also available, such as <CODE>texDepth2Dproj</CODE>, and use the same arguments as the corresponding <CODE>tex</CODE> instruction.
 *
 * Note that when filtering is disabled, the depth is returned with full precision.  However, when filtering is
 * enabled, only the upper 8 bits of the mantissa have the correct value.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * If <I>pname</I> is <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>, then <I><c>param</c></I> specifies the texture comparison operator
 * used when <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE> is set to <CODE>GL_COMPARE_R_TO_TEXTURE_ARB</CODE>.
 * Permissible values are:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_LEQUAL</CODE></TD>
 * <TD>
 * Specifies that the comparison of the <I>r</I> texture coordinate to the depth value evaluates to 1.0, if r <= Dt,
 * or to 0.0, if r > Dt.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_GEQUAL</CODE></TD>
 * <TD>
 * Specifies that the comparison of the <I>r</I> texture coordinate to the depth value evaluates to 1.0, if r >= Dt,
 * or to 0.0, if r < Dt.
 * </TD>
 * </TR>
 * </TABLE>

 * The evaluated value is assigned to one of the luminance, intensity, or alpha,
 * as specified by <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>.
 *
 *  @note
 *  Suppose that a program has enabled texturing (by calling glEnable() with argument <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>, or cube mapping constant) and has set <CODE>GL_TEXTURE_MIN_FILTER</CODE> to one of the functions that requires a mipmap. If either the dimensions of the texture images currently defined (with previous calls to glTexImage2D(), glTexImage3D(), or glCopyTexImage2D()) do not follow the proper sequence for mipmaps (described above), or there are fewer texture images defined than are needed, or the set of texture images have differing numbers of texture components, then it is as if texture mapping were disabled.
 *
Linear filtering accesses the four nearest texture elements.
 *
glTexParameterf() specifies the texture parameters for the active texture object, specified by calling glActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> or <I><c>pname</c></I> is not one of the accepted defined values.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>param</c></I> should have a defined
constant value (based on the value of <I><c>pname</c></I>) and does not.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALID</CODE></TD>
<TD><I><c>pname</c></I> is <CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE>
and <I><c>param</c></I> is either less than 1.0 or greater than the value indicated by querying
<CODE>GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT</CODE>.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCopyTexImage2D, glCopyTexSubImage2D, glCopyTexSubImage3D, glEnable,
 * glPixelStorei, glTexEnvfv, glTexImage2D, glTexImage3D, glTexSubImage2D, glTexSubImage3D
 */
GLAPI void APIENTRY glTexParameterf( GLenum target, GLenum pname, GLfloat param )
{
    JS_APIENTRY_TIMER( glTexParameterf );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexParameterf, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLfloat ), INPUT_BY_VALUE, DPM_API_ARGUMENT_FLOAT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    jsTexture *texture = _jsGetCurrentTexture( LContext->CurrentImageUnit, target );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    switch ( pname )
    {
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MAX_LEVEL:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
        case GL_TEXTURE_GAMMA_REMAP_R_SCE:
        case GL_TEXTURE_GAMMA_REMAP_G_SCE:
        case GL_TEXTURE_GAMMA_REMAP_B_SCE:
        case GL_TEXTURE_GAMMA_REMAP_A_SCE:
        case GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE:
        case GL_TEXTURE_ALLOCATION_HINT_SCE:
        case GL_TEXTURE_MULTISAMPLING_HINT_SCE:
            glTexParameteri( target, pname, ( GLint )param );
            return;
        case GL_TEXTURE_MIN_LOD:
            texture->minLod = param;
            break;
        case GL_TEXTURE_MAX_LOD:
            texture->maxLod = param;
            break;
        case GL_TEXTURE_LOD_BIAS:
            texture->lodBias = param;
            break;
        case GL_TEXTURE_MAX_ANISOTROPY_EXT:
#ifndef JS_NO_ERROR_CHECK
            if ( param < 1.f )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            texture->maxAnisotropy = param;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED  | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
@short Set texture parameters
 *
 *  glTexParameteri() assigns the value or values in <I><c>param</c></I> to the texture parameter specified as
 *  <I><c>pname</c></I>.  It differs from glTexParameterf() only in argument type.
 *  Please refer to glTexParameterf() for more information.
 *
 *  @param target  Specifies the target texture, which must be one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>,
 *  or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param pname   Specifies the symbolic name of a single-valued texture parameter.
 *  <I><c>pname</c></I> can be one of the following:
 *  <CODE>GL_TEXTURE_ALLOCATION_HINT_SCE</CODE>,
 *  <CODE>GL_TEXTURE_MIN_FILTER</CODE>, <CODE>GL_TEXTURE_MAG_FILTER</CODE>,
 *  <CODE>GL_TEXTURE_LOD_BIAS</CODE>, <CODE>GL_TEXTURE_MIN_LOD</CODE>, <CODE>GL_TEXTURE_MAX_LOD</CODE>,
 *  <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, <CODE>GL_TEXTURE_WRAP_R</CODE>.
 *  <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>,
 *  <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE>,
 *  <CODE>GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_R_SCE</CODE>, <CODE>GL_TEXTURE_GAMMA_REMAP_G_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_B_SCE</CODE>, or <CODE>GL_TEXTURE_GAMMA_REMAP_A_SCE</CODE>.
 *
 *  @param param   Specifies the value of <I><c>pname</c></I>.
 *
 */

GLAPI void APIENTRY glTexParameteri( GLenum target, GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glTexParameteri );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexParameteri, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    jsTexture *texture = _jsGetCurrentTexture( LContext->CurrentImageUnit, target );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    switch ( pname )
    {
        case GL_TEXTURE_MIN_FILTER:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsMinFilter( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->minFilter = param;
            if ( texture->referenceBuffer == 0 )
            {
                texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
            }
            break;
        case GL_TEXTURE_MAG_FILTER:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsMagFilter( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->magFilter = param;
            break;
        case GL_TEXTURE_MAX_LEVEL:
#ifndef JS_NO_ERROR_CHECK
            if ( texture->referenceBuffer )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
            if ( param < 0 )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            texture->maxLevel = param;
            break;
        case GL_TEXTURE_WRAP_S:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsWrapMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->wrapS = param;
            break;
        case GL_TEXTURE_WRAP_T:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsWrapMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->wrapT = param;
            break;
        case GL_TEXTURE_WRAP_R:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsWrapMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->wrapR = param;
            break;
        case GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE:
#ifndef JS_NO_ERROR_CHECK
            if ( texture->referenceBuffer )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
#endif
            if ( param != 0 )
            {
#ifndef JS_NO_ERROR_CHECK
                if ( texture->target != GL_TEXTURE_2D )
                {
                    _jsSetError( GL_INVALID_OPERATION );
                    return;
                }
#endif
                texture->vertexEnable = GL_TRUE;
            }
            else texture->vertexEnable = GL_FALSE;
            texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
            break;
        case GL_TEXTURE_ALLOCATION_HINT_SCE:
#ifndef JS_NO_ERROR_CHECK
            if ( texture->referenceBuffer )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
            switch ( param )
            {
                case GL_TEXTURE_TILED_GPU_SCE:
                case GL_TEXTURE_LINEAR_GPU_SCE:
                case GL_TEXTURE_SWIZZLED_GPU_SCE:
                case GL_TEXTURE_LINEAR_SYSTEM_SCE:
                case GL_TEXTURE_SWIZZLED_SYSTEM_SCE:
                    break;
                default:
                    _jsSetError( GL_INVALID_ENUM );
                    return;
            }
#endif
            texture->usage = param;
            texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
            break;
        case GL_TEXTURE_MULTISAMPLING_HINT_SCE:
#ifndef JS_NO_ERROR_CHECK
            if ( texture->referenceBuffer )
            {
                _jsSetError( GL_INVALID_OPERATION );
                return;
            }
            if ( !_jsIsMultisampling( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->aaUsage = param;
            texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
            break;
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_LOD_BIAS:
        case GL_TEXTURE_MAX_ANISOTROPY_EXT:
            glTexParameterf( target, pname, ( GLfloat )param );
            return;
        case GL_DEPTH_TEXTURE_MODE_ARB:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsDepthTextureMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->depthMode = param;
            break;
        case GL_TEXTURE_COMPARE_MODE_ARB:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsCompareMode( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->compareMode = param;
            break;
        case GL_TEXTURE_COMPARE_FUNC_ARB:
#ifndef JS_NO_ERROR_CHECK
            if ( !_jsIsTestFunc( param ) )
            {
                _jsSetError( GL_INVALID_ENUM );
                return;
            }
#endif
            texture->compareFunc = param;
            break;
        case GL_TEXTURE_GAMMA_REMAP_R_SCE:
        case GL_TEXTURE_GAMMA_REMAP_G_SCE:
        case GL_TEXTURE_GAMMA_REMAP_B_SCE:
        case GL_TEXTURE_GAMMA_REMAP_A_SCE:
        {
            GLuint bit = 1 << ( pname - GL_TEXTURE_GAMMA_REMAP_R_SCE );
            if ( param ) texture->gammaRemap |= bit;
            else texture->gammaRemap &= ~bit;
        }
        break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
@short Set texture parameters

 *  glTexParameterfv() assigns the value or values in <I><c>params</c></I> to the texture parameter specified as
 *  <I><c>pname</c></I>.  It differs from glTexParameterf() only in argument type.
 *  Please refer to glTexParameterf() for more information.


 *  @param target  Specifies the target texture, which must be one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>,
 *  or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param pname   Specifies the symbolic name of a texture parameter.
 *  <I><c>pname</c></I> can be one of the following: <CODE>GL_TEXTURE_MIN_FILTER</CODE>, <CODE>GL_TEXTURE_MAG_FILTER</CODE>,
 *  <CODE>GL_TEXTURE_LOD_BIAS</CODE>, <CODE>GL_TEXTURE_MIN_LOD</CODE>, <CODE>GL_TEXTURE_MAX_LOD</CODE>,
 *  <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, <CODE>GL_TEXTURE_WRAP_R</CODE>,
 *  <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>,
 *  <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE>,
 *  <CODE>GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_R_SCE</CODE>, <CODE>GL_TEXTURE_GAMMA_REMAP_G_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_B_SCE</CODE>, or <CODE>GL_TEXTURE_GAMMA_REMAP_A_SCE</CODE>.
 *
 *  @param params   Specifies a pointer to an array where the value or values of <I><c>pname</c></I> are stored.
 *
*/

GLAPI void APIENTRY glTexParameterfv( GLenum target, GLenum pname, const GLfloat *params )
{
    JS_APIENTRY_TIMER( glTexParameterfv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexParameterfv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLfloat ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }
    PSGLcontext*	LContext = _CurrentContext;
#ifndef JS_NO_ERROR_CHECK
    JS_ASSERT( LContext );
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    jsTexture *texture = _jsGetCurrentTexture( LContext->CurrentImageUnit, target );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    switch ( pname )
    {
		case GL_TEXTURE_BORDER_COLOR:
			{
				JS_ASSERT( params );
				jsColorRGBAf *borderColor = ( jsColorRGBAf * )params;
				texture->borderColor.R = (( borderColor->R ) >= 0.0f ? (( borderColor->R ) > 1.0f ? 1.0f : ( borderColor->R ) ) : 0.0f );
				texture->borderColor.G = (( borderColor->G ) >= 0.0f ? (( borderColor->G ) > 1.0f ? 1.0f : ( borderColor->G ) ) : 0.0f );
				texture->borderColor.B = (( borderColor->B ) >= 0.0f ? (( borderColor->B ) > 1.0f ? 1.0f : ( borderColor->B ) ) : 0.0f );
				texture->borderColor.A = (( borderColor->A ) >= 0.0f ? (( borderColor->A ) > 1.0f ? 1.0f : ( borderColor->A ) ) : 0.0f );
			}
			break;;
#ifndef JS_NO_ERROR_CHECK
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MAX_LEVEL:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_LOD_BIAS:
        case GL_TEXTURE_MAX_ANISOTROPY_EXT:
        case GL_TEXTURE_GAMMA_REMAP_R_SCE:
        case GL_TEXTURE_GAMMA_REMAP_G_SCE:
        case GL_TEXTURE_GAMMA_REMAP_B_SCE:
        case GL_TEXTURE_GAMMA_REMAP_A_SCE:
        case GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE:
        case GL_TEXTURE_ALLOCATION_HINT_SCE:
        case GL_TEXTURE_MULTISAMPLING_HINT_SCE:
        case GL_DEPTH_TEXTURE_MODE_ARB:
        case GL_TEXTURE_COMPARE_MODE_ARB:
		case GL_TEXTURE_COMPARE_FUNC_ARB:
#else
		default:
#endif
            glTexParameterf( target, pname, *params );
            return;
#ifndef JS_NO_ERROR_CHECK
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
#endif
    }
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
@short Set texture parameters

 *  glTexParameteriv() assigns the value or values in <I><c>params</c></I> to the texture parameter specified as
 *  <I><c>pname</c></I>.  It differs from glTexParameterf() only in argument type.
 *  Please refer to glTexParameterf() for more information.

 *  @param target  Specifies the target texture, which must be one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>,
 *  or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param pname   Specifies the symbolic name of a texture parameter.
 *  <I><c>pname</c></I> can be one of the following: <CODE>GL_TEXTURE_MIN_FILTER</CODE>, <CODE>GL_TEXTURE_MAG_FILTER</CODE>,
 *  <CODE>GL_TEXTURE_LOD_BIAS</CODE>, <CODE>GL_TEXTURE_MIN_LOD</CODE>, <CODE>GL_TEXTURE_MAX_LOD</CODE>,
 *  <CODE>GL_TEXTURE_WRAP_S</CODE>, <CODE>GL_TEXTURE_WRAP_T</CODE>, <CODE>GL_TEXTURE_WRAP_R</CODE>,
 *  <CODE>GL_TEXTURE_COMPARE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_COMPARE_FUNC_ARB</CODE>,
 *  <CODE>GL_DEPTH_TEXTURE_MODE_ARB</CODE>, <CODE>GL_TEXTURE_MAX_ANISOTROPY_EXT</CODE>,
 *  <CODE>GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_R_SCE</CODE>, <CODE>GL_TEXTURE_GAMMA_REMAP_G_SCE</CODE>,
 *  <CODE>GL_TEXTURE_GAMMA_REMAP_B_SCE</CODE>, or <CODE>GL_TEXTURE_GAMMA_REMAP_A_SCE</CODE>.
 *
 *  @param params   Specifies a pointer to an array where the value or values of <I><c>pname</c></I> are stored.
 *
*/

GLAPI void APIENTRY glTexParameteriv( GLenum target, GLenum pname, const GLint *params )
{
    JS_APIENTRY_TIMER( glTexParameteriv );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_3( glTexParameteriv, JSDPM_CALL_TYPE_STATE_TEX, , 3,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             params, sizeof( GLint ), INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
    }

    PSGLcontext*	LContext = _CurrentContext;
#ifndef JS_NO_ERROR_CHECK
    JS_ASSERT( LContext );
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif
    jsTexture *texture = _jsGetCurrentTexture( LContext->CurrentImageUnit, target );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    switch ( pname )
    {
		case GL_TEXTURE_BORDER_COLOR:
			{
				JS_ASSERT( params );
				texture->borderColor.R = params[0] > 0 ? float( params[0] ) / float( INT_MAX ) : 0.0f;
				texture->borderColor.G = params[1] > 0 ? float( params[1] ) / float( INT_MAX ) : 0.0f;
				texture->borderColor.B = params[2] > 0 ? float( params[2] ) / float( INT_MAX ) : 0.0f;
				texture->borderColor.A = params[3] > 0 ? float( params[3] ) / float( INT_MAX ) : 0.0f;
			}
			break;;
#ifndef JS_NO_ERROR_CHECK
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MAX_LEVEL:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_LOD_BIAS:
        case GL_TEXTURE_MAX_ANISOTROPY_EXT:
        case GL_TEXTURE_GAMMA_REMAP_R_SCE:
        case GL_TEXTURE_GAMMA_REMAP_G_SCE:
        case GL_TEXTURE_GAMMA_REMAP_B_SCE:
        case GL_TEXTURE_GAMMA_REMAP_A_SCE:
        case GL_TEXTURE_FROM_VERTEX_PROGRAM_SCE:
        case GL_TEXTURE_ALLOCATION_HINT_SCE:
        case GL_TEXTURE_MULTISAMPLING_HINT_SCE:
        case GL_DEPTH_TEXTURE_MODE_ARB:
        case GL_TEXTURE_COMPARE_MODE_ARB:
        case GL_TEXTURE_COMPARE_FUNC_ARB:
#else
		default:
#endif
            glTexParameteri( target, pname, *params );
            return;
#ifndef JS_NO_ERROR_CHECK
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
#endif
    }
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_PARAMETERS;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
	return;
}

/**
 *  @short Bind a named texture to a texturing target
 *
glBindTexture() lets you create or use a named texture. Calling glBindTexture() with <I><c>target</c></I> set to
one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP</CODE>,
and <I><c>name</c></I> set to the name of the new texture binds the texture name to the target.
When a texture is bound to a target, the previous binding for that target is automatically broken.
 *
Texture names are unsigned integers. The value 0 is reserved to represent the default texture for each texture target. Texture names and the corresponding texture contents are local to the texture-object space of the current GL rendering context.
 *
You may use glGenTextures() to generate a set of new texture names.
 *
While a texture is bound, GL operations on the target to which it is bound affect the bound texture. If texture mapping of the dimensionality of the target to which a texture is bound is active, the bound texture is used. In effect, the texture targets become aliases for the textures currently bound to them, and the texture name 0 refers to the default textures that were bound to them at initialization.
 *
A texture binding created with glBindTexture() remains active until a different texture is bound to the same target, or until the bound texture is deleted with glDeleteTextures().
 *
Once created, a named texture may be re-bound to the target of the matching dimensionality as often as needed. It is usually much faster to use glBindTexture() to bind an existing named texture to one of the texture targets than it is to reload the texture image using glTexImage2D() or glTexImage3D().
 *
 *  @param target Specifies the target to which the texture is bound. Must be
 *  one of <CODE>GL_TEXTURE_2D</CODE>, <CODE>GL_TEXTURE_3D</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param name Specifies the name of a texture.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of the allowable values.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glCompressedTexImage2D, glCompressedTexImage3D, glCopyTexImage2D, glDeleteTextures, glGenTextures,
 *  glGetIntegerv(), glTexImage2D, glTexImage3D, glTexParameterf
 */
#ifdef JS_DPM
int __jsNumTextureBinds;
#endif
GLAPI void APIENTRY glBindTexture( GLenum target, GLuint name )
{
    JS_APIENTRY_TIMER( glBindTexture );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glBindTexture, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             name, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
#ifndef JS_NO_ERROR_CHECK
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

#ifdef JS_DPM
    __jsNumTextureBinds++;
#endif

    _jsBindTextureInternal( unit, name, target );
}

/**
 *  @short Specify a two-dimensional texture image
 *
 *  Texturing maps a portion of a specified texture image onto each graphical primitive for which
 *  texturing is enabled. To enable and disable two-dimensional texturing, call glEnable() and glDisable()
 *  with argument <CODE>GL_TEXTURE_2D</CODE> or <CODE>GL_TEXTURE_CUBE_MAP</CODE>. Two-dimensional texturing is initially disabled.
 *
 *  To define texture images, call glTexImage2D(). The arguments describe the parameters of the texture image, such as height, width, width of the border, and number of color components provided. The last two arguments describe how the image is represented in memory.
 *
Data is read from <I><c>pixels</c></I> as a sequence of unsigned bytes, shorts, long integers, or floats, depending on <I><c>type</c></I>.
These values are grouped into sets of one, two, three, or four values, depending on <I><c>format</c></I>, to form elements.
 *
When <I><c>type</c></I> is <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, or something similar, each byte/float/type is interpreted as one color component, depending on <I><c>format</c></I>. When <I><c>type</c></I> is one of <CODE>GL_UNSIGNED_SHORT_5_6_5</CODE>, <CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE>, <CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE>, or something similar, each unsigned value is interpreted as containing all the components for a single pixel, with the color components arranged according to format.
 *
The first element corresponds to the lower left corner of the texture image. Subsequent elements progress left-to-right through the remaining texels in the lowest row of the texture image, and then in successively higher rows of the texture image. The final element corresponds to the upper right corner of the texture image.
 *
By default, adjacent pixels are taken from adjacent memory locations, except that after all <I><c>width</c></I> pixels are read, the read pointer is advanced to the next four-byte boundary. The four-byte row alignment is specified by glPixelStorei() with argument <CODE>GL_UNPACK_ALIGNMENT</CODE>, and it can be set to one, two, four, or eight bytes.

 * Some combinations of <i><c>format</c></I> and <i><c>type</c></i> cause the
 * texture data to be converted into an internal format that is
 * slightly different from the one requested by <i><c>internalFormat</c></i>,
 * wasting computing resources in the conversion.  For your program to
 * run its fastest, you should specify compatible combinations of
 * values for <i><c>format,</c></I> &nbsp; <i><c>type,</c></i> and <i><c>internalFormat.</c></i>

 * The following table lists the most efficient combinations of
 * <i><c>internalFormat</c></i>  &nbsp; (either generic or specific), <i><c>format,</c></I>
 * and <i><c>type</c></i>. The <i><c>internalFormat</c></i> value listed in this table only includes the hardware 
 * supported formats.

 * <TABLE rules=all>
 * <TR>
 * <TD><i><c>internalFormat</c></i></TD>
 * <TD><i><c>format</c></i></TD>
 * <TD><i><c>type</c></i></TD>
 * </TR>
 * <TR><TD><code>GL_ALPHA16</code></TD><TD><code>GL_ALPHA</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_ALPHA8</code></TD><TD><code>GL_ALPHA</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_INTENSITY16</code></TD><TD><code>GL_INTENSITY</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_INTENSITY8</code></TD><TD><code>GL_INTENSITY</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE16</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE8</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE16_ALPHA16</code></TD><TD><code>GL_LUMINANCE_ALPHA</code></TD><TD><code>GL_UNSIGNED_INT_16_16_REV_SCE</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE8_ALPHA8</code></TD><TD><code>GL_LUMINANCE_ALPHA</code></TD><TD><code>GL_UNSIGNED_SHORT_8_8_REV_SCE</code></TD></TR>
 * <TR><TD><code>GL_RGBA8</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8</code></TD></TR>
 * <TR><TD><code>GL_ARGB_SCE</code></TD><TD><code>GL_BGRA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB5_A1</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_UNSIGNED_SHORT_1_5_5_5_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB5</code></TD><TD><code>GL_RGB</code></TD><TD><code>GL_UNSIGNED_SHORT_5_6_5_REV</code></TD></TR>
 * <TR><TD><CODE>GL_RGBA16F_ARB</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_HALF_FLOAT_ARB</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE_ALPHA16F_ARB</code></TD><TD><code>GL_ALPHA_LUMINANCE_SCE</code></TD><TD><code>GL_HALF_FLOAT_ARB</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE32F_ARB</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_FLOAT</code></TD></TR>
 * <TR><TD><code>GL_RGBA32F_ARB</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_FLOAT</code></TD></TR>
 * <TR><TD><code>GL_DEPTH_COMPONENT24</code></TD><TD><code>GL_DEPTH_COMPONENT</code></TD><TD><code>GL_UNSIGNED_INT_24_8_SCE</code></TD></TR>
 * <TR><TD><code>GL_BGRA</code></TD><TD><code>GL_BGRA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8</code></TD></TR>
 * <TR><TD><code>GL_COMPRESSED_RGB_S3TC_DXT1_EXT</code></TD><TD><code>GL_RGB</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_COMPRESSED_RGBA_S3TC_DXT1_EXT</code>, <code>GL_COMPRESSED_RGBA_S3TC_DXT3_EXT</code> or <code>GL_COMPRESSED_RGBA_S3TC_DXT5_EXT</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * </TABLE>

 * The parameter <I><c>format</c></I> determines the composition of each
 * element in <I><c>pixels</c></I>. It can assume one of the following
 * symbolic values:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_ALPHA</CODE></TD>
 * <TD>
 * Each element is a single alpha component. The GL converts it to floating point and assembles it into an RGBA element by attaching 0 for red, green, and blue.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ARGB_SCE</CODE></TD>
 * <TD>
 * Each element contains all four components. The GL converts it to four 8-byte values in the order specified.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DEPTH_COMPONENT</CODE></TD>
 * <TD>
 * Each element is a single depth component. The GL converts it to a 24 bit value.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE</CODE></TD>
 * <TD>
 * Each element is a single luminance value. The GL converts it to fixed-point or floating-point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue and attaching 1 for alpha.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE_ALPHA</CODE></TD>
 * <TD>
 * Each element is a luminance/alpha pair. The GL converts it to fixed-point or floating point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGB</CODE></TD>
 * <TD>
 * Each element is an RGB triple. The GL converts it to fixed-point or floating-point and assembles it into an RGBA element by attaching 1 for alpha.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGBA</CODE></TD>
 * <TD>
 * Each element contains all four components. The GL converts it to fixed-point or floating-point.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALPHA_LUMINANCE_SCE</CODE></TD>
 * <TD>
 * Each element is an alpha/luminance pair.  The GL converts it to fixed-point or floating point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue
 * </TD>
 * </TR>
 * </TABLE>
 *
 * All of the internal formats are not directly supported by the hardware and are converted as detailed in the
 * following table:
<TABLE>
<TR>
<TD>Internal Format</TD>
<TD>Converted To</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA</CODE>, <CODE>GL_ALPHA4</CODE></TD>
<TD NOWRAP><CODE>GL_ALPHA8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE</CODE>, <CODE>GL_LUMINANCE4</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_INTENSITY</CODE>, <CODE>GL_INTENSITY4</CODE></TD>
<TD NOWRAP><CODE>GL_INTENSITY8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA_LUMINANCE_SCE</CODE>, <CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_LUMINANCE4_ALPHA4</CODE>, <CODE>GL_LUMINANCE6_ALPHA2</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE8_ALPHA8</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_R3_G3_B2</CODE>, <CODE>GL_RGB4</CODE>,
<CODE>GL_RGB8</CODE>, <CODE>GL_RGB</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_RGBA2</CODE>, <CODE>GL_RGBA4</CODE></TD>
<TD NOWRAP><CODE>GL_RGBA8</CODE> (4 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA12</CODE></TD>
<TD NOWRAP><CODE>GL_ALPHA16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_INTENSITY12</CODE></TD>
<TD NOWRAP><CODE>GL_INTENSITY16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE12</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE12_ALPHA4</CODE>, <CODE>GL_LUMINANCE12_ALPHA12</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE16_ALPHA16</CODE> (4 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA16F_ARB</CODE>, <CODE>GL_LUMINANCE16F_ARB</CODE>,
<CODE>GL_RGB16F_ARB</CODE></TD>
<TD NOWRAP><CODE>GL_RGBA16F_ARB</CODE> (8 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_RGB10</CODE>, <CODE>GL_RGLB10_A2</CODE>, <CODE>GL_RGB12</CODE>, <CODE>GL_RGB16</CODE>,
<CODE>GL_RGBA12</CODE>, <CODE>GL_RGBA16</CODE>, <CODE>GL_ALPHA32F_ARB</CODE>, <CODE>GL_LUMINANCE_ALPHA32F_ARB</CODE>,
<CODE>GL_RGB32F_ARB</CODE></TD>
<TD NOWRAP><CODE>GL_RGBA32F_ARB</CODE> (16 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_DEPTH_COMPONENT</CODE>, <CODE>GL_DEPTH_COMPONENT16</CODE>, <CODE>GL_DEPTH_COMPONENT24</CODE>,
<CODE>GL_DEPTH_COMPONENT32</CODE></TD>
<TD NOWRAP><CODE>GL_DEPTH_COMPONENT24</CODE> internal depth24/x8 format (4 bytes per pixel)</TD>
</TR>
</TABLE>
 *
 *  @param target Specifies the target texture. Must be one of:
<UL>
<LI><CODE>GL_TEXTURE_2D</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE></LI>
</UL>
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  Must be greater or equal 0
 *  @param internalFormat  Specifies the color components in the texture.
 *  The following symbolic values are accepted:
<UL>
<LI><CODE>GL_ALPHA</CODE>,
    <CODE> GL_ALPHA4</CODE>,
    <CODE> GL_ALPHA8</CODE></LI>
<LI><CODE>GL_ALPHA12</CODE>, 
    <CODE> GL_ALPHA16</CODE></LI>
<LI><CODE>GL_INTENSITY</CODE>,
    <CODE> GL_INTENSITY4</CODE></LI>
<LI><CODE>GL_INTENSITY8</CODE>, 
    <CODE> GL_INTENSITY12</CODE></LI>
<LI><CODE>GL_INTENSITY16</CODE></LI>
<LI><CODE>GL_LUMINANCE</CODE>, 
    <CODE> GL_LUMINANCE4</CODE></LI>
<LI><CODE>GL_LUMINANCE8</CODE>,
    <CODE> GL_LUMINANCE12</CODE></LI>
<LI><CODE>GL_LUMINANCE16</CODE></LI>
<LI><CODE>GL_ALPHA_LUMINANCE_SCE</CODE></LI>
<LI><CODE>GL_LUMINANCE_ALPHA</CODE>, 
    <CODE> GL_LUMINANCE8_ALPHA8</CODE></LI>
<LI><CODE>GL_LUMINANCE4_ALPHA4</CODE>,
    <CODE> GL_LUMINANCE6_ALPHA2</CODE></LI>
<LI><CODE>GL_LUMINANCE12_ALPHA4</CODE>,
    <CODE> GL_LUMINANCE12_ALPHA12</CODE></LI>
<LI><CODE>GL_LUMINANCE16_ALPHA16</CODE></LI>
<LI><CODE>GL_DEPTH_COMPONENT</CODE>, 
    <CODE> GL_DEPTH_COMPONENT16</CODE></LI>
<LI><CODE>GL_DEPTH_COMPONENT24</CODE>,
    <CODE> GL_DEPTH_COMPONENT32</CODE></LI>
<LI><CODE>GL_R3_G3_B2</CODE></LI>
<LI><CODE>GL_RGB</CODE>, 
    <CODE> GL_RGB4</CODE>,
    <CODE> GL_RGB5</CODE>, 
    <CODE> GL_RGB8</CODE></LI>
<LI><CODE>GL_RGB10</CODE>,
    <CODE> GL_RGB10_A2</CODE>,
    <CODE> GL_RGB12</CODE>,
    <CODE> GL_RGB16</CODE></LI>
<LI><CODE>GL_RGBA</CODE>,
    <CODE> GL_RGBA2</CODE>,
    <CODE> GL_RGBA4</CODE>,
    <CODE> GL_RGB5_A1</CODE></LI>
<LI><CODE>GL_RGBA8</CODE>,
    <CODE> GL_RGBA12</CODE>,
    <CODE> GL_RGBA16</CODE></LI>
<LI><CODE>GL_ALPHA16F_ARB</CODE>,
    <CODE> GL_ALPHA32F_ARB</CODE></LI>
<LI><CODE>GL_RGB16F_ARB</CODE>,
    <CODE> GL_RGBA16F_ARB</CODE></LI>
<LI><CODE>GL_RGB32F_ARB</CODE>,
    <CODE> GL_RGBA32F_ARB</CODE></LI>
<LI><CODE>GL_LUMINANCE16F_ARB</CODE></LI>
<LI><CODE>GL_LUMINANCE_ALPHA_16F_ARB</CODE></LI>
<LI><CODE>GL_LUMINANCE32F_ARB</CODE></LI>
<LI><CODE>GL_LUMINANCE_ALPHA32F_ARB</CODE></LI>
<LI><CODE>GL_ARGB_SCE</CODE></LI>
<LI><CODE>GL_BGRA</CODE></LI>
<LI><CODE>GL_COMPRESSED_RGB_S3TC_DXT1_EXT</CODE></LI>
<LI><CODE>GL_COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE></LI>
<LI><CODE>GL_COMPRESSED_RGBA_S3TC_DXT3_EXT</CODE></LI>
<LI><CODE>GL_COMPRESSED_RGBA_S3TC_DXT5_EXT</CODE></LI>
</UL>
 *  @param width  Specifies the width of the texture image.
 *  All implementations support texture images that are at least 64 texels wide.
 *  @param height Specifies the height of the texture image.
 *  All implementations support texture images that are at least 64 texels high.
 *  @param border Specifies the width of the border. Must be 0.
 *  @param format Specifies the format of the pixel data.
 *  The following symbolic values are accepted:
<UL>
<LI><CODE>GL_RGB</CODE></LI>
<LI><CODE>GL_BGR</CODE></LI>
<LI><CODE>GL_RGBA</CODE></LI>
<LI><CODE>GL_BGRA</CODE></LI>
<LI><CODE>GL_ABGR</CODE></LI>
<LI><CODE>GL_ARGB_SCE</CODE></LI>
<LI><CODE>GL_RED</CODE></LI>
<LI><CODE>GL_GREEN</CODE>,
<LI><CODE>GL_BLUE</CODE></LI>
<LI><CODE>GL_ALPHA</CODE></LI>
<LI><CODE>GL_LUMINANCE</CODE></LI>
<LI><CODE>GL_LUMINANCE_ALPHA</CODE></LI>
<LI><CODE>GL_ALPHA_LUMINANCE_SCE</CODE></LI>
<LI><CODE>GL_DEPTH_COMPONENT</CODE></LI>
</UL>
 *
 *  @param type   Specifies the data type of the pixel data. The following symbolic values are accepted:
<UL>
<LI><CODE>GL_BYTE</CODE></LI>
<LI><CODE>GL_UNSIGNED_BYTE</CODE></LI>
<LI><CODE>GL_SHORT</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT</CODE></LI>
<LI><CODE>GL_INT</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT</CODE></LI>
<LI><CODE>GL_HALF_FLOAT_ARB</CODE></LI>
<LI><CODE>GL_FLOAT</CODE></LI>
<LI><CODE>GL_UNSIGNED_BYTE_3_3_2</CODE></LI>
<LI><CODE>GL_UNSIGNED_BYTE_2_3_3_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_4_4_4_4_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_1_5_5_5_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_5_6_5</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_5_6_5_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_8_8_SCE</CODE></LI>
<LI><CODE>GL_UNSIGNED_SHORT_8_8_REV_SCE</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_8_8_8_8</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_10_10_10_2</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_2_10_10_10_REV</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_16_16_SCE</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_16_16_REV_SCE</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_24_8_SCE</CODE></LI>
<LI><CODE>GL_UNSIGNED_INT_8_24_REV_SCE</CODE></LI>
</UL>
 *
 *  @param pixels Usually specifies a pointer to the image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>pixels</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 * @note
PSGL does not upload the texture to RSX&trade; memory until a call to either glFlush() or a draw call.
If your application uploads many textures, be sure to call glFlush() to force
the upload and to avoid an out of memory condition.
@par
 * <I><c>pixels</c></I> may be <CODE>NULL</CODE>. In this case texture memory is allocated to accommodate a texture of width <I><c>width</c></I> and height <I><c>height</c></I>. You can then download subtextures to initialize this texture memory. The image is undefined if the user tries to apply an uninitialized portion of the texture image to a primitive.
@par
If there is a currently bound pixel buffer object, then <I><c>pixels</c></I> is not a direct pointer
to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server memory.
In this case, if <I><c>pixels</c></I> is <CODE>NULL</CODE>, then the start of the pixel buffer object is referenced.
@par
glTexImage2D() specifies the two-dimensional texture for the currently bound texture specified with glBindTexture(), and the current texture unit, specified with glActiveTexture().
@par
If the <I><CODE>internalFormat</CODE></I> is <CODE>GL_DEPTH_COMPONENT16</CODE>, stencil buffers are implicitly turned off.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of:
<UL>
<LI><CODE>GL_TEXTURE_2D</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE></LI>
<LI><CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE></LI>
</UL>
</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>format</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not a type constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is less than 0 or greater than
the value returned by glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_SIZE</CODE> divided by 2<sup>level</sup>. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>border</c></I> is not 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>type</c></I> and <I><c>format</c></I> are incompatible.</TD>
</TR>
</TABLE>

 *  @sa glActiveTexture, glBindTexture, glCopyTexImage2D, glCopyTexSubImage2D,
 *  glGetIntegerv, glMatrixMode, glPixelStorei, glTexEnvfv, glTexSubImage2D, glTexParameterf, glTextureReferenceSCE
 */

GLAPI void APIENTRY glTexImage2D( GLenum target, GLint level, GLint internalFormat,
                                  GLsizei width, GLsizei height, GLint border, GLenum format,
                                  GLenum type, const GLvoid *pixels )
{
    JS_APIENTRY_TIMER( glTexImage2D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM_V2
    _JS_DPM_API_ENTRY_9( glTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                         target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                         level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                         internalFormat, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                         width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                         height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                         border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                         format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                         type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                         pixels, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
#else
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer || ( _JS_DPM_STATE( CAPTURE ) && pixels == NULL ) )
        {
            _JS_DPM_API_ENTRY_9( glTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_9( glTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, _jsGetPixelSize( format, type )*width*height, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif
#endif


#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // not compliant with GL, but better than asserting.
    // this is a temporary fix
    if (( level < 0 ) || ( width <= 0 ) || ( height <= 0 ) ||
            ( width > ( _JS_MAX_TEXTURE_SIZE >> level ) ) ||
            ( height > ( _JS_MAX_TEXTURE_SIZE >> level ) ) ||
            ( border != 0 ) ||
            ( !_jsIsInternalFormat( internalFormat ) ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if (( !_jsIsFormat( format ) ) || ( !_jsIsType( type ) ) ||
            ( target == GL_TEXTURE_3D ) ) // other cases handled by _jsGetImage
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    // test compatible format and type
    //  This tests the number of components in a packed type matches the
    //  format.  Depth is handled separately because it may be in packed
    //  format with one of the components ignored.
    if ( format != GL_DEPTH_COMPONENT )
    {
        if (( !_jsIsValidPair( format, type ) ) )
        {
            _jsSetError( GL_INVALID_OPERATION );
            return;
        }
    }
    else if ( type != GL_UNSIGNED_SHORT &&
              type != GL_UNSIGNED_INT &&
              type != GL_UNSIGNED_INT_24_8_SCE )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, MAX( width, height ) ) )
    {
        return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // mark the image data invalid
    //  We do this here to make sure that any RSX&trade; side data (e.g. from PBO
    //  or copy texture) is not copied back for this image.
    image->dataState = _JS_IMAGE_DATASTATE_UNSET;

    // pixel buffer object
    //  If the unpack buffer is active, call down to the platform to implement
    //  any fast path data assignment.
    GLboolean directPBO = GL_FALSE;
    if ( LContext->PixelUnpackBuffer != 0 )
    {
        directPBO = _jsPlatformTexturePBOImage(
                        texture,
                        image,
                        level,
                        internalFormat,
                        width, height, 1,
                        format, type,
                        pixels );
    }

    if ( !directPBO )
    {
        // slow path
        //  If the platform has no PBO fast path, map the buffer and pass
        //  the mapped address (with offset).
        // TODO: performance warning?
        jsBufferObject* bufferObject = NULL;
        if ( LContext->PixelUnpackBuffer != 0 )
        {
            bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
            JS_ASSERT( bufferObject != NULL );
            pixels = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                     (( const GLubyte* )pixels - ( const GLubyte* )NULL );
        }

        _jsSetImage(
            image,
            internalFormat,
            width, height, 1,
            LContext->unpackAlignment,
            format, type,
            pixels );


        if ( LContext->PixelUnpackBuffer != 0 )
        {
            JS_ASSERT( bufferObject != NULL );
            _jsPlatformBufferObjectUnmap( bufferObject );
        }

        texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    }

    // mipmap flag
    //  Do not move this setting before the PBO handling.  The reason is
    //  that if fast path RSX&trade;-side mipmaps are not supported, a copyback
    //  from RSX&trade; memory may be required.  The copyback code checks the
    //  hasMipMaps flag to know what it needs to copy.  If the flag is set
    //  too early, the copyback code thinks it has to copy mipmaps that
    //  don't exist.
    if ( level > 0 )
        texture->hasMipmaps = GL_TRUE;

    // check if bound to active framebuffer
    _jsTextureTouchFBOs( texture );
    // non-default framebuffer

    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED | PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}

/**
 *  @short Specify a two-dimensional texture subimage
 *
 *  Texturing maps a portion of a specified texture image onto each graphical primitive for which
 *  texturing is enabled. To enable and disable two-dimensional texturing, call
 *  glEnable() and glDisable() with argument <CODE>GL_TEXTURE_2D</CODE> or <CODE>GL_TEXTURE_CUBE_MAP</CODE>. Two-dimensional texturing is initially disabled.
 *
 *  glTexSubImage2D() redefines a contiguous subregion of an existing two-dimensional texture image.
 *  The texels referenced by <I><c>pixels</c></I> replace the portion of the existing texture array with x indices
 *  <I><c>xoffset</c></I> and <I><c>xoffset</c></I> + <I><c>width</c></I> - 1, inclusive, and y indices
 *  <I><c>yoffset</c></I> and <I><c>yoffset</c></I> + <I><c>height</c></I> - 1, inclusive. This region may not include any texels
 *  outside the range of the texture array as it was originally specified. It is not an error to specify a
 *  subtexture with zero width or height, but such a specification has no effect.
 *
 *  @param target Specifies the target texture. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  @param xoffset  Specifies a texel offset in the x direction within the texture array.
 *  @param yoffset  Specifies a texel offset in the y direction within the texture array.
 *  @param width    Specifies the width of the texture subimage.
 *  @param height   Specifies the height of the texture subimage.
 *  @param format Specifies the format of the pixel data.
 *  The following symbolic values are accepted: <CODE>GL_RGB</CODE>, <CODE>GL_BGR</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_BGRA</CODE>,
<CODE>GL_ABGR</CODE>, <CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RED</CODE>, <CODE>GL_GREEN</CODE>,
<CODE>GL_BLUE</CODE>, <CODE>GL_ALPHA</CODE>, <CODE>GL_LUMINANCE</CODE>,
<CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_ALPHA_LUMINANCE_SCE</CODE>, and <CODE>GL_DEPTH_COMPONENT</CODE>.
 *  @param type   Specifies the data type of the pixel data. The following symbolic values are accepted:
<CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>,
<CODE>GL_SHORT</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>,
<CODE>GL_INT</CODE>, <CODE>GL_UNSIGNED_INT</CODE>,
<CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FLOAT</CODE>,
<CODE>GL_UNSIGNED_BYTE_3_3_2</CODE>, <CODE>GL_UNSIGNED_BYTE_2_3_3_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE>, <CODE>GL_UNSIGNED_SHORT_4_4_4_4_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE>, <CODE>GL_UNSIGNED_SHORT_1_5_5_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_6_5</CODE>, <CODE>GL_UNSIGNED_SHORT_5_6_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_8_8_SCE</CODE>, <CODE>GL_UNSIGNED_SHORT_8_8_REV_SCE</CODE>,
<CODE>GL_UNSIGNED_INT_8_8_8_8</CODE>, <CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE>,
<CODE>GL_UNSIGNED_INT_10_10_10_2</CODE>, <CODE>GL_UNSIGNED_INT_2_10_10_10_REV</CODE>,
<CODE>GL_UNSIGNED_INT_16_16_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_16_16_REV_SCE</CODE>.
<CODE>GL_UNSIGNED_INT_24_8_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_8_24_REV_SCE</CODE>.
 *  @param pixels Usually specifies a pointer to the image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>pixels</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 * @note
 * glPixelStorei() affects texture images in exactly the way it affects glTexImage2D().
 * @par
If there is a currently bound pixel buffer object, then <I><c>pixels</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>pixels</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 * @par
 * glTexSubImage2D() specifies a two-dimensional sub texture for the currently bound texture,
 * specified with glBindTexture() and current texture unit, specified with glActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a
 previous glTexImage2D() or glCopyTexImage2D() operation.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c> < - b, <c>xoffset</c> + <c>width</c> > (w - b),
<c>yoffset</c> < - b,</I> or <I><c>yoffset</c> + <c>height</c> > (h - b)</I> , where <I>w</I> is the texture width, <I>h</I> is the texture height,
 and <I>b</I> is the border of the texture image being modified.
 Note that <I>w</I> and <I>h</I> include twice the border width.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>format</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not a type constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>type</c></I> and <I><c>format</c></I> are incompatible.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCopyTexImage2D, glCopyTexSubImage2D,
 *  glGetIntegerv, glMatrixMode, glPixelStorei, glTexEnvfv, glTexParameterf
 */

GLAPI void APIENTRY glTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset,
                                     GLsizei width, GLsizei height, GLenum format, GLenum type,
                                     const GLvoid* pixels )
{
    JS_APIENTRY_TIMER( glTexSubImage2D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_9( glTexSubImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_9( glTexSubImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 pixels, _jsGetPixelSize( format, type )*width*height, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if (( level < 0 ) || ( width < 0 ) || ( height < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if (( !_jsIsFormat( format ) ) || ( !_jsIsType( type ) ) ||
            ( target == GL_TEXTURE_3D ) ) // other cases handled by _jsGetImage
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if (( !_jsIsValidPair( format, type ) ) || ( LContext->PixelUnpackBuffer == 0 && pixels == NULL ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        return;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( !image->isSet )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // pixel buffer object
    //  If the unpack buffer is active, call down to the platform to implement
    //  any fast path data assignment.
    GLboolean directPBO = GL_FALSE;
    if ( LContext->PixelUnpackBuffer != 0 )
    {
        directPBO = _jsPlatformTexturePBOSubImage(
                        texture,
                        image,
                        level,
                        xoffset, yoffset, 0,
                        width, height, 1,
                        format, type,
                        pixels );
    }

    if ( !directPBO )
    {
        // slow path
        //  If the platform has no PBO fast path, map the buffer and pass
        //  the mapped address (with offset).
        // TODO: performance warning?
        jsBufferObject* bufferObject = NULL;
        if ( LContext->PixelUnpackBuffer != 0 )
        {
            bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
            JS_ASSERT( bufferObject != NULL );
            pixels = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                     (( const GLubyte* )pixels - ( const GLubyte* )NULL );
        }

        _jsSetSubImage(
            target,
            level,
            texture,
            image,
            xoffset, yoffset, 0,
            width, height, 1,
            LContext->unpackAlignment,
            format, type,
            pixels );

        if ( LContext->PixelUnpackBuffer != 0 )
        {
            JS_ASSERT( bufferObject != NULL );
            _jsPlatformBufferObjectUnmap( bufferObject );
        }

        texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    }
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
 *  @short Specify a two-dimensional texture image with pixels from the color buffer
 *
glCopyTexImage2D() defines a two-dimensional texture image with pixels from the color buffer.
 *
The screen-aligned pixel rectangle with lower left corner at (<I><c>x</c>, <c>y</c></I>) and with a width of <I><c>width</c></I> + 2 * <I><c>border</c></I>
and a height of <I><c>height</c></I> + 2 &times; <I><c>border</c></I> defines the texture array at the mipmap level specified by <I><c>level</c></I>.
<I><c>internalFormat</c></I> specifies the color components of the texture.
 *
The red, green, blue, and alpha components of each pixel that is read are converted to an internal fixed-point or floating-point
format with unspecified precision. The conversion maps the largest representable component value to 1.0, and component value
0 to 0.0. The values are then converted to the texture&rsquo;s internal format for storage in the texel array.
 *
 * For best performance, the <I><c>internalFormat</c></I> of the texture
 * should be chosen to match the <I><c>internalFormat</c></I> of the
 * frame buffer attachment which serves as the source of the
 * texture. If the attachment and texture internal formats don&rsquo;t
 * match, performance may be significantly reduced by format
 * conversion.
 *
 * The device frame buffer color attachment has an implicit
 * <I><c>internalFormat</c></I> of <CODE>GL_ARGB_SCE</CODE>, and the depth
 * attachment has an implicit <I><c>internalFormat</c></I> of
 * <CODE>GL_DEPTH24_STENCIL8_SCE</CODE>.
 *
Pixel ordering is such that lower <I><c>x</c></I> and <I><c>y</c></I> screen coordinates correspond to lower <I>s</I> and <I>t</I> texture coordinates.
 *
If any of the pixels within the specified rectangle of the color buffer are outside the window associated with the current rendering context, then the values obtained for those pixels are undefined.
 *
 * When copying pixels from a multi-sampled framebuffer object, the samples are filtered and converted to pixels.
 *
 *  @param target Specifies the target texture. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  @param internalFormat  Specifies the color components requested for the texture.
 *  May be one of the following recommended symbolic constants:
 *  <CODE>GL_DEPTH_COMPONENT</CODE>, <CODE>GL_RGBA16F_ARB</CODE>, <CODE>GL_RGBA32F_ARB</CODE>, or <CODE>GL_ARGB_SCE</CODE>.
 *  May also be one of the following, but performance may suffer: <CODE>GL_ALPHA</CODE>, <CODE>GL_LUMINANCE</CODE>,
 *  <CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_RGB</CODE>, <CODE>GL_RGBA</CODE>,
<CODE>GL_BGR</CODE>, <CODE>GL_BGRA</CODE>, <CODE>GL_RED</CODE>, <CODE>GL_GREEN</CODE>, or <CODE>GL_BLUE</CODE>.

 *  @param x,y    Specify the window coordinates of the lower left corner of the rectangular region of pixels to be copied.

 *  @param width  Specifies the width of the texture image.
 *  @param height Specifies the height of the texture image.
 *  @param border Specifies the width of the border. Must be 0.
 *
 *  @note
 *  An image with height or width of 0 indicates a null-texture.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>internalFormat</c></I> is not compatible with the color buffer format.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is less than 0 or greater than
the value returned by calling glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_SIZE</CODE> divided by 2<sup>level</sup>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>border</c></I> is not 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
</TABLE>
 *
 *  @sa glCompressedTexImage2D, glCopyTexSubImage2D, glGetIntegerv,
 *  glTexEnvfv, glTexImage2D, glTexSubImage2D, glTexParameterf,
 *  glFramebufferParameteriSCE
 */
GLAPI void APIENTRY glCopyTexImage2D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
    JS_APIENTRY_TIMER( glCopyTexImage2D );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_8( glCopyTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 8,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    _JS_PERFORMANCE_START( CopyTexImageCounter );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if (( level < 0 ) || ( width < 0 ) || ( height < 0 ) || ( border != 0 ) ||
            ( width > ( _JS_MAX_TEXTURE_SIZE >> level ) ) ||
            ( height > ( _JS_MAX_TEXTURE_SIZE >> level ) ) ||
            ( !_jsIsInternalFormat( internalFormat ) )
       )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( target == GL_TEXTURE_3D )
    {
        // other cases handled by _jsGetImage
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, MAX( width, height ) ) )
    {
        // error set by _jsGetImage
        return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    if ( level > 0 ) texture->hasMipmaps = GL_TRUE;

    _jsSetImage( image, internalFormat, width, height, 1, LContext->unpackAlignment, 0, 0, NULL );
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_LAYOUT;
    if ( width && height )
    {
        _jsPlatformCopyTexSubImage3D( target, level, 0, 0, 0, x, y, width, height );
    }

    _jsTextureTouchFBOs( texture );
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED | PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
    _JS_PERFORMANCE_STOP( CopyTexImageCounter );
}

/**
@short Specify a two-dimensional texture subimage with pixels from the color buffer
 *
glCopyTexSubImage2D() replaces a rectangular portion of a two-dimensional texture image with pixels from the color buffer.
 *
The screen-aligned pixel rectangle with lower left corner at (<I><c>x</c>, <c>y</c></I>) and with width <I><c>width</c></I> and
height <I><c>height</c></I> replaces the portion of the texture array with x indices <I><c>xoffset</c></I> through
<I><c>xoffset</c></I> + <I><c>width</c></I> - 1, inclusive, and y indices <I><c>yoffset</c></I> through <I><c>yoffset</c></I> + <I><c>height</c></I> - 1,
inclusive, at the mipmap level specified by <I><c>level</c></I>.
 *
The pixels in the rectangle are processed the same way as with glCopyTexImage2D().
 *
glCopyTexSubImage2D() requires that the internal format of the currently bound texture is such that color buffer components
can be dropped during conversion to the internal format, but new components cannot be added. For example,
an <CODE>RGB</CODE> color buffer can be used to create <CODE>LUMINANCE</CODE> or <CODE>RGB</CODE> textures,
but not <CODE>ALPHA</CODE>, <CODE>LUMINANCE_ALPHA</CODE> or <CODE>RGBA</CODE> textures.
 *
The destination rectangle in the texture array may not include any texels outside the texture array as it was originally specified. It is not an error to specify a subtexture with zero width or height, but such a specification has no effect.
 *
If any of the pixels within the specified rectangle of the current color buffer are outside the read window associated
with the current rendering context, then the values obtained for those pixels are undefined.
 *
No change is made to the <I><c>internalFormat</c></I>, <I><c>width</c></I>, <I><c>height</c></I>, or <I><c>border</c></I> parameters of the specified texture
array or to texel values outside the specified subregion.
 *
 *  @param target Specifies the target texture. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
 *
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  @param xoffset Specifies a texel offset in the x direction within the texture array.
 *  @param yoffset Specifies a texel offset in the y direction within the texture array.
 *  @param x,y    Specify the window coordinates of the lower left corner of the rectangular region of pixels to be copied.
 *  @param width  Specifies the width of the texture subimage.
 *  @param height Specifies the height of the texture subimage.
 *
 *  @note
 *  glCopyTexSubImage2D() will try to do all copies in place if the following conditions are met:
 *  <ul>
 *  <li><i><c>x</c></i> and <i><c>y</c></i> are both 0.</li>
 *  <li><i><c>width</c></i> and <i><c>height</c></i> match the source and destination width and height.</li>
 *  <li>The destination and source are not flipped (or require flipping).
 *  </ul>
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a previous
 glTexImage2D() or glCopyTexImage2D() operation or if the internal format of the currently bound texture is not
 compatible with the color buffer format.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>x</c></I> < <I>- b</I>, or <I><c>y</c></I> < <I>- b</I>,
 where <I>b</I> is the border of the texture being modified.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c></I> < <I>- b</I>,
 <I><c>xoffset</c></I> + <I><c>width</c></I> > <I>(w - b)</I> , <I><c>yoffset</c></I> < <I>- b</I>, or <I><c>yoffset</c></I> + <I><c>height</c></I> > <I>(h - b)</I> ,
 where <I>w</I> is the texture width, <I>h</I> is the texture height, and <I>b</I> is the border of the texture
 image being modified. Note that <I>w</I> and <I>h</I> include twice the border width.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
</TABLE>
 *
 *  @sa glCompressedTexSubImage2D, glCopyTexImage2D, glGetIntegerv,
 *      glTexEnvfv, glTexImage2D, glTexParameterf, glTexSubImage2D
 */

GLAPI void APIENTRY glCopyTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
    JS_APIENTRY_TIMER( glCopyTexSubImage2D );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_8( glCopyTexSubImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 8,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( target == GL_TEXTURE_3D )
    {
        // other cases handled by _jsGetImage
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if (( level < 0 ) || ( width < 0 ) || ( height < 0 ) || ( x < 0 ) || ( y < 0 ) || ( xoffset < 0 ) || ( yoffset < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        return;
    }

#ifndef JS_NO_ERROR_CHECK
    if (( xoffset + width > image->width ) || ( yoffset + height > image->height ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( !image->isSet )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _jsPlatformCopyTexSubImage3D( target, level, xoffset, yoffset, 0, x, y, width, height );

    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
@short Specify a three-dimensional texture subimage with pixels from the color buffer

glCopyTexSubImage3D() replaces a rectangular portion of a three-dimensional texture image with pixels from the color buffer.
 *
The screen-aligned pixel rectangle with lower left corner at (<I><c>x</c>, <c>y</c></I>) and with width <I><c>width</c></I> and
height <I><c>height</c></I> replaces the portion of the texture array with x indices <I><c>xoffset</c></I> through
<I><c>xoffset</c></I> + <I><c>width</c></I> - 1, inclusive, and y indices <I><c>yoffset</c></I> through <I><c>yoffset</c></I> + <I><c>height</c></I> - 1,
inclusive, at z index <I><c>zoffset</c></I> and at the mipmap level specified by <I><c>level</c></I>.
 *
glCopyTexSubImage3D() requires that the internal format of the currently bound texture is such that color buffer components
can be dropped during conversion to the internal format, but new components cannot be added. For example,
an <CODE>RGB</CODE> color buffer can be used to create <CODE>LUMINANCE</CODE> or <CODE>RGB</CODE> textures,
but not <CODE>ALPHA</CODE>, <CODE>LUMINANCE_ALPHA</CODE> or <CODE>RGBA</CODE> textures.
 *
The destination rectangle in the texture array may not include any texels outside the texture array as it was originally specified. It is not an error to specify a subtexture with zero width or height, but such a specification has no effect.
 *
If any of the pixels within the specified rectangle of the current color buffer are outside the read window associated
with the current rendering context, then the values obtained for those pixels are undefined.
 *
No change is made to the <I><c>internalFormat</c></I>, <I><c>width</c></I>, <I><c>height</c></I>, <I><c>depth</c></I> or <I><c>border</c></I> parameters of the specified texture
array or to texel values outside the specified subregion.
 *
 *  @param target Specifies the target texture. Must be <CODE>GL_TEXTURE_3D</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  @param xoffset Specifies a texel offset in the x direction within the texture array.
 *  @param yoffset Specifies a texel offset in the y direction within the texture array.
 *  @param zoffset Specifies a texel offset in the z direction within the texture array.
 *  @param x,y    Specify the window coordinates of the lower left corner of the rectangular region of pixels to be copied.
 *  @param width  Specifies the width of the texture subimage.
 *  @param height Specifies the height of the texture subimage.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_TEXTURE_3D</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a previous
 glTexImage3D() operation or if the internal format of the currently bound texture is not
 compatible with the color buffer format.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c></I> < <I>- b</I>,
   <I><c>xoffset</c></I> + <I><c>width</c></I> > <I>(w - b)</I>, <I><c>yoffset</c></I> < <I>- b</I>,
   <I><c>yoffset</c></I> + <I><c>height</c></I> > <I>(h - b)</I>, <I><c>zoffset</c></I> < <I>- b</I>,
   <I><c>zoffset</c></I> > <I>(d - b)</I>,
   where <I>w</I> is the texture width, <I>h</I> is the texture height,
   <I>d</I> is the texture depth, and <I>b</I> is the border of the texture
   image being modified. Note that <I>w</I>, <I>h</I>, and <I>d</I> include twice the border width.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_FRAMEBUFFER_OPERATION_OES</CODE></TD>
<TD>One or more of the following conditions is not true:
<UL>
<LI>All textures are complete.</LI>
<LI>Color formats are consistent across the color textures</LI>
<LI>Swizzling hint is consistent across all the textures</LI>
<LI>Drawable color format for the color attachments if any</LI>
<LI>Drawable depth format for the depth attachment if any</LI>
<LI>No anti-aliasing if FP16 or FP32 color format</LI>
<LI>No swizzling with RGBA/FP16 or RGBA/FP32 (any format that is more than
32 bits per pixel)</LI>
<LI>Matching sizes, if swizzled</LI>
</UL></TD>
</TR>
</TABLE>
 *
 *  @sa glCompressedTexSubImage2D, glCopyTexImage2D, glCopyTexSubImage2D,
 *      glGetIntegerv, glTexEnvfv, glTexImage3D, glTexParameterf, glTexSubImage3D
 */

GLAPI void APIENTRY glCopyTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
    JS_APIENTRY_TIMER( glCopyTexSubImage3D );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_9( glCopyTexSubImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             zoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             x, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             y, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if ( target != GL_TEXTURE_3D )
    {
        // other cases handled by _jsGetImage
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if (( level < 0 ) || ( width < 0 ) || ( height < 0 ) ||
            ( x < 0 ) || ( y < 0 ) || ( xoffset < 0 ) || ( yoffset < 0 ) || ( zoffset < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        return;
    }

#ifndef JS_NO_ERROR_CHECK
    if (( xoffset + width > image->width ) || ( yoffset + height > image->height ) || ( zoffset > image->depth ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( !image->isSet )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _jsPlatformCopyTexSubImage3D( target, level, xoffset, yoffset, zoffset, x, y, width, height );

    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
 *  @short Specify a three-dimensional texture image
 *
 *  Texturing maps a portion of a specified texture image onto each graphical primitive for which
 *  texturing is enabled. To enable and disable three-dimensional texturing, call glEnable() and glDisable()
 *  with argument <CODE>GL_TEXTURE_3D</CODE>. Three-dimensional texturing is initially disabled.
 *
 *  To define texture images, call glTexImage3D(). The arguments describe the parameters of the texture image, such as height, width, depth, width of the border, and number of color components provided. The last two arguments describe how the image is represented in memory.
 *
Data is read from <I><c>pixels</c></I> as a sequence of unsigned bytes, shorts, long integers, or floats, depending on <I><c>type</c></I>.
These values are grouped into sets of one, two, three, or four values, depending on <I><c>format</c></I>, to form elements.
 *
When <I><c>type</c></I> is <CODE>GL_UNSIGNED_BYTE</CODE>, <CODE>GL_HALF_FLOAT_ARB</CODE>, or something similar, each byte/float/type is interpreted as one color component, depending on <I><c>format</c></I>. When <I>type</I> is one of <CODE>GL_UNSIGNED_SHORT_5_6_5</CODE>, <CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE>, <CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE>, or something similar, each unsigned value is interpreted as containing all the components for a single pixel, with the color components arranged according to format.
 *
The first element corresponds to the lower left corner of the texture image. Subsequent elements progress left-to-right through the remaining texels in the lowest row of the texture image, and then in successively higher rows of the texture image. The final element corresponds to the upper right corner of the texture image.
 *
By default, adjacent pixels are taken from adjacent memory locations, except that after all <I><c>width</c></I> pixels are read, the read pointer is advanced to the next four-byte boundary. The four-byte row alignment is specified by glPixelStorei() with argument <CODE>GL_UNPACK_ALIGNMENT</CODE>, and it can be set to one, two, four, or eight bytes.

 * Some combinations of <i><c>format</c></I> and <i><c>type</c></i> cause the
 * texture data to be converted into an internal format that is
 * slightly different from the one requested by <i><c>internalFormat</c></i>,
 * wasting computing resources in the conversion.  For your program to
 * run its fastest, you should specify compatible combinations of
 * values for <i><c>format</c></I>, <i><c>type</c></i>, and <i><c>internalFormat.</c></i>

 * This table lists the most efficient combinations of
 * <i><c>internalFormat</c></i> (either generic or specific), <i><c>format</c></I>,
 * and <i><c>type</c></i>:

 * <TABLE rules=all>
 * <TR>
 * <TD><i><c>internalFormat</c></i></TD>
 * <TD><i><c>format</c></i></TD>
 * <TD><i><c>type</c></i></TD>
 * </TR>
 * <TR><TD><code>GL_ALPHA16</code></TD><TD><code>GL_ALPHA</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_ALPHA8 </code> or <code> GL_ALPHA</code></TD><TD><code>GL_ALPHA</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_INTENSITY16</code></TD><TD><code>GL_INTENSITY</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_INTENSITY8 </code></TD><TD><code>GL_INTENSITY</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE16</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_UNSIGNED_SHORT</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE8 </code> or <code> GL_LUMINANCE</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_UNSIGNED_BYTE</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE16_ALPHA16</code></TD><TD><code>GL_LUMINANCE_ALPHA</code></TD><TD><code>GL_UNSIGNED_INT_16_16_REV</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE8_ALPHA8 </code> or <code> GL_LUMINANCE_ALPHA</code></TD><TD><code>GL_LUMINANCE_ALPHA</code></TD><TD><code>GL_UNSIGNED_SHORT_8_8_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB8</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8</code></TD></TR>
 * <TR><TD><code>GL_RGBA8 </code> or <code> GL_RGBA8</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8</code></TD></TR>
 * <TR><TD><code>GL_ARGB_SCE</code></TD><TD><code>GL_BGRA</code></TD><TD><code>GL_UNSIGNED_INT_8_8_8_8_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB5_A1</code></TD><TD><code>GL_BGRA</code></TD><TD><code>GL_UNSIGNED_SHORT_1_5_5_5_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB5</code></TD><TD><code>GL_BGR</code></TD><TD><code>GL_UNSIGNED_SHORT_5_6_5_REV</code></TD></TR>
 * <TR><TD><code>GL_RGB16F</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_HALF_FLOAT_ARB</code></TD></TR>
 * <TR><TD><code>GL_RGBA16F</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_HALF_FLOAT_ARB</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE_ALPHA16F_ARB</code></TD><TD><code>GL_ALPHA_LUMINANCE_SCE</code></TD><TD><code>GL_HALF_FLOAT_ARB</code></TD></TR>
 * <TR><TD><code>GL_LUMINANCE32F_ARB</code></TD><TD><code>GL_LUMINANCE</code></TD><TD><code>GL_FLOAT</code></TD></TR>
 * <TR><TD><code>GL_RGB32F_ARB</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_FLOAT</code></TD></TR>
 * <TR><TD><code>GL_RGBA32F_ARB</code></TD><TD><code>GL_RGBA</code></TD><TD><code>GL_FLOAT</code></TD></TR>
 * <TR><TD><code>GL_DEPTH_COMPONENT24 </code> or <code> GL_DEPTH_COMPONENT</code></TD><TD><code>GL_DEPTH_COMPONENT</code></TD><TD><code>GL_UNSIGNED_INT_24_8_SCE</code></TD></TR>
 * </TABLE>

 * The parameter <I><c>format</c></I> determines the composition of each
 * element in <I><c>pixels</c></I>. It can assume one of the following
 * symbolic values:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_ALPHA</CODE></TD>
 * <TD>
 * Each element is a single alpha component. The GL converts it to floating point and assembles it into an RGBA element by attaching 0 for red, green, and blue.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ARGB_SCE</CODE></TD>
 * <TD>
 * Each element contains all four components. The GL converts it to four 8-byte values in the order specified.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_DEPTH_COMPONENT</CODE></TD>
 * <TD>
 * Each element is a single depth component. The GL converts it to a 24 bit value.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE</CODE></TD>
 * <TD>
 * Each element is a single luminance value. The GL converts it to fixed-point or floating-point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue and attaching 1 for alpha.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LUMINANCE_ALPHA</CODE></TD>
 * <TD>
 * Each element is a luminance/alpha pair. The GL converts it to fixed-point or floating point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGB</CODE></TD>
 * <TD>
 * Each element is an RGB triple. The GL converts it to fixed-point or floating-point and assembles it into an RGBA element by attaching 1 for alpha.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_RGBA</CODE></TD>
 * <TD>
 * Each element contains all four components. The GL converts it to fixed-point or floating-point.
 * </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_ALPHA_LUMINANCE_SCE</CODE></TD>
 * <TD>
 * Each element is an alpha/luminance pair.  The GL converts it to fixed-point or floating point, then assembles it into an RGBA element by replicating the luminance value three times for red, green, and blue
 * </TD>
 * </TR>
 * </TABLE>
 *
 * All of the internal formats are not directly supported by the hardware and are converted as detailed in the
 * following table:
<TABLE>
<TR>
<TD>Internal Format</TD>
<TD>Converted To</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA</CODE>, <CODE>GL_ALPHA4</CODE></TD>
<TD NOWRAP><CODE>GL_ALPHA8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE</CODE>, <CODE>GL_LUMINANCE4</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_INTENSITY</CODE>, <CODE>GL_INTENSITY4</CODE></TD>
<TD NOWRAP><CODE>GL_INTENSITY8</CODE> (1 byte per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_LUMINANCE4_ALPHA4</CODE>, <CODE>GL_LUMINANCE6_ALPHA2</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE8_ALPHA8</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_R3_G3_B2</CODE>, <CODE>GL_RGB4</CODE>,
<CODE>GL_RGB8</CODE>, <CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_RGBA2</CODE>, <CODE>GL_RGBA4</CODE>,
</TD>
<TD NOWRAP><CODE>GL_RGBA8</CODE> (4 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA12</CODE></TD>
<TD NOWRAP><CODE>GL_ALPHA16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_INTENSITY12</CODE></TD>
<TD NOWRAP><CODE>GL_INTENSITY16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE12</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE16</CODE> (2 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_LUMINANCE12_ALPHA4</CODE>, <CODE>GL_LUMINANCE12_ALPHA12</CODE></TD>
<TD NOWRAP><CODE>GL_LUMINANCE16_ALPHA16</CODE> (4 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_ALPHA16F_ARB</CODE>, <CODE>GL_LUMINANCE16F_ARB</CODE>,
<CODE>GL_RGB16F_ARB</CODE></TD>
<TD NOWRAP><CODE>GL_RGBA16F_ARB</CODE> (8 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_RGB10</CODE>, <CODE>GL_RGB10_A2</CODE>, <CODE>GL_RGB12</CODE>, <CODE>GL_RGB16</CODE>,
<CODE>GL_RGBA12</CODE>, <CODE>GL_RGBA16</CODE>, <CODE>GL_ALPHA32F_ARB</CODE>, <CODE>GL_LUMINANCE_ALPHA32F_ARB</CODE>,
<CODE>GL_RGB32F_ARB</CODE></TD>
<TD NOWRAP><CODE>GL_RGBA32F_ARG</CODE> (16 bytes per pixel)</TD>
</TR>
<TR>
<TD WRAP><CODE>GL_DEPTH_COMPONENT</CODE>, <CODE>GL_DEPTH_COMPONENT16</CODE>, <CODE>GL_DEPTH_COMPONENT24</CODE>,
<CODE>GL_DEPTH_COMPONENT32</CODE></TD>
<TD NOWRAP>internal depth24/x8 format (4 bytes per pixel)</TD>
</TR>
</TABLE>

 *
 *  @param target Specifies the target texture. Must be <CODE>GL_TEXTURE_3D</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  Must be greater or equal 0
 *  @param internalFormat  Specifies the color components requested for the texture.
zzz *  The following symbolic values are accepted:
<CODE>GL_RGBA</CODE>, <CODE>GL_RGBA8</CODE>, <CODE>GL_RGBA16</CODE>,
<CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RGBA16F_ARB</CODE>, <CODE>GL_RGBA32F_ARB</CODE>, <CODE>GL_RGB</CODE>,
<CODE>GL_RGB8</CODE>, <CODE>GL_RGB16</CODE>, <CODE>GL_RGB16F_ARB</CODE>,
<CODE>GL_RGB32F_ARB</CODE>, <CODE>GL_ALPHA</CODE>, <CODE>GL_ALPHA8</CODE>, <CODE>GL_ALPHA16</CODE>,
<CODE>GL_INTENSITY</CODE>, <CODE>GL_INTENSITY8</CODE>, <CODE>GL_LUMINANCE</CODE>, <CODE>GL_LUMINANCE8</CODE>,
<CODE>GL_INTENSITY16</CODE>, <CODE>GL_LUMINANCE16</CODE>, <CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_LUMINANCE8_ALPHA8</CODE>,
<CODE>GL_LUMINANCE16_ALPHA16</CODE>, <CODE>GL_DEPTH_COMPONENT</CODE>, <CODE>GL_DEPTH_COMPONENT16</CODE>, and <CODE>GL_DEPTH_COMPONENT24</CODE>.
 *  @param width  Specifies the width of the texture image.
 *  All implementations support texture images that are at least 64 texels wide.
 *  @param height Specifies the height of the texture image.
 *  All implementations support texture images that are at least 64 texels high.
 *  @param depth  Specifies the depth of the texture image.
 *  All implementations support texture images that are at least 64 texels deep.
 *  @param border Specifies the width of the border. Must be 0.
 *  @param format Specifies the format of the pixel data.
 *  The following symbolic values are accepted: <CODE>GL_RGB</CODE>, <CODE>GL_BGR</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_BGRA</CODE>,
<CODE>GL_ABGR</CODE>, <CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RED</CODE>, <CODE>GL_GREEN</CODE>,
<CODE>GL_BLUE</CODE>, <CODE>GL_ALPHA</CODE>, <CODE>GL_LUMINANCE</CODE>,
<CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_ALPHA_LUMINANCE_SCE</CODE>, and <CODE>GL_DEPTH_COMPONENT</CODE>.
 *  @param type   Specifies the data type of the pixel data. The following symbolic values are accepted:
<CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>,
<CODE>GL_SHORT</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>,
<CODE>GL_INT</CODE>, <CODE>GL_UNSIGNED_INT</CODE>,
<CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FLOAT</CODE>,
<CODE>GL_UNSIGNED_BYTE_3_3_2</CODE>, <CODE>GL_UNSIGNED_BYTE_2_3_3_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE>, <CODE>GL_UNSIGNED_SHORT_4_4_4_4_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE>, <CODE>GL_UNSIGNED_SHORT_1_5_5_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_6_5</CODE>, <CODE>GL_UNSIGNED_SHORT_5_6_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_8_8_SCE</CODE>, <CODE>GL_UNSIGNED_SHORT_8_8_REV_SCE</CODE>,
<CODE>GL_UNSIGNED_INT_8_8_8_8</CODE>, <CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE>,
<CODE>GL_UNSIGNED_INT_10_10_10_2</CODE>, <CODE>GL_UNSIGNED_INT_2_10_10_10_REV</CODE>,
<CODE>GL_UNSIGNED_INT_16_16_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_16_16_REV_SCE</CODE>.
<CODE>GL_UNSIGNED_INT_24_8_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_8_24_REV_SCE</CODE>.
 *  @param pixels Usually specifies a pointer to the image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>pixels</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 * @note
 * <I><c>pixels</c></I> may be <CODE>NULL</CODE>. In this case texture memory is allocated to accommodate a texture of
width <I><c>width</c></I>, height <I><c>height</c></I>, and depth <I><c>depth</c></I>. You can then download subtextures to initialize this texture memory. The image is undefined if the user tries to apply an uninitialized portion of the texture image to a primitive.
 * @par
If there is a currently bound pixel buffer object, then <I><c>pixels</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>pixels</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 * @par
glTexImage3D() specifies the three-dimensional texture for the currently bound texture specified with glBindTexture(), and the current texture unit, specified with glActiveTexture().
 * @par
If the <I><CODE>internalFormat</CODE></I> is <CODE>GL_DEPTH_COMPONENT16</CODE>, stencil buffers are implicitly turned off.

 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_TEXTURE_3D</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>format</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not a type constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I>, <I><c>height</c></I> or <I><c>depth</c></I> is less than 0 or greater than <CODE>GL_MAX_3D_TEXTURE_SIZE</CODE> divided by 2<sup>level</sup>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>border</c></I> is not 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>type</c></I> and <I><c>format</c></I> are incompatible.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCopyTexSubImage3D,
 *  glGetIntegerv, glMatrixMode, glPixelStorei, glTexEnvfv, glTexSubImage3D, glTexParameterf, glTextureReferenceSCE
 */

GLAPI void APIENTRY glTexImage3D( GLenum target, GLint level, GLint internalFormat,
                                  GLsizei width, GLsizei height, GLsizei depth, GLint border,
                                  GLenum format, GLenum type, const GLvoid *pixels )
{
    JS_APIENTRY_TIMER( glTexImage3D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer || ( _JS_DPM_STATE( CAPTURE ) && pixels == NULL ) )
        {
            _JS_DPM_API_ENTRY_10( glTexImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 10,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  internalFormat, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  pixels, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_10( glTexImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 10,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  internalFormat, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  pixels, _jsGetPixelSize( format, type )*width*height*depth, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // not compliant with GL, but better than asserting.
    // this is a temporary fix
    if (( level < 0 ) || ( width <= 0 ) || ( height <= 0 ) || ( depth <= 0 ) || ( border != 0 ) ||
            ( width > ( _JS_MAX_TEXTURE_SIZE_3D >> level ) ) ||
            ( height > ( _JS_MAX_TEXTURE_SIZE_3D >> level ) ) ||
            ( depth > ( _JS_MAX_TEXTURE_SIZE_3D >> level ) ) ||
            ( !_jsIsInternalFormat( internalFormat ) ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if (( !_jsIsFormat( format ) ) || ( !_jsIsType( type ) ) || ( target != GL_TEXTURE_3D ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if (( !_jsIsValidPair( format, type ) ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, MAX( MAX( width, height ), depth ) ) )
    {
        return;
    }
#ifndef JS_NO_ERROR_CHECK
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // mark the image data invalid
    //  We do this here to make sure that any RSX&trade; side data (e.g. from PBO
    //  or copy texture) is not copied back for this image.
    image->dataState = _JS_IMAGE_DATASTATE_UNSET;

    // pixel buffer object
    //  If the unpack buffer is active, call down to the platform to implement
    //  any fast path data assignment.

    GLboolean directPBO = GL_FALSE;
    if ( LContext->PixelUnpackBuffer != 0 )
    {
        directPBO = _jsPlatformTexturePBOImage(
                        texture,
                        image,
                        level,
                        internalFormat,
                        width, height, depth,
                        format, type,
                        pixels );
    }
    if ( !directPBO )
    {
        // slow path
        //  If the platform has no PBO fast path, map the buffer and pass
        //  the mapped address (with offset).
        // TODO: performance warning?
        jsBufferObject* bufferObject = NULL;
        if ( LContext->PixelUnpackBuffer != 0 )
        {
            bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
            JS_ASSERT( bufferObject != NULL );
            pixels = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                     (( const GLubyte* )pixels - ( const GLubyte* )NULL );
        }

        _jsSetImage(
            image,
            internalFormat,
            width, height, depth,
            LContext->unpackAlignment,
            format, type,
            pixels );

        if ( LContext->PixelUnpackBuffer != 0 )
        {
            JS_ASSERT( bufferObject != NULL );
            _jsPlatformBufferObjectUnmap( bufferObject );
        }

        texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    }

    // mipmap flag
    //  Do not move this setting before the PBO handling.  The reason is
    //  that if fast path RSX&trade;-side mipmaps are not supported, a copyback
    //  from RSX&trade; memory may be required.  The copyback code checks the
    //  hasMipMaps flag to know what it needs to copy.  If the flag is set
    //  too early, the copyback code thinks it has to copy mipmaps that
    //  don't exist.
    if ( level > 0 )
        texture->hasMipmaps = GL_TRUE;

    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_FFX_FRAGMENT_PROGRAM;
}

/**
 *  @short Specify a three-dimensional texture subimage
 *
 *  Texturing maps a portion of a specified texture image onto each graphical primitive for which
 *  texturing is enabled. To enable and disable two-dimensional texturing, call
 *  glEnable() and glDisable() with argument <CODE>GL_TEXTURE_3D</CODE>. Two-dimensional texturing is initially disabled.
 *
 *  glTexSubImage3D() redefines a contiguous subregion of an existing two-dimensional texture image.
 *  The texels referenced by <I><c>pixels</c></I> replace the portion of the existing texture array with x indices
 *  <I><c>xoffset</c></I> and <I><c>xoffset</c></I> + <I><c>width</c></I> - 1, inclusive, y indices
 *  <I><c>yoffset</c></I> and <I><c>yoffset</c></I> + <I><c>height</c></I> - 1, inclusive, and
 *  z indices <I><c>zoffset</c></I> and <I><c>zoffset</c></I> + <I><c>depth</c></I> - 1, inclusive. This region may not include any texels
 *  outside the range of the texture array as it was originally specified. It is not an error to specify a
 *  subtexture with zero width, height, or depth, but such a specification has no effect.
 *
 *  @param target Specifies the target texture. Must be <CODE>GL_TEXTURE_3D</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  @param xoffset  Specifies a texel offset in the x direction within the texture array.
 *  @param yoffset  Specifies a texel offset in the y direction within the texture array.
 *  @param zoffset  Specifies a texel offset in the z direction within the texture array.
 *  @param width    Specifies the width of the texture subimage.
 *  @param height   Specifies the height of the texture subimage.
 *  @param depth    Specifies the depth of the texture subimage.
 *  @param format Specifies the format of the pixel data.
 *  The following symbolic values are accepted: <CODE>GL_RGB</CODE>, <CODE>GL_BGR</CODE>, <CODE>GL_RGBA</CODE>, <CODE>GL_BGRA</CODE>,
<CODE>GL_ABGR</CODE>, <CODE>GL_ARGB_SCE</CODE>, <CODE>GL_RED</CODE>, <CODE>GL_GREEN</CODE>,
<CODE>GL_BLUE</CODE>, <CODE>GL_ALPHA</CODE>, <CODE>GL_LUMINANCE</CODE>,
<CODE>GL_LUMINANCE_ALPHA</CODE>, <CODE>GL_ALPHA_LUMINANCE_SCE</CODE>, and <CODE>GL_DEPTH_COMPONENT</CODE>.
 *  @param type   Specifies the data type of the pixel data. The following symbolic values are accepted:
<CODE>GL_BYTE</CODE>, <CODE>GL_UNSIGNED_BYTE</CODE>,
<CODE>GL_SHORT</CODE>, <CODE>GL_UNSIGNED_SHORT</CODE>,
<CODE>GL_INT</CODE>, <CODE>GL_UNSIGNED_INT</CODE>,
<CODE>GL_HALF_FLOAT_ARB</CODE>, <CODE>GL_FLOAT</CODE>,
<CODE>GL_UNSIGNED_BYTE_3_3_2</CODE>, <CODE>GL_UNSIGNED_BYTE_2_3_3_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_4_4_4_4</CODE>, <CODE>GL_UNSIGNED_SHORT_4_4_4_4_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_5_5_1</CODE>, <CODE>GL_UNSIGNED_SHORT_1_5_5_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_5_6_5</CODE>, <CODE>GL_UNSIGNED_SHORT_5_6_5_REV</CODE>,
<CODE>GL_UNSIGNED_SHORT_8_8_SCE</CODE>, <CODE>GL_UNSIGNED_SHORT_8_8_REV_SCE</CODE>,
<CODE>GL_UNSIGNED_INT_8_8_8_8</CODE>, <CODE>GL_UNSIGNED_INT_8_8_8_8_REV</CODE>,
<CODE>GL_UNSIGNED_INT_10_10_10_2</CODE>, <CODE>GL_UNSIGNED_INT_2_10_10_10_REV</CODE>,
<CODE>GL_UNSIGNED_INT_16_16_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_16_16_REV_SCE</CODE>.
<CODE>GL_UNSIGNED_INT_24_8_SCE</CODE>, and <CODE>GL_UNSIGNED_INT_8_24_REV_SCE</CODE>.
 *  @param pixels Usually specifies a pointer to the image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>pixels</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 * @note
 * glPixelStorei() affects texture images in exactly the way it affects glTexImage3D().
 * @par
If there is a currently bound pixel buffer object, then <I>pixels</I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>pixels</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 * @par
 * glTexSubImage3D() specifies a three-dimensional sub texture for the currently bound texture,
 * specified with glBindTexture() and current texture unit, specified with glActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_TEXTURE_3D</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a
 previous glTexImage3D() operation.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c> < - b, <c>xoffset</c> + <c>width</c> > (w - b),
<c>yoffset</c> < - b, <c>yoffset</c> + <c>height</c> > (h - b), <c>zoffset</c> < - b,</I> or <I><c>zoffset</c> + <c>depth</c> > (d - b)</I>
where <I>w</I> is the texture width, <I>h</I> is the texture height, <I>d</I> is the texture depth,
and <I>b</I> is the border of the texture image being modified.
Note that <I>w</I> and <I>h</I> include twice the border width.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I>, <I><c>height</c></I>, or <I><c>depth</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>format</c></I> is not an accepted constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>type</c></I> is not a type constant.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD><I><c>type</c></I> and <I>format</I> are incompatible.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCopyTexSubImage3D, glGetIntegerv, glPixelStorei,
 *  glTexEnvfv, glTexImage3D, glTexSubImage2D, glTexParameterf
 */

GLAPI void APIENTRY glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
                                     GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type,
                                     const GLvoid* pixels )
{
    JS_APIENTRY_TIMER( glTexSubImage3D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_11( glTexSubImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 11,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  zoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  pixels, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_11( glTexSubImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 11,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  zoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  type, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  pixels, _jsGetPixelSize( format, type )*width*height*depth, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    jsTextureImageUnit *unit = LContext->CurrentImageUnit;
    if ( !unit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if (( level < 0 ) || ( width < 0 ) || ( height < 0 ) || ( depth < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if (( !_jsIsFormat( format ) ) || ( !_jsIsType( type ) ) || ( target != GL_TEXTURE_3D ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }

    if (( !_jsIsValidPair( format, type ) ) || ( LContext->PixelUnpackBuffer == 0 && pixels == NULL ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    jsTexture *texture;
    jsImage *image;
    if ( _jsGetImage( target, level, &texture, &image, 0 ) )
    {
        return;
    }

#ifndef JS_NO_ERROR_CHECK
    if ( !image->isSet )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if ( texture->referenceBuffer )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    _jsSetSubImage( target, level, texture, image, xoffset, yoffset, zoffset, width, height, depth, LContext->unpackAlignment, format, type, pixels );
    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
}

/**
 *  @short Specify a two-dimensional compressed texture image
 *
 *  glCompressedTexImage2D() defines a two-dimensional texture image in compressed format.
 *
 *  S3TC compression is supported for texture images.
 *  A texture image that uses the S3TC compression algorithm is scrutinized in blocks of 4x4 texels.
 *  With each block, there is a 4 color look-up table with 16-bit colors (RGB565) and
 *  two representative colors which are loaded into the look-up table.  The other two indices
 *  in the look-up table are occupied by colors derived from the two representative colors.
 *  (If transparency is represented, then only one of the two indices is derived, and the
 *  other represents a transparent texel.)
 *  Each of the 16 texels in the 4x4 block is represented by two bits, which acts as an index
 *  into the look-up table.
 *
 *  The following are the valid values for <I><c>internalFormat</c></I> for S3TC textures:
<TABLE>
<TR>
<TD><CODE>COMPRESSED_RGB_S3TC_DXT1_EXT</CODE></TD>
<TD>Each 4x4 block of texels consists of 64
bits of RGB image data.

Each RGB image data block is encoded as a sequence of 8 bytes, called (in
order of increasing address):

c0_lo, c0_hi, c1_lo, c1_hi, bits_0, bits_1, bits_2, bits_3

The 8 bytes of the block are decoded into three quantities:

color0 = c0_lo + c0_hi &times; 256

color1 = c1_lo + c1_hi &times; 256

bits   = bits_0 + 256 &times; (bits_1 + 256 &times; (bits_2 + 256 &times; bits_3))

color0 and color1 are 16-bit unsigned integers that are unpacked to
RGB colors RGB0 and RGB1 as though they were 16-bit packed pixels with
a <I><c>format</c></I> of RGB and a type of <CODE>UNSIGNED_SHORT_5_6_5</CODE>.

bits is a 32-bit unsigned integer, from which a two-bit control code
is extracted for a texel at location (x,y) in the block using:

code(x,y) = bits[2&times;(4&times;y+x)+1..2&times;(4&times;y+x)+0]

where bit 31 is the most significant and bit 0 is the least
significant bit.

The RGB color for a texel at location (x,y) in the block is given by:


RGB0 if color0 > color1 and code(x,y) == 0

RGB1 if color0 > color1 and code(x,y) == 1

(2&times;RGB0+RGB1)/3 if color0 > color1 and code(x,y) == 2

(RGB0+2&times;RGB1)/3 if color0 > color1 and code(x,y) == 3

RGB0 if color0 <= color1 and code(x,y) == 0

RGB1 if color0 <= color1 and code(x,y) == 1

(RGB0+RGB1)/2 if color0 <= color1 and code(x,y) == 2

BLACK if color0 <= color1 and code(x,y) == 3

Arithmetic operations are done per component, and BLACK refers to an
RGB color where red, green, and blue are all zero.

Because this image has an RGB format, there is no alpha component and this
image is considered fully opaque.
</TD>
</TR>
<TR>
<TD><CODE>COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE></TD>
<TD>Each 4x4 block of texels consists of 64
bits of RGB image data and minimal alpha information.  The RGB components
of a texel are extracted in the same way as <CODE>COMPRESSED_RGB_S3TC_DXT1_EXT</CODE>.

The alpha component for a texel at location (x,y) in the block is
given by:


0.0 if color0 <= color1 and code(x,y) == 3

1.0 otherwise


<STRONG>IMPORTANT:</STRONG>  When encoding an RGBA image into a format using 1-bit
alpha, any texels with an alpha component less than 0.5 end up with an
alpha of 0.0 and any texels with an alpha component greater than or
equal to 0.5 end up with an alpha of 1.0.  When encoding an RGBA image
into the <CODE>COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE> format, the resulting red,
green, and blue components of any texels with a final alpha of 0.0
will automatically be zero (black).  If this behavior is not desired
by an application, it should not use <CODE>COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE>.
</TD>
</TR>
<TR>
<TD><CODE>COMPRESSED_RGBA_S3TC_DXT3_EXT</CODE></TD>
<TD>Each 4x4 block of texels consists of 64
bits of uncompressed alpha image data followed by 64 bits of RGB image
data.

Each RGB image data block is encoded according to the
<CODE>COMPRESSED_RGB_S3TC_DXT1_EXT</CODE> format, with the exception that the two code
bits always use the non-transparent encodings.  In other words, they are
treated as though color0 > color1, regardless of the actual values of
color0 and color1.

Each alpha image data block is encoded as a sequence of 8 bytes, called
(in order of increasing address):

a0, a1, a2, a3, a4, a5, a6, a7

The 8 bytes of the block are decoded into one 64-bit integer:

alpha = a0 + 256 &times; (a1 + 256 &times; (a2 + 256 &times; (a3 + 256 * (a4 +

             256 &times; (a5 + 256 * (a6 + 256 &times; a7))))))

alpha is a 64-bit unsigned integer, from which a four-bit alpha value
is extracted for a texel at location (x,y) in the block using:

alpha(x,y) = bits[4&times;(4&times;y+x)+3..4&times;(4&times;y+x)+0]

where bit 63 is the most significant and bit 0 is the least
significant bit.

The alpha component for a texel at location (x,y) in the block is
given by alpha(x,y) / 15.
</TD>
</TR>
<TR>
<TD><CODE>COMPRESSED_RGBA_S3TC_DXT5_EXT</CODE></TD>
<TD>Each 4x4 block of texels consists of 64
bits of compressed alpha image data followed by 64 bits of RGB image data.

Each RGB image data block is encoded according to the
<CODE>COMPRESSED_RGB_S3TC_DXT1_EXT</CODE> format, with the exception that the two code
bits always use the non-transparent encodings.  In other words, they are
treated as though color0 > color1, regardless of the actual values of
color0 and color1.

Each alpha image data block is encoded as a sequence of 8 bytes, called
(in order of increasing address):

alpha0, alpha1, bits_0, bits_1, bits_2, bits_3, bits_4, bits_5

The alpha0 and alpha1 are 8-bit unsigned bytes converted to alpha
components by multiplying by 1/255.

The 6 "bits" bytes of the block are decoded into one 48-bit integer:

bits = bits_0 + 256 &times; (bits_1 + 256 &times; (bits_2 + 256 &times; (bits_3 +
                          256 &times; (bits_4 + 256 * bits_5))))

bits is a 48-bit unsigned integer, from which a three-bit control code
is extracted for a texel at location (x,y) in the block using:

code(x,y) = bits[3&times;(4&times;y+x)+1..3&times;(4&times;y+x)+0]

where bit 47 is the most significant and bit 0 is the least
significant bit.

The alpha component for a texel at location (x,y) in the block is
given by:

alpha0,                   code(x,y) == 0

alpha1,                   code(x,y) == 1


(6&times;alpha0 + 1&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 2

(5&times;alpha0 + 2&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 3

(4&times;alpha0 + 3&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 4

(3&times;alpha0 + 4&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 5

(2&times;alpha0 + 5&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 6

(1&times;alpha0 + 6&times;alpha1)/7,  alpha0 > alpha1 and code(x,y) == 7


(4&times;alpha0 + 1&times;alpha1)/5,  alpha0 <= alpha1 and code(x,y) == 2

(3&times;alpha0 + 2&times;alpha1)/5,  alpha0 <= alpha1 and code(x,y) == 3

(2&times;alpha0 + 3&times;alpha1)/5,  alpha0 <= alpha1 and code(x,y) == 4

(1&times;alpha0 + 4&times;alpha1)/5,  alpha0 <= alpha1 and code(x,y) == 5

0.0,                      alpha0 <= alpha1 and code(x,y) == 6

1.0,                      alpha0 <= alpha1 and code(x,y) == 7

</TD>
</TR>
</TABLE>
 *
 *  In this implementation, compressed formats for palettized textures are supported,
 *  but their use is strongly discouraged.
 *  Unlike S3TC compressed textures, palettized textures are internally expanded and stored
 *  as uncompressed data, so they are no more efficient than originally uncompressed textures.
 *
 *  A palettized texture consists of a palette followed by
multiple mip-levels of texture indices used for lookup into the palette. The palette format can be one of
<CODE>R5_G6_B5</CODE>, <CODE>RGBA4</CODE>, <CODE>RGB5_A1</CODE>, <CODE>RGB8</CODE>, or <CODE>RGBA8</CODE>.
The texture indices can have a resolution of 4 or 8 bits, so the number of palette entries is either 16 or 256.
If <I><c>level</c></I> is 0, only one mip-level of texture indices is described in <I><c>data</c></I>. Otherwise, the negative value of <i><c>level</c></i>
specifies up to which mip-level the texture indices are described. A possibly remaining pad nibble for the lowest
resolution mip-level is ignored.
 *
 *  @param target Specifies the target texture. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  Must be greater or equal 0.

 *  @param internalFormat  Specifies the color components in the texture.
 *  The following symbolic constants are accepted: <CODE>GL_COMPRESSED_RGB_S3TC_DXT1_EXT</CODE>,
 *  <CODE>GL_COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE>, <CODE>GL_COMPRESSED_RGBA_S3TC_DXT3_EXT</CODE>,
 *  <CODE>GL_COMPRESSED_RGBA_S3TC_DXT5_EXT</CODE>, <CODE>GL_PALETTE4_RGB8_OES</CODE>, <CODE>GL_PALETTE4_RGBA8_OES</CODE>,
 *  <CODE>GL_PALETTE4_R5_G6_B5_OES</CODE>, <CODE>GL_PALETTE4_RGBA4_OES</CODE>, <CODE>GL_PALETTE4_RGB5_A1_OES</CODE>,
 *  <CODE>GL_PALETTE8_RGB8_OES</CODE>, <CODE>GL_PALETTE8_RGBA8_OES</CODE>, <CODE>GL_PALETTE8_R5_G6_B5_OES</CODE>,
 *  <CODE>GL_PALETTE8_RGBA4_OES</CODE>, or <CODE>GL_PALETTE8_RGB5_A1_OES</CODE>.
 *  @param width  Specifies the width of the texture image. DXT images must be
 * specified in full DXT blocks (4x4). The width and height for DXT images can be
 * arbitrary, but the size is computed as if they are multiple of 4 (in order to have complete
 * blocks).  See <I><c>imageSize</c></I> for more information.
 * @par
 *  All implementations support texture images that are at least 64 texels wide.
 *  @param height Specifies the height of the texture image. See <I><c>width</c></I> for limitations.
 *  All implementations support texture images that are at least 64 texels high.
 *  @param border Specifies the width of the border. Must be 0.
 *  @param imageSize  Specifies the size of the compressed image data in bytes.  For DXT images, must be:
@code
ceil(width/4) &times; ceil(height/4) &times; bytesPerBlock
@endcode
where bytesPerBlock is 8 for DXT1, and 16 for DXT3 and 5.
 *  @param data       Usually specifies a pointer to the compressed image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>data</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 *  @note
 *  glPixelStorei() has no effect on compressed texture images.
 * @par
If there is a currently bound pixel buffer object, then <I><c>data</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>data</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 *  @par
 *  Use of palettized textures is strongly discouraged.
 *  @par
 *  glCompressedTexImage2D() specifies the two-dimensional texture for the currently bound texture, specified with glBindTexture(),
 *  and the current texture unit, specified with glActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD></TD>
<TD><I><c>internalFormat</c></I> is not one of the accepted symbolic constants.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is less than 0 or greater than
 the value returned by calling glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_SIZE</CODE> (divided by 2<sup>level</sup> if level is positive).</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>border</c></I> is not 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>imageSize</c></I> is not consistent with format, dimensions, and
contents of the compressed image.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCompressedTexSubImage2D, glCopyTexImage2D,
 *  glCopyTexSubImage2D, glPixelStorei, glTexEnvfv, glTexImage2D, glTexParameterf
 */

GLAPI void APIENTRY glCompressedTexImage2D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data )
{
    JS_APIENTRY_TIMER( glCompressedTexImage2D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_8( glCompressedTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 8,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_8( glCompressedTexImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 8,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, imageSize, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif


#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // width/height correspond to mip 0 or mip 'level' based on the sign of level...
    GLsizei maxSizeForFirstImage = level < 0 ? _JS_MAX_TEXTURE_SIZE : ( _JS_MAX_TEXTURE_SIZE >> level );
    if (( width < 0 ) || ( height < 0 ) || ( imageSize < 0 ) || ( border != 0 ) ||
            ( width > maxSizeForFirstImage ) ||
            ( height > maxSizeForFirstImage ) ||
            ( 1 - level > _JS_MAX_TEXTURE_SIZE_LOG2 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_2D:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    // determine if platform support for this format exists
    const GLenum status = _jsPlatformChooseInternalFormat( internalFormat );
    if ( status != GL_INVALID_ENUM )
    {
        // map PBO if used as input
        jsBufferObject* bufferObject = NULL;
        if ( LContext->PixelUnpackBuffer != 0 )
        {
            bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
            JS_ASSERT( bufferObject != NULL );
            data = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                   (( const GLubyte* )data - ( const GLubyte* )NULL );
        }

        // let the platform take it from here
        _jsPlatformSetCompressedTexture(
            target,
            level,
            internalFormat,
            width, height, 1,
            border,
            imageSize,
            data );

        if ( LContext->PixelUnpackBuffer != 0 )
        {
            JS_ASSERT( bufferObject != NULL );
            _jsPlatformBufferObjectUnmap( bufferObject );
        }
    }
    else
    {
        // no support, but we'll deindex paletted textures
        switch ( internalFormat )
        {
            case GL_PALETTE4_RGB8_OES:
            case GL_PALETTE4_RGBA8_OES:
            case GL_PALETTE4_R5_G6_B5_OES:
            case GL_PALETTE4_RGBA4_OES:
            case GL_PALETTE4_RGB5_A1_OES:
            case GL_PALETTE8_RGB8_OES:
            case GL_PALETTE8_RGBA8_OES:
            case GL_PALETTE8_R5_G6_B5_OES:
            case GL_PALETTE8_RGBA4_OES:
            case GL_PALETTE8_RGB5_A1_OES:
            {
                // map PBO if used as input
                jsBufferObject* bufferObject = NULL;
                if ( LContext->PixelUnpackBuffer != 0 )
                {
                    bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
                    JS_ASSERT( bufferObject != NULL );
                    data = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                           (( const GLubyte* )data - ( const GLubyte* )NULL );
                }

                GLboolean r = _jsPaletteTexImage(
                                  target,
                                  level,
                                  internalFormat,
                                  width, height, 1,
                                  border,
                                  imageSize,
                                  data );

                if ( LContext->PixelUnpackBuffer != 0 )
                {
                    JS_ASSERT( bufferObject != NULL );
                    _jsPlatformBufferObjectUnmap( bufferObject );
                }

                if ( !r ) return;
            }
            break;
            default:
                //
                _jsSetError( status );
                return;
        }
    }

    jsTexture *texture = NULL;
    jsImage *image = NULL;
    _jsGetImage( target, level, &texture, &image, MAX( width, height ) );

    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED;
}

/**
 *  @short Specify a three-dimensional compressed texture image
 *
 *  glCompressedTexImage3D() defines a three-dimensional texture image in compressed format.
 *
 *  In this implementation, compressed formats for palettized textures are supported,
 *  but their use is strongly discouraged.
 *  Palettized textures are internally expanded and stored
 *  as uncompressed data, so they are no more efficient than originally uncompressed textures.
 *
 *  A palettized texture consists of a palette followed by
multiple mip-levels of texture indices used for lookup into the palette. The palette format can be one of
<CODE>R5_G6_B5</CODE>, <CODE>RGBA4</CODE>, <CODE>RGB5_A1</CODE>, <CODE>RGB8</CODE>, or <CODE>RGBA8</CODE>.
The texture indices can have a resolution of 4 or 8 bits, so the number of palette entries is either 16 or 256.
If <I><c>level</c></I> is 0, only one mip-level of texture indices is described in <I><c>data</c></I>. Otherwise, the negative value of <i><c>level</c></i>
specifies up to which mip-level the texture indices are described. A possibly remaining pad nibble for the lowest
resolution mip-level is ignored.
 *
 *  @param target Specifies the target texture. Must be <CODE>GL_TEXTURE_3D</CODE>.
 *  @param level  Specifies the level-of-detail number. Level 0 is the base image level.
 *  Level <I>n</I> is the <I>n</I>th mipmap reduction image.
 *  Must be greater or equal 0.

 *  @param internalFormat  Specifies the color components in the texture.
 *  The following symbolic constants are accepted: <CODE>GL_PALETTE4_RGB8_OES</CODE>, <CODE>GL_PALETTE4_RGBA8_OES</CODE>,
 *  <CODE>GL_PALETTE4_R5_G6_B5_OES</CODE>, <CODE>GL_PALETTE4_RGBA4_OES</CODE>, <CODE>GL_PALETTE4_RGB5_A1_OES</CODE>,
 *  <CODE>GL_PALETTE8_RGB8_OES</CODE>, <CODE>GL_PALETTE8_RGBA8_OES</CODE>, <CODE>GL_PALETTE8_R5_G6_B5_OES</CODE>,
 *  <CODE>GL_PALETTE8_RGBA4_OES</CODE>, and <CODE>GL_PALETTE8_RGB5_A1_OES</CODE>.
 *  @param width  Specifies the width of the texture image.
 *  All implementations support texture images that are at least 64 texels wide.
 *  @param height Specifies the height of the texture image.
 *  All implementations support texture images that are at least 64 texels high.
 *  @param depth Specifies the depth of the texture image.
 *  All implementations support texture images that are at least 64 texels high.
 *  @param border Specifies the width of the border. Must be 0.
 *  @param imageSize  Specifies the size of the compressed image data in bytes.
 *  @param data       Usually specifies a pointer to the compressed image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>data</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 *  @note
 *  glPixelStorei() has no effect on compressed texture images.
 *  @par
If there is a currently bound pixel buffer object, then <I><c>data</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>data</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 *  @par
 *  Use of palettized textures is strongly discouraged.
 *  @par
 *  glCompressedTexImage3D() specifies the three-dimensional texture for the currently bound texture, specified with glBindTexture(),
 *  and the current texture unit, specified with glActiveTexture().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_3D</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not one of the accepted symbolic constants.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I>, <I><c>height</c></I>, or <I><c>depth</c></I> is less than 0 or greater than
 the value returned by calling glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_SIZE</CODE> (divided by 2<sup>level</sup> when level is positive).</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>border</c></I> is not 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>imageSize</c></I> is not consistent with format, dimensions, and contents of the compressed image.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCompressedTexSubImage3D,
 *  glCopyTexSubImage3D, glPixelStorei, glTexEnvfv,
 *  glTexImage3D, glTexParameterf
 */


GLAPI void APIENTRY glCompressedTexImage3D( GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data )
{
    JS_APIENTRY_TIMER( glCompressedTexImage3D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_9( glCompressedTexImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_9( glCompressedTexImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 border, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, imageSize, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    // width/height correspond to mip 0 or mip 'level' based on the sign of level...
    GLsizei maxSizeForFirstImage = level < 0 ? _JS_MAX_TEXTURE_SIZE_3D : ( _JS_MAX_TEXTURE_SIZE_3D >> level );
    if (( width < 0 ) || ( height < 0 ) || ( depth < 0 ) || ( imageSize < 0 ) || ( border != 0 ) ||
            ( width > maxSizeForFirstImage ) ||
            ( height > maxSizeForFirstImage ) ||
            ( depth > maxSizeForFirstImage ) ||
            ( 1 - level > _JS_MAX_TEXTURE_SIZE_3D_LOG2 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    if ( target != GL_TEXTURE_3D )
    {
        // other cases handled by _jsGetImage
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    // determine if platform support for this format exists
    const GLenum status = _jsPlatformChooseInternalFormat( internalFormat );
    if ( status != GL_INVALID_ENUM )
    {
        // map PBO if used as input
        jsBufferObject* bufferObject = NULL;
        if ( LContext->PixelUnpackBuffer != 0 )
        {
            bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
            JS_ASSERT( bufferObject != NULL );
            data = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                   (( const GLubyte* )data - ( const GLubyte* )NULL );
        }

        // let the platform take it from here
        _jsPlatformSetCompressedTexture(
            target,
            level,
            internalFormat,
            width, height, depth,
            border,
            imageSize,
            data );

        if ( LContext->PixelUnpackBuffer != 0 )
        {
            JS_ASSERT( bufferObject != NULL );
            _jsPlatformBufferObjectUnmap( bufferObject );
        }
    }
    else
    {
        // no support, but we'll deindex paletted textures
        switch ( internalFormat )
        {
            case GL_PALETTE4_RGB8_OES:
            case GL_PALETTE4_RGBA8_OES:
            case GL_PALETTE4_R5_G6_B5_OES:
            case GL_PALETTE4_RGBA4_OES:
            case GL_PALETTE4_RGB5_A1_OES:
            case GL_PALETTE8_RGB8_OES:
            case GL_PALETTE8_RGBA8_OES:
            case GL_PALETTE8_R5_G6_B5_OES:
            case GL_PALETTE8_RGBA4_OES:
            case GL_PALETTE8_RGB5_A1_OES:
            {
                // map PBO if used as input
                jsBufferObject* bufferObject = NULL;
                if ( LContext->PixelUnpackBuffer != 0 )
                {
                    bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
                    JS_ASSERT( bufferObject != NULL );
                    data = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
                           (( const GLubyte* )data - ( const GLubyte* )NULL );
                }

                GLboolean r = _jsPaletteTexImage(
                                  target,
                                  level,
                                  internalFormat,
                                  width, height, depth,
                                  border,
                                  imageSize,
                                  data );

                if ( LContext->PixelUnpackBuffer != 0 )
                {
                    JS_ASSERT( bufferObject != NULL );
                    _jsPlatformBufferObjectUnmap( bufferObject );
                }

                if ( !r ) return;
            }
            break;
            default:
                _jsSetError( GL_INVALID_ENUM );
                return;
        }
    }

    jsTexture *texture = NULL;
    jsImage *image = NULL;
    _jsGetImage( target, level, &texture, &image, MAX( MAX( width, height ), depth ) );

    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED;
}
/**
 *  @short Specify a two-dimensional texture subimage in a compressed format
 *
 *  glCompressedTexSubImage2D() redefines a contiguous subregion of an existing two-dimensional compressed texture image.
The texels referenced by <I><c>data</c></I> replace the portion of the existing texture array with x indices <I><c>xoffset</c></I>
and <I><c>xoffset</c></I> + <I><c>width</c></I> - 1, inclusive, and y indices <I><c>yoffset</c></I> and <I><c>yoffset</c></I> + <I><c>height</c></I> - 1,
inclusive. This region may not include any texels outside the range of the texture array as it was originally
specified. It is not an error to specify a subtexture with zero width or height, but such a specification has no effect.
 *
 *  @param target Specifies the target texture. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.
 *  @param level  Specifies the level-of-detail number.
 *  @param xoffset  Specifies a texel offset in the x direction within the texture array.  For DXT images, must be a multiple of 4.
 *  @param yoffset  Specifies a texel offset in the y direction within the texture array. For DXT images, must be a multiple of 4.
 *  @param width  Specifies the width of the texture subimage. For DXT images, must be a multiple of 4.
 *  @param height Specifies the height of the texture subimage. For DXT images, must be a multiple of 4.
 *  @param format     Specifies the format of the compressed image data stored at the address <I><c>data</c></I>.
 *  The following symbolic constants are accepted: <CODE>GL_COMPRESSED_RGB_S3TC_DXT1_EXT</CODE>,
 *  <CODE>GL_COMPRESSED_RGBA_S3TC_DXT1_EXT</CODE>, <CODE>GL_COMPRESSED_RGBA_S3TC_DXT3_EXT</CODE>,
 *  or <CODE>GL_COMPRESSED_RGBA_S3TC_DXT5_EXT</CODE>.
 *  @param imageSize  Specifies the size of the compressed pixel data in bytes.
 *  @param data       Usually specifies a pointer to the compressed image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>data</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 *  @note
glPixelStorei() has no effect on compressed texture images.
 *  @par
If there is a currently bound pixel buffer object, then <I><c>data</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>data</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 *  @par
glCompressedTexSubImage2D() specifies the two-dimensional sub texture for the currently bound texture, specified with glBindTexture(), and the current texture unit, specified with glActiveTexture().
@par
The format of the data, <I><c>format</c></I>, ought to agree with the format of the previously-stored texture.
@par
Modifications of S3TC textures cannot replace partial 4x4 blocks of texels.
For S3TC textures, the offsets (<I><c>xoffset</c></I> and <I><c>yoffset</c></I>)
must be multiples of four texels.  The <I><c>width</c></I> (or <I><c>height</c></I>) of an S3TC texture must also be a
multiple of four, unless the entire width (or height) of the texture is replaced.
 *  @par
 *  On this implementation, you should not specify a palettized texture as a subimage, because
 *  the format does not match any internally stored textures.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not one of <CODE>GL_TEXTURE_2D</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_X</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_X</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Y</CODE>, <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Y</CODE>,
<CODE>GL_TEXTURE_CUBE_MAP_POSITIVE_Z</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP_NEGATIVE_Z</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a
 previous glCompressedTexImage2D() operation.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>Either <I><c>xoffset</c></I> and <I><c>yoffset</c></I> is not a multiple of four.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>Either <I><c>width</c></I> is neither a multiple of four nor equal to the value returned by querying <CODE>GL_TEXTURE_WIDTH</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>Either <I><c>height</c></I> is neither a multiple of four nor equal to the value returned by querying <CODE>GL_TEXTURE_HEIGHT</CODE>.</TD></TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c> < - b, <c>xoffset</c> + <c>width</c> > (w - b), <c>yoffset</c> < - b,</I> or <I><c>yoffset</c> + <c>height</c> > (h - b)</I> , where <I>w</I> is the texture width, <I>h</I> is the texture height,
and <I>b</I> is the border of the texture image being modified.
Note that <I>w</I> and <I>h</I> include twice the border width.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> or <I><c>height</c></I> is less than 0.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glBindTexture, glCompressedTexImage2D, glCopyTexSubImage2D,
 *  glGetIntegerv, glPixelStorei, glTexEnvfv, glTexParameterf
 */

GLAPI void APIENTRY glCompressedTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data )
{
    JS_APIENTRY_TIMER( glCompressedTexSubImage2D );
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifdef JS_DPM
    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_9( glCompressedTexSubImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_9( glCompressedTexSubImage2D, JSDPM_CALL_TYPE_STATE_TEX, , 9,
                                 target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                 imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                 data, imageSize, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

#ifndef JS_NO_ERROR_CHECK
    if ( !LContext->CurrentImageUnit )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
    if (( width < 0 ) || ( height < 0 ) || ( imageSize < 0 ) || ( level < 0 ) || ( xoffset < 0 ) || ( yoffset < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    if (( width == 0 ) || ( height == 0 ) )
        return;	// no error

#ifndef JS_NO_ERROR_CHECK
    if ( LContext->PixelUnpackBuffer == 0 && data == NULL )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    switch ( target )
    {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_2D:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif

    // map PBO if used as input
    jsBufferObject* bufferObject = NULL;
    if ( LContext->PixelUnpackBuffer != 0 )
    {
        bufferObject = _jsGetBufferObject( LContext, LContext->PixelUnpackBuffer );
        JS_ASSERT( bufferObject != NULL );
        data = _jsPlatformBufferObjectMap( bufferObject, GL_READ_ONLY ) +
               (( const GLubyte* )data - ( const GLubyte* )NULL );
    }

    _jsPlatformSetCompressedTextureSub(
        target,
        level,
        xoffset, yoffset, 0,
        width, height, 1,
        format,
        imageSize,
        data );

    // unmap PBO
    if ( LContext->PixelUnpackBuffer != 0 )
    {
        JS_ASSERT( bufferObject != NULL );
        _jsPlatformBufferObjectUnmap( bufferObject );
    }

    jsTexture *texture = NULL;
    jsImage *image = NULL;
    _jsGetImage( target, level, &texture, &image, MAX( width, height ) );

    texture->revalidate |= _JS_TEXTURE_REVALIDATE_IMAGES;
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED ;
}

/**
 *  @short Specify a three-dimensional texture subimage in a compressed format
 *
 *  glCompressedTexSubImage3D() redefines a contiguous subregion of an existing three-dimensional compressed texture image.
The texels referenced by <I><c>data</c></I> replace the portion of the existing texture array with x indices <I><c>xoffset</c></I>
and <I><c>xoffset</c></I> + <I><c>width</c></I> - 1, and the y indices <I><c>yoffset</c></I> and <I><c>yoffset</c></I> + <I><c>height</c></I> - 1,
and the z indices <I><c>zoffset</c></I> and <I><c>zoffset</c></I> + <I><c>depth</c></I> - 1,
inclusive. This region may not include any texels outside the range of the texture array as it was originally
specified. It is not an error to specify a subtexture with zero width or height, but such a specification has no effect.
 *
Currently, there is no supported compressed format for this function.
 *
 *  @param target Specifies the target texture. Must be <CODE>GL_TEXTURE_3D</CODE>.
 *  @param level  Specifies the level-of-detail number.
 *  @param xoffset  Specifies a texel offset in the x direction within the texture array.
 *  @param yoffset  Specifies a texel offset in the y direction within the texture array.
 *  @param zoffset  Specifies a texel offset in the z direction within the texture array.
 *  @param width  Specifies the width of the texture subimage.
 *  @param height Specifies the height of the texture subimage.
 *  @param depth  Specifies the depth of the texture subimage.
 *  @param format     Specifies the format of the compressed image data stored at the address <I><c>data</c></I>.
 *  @param imageSize  Specifies the number of unsigned bytes of image data starting at the address
specified by <I><c>data</c></I>.
 *  @param data       Usually specifies a pointer to the image data in memory.
If a pixel buffer object is bound with the target
<CODE>GL_PIXEL_UNPACK_BUFFER_ARB</CODE>, then <I><c>data</c></I> is not
a pointer, but an offset into a buffer object with its data store on the server side.
 *
 *  @note
glPixelStorei() has no effect on compressed texture images.
 *  @par
If there is a currently bound pixel buffer object, then <I><c>data</c></I> is not a
direct pointer to a client memory location but is an offset into a
buffer object with its data store on the server side. glBufferData() or
glBufferSubData() should have previously loaded the texture data into server
memory. In this case, if <I><c>data</c></I> is <CODE>NULL</CODE>, then the start of the
pixel buffer object is referenced.
 *  @par
glCompressedTexSubImage3D() specifies the three-dimensional sub texture for the currently bound texture,
 *  specified with glBindTexture(), and the current texture unit, specified with glActiveTexture().
 *  @par
 *  On this implementation, you should not specify a palettized texture as a subimage, because
 *  the format does not match any internally stored textures.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not <CODE>GL_TEXTURE_3D</CODE>.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>The texture array has not been defined by a previous glCompressedTexImage3D() operation.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>level</c></I> is less than 0.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>xoffset</c> < - b, <c>xoffset</c> + <c>width</c> > (w - b), <c>yoffset</c> < - b, <c>yoffset</c> + <c>height</c> > (h - b),
<c>zoffset</c> < - b</I>, or <I><c>zoffset</c> + <c>depth</c> > (d - b)</I>, where <I>w</I> is the texture width,
<I>h</I> is the texture height, <I>d</I> is the texture depth,
and <I>b</I> is the border of the texture image being modified.
Note that <I>w</I> and <I>h</I> include twice the border width.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glCompressedTexImage3D, glCopyTexSubImage3D,
 *  glGetIntegerv, glPixelStorei, glTexEnvfv, glTexParameterf, glTexImage3D, glTexSubImage3D
 */

GLAPI void APIENTRY glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data )
{
    JS_APIENTRY_TIMER( glCompressedTexSubImage3D );
#ifdef JS_DPM
    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    if ( _jsdpmState )
    {
        if ( LContext->PixelUnpackBuffer )
        {
            _JS_DPM_API_ENTRY_11( glCompressedTexSubImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 11,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  zoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  data, sizeof( GLvoid * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
        }
        else
        {
            _JS_DPM_API_ENTRY_11( glCompressedTexSubImage3D, JSDPM_CALL_TYPE_STATE_TEX, , 11,
                                  target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  level, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  xoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  yoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  zoffset, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  width, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  height, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  depth, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  format, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                                  imageSize, sizeof( GLsizei ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                                  data, imageSize, INPUT_BY_REFERENCE, DPM_API_ARGUMENT_POINTER );
        }
    }
#endif

    // there is no supported enums. The palette ones are not supposed to be
    // called with TexSubImage
    _jsSetError( GL_INVALID_ENUM );
    return;
}

/**
 *  @short Select server-side active texture unit
 *
 *  glActiveTexture() selects which texture unit subsequent texture state calls will affect.
 *  The number of texture units an implementation supports is implementation dependent, it must be at least 1.
 *
 *  @param texture Specifies which texture unit to make active. The number of texture units
 *  is implementation dependent, but must be at least one.
 *  texture must be one of <CODE>GL_TEXTURE</CODE><I>i</I>, where 0 <= <I>i</I> < k, where
 *  k is the maximum of the values returned by calling glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_UNITS</CODE>, <CODE>GL_MAX_TEXTURE_IMAGE_UNITS_ARB</CODE>,
 *  and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>, which are all implementation-dependent values.
 *  The initial value is <CODE>GL_TEXTURE0</CODE>.
 *
 *  @note
 *  It is always the case that <CODE>GL_TEXTURE</CODE><I>i</I> = <CODE>GL_TEXTURE0</CODE> + <I>i</I>.
 *  @par
 *  If the fixed-function fragment processing pipeline is used, a texture unit consists of the texture enable state,
 *  texture matrix stack, texture environment and
 *  currently bound texture. Modifying any of these states has an effect only on the active texture unit.
 *  If a custom fragment shader program is written and used, a texture unit consists of either
 *  a texture coordinate set, texture image unit, or both.
 *  @par
 *  Vertex arrays are client-side GL resources, which are selected by the
 *  glClientActiveTexture() routine.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I>texture</I> is not one of <CODE>GL_TEXTURE</CODE><I>i</I>,
  where 0 <= <I>i</I> < k, where k is the maximum of the values returned by calling glGetIntegerv() with argument <CODE>GL_MAX_TEXTURE_UNITS</CODE>,
  <CODE>GL_MAX_TEXTURE_IMAGE_UNITS_ARB</CODE>, and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>.
  The number of texture units with a custom fragment shader program is always either equal to or
  exceeds the number of texture units with the fixed-function fragment processing pipeline.
  If you are using the fixed-function fragment processing pipeline and you reference
  a texture unit that is beyond <CODE>GL_MAX_TEXTURE_UNITS</CODE>, but less than the overall maximum,
  it does not generate a GL error. However, it will behave as if the texture unit is disabled.</TD>
</TR>
</TABLE>
 *
 *  @sa glBindTexture, glClientActiveTexture, glEnable, glGetIntegerv,
 *  glMatrixMode, glMultiTexCoord4f, glTexEnvfv
 */

GLAPI void APIENTRY glActiveTexture( GLenum texture )
{
    JS_APIENTRY_TIMER( glActiveTexture );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glActiveTexture, JSDPM_CALL_TYPE_STATE_TEX, , 1, texture, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    PSGLcontext*	LContext = _CurrentContext;

    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( texture < GL_TEXTURE0 || texture >= GL_TEXTURE0 + _JS_MAX_TEXTURE_UNITS_MAX )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    int unit = texture - GL_TEXTURE0;
    LContext->ActiveTexture = unit;
    LContext->CurrentImageUnit = unit < _JS_MAX_TEXTURE_IMAGE_UNITS ? LContext->TextureImageUnits + unit : NULL;
    LContext->CurrentCoordsUnit = unit < _JS_MAX_TEXTURE_COORDS ? LContext->TextureCoordsUnits + unit : NULL;
}

/**
 *  @short Select client-side active texture unit
 *
 *  glClientActiveTexture() selects the vertex array client state parameters to be modified by
 *  glTexCoordPointer(), and enabled or disabled with glEnableClientState() or glDisableClientState(),
 *  respectively, when called with a parameter of <CODE>GL_TEXTURE_COORD_ARRAY</CODE>.
 *
 *  @param texture Specifies which texture unit to make active. The number of texture units
 *  is implementation dependent, but must be at least one.
 *  texture must be one of <CODE>GL_TEXTURE</CODE><I>i</I>, where 0 <= <I>i</I> < k, where
 *  k is the maximum of the values returned by <CODE>GL_MAX_TEXTURE_UNITS</CODE>
 *  and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>, which are both implementation-dependent values.
 *  The initial value is <CODE>GL_TEXTURE0</CODE>.
 *
 *  @note
 *  It is always the case that <CODE>GL_TEXTURE</CODE><I>i</I> = <CODE>GL_TEXTURE0</CODE> + <I>i</I>.
 *  @par
 *  If a custom shader program is written and used, a texture unit consists of either
 *  a texture coordinate set, texture image unit, or both.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I>texture</I> is not one of <CODE>GL_TEXTURE</CODE><I>i</I>,
  where 0 <= <I>i</I> < k, where k is the maximum of the values returned by calling glGetIntegerv() with argument
  <CODE>GL_MAX_TEXTURE_UNITS</CODE>,
  and <CODE>GL_MAX_TEXTURE_COORDS_ARB</CODE>.
  If you are using the fixed-function processing pipeline and you reference
  a texture unit that is beyond <CODE>GL_MAX_TEXTURE_UNITS</CODE>, but less than the overall maximum,
  it does not generate a GL error. However, it will behave as if the texture unit is disabled.</TD>
</TR>
</TABLE>
 *
 *  @sa glActiveTexture, glEnableClientState, glGetIntegerv, glMultiTexCoord4f, glTexCoordPointer
 */

GLAPI void APIENTRY glClientActiveTexture( GLenum texture )
{
    JS_APIENTRY_TIMER( glClientActiveTexture );
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_1( glClientActiveTexture, JSDPM_CALL_TYPE_STATE_TEX, , 1, texture, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM ); }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( texture < GL_TEXTURE0 || texture >= GL_TEXTURE0 + _JS_MAX_TEXTURE_COORDS )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    LContext->CS_ActiveTexture = texture - GL_TEXTURE0;
}

/**
 *  @short Set pixel storage modes
 *
 *  glPixelStorei() sets pixel storage modes that affect the operation of subsequent glReadPixels()
 *  as well as the unpacking of glTexImage2D(), and glTexSubImage2D().
 *
 *  <I><c>pname</c></I> is a symbolic constant indicating the parameter to be set, and <I><c>param</c></I> is the new value.
 *  The following storage parameter affects how pixel data is returned to client memory.
 *  This value is significant for glReadPixels():

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_PACK_ALIGNMENT</CODE></TD>
 * <TD>
 * Specifies the alignment requirements for the start of each pixel row in memory. The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries). The initial value is 4.
 * </TD>
 * </TR>
 * </TABLE>
 *
 * The following storage parameter affects how pixel data is read from client memory.
 * This value is significant for glTexImage2D() and glTexSubImage2D():

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_UNPACK_ALIGNMENT</CODE></TD>
 * <TD>
 * Specifies the alignment requirements for the start of each pixel row in memory. The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries). The initial value is 4.
 * </TD>
 * </TR>
 * </TABLE>
 *
 *  @param pname Specifies the symbolic name of the parameter to be set.
 *  <CODE>GL_PACK_ALIGNMENT</CODE> affects the packing of pixel data into memory.
 *  <CODE>GL_UNPACK_ALIGNMENT</CODE> affects the unpacking of pixel data from memory.
 *
 *  @param param Specifies the value that <I><c>pname</c></I> is set to.
 *
 *  @note
 *  Pixel storage modes are client states.
 *  @par
    glCompressedTexImage2D(), glCompressedTexImage3D(), glCompressedTexSubImage2D(), and glCompressedTexSubImage3D()
 *  are not affected by glPixelStorei().
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>pname</c></I> is not an accepted value. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>Alignment is specified as other than 1, 2, 4, or 8.</TD>
</TR>
</TABLE>
 *
 *  @sa glReadPixels, glCompressedTexImage2D, glCompressedTexImage3D, glCompressedTexSubImage2D, glCompressedTexSubImage3D,
 *  glTexImage2D, glTexImage3D, glTexSubImage2D, glTexSubImage3D
 */

GLAPI void APIENTRY glPixelStorei( GLenum pname, GLint param )
{
    JS_APIENTRY_TIMER( glPixelStorei );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( glPixelStorei, JSDPM_CALL_TYPE_STATE_TEX, , 2,
                             pname, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             param, sizeof( GLint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    switch ( pname )
    {
        case GL_PACK_ALIGNMENT:
#ifndef JS_NO_ERROR_CHECK
            if (( param == 0 ) || ( param & ( param - 1 ) ) )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LContext->packAlignment = param;
            break;
        case GL_UNPACK_ALIGNMENT:
#ifndef JS_NO_ERROR_CHECK
            if (( param == 0 ) || ( param & ( param - 1 ) ) )
            {
                _jsSetError( GL_INVALID_VALUE );
                return;
            }
#endif
            LContext->unpackAlignment = param;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
}

/**
 *  @short Creates a texture by reference from a buffer object
 *
 *  glTextureReferenceSCE() creates a texture and sets its data by reference
 *  from a buffer object. Unlike glTexImage2D(), which creates a texture image
 *  by copying, you do not have to call glTextureReferenceSCE() when modifying the
 *  data. The texture address is specified by an offset relative to the
 *  buffer object currently bound to the <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE> target.
 *
 *  The texture must be specified in a RSX&trade; format so that it can directly use
 *  the texture data. As a general rule, any format that is supported by the RSX&trade;
 *  can be used for a texture buffer object.  For a complete list, see glTexImage2D().
 *  
 *  There are two classes of texture formats, <I>swizzled</I> and <I>linear</I>,
 *  each of which have their own characteristics and limitations.
 *
 *  Swizzled textures only work for power-of-two textures. The basic layout of a swizzled texture is
 *  the following:
 *  <UL>
 *  <LI>The origin of the texture must be aligned on 128 bytes</LI>
 *  <LI>In a given image, the texels are ordered in the swizzled format, meaning that they
 *  have a linear address which is generated from the x,y, and z coordinates by
 *  interleaving the bits of each coordinate. For example, suppose x, y, and z are expressed
 *  as follows:
 *  <BR><BR>
 *  <c>x_n....x_2.x_1.x_0</c><BR>
 *  <c>y_m....y_2.y_1.y_0</c><BR>
 *  <c>z_p....z_2.z_1.z_0</c><BR>
 *  <BR>Assuming <c>p > m > n</c>, the generated linear address will
 *  be the following:
 *  <BR><BR>
 *  <c>z_p.z_p-1...z_m+1.z_m.y_m...z_n+1.y_n+1.z_n.y_n.x_n...z_1.y_1.x_1.z_0.y_0.x_0</c></LI>
 *  <LI>The various mipmap levels of a given face are stored consecutively, tightly
 *  packed, in decreasing size.</LI>
 *  <LI>Between each face there is padding so that each face is aligned on 128
 *  bytes.</LI>
 *  </UL>
 *
 *  This layout is valid for texture formats that have up to 4 byte per
 *  texel. For formats having 8 or 16 bytes per texel, the same layout is
 *  used, but considered as an image that is respectively twice or four times as
 *  wide, but with 4 bytes per texel. For example a 64x64 texture using an
 *  internal format of <CODE>GL_RGBA16F_ARB</CODE> uses the layout of a 128x64 texture.
 *
 *  Linear textures work for any sort of dimensions. The basic layout is the
 *  following:
 *  <UL>
 *  <LI>The origin of the texture has to be naturally aligned on the size of the
 *  selected internal format (for example, 4 bytes if the internal format is
 *  <CODE>GL_RGBA</CODE>).</LI>
 *  <LI>Each texel line is padded to the selected pitch, and the same pitch is
 *  used for all the mipmap levels.</LI>
 *  <LI>The mipmap levels are stored consecutively, tightly packed (apart from
 *  the per-line pitch) in decreasing size</LI>
 *  <LI>Each face is stored consecutively without additional padding.</LI>
 *  </UL>
 *
 *  Special consideration must be taken if the buffer object is tiled (that is,
 *  if it was created with glBufferSurfaceSCE()). Swizzled textures are not
 *  supported on a tiled buffer. Also, if the texture is a render target, the
 *  texture must respect the following additional constraints:
 *  <UL>
 *  <LI>The pitch must be equal to that of the buffer object. The buffer object
 *  pitch can be queried with glGetBufferParameteriv(), using a <CODE><I>pname</I></CODE> of
 *  <CODE>GL_BUFFER_PITCH_SCE</CODE>.</LI>
 *  <LI>The offset of the texture in the buffer object must be a multiple of
 *  4 &times; pitch.</LI>
 *  </UL>
 *
 *  DXT textures use a specific layout, which is derived from the linear layout. Each
 *  4x4 DXT block is stored consecutively. The pitch must be equal to the
 *  number of bytes per texel line, that is 1/4 of the number of bytes per line
 *  of blocks.
 *
 *
 *  @param target Specifies the texture target. Must be one of <CODE>GL_TEXTURE_2D</CODE>,
 *  <CODE>GL_TEXTURE_3D</CODE>, or <CODE>GL_TEXTURE_CUBE_MAP</CODE>.
 *
 *  @param levels Specifies the number of mipmap levels of the texture
 *
 *  @param baseWidth  Specifies the width of the base level
 *  @param baseHeight Specifies the height of the base level
 *  @param baseDepth  Specifies the depth of the base level. Must be 1 for 2D
 *  and CUBE_MAP textures.
 *
 *  @param internalFormat Specifies the internal format of the texture.
 *
 *  @param pitch Specifies the pitch of the texture. A pitch of 0 means the
 *  texture is swizzled.
 *
 *  @param offset Specifies the offset of the texture relative to the buffer
 *  object.
 *
 *  @note
 *  For more information, including sample code, refer to the "Texture Buffer Objects" section of the
 *  <i>PlayStation&reg; Graphics Library Programming Guide</i>.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>target</c></I> is not an accepted value. </TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>internalFormat</c></I> is not a valid internal format.</TD>
</TR>
<TR>
<TD><CODE>GL_INVALID_OPERATION</CODE></TD>
<TD>There is no buffer object bound to <CODE>GL_TEXTURE_REFERENCE_BUFFER_SCE</CODE>.</TD>
</TR>
</TABLE>
 *
 *  @sa glTexImage2D, glTexImage3D, glBufferData, glBufferSurfaceSCE, glGetBufferParameteriv
 */

GLAPI void APIENTRY glTextureReferenceSCE( GLenum target, GLuint levels, GLuint baseWidth, GLuint baseHeight, GLuint baseDepth, GLenum internalFormat, GLuint pitch, GLintptr offset )
{
    JS_APIENTRY_TIMER( glTextureReferenceSCE );
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_8( glTextureReferenceSCE, JSDPM_CALL_TYPE_STATE_TEX, , 8,
                             target, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             levels, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             baseWidth, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             baseHeight, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             baseDepth, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             internalFormat, sizeof( GLenum ), INPUT_BY_VALUE, DPM_API_ARGUMENT_ENUM,
                             pitch, sizeof( GLuint ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE,
                             offset, sizeof( GLintptr ), INPUT_BY_VALUE, DPM_API_ARGUMENT_INT_VALUE );
    }

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if (( !LContext->CurrentImageUnit ) || ( !LContext->TextureBuffer ) )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }

    if (( !_jsIsInternalFormat( internalFormat ) ) && ( !_jsIsDXTInternalFormat( internalFormat ) ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }

    // XXX add correct checking

    switch ( target )
    {
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D:
            break;
        case GL_TEXTURE_CUBE_MAP:
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
#endif
    jsTexture *texture = _jsGetCurrentTexture( LContext->CurrentImageUnit, target );
#ifndef JS_NO_ERROR_CHECK
    if ( !texture )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    jsBufferObject *bufferObject = _jsGetBufferObject( LContext, LContext->TextureBuffer );
    JS_ASSERT( bufferObject );
    _jsReallocateImages( texture, 0, MAX( baseWidth, MAX( baseHeight, baseDepth ) ) );

    GLuint faces = texture->faceCount;
    for ( GLuint face = 0;face < faces;++face )
    {
        GLuint width = baseWidth;
        GLuint height = baseHeight;
        GLuint depth = baseDepth;
        for ( GLuint l = 0;l < levels;++l )
        {
            _jsSetImage(
                texture->images[face] + l,
                internalFormat,
                width, height, depth,
                LContext->unpackAlignment,
                0, 0, // format, type : unused
                NULL );
            width = MAX( 1U, width / 2 );
            height = MAX( 1U, height / 2 );
            depth = MAX( 1U, depth / 2 );
        }
    }
    texture->maxLevel = levels - 1;
    texture->usage = ( pitch == 0 ) ? GL_TEXTURE_SWIZZLED_GPU_SCE : GL_TEXTURE_LINEAR_GPU_SCE;

    GLboolean r = _jsPlatformTextureReference( texture, pitch, bufferObject, offset );
    if ( !r ) return;
    bufferObject->textureReferences.pushBack( texture );
    texture->referenceBuffer = bufferObject;
    texture->offset = offset;
    _jsTextureTouchFBOs( texture );
    LContext->needValidate |= PSGL_VALIDATE_TEXTURES_USED | PSGL_VALIDATE_VERTEX_TEXTURES_USED ;
}

/** @} OpenGL */
