/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYSUTIL_SUBDISPLAY_H_
#define _CELL_SYSUTIL_SUBDISPLAY_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_SUBDISPLAY_ERROR_OUT_OF_MEMORY    ( 0x80029851 )
#define CELL_SUBDISPLAY_ERROR_FATAL            ( 0x80029852 )
#define CELL_SUBDISPLAY_ERROR_NOT_FOUND        ( 0x80029853 )
#define CELL_SUBDISPLAY_ERROR_INVALID_VALUE    ( 0x80029854 )
#define CELL_SUBDISPLAY_ERROR_NOT_INITIALIZED  ( 0x80029855 )

#define CELL_SUBDISPLAY_ERROR_SET_SAMPLE       ( 0x80029860 )
#define CELL_SUBDISPLAY_ERROR_AUDIOOUT_IS_BUSY ( 0x80029861 )

/* num of registered PSP is zero */	
#define CELL_SUBDISPLAY_ERROR_ZERO_REGISTERED  ( 0x80029813 )

/* obsolete */
#define CELL_REMOTEPLAY_ERROR_ZERO_REGISTERED  ( 0x80029813 )

#define CELL_SUBDISPLAY_STATUS_JOIN           ( 0x00000001 )
#define CELL_SUBDISPLAY_STATUS_LEAVE          ( 0x00000002 )
#define CELL_SUBDISPLAY_STATUS_FATALERROR     ( 0x00000003 )

/* The Value Of version @ CellSubDisplayParam */
#define CELL_SUBDISPLAY_VERSION_0001          ( 0x00000001 )
#define CELL_SUBDISPLAY_VERSION_0002          ( 0x00000002 )

/* The Value Of mode @ CellSubDisplayParam */
#define CELL_SUBDISPLAY_MODE_REMOTEPLAY       ( 0x00000001 )

/* The Value Of videoCodec @ CellSubDisplayVideoParam */
#define CELL_SUBDISPLAY_VIDEO_FORMAT_A8R8G8B8 ( 0x00000001 )
#define CELL_SUBDISPLAY_VIDEO_FORMAT_R8G8B8A8 ( 0x00000002 )

/* The Value Of aspectRatio @ CellSubDisplayVideoParam */
#define CELL_SUBDISPLAY_VIDEO_ASPECT_RATIO_16_9   (0x0000)
#define CELL_SUBDISPLAY_VIDEO_ASPECT_RATIO_4_3    (0x0001)

/* The Value Of videoMode @ CellSubDisplayVideoParam */
#define CELL_SUBDISPLAY_VIDEO_MODE_SETDATA  (0x0000)
#define CELL_SUBDISPLAY_VIDEO_MODE_CAPTURE  (0x0001)

/* The Value Of audioMode @ CellSubDisplayAudioParam */
#define CELL_SUBDISPLAY_AUDIO_MODE_SETDATA  (0x0000)
#define CELL_SUBDISPLAY_AUDIO_MODE_CAPTURE  (0x0001)

#define CELL_SUBDISPLAY_0001_MEMORY_CONTAINER_SIZE     (8*1024*1024)
#define CELL_SUBDISPLAY_0002_MEMORY_CONTAINER_SIZE     (10*1024*1024)

typedef void (*CellSubDisplayHandler)( int cbMsg, uint64_t cbParam, void *userdata );

typedef struct
{
	int   format;
	int   width;
	int   height;
	int   pitch;
	int   aspectRatio;
	int   videoMode;

} CellSubDisplayVideoParam;

typedef struct
{
	int   ch;
	int   audioMode;

} CellSubDisplayAudioParam;

typedef struct
{
	int version;
	int mode;
	int nGroup;
	int nPeer;
	CellSubDisplayVideoParam videoParam;
	CellSubDisplayAudioParam audioParam;

} CellSubDisplayParam;

#define CELL_SUBDISPLAY_NICKNAME_LEN (256)
typedef struct {
	char data[CELL_SUBDISPLAY_NICKNAME_LEN];
} CellSubDisplayNickname;

#define CELL_SUBDISPLAY_PSPID_SIZE (16)
typedef struct
{
	char data[CELL_SUBDISPLAY_PSPID_SIZE];

} CellSubDisplayPSPId;

typedef struct
{
	uint64_t                sessionId;
	uint32_t                portNo;
	CellSubDisplayPSPId     pspId;
	CellSubDisplayNickname  pspNickname;

} CellSubDisplayPeerInfo;

int cellSubDisplayGetRequiredMemory ( CellSubDisplayParam* pParam );

int cellSubDisplayInit ( CellSubDisplayParam*   pParam,
						 CellSubDisplayHandler  func,
						 void*                  userdata,
						 sys_memory_container_t	container );

int cellSubDisplayEnd ( void );

int cellSubDisplayStart ( void );

int cellSubDisplayStop ( void );

int cellSubDisplayGetVideoBuffer ( int groupId, void **ppVideoBuf, size_t* pSize );

int cellSubDisplayAudioOutBlocking( int groupId, 
								    void *pvData,
								    int  samples );

int cellSubDisplayAudioOutNonBlocking( int groupId, 
									   void *pvData,
									   int  samples );

int cellSubDisplayGetPeerNum ( int groupId );

int cellSubDisplayGetPeerList ( int groupId, CellSubDisplayPeerInfo* pInfo, int* pNum );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SUBDISPLAY_H_*/
