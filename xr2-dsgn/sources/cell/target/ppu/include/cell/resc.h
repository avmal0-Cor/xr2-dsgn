/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: resc.h
 * Description:
 *
 */

#ifndef __CELL_RESC_H__
#define __CELL_RESC_H__

#include <stdint.h>
#include <cell/gcm.h>
#include <sysutil/sysutil_sysparam.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*J エラー値 */
/*E Error macros */
#define CELL_RESC_ERROR_BASE                  (0x80210300)

#define CELL_RESC_ERROR_NOT_INITIALIZED       (CELL_RESC_ERROR_BASE | 0x1)
#define CELL_RESC_ERROR_REINITIALIZED         (CELL_RESC_ERROR_BASE | 0x2)
#define CELL_RESC_ERROR_BAD_ALIGNMENT         (CELL_RESC_ERROR_BASE | 0x3)
#define CELL_RESC_ERROR_BAD_ARGUMENT          (CELL_RESC_ERROR_BASE | 0x4)
#define CELL_RESC_ERROR_LESS_MEMORY           (CELL_RESC_ERROR_BASE | 0x5)
#define CELL_RESC_ERROR_GCM_FLIP_QUE_FULL     (CELL_RESC_ERROR_BASE | 0x6)
#define CELL_RESC_ERROR_BAD_COMBINATION       (CELL_RESC_ERROR_BASE | 0x7)


/*J emum 値 */
/*E Enum */
typedef enum CellRescResourcePolicy
{
	CELL_RESC_CONSTANT_VRAM          = (0 << 0), 
	CELL_RESC_MINIMUM_VRAM           = (1 << 0), 
	CELL_RESC_CONSTANT_GPU_LOAD      = (0 << 1),  // cannot use.
	CELL_RESC_MINIMUM_GPU_LOAD       = (1 << 1), 
} CellRescResourcePolicy;

typedef enum CellRescDstFormat
{
	CELL_RESC_SURFACE_A8R8G8B8       = CELL_GCM_SURFACE_A8R8G8B8,       // 8
	CELL_RESC_SURFACE_F_W16Z16Y16X16 = CELL_GCM_SURFACE_F_W16Z16Y16X16, // 11
}CellRescDstFormat;

typedef enum CellRescDisplayBufferMode
{
	CELL_RESC_720x480                = (1 << 0), 
	CELL_RESC_720x576                = (1 << 1), 
	CELL_RESC_1280x720               = (1 << 2), 
	CELL_RESC_1920x1080              = (1 << 3), 
} CellRescBufferMode;

typedef enum CellRescRatioConvertMode
{
	CELL_RESC_FULLSCREEN             = 0, 
	CELL_RESC_LETTERBOX              = 1, 
	CELL_RESC_PANSCAN                = 2, 
} CellRescRatioConvertMode;

typedef enum CellRescPalTemporalMode
{
	CELL_RESC_PAL_50                            = 0, 
	CELL_RESC_PAL_60_DROP                       = 1, 
	CELL_RESC_PAL_60_INTERPOLATE                = 2, 
	CELL_RESC_PAL_60_INTERPOLATE_30_DROP        = 3, 
	CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE  = 4, 
	CELL_RESC_PAL_60_FOR_HSYNC                  = 5, 
} CellRescPalTemporalMode;

typedef enum CellRescConvolutionFilterMode
{
	CELL_RESC_NORMAL_BILINEAR        = 0, 
	CELL_RESC_INTERLACE_FILTER       = 1, 
	CELL_RESC_3X3_GAUSSIAN           = 2, 
	CELL_RESC_2X3_QUINCUNX           = 3, 
	CELL_RESC_2X3_QUINCUNX_ALT       = 4, 
} CellRescConvolutionFilterMode;

typedef CellRescConvolutionFilterMode CellRescInterlaceFilterMode;  //E for backword compatibility

typedef enum CellRescTableElement
{
	CELL_RESC_ELEMENT_HALF           = 0, 
	CELL_RESC_ELEMENT_FLOAT          = 1, 
} CellRescTableElement;

typedef enum CellRescFlipMode
{
	CELL_RESC_DISPLAY_VSYNC          = 0, 
	CELL_RESC_DISPLAY_HSYNC          = 1, 
} CellRescFlipMode;


/*J 値設定用 構造体 */
/*E Structures */
typedef struct CellRescInitConfig
{
	size_t                   size;
	uint32_t                 resourcePolicy;
	uint32_t                 supportModes;
	uint32_t                 ratioMode;
	uint32_t                 palTemporalMode;
	uint32_t                 interlaceMode;
	uint32_t                 flipMode;
}CellRescInitConfig;

typedef struct CellRescSrc
{
	uint32_t                 format;
	uint32_t                 pitch;
	uint16_t                 width;
	uint16_t                 height;
	uint32_t                 offset;
} CellRescSrc;

typedef struct CellRescDsts
{
	uint32_t                 format;
	uint32_t                 pitch;
	uint32_t                 heightAlign;
} CellRescDsts;


/*J 標準API */
/*E Standard APIs */
int32_t  cellRescInit(const CellRescInitConfig* const initConfig);
void     cellRescExit(void);
int32_t  cellRescSetDsts(const CellRescBufferMode dstsMode, const CellRescDsts* const dsts);
int32_t  cellRescSetDisplayMode(const CellRescBufferMode bufferMode);
int32_t  cellRescGetNumColorBuffers(const CellRescBufferMode dstMode, const CellRescPalTemporalMode palTemporalMode, const uint32_t reserved);
int32_t  cellRescGetBufferSize(int32_t* const colorBuffers, int32_t* const vertexArray, int32_t* const fragmentShader);
int32_t  cellRescSetBufferAddress(const void* const colorBuffers, const void* const vertexArray, const void* const fragmentShader);
int32_t  cellRescSetSrc(const int32_t idx, const CellRescSrc* const src);
int32_t  cellRescSetConvertAndFlip(CellGcmContextData* con, const int32_t idx);
void     cellRescSetWaitFlip(CellGcmContextData* con);
system_time_t cellRescGetLastFlipTime(void);
void     cellRescResetFlipStatus(void);
uint32_t cellRescGetFlipStatus(void);
int32_t  cellRescGetRegisterCount(void);
void     cellRescSetRegisterCount(const int32_t regCount);
int32_t  cellRescSetPalInterpolateDropFlexRatio(const float ratio);
int32_t  cellRescCreateInterlaceTable(void* ea, const float srcH, const CellRescTableElement depth, const int length);
int32_t  cellRescAdjustAspectRatio(const float horizontal, const float vertical);

/*J イベントハンドラ登録 */
/*E Register event handler */
typedef void (*CellRescHandler)(uint32_t head);
void     cellRescSetVBlankHandler(const CellRescHandler handler);
void     cellRescSetFlipHandler(const CellRescHandler handler);

/*J サービスAPI */
/*E Utility functions */
int32_t  cellRescGcmSurface2RescSrc(const CellGcmSurface* const gcmSurface, 
									CellRescSrc* const rescSrc);
int32_t  cellRescVideoOutResolutionId2RescBufferMode(const CellVideoOutResolutionId resolutionId, 
													 CellRescBufferMode* const bufferMode);

#ifdef __cplusplus
}

#ifndef CELL_GCM_DENY_IMPLICIT_ARG
#define CELL_RESC_INLINE extern inline __attribute__((always_inline))

/*J c++ 多重定義を用いた inline ラッパー : c++ 時のみ使用可 */
/*E Inline wrappers which uses c++ function overloading : can use only in c++ */
CELL_RESC_INLINE int32_t cellRescSetConvertAndFlip(const int32_t idx)
{
	return cellRescSetConvertAndFlip(gCellGcmCurrentContext, idx);
}

CELL_RESC_INLINE void cellRescSetWaitFlip(void)
{
	cellRescSetWaitFlip(gCellGcmCurrentContext);
}

#undef CELL_RESC_INLINE
#endif /* CELL_GCM_DENY_IMPLICIT_ARG */
#endif /* __cplusplus */

#endif /* __CELL_RESC_H__ */
