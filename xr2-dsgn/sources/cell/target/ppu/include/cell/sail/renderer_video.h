/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_RENDERER_VIDEO_H__
#define __CELL_SAIL_RENDERER_VIDEO_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


typedef struct CellSailVideoFrameInfo {
    void     *pPic;
    int       status;
    uint64_t  pts;
    uint64_t  reserved;
    uint16_t  interval;
    uint8_t   structure;
    int8_t    repeatNum;
    uint8_t   reserved2[4];
} CellSailVideoFrameInfo;


// main
typedef int (*CellSailRendererVideoFuncMakeup)(void *pArg);
typedef int (*CellSailRendererVideoFuncCleanup)(void *pArg);
typedef void (*CellSailRendererVideoFuncOpen)(
    void                      *pArg,
    const CellSailVideoFormat *pInfo,
    uint32_t                   frameNum,
    uint32_t                   minFrameNum);
typedef void (*CellSailRendererVideoFuncClose)(void *pArg);
typedef void (*CellSailRendererVideoFuncStart)(void *pArg,
                                              bool  buffering);
typedef void (*CellSailRendererVideoFuncStop)(void *pArg,
                                              bool  flush,
                                              bool  keepRendering);
typedef void (*CellSailRendererVideoFuncCancel)(void *pArg);
typedef int (*CellSailRendererVideoFuncCheckout)(void                    *pArg,
                                                 CellSailVideoFrameInfo **pInfo);
typedef int (*CellSailRendererVideoFunkCheckin)(void                   *pArg,
                                                CellSailVideoFrameInfo *pInfo);

typedef struct CellSailRendererVideoFuncs {
    CellSailRendererVideoFuncMakeup   pMakeup;
    CellSailRendererVideoFuncCleanup  pCleanup;
    CellSailRendererVideoFuncOpen     pOpen;
    CellSailRendererVideoFuncClose    pClose;
    CellSailRendererVideoFuncStart    pStart;
    CellSailRendererVideoFuncStop     pStop;
    CellSailRendererVideoFuncCancel   pCancel;
    CellSailRendererVideoFuncCheckout pCheckout;
    CellSailRendererVideoFunkCheckin  pCheckin;
} CellSailRendererVideoFuncs;

typedef struct CellSailRendererVideo {
    uint64_t internalData[32];
} CellSailRendererVideo;

typedef struct CellSailRendererVideoAttribute {
    size_t               thisSize;
    CellSailVideoFormat *pPreferredFormat;
} CellSailRendererVideoAttribute;

// public 
int cellSailRendererVideoInitialize(CellSailRendererVideo                *pSelf,
                                    const CellSailRendererVideoFuncs     *pFuncs,
                                    void                                 *pArg,
                                    const CellSailRendererVideoAttribute *pAttr);
int cellSailRendererVideoFinalize(CellSailRendererVideo* pSelf);

void cellSailRendererVideoNotifyCallCompleted(CellSailRendererVideo *pSelf,
                                              int                    status);

void cellSailRendererVideoNotifyFrameDone(CellSailRendererVideo *pSelf);

void cellSailRendererVideoNotifyOutputEos(CellSailRendererVideo *pSelf);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_RENDERER_VIDEO_H__*/

