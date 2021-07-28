/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_H__
#define __CELL_GCM_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <alloca.h>
#include <sys/timer.h>
#include <Cg/cg.h>

#ifndef CELL_GCM_MEMCPY  // [
# define CELL_GCM_MEMCPY __builtin_memcpy
#endif  // ]

#include <cell/gcm/gcm_error.h>
#include <cell/gcm/gcm_macros.h>
#include <cell/gcm/gcm_helper.h>
#include <cell/gcm/gcm_methods.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cell/gcm/gcm_enum.h>
#include <cell/gcm/gcm_struct.h>


/*	global command context	*/
extern struct CellGcmContextData *gCellGcmCurrentContext;

#ifdef CELL_GCM_DEBUG
/*  debug library functions */
extern void (*gCellGcmDebugCallback)(struct CellGcmContextData*);
void cellGcmDebugFinish(struct CellGcmContextData*);

extern void (*gCellGcmDebugCheckCallback)(uint32_t* cmd);
void cellGcmDebugCheck(uint32_t* cmd);
void cellGcmDebugCheckEnable(uint32_t enable);
uint32_t cellGcmIsDebugCheckEnabled(void);
#endif 
int32_t cellGcmDumpGraphicsError(void);

/*	system functions	*/
int32_t cellGcmInitSystemMode(const uint64_t mode);
int32_t cellGcmInit(const uint32_t cmdSize, const uint32_t ioSize, const void *ioAddress);
int32_t cellGcmAddressToOffset(const void *address, uint32_t *offset);
int32_t cellGcmMapMainMemory(const void *address, const uint32_t size, uint32_t *offset);
void cellGcmGetConfiguration(CellGcmConfig *config);
CellGcmControl *cellGcmGetControlRegister(void);
uint32_t cellGcmGetTiledPitchSize(const uint32_t size);

void cellGcmSetVBlankHandler(void (*handler)(const uint32_t head));
void cellGcmSetSecondVHandler(void (*handler)(const uint32_t head));
void cellGcmSetGraphicsHandler(void (*handler)(const uint32_t val));
void cellGcmSetFlipHandler(void (*handler)(const uint32_t head));
void cellGcmSetQueueHandler(void (*handler)(const uint32_t head));
void cellGcmSetUserHandler(void (*handler)(const uint32_t cause));
void cellGcmSetDebugOutputLevel(int32_t level);

void cellGcmGetOffsetTable(CellGcmOffsetTable *table);
int32_t cellGcmIoOffsetToAddress(uint32_t ioOffset, void **address);
int32_t cellGcmSortRemapEaIoAddress(void);
int32_t cellGcmMapEaIoAddress(const void *ea, const uint32_t io, const uint32_t size);
int32_t cellGcmUnmapIoAddress(const uint32_t io);
int32_t cellGcmUnmapEaIoAddress(const void *ea);
int32_t cellGcmMapEaIoAddressWithFlags(const void *ea, const uint32_t io, const uint32_t size, uint32_t userflags);
int32_t cellGcmCallbackForSnc(struct CellGcmContextData* con, uint32_t count);

int32_t cellGcmMapLocalMemory(void **address, uint32_t *size);

/*	display functions	*/
int32_t cellGcmSetDisplayBuffer(const uint8_t id, const uint32_t offset, const uint32_t pitch, const uint32_t width, const uint32_t height);
system_time_t cellGcmGetLastFlipTime(void);
void cellGcmSetFlipMode(const uint32_t mode);
int32_t cellGcmSetFlipImmediate(const uint8_t id);
void cellGcmResetFlipStatus(void);
void cellGcmSetFlipStatus(void);
uint32_t cellGcmGetFlipStatus(void);
void cellGcmSetSecondVFrequency(const uint32_t freq);
int32_t cellGcmGetDisplayBufferByFlipIndex(const uint32_t qid);
uint64_t cellGcmGetVBlankCount(void);
system_time_t cellGcmGetLastSecondVTime(void);
uint32_t cellGcmGetCurrentField(void);
void cellGcmSetVBlankFrequency(const uint32_t freq);
int32_t cellGcmGetCurrentDisplayBufferId(uint8_t *id);


/* flow control functions */
uint32_t *cellGcmGetCurrentBuffer(void);
void cellGcmSetCurrentBuffer(const uint32_t *addr, const size_t size);
void cellGcmSetDefaultCommandBuffer(void);
void cellGcmSetUserCallback(CellGcmContextCallback callback);
void cellGcmSetupContextData(CellGcmContextData *context, const uint32_t *addr, const uint32_t size, CellGcmContextCallback callback);
int32_t cellGcmSetDefaultCommandBufferAndSegmentWordSize(const uint32_t bufferSize, const uint32_t segmentSize);
uint32_t cellGcmGetDefaultCommandWordSize(void);
uint32_t cellGcmGetDefaultSegmentWordSize(void);
int32_t cellGcmInitDefaultFifoMode(int32_t mode);
int32_t cellGcmSetDefaultFifoSize(const uint32_t bufferSize, const uint32_t segmentSize);


/* cg functions */
#include<cell/gcm/gcm_cg_func.h>

/*
render command functions
*/
uint32_t *cellGcmGetLabelAddress(const uint8_t index);
uint64_t cellGcmGetTimeStamp(const uint32_t index);
uint32_t cellGcmGetReport(const uint32_t type, const uint32_t index);
void cellGcmSetTile(const uint8_t index, const uint8_t location, const uint32_t offset, const uint32_t size, const uint32_t pitch, const uint8_t comp, const uint16_t base, const uint8_t bank);
void cellGcmSetInvalidateTile(const uint8_t index);
void cellGcmSetZcull(const uint8_t index, const uint32_t offset, const uint32_t width, const uint32_t height, const uint32_t cullStart, const uint32_t zFormat, const uint32_t aaFormat, const uint32_t zCullDir, const uint32_t zCullFormat, const uint32_t sFunc, const uint32_t sRef, const uint32_t sMask);

int32_t cellGcmSetTileInfo(const uint8_t index, const uint8_t location, const uint32_t offset, const uint32_t size, const uint32_t pitch, const uint8_t comp, const uint16_t base, const uint8_t bank);
int32_t cellGcmBindTile(const uint8_t index);
int32_t cellGcmUnbindTile(const uint8_t index);
int32_t cellGcmBindZcull(const uint8_t index, const uint32_t offset, const uint32_t width, const uint32_t height, const uint32_t cullStart, const uint32_t zFormat, const uint32_t aaFormat, const uint32_t zCullDir, const uint32_t zCullFormat, const uint32_t sFunc, const uint32_t sRef, const uint32_t sMask);
int32_t cellGcmUnbindZcull(const uint8_t index);
CellGcmReportData *cellGcmGetReportDataAddress(const uint32_t index);

/*
  report data functions
*/
CellGcmReportData *cellGcmGetReportDataAddressLocation(const uint32_t index, const uint32_t location);
uint64_t cellGcmGetTimeStampLocation(const uint32_t index, const uint32_t location);
uint32_t cellGcmGetReportDataLocation(const uint32_t index, const uint32_t location);


/*
data transfer and format conversion functions
*/
void cellGcmConvertSwizzleFormat(void *swizzledTexture, const void *texture, const uint32_t  dstx0, const uint32_t  dsty0, const uint32_t  dstz0, const uint32_t  dstWidth, const uint32_t  dstHeight, const uint32_t  dstDepth, const uint32_t  srcx0, const uint32_t  srcy0, const uint32_t  srcz0, const uint32_t  srcx1, const uint32_t  srcy1, const uint32_t  srcz1, const uint32_t  srcWidth, const uint32_t  srcHeight, const uint32_t  srcDepth, const uint32_t  dstLog2cdepth, const uint32_t  srcColordepth, const uint32_t  border, const uint32_t  dim, void (*copyTexel)(void *dst, const void *src));

/*
  notify data functions
*/
CellGcmNotifyData *cellGcmGetNotifyDataAddress(const uint32_t index);

/*
  tile/zcull/display info
*/
const CellGcmTileInfo *cellGcmGetTileInfo(void);
const CellGcmZcullInfo *cellGcmGetZcullInfo(void);
const CellGcmDisplayInfo *cellGcmGetDisplayInfo(void);



// C style functions
#include <cell/gcm/gcm_command_c.h>

/* Cg extension */
#include <cell/gcm/gcm_cg_extension.h>


#ifdef __cplusplus
}
#endif /* __cplusplus */


// C++ style functions
#ifdef __cplusplus

namespace cell {
namespace Gcm {

#include <cell/gcm/gcm_command_cpp_implicit.h>
#include <cell/gcm/gcm_command_cpp_explicit.h>

} /* namespace Gcm */
} /* namespace cell */

#endif /* __cplusplus */


#endif	/* __CELL_GCM_H__ */
