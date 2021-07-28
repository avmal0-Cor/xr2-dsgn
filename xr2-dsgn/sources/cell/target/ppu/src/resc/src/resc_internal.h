/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: resc_internal.h
 * Description:
 *
 */

#ifndef __CELL_RESC_INTERNAL_H__
#define __CELL_RESC_INTERNAL_H__

#include <stdint.h>
#include <sys/ppu_thread.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/synchronization.h>
#ifdef SCE_INTERNAL_BUILD
# include <sys/sysclib.h>
# include <cell/name_rules.h>
#else
# include <stdio.h>
# include <string.h>
# define  SYS_NAME_PREFIX_LIBRESC_SPRX   "_res"
#endif

#include "cell/resc.h"

#define GcmCmdTypePrefix               Inline    // use inline version gcm_cmd
//#define GcmCmdTypePrefix                         // use function version gcm_cmd
#define GcmTexTypePrefix               Inline    // use inline version texture API
//#define GcmTexTypePrefix   cell::Resc::GcmTex    // use function wrapped version texture API ( need other changes )
#define GcmCgTypePrefix                          // use function version at GcmCg API
#define GcmSysTypePrefix                         // use function version gcm_sys ( for coherency )
#define GcmFlipPrefix                            // use function call

#include "shader.h"
#include "que.h"

namespace cell {
namespace Resc {

#define IN
#define OUT

#define COLOR_BUFFER_ALIGNMENT         128
#define VERTEX_BUFFER_ALIGNMENT        4         // no restriction
#define FRAGMENT_SHADER_ALIGNMENT      64
#define VERTEX_NUMBER_NORMAL           4
#define VERTEX_NUMBER_RESERVED         16
#define FRAGMENT_SHADER_CONST_SIZE_MAX 0x100
#define TILED_REGION_BASE_BOUNDARY     0x10000   // 64k
#define TILED_REGION_SIZE_BOUNDARY     0x10000   // 64k
#define TILED_BUFFER_H_ALIGNMENT       32        // local -> 32, system -> 64

#define SRC_BUFFER_NUM                 8
#define MAX_DST_BUFFER_NUM             6

#define CELL_GCM_PREPARE_QUEUE_MAX     8
#define RESC_COMMAND_FIFO_DEPTH        CELL_GCM_PREPARE_QUEUE_MAX
#define RESC_CFIXQUEUE_SIZE            (RESC_COMMAND_FIFO_DEPTH+1)
#define RESC_NO_COMMAND_DELIVERED      0xFE
#define MAX_INTERVAL_FOR_INTERPOLATE   4

#define PICTURE_SIZE                   (1.0f)        // changing this can make under/over scan.
#define UV_DELTA_PS                    (1.f / 8.f)   // = 0.125
#define UV_DELTA_LB                    (1.f / 6.f)   // = 0.1666 (infinite decimal)
#define XY_DELTA_PS                    (1.f / 6.f)   // = 0.1666 (infinite decimal)
#define XY_DELTA_LB                    (1.f / 8.f)   // = 0.125

#define RESC_THREAD_PRIORITY50         2
#define RESC_THREAD_STACKSIZE50        (16*1024)
#define RESC_EVENT_QUEUE_SIZE          4
#define RESC_EVENT_EXIT_THREAD         0x4649

#define RESC_OAT_USEC                  3000
#define RESC_60HZ_INTERVAL_USEC        16683
#define RESC_50HZ_INTERVAL_USEC        20000

enum {
//	RESC_LABEL_NOW_UNUSED1             = 16, //CELL_RESC_LABEL_INTERNAL_1, 
	RESC_LABEL_PAL_COMMAND_IDX         = 17, //CELL_RESC_LABEL_INTERNAL_2, 
	RESC_LABEL_FOR_DUMMY_FLIP0         = 18, //CELL_RESC_LABEL_INTERNAL_3, 
	RESC_LABEL_FOR_DUMMY_FLIP1         = 19, //CELL_RESC_LABEL_INTERNAL_4, 

	RESC_DUMMY_FLIPPED                 = 0,
	RESC_DUMMY_FLIP_CLEARED            = 1,
};

enum EDefaultShaders {
	RESC_SHADER_DEFAULT_BILINEAR = 0, 
	RESC_SHADER_DEFAULT_BILINEAR_PAL, 
	RESC_SHADER_DEFAULT_INTERLACE, 
	RESC_SHADER_DEFAULT_INTERLACE_PAL, 
	RESC_SHADER_USER1, 
	RESC_SHADER_USER2, 
	RESC_SHADER_USER3, 
	RESC_SHADER_USER4, 

	RESC_SHADER_NUM, 
};

enum EShaderParam {
	RESC_PARAM_IPOSITION = 0,                    // vertex shader
	RESC_PARAM_ITEXCOORD,                        // vertex shader
	RESC_PARAM_ITEXCOORD_NORATIO,                // vertex shader

	RESC_PARAM_SAMPLER_SRC0, 
	RESC_PARAM_SAMPLER_SRC1, 
	RESC_PARAM_SAMPLER_SRC2, 
	RESC_PARAM_SAMPLER_SRC3, 
	RESC_PARAM_SAMPLER_SRC4, 
	RESC_PARAM_SAMPLER_SRC5, 
	RESC_PARAM_SAMPLER_SRC6, 
	RESC_PARAM_SAMPLER_SRC7, 
	RESC_PARAM_SAMPLER_INTERLACE, 
	RESC_PARAM_SAMPLER_PAL_PREV, 
	RESC_PARAM_SAMPLER_PAL_NOW, 
	RESC_PARAM_PAL_TVALUE, 

	RESC_PARAM_NUM, 
};

typedef struct {
	float Px, Py;
	float u, v;
	float u2, v2;
} RescVertex_t;

// ============================================================================
// CRescCommand class definition (CFixQueue's aliase is QUEUE1)
// ============================================================================

class CRescCommand
{
public:
	int64_t m_index, m_frameIdx60, m_frameIdx50;
	uint8_t m_bufId, m_queId;
	bool    m_mark60Intr;

public:
	CRescCommand() {}
	CRescCommand(int64_t index, int64_t frameIdx60, int64_t frameIdx50, uint8_t bufId, uint8_t queId, bool mark60Intr = false)
	  : m_index(index), m_frameIdx60(frameIdx60), m_frameIdx50(frameIdx50), m_bufId(bufId), m_queId(queId), m_mark60Intr(mark60Intr) { }

	// Substitution
	CRescCommand& operator=(CRescCommand& item) {
		if(this != &item){
			m_index      = item.m_index;
			m_frameIdx60 = item.m_frameIdx60;
			m_frameIdx50 = item.m_frameIdx50;
			m_bufId      = item.m_bufId;
			m_queId      = item.m_queId;
			m_mark60Intr = item.m_mark60Intr;
		}
		return *this;
	}
};

// The alias of queue for CRescCommand
typedef CFixQueue<CRescCommand, RESC_CFIXQUEUE_SIZE> QUEUE1;


// ============================================================================
// CCellRescInternal class definition
// ============================================================================

class CCellRescInternal
{
private:
	CellRescInitConfig  m_initConfig;
	CellRescSrc         m_rescSrc[SRC_BUFFER_NUM];
	CellRescDsts        m_rescDsts[4], *m_pRescDsts;
	CellRescBufferMode  m_dstMode;
	CellRescTableElement m_interlaceElement;

	Shader::CVertexShader   *m_pCVertexShader;
	Shader::CFragmentShader *m_pCFragmentShaderArray[RESC_SHADER_NUM], *m_pCFragmentShader;

	const void         *m_colorBuffersEA, *m_vertexArrayEA, *m_fragmentUcodeEA;
	const void         *m_interlaceTableEA;
	uint32_t            m_bufIdFront;
	int32_t             m_dstWidth, m_dstHeight, m_dstPitch;
	uint16_t            m_srcWidthInterlace, m_srcHeightInterlace;
	uint32_t            m_dstBufInterval, m_dstOffsets[MAX_DST_BUFFER_NUM];
	int32_t             m_nVertex, m_interlaceTableLength;
	float               m_ratioAdjX, m_ratioAdjY;
	bool                m_bInitialized, m_bNewlyAdjustRatio;
	uint8_t             m_cgParamIndex[RESC_PARAM_NUM];
	
	// PAL related variables
	sys_ppu_thread_t    m_intrThread50;
    sys_lwmutex_t       m_lwmutexA, m_lwmutexB;
	sys_timer_t         m_timer;
	sys_event_queue_t   m_eventQueue;
	sys_event_port_t    m_eventPort;

	typedef struct CellRescVBlank {
		int64_t idx;
		system_time_t time;
	};
	typedef struct CellRescFrameFeedback {
		int32_t interval60;
	};

	system_time_t       m_lastDummyFlip, m_lastVsync60, m_lastVsync50;
	uint32_t            m_bufIdPalMidPrev, m_bufIdPalMidNow;
	uint32_t            m_bufIdFrontPrevDrop;
	CGparameter         m_cgpTvalue;
	bool                m_isDummyFlipped;
	const uint32_t      m_head; // = 0
	float               m_flexRatio;
	uint64_t            m_commandIdxCaF, m_rcvdCmdIdx;
	CellRescVBlank      m_lastV60, m_lastV50;
	CellRescFrameFeedback m_feedback;
	
	static CCellRescInternal *s_rescInternal;
	static CellRescHandler s_applicationVBlankHandler, s_applicationFlipHandler;

	void InitMembers(void);
	void InitLabels(void);
	bool CheckInitConfig(const CellRescInitConfig * const initConfig);
	void SetupRsxRenderingStates(CellGcmContextData* con);
	void SetupVertexArrays(CellGcmContextData *con);
	void SetupSurfaces(CellGcmContextData* con);
	void SetupTextures(CellGcmContextData* con, const int32_t srcIndex);
	void SetupDstSurface(CellGcmContextData* con, const bool isMrt, const uint32_t dstOffset0, const uint32_t dstOffset1);
	void SetupSrcTexture(CellGcmContextData* con, const int32_t srcIndex, const uint8_t cgIndexSampler);
	void SetupPrevTexture(CellGcmContextData* con, const int32_t dstIndex, const uint8_t cgIndexSampler);
	void SetupInterlaceTableTexture(CellGcmContextData* con, uint32_t offset, uint8_t cgIndexSampler);
	void ResolveVertexParamBindingsByName(void);
	void ResolveFragmentParamBindingsByName(void);
	void BuildupVertexBufferNR(void);
	void BuildupVertexBufferUN(int32_t srcIdx);
	int32_t CalculateMaxColorBuffersSize(void);
	
	// PAL related methods
	int32_t InitSystemResource(void);
	int32_t ExitSystemResource(void);
	void CalculateTvalue(float* OUT tValue, CellRescVBlank* OUT targetV60, CellRescVBlank* OUT targetOav, 
						 CellRescVBlank* IN lastV60, CellRescVBlank* IN lastV50, int32_t interval, int32_t nInQue);
	float ModulateTvalue(float t);
	int32_t OavCallback(void);

	inline int32_t InternalVersion(const CellRescInitConfig* conf) const {
		switch (conf->size)
		{
		case 20: return 1;
		case 24: return 2;
		case 28: return 3;
		default: return -1;
		}
	}
	inline int32_t InternalVersion(void) const { return InternalVersion(&m_initConfig); }

	static void IntrThread50(uint64_t arg);
	static void IntrHandler50(uint32_t head);
	static void IntrHandler60(uint32_t head);
	static void IntrHandler60Drop(uint32_t head);

	inline bool IsBilinear(void) const { return m_initConfig.interlaceMode == CELL_RESC_NORMAL_BILINEAR; }
	inline bool IsInterlace(void) const { return m_initConfig.interlaceMode == CELL_RESC_INTERLACE_FILTER; }
	inline bool Is3x3Gaussian(void) const { return m_initConfig.interlaceMode == CELL_RESC_3X3_GAUSSIAN; }
	inline bool IsQuincunx(void) const { return m_initConfig.interlaceMode == CELL_RESC_2X3_QUINCUNX; }
	inline bool IsQuincunxAlt(void) const { return m_initConfig.interlaceMode == CELL_RESC_2X3_QUINCUNX_ALT; }
	inline bool IsTextureNR(void) const { return !IsInterlace(); }

	inline bool IsPal(void) const { return m_dstMode == CELL_RESC_720x576; }
	inline bool IsPalTemporal(void) const { return ( IsPal() && m_initConfig.palTemporalMode != CELL_RESC_PAL_50); }
	inline bool IsPalDrop(void) const { return ( IsPal() && m_initConfig.palTemporalMode == CELL_RESC_PAL_60_DROP); }
	inline bool IsPalInterpolate(void) const { return ( IsPal() && ((m_initConfig.palTemporalMode == CELL_RESC_PAL_60_INTERPOLATE)
																	|| (m_initConfig.palTemporalMode == CELL_RESC_PAL_60_INTERPOLATE_30_DROP)
																	|| (m_initConfig.palTemporalMode == CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE))); }
	inline bool IsPalInterpolate30Drop(void) const { return ( IsPal() && m_initConfig.palTemporalMode == CELL_RESC_PAL_60_INTERPOLATE_30_DROP); }
	inline bool IsPalInterpolateDropFlexible(void) const { return ( IsPal() && m_initConfig.palTemporalMode == CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE); }
	inline bool IsPal60Hsync(void) const { return ( IsPal() && m_initConfig.palTemporalMode == CELL_RESC_PAL_60_FOR_HSYNC); }
	inline bool IsGcmFlip(void) const { return ( IsNotPal() || (IsPal() && (m_initConfig.palTemporalMode == CELL_RESC_PAL_50
																			|| m_initConfig.palTemporalMode == CELL_RESC_PAL_60_FOR_HSYNC)) ); }

	inline bool IsNotPal(void) const { return !IsPal(); }
	inline bool IsNotPalTemporal(void) const { return !IsPalTemporal(); }
	inline bool IsNotPalDrop(void) const { return !IsPalDrop(); }
	inline bool IsNotPalInterpolate(void) const { return !IsPalInterpolate(); }

	inline int32_t GetRescDestsIndex(CellRescBufferMode dstMode) {
		if(dstMode == CELL_RESC_720x480)         return 0;
		else if (dstMode == CELL_RESC_720x576)   return 1;
		else if (dstMode == CELL_RESC_1280x720)  return 2;
		else if (dstMode == CELL_RESC_1920x1080) return 3;
		else                                     return -1;
	}
	inline int32_t GetNumColorBuffers() const { return IsPalInterpolate() ? 6 : (IsPalDrop() ? 3 : 2); }

	inline uint8_t GetLabelIndexOfPalFlipStatus(void) const { return (m_head!=1 ? RESC_LABEL_FOR_DUMMY_FLIP0 : RESC_LABEL_FOR_DUMMY_FLIP1); }
    inline static CCellRescInternal* GetInstance(void) { return s_rescInternal; }

public:
	CCellRescInternal(void);
	~CCellRescInternal(void);

	int32_t  Init(const CellRescInitConfig* const initConfig);
	void     Exit(void);
	int32_t  SetDsts(const CellRescBufferMode displayMode, const CellRescDsts* const dsts);
	int32_t  SetDisplayMode(const CellRescBufferMode displayMode);
	int32_t  GetBufferSize(int32_t* const colorBuffers, int32_t* const vertexArray, int32_t* const fragmentShader);
	int32_t  SetBufferAddress(const void* const colorBuffers, const void* const vertexArray, const void* const fragmentShader);
	int32_t  SetSrc(const int32_t index, const CellRescSrc* const src);
	int32_t  SetConvertAndFlip(CellGcmContextData* con, const int32_t index, const int32_t indexPrev = -1);
	int32_t  SetPalInterpolateDropFlexRatio(const float ratio);
	int32_t  AdjustAspectRatio(const float horizontal, const float vertical);
	void     SetVBlankHandler(const CellRescHandler handler);
	void     SetFlipHandler(const CellRescHandler handler);
	void     SetWaitFlip(CellGcmContextData *con);
	system_time_t GetLastFlipTime(void);
	void     ResetFlipStatus(void);
	uint32_t GetFlipStatus(void);
	int32_t  CreateInterlaceTable(void* ea, const float srcH, const CellRescTableElement depth, const int length);
	
	int32_t  GcmSurface2RescSrc(const CellGcmSurface* const gcmSurface, CellRescSrc* const rescSrc);
	int32_t  VideoOutResolutionId2RescBufferMode(const CellVideoOutResolutionId resolutionId, CellRescBufferMode* const bufferMode);

	inline int32_t GetRegisterCount(void) const { return m_pCFragmentShader->GetRegisterCount(); }
	inline void    SetRegisterCount(int32_t regCount) { m_pCFragmentShader->SetRegisterCount(regCount); }

	inline int32_t GetNumColorBuffers(const CellRescBufferMode dstMode, const CellRescPalTemporalMode palTemporalMode, const uint32_t reserved) const {
		if(reserved != 0) return CELL_RESC_ERROR_BAD_ARGUMENT;
		return dstMode==CELL_RESC_720x576 ? 
			((palTemporalMode==CELL_RESC_PAL_60_INTERPOLATE || 
			  palTemporalMode==CELL_RESC_PAL_60_INTERPOLATE_30_DROP || 
			  palTemporalMode==CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE)
			 ? 6 : (palTemporalMode==CELL_RESC_PAL_60_DROP ? 3 : 2)) : 2;
	}
};


// ============================================================================
// Utility functions
// ============================================================================

namespace Util
{
	void    InitTimeBaseFrequency(void);
	system_time_t GetSystemTime(void);

	void    GlViewPort(CellGcmContextData* con, int x, int y, int w, int h);
	void    PrintDebugTimeSpan(int start, int end, int interval);
	void    PrintDebugTime(int idx);

	uint8_t RescBufferMode2SysutilResolutionId(CellRescBufferMode bufferMode);
	uint8_t RescDstFormat2SysutilFormat(CellRescDstFormat dstFormat);
	void    GetScreenSize(CellRescBufferMode mode, int32_t *width, int32_t *height);
	uint8_t GcmSurfaceFormat2GcmTextureFormat(uint8_t surfaceFormat, uint8_t surfaceType);
	int32_t CalculateSurfaceByteSize(CellRescBufferMode mode, CellRescDsts *dsts);
	void    NullPrintf(...);
	int32_t CreateInterlaceTable(void* ea, const float srcH, const float dstH, const CellRescTableElement depth, const int length);
	uint16_t FloatToHalf(const float val);

}; /* namespace Util */


#if 0
 namespace GcmTex
 {
 	void cellGcmSetTexture(CellGcmContextData* con, const uint8_t index, const CellGcmTexture *texture);
 	void cellGcmSetTextureFilter(CellGcmContextData* con, const uint8_t index, const uint16_t bias, const uint8_t min, const uint8_t mag, const uint8_t conv);
 	void cellGcmSetTextureAddress(CellGcmContextData* con, const uint8_t index, const uint8_t wraps, const uint8_t wrapt, const uint8_t wrapr, const uint8_t unsignedRemap, const uint8_t zfunc, const uint8_t gamma);
 	void cellGcmSetTextureControl(CellGcmContextData* con, const uint8_t index, const uint32_t enable, const uint16_t minlod, const uint16_t maxlod, const uint8_t maxaniso);
 }; /* namespace GcmTex */
#endif
// ============================================================================
// Macros
// ============================================================================

#ifdef SCE_INTERNAL_BUILD
# define SYSCLIB 1
#endif

#if SYSCLIB
# define _printf(...)     _sys_printf(__VA_ARGS__)
# define _memset          _sys_memset
# define _memcpy          _sys_memcpy
# define _strcpy          _sys_strcpy
# define _strcat          _sys_strcat
# define _sprintf         _sys_sprintf
# define _strlen          _sys_strlen
#else
# define _printf(...)     printf(__VA_ARGS__)
# define _memset          memset
# define _memcpy          memcpy
# define _strcpy          strcpy
# define _strcat          strcat
# define _sprintf         sprintf
# define _strlen          strlen
#endif

#if CELL_RESC_DEBUG
# define RESC_SET_TIME_STAMP(con, x)  GcmCmdTypePrefix::cellGcmSetTimeStamp(con, x)
# define RESC_GET_TIME_STAMP(x)       Util::PrintDebugTime(x)
# define RESC_GET_TIME_SPAN(start,end,interval)  Util::PrintDebugTimeSpan(start, end, interval)
# define RESC_DEBUG_PRINTF(...)   _printf("resc: " __VA_ARGS__)
# define DEBUG_PRINTF(...)        _printf(__VA_ARGS__)
# define PRINTF10(...)            if(GetInstance()->m_lastV60.idx < 10){ _printf(__VA_ARGS__); }
# define RESC_ASSERT(cond) do { if (!(cond)) { _printf("Assertation failed: '(%s) == false'", #cond); asm __volatile__("trap"); } } while (false)
# if CELL_RESC_DEBUG >= 2
#  define RESC_DEBUG_PRINTF2(...) _printf("resc: " __VA_ARGS__)
# else
#  define RESC_DEBUG_PRINTF2(...) cell::Resc::Util::NullPrintf(__VA_ARGS__)
# endif
#else
# define RESC_SET_TIME_STAMP(con, x)  
# define RESC_GET_TIME_STAMP(x)  
# define RESC_GET_TIME_SPAN(start,end,interval)
# define RESC_DEBUG_PRINTF(...)   cell::Resc::Util::NullPrintf(__VA_ARGS__)
# define RESC_DEBUG_PRINTF2(...)  cell::Resc::Util::NullPrintf(__VA_ARGS__)
# define DEBUG_PRINTF(...)        cell::Resc::Util::NullPrintf(__VA_ARGS__)
# define PRINTF10(...)            cell::Resc::Util::NullPrintf(__VA_ARGS__)
# define RESC_ASSERT(cond)
#endif

// works only when 'a' is 2^n
#define roundup(x,a) (((x)+(a)-1)&(~((a)-1)))

#define RESC_LOCK(name, timeout) {                                                     \
	int ret = sys_lwmutex_lock(&GetInstance()->m_lwmutex##name, timeout);              \
	if (ret != CELL_OK) { RESC_DEBUG_PRINTF("sys_lwmutex_lock failed: %d\n", ret); }   \
}

#define RESC_UNLOCK(name) {                                                            \
	int ret = sys_lwmutex_unlock(&GetInstance()->m_lwmutex##name);                     \
	if (ret != CELL_OK) { RESC_DEBUG_PRINTF("sys_lwmutex_unlock failed: %d\n", ret); } \
}

// ============================================================================

}; /* namespace Resc */
}; /* namespace cell */


#endif /* __CELL_RESC_INTERNAL_H__ */
