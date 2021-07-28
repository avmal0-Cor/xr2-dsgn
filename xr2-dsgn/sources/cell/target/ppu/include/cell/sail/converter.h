/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_CONVERTER_H__
#define __CELL_SAIL_CONVERTER_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

typedef void (*CellSailVideoConverterFuncProcessDone)(void *pArg);

typedef struct CellSailVideoConverter {
    uint64_t internalData[32];
} CellSailVideoConverter;


int cellSailVideoConverterCanProcess(CellSailVideoConverter *pSelf);
int cellSailVideoConverterProcess(CellSailVideoConverter *pSelf,
                                  void                   *pIn,
                                  size_t                  inSize,
                                  void                   *pOut,
                                  void                   *pReserved,
                                  usecond_t               waitTimeout,
                                  uint32_t                userParam);

int cellSailVideoConverterCanGetResult(CellSailVideoConverter *pSelf);
int cellSailVideoConverterGetResult(CellSailVideoConverter *pSelf,
                                    usecond_t               waitTimeout,
                                    int                    *pResult,
                                    uint32_t               *pUserParam);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_CONVERTER_H__ */
/*[eof]*/
