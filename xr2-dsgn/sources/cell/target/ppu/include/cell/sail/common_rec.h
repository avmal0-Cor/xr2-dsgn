/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_COMMON_REC_H__
#define __CELL_SAIL_COMMON_REC_H__


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

enum {
    CELL_SAIL_EVENT_RECORDER_CALL_COMPLETED = 2,
    CELL_SAIL_EVENT_RECORDER_STATE_CHANGED  = 3,
};


enum {
    CELL_SAIL_VIDEO_FRAME_RATE_100HZ         = 8,
    CELL_SAIL_VIDEO_FRAME_RATE_120000_1001HZ = 9,
    CELL_SAIL_VIDEO_FRAME_RATE_120HZ         = 10,
};


typedef struct CellSailCommand {
    uint32_t  key;
    uintptr_t value;
} CellSailCommand;

#define CELL_SAIL_MAX_NUM_FILTER_COMMANDS (16)

typedef struct CellSailFeederFrameInfo {
    void            *pFrame;
    size_t           size;
    uint32_t         sampleNum;
    uint32_t         reserved0;
    uint64_t         timeStamp;
    uint64_t         reserved1;
    uint32_t         commandNum;
    CellSailCommand *pCommands;
    uint32_t         reserved2;
    uint32_t         reserved3;
} CellSailFeederFrameInfo;

typedef CellSailFeederFrameInfo CellSailCaptureFrameInfo;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_COMMON_REC_H__ */
