/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: export_c.cc
 * Description:
 *
 */

#include "cell/resc.h"
#include "resc_internal.h"

#define RESC_DEBUG_PRINT_ENTERING(...) RESC_DEBUG_PRINTF2("Enter L%4d %s \n", __LINE__, __FUNCTION__)

static cell::Resc::CCellRescInternal s_rescInternalInstance;  //E Make instance as static global.

// ============================================================================
//E Wrappers for exporting as C functions.
// ============================================================================

int32_t cellRescInit(const CellRescInitConfig* const initConfig)
{
	return s_rescInternalInstance.Init(initConfig);
}

void cellRescExit(void)
{
	s_rescInternalInstance.Exit();
}

int32_t cellRescSetDsts(const CellRescBufferMode dstsMode, const CellRescDsts* const dsts)
{
	return s_rescInternalInstance.SetDsts(dstsMode, dsts);
}

int32_t cellRescSetDisplayMode(const CellRescBufferMode displayMode)
{
	return s_rescInternalInstance.SetDisplayMode(displayMode);
}

int32_t cellRescGetNumColorBuffers(const CellRescBufferMode dstMode, const CellRescPalTemporalMode palTemporalMode, uint32_t reserved)
{
	return s_rescInternalInstance.GetNumColorBuffers(dstMode, palTemporalMode, reserved);
}

int32_t cellRescGetBufferSize(int32_t* const colorBuffers, 
							  int32_t* const vertexArray, 
							  int32_t* const fragmentShader)
{
	return s_rescInternalInstance.GetBufferSize(colorBuffers, vertexArray, fragmentShader);
}

int32_t cellRescSetBufferAddress(const void* const colorBuffers, 
								 const void* const vertexArray, 
								 const void* const fragmentShader)
{
	return s_rescInternalInstance.SetBufferAddress(colorBuffers, vertexArray, fragmentShader);
}

int32_t cellRescSetSrc(const int32_t idx, const CellRescSrc* const src)
{
	return s_rescInternalInstance.SetSrc(idx, src);
}

int32_t cellRescSetConvertAndFlip(CellGcmContextData* con, const int32_t idx)
{
	return s_rescInternalInstance.SetConvertAndFlip(con, idx);
}

int32_t cellRescSetPalInterpolateDropFlexRatio(const float ratio)
{
	return s_rescInternalInstance.SetPalInterpolateDropFlexRatio(ratio);
}

void cellRescSetWaitFlip(CellGcmContextData* con)
{
	s_rescInternalInstance.SetWaitFlip(con);
}

system_time_t cellRescGetLastFlipTime(void)
{
	return s_rescInternalInstance.GetLastFlipTime();
}

void cellRescResetFlipStatus(void)
{
	s_rescInternalInstance.ResetFlipStatus();
}

uint32_t cellRescGetFlipStatus(void)
{
	return s_rescInternalInstance.GetFlipStatus();
}

int32_t cellRescGetRegisterCount(void)
{
	return s_rescInternalInstance.GetRegisterCount();
}

void cellRescSetRegisterCount(int32_t regCount)
{
	s_rescInternalInstance.SetRegisterCount(regCount);
}

int32_t cellRescCreateInterlaceTable(void* ea, const float srcH, const CellRescTableElement depth, const int length)
{
	return s_rescInternalInstance.CreateInterlaceTable(ea, srcH, depth, length);
}

int32_t cellRescAdjustAspectRatio(const float horizontal, const float vertical)
{
	return s_rescInternalInstance.AdjustAspectRatio(horizontal, vertical);
}

void cellRescSetVBlankHandler(const CellRescHandler handler)
{
	return s_rescInternalInstance.SetVBlankHandler(handler);
}

void cellRescSetFlipHandler(const CellRescHandler handler)
{
	return s_rescInternalInstance.SetFlipHandler(handler);
}

int32_t cellRescGcmSurface2RescSrc(const CellGcmSurface* const gcmSurface, 
								   CellRescSrc* const rescSrc)
{
	return s_rescInternalInstance.GcmSurface2RescSrc(gcmSurface, rescSrc);
}

int32_t cellRescVideoOutResolutionId2RescBufferMode(const CellVideoOutResolutionId resolutionId, 
													CellRescBufferMode* const bufferMode)
{
	return s_rescInternalInstance.VideoOutResolutionId2RescBufferMode(resolutionId, bufferMode);
}

// ============================================================================

