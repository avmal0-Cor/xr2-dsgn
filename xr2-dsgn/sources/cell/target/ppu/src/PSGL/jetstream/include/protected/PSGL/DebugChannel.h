/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Export for the _psglDebugChannel variable that allows debugging of PSGL/RSX crashes.
 */

#ifndef _PSGL_DEBUGCHANNEL_H
#define _PSGL_DEBUGCHANNEL_H

#ifdef __cplusplus
extern "C"
{
#endif


    // Set _psglDebugChannel to:
    //   1 to cause Finish after every pushbuffer method
    //   2 to cause flush after every pushbuffer method
    //   3 to increment the fence reference after every pushbuffer method
    extern int _psglDebugChannel;

#ifdef __cplusplus
}
#endif

#endif
