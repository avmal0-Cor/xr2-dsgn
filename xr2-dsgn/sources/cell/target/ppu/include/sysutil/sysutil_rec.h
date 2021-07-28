/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYSUTIL_REC_H_
#define _CELL_SYSUTIL_REC_H_

#include <sys/types.h>
#include <sdk_version.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/* container size */
#define CELL_REC_MIN_MEMORY_CONTAINER_SIZE ( 0 )

#if CELL_SDK_VERSION < 0x300000
#define CELL_REC_MAX_MEMORY_CONTAINER_SIZE ( 9 * 1024 * 1024 )
#else
#define CELL_REC_MAX_MEMORY_CONTAINER_SIZE ( 16 * 1024 * 1024 )
#endif

/* error */
#define CELL_REC_ERROR_OUT_OF_MEMORY  ( 0x8002c501 )
#define CELL_REC_ERROR_FATAL          ( 0x8002c502 )
#define CELL_REC_ERROR_INVALID_VALUE  ( 0x8002c503 )
#define CELL_REC_ERROR_FILE_OPEN      ( 0x8002c504 )
#define CELL_REC_ERROR_FILE_WRITE     ( 0x8002c505 )
#define CELL_REC_ERROR_INVALID_STATE  ( 0x8002c506 )
#define CELL_REC_ERROR_FILE_NO_DATA   ( 0x8002c507 )


/* type, constant definision */

typedef struct
{
	struct CellSpurs *pSpurs;
	int spu_usage_rate;
	uint8_t priority[8];
} CellRecSpursParam;

typedef struct
{
	int option;
	union {
		int ppu_thread_priority;
		int spu_thread_priority;
		int capture_priority;
		int use_system_spu;
		int fit_to_youtube;
		int xmb_bgm;
		int mpeg4_fast_encode;
		uint32_t ring_sec;
		int video_input;
		int audio_input;
		int audio_input_mix_vol;
		int reduce_memsize;
		int show_xmb;
		char *metadata_filename;
		CellRecSpursParam *pSpursParam;

		uint64_t  dummy;
	} value;
} CellRecOption;

typedef struct
{
	int videoFmt;
	int audioFmt;
	int numOfOpt;
	CellRecOption *pOpt;
} CellRecParam;


/* option type ( in CellRecOption ) */
#define CELL_REC_OPTION_PPU_THREAD_PRIORITY  (1)
#define CELL_REC_OPTION_SPU_THREAD_PRIORITY  (2)
#define CELL_REC_OPTION_CAPTURE_PRIORITY     (3)
#define CELL_REC_OPTION_USE_SYSTEM_SPU       (4)
#define CELL_REC_OPTION_FIT_TO_YOUTUBE       (5)
#define CELL_REC_OPTION_XMB_BGM              (6)
#define CELL_REC_OPTION_RING_SEC             (7)
#define CELL_REC_OPTION_MPEG4_FAST_ENCODE    (8)
#define CELL_REC_OPTION_VIDEO_INPUT          (9)
#define CELL_REC_OPTION_AUDIO_INPUT          (10)
#define CELL_REC_OPTION_AUDIO_INPUT_MIX_VOL  (11)
#define CELL_REC_OPTION_REDUCE_MEMSIZE       (12)
#define CELL_REC_OPTION_SHOW_XMB             (13)
#define CELL_REC_OPTION_METADATA_FILENAME    (14)
#define CELL_REC_OPTION_SPURS                (15)

/* option value: default ppu/spu thread priority */
#define CELL_REC_PARAM_PPU_THREAD_PRIORITY_DEFAULT ( 400 )
#define CELL_REC_PARAM_SPU_THREAD_PRIORITY_DEFAULT ( 60 )

/* option value: capture priority */
#define CELL_REC_PARAM_CAPTURE_PRIORITY_HIGHEST              (0)
#define CELL_REC_PARAM_CAPTURE_PRIORITY_EXCEPT_NOTIFICATION  (1)

/* option value: use system spu */
#define CELL_REC_PARAM_USE_SYSTEM_SPU_DISABLE (0)
#define CELL_REC_PARAM_USE_SYSTEM_SPU_ENABLE  (1)
/* obsolete value: typographical error */
#define CELL_REC_PARAM_USE_SYSTEM_SPU_ENALBE  (1)

/* option value: xmb bgm */
#define CELL_REC_PARAM_XMB_BGM_DISABLE (0)
#define CELL_REC_PARAM_XMB_BGM_ENABLE  (1)

/* option value: fast encode */
#define CELL_REC_PARAM_MPEG4_FAST_ENCODE_DISABLE (0)
#define CELL_REC_PARAM_MPEG4_FAST_ENCODE_ENABLE  (1)

/* option value: video input mode */
#define CELL_REC_PARAM_VIDEO_INPUT_DISABLE   (0)
#define CELL_REC_PARAM_VIDEO_INPUT_ARGB_4_3  (1)
#define CELL_REC_PARAM_VIDEO_INPUT_ARGB_16_9 (2)
#define CELL_REC_PARAM_VIDEO_INPUT_RGBA_4_3  (3)
#define CELL_REC_PARAM_VIDEO_INPUT_RGBA_16_9 (4)
#define CELL_REC_PARAM_VIDEO_INPUT_YUV420PLANAR_16_9 (5)

/* option value: audio input mode */
#define CELL_REC_PARAM_AUDIO_INPUT_DISABLE (0)
#define CELL_REC_PARAM_AUDIO_INPUT_ENABLE  (1)

#define CELL_REC_PARAM_AUDIO_INPUT_MIX_VOL_MIN (0)
#define CELL_REC_PARAM_AUDIO_INPUT_MIX_VOL_MAX (100)

/* option value: reduce memory size (but some utilities cannot be called in recording) */
#define CELL_REC_PARAM_REDUCE_MEMSIZE_DISABLE (0)
#define CELL_REC_PARAM_REDUCE_MEMSIZE_ENABLE  (1)


/* video format ( in CellRecParam )
 *
 *   SMALL  = 320x240 (4:3) or 368x208 (16:9)
 *   MIDDLE = 368x272 (4:3) or 480x272 (16:9)
 *   LARGE  = 480x368 (4:3) or 640x368 (16:9)
 *
 *   HD720  = 1280x720 (16:9)
 *
 *   PS3 playable format; all
 *   PSP playable format: MPEG4 + SMALL, AVC + SMALL, AVC + MIDDLE
 */

#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_SMALL_512K_30FPS    ( 0x0000 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_SMALL_768K_30FPS    ( 0x0010 )

#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_MIDDLE_512K_30FPS   ( 0x0100 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_MIDDLE_768K_30FPS   ( 0x0110 )

#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_512K_30FPS    ( 0x0200 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_768K_30FPS    ( 0x0210 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_1024K_30FPS   ( 0x0220 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_1536K_30FPS   ( 0x0230 )
#define CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_2048K_30FPS   ( 0x0240 )

#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_SMALL_512K_30FPS   ( 0x1000 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_SMALL_768K_30FPS   ( 0x1010 )

#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_512K_30FPS  ( 0x1100 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_768K_30FPS  ( 0x1110 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_1024K_30FPS ( 0x1120 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_MP_MIDDLE_1536K_30FPS ( 0x1130 )

#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_SMALL_512K_30FPS   ( 0x2000 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_SMALL_768K_30FPS   ( 0x2010 )

#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_MIDDLE_512K_30FPS  ( 0x2100 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_MIDDLE_768K_30FPS  ( 0x2110 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_MIDDLE_1024K_30FPS ( 0x2120 )
#define CELL_REC_PARAM_VIDEO_FMT_AVC_BL_MIDDLE_1536K_30FPS ( 0x2130 )

#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_SMALL_5000K_30FPS   ( 0x3060 )
#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_MIDDLE_5000K_30FPS  ( 0x3160 )
#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_LARGE_11000K_30FPS  ( 0x3270 )
#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_HD720_11000K_30FPS  ( 0x3670 )
#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_HD720_20000K_30FPS  ( 0x3680 )
#define CELL_REC_PARAM_VIDEO_FMT_MJPEG_HD720_25000K_30FPS  ( 0x3690 )

#define CELL_REC_PARAM_VIDEO_FMT_M4HD_SMALL_768K_30FPS    ( 0x4010 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_MIDDLE_768K_30FPS   ( 0x4110 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_LARGE_1536K_30FPS   ( 0x4230 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_LARGE_2048K_30FPS   ( 0x4240 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_2048K_30FPS   ( 0x4640 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_5000K_30FPS   ( 0x4660 )
#define CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_11000K_30FPS  ( 0x4670 )

/* audio format ( in CellRecParam ) */
#define CELL_REC_PARAM_AUDIO_FMT_AAC_64K   ( 0x0002 )
#define CELL_REC_PARAM_AUDIO_FMT_AAC_96K   ( 0x0000 )
#define CELL_REC_PARAM_AUDIO_FMT_AAC_128K  ( 0x0001 )
#define CELL_REC_PARAM_AUDIO_FMT_ULAW_384K ( 0x1007 )
#define CELL_REC_PARAM_AUDIO_FMT_ULAW_768K ( 0x1008 )
#define CELL_REC_PARAM_AUDIO_FMT_PCM_384K  ( 0x2007 )
#define CELL_REC_PARAM_AUDIO_FMT_PCM_768K  ( 0x2008 )
#define CELL_REC_PARAM_AUDIO_FMT_PCM_1536K ( 0x2009 )

/* YouTube recommeded video/audio format */
#define CELL_REC_PARAM_VIDEO_FMT_YOUTUBE        ( 0x0310 )
#define CELL_REC_PARAM_VIDEO_FMT_YOUTUBE_LARGE  CELL_REC_PARAM_VIDEO_FMT_MPEG4_LARGE_2048K_30FPS
#define CELL_REC_PARAM_VIDEO_FMT_YOUTUBE_HD720  CELL_REC_PARAM_VIDEO_FMT_M4HD_HD720_5000K_30FPS
#define CELL_REC_PARAM_AUDIO_FMT_YOUTUBE        CELL_REC_PARAM_AUDIO_FMT_AAC_64K

#define CELL_REC_PARAM_VIDEO_FMT_YOUTUBE_MJPEG  CELL_REC_PARAM_VIDEO_FMT_MJPEG_HD720_11000K_30FPS
#define CELL_REC_PARAM_AUDIO_FMT_YOUTUBE_MJPEG  CELL_REC_PARAM_AUDIO_FMT_PCM_768K


/* audio samples per block in CELL_REC_OPTION_AUDIO_INPUT mode */
#define CELL_REC_AUDIO_BLOCK_SAMPLES  (256)


/* callback function */
typedef void (* CellRecCallback )( int recStatus,
								   int recError,
								   void *userdata );


/* status ( recStatus in CellRecCallback ) */
#define CELL_REC_STATUS_UNLOAD        (0)
#define CELL_REC_STATUS_OPEN          (1)
#define CELL_REC_STATUS_START         (2)
#define CELL_REC_STATUS_STOP          (3)
#define CELL_REC_STATUS_CLOSE         (4)
#define CELL_REC_STATUS_ERR           (10)


/* max path length */
#define CELL_REC_MAX_PATH_LEN  ( 1023 )


/* API */

size_t
cellRecQueryMemSize ( const CellRecParam *pParam );

int
cellRecOpen ( const char *pDirName,
			  const char *pFileName,
			  const CellRecParam *pParam,
			  sys_memory_container_t container,
			  CellRecCallback cb,
			  void* cbUserData );

int
cellRecStart ( void );

int
cellRecStop ( void );

int
cellRecClose ( int isDiscard );



/* supplemental API */

#define CELL_REC_INFO_VIDEO_INPUT_ADDR    (0)
#define CELL_REC_INFO_VIDEO_INPUT_WIDTH   (1)
#define CELL_REC_INFO_VIDEO_INPUT_PITCH   (2)
#define CELL_REC_INFO_VIDEO_INPUT_HEIGHT  (3)
#define CELL_REC_INFO_AUDIO_INPUT_ADDR    (4)
#define CELL_REC_INFO_MOVIE_TIME_MSEC     (5)
#define CELL_REC_INFO_SPURS_SYSTEMWORKLOAD_ID (6)

void
cellRecGetInfo ( int info, uint64_t *pValue );


#define CELL_REC_SETINFO_MOVIE_START_TIME_MSEC  (100)
#define CELL_REC_SETINFO_MOVIE_END_TIME_MSEC    (101)
#define CELL_REC_SETINFO_MOVIE_META             (200)
#define CELL_REC_SETINFO_SCEME_META             (201)

#define CELL_REC_MOVIE_META_GAME_TITLE_LEN      (128)
#define CELL_REC_MOVIE_META_MOVIE_TITLE_LEN     (128)
#define CELL_REC_MOVIE_META_DESCRIPTION_LEN     (384)
#define CELL_REC_MOVIE_META_USERDATA_LEN        (64)

#define CELL_REC_SCENE_META_TYPE_CHAPTER        (0)
#define CELL_REC_SCENE_META_TYPE_CLIP_HIGHLIGHT (1)
#define CELL_REC_SCENE_META_TYPE_CLIP_USER      (2)

#define CELL_REC_SCENE_META_TITLE_LEN           (128)
#define CELL_REC_SCENE_META_TAG_NUM             (6)
#define CELL_REC_SCENE_META_TAG_LEN             (64)

/* metadata data type */
typedef struct {
	char *gameTitle;
	char *movieTitle;
	char *description;
	char *userdata;
} CellRecMovieMetadata;

typedef struct {
	uint32_t type;
	uint64_t startTime;
	uint64_t endTime;
	char     *title;
	uint32_t tagNum;
	char     **tag;
} CellRecSceneMetadata;


int32_t
cellRecSetInfo ( int setInfo, uint64_t value );


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_REC_H_*/
