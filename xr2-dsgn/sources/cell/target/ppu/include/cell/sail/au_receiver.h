/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_AU_RECEIVER_H__
#define __CELL_SAIL_AU_RECEIVER_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


enum {
    CELL_SAIL_SYNC_MODE_REPEAT      = 1<<0,
    CELL_SAIL_SYNC_MODE_SKIP        = 1<<1,
};


typedef struct CellSailAuReceiver {
    uint64_t internalData[64];
} CellSailAuReceiver;


int cellSailAuReceiverInitialize(CellSailAuReceiver  *pSelf,
                                 void                *pFuncs,
                                 void                *pArg,
                                 int                  mode);
int cellSailAuReceiverFinalize(CellSailAuReceiver* pSelf);


int cellSailAuReceiverGet(CellSailAuReceiver    *pSelf,
                          uint64_t               mode,
                          uint32_t               sessionId,
                          uint64_t               timestamp,
                          const CellSailAuInfo **pFrame);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_AU_RECEIVER_H__*/
/*[eof]*/
