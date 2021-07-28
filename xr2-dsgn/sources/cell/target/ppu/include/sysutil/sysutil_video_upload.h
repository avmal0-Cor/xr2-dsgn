/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _CELL_SYSUTIL_VIDEO_UPLOAD_H_
#define _CELL_SYSUTIL_VIDEO_UPLOAD_H_

#include <sys/types.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */


/* error */
#define CELL_VIDEO_UPLOAD_ERROR_CANCEL                ( (int) 0x8002d000 )
#define CELL_VIDEO_UPLOAD_ERROR_NETWORK               ( (int) 0x8002d001 )
#define CELL_VIDEO_UPLOAD_ERROR_SERVICE_STOP          ( (int) 0x8002d002 )
#define CELL_VIDEO_UPLOAD_ERROR_SERVICE_BUSY          ( (int) 0x8002d003 )
#define CELL_VIDEO_UPLOAD_ERROR_SERVICE_UNAVAILABLE   ( (int) 0x8002d004 )
#define CELL_VIDEO_UPLOAD_ERROR_SERVICE_QUOTA         ( (int) 0x8002d005 )
#define CELL_VIDEO_UPLOAD_ERROR_ACCOUNT_STOP          ( (int) 0x8002d006 )

#define CELL_VIDEO_UPLOAD_ERROR_OUT_OF_MEMORY         ( (int) 0x8002d020 )
#define CELL_VIDEO_UPLOAD_ERROR_FATAL                 ( (int) 0x8002d021 )
#define CELL_VIDEO_UPLOAD_ERROR_INVALID_VALUE         ( (int) 0x8002d022 )
#define CELL_VIDEO_UPLOAD_ERROR_FILE_OPEN             ( (int) 0x8002d023 )
#define CELL_VIDEO_UPLOAD_ERROR_INVALID_STATE         ( (int) 0x8002d024 )

#define CELL_VIDEO_UPLOAD_ERROR_INTERNAL_INVALID_USER ( (int) 0x8002d040 )
#define CELL_VIDEO_UPLOAD_ERROR_INTERNAL_BOUNDARY     ( (int) 0x8002d041 )


/* option type ( in CellVideoUploadOption ) */
#define CELL_VIDEO_UPLOAD_OPTION_TYPE_TITLE_EDIT        (0)
#define CELL_VIDEO_UPLOAD_OPTION_TYPE_FORCE_USERNAME_FOR_DEBUG (1)
#define CELL_VIDEO_UPLOAD_OPTION_TYPE_FORCE_PASSWORD_FOR_DEBUG (2)

/* option value */
#define CELL_VIDEO_UPLOAD_OPTION_VALUE_TITLE_EDIT_ENABLE     (0)
#define CELL_VIDEO_UPLOAD_OPTION_VALUE_TITLE_EDIT_DISABLE    (1)


typedef struct {
	int32_t type;
	uint64_t value;
} CellVideoUploadOption;


/* site ID ( in CellVideoUploadParam ) */
#define CELL_VIDEO_UPLOAD_SITE_ID_YOUTUBE   (0)

/* limit of path, metadata (in CellVidepUploadParam ) */
#define CELL_VIDEO_UPLOAD_MAX_FILE_PATH_LEN             ( 1023 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_CLIENT_ID_LEN     ( 64 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_DEVELOPER_KEY_LEN ( 128 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_TITLE_LEN         ( 61 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_DESCRIPTION_LEN   ( 1024 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_KEYWORD_LEN       ( 25 )

#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_USERNAME_LEN      ( 41 )
#define CELL_VIDEO_UPLOAD_MAX_YOUTUBE_PASSWORD_LEN      ( 31 )

typedef struct {

	int32_t siteID;
	const char *pFilePath;

	union {
		struct {
			const char *pClientId;
			const char *pDeveloperKey;
			const char *pTitle_UTF8;
			const char *pDescription_UTF8;
			const char *pKeyword_1_UTF8;
			const char *pKeyword_2_UTF8;
			const char *pKeyword_3_UTF8;
			uint8_t isPrivate;
			uint8_t rating;
		} youtube;

	} u;

	int numOfOption;
	CellVideoUploadOption *pOption;

} CellVideoUploadParam;


/* status ( in CellVideoUploadCallback ) */
#define CELL_VIDEO_UPLOAD_STATUS_INITIALIZED   (1)
#define CELL_VIDEO_UPLOAD_STATUS_FINALIZED     (2)

/* callback function */
typedef void (* CellVideoUploadCallback )( int32_t status,
										   int32_t errorCode,
										   const char *pResultURL,
										   void *userdata );

/* API */
int32_t
cellVideoUploadInitialize ( const CellVideoUploadParam *pParam,
							CellVideoUploadCallback cb,
							void *userdata );

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*#ifndef _CELL_SYSUTIL_VIDEO_UPLOAD_H_ */
