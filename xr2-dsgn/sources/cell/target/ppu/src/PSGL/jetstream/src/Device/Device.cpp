/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Main device functions file.
 */

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <string.h>
#include <PSGL/DPM2.h>
#include <sysutil/sysutil_sysparam.h>

PSGLdevice *_CurrentDevice = NULL;

void _jsDeviceInit( PSGLinitOptions* options )
{
    _jsPlatformDeviceInit( options );
}

void _jsDeviceExit()
{
    _jsPlatformDeviceExit();
}

static const PSGLdeviceParameters defaultParameters =
    {
        enable: 0,
        colorFormat: GL_ARGB_SCE,
        depthFormat: GL_DEPTH_COMPONENT24,
        multisamplingMode: GL_MULTISAMPLING_NONE_SCE,
        TVStandard: PSGL_TV_STANDARD_NONE,
        connector: PSGL_DEVICE_CONNECTOR_NONE,
        bufferingMode: PSGL_BUFFERING_MODE_DOUBLE,
        width: 0,
        height: 0,
        renderWidth: 0,
        renderHeight: 0,
        rescRatioMode: RESC_RATIO_MODE_LETTERBOX,
        rescPalTemporalMode: RESC_PAL_TEMPORAL_MODE_50_NONE,
        rescInterlaceMode: RESC_INTERLACE_MODE_NORMAL_BILINEAR,
        horizontalScale: 1.0f,
        verticalScale: 1.0f
    };


#if	(defined(JS_DPM) && defined(JS_USE_NV))
// shadow parameters used for DPM save/restore
static PSGLdeviceParameters	shadowJsDeviceParameters;

PSGLdeviceParameters * _jsShadowDeviceParameters()
{
    return &shadowJsDeviceParameters;
}

#endif

#include  <stdio.h>

/**
 * @addtogroup PSGL
 *
 * @{
 */

/**
 * @short Creates a new device, choosing a video mode automatically.
 *
 * Creates a new PSGL device and initializes it, choosing a video mode automatically by calling cellVideoOutGetState()
 * and cellVideoOutGetResolution(). (See the <i>Video Configuration Utility Reference</i>.) The device is created using double-buffering.
 *
 * Use psglGetDeviceDimensions() to retrieve the dimensions of the created device.
 *
 * @param colorFormat The format of the color buffer. Must be one of <CODE>GL_ARGB_SCE</CODE> or
 *                    <CODE>GL_RGBA16F_ARB</CODE>. Note that anti-aliasing is disabled with GL_RGBA16F_ARB.
 * @param depthFormat The format of the depth buffer. Must be one of <CODE>GL_NONE</CODE> (to disable depth buffer), 
 *                    <CODE>GL_DEPTH_COMPONENT16</CODE> (stencil buffers are implicitly turned off when this is used), or
 *                    <CODE>GL_DEPTH_COMPONENT24</CODE>
 * @param multisamplingMode The multisampling (anti-aliasing) mode. Must be <CODE>GL_MULTISAMPLING_NONE_SCE</CODE>
 *                          to disable anti-aliasing, or one of <CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE>,
 *                          <CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE>, or <CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE>
 *
 * @return Pointer to the newly created device, or NULL in case of an error
 *
@note
When using HDMI, if the display is turned off or the cable is disconnected, psglCreateDeviceAuto() will continue to poll
for display information, which appears to hang the application. After the display is turned on and reconnected, the API continues. If 
you require background start up processing, regardless of the display status, it should be performed before calling psglCreateDeviceAuto().

 * @note
 * For more information about creating devices using PSGL, refer to the <i>PlayStation&reg; Graphics Library Programming Guide</i>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>psglCreateDeviceAuto() failed to allocate memory for a PSGL device.</TD>
</TR>
</TABLE>
 *
 * @sa psglDestroyDevice, psglCreateDeviceExtended, psglGetDeviceDimensions
 */
PSGL_EXPORT PSGLdevice*	psglCreateDeviceAuto( GLenum colorFormat, GLenum depthFormat, GLenum multisamplingMode )
{
    PSGLdeviceParameters parameters;
    parameters.enable = PSGL_DEVICE_PARAMETERS_COLOR_FORMAT | PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT | PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE;
    parameters.colorFormat = colorFormat;
    parameters.depthFormat = depthFormat;
    parameters.multisamplingMode = multisamplingMode;
    return psglCreateDeviceExtended( &parameters );
}

/**
 * @short Create a new PSGL device and initialize it
 *
 * Creates a new PSGL device and initializes it.  The device represents a frame buffer.
 *
 * <i><c>parameters</c></i> is a pointer to the following structure:
 *
<TABLE>
<TR>
<TD><CODE>enable</CODE></TD>
<TD>This field is a bitmask representing which field is valid in the structure. If a particular field is not valid,
a default value is chosen instead. This field must be a bitwise OR of a selection of the following:
<UL>
<LI><CODE>PSGL_DEVICE_PARAMETERS_COLOR_FORMAT</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_TV_STANDARD</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_CONNECTOR</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_BUFFERING_MODE</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_RESC_RATIO_MODE</CODE></LI>
<LI><CODE>PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE</CODE></LI>
</UL>
</TD>
</TR>
<TR>
<TD><CODE>colorFormat</CODE></TD>
<TD>The format of the color buffer. Must be one of <CODE>GL_ARGB_SCE</CODE> or <CODE>GL_RGBA16F_ARB</CODE>. Note
that anti-aliasing is disabled with <CODE>GL_RGBA16F_ARB</CODE>. The default value is <CODE>GL_ARGB_SCE</CODE>.  This field is enabled by the
<CODE>PSGL_DEVICE_PARAMETERS_COLOR_FORMAT</CODE> flag.</TD>
</TR>
<TR>
<TD><CODE>depthFormat</CODE></TD>
<TD>The format of the depth buffer. Must be one of <CODE>GL_NONE</CODE> (to disable depth buffer), <CODE>GL_DEPTH_COMPONENT16</CODE>
(stencil buffers are implicitly turned off when this is used), or
<CODE>GL_DEPTH_COMPONENT24</CODE>. The default value is <CODE>GL_DEPTH_COMPONENT24</CODE>. This field is enabled by the <CODE>PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT</CODE>
flag.</TD>
</TR>
<TR>
<TD><CODE>multisamplingMode</CODE></TD>
<TD>Must be <CODE>GL_MULTISAMPLING_NONE_SCE</CODE> to disable anti-aliasing, or one of:
<UL>
<LI><CODE>GL_MULTISAMPLING_2X_DIAGONAL_CENTERED_SCE</CODE></LI>
<LI><CODE>GL_MULTISAMPLING_4X_SQUARE_CENTERED_SCE</CODE></LI>
<LI><CODE>GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE</CODE></LI>
</UL>
The default value is
<CODE>GL_MULTISAMPLING_NONE_SCE</CODE>. This field is enabled by the
<CODE>PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE</CODE> flag.  </TD>
</TR>
<TR>
<TD><CODE>TVStandard</CODE></TD>
<TD>
If enabled by the <CODE>PSGL_DEVICE_PARAMETERS_TV_STANDARD</CODE>, select the video mode. Must be one of the following enumerants:
<UL>
<LI><CODE>PSGL_TV_STANDARD_NONE</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_NTSC_M</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_NTSC_J</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_M</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_B</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_D</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_G</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_H</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_I</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_N</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_PAL_NC</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD480I</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD480P</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD576I</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD576P</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD720P</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD1080I</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_HD1080P</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_1280x720_ON_VESA_1280x768</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_1280x720_ON_VESA_1280x1024</CODE></LI>
<LI><CODE>PSGL_TV_STANDARD_1920x1080_ON_VESA_1920x1200</CODE></LI>
</UL>
The default value is <CODE>PSGL_TV_STANDARD_NONE</CODE>.
</TD>
</TR>
<TR>
<TD><CODE>connector</CODE></TD>
<TD>
This field selects which connector the video is output to. Note that this field
is ignored on the Reference Tool, the valid connector being chosen based on the TV standard.
Must be one of the following enumerants:
<UL>
<LI><CODE>PSGL_DEVICE_CONNECTOR_NONE</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_VGA</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_DVI</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_HDMI</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_COMPOSITE</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_SVIDEO</CODE></LI>
<LI><CODE>PSGL_DEVICE_CONNECTOR_COMPONENT</CODE></LI>
</UL>
The default value is <CODE>PSGL_DEVICE_CONNECTOR_NONE</CODE>.
This field is enabled by the <CODE>PSGL_DEVICE_PARAMETERS_CONNECTOR</CODE> flag.
</TD>
</TR>
<TR>
<TD><CODE>bufferingMode</CODE></TD>
<TD>
Must be one of the following enumerants:
<UL>
<LI><CODE>PSGL_BUFFERING_MODE_SINGLE</CODE></LI>
<LI><CODE>PSGL_BUFFERING_MODE_DOUBLE</CODE></LI>.
<LI><CODE>PSGL_BUFFERING_MODE_TRIPLE</CODE></LI>.
</UL>
The default value is <CODE>PSGL_BUFFERING_MODE_DOUBLE</CODE>
This field is enabled by the <CODE>PSGL_DEVICE_PARAMETERS_BUFFERING_MODE</CODE> flag.
</TD>
</TR>
<TR>
<TD><CODE>width</CODE>, <CODE>height</CODE></TD>
<TD>The width and height of the device, in pixels.  Valid width/height combinations are:
<UL>
<LI><CODE>720x480</CODE></LI>
<LI><CODE>720x576</CODE></LI>
<LI><CODE>1280x720</CODE></LI>
<LI><CODE>1920x1080</CODE></LI>
</UL>

Both height and width are enabled by the single flag <CODE>PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT</CODE>. 

The following width/height
combinations enable a special hardware horizontal scaling mode from the specified resolution to 1920x1080. This mode supports 1080p
hardware upscaling but does not require an additional render target buffer, has little or no performance penalty, and provides 
significant memory savings because only the smaller buffer will be stored and rendered into.
<UL>
<LI><CODE>960x1080</CODE></LI>
<LI><CODE>1280x1080</CODE></LI>
<LI><CODE>1440x1080</CODE></LI>
<LI><CODE>1600x1080</CODE></LI>
</UL>
</TD>
</TR>
<TR>
<TD><CODE>renderWidth</CODE>, <CODE>renderHeight</CODE></TD>
<TD>The width and height of the render buffer, in pixels, for resolution scaling. Both <c>renderHeight</c> and <c>renderWidth</c>
are enabled by the single flag <CODE>PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT</CODE>. The render buffer is scaled to fit the display buffer.
</TD>
</TR>
<TR>
<TD><CODE>rescRatioMode</CODE></TD>
<TD>The resolution conversion rescaling mode for aspect ratio differences when converting between standard and high definition
display modes.  Valid values are:
<ul>
<li><c>RESC_RATIO_MODE_FULLSCREEN</c> &mdash; no rescaling </li>
<li><c>RESC_RATIO_MODE_LETTERBOX</c> &mdash; preserve aspect ratio but fit horizontally with black bands on top and bottom</li>
<li><c>RESC_RATIO_MODE_PANSCAN</c> &mdash; preserve aspect ratio but fit vertically with black bands on left and right</li>
</ul>
</TD>
</TR>
<TR>
<TD><CODE>rescPalTemporalMode</CODE></TD>
<TD>The PAL frame rate conversion modes when using resolution conversion. <c>rescPalTemporalMode</c> only applies to 720x576
50Hz PAL display scanout resolution.  Valid values are:
<ul>
<li><c>RESC_PAL_TEMPORAL_MODE_50_NONE</c> &mdash; no conversion</li>
<li><c>RESC_PAL_TEMPORAL_MODE_60_DROP</c> &mdash; drop frames to convert from 59.94Hz to 50Hz</li>
<li><c>RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE</c> &mdash; interpolate frames to convert from 59.94Hz to 50Hz</li>
<li><c>RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_30_DROP</c> &mdash; Frame drop mode for slow rendering: Frame interpolation mode normally, frame drop mode when running at 29.97Hz or slower (from 59.95Hz to 50Hz conversion)</li>
<li><c>RESC_PAL_TEMPORAL_MODE_60_INTERPOLATE_DROP_FLEXIBLE</c> &mdash; Variable combination mode: Combination of frame interpolation mode and frame drop mode (from 59.95Hz to 50Hz conversion)</li>
</ul>
</TD>
</TR>
<TR>
<TD><CODE>rescInterlaceMode</CODE></TD>
<TD>For the interlace mode, specify whether or not to use the interlace filter for anti-flicker purposes.  Valid values are:
<ul>
<li><c>RESC_INTERLACE_MODE_NORMAL_BILINEAR</c> &mdash; Do not use the interlace filter. Only perform normal scaling of the 1 bilinear sampling (Default)</li>
<li><c>RESC_INTERLACE_MODE_INTERLACE_FILTER</c> &mdash; Use the interlace filter</li>
</ul>
</TD>
</TR>
<TR>
<TD><CODE>horizontalScale</CODE> and <CODE>verticalScale</CODE></TD>
<TD>Horizontal and vertical scaling to adjust for the difference in overscan rates for each SD/HD mode or TV</TD>
</TR>
</TABLE>
 *
 * @param parameters A pointer to a structure containing data describing a rectangular drawing region.
 *
 * @return Pointer to the newly created device, or NULL in case of an error
 *
@note
If a device is created with resolution scaling (resc) (<CODE>PSGL_DEVICE_PARAMETERS_RESC_*</CODE> is enabled in psglCreateDeviceExtended()), <code>GL_VSYNC_SCE</code> is
always enabled, and cannot be disabled with glDisable().

@note
When using HDMI, if the display is turned off or the cable is disconnected, psglCreateDeviceExtended() will continue to poll
for display information, which appears to hang the application. After the display is turned on and reconnected, the API continues. If 
you require background start up processing, regardless of the display status, it should be performed before calling psglCreateDeviceExtended().

@note
 * For more information about creating devices using PSGL, including setting the device video mode and
 * using libresc for resolution scaling, refer to the <i>PlayStation&reg; Graphics Library Programming Guide</i>.
 *
 * @par Errors
Use glGetError() to retrieve the value of the error flag.
<TABLE>
<TR>
<TD><CODE>GL_OUT_OF_MEMORY</CODE></TD>
<TD>psglCreateDeviceExtended() fails to allocate memory for a PSGL device.</TD>
</TR>
</TABLE>
 *
 * @sa psglDestroyDevice, psglCreateDeviceAuto, psglGetDeviceDimensions, psglGetRenderBufferDimensions
 */
PSGL_EXPORT PSGLdevice*	psglCreateDeviceExtended( const PSGLdeviceParameters *parameters )
{
#if	(defined(JS_DPM) && defined(JS_USE_NV))
    memcpy( &shadowJsDeviceParameters, parameters, sizeof( PSGLdeviceParameters ) );
#endif
    PSGLdevice *device = ( PSGLdevice * )jsMalloc( sizeof( PSGLdevice ) + _jsPlatformDeviceSize() );
    if ( !device )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return NULL;
    }
    memset( device, 0, sizeof( PSGLdevice ) + _jsPlatformDeviceSize() );

    // initialize fields
    memcpy( &device->deviceParameters, parameters, sizeof( PSGLdeviceParameters ) );

    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_COLOR_FORMAT ) == 0 )
    {
        device->deviceParameters.colorFormat = defaultParameters.colorFormat;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT ) == 0 )
    {
        device->deviceParameters.depthFormat = defaultParameters.depthFormat;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE ) == 0 )
    {
        device->deviceParameters.multisamplingMode = defaultParameters.multisamplingMode;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_TV_STANDARD ) == 0 )
    {
        device->deviceParameters.TVStandard = defaultParameters.TVStandard;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_CONNECTOR ) == 0 )
    {
        device->deviceParameters.connector = defaultParameters.connector;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_BUFFERING_MODE ) == 0 )
    {
        device->deviceParameters.bufferingMode = defaultParameters.bufferingMode;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT ) == 0 )
    {
        device->deviceParameters.width = defaultParameters.width;
        device->deviceParameters.height = defaultParameters.height;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT ) == 0 )
    {
        device->deviceParameters.renderWidth = defaultParameters.renderWidth;
        device->deviceParameters.renderHeight = defaultParameters.renderHeight;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_RESC_RATIO_MODE ) == 0 )
    {
        device->deviceParameters.rescRatioMode = defaultParameters.rescRatioMode;
    }
    if (( parameters->enable & PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE ) == 0 )
    {
        device->deviceParameters.rescPalTemporalMode = defaultParameters.rescPalTemporalMode;
    }
	if (( parameters->enable & PSGL_DEVICE_PARAMETERS_RESC_INTERLACE_MODE ) == 0 )
	{
		device->deviceParameters.rescInterlaceMode = defaultParameters.rescInterlaceMode;
	}
	if (( parameters->enable & PSGL_DEVICE_PARAMETERS_RESC_ADJUST_ASPECT_RATIO ) == 0 )
	{
		device->deviceParameters.horizontalScale = defaultParameters.horizontalScale;
		device->deviceParameters.verticalScale = defaultParameters.verticalScale;
	}

    device->rasterDriver = NULL;

    // platform-specific initialization
    //  This creates the default framebuffer.
    int result = _jsPlatformCreateDevice( device );
    if ( result < 0 )
    {
        jsFree( device );
        return NULL;
    }
    return device;
}

/**
 * @short Get device aspect ratio
 *
 * Retrieves the display aspect ratio (16/9 or 4/3). It is important to use this value in the perspective transformation
 * (gluPerspectivef() or glOrthof()) instead of the rendering pixel dimensions ratio. For example, when rendering in the
 * 960p &times; 1080p hardware upscaling mode, the pixel aspect ratio is 960/1080, but the display is still 16/9).
 *
 * @param device The device to retrieve parameters from
 * 
 * @return Device aspect ratio
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended, psglGetRenderBufferDimensions
 */
PSGL_EXPORT GLfloat psglGetDeviceAspectRatio( const PSGLdevice * device )
{
  JS_ASSERT( device );

	CellVideoOutState videoState;
	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);

	switch (videoState.displayMode.aspect){
	  case CELL_VIDEO_OUT_ASPECT_4_3:  return 4.0f/3.0f;
	  case CELL_VIDEO_OUT_ASPECT_16_9: return 16.0f/9.0f;
	};

	return 16.0f/9.0f;
}



/**
 * @short Gets device dimensions.
 *
 * Retrieves the device dimensions from a PSGL device. This is particularly useful to query the size of a PSGL device that was created with psglCreateDeviceAuto().
 *
 * @param device the device to retrieve parameters from
 * @param width  a pointer to an unsigned int that will hold the width of the device
 * @param height  a pointer to an unsigned int that will hold the height of the device
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended, psglGetRenderBufferDimensions
 */
PSGL_EXPORT void psglGetDeviceDimensions( const PSGLdevice * device, GLuint *width, GLuint *height )
{
    JS_ASSERT( device );
    JS_ASSERT( width );
    JS_ASSERT( height );
    *width = device->deviceParameters.width;
    *height = device->deviceParameters.height;
}

/**
 * @short Get render buffer dimensions
 *
 * Retrieves the render buffer dimensions from a PSGL device. This is particularly useful to query the size of a PSGL render
buffer that was created with psglCreateDeviceAuto(). When the render buffer is the same size as the device scanout
buffer, you can use psglGetDeviceDimensions() to get the buffer dimensions. However, when using resolution scaling, the render target buffer can be a different size than the display device scanout buffer.  Therefore, in most cases,
it is safer to call this function than to call psglGetDeviceDimensions() because this function always returns the correct
rendering dimensions.
 *
 * @param device the device to retrieve parameters from
 * @param width  a pointer to an unsigned int that will hold the width of the render buffer
 * @param height  a pointer to an unsigned int that will hold the height of the render buffer
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended, psglGetDeviceDimensions
 */
PSGL_EXPORT void psglGetRenderBufferDimensions( const PSGLdevice * device, GLuint *width, GLuint *height )
{
    JS_ASSERT( device );
    JS_ASSERT( width );
    JS_ASSERT( height );
    *width = device->deviceParameters.renderWidth;
    *height = device->deviceParameters.renderHeight;
}

/**
 * @short Destroys a given device.
 *
 * Gracefully destroys a given PSGL device, freeing up its resources.
 *
 * @param device the context to be destroyed
 *
 * @note 
 * This function calls <c>cellGcmUnbindTile()</c>, which invalidates the tile regions 
 * that are currently active, including the frame buffers.  In many cases, this causes 
 * temporary noise in the transistion back to PlayStation&reg;3 vshell.  
 * @par
 * It is best to not call this function before exiting, and instead
 * allow the os/vshell to reset the tile regions and frame buffers, which avoids the
 * quick noisy screen when exiting the game application. 
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended
 */
PSGL_EXPORT void psglDestroyDevice( PSGLdevice *device )
{
    JS_ASSERT( device );
    if ( _CurrentDevice == device ) psglMakeCurrent( NULL, NULL );

    if ( device->rasterDriver ) _jsPlatformRasterExit( device->rasterDriver );
    _jsPlatformDestroyDevice( device );

    jsFree( device );
}

/**
 * @short Attaches a PSGL context to a PSGL device and makes both currently active.
 *
 * psglMakeCurrent() attaches a given PSGL context to a given PSGL device
 * that represents the window and its associated frame buffer.
 * It makes the PSGL device the current rendering device. It makes the
 * PSGL context the current rendering context of the
 * calling thread, replacing any previously active context.
 *
 * @param context the context to be made the current rendering context and attached to the device
 * @param device the device to be made current rendering device
 *
 * @sa psglGetCurrentContext, psglGetCurrentDevice
 */
void PSGL_EXPORT psglMakeCurrent( PSGLcontext *context, PSGLdevice *device )
{
#ifdef JS_DPM
    if ( _jsdpmState )
    {
        _JS_DPM_API_ENTRY_2( psglMakeCurrent, JSDPM_CALL_TYPE_CONTEXT, , 2,
                             context, sizeof( PSGLcontext * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER,
                             device, sizeof( PSGLdevice * ), INPUT_BY_VALUE, DPM_API_ARGUMENT_POINTER );
    }
#endif
    if ( context && device )
    {
        _jsPlatformMakeCurrent( device->platformDevice );
        _CurrentContext = context;
        _CurrentDevice = device;
        if ( !device->rasterDriver )
        {
            device->rasterDriver = _jsPlatformRasterInit();
        }
        _jsAttachContext( device, context );
    }
    else
    {
        _jsPlatformMakeCurrent( NULL );
        _CurrentContext = NULL;
        _CurrentDevice = NULL;
    }
}

/**
 * @short Retrieves the current device.
 *
 * psglGetCurrentDevice() returns a pointer to the current device.
 *
 * @return Pointer to the current device, or NULL if no device is current
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended, psglMakeCurrent
 */
PSGLdevice *psglGetCurrentDevice()
{
    return _CurrentDevice;
}

#ifdef JS_DPM_V2
void psglSwapAfter( void )
{
    _JS_DPM_PSEUDO_ENTRY_0( psglSwapAfter, JSDPM_CALL_TYPE_SWAP, );
}
#endif
/**
 * @short Swap displayed buffer
 *
 * psglSwap() swaps the currently scanned and drawn device buffers.  If
 * <CODE>GL_VSYNC_SCE</CODE> is enabled, the change in the scanned buffer
 * will be deferred until the next vertical retrace interval.
 *
 * @sa psglCreateDeviceAuto, psglCreateDeviceExtended
 */
GLAPI void PSGL_EXPORT psglSwap( void )
{
#ifdef JS_DPM
    if ( _jsdpmState ) { _JS_DPM_API_ENTRY_0( psglSwap, JSDPM_CALL_TYPE_SWAP, ); }

#ifndef JS_DPM_V2
    if ( !_JS_DPM_STATE( SUSPENDED ) ) _jsdpmFrameBarrier();
#endif
#endif // JS_DPM

    if ( _CurrentDevice != NULL )
    {
        _jsPlatformSwapBuffers( _CurrentDevice );
    }
#ifdef JS_DPM_V2
    psglSwapAfter();
#endif
}

/**
 *  @short Register callback function for when a flip is executed.
 *
 *  This function registers the callback function to be executed when the RSX&trade; actually carries out a flip.
 *  It may only be called after psglInit.
 *  In the current specifications, the argument passed to the callback function is always 1.
 *  If NULL is specified for handler, the registered callback can be canceled.
 *
 *  @param handler Callback function to register
 *
 *  @sa cellGcmSetFlipHandler (see <i>libgcm Reference</i>), psglInit
 */
PSGL_EXPORT void psglSetFlipHandler( void( *handler )( const GLuint head ) )
{
    if ( _CurrentDevice != NULL )
    {
        _jsSetFlipHandler( handler, _CurrentDevice );
    }

}

/**
 * @}
 */
