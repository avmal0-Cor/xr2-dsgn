/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYSUTIL_REMOTEPLAY_H_
#define _CELL_SYSUTIL_REMOTEPLAY_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_REMOTEPLAY_STATUS_LOADING         (0x00000000)
#define CELL_REMOTEPLAY_STATUS_WAIT            (0x00000001)
#define CELL_REMOTEPLAY_STATUS_RUNNING         (0x00000002)
#define CELL_REMOTEPLAY_STATUS_UNLOADING       (0x00000003)
#define CELL_REMOTEPLAY_STATUS_FATALERROR      (0x00000004)
#define CELL_REMOTEPLAY_STATUS_PREMOEND        (0x00000005)

#define CELL_REMOTEPLAY_M4V_MEMORY_CONTAINER_SIZE     (8*1024*1024)
#define CELL_REMOTEPLAY_AVC_MEMORY_CONTAINER_SIZE     (10*1024*1024)

#define CELL_REMOTEPLAY_ERROR_INTERNAL         (0x80029830)

int cellRemotePlayGetStatus ( void );

int cellRemotePlayGetComparativeVolume( float *pComparatveAudioVolume );

int cellRemotePlaySetComparativeVolume( float comparatveAudioVolume );

int cellRemotePlayBreak ( void );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_REMOTEPLAY_H_*/
