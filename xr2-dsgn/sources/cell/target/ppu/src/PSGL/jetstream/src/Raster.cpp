/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * External PSGL functions relating to rasterization:
 *     glCullFace, glDepthRange, glFrontFace, glLineWidth, glPointSize, 
 *     glPolygonMode, glPolygonOffset, glSampleCoverage, glShadeModel, 
 *     glViewport
 */

#include <PSGL/psgl.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/DPM2.h>

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short  Set the viewport
 *
 * glViewport specifies the affine transformation of <I><c>x</c></I> and <I><c>y</c></I> from normalized
 * device coordinates to window coordinates. Let (xnd, ynd) be normalized device coordinates.
 * Then the window coordinates (xw, yw) are computed as follows:
 *
@code
   xw = (( xnd + 1 ) &times; width/2 ) + x
   yw = (( ynd + 1 ) &times; height/2 ) + y
@endcode
 *
 *  Viewport width and height are silently clamped to a range that depends on the implementation.
 *  To query this range, call glGetIntegerv() with argument <CODE>GL_MAX_VIEWPORT_DIMS</CODE>.
 *
 * @param x,y Specify the lower left corner of the viewport rectangle, in pixels. The initial value is (0, 0).
 * @param width,height Specify the width and height of the viewport.
 * When a GL context is first attached to a surface (e.g. window),
 * <I><c>width</c></I> and <I><c>height</c></I> are set to the dimensions of that surface.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD>Either <I><c>width</c></I> or <I><c>height</c></I> is negative.</TD>
</TR>
</TABLE>
 *
 * @sa glDepthRangef, glGetIntegerv
 */
GLAPI void APIENTRY glViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    JS_APIENTRY_TIMER( glViewport );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if (( width < 0 ) || ( height < 0 ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    LContext->ViewPort.X = x;
    LContext->ViewPort.Y = y;
    LContext->ViewPort.XSize = width;
    LContext->ViewPort.YSize = height;
    LContext->needValidate |= PSGL_VALIDATE_VIEWPORT;
}

/**
 * @short Specify mapping of depth values from normalized device coordinates to window coordinates
 *
 * After clipping and division by w, depth coordinates range from -1 to 1,
 * corresponding to the near and far clipping planes. glDepthRangef() specifies
 * a linear mapping of the normalized depth coordinates in this range
 * to window depth coordinates. The initial value of <I><c>zNear</c></I> is 0 and the initial value of <I><c>zFar</c></I> is 1.
 *
 * Regardless of the actual depth buffer
 * implementation, window coordinate depth values are treated as though
 * they range from 0 through 1 (like color components). Thus, the values
 * accepted by glDepthRangef() are both clamped to this range before they are
 * accepted.
 *
 * The setting of (0,1) maps the near plane to 0 and the far plane to 1.
 * With this mapping, the depth buffer range is fully utilized.
 *
 * @param zNear Specifies the mapping of the near clipping plane to window
 *        coordinates.
 * @param zFar Specifies the mapping of the far clipping plane to window
 *        coordinates.
 *
 * @note
 * It is not necessary that <I><c>zNear</c></I> be less than <I><c>zFar</c></I>.
 * Reverse mappings such as <I><c>zNear</c></I> = 1, and <I><c>zFar</c></I> = 0 are acceptable.
 *
 * @sa glDepthFunc, glPolygonOffset, glViewport
 */
GLAPI void APIENTRY glDepthRangef( GLclampf zNear, GLclampf zFar )
{
    JS_APIENTRY_TIMER( glDepthRangef );

    PSGLcontext*	LContext = _CurrentContext;

    LContext->DepthNear = _jsClampf( zNear );
    LContext->DepthFar = _jsClampf( zFar );
    LContext->needValidate |= PSGL_VALIDATE_VIEWPORT;
}

/** @} Matrix */

/**
 * @addtogroup OpenGL
 *
 * @{
 */

/**
 * @short Specify whether front- or back-facing polygons are culled
 *
 * glCullFace() specifies whether front- or back-facing polygons are culled
 * (as specified by mode) when polygon culling is enabled. To enable and disable polygon culling, call the
 * glEnable() and glDisable() commands with the argument <CODE>GL_CULL_FACE</CODE>.
 * Culling is initially disabled.
 *
 * glFrontFace() specifies which of the clockwise and counterclockwise
 * polygon are front-facing and back-facing.
 *
 * @param mode Specifies whether front- or back-facing polygons are candidates
 *        for culling. Symbolic constants <CODE>GL_FRONT</CODE>, <CODE>GL_BACK</CODE>, and
 *        <CODE>GL_FRONT_AND_BACK</CODE> are accepted. The initial value is <CODE>GL_BACK</CODE>.
 *
 * @note
 * If <I><c>mode</c></I> is <CODE>GL_FRONT_AND_BACK</CODE>, no polygons are drawn, but other primitives such as points and lines
 * are drawn.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
</TABLE>

 *
 * @sa glFrontFace, glEnable
 */
GLAPI void APIENTRY glCullFace( GLenum mode )
{
    JS_APIENTRY_TIMER( glCullFace );

    PSGLcontext*	LContext = _CurrentContext;

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsFaceMode( mode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    LContext->CullFaceMode = mode;
    LContext->needValidate |= PSGL_VALIDATE_FACE_CULL;
}

/**
 * @short Define front- and back-facing polygons
 *
 * In a scene composed entirely of opaque closed surfaces, back-facing polygons
 * are never visible. Eliminating these invisible polygons has the obvious
 * benefit of speeding up the rendering of the image. To enable and disable
 * elimination of back-facing polygons, call glEnable() and glDisable() with
 * argument <CODE>GL_CULL_FACE</CODE>.
 *
 * The projection of a polygon to window coordinates is said to have clockwise
 * winding if an imaginary object following the path from its first vertex, its
 * second vertex, and so on, to its last vertex, and finally back to its first
 * vertex, moves in a clockwise direction about the interior of the polygon.
 * The polygon&rsquo;s winding is said to be counterclockwise if the imaginary object
 * following the same path moves in a counterclockwise direction about the
 * interior of the polygon. glFrontFace() specifies whether polygons with
 * clockwise winding in window coordinates, or counterclockwise winding in
 * window coordinates, are taken to be front-facing. Passing <CODE>GL_CCW</CODE> to mode
 * selects counterclockwise polygons as front-facing; <CODE>GL_CW</CODE> selects clockwise
 * polygons as front-facing. By default, counterclockwise polygons are taken
 * to be front-facing.
 *
 * @param mode Specifies the orientation of front-facing polygons. <CODE>GL_CW</CODE> and
 *        <CODE>GL_CCW</CODE> are accepted. The initial value is <CODE>GL_CCW</CODE>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is not an accepted value.</TD>
</TR>
</TABLE>

 *
 * @sa glCullFace, glEnable, glLightModelfv
 */
GLAPI void APIENTRY glFrontFace( GLenum mode )
{
    JS_APIENTRY_TIMER( glFrontFace );

    PSGLcontext*	LContext = _CurrentContext;

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsFaceOrientation( mode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    LContext->FrontFace = mode;
    LContext->needValidate |= PSGL_VALIDATE_FACE_CULL;
}

/**
 * @short Select a polygon rasterization mode
 *
 * glPolygonMode() controls the interpretation of polygons for
   rasterization.  <I><c>face</c></I> describes which polygons <I><c>mode</c></I> applies
   to:  front-facing polygons (<CODE>GL_FRONT</CODE>), back-facing polygons
   (<CODE>GL_BACK</CODE>), or both (<CODE>GL_FRONT_AND_BACK</CODE>).  The polygon
   affects only the final rasterization of polygons.  In
   particular, a polygon&rsquo;s vertices are lit and the polygon is
   clipped and possibly culled before these modes are applied.

Three modes are defined and can be specified in <I><c>mode</c></I>:

 * <TABLE rules=all>
 * <TR>
 * <TD><CODE>GL_POINT</CODE></TD>
 * <TD>Polygon vertices that are marked as the	start
 of a boundary edge are drawn as points. Point
 attributes such as <CODE>GL_POINT_SIZE</CODE> and
 <CODE>GL_POINT_SMOOTH</CODE> control the rasterization of
 the points.  Polygon rasterization attributes
 other than <CODE>GL_POLYGON_MODE</CODE> have no effect.
   </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_LINE</CODE></TD>
 * <TD>Line attributes such as <CODE>GL_LINE_WIDTH</CODE> and
   <CODE>GL_LINE_SMOOTH</CODE> control the rasterization of
   the lines.  Polygon rasterization attributes
   other than <CODE>GL_POLYGON_MODE</CODE> have no effect.
   </TD>
 * </TR>
 * <TR>
 * <TD><CODE>GL_FILL</CODE></TD>
 * <TD>The interior of	the polygon is filled.
   </TD>
 * </TR>
 * </TABLE>
 *
 * @param face Specifies the polygons that <I><c>mode</c></I> applies to.  Must be
 *             <CODE>GL_FRONT</CODE> for front-facing polygons, <CODE>GL_BACK</CODE> for
 *              back-facing polygons, or <CODE>GL_FRONT_AND_BACK</CODE>
 *              for front- and back-facing polygons.
 *
 * @param mode Specifies how polygons will be rasterized.  Accepted
		values are <CODE>GL_POINT</CODE>, <CODE>GL_LINE</CODE>, and <CODE>GL_FILL</CODE>.  The
		initial	value is <CODE>GL_FILL</CODE> for both front- and back-facing polygons.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD>Either <I><c>face</c></I> or <I><c>mode</c></I> is not
	  an accepted value.</TD>
</TR>
</TABLE>

 * @sa glLineWidth, glPointSize
 */

GLAPI void APIENTRY glPolygonMode( GLenum face, GLenum mode )
{
    JS_APIENTRY_TIMER( glPolygonMode );

    PSGLcontext*	LContext = _CurrentContext;

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsPolygonMode( mode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif
    switch ( face )
    {
        case GL_FRONT:
            LContext->FrontPolygonMode = mode;
            break;
        case GL_BACK:
            LContext->BackPolygonMode = mode;
            break;
        case GL_FRONT_AND_BACK:
            LContext->FrontPolygonMode = mode;
            LContext->BackPolygonMode = mode;
            break;
        default:
            _jsSetError( GL_INVALID_ENUM );
            return;
    }
    LContext->needValidate |= PSGL_VALIDATE_POLYGON_MODE;
}

/**
 * @short Specify the diameter of rasterized points
 *
 * glPointSize() specifies the rasterized diameter of both aliased and antialiased points.
 * Using a point size other than 1 has different effects, depending on whether point
 * antialiasing is enabled. To enable and disable point antialiasing, call glEnable() and
 * glDisable() with argument <CODE>GL_POINT_SMOOTH</CODE>. Point antialiasing is initially disabled.
 *
 * If point antialiasing is disabled, the actual size is determined by rounding the supplied
 * size to the nearest integer. (If the rounding results in the value 0, it is as if the
 * point size were 1.) If the rounded size is odd, then the center point (x, y) of the
 * pixel fragment that represents the point is computed as
 *
@code
    ( floor(xw) + 1/2, floor(yw) + 1/2)
@endcode
 *
 * where w indicates window coordinates. All pixels that lie within the square grid of the
 * rounded size centered at (x, y) make up the fragment. If the size is even, the center point is
 *
@code
    ( floor(xw + 1/2), floor(yw + 1/2))
@endcode
 *
 * and the rasterized fragment&rsquo;s centers are the half-integer window coordinates within the square
 * of the rounded size centered at (x, y) . All pixel fragments produced in rasterizing a
 * nonantialiased point are assigned the same associated data, that of the vertex corresponding
 * to the point.
 *
 * If antialiasing is enabled, then point rasterization produces a fragment for each pixel square that
 * intersects the region lying within the circle having diameter equal to the current point size and
 * centered at the point&rsquo;s (xw, yw) . The coverage value for each fragment is the window coordinate area
 * of the intersection of the circular region with the corresponding pixel square. This value is saved
 * and used in the final rasterization step. The data associated with each fragment is the data
 * associated with the point being rasterized.
 *
 * Not all sizes are supported when point antialiasing is enabled. If an unsupported size is requested, the
 * nearest supported size is used. Only size 1 is guaranteed to be supported; others depend on the
 * implementation. To query the range of supported sizes, call glGetIntegerv() with the
 * argument <CODE>GL_SMOOTH_POINT_SIZE_RANGE</CODE>. For aliased points, to query the supported ranges, use
 * glGetIntegerv() with the argument <CODE>GL_ALIASED_POINT_SIZE_RANGE</CODE>.
 *
 * The initial value of <I><c>size</c></I> is 1.
 *
 * @param size Specifies the diameter of rasterized points.
 *
 * @note
 * If <CODE>GL_VERTEX_PROGRAM_POINT_SIZE_ARB</CODE> is enabled,
 * then glPointSize() does not determine the point size.  Instead, the
 * point size is determined by the output of the vertex program,
 * the value of the semantic <CODE>PSIZE</CODE>.
 * @par 
 * A non-antialiased point size may be clamped to an implementation-dependent maximum. Although this
 * maximum cannot be queried, it must be no less than the maximum value for antialiased points, rounded
 * to the nearest integer value.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>size</c></I> is less than or equal to 0.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glGetIntegerv
 */
GLAPI void APIENTRY glPointSize( GLfloat size )
{
    JS_APIENTRY_TIMER( glPointSize );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( size <= 0.f )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    LContext->PointSize = size;
    LContext->needValidate |= PSGL_VALIDATE_POINT_RASTER;
}

/**
 *
 *  @short Specify the width of rasterized lines
 *
 * glLineWidth() specifies the rasterized width of both aliased and antialiased lines.
 * Using a line width other than 1 has different effects, depending on whether line
 * antialiasing is enabled. To enable and disable line antialiasing, call glEnable()
 * and glDisable() with argument <CODE>GL_LINE_SMOOTH</CODE>. Line antialiasing is initially disabled.
 *
 * If line antialiasing is disabled, the actual width is determined by rounding the supplied width to the
 * nearest integer. (If the rounding results in the value 0, it is as if the line width were 1.)
 * If | delta(x) | >= | delta(y) | , i pixels are filled in each column that is rasterized, where i is the rounded value
 * of width. Otherwise, i pixels are filled in each row that is rasterized.
 *
 * If antialiasing is enabled, line rasterization produces a fragment for each pixel square that
 * intersects the region lying within the rectangle having width equal to the current line width,
 * length equal to the actual length of the line, and centered on the mathematical line segment. The
 * coverage value for each fragment is the window coordinate area of the intersection of the rectangular
 * region with the corresponding pixel square. This value is saved and used in the final rasterization step.
 *
 * Not all widths can be supported when line antialiasing is enabled. If an unsupported width is requested,
 * the nearest supported width is used. Only width 1 is guaranteed to be supported; others depend on the
 * implementation. Likewise, there is a range for aliased line widths as well. To query the range of
 * supported widths and the size difference between supported widths within the range, call
 * glGetIntegerv()
 * with arguments <CODE>GL_ALIASED_LINE_WIDTH_RANGE</CODE>, <CODE>GL_SMOOTH_LINE_WIDTH_RANGE</CODE>,
 * <CODE>GL_SMOOTH_LINE_WIDTH_GRANULARITY</CODE>.
 *
 *  @param width Specifies the width of rasterized lines. The initial value is 1.
 *
 * @note
 * Nonantialiased line width may be clamped to an implementation-dependent maximum.
 * Call glGetIntegerv() with <CODE>GL_ALIASED_LINE_WIDTH_RANGE</CODE> to determine the maximum width.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_VALUE</CODE></TD>
<TD><I><c>width</c></I> is less than or equal to 0.</TD>
</TR>
</TABLE>
 *
 * @sa glEnable, glGetIntegerv
 *
 */

GLAPI void APIENTRY glLineWidth( GLfloat width )
{
    JS_APIENTRY_TIMER( glLineWidth );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );
#ifndef JS_NO_ERROR_CHECK
    if ( width <= 0.f )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    LContext->LineWidth = width;
    LContext->needValidate |= PSGL_VALIDATE_LINE_RASTER;
}

/**
 * @short Set the scale and units used to calculate depth values
 *
 * glPolygonOffset() sets the scale and units used to calculate depth values.
 *
 *  When <CODE>GL_POLYGON_OFFSET_FILL</CODE> is enabled, each fragment&rsquo;s depth value is offset after it is
 * interpolated from the depth values of the appropriate vertices. The value of the offset is
 * <i>m</i> &times; <I><c>factor</c></I> + r &times; <I><c>units</c></I>, where <i>m</i> is a measurement of the change in depth relative to the
 * screen area of the polygon, and r is the smallest value that is guaranteed to produce a
 * resolvable offset for a given implementation. The offset is added before the depth test
 * is performed and before the value is written into the depth buffer.
 *
 * glPolygonOffset() is useful for applying decals to surfaces.
 *
 * The initial values of <I><c>factor</c></I> and <I><c>units</c></I> are both 0.
 *
 * @param factor Specifies a scale factor that is used to create a variable depth offset
 * for each polygon.
 * @param units Is multiplied by an implementation-specific value to create a constant depth offset.
 *
 * @sa glDepthFunc, glEnable
 */

GLAPI void APIENTRY glPolygonOffset( GLfloat factor, GLfloat units )
{
    JS_APIENTRY_TIMER( glPolygonOffset );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    LContext->PolygonOffsetFactor = factor;
    LContext->PolygonOffsetUnits = units;
    LContext->needValidate |= PSGL_VALIDATE_POLYGON_OFFSET;
}

/**
 *
 *  @short Select flat or smooth shading
 *
 *  GL primitives can have either flat or smooth shading. Smooth shading, the default,
 *  causes the computed colors of vertices to be interpolated as the primitive is rasterized,
 *  typically assigning different colors to each resulting pixel fragment. Flat shading
 *  selects the computed color of just one vertex and assigns it to all the pixel fragments
 *  generated by rasterizing a single primitive. In either case, the computed color of a
 *  vertex is the result of lighting if lighting is enabled, or it is the current color
 *  at the time the vertex was specified if lighting is disabled.
 *
 *  Flat and smooth shading are indistinguishable for points. Starting at the beginning
 *  of the vertex array and counting vertices and primitives from 1, the GL gives each
 *  flat-shaded line segment <I>i</I> the computed color of vertex <I>i</I> + 1, its second vertex.
 *  Counting similarly from 1, the GL gives each flat-shaded polygon the computed
 *  color of vertex <I>i</I> + 2, which is the last vertex to specify the polygon.
 *
 *  Flat and smooth shading are specified by glShadeModel() with mode set
 *  to <CODE>GL_FLAT</CODE> and <CODE>GL_SMOOTH</CODE>, respectively.
 *
 *  @param mode Specifies a symbolic value representing a shading technique.
 *  Accepted values are <CODE>GL_FLAT</CODE> and <CODE>GL_SMOOTH</CODE>.
 *  The initial value is <CODE>GL_SMOOTH</CODE>.
 *
 *  @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_INVALID_ENUM</CODE></TD>
<TD><I><c>mode</c></I> is any value other than <CODE>GL_FLAT</CODE> or <CODE>GL_SMOOTH</CODE>.</TD>
</TR>
</TABLE>
 *
 *  @sa glColor4f, glColorPointer, glLightfv, glLightModelfv, glMaterialfv
 */

GLAPI void APIENTRY glShadeModel( GLenum mode )
{
    JS_APIENTRY_TIMER( glShadeModel );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

#ifndef JS_NO_ERROR_CHECK
    if ( !_jsIsShadeModel( mode ) )
    {
        _jsSetError( GL_INVALID_ENUM );
        return;
    }
#endif

    LContext->ShadeModel = mode;
    LContext->needValidate |= PSGL_VALIDATE_SHADE_MODEL;
}

/**
 *
 * @short Specify mask to modify multisampled pixel fragments
 *
 * glSampleCoverage() defines a mask to modify the coverage of multisampled pixel fragments.
 * This capability is used for antialiased screen-door transparency and smooth transitions
 * between two renderings of an object (often for level-of-detail management in simulation systems).
 *
 * When multisampling is enabled (see glEnable() with argument <CODE>GL_MULTISAMPLE</CODE>),
 * a "fragment mask" is computed for each fragment generated by a primitive.
 * This mask reflects the amount of the pixel covered by the fragment, and
 * determines the frame buffer samples that may be affected by the fragment.
 *
 * If conversion of alpha values to masks is enabled (glEnable() with argument
 * <CODE>GL_SAMPLE_ALPHA_TO_MASK</CODE>), the fragment alpha value is used to generate
 * a temporary modification mask which is then ANDed with the fragment mask.
 * One way to interpret this is as a form of dithering: a multivalued alpha
 * (coverage or opacity) for the whole fragment is converted to simple binary
 * values of coverage at many locations (the samples).
 *
 * After conversion of alpha values to masks, if replacement of alpha values
 * is enabled (glEnable() with argument <CODE>GL_SAMPLE_ALPHA_TO_ONE</CODE>), the fragment&rsquo;s
 * alpha is set to the maximum allowable value.
 *
 * Finally, if fragment mask modification is enabled (glEnable() with argument
 * <CODE>GL_SAMPLE_MASK</CODE>), glSampleCoverage() defines an additional modification mask.
 * <I><c>value</c></I> is used to generate a modification mask in much the same way alpha was used above.
 * If <I><c>invert</c></I> is <CODE>GL_TRUE</CODE>, then the modification mask specified by <I><c>value</c></I> will be inverted.
 * The final modification mask will then be ANDed with the fragment mask resulting
 * from the previous steps. This can be viewed as an "override" control that
 * selectively fades the effects of multisampled fragments.
 *
 * Note that <CODE>glSampleCoverage(value, GL_TRUE)</CODE> is not necessarily equivalent
 * to <CODE>glSampleCoverage(1.0 - value, GL_FALSE)</CODE>; due to round-off and other issues,
 * complementing the coverage will not necessarily yield an inverted modification mask.
 *
 * The initial value of <I><c>value</c></I> is 1.  The initial value of <I><c>invert</c></I> is <CODE>GL_FALSE</CODE>.
 * @param value Specifies the coverage of the modification mask.
 * The value is clamped to the range [0, 1], where 0 represents no coverage and 1 full coverage.
 * @param invert Specifies whether the modification mask implied by value is inverted or not.
 *
 * @sa glEnable
 *
 */

GLAPI void APIENTRY glSampleCoverage( GLclampf value, GLboolean invert )
{
    JS_APIENTRY_TIMER( glSampleCoverage );

    PSGLcontext*	LContext = _CurrentContext;
    JS_ASSERT( LContext );

    LContext->SampleCoverageValue = _jsClampf( value );
    LContext->SampleCoverageInvert = invert;
    LContext->needValidate |= PSGL_VALIDATE_MULTISAMPLING;
}

/** @} Rasterizer */
