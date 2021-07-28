/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * platform-specific performance-related utilities
 */

#include <PSGL/psgl.h>
#include <PSGL/Utils.h>
#include <PSGL/private.h>
#include <PSGL/DPM2.h>

#include <cell/gcm.h>

#include <sys/sys_time.h>


double _PerformanceResolution = 1.0 / 24.0e6;
jsUint64 _PerformanceStep = 0;

void _jsInitPerformanceCounter()
{
    _PerformanceStep = sys_time_get_timebase_frequency();
    _PerformanceResolution = 1.0 / ( double )_PerformanceStep;
}


jsUint64 _jsGetSystemTime()
{
    return sys_time_get_system_time();
}

// resc is enabled by setting ANY of the resc related device parameters (using the enable mask)
static inline int rescIsEnabled( PSGLdeviceParameters* params )
{
    return params->enable & ( PSGL_DEVICE_PARAMETERS_RESC_RENDER_WIDTH_HEIGHT |
                              PSGL_DEVICE_PARAMETERS_RESC_RATIO_MODE |
                              PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE |
                              PSGL_DEVICE_PARAMETERS_RESC_INTERLACE_MODE |
							                PSGL_DEVICE_PARAMETERS_RESC_ADJUST_ASPECT_RATIO );
}

jsUint64 _jsGetLastFlipTime()
{
	// special case for resc
	PSGLdevice *device = psglGetCurrentDevice();
	if ( rescIsEnabled( &device->deviceParameters ) )
		return cellRescGetLastFlipTime();

	// general case
	return cellGcmGetLastFlipTime();
}

//void _jsSetFlipHandler( void( *handler )( const uint32_t head ) )
//{
//    cellGcmSetFlipHandler( handler );
//}

