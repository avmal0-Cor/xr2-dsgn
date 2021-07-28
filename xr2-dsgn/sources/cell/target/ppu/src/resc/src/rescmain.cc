/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: rescmain.cc
 * Description:
 *
 */

#define CELL_GCM_DENY_IMPLICIT_ARG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <sys/timer.h>
#include <sys/sys_time.h>
#include <cell/error.h>
#include <sysutil/sysutil_sysparam.h>
#include <fastmath.h>
#include <Cg/cg.h>

#include <utility>             // std::swap

#include "cell/resc.h"         //E this includes local one, not in /usr/local/cell
#include "resc_internal.h"

#define TRY_FAIL_RETURN(func)  {int32_t ret; if((ret = (func)) != CELL_OK) \
  { RESC_DEBUG_PRINTF("resc %s line %d failed %d\n", __FILE__, __LINE__, ret); return ret; }}

using namespace cell::Resc;
using namespace cell::Resc::Shader;
using namespace cell::Gcm;

// ============================================================================

extern struct _CGprogram _binary_RescBilinear_vpo_start;
extern struct _CGprogram _binary_RescBilinear_fpo_start;
extern struct _CGprogram _binary_RescBilinearPal_fpo_start;
extern struct _CGprogram _binary_RescInterlaceFilter_fpo_start;
extern struct _CGprogram _binary_RescInterlaceFilterPal_fpo_start;

static const char *s_paramName[RESC_PARAM_NUM] =
{
	"iPosition", 
	"iTexcoord", 
	"iTexcoordNoRatio", 
	"samplerSrc0", 
	"samplerSrc1", 
	"samplerSrc2", 
	"samplerSrc3", 
	"samplerSrc4", 
	"samplerSrc5", 
	"samplerSrc6", 
	"samplerSrc7", 
	"samplerInterlace", 
	"samplerPalPrev", 
	"samplerPalNow", 
	"tValue", 
};

namespace cell {
namespace Resc {

CellRescHandler CCellRescInternal::s_applicationVBlankHandler = NULL;
CellRescHandler CCellRescInternal::s_applicationFlipHandler = NULL;
CCellRescInternal* CCellRescInternal::s_rescInternal = NULL;

// ============================================================================
//E Internal main class's methods (ctor/dtor and public)
// ============================================================================

CCellRescInternal::CCellRescInternal() :
	m_pCVertexShader(NULL),
	m_pCFragmentShader(NULL),
	m_head(0)
{
	if (s_rescInternal) { // singleton.
		_printf("libresc: Cannot make 2nd instance\n");
		while(1);
	}

	s_rescInternal = this;
	
	for(int i=0; i<RESC_SHADER_NUM; i++) {
		m_pCFragmentShaderArray[i] = NULL;
	}

	m_isDummyFlipped           = true;
	m_lastDummyFlip            = 0;

	Util::InitTimeBaseFrequency();
}


CCellRescInternal::~CCellRescInternal()
{
	s_rescInternal = NULL;
}


int32_t CCellRescInternal::Init(const CellRescInitConfig* const initConfig)
{
	if(m_bInitialized) return CELL_RESC_ERROR_REINITIALIZED;
	if(initConfig == NULL) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if(InternalVersion(initConfig) == -1) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if(!CheckInitConfig(initConfig)) return CELL_RESC_ERROR_BAD_ARGUMENT;

	InitMembers();

	//E 0 clear and save the initConfig structure.
	//E initConfig->size was checked above.
	_memset(&m_initConfig, 0, sizeof(CellRescInitConfig));
	_memcpy(&m_initConfig, initConfig, initConfig->size);

	//E Creating shader class instance. (do only in first time calling)
	if (!m_pCVertexShader) {
		m_pCVertexShader = (CVertexShader *)CShaderBase::Create(CG_PROGRAM_VERTEX, &_binary_RescBilinear_vpo_start);
		if(!m_pCVertexShader) return CELL_RESC_ERROR_LESS_MEMORY;
	}
	if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR]) {
		m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR] = (CFragmentShader *)CShaderBase::Create(CG_PROGRAM_FRAGMENT, &_binary_RescBilinear_fpo_start);
		if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR]) return CELL_RESC_ERROR_LESS_MEMORY;
	}
	if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR_PAL]) {
		m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR_PAL] = (CFragmentShader *)CShaderBase::Create(CG_PROGRAM_FRAGMENT, &_binary_RescBilinearPal_fpo_start);
		if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR_PAL]) return CELL_RESC_ERROR_LESS_MEMORY;
	}
	if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE]) {
		m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE] = (CFragmentShader *)CShaderBase::Create(CG_PROGRAM_FRAGMENT, &_binary_RescInterlaceFilter_fpo_start);
		if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE]) return CELL_RESC_ERROR_LESS_MEMORY;
	}
	if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE_PAL]) {
		m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE_PAL] = (CFragmentShader *)CShaderBase::Create(CG_PROGRAM_FRAGMENT, &_binary_RescInterlaceFilterPal_fpo_start);
		if (!m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE_PAL]) return CELL_RESC_ERROR_LESS_MEMORY;
	}

	RESC_ASSERT(m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR]->GetUcodeSize()      == 0x10);
	RESC_ASSERT(m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR_PAL]->GetUcodeSize()  == 0x60);
	RESC_ASSERT(m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE]->GetUcodeSize()     <= 0x200);
	RESC_ASSERT(m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE_PAL]->GetUcodeSize() <= 0x280);

	m_pCFragmentShader = m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR];

	ResolveVertexParamBindingsByName();

	m_bInitialized = true;
	
	return CELL_OK;
}


void CCellRescInternal::Exit(void)
{
	if(!m_bInitialized) return;
	
	if (IsPalTemporal()){
		GcmSysTypePrefix::cellGcmSetSecondVFrequency(CELL_GCM_DISPLAY_FREQUENCY_DISABLE);
		GcmSysTypePrefix::cellGcmSetVBlankHandler(NULL);
		GcmSysTypePrefix::cellGcmSetSecondVHandler(NULL);

		if (IsPalInterpolate()){
			int ret = ExitSystemResource();
			if(ret != CELL_OK)
				RESC_DEBUG_PRINTF("failed to clean up system resources.. continue. 0x%x\n", ret);
		}
	}

	m_bInitialized = false;
}


int32_t CCellRescInternal::SetDsts(const CellRescBufferMode displayMode, const CellRescDsts* const dsts)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(!dsts) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if((displayMode != CELL_RESC_720x480) && (displayMode != CELL_RESC_720x576) && 
	   (displayMode != CELL_RESC_1280x720) && (displayMode != CELL_RESC_1920x1080))
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	
	m_rescDsts[GetRescDestsIndex(displayMode)] = *dsts;

	return CELL_OK;
}


int32_t CCellRescInternal::SetDisplayMode(const CellRescBufferMode bufferMode)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(!(m_initConfig.supportModes & bufferMode)) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if((bufferMode != CELL_RESC_720x480) && (bufferMode != CELL_RESC_720x576) && 
	   (bufferMode != CELL_RESC_1280x720) && (bufferMode != CELL_RESC_1920x1080))
		return CELL_RESC_ERROR_BAD_ARGUMENT;

	m_dstMode = bufferMode;

	if((IsPalInterpolate() || IsPalDrop()) && m_initConfig.flipMode==CELL_RESC_DISPLAY_HSYNC)
		return CELL_RESC_ERROR_BAD_COMBINATION;
	if(IsPal60Hsync() && m_initConfig.flipMode==CELL_RESC_DISPLAY_VSYNC)
		return CELL_RESC_ERROR_BAD_COMBINATION;

	m_pRescDsts = &m_rescDsts[GetRescDestsIndex(bufferMode)];

	//E Setup m_dstWidth, m_dstHeight m_dstPitch and m_bufInterval
	Util::GetScreenSize(IN m_dstMode, OUT &m_dstWidth, OUT &m_dstHeight);

	m_dstPitch = m_pRescDsts->pitch;
	m_dstBufInterval = m_dstPitch * roundup(m_dstHeight, m_pRescDsts->heightAlign);

	if (IsPalInterpolate()) {
		if(IsInterlace()){
			m_pCFragmentShader = m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE_PAL];
		} else {
			m_pCFragmentShader = m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR_PAL];
		}
	} else {
		if(IsInterlace()){
			m_pCFragmentShader = m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_INTERLACE];
		} else {
			m_pCFragmentShader = m_pCFragmentShaderArray[RESC_SHADER_DEFAULT_BILINEAR];
		}
	}

	uint8_t sysutilResolutionId = Util::RescBufferMode2SysutilResolutionId(m_dstMode);
	uint8_t sysutilFormat       = Util::RescDstFormat2SysutilFormat((CellRescDstFormat)m_pRescDsts->format);

	CellVideoOutConfiguration videocfg;

	_memset(&videocfg, 0, sizeof(videocfg));
	videocfg.resolutionId = sysutilResolutionId;
	videocfg.format       = sysutilFormat;
	videocfg.aspect       = CELL_VIDEO_OUT_ASPECT_AUTO;
	videocfg.pitch        = m_dstPitch;

	cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &videocfg, NULL, 0);

	if (IsPalInterpolate()){
		TRY_FAIL_RETURN( InitSystemResource() );
		InitLabels();

		GcmSysTypePrefix::cellGcmSetSecondVFrequency(CELL_GCM_DISPLAY_FREQUENCY_59_94HZ);
		GcmSysTypePrefix::cellGcmSetVBlankHandler(CCellRescInternal::IntrHandler50);
		GcmSysTypePrefix::cellGcmSetSecondVHandler(CCellRescInternal::IntrHandler60);
		GcmSysTypePrefix::cellGcmSetFlipHandler(NULL);
	} else if (IsPalDrop()){
		InitLabels();

		GcmSysTypePrefix::cellGcmSetSecondVFrequency(CELL_GCM_DISPLAY_FREQUENCY_59_94HZ);
		GcmSysTypePrefix::cellGcmSetVBlankHandler(NULL);
		GcmSysTypePrefix::cellGcmSetSecondVHandler(CCellRescInternal::IntrHandler60Drop);
		GcmSysTypePrefix::cellGcmSetFlipHandler(NULL);
	} else if (IsPal60Hsync()){
		GcmSysTypePrefix::cellGcmSetSecondVFrequency(CELL_GCM_DISPLAY_FREQUENCY_59_94HZ);
		GcmSysTypePrefix::cellGcmSetVBlankHandler(NULL);
	}

	if(s_applicationVBlankHandler) SetVBlankHandler(s_applicationVBlankHandler);
	if(s_applicationFlipHandler)   SetFlipHandler(s_applicationFlipHandler);

	GcmSysTypePrefix::cellGcmSetFlipMode((m_initConfig.flipMode==CELL_RESC_DISPLAY_VSYNC) 
										 ? CELL_GCM_DISPLAY_VSYNC : CELL_GCM_DISPLAY_HSYNC);

	return CELL_OK;
}


int32_t CCellRescInternal::GetBufferSize(int32_t* const colorBuffers, 
										 int32_t* const vertexArray, 
										 int32_t* const fragmentUcode)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;

	int32_t colorBuffersSize, vertexArraySize, fragmentUcodeSize;


	if (m_initConfig.resourcePolicy & CELL_RESC_MINIMUM_VRAM) {
		colorBuffersSize  = m_dstBufInterval * GetNumColorBuffers();
		vertexArraySize   = sizeof(RescVertex_t) * VERTEX_NUMBER_RESERVED;  // now 0x180 fix.
		fragmentUcodeSize = m_pCFragmentShader->GetUcodeSize();
	} else {
		// CELL_RESC_CONSTANT_VRAM
		colorBuffersSize  = CalculateMaxColorBuffersSize();
		vertexArraySize   = sizeof(RescVertex_t) * VERTEX_NUMBER_RESERVED;  // now 0x180 fix.
		fragmentUcodeSize = 0x300;                                          // now 0x300 fix.
	}
	
	if(colorBuffers)   *colorBuffers  = colorBuffersSize;
	if(vertexArray)    *vertexArray   = vertexArraySize;
	if(fragmentUcode)  *fragmentUcode = fragmentUcodeSize;

	return CELL_OK;
}


int32_t CCellRescInternal::SetBufferAddress(const void* const colorBuffers, 
											const void* const vertexArray, 
											const void* const fragmentUcode)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(colorBuffers==NULL || vertexArray==NULL || fragmentUcode==NULL) 
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	if((int)colorBuffers % COLOR_BUFFER_ALIGNMENT ||
	   (int)vertexArray % VERTEX_BUFFER_ALIGNMENT ||
	   (int)fragmentUcode % FRAGMENT_SHADER_ALIGNMENT) return CELL_RESC_ERROR_BAD_ALIGNMENT;

	m_colorBuffersEA  = colorBuffers;
	m_vertexArrayEA   = vertexArray;
	m_fragmentUcodeEA = fragmentUcode;
	
	//E Allocate 2-6 destination buffers on local memory.
	uint32_t dstOffset;
	
	GcmSysTypePrefix::cellGcmAddressToOffset(m_colorBuffersEA, &dstOffset);

	for(int i=0; i<(int)GetNumColorBuffers(); i++){
		m_dstOffsets[i] = dstOffset + i*m_dstBufInterval;
	}

	for(int i=0; i<(int)GetNumColorBuffers(); i++) {
		TRY_FAIL_RETURN( GcmSysTypePrefix::cellGcmSetDisplayBuffer(i, m_dstOffsets[i], m_dstPitch, m_dstWidth, m_dstHeight) );
	};

	//E Prepare vertex buffer
	if(IsTextureNR())
		BuildupVertexBufferNR();

	//E Load current fragment ucode to local memory, and resolve its parameter's name binding.
	m_pCFragmentShader->LoadToLocal(m_fragmentUcodeEA);
	ResolveFragmentParamBindingsByName();

	return CELL_OK;
}


int32_t CCellRescInternal::SetSrc(const int32_t idx, const CellRescSrc* const src)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(idx < 0 || SRC_BUFFER_NUM <= idx || src==NULL) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if(src->width < 1 || 4096 < src->width || src->height < 1 || 4096 < src->height)
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	if(!(((src->format & 0x9F)==CELL_GCM_TEXTURE_A8R8G8B8)||((src->format & 0x9F)==CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT)))
		return CELL_RESC_ERROR_BAD_ARGUMENT;

	m_rescSrc[idx] = *src;

	return CELL_OK;
}


int32_t CCellRescInternal::SetConvertAndFlip(CellGcmContextData* con, const int32_t idx, 
											 int32_t idxPrev __attribute__ ((unused)))
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(idx < 0 || SRC_BUFFER_NUM <= idx) return CELL_RESC_ERROR_BAD_ARGUMENT;

	RESC_GET_TIME_SPAN(2046, 2047, 100);  //E  for library perfomance measuring.(only in debug mode)
	RESC_SET_TIME_STAMP(con, 2046);       //E  release build doesn't use report region.

	if(!IsTextureNR()){
		BuildupVertexBufferUN(idx);
	}

	//E setup GPU internal status
	SetupRsxRenderingStates(con);

	//E set vertex array pointers
	SetupVertexArrays(con);

	//E set surface
	SetupSurfaces(con);

	//E bind texture
	SetupTextures(con, idx);

	//E set shader parameter
	CellRescVBlank targetV60, targetOav, lastV60, lastV50;
	CellRescFrameFeedback feedback;
	float tValue;
	int32_t nUnmarked = 0;
	if (IsPalInterpolate()) {
		RESC_LOCK(A, SYS_NO_TIMEOUT) {
			feedback = m_feedback;
			lastV60  = m_lastV60;
			lastV50  = m_lastV50;
		} RESC_UNLOCK(A)
		RESC_LOCK(B, SYS_NO_TIMEOUT) {
			for(QUEUE1::iterator que_itr = QUEUE1::Begin(); que_itr != QUEUE1::End(); ++que_itr)
			{
				if(!que_itr->m_mark60Intr) ++nUnmarked;
			}
		} RESC_UNLOCK(B)

		CalculateTvalue(&tValue, &targetV60, &targetOav, &lastV60, &lastV50, feedback.interval60, nUnmarked);

		GcmCmdTypePrefix::cellGcmSetTransferLocation(con, CELL_GCM_LOCATION_LOCAL);
		GcmCmdTypePrefix::cellGcmSetFragmentProgramParameter(con, m_pCFragmentShader->GetCGprogram(), 
										   m_cgpTvalue, 
										   &tValue, 
										   m_pCFragmentShader->GetUcodeOffset());
	}

	//E bind shaders
	m_pCVertexShader->Bind(con);
	m_pCFragmentShader->Bind(con);

	//E issue conversion draw
	GcmCmdTypePrefix::cellGcmSetDrawArrays(con, CELL_GCM_PRIMITIVE_QUADS, 0, m_nVertex);
	
	RESC_SET_TIME_STAMP(con, 2047);    //E  for library perfomance measuring.(only in debug mode)
	
	//E set flip  and  swap buffer
	if (IsGcmFlip()) {
		GcmFlipPrefix::cellGcmSetFlip(con, m_bufIdFront);

		m_bufIdFront ^= 1;
	} else if (IsPalDrop()) {
		GcmFlipPrefix::cellGcmSetFlip(con, m_bufIdFront);

		GcmCmdTypePrefix::cellGcmSetWriteBackEndLabel(con, RESC_LABEL_PAL_COMMAND_IDX, (m_commandIdxCaF & 0xFFFFFFFF));
		uint8_t currentFlipLabelId = GetLabelIndexOfPalFlipStatus();
		GcmCmdTypePrefix::cellGcmSetWriteCommandLabel(con, currentFlipLabelId, RESC_DUMMY_FLIP_CLEARED);

		m_commandIdxCaF++;

		uint32_t next = 3 - (m_bufIdFront + m_bufIdFrontPrevDrop);
		m_bufIdFrontPrevDrop = m_bufIdFront;
		m_bufIdFront = next;
	} else {
		int32_t result = CELL_OK;
		uint32_t retryCount = 2;

		while( (retryCount > 0) && (QUEUE1::GetNumElements() >= CELL_GCM_PREPARE_QUEUE_MAX) ) {
			--retryCount;
			sys_timer_usleep(RESC_50HZ_INTERVAL_USEC);  //E Wait a frame to let OavCallback drain queue
		}

		if(retryCount == 0) {
			result = CELL_RESC_ERROR_GCM_FLIP_QUE_FULL;
		} else {
			int32_t flipId = GcmSysTypePrefix::cellGcmSetPrepareFlip(con, m_bufIdFront);
			if(flipId == CELL_GCM_ERROR_FAILURE) {
				//E This should never happen unless there is a GCM bug, because if QUEUE1 has no more than
				//E CELL_GCM_PREPARE_QUEUE_MAX, then  GCM queue should have space.  We don't call the 
				//E setPrepareFlip without making sure QUEUE1 has space
				result = CELL_RESC_ERROR_GCM_FLIP_QUE_FULL;
			} else {
				//E Only add the command to resc queue if cellGcmSetPrepareFlip returned a valid index.
				uint8_t queId = flipId;

				CRescCommand command(m_commandIdxCaF, targetV60.idx, targetOav.idx, m_bufIdFront, queId, false);
				QUEUE1::EnqLock(command, &m_lwmutexB);

				GcmCmdTypePrefix::cellGcmSetWriteBackEndLabel(con, RESC_LABEL_PAL_COMMAND_IDX, (m_commandIdxCaF & 0xFFFFFFFF));
				uint8_t currentFlipLabelId = GetLabelIndexOfPalFlipStatus();
				GcmCmdTypePrefix::cellGcmSetWriteCommandLabel(con, currentFlipLabelId, RESC_DUMMY_FLIP_CLEARED);

				//E draw buffer switching
				m_bufIdFront = (++m_bufIdFront) % 4;
			}
		}

		m_commandIdxCaF++;

		//E mid buffer swapping
		std::swap(m_bufIdPalMidNow, m_bufIdPalMidPrev);
	    
		return result;
	}

	return CELL_OK;
}


int32_t CCellRescInternal::SetPalInterpolateDropFlexRatio(const float ratio)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if(ratio < 0.f || 1.f < ratio) return CELL_RESC_ERROR_BAD_ARGUMENT;

	m_flexRatio = ratio;

	return CELL_OK;
}


int32_t CCellRescInternal::AdjustAspectRatio(const float horizontal, const float vertical)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if((horizontal < 0.5f || 2.f < horizontal) || (vertical < 0.5f || 2.f < vertical)) return CELL_RESC_ERROR_BAD_ARGUMENT;

	m_ratioAdjX = horizontal;
	m_ratioAdjY = vertical;

	//E Being called after SetBufferAddress(), prepare vertex buffer here again.
	if(m_vertexArrayEA){
		if(IsTextureNR())
			BuildupVertexBufferNR();
		else
			m_bNewlyAdjustRatio = true;
	}
	return CELL_OK;
}


void CCellRescInternal::SetVBlankHandler(const CellRescHandler handler)
{
	if (!m_bInitialized || m_dstMode == 0) {
		//E If this function is called before SetDisplayMode, 
		//E handler should be stored and set it properly later in SetDisplayMode.
		s_applicationVBlankHandler = handler;
		return;
	}

	if (IsNotPalTemporal()) {
		GcmSysTypePrefix::cellGcmSetVBlankHandler(handler);
		s_applicationVBlankHandler = NULL;
	} else if(IsPal60Hsync()) {
		GcmSysTypePrefix::cellGcmSetSecondVHandler(handler);
		s_applicationVBlankHandler = NULL;
	} else {
		s_applicationVBlankHandler = handler;
	}
}


void CCellRescInternal::SetFlipHandler(const CellRescHandler handler)
{
	if (!m_bInitialized || m_dstMode == 0) {
		//E If this function is called before SetDisplayMode, 
		//E handler should be stored and set it properly later in SetDisplayMode.
		s_applicationFlipHandler = handler;
		return;
	}

	if (IsGcmFlip()) {
		GcmSysTypePrefix::cellGcmSetFlipHandler(handler);
		s_applicationFlipHandler = NULL;
	} else {
		s_applicationFlipHandler = handler;
	}
}


void CCellRescInternal::SetWaitFlip(CellGcmContextData* con)
{
	if (IsGcmFlip()) {
		GcmCmdTypePrefix::cellGcmSetWaitFlip(con);
	} else {
		uint8_t currentFlipLabelId = GetLabelIndexOfPalFlipStatus();

		GcmCmdTypePrefix::cellGcmSetWaitLabel(con, currentFlipLabelId, RESC_DUMMY_FLIPPED);
	}
}


system_time_t CCellRescInternal::GetLastFlipTime(void)
{
	system_time_t retValue;

	if (IsGcmFlip()) {
		retValue = GcmSysTypePrefix::cellGcmGetLastFlipTime();
	} else {
		retValue = m_lastDummyFlip;
	}

	return retValue;
}


void CCellRescInternal::ResetFlipStatus(void)
{
	if (IsGcmFlip()) {
		GcmSysTypePrefix::cellGcmResetFlipStatus();
	} else {
		m_isDummyFlipped = false;
	}
}


uint32_t CCellRescInternal::GetFlipStatus(void)
{
	uint32_t retValue;

	if (IsGcmFlip()) {
		retValue = GcmSysTypePrefix::cellGcmGetFlipStatus();
	} else {
		retValue = (m_isDummyFlipped ? 0 : 1);
	}

	return retValue;
}


int32_t CCellRescInternal::CreateInterlaceTable(void* ea, const float srcH, const CellRescTableElement depth, const int length)
{
	if(!m_bInitialized) return CELL_RESC_ERROR_NOT_INITIALIZED;
	if( (ea == NULL) ||
		(srcH <= 0.f) ||
		( !(depth == CELL_RESC_ELEMENT_HALF || depth == CELL_RESC_ELEMENT_FLOAT) ) ||
		(length <= 0) ) return CELL_RESC_ERROR_BAD_ARGUMENT;
	if(m_dstHeight == 0)
		return CELL_RESC_ERROR_BAD_COMBINATION;

	float ratioModeCoefficient = (m_initConfig.ratioMode != CELL_RESC_LETTERBOX) ? 1.f : (1.f - 2.f*XY_DELTA_LB);
	float dstH = m_dstHeight * ratioModeCoefficient * m_ratioAdjY;

	if(int32_t retValue = Util::CreateInterlaceTable(ea, srcH, dstH, depth, length) == CELL_OK){
		//E This function always success at the moment.
		m_interlaceTableEA = ea;
		m_interlaceElement = depth;
		m_interlaceTableLength = length;
		return CELL_OK;
	} else {
		return retValue;
	}
}


int32_t CCellRescInternal::GcmSurface2RescSrc(const CellGcmSurface* const gcmSurface, CellRescSrc* const rescSrc)
{
	if(gcmSurface == NULL || rescSrc == NULL)
		return CELL_RESC_ERROR_BAD_ARGUMENT;

	uint8_t textureFormat = Util::GcmSurfaceFormat2GcmTextureFormat(gcmSurface->colorFormat, gcmSurface->type);

	//E This function use MRT[0] buffer.
	_memset(rescSrc, 0, sizeof(CellRescSrc));

	int32_t xW = 1, xH = 1;

	switch(gcmSurface->antialias){
	case CELL_GCM_SURFACE_SQUARE_ROTATED_4:
	case CELL_GCM_SURFACE_SQUARE_CENTERED_4:
		xW = xH = 2;
		break;
	case CELL_GCM_SURFACE_DIAGONAL_CENTERED_2:
		xW = 2;
		break;
	default:
		break;
	}

	rescSrc->format  = textureFormat;
	rescSrc->pitch   = gcmSurface->colorPitch[0];
	rescSrc->width   = gcmSurface->width  * xW;
	rescSrc->height  = gcmSurface->height * xH;
	rescSrc->offset  = gcmSurface->colorOffset[0];

	return CELL_OK;
}


int32_t CCellRescInternal::VideoOutResolutionId2RescBufferMode(const CellVideoOutResolutionId resolutionId, 
															   CellRescBufferMode* const bufferMode)
{
	if(bufferMode == NULL)
		return CELL_RESC_ERROR_BAD_ARGUMENT;

	switch (resolutionId) {
	case CELL_VIDEO_OUT_RESOLUTION_1080:
		*bufferMode = CELL_RESC_1920x1080;
		break;
	case CELL_VIDEO_OUT_RESOLUTION_720:
		*bufferMode = CELL_RESC_1280x720;
		break;
	case CELL_VIDEO_OUT_RESOLUTION_480:
		*bufferMode = CELL_RESC_720x480;
		break;
	case CELL_VIDEO_OUT_RESOLUTION_576:
		*bufferMode = CELL_RESC_720x576;
		break;
	case CELL_VIDEO_OUT_RESOLUTION_1600x1080:
	case CELL_VIDEO_OUT_RESOLUTION_1440x1080:
	case CELL_VIDEO_OUT_RESOLUTION_1280x1080:
	case CELL_VIDEO_OUT_RESOLUTION_960x1080:
	case CELL_VIDEO_OUT_RESOLUTION_UNDEFINED:
	default:
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	return CELL_OK;
}


// ============================================================================
//E Internal main class's methods (private)
// ============================================================================

// ============================================================================
// InitMembers
//J メンバー変数を初期化する。instance スコープと、Init <-> Exit が別なので。
// 
void CCellRescInternal::InitMembers(void)
{
	m_dstMode                  = (CellRescBufferMode)0; // invalid enum value.
	m_interlaceElement         = CELL_RESC_ELEMENT_FLOAT;
	m_colorBuffersEA           = NULL;
	m_vertexArrayEA            = NULL;
	m_fragmentUcodeEA          = NULL;
	m_interlaceTableEA         = NULL;
	m_bufIdFront               = 0;
	m_dstWidth                 = 0;
	m_dstHeight                = 0;
	m_dstPitch                 = 0;
	m_srcWidthInterlace        = 0;
	m_srcHeightInterlace       = 0;
	m_dstBufInterval           = 0;
	m_nVertex                  = 0;
	m_ratioAdjX                = 1.f;
	m_ratioAdjY                = 1.f;
	m_interlaceTableLength     = 32;
	m_bInitialized             = false;
	m_bNewlyAdjustRatio        = false;

	//E PAL related variables
	m_intrThread50             = 0;
	m_lastDummyFlip            = 0;
	m_lastVsync60              = 0;
	m_lastVsync50              = 0;
	m_bufIdFrontPrevDrop       = 2;
	m_bufIdPalMidPrev          = 4;
	m_bufIdPalMidNow           = 5;
	m_cgpTvalue                = 0;
	m_isDummyFlipped           = true;
	m_flexRatio                = 0.f;   // interpolate
	m_commandIdxCaF            = 1;
	m_rcvdCmdIdx               = 0;

	m_lastV60.idx              = 0;
	m_lastV60.time             = Util::GetSystemTime();
	m_lastV50.idx              = 0;
	m_lastV50.time             = Util::GetSystemTime();

	m_feedback.interval60      = 1;

	for(int i=0; i<SRC_BUFFER_NUM; i++) {
		m_rescSrc[i].format = 0;
		m_rescSrc[i].pitch  = 0;
		m_rescSrc[i].width  = 0;
		m_rescSrc[i].height = 0;
		m_rescSrc[i].offset = 0;
	}
	for(int i=0; i<MAX_DST_BUFFER_NUM; i++) {
		m_dstOffsets[i] = 0;
	}
	for(int i=0; i<RESC_PARAM_NUM; i++) {
		m_cgParamIndex[i] = 0xFF;
	}
	{
		m_rescDsts[0].format      = CELL_RESC_SURFACE_A8R8G8B8;
		m_rescDsts[0].pitch       = GcmSysTypePrefix::cellGcmGetTiledPitchSize(720*4);
		m_rescDsts[0].heightAlign = 8;
		m_rescDsts[1].format      = CELL_RESC_SURFACE_A8R8G8B8;
		m_rescDsts[1].pitch       = GcmSysTypePrefix::cellGcmGetTiledPitchSize(720*4);
		m_rescDsts[1].heightAlign = 8;
		m_rescDsts[2].format      = CELL_RESC_SURFACE_A8R8G8B8;
		m_rescDsts[2].pitch       = GcmSysTypePrefix::cellGcmGetTiledPitchSize(1280*4);
		m_rescDsts[2].heightAlign = 8;
		m_rescDsts[3].format      = CELL_RESC_SURFACE_A8R8G8B8;
		m_rescDsts[3].pitch       = GcmSysTypePrefix::cellGcmGetTiledPitchSize(1920*4);
		m_rescDsts[3].heightAlign = 8;
	}
}


bool CCellRescInternal::CheckInitConfig(const CellRescInitConfig* const initConfig)
{
	if( (initConfig->resourcePolicy & ~((uint32_t)0x3)) ||
		(initConfig->supportModes & 0xF) == 0 ||
		(initConfig->ratioMode > 2) ||
		(initConfig->palTemporalMode > 5) )
		return false;

	if( InternalVersion() >= 2 ){
		if(InternalVersion() == 2 && initConfig->interlaceMode > 1) 
			return false; 
	}

	if( InternalVersion() >= 3 ){
		if(initConfig->interlaceMode > 4)
			return false;
		if(initConfig->flipMode > 1)
			return false;
	}

	return true;
}


void CCellRescInternal::InitLabels(void)
{
	//E Initialize GPU labels
	volatile uint32_t* pLabel;
	pLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(RESC_LABEL_PAL_COMMAND_IDX);
	*pLabel = 0;
	pLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(RESC_LABEL_FOR_DUMMY_FLIP0);
	*pLabel = RESC_DUMMY_FLIPPED;
	pLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(RESC_LABEL_FOR_DUMMY_FLIP1);
	*pLabel = RESC_DUMMY_FLIPPED;
}


int32_t CCellRescInternal::InitSystemResource(void)
{
	int ret;

	//E Create 2 light-weight mutex, A and B.
	sys_lwmutex_attribute_t lwmutex_attr;
	sys_lwmutex_attribute_initialize(lwmutex_attr);

	sys_lwmutex_attribute_name_set(lwmutex_attr.name, SYS_NAME_PREFIX_LIBRESC_SPRX "lm0");
	ret = sys_lwmutex_create(&m_lwmutexA, &lwmutex_attr);
	if (ret != CELL_OK) {
		//_printf("sys_lwmutex_create failed: %d\n", ret);
		return ret;
	}

	sys_lwmutex_attribute_name_set(lwmutex_attr.name, SYS_NAME_PREFIX_LIBRESC_SPRX "lm1");
	ret = sys_lwmutex_create(&m_lwmutexB, &lwmutex_attr);
	if (ret != CELL_OK) {
		//_printf("sys_lwmutex_create failed: %d\n", ret);
		return ret;
	}

	//E Create timer object, event port and event queue.
    ret = sys_timer_create(&m_timer);
	if (ret != CELL_OK) {
		//_printf("sys_timer_create failed: %d\n", ret);
		return ret;
	}
    
	ret = sys_event_port_create(&m_eventPort, SYS_EVENT_PORT_LOCAL, 0);
	if (ret != CELL_OK) {
		//_printf("sys_event_port_create failed: %d\n", ret);

		return ret;
	}

    sys_event_queue_attribute_t queue_attr;
	sys_event_queue_attribute_initialize(queue_attr);
	
	sys_event_queue_attribute_name_set(queue_attr.name, SYS_NAME_PREFIX_LIBRESC_SPRX "eq0");
	ret = sys_event_queue_create(&m_eventQueue, &queue_attr, SYS_EVENT_QUEUE_LOCAL, RESC_EVENT_QUEUE_SIZE);
	if (ret != CELL_OK) {
		//_printf("sys_event_queue_create failed: %d\n", ret);
		return ret;
	}

	//E Then, connect the timer and the port to the event queue.
	ret = sys_timer_connect_event_queue(m_timer, m_eventQueue, 
										SYS_TIMER_EVENT_NO_NAME,
										0, 0);
	if (ret != CELL_OK) {
		//_printf("sys_timer_connect_event_queue failed: %d\n", ret);
		return ret;
	}

	ret = sys_event_port_connect_local(m_eventPort, m_eventQueue);
	if (ret != CELL_OK) {
		//_printf("sys_event_port_create failed: %d\n", ret);
		sys_event_port_destroy(m_eventPort);
		
		return ret;
	}

	//E Create thread.
	ret = sys_ppu_thread_create(&m_intrThread50, 
								IntrThread50, 
								NULL, 
								RESC_THREAD_PRIORITY50, 
								RESC_THREAD_STACKSIZE50, 
								SYS_PPU_THREAD_CREATE_JOINABLE, 
								"_resc_intr_thread");
	if (ret != CELL_OK) {
		//_printf("sys_ppu_thread_create failed: %d\n", ret);
		return ret;
	}

	return CELL_OK;
}


int32_t CCellRescInternal::ExitSystemResource(void)
{
	sys_event_port_send(m_eventPort, RESC_EVENT_EXIT_THREAD, 0, 0);
	
	int	ret = sys_timer_disconnect_event_queue(m_timer);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 1\n");
		return ret;
	}

	ret = sys_timer_destroy(m_timer);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 2\n");
		return ret;
	}

	uint64_t exitCode;
	ret = sys_ppu_thread_join(m_intrThread50, &exitCode);
	//RESC_DEBUG_PRINTF("Exit code of intrThread50 is %llu\n", exitCode);
	
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 3\n");
		return ret;
	}

	ret = sys_event_port_disconnect(m_eventPort);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 4\n");
		return ret;
	}

	ret = sys_event_port_destroy(m_eventPort);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 5\n");
		return ret;
	}

	ret = sys_event_queue_destroy(m_eventQueue, 0);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 6\n");
		return ret;
	}

	ret = sys_lwmutex_destroy(&m_lwmutexA);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 7\n");
		return ret;
	}

	ret = sys_lwmutex_destroy(&m_lwmutexB);
	if (ret != CELL_OK) {
		//RESC_DEBUG_PRINTF("exit 8\n");
		return ret;
	}

	return CELL_OK;
}


void CCellRescInternal::SetupRsxRenderingStates(CellGcmContextData* con)
{
	GcmCmdTypePrefix::cellGcmSetColorMask(con, CELL_GCM_COLOR_MASK_A | CELL_GCM_COLOR_MASK_R | CELL_GCM_COLOR_MASK_G | CELL_GCM_COLOR_MASK_B);
	GcmCmdTypePrefix::cellGcmSetDepthMask(con, CELL_GCM_FALSE);           //E different from default.
	GcmCmdTypePrefix::cellGcmSetAlphaTestEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetBlendEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetBlendEnableMrt(con, CELL_GCM_FALSE, CELL_GCM_FALSE, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetLogicOpEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetCullFaceEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetDepthBoundsTestEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetDepthTestEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetPolygonOffsetFillEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetStencilTestEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetTwoSidedStencilTestEnable(con, CELL_GCM_FALSE);
	GcmCmdTypePrefix::cellGcmSetPointSpriteControl(con, CELL_GCM_FALSE, 0, 0);
	GcmCmdTypePrefix::cellGcmSetDitherEnable(con, CELL_GCM_TRUE);
	GcmCmdTypePrefix::cellGcmSetShadeMode(con, CELL_GCM_SMOOTH);
	GcmCmdTypePrefix::cellGcmSetFrequencyDividerOperation(con, 0);

	GcmCmdTypePrefix::cellGcmSetScissor(con, 0, 0, m_dstWidth, m_dstHeight);
	Util::GlViewPort(con, 0, 0, m_dstWidth, m_dstHeight);

	if (IsPalInterpolate()) {
		//MRT
		GcmCmdTypePrefix::cellGcmSetColorMaskMrt(con, CELL_GCM_COLOR_MASK_MRT1_A | CELL_GCM_COLOR_MASK_MRT1_R | CELL_GCM_COLOR_MASK_MRT1_G | CELL_GCM_COLOR_MASK_MRT1_B);
	}
}


void CCellRescInternal::SetupVertexArrays(CellGcmContextData* con)
{
	uint32_t vertexOffset, uOffset, u2Offset;
	RescVertex_t* vertexBuffer = (RescVertex_t*)m_vertexArrayEA;
	uOffset = (uint8_t*)&vertexBuffer->u - (uint8_t*)&vertexBuffer->Px;
	u2Offset = (uint8_t*)&vertexBuffer->u2 - (uint8_t*)&vertexBuffer->Px;

	GcmSysTypePrefix::cellGcmAddressToOffset(vertexBuffer, &vertexOffset);

	GcmCmdTypePrefix::cellGcmSetVertexDataArray(con, m_cgParamIndex[RESC_PARAM_IPOSITION], 0, sizeof(RescVertex_t), 
									  2, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, vertexOffset);
	GcmCmdTypePrefix::cellGcmSetVertexDataArray(con, m_cgParamIndex[RESC_PARAM_ITEXCOORD], 0, sizeof(RescVertex_t), 
									  2, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, vertexOffset + uOffset);

	if (IsPalInterpolate()) {
		//MRT
		GcmCmdTypePrefix::cellGcmSetVertexDataArray(con, m_cgParamIndex[RESC_PARAM_ITEXCOORD_NORATIO], 0, sizeof(RescVertex_t), 
										  2, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, vertexOffset + u2Offset);
	}

	GcmCmdTypePrefix::cellGcmSetInvalidateVertexCache(con);
}


void CCellRescInternal::SetupSurfaces(CellGcmContextData* con)
{
	if (IsNotPalInterpolate()) {
		SetupDstSurface(con, false, m_dstOffsets[m_bufIdFront], 0);
	} else {
		SetupDstSurface(con, true, m_dstOffsets[m_bufIdFront], m_dstOffsets[m_bufIdPalMidNow]);
	}
}


void CCellRescInternal::SetupTextures(CellGcmContextData* con, int32_t srcIndex)
{
	if (IsNotPalInterpolate()) {
		SetupSrcTexture(con, srcIndex, m_cgParamIndex[RESC_PARAM_SAMPLER_SRC0]);
	} else {
		SetupPrevTexture(con, m_bufIdPalMidPrev, m_cgParamIndex[RESC_PARAM_SAMPLER_PAL_PREV]);
		SetupSrcTexture(con, srcIndex, m_cgParamIndex[RESC_PARAM_SAMPLER_PAL_NOW]);
	}

	if (IsInterlace() && m_interlaceTableEA) {
		uint32_t offset;
		GcmSysTypePrefix::cellGcmAddressToOffset(m_interlaceTableEA, &offset);
		SetupInterlaceTableTexture(con, offset, m_cgParamIndex[RESC_PARAM_SAMPLER_INTERLACE]);
	}

	GcmCmdTypePrefix::cellGcmSetInvalidateTextureCache(con, CELL_GCM_INVALIDATE_TEXTURE);
}


void CCellRescInternal::SetupDstSurface(CellGcmContextData* con, bool isMrt, uint32_t dstOffset0, uint32_t dstOffset1)
{
	CellGcmSurface surface;

	surface.type             = CELL_GCM_SURFACE_PITCH;
	surface.antialias        = CELL_GCM_SURFACE_CENTER_1;
	surface.colorFormat      = (uint8_t)m_pRescDsts->format;  //E RescFormat to GcmFormat conversion, value is same.
	surface.colorTarget      = (!isMrt) ? CELL_GCM_SURFACE_TARGET_0 : CELL_GCM_SURFACE_TARGET_MRT1;
	surface.colorLocation[0] = CELL_GCM_LOCATION_LOCAL;
	surface.colorOffset[0]   = dstOffset0;
	surface.colorPitch[0]    = m_dstPitch;
	surface.colorLocation[1] = CELL_GCM_LOCATION_LOCAL;
	surface.colorOffset[1]   = (!isMrt) ? 0  : dstOffset1;
	surface.colorPitch[1]    = (!isMrt) ? 64 : m_dstPitch;
	surface.colorLocation[2] = CELL_GCM_LOCATION_LOCAL;
	surface.colorOffset[2]   = 0;
	surface.colorPitch[2]    = 64;
	surface.colorLocation[3] = CELL_GCM_LOCATION_LOCAL;
	surface.colorOffset[3]   = 0;
	surface.colorPitch[3]    = 64;
	surface.depthFormat      = CELL_GCM_SURFACE_Z24S8;  //E doesn't use depth stencil
	surface.depthLocation    = CELL_GCM_LOCATION_LOCAL;
	surface.depthOffset      = 0;
	surface.depthPitch       = 64;
	surface.width            = m_dstWidth;
	surface.height           = m_dstHeight;
	surface.x                = 0;
	surface.y                = 0;

	GcmCmdTypePrefix::cellGcmSetSurface(con, &surface);
}


void CCellRescInternal::SetupSrcTexture(CellGcmContextData* con, int32_t srcIndex, uint8_t cgIndexSampler)
{
	CellGcmTexture texture;
	
	if (IsTextureNR()) {
		texture.format    = m_rescSrc[srcIndex].format & ~CELL_GCM_TEXTURE_UN;
	} else {
		texture.format    = m_rescSrc[srcIndex].format | CELL_GCM_TEXTURE_UN;
	}
	texture.mipmap    = 1;
	texture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
	texture.cubemap   = CELL_GCM_FALSE;
	texture.remap     = CELL_GCM_TEXTURE_REMAP_REMAP << 14 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 12 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 10 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 8  |
						CELL_GCM_TEXTURE_REMAP_FROM_B << 6 |
						CELL_GCM_TEXTURE_REMAP_FROM_G << 4 |
						CELL_GCM_TEXTURE_REMAP_FROM_R << 2 |
						CELL_GCM_TEXTURE_REMAP_FROM_A;
	texture.width     = m_rescSrc[srcIndex].width;
	texture.height    = m_rescSrc[srcIndex].height;
	texture.depth     = 1;
	texture.location  = CELL_GCM_LOCATION_LOCAL;
	texture.pitch     = m_rescSrc[srcIndex].pitch;
	texture.offset    = m_rescSrc[srcIndex].offset;

	GcmTexTypePrefix::cellGcmSetTexture(con, cgIndexSampler, &texture);

	GcmTexTypePrefix::cellGcmSetTextureControl(con, 
											   cgIndexSampler,                         // index
											   CELL_GCM_TRUE,                          // enable
											   0,                                      // minlod
											   15,                                     // maxlod
											   CELL_GCM_TEXTURE_MAX_ANISO_1);          // maxaniso
	
	GcmTexTypePrefix::cellGcmSetTextureAddress(con, 
											   cgIndexSampler,                         // index  
											   CELL_GCM_TEXTURE_BORDER,                // wrap_s  >> set border color 
											   CELL_GCM_TEXTURE_BORDER,                // wrap_t  >> set border color 
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_r  >> isn't used
											   CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, // unsigned remap
											   CELL_GCM_TEXTURE_ZFUNC_LESS,            // zfunc   >> don't use
											   0);                                     // gamma

	uint8_t formatPart = texture.format & (~(CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_UN));
	uint8_t minFilter, magFilter, conv;

	if(formatPart == CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT ||
	   formatPart == CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT ) {
		// F16 texturing needs point sampling.
		minFilter = CELL_GCM_TEXTURE_NEAREST;
		magFilter = CELL_GCM_TEXTURE_NEAREST;
		conv      = CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX;
	} else {
		// integer texture (normal case)
		if(IsBilinear() || IsInterlace()){
			minFilter = CELL_GCM_TEXTURE_LINEAR;
			magFilter = CELL_GCM_TEXTURE_LINEAR;
			conv      = CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX;
		} else {
			minFilter = CELL_GCM_TEXTURE_CONVOLUTION_MIN;
			magFilter = CELL_GCM_TEXTURE_CONVOLUTION_MAG;
			if(Is3x3Gaussian()) conv = CELL_GCM_TEXTURE_CONVOLUTION_GAUSSIAN;
			if(IsQuincunx())    conv = CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX;
			if(IsQuincunxAlt()) conv = CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX_ALT;
		}
	}

	GcmTexTypePrefix::cellGcmSetTextureFilter(con, 
											  cgIndexSampler,                          // index
											  0,                                       // bias(mipmap)
											  minFilter,                               // min 
											  magFilter,                               // mag 
											  conv);                                   // conv

	GcmCmdTypePrefix::cellGcmSetTextureBorderColor(con, cgIndexSampler, 
												   0x00000000);                        // color >> black
}


void CCellRescInternal::SetupPrevTexture(CellGcmContextData* con, int32_t dstIndex, uint8_t cgIndexSampler)
{
	CellGcmTexture texture;

	if(m_pRescDsts->format == CELL_RESC_SURFACE_A8R8G8B8){
		texture.format    = CELL_GCM_TEXTURE_A8R8G8B8 | CELL_GCM_TEXTURE_LN;
	} else {
		texture.format    = CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT | CELL_GCM_TEXTURE_LN;
	}
	if(IsTextureNR()) {
		texture.format    &= ~CELL_GCM_TEXTURE_UN;
	} else {
		texture.format    |= CELL_GCM_TEXTURE_UN;
	}

	texture.mipmap    = 1;
	texture.dimension = CELL_GCM_TEXTURE_DIMENSION_2;
	texture.cubemap   = CELL_GCM_FALSE;
	texture.remap     = CELL_GCM_TEXTURE_REMAP_REMAP << 14 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 12 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 10 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 8  |
						CELL_GCM_TEXTURE_REMAP_FROM_B << 6 |
						CELL_GCM_TEXTURE_REMAP_FROM_G << 4 |
						CELL_GCM_TEXTURE_REMAP_FROM_R << 2 |
						CELL_GCM_TEXTURE_REMAP_FROM_A;
	texture.width     = m_dstWidth;
	texture.height    = m_dstHeight;
	texture.depth     = 1;
	texture.location  = CELL_GCM_LOCATION_LOCAL;
	texture.pitch     = m_dstPitch;
	texture.offset    = m_dstOffsets[dstIndex];

	GcmTexTypePrefix::cellGcmSetTexture(con, cgIndexSampler, &texture);

	GcmTexTypePrefix::cellGcmSetTextureControl(con, 
											   cgIndexSampler,                         // index
											   CELL_GCM_TRUE,                          // enable
											   0,                                      // minlod
											   15,                                     // maxlod
											   CELL_GCM_TEXTURE_MAX_ANISO_1);          // maxaniso

	GcmTexTypePrefix::cellGcmSetTextureAddress(con, 
											   cgIndexSampler,                         // index  
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_s 
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_t
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_r  >> isn't used
											   CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, // unsigned remap
											   CELL_GCM_TEXTURE_ZFUNC_LESS,            // zfunc   >> don't use
											   0);                                     // gamma

	GcmTexTypePrefix::cellGcmSetTextureFilter(con, 
											  cgIndexSampler,                          // index
											  0,                                       // bias
											  CELL_GCM_TEXTURE_NEAREST,                // min 
											  CELL_GCM_TEXTURE_NEAREST,                // mag 
											  CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX);  // conv
}


void CCellRescInternal::SetupInterlaceTableTexture(CellGcmContextData* con, uint32_t offset, uint8_t cgIndexSampler)
{
	CellGcmTexture texture;

	if(m_interlaceElement == CELL_RESC_ELEMENT_HALF){
		texture.format    = CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT | CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR;
	} else {
		texture.format    = CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT | CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR;
	}
	texture.mipmap    = 1;
	texture.dimension = CELL_GCM_TEXTURE_DIMENSION_1;
	texture.cubemap   = CELL_GCM_FALSE;
	texture.remap     = CELL_GCM_TEXTURE_REMAP_REMAP << 14 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 12 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 10 |
						CELL_GCM_TEXTURE_REMAP_REMAP << 8  |
						CELL_GCM_TEXTURE_REMAP_FROM_B << 6 |
						CELL_GCM_TEXTURE_REMAP_FROM_G << 4 |
						CELL_GCM_TEXTURE_REMAP_FROM_R << 2 |
						CELL_GCM_TEXTURE_REMAP_FROM_A;
	texture.width     = m_interlaceTableLength;
	texture.height    = 1;
	texture.depth     = 1;
	texture.location  = CELL_GCM_LOCATION_LOCAL;
	texture.pitch     = m_interlaceTableLength * 4 * (m_interlaceElement == CELL_RESC_ELEMENT_HALF ? 2 : 4);
	texture.offset    = offset;

	GcmTexTypePrefix::cellGcmSetTexture(con, cgIndexSampler, &texture);

	GcmTexTypePrefix::cellGcmSetTextureControl(con, 
											   cgIndexSampler,                         // index
											   CELL_GCM_TRUE,                          // enable
											   0,                                      // minlod
											   15,                                     // maxlod
											   CELL_GCM_TEXTURE_MAX_ANISO_1);          // maxaniso

	GcmTexTypePrefix::cellGcmSetTextureAddress(con, 
											   cgIndexSampler,                         // index  
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_s 
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_t  >> isn't used
											   CELL_GCM_TEXTURE_CLAMP,                 // wrap_r  >> isn't used
											   CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL, // unsigned remap
											   CELL_GCM_TEXTURE_ZFUNC_LESS,            // zfunc   >> don't use
											   0);                                     // gamma

	GcmTexTypePrefix::cellGcmSetTextureFilter(con, 
											  cgIndexSampler,                          // index
											  0,                                       // bias
											  CELL_GCM_TEXTURE_NEAREST,                // min 
											  CELL_GCM_TEXTURE_NEAREST,                // mag 
											  CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX);  // conv
}


void CCellRescInternal::ResolveVertexParamBindingsByName(void)
{
	CGprogram cgProgram = m_pCVertexShader->GetCGprogram();
	CGparameter cgParam;

	for (int i=RESC_PARAM_IPOSITION; i<RESC_PARAM_SAMPLER_SRC0; i++) {
		cgParam = GcmCgTypePrefix::cellGcmCgGetNamedParameter(cgProgram, s_paramName[i]);
		m_cgParamIndex[i] = GcmCgTypePrefix::cellGcmCgGetParameterResource(cgProgram, cgParam) - CG_ATTR0;
	}
}


void CCellRescInternal::ResolveFragmentParamBindingsByName(void)
{
	CGprogram cgProgram = m_pCFragmentShader->GetCGprogram();
	CGparameter cgParam;

	for (int i=RESC_PARAM_SAMPLER_SRC0; i<=RESC_PARAM_SAMPLER_PAL_NOW; i++) {
		cgParam = GcmCgTypePrefix::cellGcmCgGetNamedParameter(cgProgram, s_paramName[i]);
		if(cgParam){
			m_cgParamIndex[i] = GcmCgTypePrefix::cellGcmCgGetParameterResource(cgProgram, cgParam) - CG_TEXUNIT0;
		} else {
			m_cgParamIndex[i] = 0xFF;
		}
	}

	m_cgpTvalue = GcmCgTypePrefix::cellGcmCgGetNamedParameter(cgProgram, s_paramName[RESC_PARAM_PAL_TVALUE]);
}


void CCellRescInternal::BuildupVertexBufferNR(void)
{
	const float PX_FS = PICTURE_SIZE;
	const float PY_FS = PICTURE_SIZE;

	const float UV_HALF   = 0.5f;
	const float UV_CENTER = 0.5f;
	float U_FS  = UV_HALF / m_ratioAdjX;
	float V_FS  = UV_HALF / m_ratioAdjY;
	float U_FS0 = UV_CENTER - U_FS;
	float V_FS0 = UV_CENTER - V_FS;
	float U_FS1 = UV_CENTER + U_FS;
	float V_FS1 = UV_CENTER + V_FS;
	float V_LB  = (UV_HALF + UV_DELTA_LB) / m_ratioAdjY;
	float V_LB0 = UV_CENTER - V_LB;
	float V_LB1 = UV_CENTER + V_LB;
	float U_PS  = (UV_HALF - UV_DELTA_PS) / m_ratioAdjX;
	float U_PS0 = UV_CENTER - U_PS;
	float U_PS1 = UV_CENTER + U_PS;

	RescVertex_t *vv = (RescVertex_t *)m_vertexArrayEA;

	if(m_dstMode == CELL_RESC_720x480 || m_dstMode == CELL_RESC_720x576){
		//E SD pass
		switch(m_initConfig.ratioMode){
		case CELL_RESC_FULLSCREEN:
		default:                   goto NR_FULLSCREEN;
		case CELL_RESC_LETTERBOX:  goto NR_LETTERBOX;
		case CELL_RESC_PANSCAN:    goto NR_PANSCAN;
		}
	} else {
		//E HD pass
		goto NR_FULLSCREEN;
	}

NR_FULLSCREEN:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_FS0; vv->v = V_FS0; vv->u2 = 0.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_FS1; vv->v = V_FS0; vv->u2 = 1.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_FS1; vv->v = V_FS1; vv->u2 = 1.0f; vv->v2 = 1.0f; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_FS0; vv->v = V_FS1; vv->u2 = 0.0f; vv->v2 = 1.0f; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;

NR_LETTERBOX:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_FS0; vv->v = V_LB0; vv->u2 = 0.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_FS1; vv->v = V_LB0; vv->u2 = 1.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_FS1; vv->v = V_LB1; vv->u2 = 1.0f; vv->v2 = 1.0f; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_FS0; vv->v = V_LB1; vv->u2 = 0.0f; vv->v2 = 1.0f; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;

NR_PANSCAN:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_PS0; vv->v = V_FS0; vv->u2 = 0.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_PS1; vv->v = V_FS0; vv->u2 = 1.0f; vv->v2 = 0.0f; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_PS1; vv->v = V_FS1; vv->u2 = 1.0f; vv->v2 = 1.0f; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_PS0; vv->v = V_FS1; vv->u2 = 0.0f; vv->v2 = 1.0f; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;
}


void CCellRescInternal::BuildupVertexBufferUN(int32_t srcIdx)
{
	if(m_bNewlyAdjustRatio){
		m_srcWidthInterlace  = m_rescSrc[srcIdx].width;
		m_srcHeightInterlace = m_rescSrc[srcIdx].height;
		m_bNewlyAdjustRatio = false;
	} else {
		if(m_srcWidthInterlace == m_rescSrc[srcIdx].width 
		   && m_srcHeightInterlace == m_rescSrc[srcIdx].height){
			return;
		} else {
			m_srcWidthInterlace  = m_rescSrc[srcIdx].width;
			m_srcHeightInterlace = m_rescSrc[srcIdx].height;
		}
	}
	const float PX_FS = PICTURE_SIZE;
	const float PY_FS = PICTURE_SIZE;
	
	float U_HALF = m_rescSrc[srcIdx].width  * 0.5f;
	float V_HALF = m_rescSrc[srcIdx].height * 0.5f;
	float U_CENTER = U_HALF;
	float V_CENTER = V_HALF;
	float U_FS  = U_HALF / m_ratioAdjX;
	float V_FS  = V_HALF / m_ratioAdjY;
	float U_FS0 = U_CENTER - U_FS;
	float V_FS0 = V_CENTER - V_FS;
	float U_FS1 = U_CENTER + U_FS;
	float V_FS1 = V_CENTER + V_FS;
	float V_LB  = V_HALF * (1.f + 2.f*UV_DELTA_LB) / m_ratioAdjY;
	float V_LB0 = V_CENTER - V_LB;
	float V_LB1 = V_CENTER + V_LB;
	float U_PS  = U_HALF * (1.f - 2.f*UV_DELTA_PS) / m_ratioAdjX;
	float U_PS0 = U_CENTER - U_PS;
	float U_PS1 = U_CENTER + U_PS;
	float U2_FS0 = 0.0f;
	float V2_FS0 = 0.0f;
	float U2_FS1 = m_dstWidth;
	float V2_FS1 = m_dstHeight;

	RescVertex_t *vv = (RescVertex_t *)m_vertexArrayEA;

	if(m_dstMode == CELL_RESC_720x480 || m_dstMode == CELL_RESC_720x576){
		//E SD pass
		switch(m_initConfig.ratioMode){
		case CELL_RESC_FULLSCREEN:
		default:                   goto UN_FULLSCREEN;
		case CELL_RESC_LETTERBOX:  goto UN_LETTERBOX;
		case CELL_RESC_PANSCAN:    goto UN_PANSCAN;
		}
	} else {
		//E HD pass
		goto UN_FULLSCREEN;
	}

UN_FULLSCREEN:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_FS0; vv->v = V_FS0; vv->u2 = U2_FS0; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_FS1; vv->v = V_FS0; vv->u2 = U2_FS1; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_FS1; vv->v = V_FS1; vv->u2 = U2_FS1; vv->v2 = V2_FS1; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_FS0; vv->v = V_FS1; vv->u2 = U2_FS0; vv->v2 = V2_FS1; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;

UN_LETTERBOX:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_FS0; vv->v = V_LB0; vv->u2 = U2_FS0; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_FS1; vv->v = V_LB0; vv->u2 = U2_FS1; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_FS1; vv->v = V_LB1; vv->u2 = U2_FS1; vv->v2 = V2_FS1; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_FS0; vv->v = V_LB1; vv->u2 = U2_FS0; vv->v2 = V2_FS1; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;

UN_PANSCAN:
	vv->Px = -PX_FS; vv->Py =  PY_FS; vv->u = U_PS0; vv->v = V_FS0; vv->u2 = U2_FS0; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py =  PY_FS; vv->u = U_PS1; vv->v = V_FS0; vv->u2 = U2_FS1; vv->v2 = V2_FS0; vv++;
	vv->Px =  PX_FS; vv->Py = -PY_FS; vv->u = U_PS1; vv->v = V_FS1; vv->u2 = U2_FS1; vv->v2 = V2_FS1; vv++;
	vv->Px = -PX_FS; vv->Py = -PY_FS; vv->u = U_PS0; vv->v = V_FS1; vv->u2 = U2_FS0; vv->v2 = V2_FS1; vv++;
	m_nVertex = VERTEX_NUMBER_NORMAL;
	return;
}


int32_t CCellRescInternal::CalculateMaxColorBuffersSize(void)
{
	int32_t oneBufSize, bufNum, totalBufSize, maxBufSize;
	CellRescBufferMode bufMode;

	maxBufSize = 0;

	for(int i = CELL_RESC_720x480; i <= CELL_RESC_1920x1080; i <<= 1){
		bufMode = (CellRescBufferMode)i;
		if(m_initConfig.supportModes & bufMode) {
			oneBufSize   = Util::CalculateSurfaceByteSize(bufMode, &m_rescDsts[GetRescDestsIndex(bufMode)]);
			bufNum       = cellRescGetNumColorBuffers(bufMode, (CellRescPalTemporalMode)m_initConfig.palTemporalMode, 0);
			totalBufSize = oneBufSize * bufNum;
			maxBufSize   = (maxBufSize > totalBufSize) ? maxBufSize : totalBufSize;
		}
	}

	return maxBufSize;
}


void CCellRescInternal::CalculateTvalue(float* OUT tValue, CellRescVBlank* OUT targetV60, CellRescVBlank* OUT targetOav, 
										CellRescVBlank* IN lastV60, CellRescVBlank* IN lastV50, int32_t interval, int32_t nInQue)
{
	//E clamp interval under MAX_INTERVAL_FOR_INTERPOLATE (= 4).
	interval = (interval <= MAX_INTERVAL_FOR_INTERPOLATE) ? interval : MAX_INTERVAL_FOR_INTERPOLATE;

	targetV60->idx  = lastV60->idx + (nInQue + 1)*interval;
	targetV60->time = lastV60->time + (targetV60->idx - lastV60->idx) * RESC_60HZ_INTERVAL_USEC;

	//E blank for-loop which update "targetOav".
	for(targetOav->time = lastV50->time - RESC_OAT_USEC, targetOav->idx = lastV50->idx; 
		 targetOav->time < targetV60->time; 
		 targetOav->time += RESC_50HZ_INTERVAL_USEC, ++(targetOav->idx))
		;

	*tValue = (float)(RESC_50HZ_INTERVAL_USEC - (targetOav->time - targetV60->time)) / (float)(RESC_50HZ_INTERVAL_USEC);

	//*tValue = ModulateTvalue(*tValue);

	if(IsPalInterpolate30Drop() && interval >= 2){
		if(*tValue < 0.5f) *tValue = 0.f;
		else               *tValue = 1.f;
	} else if(IsPalInterpolateDropFlexible()){
		*tValue = (m_flexRatio * 1.f) + ((1.f - m_flexRatio) * (*tValue));
	}
	//E clip tValue between 0 and 1.
	if(*tValue > 1.f) *tValue = 1.f;
	if(*tValue < 0.f) *tValue = 0.f;
}

#if 0
//E Can give modulation from linear t
float CCellRescInternal::ModulateTvalue(float t)
{
	return 0.5f + 0.5f*sinf((t-0.5f)*M_PI);
}
#endif

int32_t CCellRescInternal::OavCallback(void)
{
	uint8_t queId = RESC_NO_COMMAND_DELIVERED;
	CRescCommand command;

	RESC_LOCK(B, SYS_NO_TIMEOUT) {
		int32_t numGpuFinished = 0;
		int32_t numPassTimeTest = 0;
		for(QUEUE1::iterator que_itr = QUEUE1::Begin(); que_itr != QUEUE1::End(); ++que_itr)
		{
			if(que_itr->m_mark60Intr) numGpuFinished++;
			if(que_itr->m_frameIdx50 <= (this->m_lastV50.idx)+1) numPassTimeTest++;
		}
		//E The desired number of frames to skip is passedTimeTest
		//E The maximum dequeue count is established to prevent tearing and display of old frames
		//E This ensures that not the GPU has finished preparing the buffer before it is presented
		int32_t const maxDeqCount = (numGpuFinished < numPassTimeTest) ? numGpuFinished : numPassTimeTest;

		//E The minimum dequeue count is established to prevent tearing and overflow of command queue
		//E This ensures that not too many GPU finished commands are left in queue
		int32_t const minDeqCount = numGpuFinished - 1;

		//E The actual dequeue count respects MIN/MAX constraints
		int32_t const deqCount = (minDeqCount > maxDeqCount) ? minDeqCount : maxDeqCount;

		for(int32_t j = 0; j < deqCount; ++j)
		{
			QUEUE1::Deq(command);
			queId = command.m_queId;
		}
	} RESC_UNLOCK(B)

	if(queId != RESC_NO_COMMAND_DELIVERED)
	{
		GcmSysTypePrefix::cellGcmSetFlipImmediate(queId);
	}

	return CELL_OK;
}


// ============================================================================
//E Threads for PAL
// ============================================================================
void CCellRescInternal::IntrThread50(uint64_t arg __attribute__ ((unused)))
{
	sys_event_t event;

	while (1) {
		int32_t ret = sys_event_queue_receive(GetInstance()->m_eventQueue, &event, SYS_NO_TIMEOUT);

		if(event.data1 == RESC_EVENT_EXIT_THREAD)
			break;

		if (ret != CELL_OK) {
			if (ret == ECANCELED) {
				break;
			} else {
				//RESC_DEBUG_PRINTF("Bad exit from intrThread50  %d\n", ret);
				sys_ppu_thread_exit(1);
			}
		}

		//E main process.
		GetInstance()->OavCallback();
	}

	sys_ppu_thread_exit(0);
}


void CCellRescInternal::IntrHandler50(uint32_t head __attribute__ ((unused)))
{
	system_time_t timeWakeup = Util::GetSystemTime();

	RESC_LOCK(A, SYS_NO_TIMEOUT) {
		GetInstance()->m_lastV50.idx++;
		GetInstance()->m_lastV50.time = timeWakeup;
	} RESC_UNLOCK(A)

	int ret = sys_timer_start_oneshot(GetInstance()->m_timer, timeWakeup + (RESC_50HZ_INTERVAL_USEC - RESC_OAT_USEC + 1000) );  //E 1000 is good enough.
	if(ret != CELL_OK){
		//RESC_DEBUG_PRINTF("sys_timer_start_oneshot   failed 0x%x\n", ret);
	}
}


void CCellRescInternal::IntrHandler60(uint32_t head __attribute__ ((unused)))
{
	static int32_t s_intervalIncrementer = 0;
	volatile uint32_t cmdLabelValue;

	if(s_applicationVBlankHandler) s_applicationVBlankHandler(head);

	volatile uint32_t* cmdLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(RESC_LABEL_PAL_COMMAND_IDX);
	cmdLabelValue = *cmdLabel;

	system_time_t timeWakeup = Util::GetSystemTime();

	RESC_LOCK(A, SYS_NO_TIMEOUT) {
		GetInstance()->m_lastV60.idx++;
		GetInstance()->m_lastV60.time = timeWakeup;
	} RESC_UNLOCK(A)

	++s_intervalIncrementer;

	if (cmdLabelValue > GetInstance()->m_rcvdCmdIdx) {
		GetInstance()->m_rcvdCmdIdx++;
		//GetInstance()->m_rcvdCmdIdx = cmdLabelValue;

		uint8_t currentFlipLabelId = GetInstance()->GetLabelIndexOfPalFlipStatus();
		volatile uint32_t* flipLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(currentFlipLabelId);

		if (*flipLabel == RESC_DUMMY_FLIP_CLEARED) {
			*flipLabel = RESC_DUMMY_FLIPPED;
		}

		GetInstance()->m_isDummyFlipped = true;
		GetInstance()->m_lastDummyFlip = GcmSysTypePrefix::cellGcmGetLastSecondVTime();
		
		if(s_applicationFlipHandler) s_applicationFlipHandler(head);

		RESC_LOCK(B, SYS_NO_TIMEOUT) {
			for(QUEUE1::iterator que_itr = QUEUE1::Begin(); que_itr != QUEUE1::End(); ++que_itr) {
				if( que_itr->m_index <= cmdLabelValue ) {
					que_itr->m_mark60Intr = true;
				}
			}
		} RESC_UNLOCK(B)

		GetInstance()->m_feedback.interval60 = s_intervalIncrementer;
		s_intervalIncrementer = 0;
	}
}


void CCellRescInternal::IntrHandler60Drop(uint32_t head __attribute__ ((unused)))
{
	volatile uint32_t cmdLabelValue;

	if(s_applicationVBlankHandler) s_applicationVBlankHandler(head);

	volatile uint32_t* cmdLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(RESC_LABEL_PAL_COMMAND_IDX);
	cmdLabelValue = *cmdLabel;

	if (cmdLabelValue > GetInstance()->m_rcvdCmdIdx) {
		GetInstance()->m_rcvdCmdIdx++;
		//GetInstance()->m_rcvdCmdIdx = cmdLabelValue;

		uint8_t currentFlipLabelId = GetInstance()->GetLabelIndexOfPalFlipStatus();
		volatile uint32_t* flipLabel = GcmSysTypePrefix::cellGcmGetLabelAddress(currentFlipLabelId);

		if (*flipLabel == RESC_DUMMY_FLIP_CLEARED) {
			*flipLabel = RESC_DUMMY_FLIPPED;
		}

		GetInstance()->m_isDummyFlipped = true;
		GetInstance()->m_lastDummyFlip = GcmSysTypePrefix::cellGcmGetLastSecondVTime();

		if(s_applicationFlipHandler) s_applicationFlipHandler(head);

	}
}

};  /* namespace Resc */
};  /* namespace cell */

