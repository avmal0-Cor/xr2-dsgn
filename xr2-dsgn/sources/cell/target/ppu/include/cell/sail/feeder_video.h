/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_FEEDER_VIDEO_H__
#define __CELL_SAIL_FEEDER_VIDEO_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef int (*CellSailFeederVideoFuncMakeup)(void *pArg);
typedef int (*CellSailFeederVideoFuncCleanup)(void *pArg);

typedef void (*CellSailFeederVideoFuncOpen)(
    void                      *pArg,
    const CellSailVideoFormat *pSuggestedFormat,
    uint64_t                   reserved);
typedef void (*CellSailFeederVideoFuncClose)(void *pArg);
typedef void (*CellSailFeederVideoFuncStart)(void    *pArg,
                                             uint64_t reserved);
typedef void (*CellSailFeederVideoFuncStop)(void    *pArg,
                                            uint64_t reserved);

typedef void (*CellSailFeederVideoFuncCancel)(void *pArg);
typedef int (*CellSailFeederVideoFuncGetFormat)(
    void                *pArg,
    uint64_t             reserved,
    CellSailVideoFormat *pFormat);

typedef int (*CellSailFeederVideoFuncCheckout)(
    void                     *pArg,
    CellSailFeederFrameInfo **pInfo);
typedef int (*CellSailFeederVideoFuncCheckin)(
    void                     *pArg,
    CellSailFeederFrameInfo *pInfo);
typedef int (*CellSailFeederVideoFuncClear)(void  *pArg);


typedef struct CellSailFeederVideoFuncs {
    CellSailFeederVideoFuncMakeup   pMakeup;
    CellSailFeederVideoFuncCleanup  pCleanup;

    CellSailFeederVideoFuncOpen      pOpen;
    CellSailFeederVideoFuncClose     pClose;
    CellSailFeederVideoFuncStart     pStart;
    CellSailFeederVideoFuncStop      pStop;
    CellSailFeederVideoFuncCancel    pCancel;
    CellSailFeederVideoFuncGetFormat pGetFormat;

    CellSailFeederVideoFuncCheckout pCheckout;
    CellSailFeederVideoFuncCheckin  pCheckin;
    CellSailFeederVideoFuncClear    pClear;
} CellSailFeederVideoFuncs;


typedef struct CellSailFeederVideoAttribute {
    size_t   thisSize;
    uint32_t reserved;
} CellSailFeederVideoAttribute;


typedef struct CellSailFeederVideo {
    uint64_t internalData[64];
} CellSailFeederVideo;


int cellSailFeederVideoInitialize(
    CellSailFeederVideo            *pSelf,
    const CellSailFeederVideoFuncs *pFuncs,
    void                           *pArg,
    CellSailFeederVideoAttribute   *pAttr); // must be NULL
int cellSailFeederVideoFinalize(CellSailFeederVideo* pSelf);

void cellSailFeederVideoNotifyCallCompleted(CellSailFeederVideo *pSelf,
                                            int                status);
void cellSailFeederVideoNotifyFrameOut(CellSailFeederVideo *pSelf,
                                       uint32_t           num);
void cellSailFeederVideoNotifySessionEnd(CellSailFeederVideo *pSelf);

void cellSailFeederVideoNotifySessionError(CellSailFeederVideo *pSelf,
                                           int                  error,
                                           bool                 isFatal);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_FEEDER_VIDEO_H__*/
/*[eof]*/
