/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: util.cc
 * Description:
 *
 */

#define CELL_GCM_DENY_IMPLICIT_ARG
#include <fastmath.h>

#include <sysutil/sysutil_sysparam.h>
#include <sys/time_util.h>
#include <sys/sys_time.h>
#include <cell/gcm.h>

#include <utility>             // std::swap

#include "resc_internal.h"

using namespace cell::Resc::Util;
using namespace cell::Gcm;

namespace cell {
namespace Resc {
namespace Util {

void NullPrintf(...) { return; };

uint8_t RescBufferMode2SysutilResolutionId(CellRescBufferMode bufferMode)
{
	switch (bufferMode) {
	case CELL_RESC_720x480:
	default:
		return CELL_VIDEO_OUT_RESOLUTION_480;
	case CELL_RESC_720x576:
		return CELL_VIDEO_OUT_RESOLUTION_576;
	case CELL_RESC_1280x720:
		return CELL_VIDEO_OUT_RESOLUTION_720;
	case CELL_RESC_1920x1080:
		return CELL_VIDEO_OUT_RESOLUTION_1080;
	}
}


uint8_t RescDstFormat2SysutilFormat(CellRescDstFormat dstFormat)
{
	switch (dstFormat) {
	case CELL_RESC_SURFACE_A8R8G8B8:
	default:
		return CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
	case CELL_RESC_SURFACE_F_W16Z16Y16X16:
		return CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_R16G16B16X16_FLOAT;
	}
}


void GetScreenSize(CellRescBufferMode mode, int32_t *width, int32_t *height)
{	
	switch (mode){
	case CELL_RESC_720x480:
		*width = 720;
		*height = 480;
		break;

	case CELL_RESC_720x576:
		*width = 720;
		*height = 576;
		break;

	case CELL_RESC_1280x720:
		*width = 1280;
		*height = 720;
		break;

	case CELL_RESC_1920x1080:
		*width = 1920;
		*height = 1080;
		break;

	default:
		*width = *height = 0;
		break;
	}
}


uint8_t GcmSurfaceFormat2GcmTextureFormat(uint8_t surfaceFormat, uint8_t surfaceType)
{
	uint8_t result = 0;

	switch(surfaceFormat){
	case CELL_GCM_SURFACE_A8R8G8B8:
		result = CELL_GCM_TEXTURE_A8R8G8B8;
		break;
	case CELL_GCM_SURFACE_F_W16Z16Y16X16:
		result = CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT;
		break;
	default:
		//E shouldn't come here.
		return 0xFF;
	}

	switch(surfaceType){
	case CELL_GCM_SURFACE_PITCH:
		result |= CELL_GCM_TEXTURE_LN;
		break;
	case CELL_GCM_SURFACE_SWIZZLE:
		result |= CELL_GCM_TEXTURE_SZ;
		break;
	default:
		//E shouldn't come here.
		return 0xFF;
	}

	result |= CELL_GCM_TEXTURE_NR;

	return result;
}


int32_t CalculateSurfaceByteSize(CellRescBufferMode mode, CellRescDsts *dsts)
{
	int32_t width, height;

	GetScreenSize(mode, &width, &height);

	return dsts->pitch * roundup(height, dsts->heightAlign);

}


void GlViewPort(CellGcmContextData *con, int x, int y, int w, int h)
{
	float min = 0.f;
	float max = 1.f;
	float scale[4], offset[4];

	scale[0] = (float)w * 0.5f;
	scale[1] = (float)h * -0.5f;
	scale[2] = (max - min) * 0.5f;
	scale[3] = 0.0f;
	offset[0] = (float)x + scale[0];
	offset[1] = (float)h - (float)y + scale[1];
	offset[2] = (max + min) * 0.5f;
	offset[3] = 0.0f;
	
	GcmCmdTypePrefix::cellGcmSetViewport(con, x, y, w, h, min, max, scale, offset);
}


#ifdef CELL_RESC_DEBUG
void PrintDebugTimeSpan(int start, int end, int interval)
{
	static int counter = 0;

	if(++counter > interval){
		counter = 0;
		_printf("resc: coversion took %llu us  (@ %d frame ave.)\n", 
				(GcmSysTypePrefix::cellGcmGetTimeStamp(end) - GcmSysTypePrefix::cellGcmGetTimeStamp(start))/1000, 
				interval);
	}
}


void PrintDebugTime(int idx)
{
	_printf("resc: time stamp(us) id:%d %llu\n", idx, GcmSysTypePrefix::cellGcmGetTimeStamp(idx)/1000);
}
#endif /* CELL_RESC_DEBUG */


static uint64_t s_timeBaseFrequency = 0;
#define CHECK_TIME_U_SEC (1000*1000)
#ifndef SYS_TIMEBASE_MAX
#define SYS_TIMEBASE_MAX (0xFFFFFFFFFFFFFFFFULL)
#endif

void InitTimeBaseFrequency(void)
{
	s_timeBaseFrequency = sys_time_get_timebase_frequency();
}

system_time_t GetSystemTime(void)
{
	uint64_t currentTimeBase;
	SYS_TIMEBASE_GET(currentTimeBase);
	usecond_t usec = 0;
	if (currentTimeBase > (SYS_TIMEBASE_MAX / CHECK_TIME_U_SEC)) {
		uint64_t c = currentTimeBase / (SYS_TIMEBASE_MAX / CHECK_TIME_U_SEC);
		currentTimeBase -= c * (SYS_TIMEBASE_MAX / CHECK_TIME_U_SEC);
		usec = (((SYS_TIMEBASE_MAX / CHECK_TIME_U_SEC) * CHECK_TIME_U_SEC) / s_timeBaseFrequency) * c;
	}
	usec += (currentTimeBase * CHECK_TIME_U_SEC) / s_timeBaseFrequency;

	return usec;
}

#define SEVIRITY 80.f

static void blackman(float window[])
{
	const float x0 = ((1.f * 2.f*M_PI)/5.f) - M_PI;
	const float x1 = ((2.f * 2.f*M_PI)/5.f) - M_PI;
	const float x2 = ((3.f * 2.f*M_PI)/5.f) - M_PI;
	const float x3 = ((4.f * 2.f*M_PI)/5.f) - M_PI;

	const float a0 = 0.42f + (0.50f * cosf(x0)) + (0.08f * cosf(2.f*x0));
	const float a1 = 0.42f + (0.50f * cosf(x1)) + (0.08f * cosf(2.f*x1));
	const float a2 = 0.42f + (0.50f * cosf(x2)) + (0.08f * cosf(2.f*x2));
	const float a3 = 0.42f + (0.50f * cosf(x3)) + (0.08f * cosf(2.f*x3));

	window[0]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.f*a0);
	window[1]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.f*a1);
	window[2]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.f*a2);
	window[3]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.f*a3);
}


int32_t CreateInterlaceTable(void* ea, const float srcH, const float dstH, const CellRescTableElement depth, const int length)
{
	float phi[4], transient[4], *buf32 = (float*)ea;
	float y_fraction;
	float bandwidth = 0.5f / (srcH / dstH);
	float phi_b     = 2.f * M_PI * bandwidth;
	float window[4];
	uint16_t *buf16 = (uint16_t*)ea;

	blackman(window);

	for(int i=0; i<length; i++) {
		y_fraction = (float)i/(float)length;

		phi[0] = phi_b * (-1.5f - y_fraction);
		phi[1] = phi_b * (-0.5f - y_fraction);
		phi[2] = phi_b * ( 0.5f - y_fraction);
		phi[3] = phi_b * ( 1.5f - y_fraction);
		
		transient[0] = (fabsf(phi[0]) > 1E-10) ? (sinf(phi[0]) / phi[0] * window[0]) : window[0];
		transient[1] = (fabsf(phi[1]) > 1E-10) ? (sinf(phi[1]) / phi[1] * window[1]) : window[1];
		transient[2] = (fabsf(phi[2]) > 1E-10) ? (sinf(phi[2]) / phi[2] * window[2]) : window[2];
		transient[3] = (fabsf(phi[3]) > 1E-10) ? (sinf(phi[3]) / phi[3] * window[3]) : window[3];

		float total4 = transient[0] + transient[1] + transient[2] + transient[3];

		if(depth == CELL_RESC_ELEMENT_HALF){
			buf16[0] = Util::FloatToHalf(transient[0] / total4);
			buf16[1] = Util::FloatToHalf(transient[1] / total4);
			buf16[2] = Util::FloatToHalf(transient[2] / total4);
			buf16[3] = Util::FloatToHalf(transient[3] / total4);
			buf16 += 4;
		} else {
			buf32[0] = transient[0] / total4;
			buf32[1] = transient[1] / total4;
			buf32[2] = transient[2] / total4;
			buf32[3] = transient[3] / total4;
			buf32 += 4;
		}
	}
	return CELL_OK;
}


uint16_t FloatToHalf(const float val)
{
	uint8_t *tmp = (uint8_t*)&val;
	uint32_t bits = ((uint32_t)tmp[0] << 24) | ((uint32_t)tmp[1] << 16) | ((uint32_t)tmp[2] << 8) |(uint32_t)tmp[3];

	if (bits == 0) {
		return 0;
	}
	int32_t e = ((bits & 0x7f800000) >> 23) - 127 + 15;
	if (e < 0) {
		return 0;
	}
	else if (e > 31) {
		e = 31;
	}
	uint32_t s = bits & 0x80000000;
	uint32_t m = bits & 0x007fffff;

	return ((s >> 16) & 0x8000) | ((e << 10) & 0x7c00) | ((m >> 13) & 0x03ff);
}


}; /* namespace Util */
}; /* namespace Resc */
}; /* namespace cell */

