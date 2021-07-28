/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_GRAPHICS_ADAPTER_H__
#define __CELL_SAIL_GRAPHICS_ADAPTER_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

enum {
    CELL_SAIL_GRAPHICS_ADAPTER_FIELD_TOP       = 0,
    CELL_SAIL_GRAPHICS_ADAPTER_FIELD_BOTTOM    = 1,
    CELL_SAIL_GRAPHICS_ADAPTER_FIELD_DONT_CARE = 2,
};

typedef int (*CellSailGraphicsAdapterFuncMakeup)(void *pArg);
typedef int (*CellSailGraphicsAdapterFuncCleanup)(void *pArg);
typedef int (*CellSailGraphicsAdapterFuncFormatChanged)(
    void                *pArg,
    CellSailVideoFormat *pFormat,
    uint32_t             count);
typedef int (*CellSailGraphicsAdapterFuncAllocFrame)(void     *pArg,
                                                     size_t    size,
                                                     int       num,
                                                     uint8_t **ppFrame);
typedef int (*CellSailGraphicsAdapterFuncFreeFrame)(void     *pArg,
                                                    int       num,
                                                    uint8_t **ppFrame);

typedef struct CellSailGraphicsAdapterFuncs {
    CellSailGraphicsAdapterFuncMakeup        pMakeup;
    CellSailGraphicsAdapterFuncCleanup       pCleanup;
    CellSailGraphicsAdapterFuncFormatChanged pFormatChanged;
    CellSailGraphicsAdapterFuncAllocFrame    pAlloc;
    CellSailGraphicsAdapterFuncFreeFrame     pFree;
} CellSailGraphicsAdapterFuncs;

typedef struct CellSailGraphicsFrameInfo {
    uint8_t *pBuffer;
    uint32_t sessionId;
    uint32_t tag;
    int      status;
    uint64_t pts;
} CellSailGraphicsFrameInfo;

typedef struct CellSailGraphicsAdapter {
    uint64_t internalData[32];
} CellSailGraphicsAdapter;

int cellSailGraphicsAdapterInitialize(
    CellSailGraphicsAdapter            *pSelf,
    const CellSailGraphicsAdapterFuncs *pFuncs,
    void                               *pArg);
                                   
int cellSailGraphicsAdapterFinalize(
    CellSailGraphicsAdapter   *pSelf);

int cellSailGraphicsAdapterSetPreferredFormat(
    CellSailGraphicsAdapter   *pSelf,
    const CellSailVideoFormat *pFormat);

int cellSailGraphicsAdapterGetFrame(
    CellSailGraphicsAdapter   *pSelf,
    CellSailGraphicsFrameInfo *pInfo);

int cellSailGraphicsAdapterGetFrame2(
    CellSailGraphicsAdapter   *pSelf,
    CellSailGraphicsFrameInfo *pInfo,
    CellSailGraphicsFrameInfo *pPrevInfo,
    uint64_t                  *pFlipTime,
    uint64_t                   flags);

int cellSailGraphicsAdapterGetFormat(
    CellSailGraphicsAdapter *pSelf,
    CellSailVideoFormat     *pFormat);

void cellSailGraphicsAdapterUpdateAvSync(
    CellSailGraphicsAdapter *pSelf,
    uint32_t                 tag,
    uint64_t                 timeStamp);

int cellSailGraphicsAdapterPtsToTimePosition(
    CellSailGraphicsAdapter *pSelf,
    uint64_t                 pts,
    uint64_t                *pTime);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_GRAPHICS_ADAPTER_H__ */
