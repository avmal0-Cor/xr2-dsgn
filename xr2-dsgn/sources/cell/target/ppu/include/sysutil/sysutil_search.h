/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _CELL_SYSUTIL_SEARCH_H_
#define _CELL_SYSUTIL_SEARCH_H_

#include <sys/types.h>
#include "sysutil_common.h"
#include "sysutil_search_types.h"
#include "sysutil_music.h"

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C"
{
#endif /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#define CELL_SEARCH_OK									(0)	/*J 正常終了 */
#define CELL_SEARCH_CANCELED							(1)	/*J 操作がキャンセルされた */
#define CELL_SEARCH_ERROR_PARAM							(0x8002c801)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x01) パラメータが不正 */
#define CELL_SEARCH_ERROR_BUSY							(0x8002c802)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x02) */
#define CELL_SEARCH_ERROR_NO_MEMORY						(0x8002c803)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x03) */
#define CELL_SEARCH_ERROR_UNKNOWN_MODE					(0x8002c804)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x04) */
#define CELL_SEARCH_ERROR_ALREADY_INITIALIZED			(0x8002c805)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x05) */
#define CELL_SEARCH_ERROR_NOT_INITIALIZED				(0x8002c806)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x06) */
#define CELL_SEARCH_ERROR_FINALIZING					(0x8002c807)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x07) */
#define CELL_SEARCH_ERROR_NOT_SUPPORTED_SEARCH			(0x8002c808)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x08) */
#define CELL_SEARCH_ERROR_CONTENT_OBSOLETE				(0x8002c809)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x09) */
#define CELL_SEARCH_ERROR_CONTENT_NOT_FOUND				(0x8002c80a)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0a) */
#define CELL_SEARCH_ERROR_NOT_LIST						(0x8002c80b)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0b) */
#define CELL_SEARCH_ERROR_OUT_OF_RANGE					(0x8002c80c)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0c) */
#define CELL_SEARCH_ERROR_INVALID_SEARCHID				(0x8002c80d)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0d) */
#define CELL_SEARCH_ERROR_ALREADY_GOT_RESULT			(0x8002c80e)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0e) */
#define CELL_SEARCH_ERROR_NOT_SUPPORTED_CONTEXT			(0x8002c80f)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x0f) */
#define CELL_SEARCH_ERROR_INVALID_CONTENTTYPE			(0x8002c810)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x10) */
#define CELL_SEARCH_ERROR_DRM							(0x8002c811)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x11) */
#define CELL_SEARCH_ERROR_TAG							(0x8002c812)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0x12) */
#define CELL_SEARCH_ERROR_GENERIC						(0x8002c8ff)	/*J (CELL_SYSUTIL_ERROR_BASE_SEARCH | 0xff) */

#define CELL_SEARCH_GAMECOMMENT_SIZE_MAX				(1024)
#define CELL_SEARCH_CONTENT_BUFFER_SIZE_MAX				(2048)
#define CELL_SEARCH_TITLE_LEN_MAX						(384)
#define CELL_SEARCH_PATH_LEN_MAX						(63)
#define CELL_SEARCH_MTOPTION_LEN_MAX					(63)
#define CELL_SEARCH_TAG_LEN_MAX							(63)
#define CELL_SEARCH_TAG_NUM_MAX							(6)
#define CELL_SEARCH_DEVELOPERDATA_LEN_MAX				(64)

typedef int32_t CellSearchId;
#define CELL_SEARCH_ID_INVALID							((CellSearchId) 0)

typedef enum
{
	CELL_SEARCH_MODE_NORMAL = 0,
} CellSearchMode;

typedef enum
{
	CELL_SEARCH_EVENT_NOTIFICATION = 0,
	CELL_SEARCH_EVENT_INITIALIZE_RESULT,
	CELL_SEARCH_EVENT_FINALIZE_RESULT,
	CELL_SEARCH_EVENT_LISTSEARCH_RESULT,
	CELL_SEARCH_EVENT_CONTENTSEARCH_INLIST_RESULT,
	CELL_SEARCH_EVENT_CONTENTSEARCH_RESULT,
	CELL_SEARCH_EVENT_SCENESEARCH_INVIDEO_RESULT,
	CELL_SEARCH_EVENT_SCENESEARCH_RESULT,
} CellSearchEvent;

typedef enum
{
	CELL_SEARCH_LISTSEARCHTYPE_NONE = 0,
	CELL_SEARCH_LISTSEARCHTYPE_MUSIC_ALBUM,
	CELL_SEARCH_LISTSEARCHTYPE_MUSIC_GENRE,
	CELL_SEARCH_LISTSEARCHTYPE_MUSIC_ARTIST,
	CELL_SEARCH_LISTSEARCHTYPE_PHOTO_YEAR,
	CELL_SEARCH_LISTSEARCHTYPE_PHOTO_MONTH,
	CELL_SEARCH_LISTSEARCHTYPE_PHOTO_ALBUM,
	CELL_SEARCH_LISTSEARCHTYPE_PHOTO_PLAYLIST,
	CELL_SEARCH_LISTSEARCHTYPE_VIDEO_ALBUM,
	CELL_SEARCH_LISTSEARCHTYPE_MUSIC_PLAYLIST,
} CellSearchListSearchType;

typedef enum
{
	CELL_SEARCH_CONTENTSEARCHTYPE_NONE = 0,
	CELL_SEARCH_CONTENTSEARCHTYPE_MUSIC_ALL,
	CELL_SEARCH_CONTENTSEARCHTYPE_PHOTO_ALL,
	CELL_SEARCH_CONTENTSEARCHTYPE_VIDEO_ALL,
} CellSearchContentSearchType;

typedef enum  {
	CELL_SEARCH_SCENESEARCHTYPE_NONE = 0,
	CELL_SEARCH_SCENESEARCHTYPE_CHAPTER,
	CELL_SEARCH_SCENESEARCHTYPE_CLIP_HIGHLIGHT,
	CELL_SEARCH_SCENESEARCHTYPE_CLIP_USER,
	CELL_SEARCH_SCENESEARCHTYPE_CLIP,
	CELL_SEARCH_SCENESEARCHTYPE_ALL,
} CellSearchSceneSearchType;

typedef enum
{
	CELL_SEARCH_SORTKEY_NONE = 0,
	CELL_SEARCH_SORTKEY_DEFAULT,
	CELL_SEARCH_SORTKEY_TITLE,
	CELL_SEARCH_SORTKEY_ALBUMTITLE,
	CELL_SEARCH_SORTKEY_GENRENAME,
	CELL_SEARCH_SORTKEY_ARTISTNAME,
	CELL_SEARCH_SORTKEY_IMPORTEDDATE,
	CELL_SEARCH_SORTKEY_TRACKNUMBER,
	CELL_SEARCH_SORTKEY_TAKENDATE,
	CELL_SEARCH_SORTKEY_USERDEFINED,
	CELL_SEARCH_SORTKEY_MODIFIEDDATE,
} CellSearchSortKey;

typedef enum
{
	CELL_SEARCH_SORTORDER_NONE = 0,
	CELL_SEARCH_SORTORDER_ASCENDING,
	CELL_SEARCH_SORTORDER_DESCENDING,
} CellSearchSortOrder;

typedef enum
{
	CELL_SEARCH_CONTENTTYPE_NONE = 0,
	CELL_SEARCH_CONTENTTYPE_MUSIC,
	CELL_SEARCH_CONTENTTYPE_MUSICLIST,
	CELL_SEARCH_CONTENTTYPE_PHOTO,
	CELL_SEARCH_CONTENTTYPE_PHOTOLIST,
	CELL_SEARCH_CONTENTTYPE_VIDEO,
	CELL_SEARCH_CONTENTTYPE_VIDEOLIST,
	CELL_SEARCH_CONTENTTYPE_SCENE,
} CellSearchContentType;

typedef enum  {
	CELL_SEARCH_SCENETYPE_NONE = 0,
	CELL_SEARCH_SCENETYPE_CHAPTER,
	CELL_SEARCH_SCENETYPE_CLIP_HIGHLIGHT,
	CELL_SEARCH_SCENETYPE_CLIP_USER,
} CellSearchSceneType;

typedef enum
{
	CELL_SEARCH_LISTTYPE_NONE = 0,
	CELL_SEARCH_LISTTYPE_MUSIC_ALBUM,
	CELL_SEARCH_LISTTYPE_MUSIC_GENRE,
	CELL_SEARCH_LISTTYPE_MUSIC_ARTIST,
	CELL_SEARCH_LISTTYPE_PHOTO_YEAR,
	CELL_SEARCH_LISTTYPE_PHOTO_MONTH,
	CELL_SEARCH_LISTTYPE_PHOTO_ALBUM,
	CELL_SEARCH_LISTTYPE_PHOTO_PLAYLIST,
	CELL_SEARCH_LISTTYPE_VIDEO_ALBUM,
	CELL_SEARCH_LISTTYPE_MUSIC_PLAYLIST,
} CellSearchListType;

typedef enum
{
	CELL_SEARCH_CODEC_UNKNOWN = 0,
	CELL_SEARCH_CODEC_MPEG2,
	CELL_SEARCH_CODEC_MPEG4,
	CELL_SEARCH_CODEC_AVC,
	CELL_SEARCH_CODEC_MPEG1,
	CELL_SEARCH_CODEC_AT3,
	CELL_SEARCH_CODEC_AT3P,
	CELL_SEARCH_CODEC_ATALL,
	CELL_SEARCH_CODEC_MP3,
	CELL_SEARCH_CODEC_AAC,
	CELL_SEARCH_CODEC_LPCM,
	CELL_SEARCH_CODEC_WAV,
	CELL_SEARCH_CODEC_WMA,
	CELL_SEARCH_CODEC_JPEG,
	CELL_SEARCH_CODEC_PNG,
	CELL_SEARCH_CODEC_TIFF,
	CELL_SEARCH_CODEC_BMP,
	CELL_SEARCH_CODEC_GIF,
	CELL_SEARCH_CODEC_MPEG2_TS,
	CELL_SEARCH_CODEC_DSD,
	CELL_SEARCH_CODEC_AC3,
	CELL_SEARCH_CODEC_MPEG1_LAYER1,
	CELL_SEARCH_CODEC_MPEG1_LAYER2,
	CELL_SEARCH_CODEC_MPEG1_LAYER3,
	CELL_SEARCH_CODEC_MPEG2_LAYER1,
	CELL_SEARCH_CODEC_MPEG2_LAYER2,
	CELL_SEARCH_CODEC_MPEG2_LAYER3,
	CELL_SEARCH_CODEC_MOTIONJPEG,
} CellSearchCodec;

typedef enum
{
	CELL_SEARCH_ORIENTATION_UNKNOWN = 0,
	CELL_SEARCH_ORIENTATION_TOP_LEFT,
	CELL_SEARCH_ORIENTATION_TOP_RIGHT,
	CELL_SEARCH_ORIENTATION_BOTTOM_RIGHT,
	CELL_SEARCH_ORIENTATION_BOTTOM_LEFT,
} CellSearchOrientation;

typedef enum
{
	CELL_SEARCH_REPEATMODE_NONE = 0,
	CELL_SEARCH_REPEATMODE_REPEAT1,
	CELL_SEARCH_REPEATMODE_ALL,
	CELL_SEARCH_REPEATMODE_NOREPEAT1,
} CellSearchRepeatMode;

typedef enum
{
	CELL_SEARCH_CONTEXTOPTION_NONE = 0,
	CELL_SEARCH_CONTEXTOPTION_SHUFFLE,
} CellSearchContextOption;

typedef enum
{
	CELL_SEARCH_CONTENTSTATUS_NONE,
	CELL_SEARCH_CONTENTSTATUS_AVILABLE,
	CELL_SEARCH_CONTENTSTATUS_NOT_SUPPORTED,
	CELL_SEARCH_CONTENTSTATUS_BROKEN,
} CellSearchContentStatus;

typedef struct{
	CellSearchId searchId;
	uint32_t resultNum;
} CellSearchResultParam;

typedef struct
{
	CellSearchListType listType;
	uint32_t numOfItems;
	int64_t duration;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
	char artistName[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved2[3];
} CellSearchMusicListInfo;

typedef struct
{
	CellSearchListType listType;
	uint32_t numOfItems;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
} CellSearchPhotoListInfo;

typedef struct
{
	CellSearchListType listType;
	uint32_t numOfItems;
	int64_t duration;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
} CellSearchVideoListInfo;

typedef struct
{
	int64_t duration;
	int64_t size;
	time_t importedDate;
	time_t lastPlayedDate;
	int32_t releasedYear;
	int32_t trackNumber;
	int32_t bitrate;
	int32_t samplingRate;
	int32_t quantizationBitrate;
	int32_t playCount;
	int32_t drmEncrypted;
	CellSearchCodec codec;
	CellSearchContentStatus status;
	char diskNumber[8];
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
	char albumTitle[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved2[3];
	char artistName[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved3[3];
	char genreName[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved4[3];
} CellSearchMusicInfo;

typedef struct
{
	int64_t size;
	time_t importedDate;
	time_t takenDate;
	int32_t width;
	int32_t height;
	CellSearchOrientation orientation;
	CellSearchCodec codec;
	CellSearchContentStatus status;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
	char albumTitle[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved2[3];
} CellSearchPhotoInfo;

typedef struct
{
	int64_t duration;
	int64_t size;
	time_t importedDate;
	time_t takenDate;
	int32_t videoBitrate;
	int32_t audioBitrate;
	int32_t drmEncrypted;
	CellSearchCodec videoCodec;
	CellSearchCodec audioCodec;
	CellSearchContentStatus status;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
	char albumTitle[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved2[3];
} CellSearchVideoInfo;

typedef struct
{
	CellSearchSceneType sceneType;
	int64_t startTime_ms;
	int64_t endTime_ms;
	CellSearchContentId videoId;
	char title[CELL_SEARCH_TITLE_LEN_MAX + 1];
	char reserved[3];
	char tags[CELL_SEARCH_TAG_NUM_MAX][CELL_SEARCH_TAG_LEN_MAX];
} CellSearchSceneInfo;

typedef struct {
	char contentPath[CELL_SEARCH_PATH_LEN_MAX + 1];
	char thumbnailPath[CELL_SEARCH_PATH_LEN_MAX + 1];
} CellSearchContentInfoPath;

typedef struct {
	char movieThumbnailPath[CELL_SEARCH_PATH_LEN_MAX + 1];
	char movieThumbnailOption[CELL_SEARCH_MTOPTION_LEN_MAX + 1];
} CellSearchContentInfoPathMovieThumb;


typedef struct
{ 
	time_t takenDate;
	time_t importedDate;
	time_t modifiedDate;
} CellSearchTimeInfo;

/*J コンテンツ検索ユーティリティのコールバック関数 */
typedef void (*CellSearchSystemCallback)(CellSearchEvent event, int result, const void *param, void *userData);


/*J コンテンツ検索ユーティリティの初期化 */
int cellSearchInitialize(CellSearchMode mode,
						 sys_memory_container_t container, // set SYS_MEMORY_CONTAINER_ID_INVALID
						 CellSearchSystemCallback func,
						 void *userData);

/*J コンテンツ検索ユーティリティの終了 */
int cellSearchFinalize(void);

/*J リスト情報の検索 */
int cellSearchStartListSearch(CellSearchListSearchType searchType,
							  CellSearchSortOrder sortOrder,
							  CellSearchId *outSearchId);

/*J 同一リストに属するコンテンツ情報の検索 */
int cellSearchStartContentSearchInList(const CellSearchContentId *listId,
									   CellSearchSortKey sortKey,
									   CellSearchSortOrder sortOrder,
									   CellSearchId *outSearchId);

/*J 検索カテゴリ指定のコンテンツ情報の検索 */
int cellSearchStartContentSearch(CellSearchContentSearchType type,
								 CellSearchSortKey sortKey,
								 CellSearchSortOrder sortOrder,
								 CellSearchId *outSearchId);

/*J 検索結果からのコンテンツ情報の取得 */
int cellSearchGetContentInfoByOffset(const CellSearchId searchId,
									 int32_t offset,
									 void *infoBuffer,
									 CellSearchContentType *outContentType,
									 CellSearchContentId *outContentId);

/*J コンテンツIDによるコンテンツ情報の取得 */
int cellSearchGetContentInfoByContentId(const CellSearchContentId *contentId,
										void *infoBuffer,
										CellSearchContentType *outContentType);

/*J コンテンツ情報のオフセットの取得 */
int cellSearchGetOffsetByContentId(const CellSearchId searchId,
								   const CellSearchContentId *contentId,
								   int32_t *outOffset);

/*J パス情報の取得 */
int cellSearchGetContentInfoPath(const CellSearchContentId *contentId,
								 CellSearchContentInfoPath *infoPath);

/*J 動画サムネールのパス情報の取得 */
int cellSearchGetContentInfoPathMovieThumb(const CellSearchContentId *contentId,
										   CellSearchContentInfoPathMovieThumb *infoMt);

/*J ゲームコメントの取得 */
int cellSearchGetContentInfoGameComment(const CellSearchContentId *contentId,
										char gameComment[CELL_SEARCH_GAMECOMMENT_SIZE_MAX]);

/*J 検索結果のコンテンツIDを取得 */
int cellSearchGetContentIdByOffset(CellSearchId searchId,
								   int32_t offset,
								   CellSearchContentType *outContentType,
								   CellSearchContentId *outContentId,
								   CellSearchTimeInfo *outTimeInfo);

/*J コンテンツへのファイルアクセス準備 */
int cellSearchPrepareFile(const char *path);

/*J 検索のキャンセル */
int cellSearchCancel(const CellSearchId searchId);

/*J 検索結果の破棄 */
int cellSearchEnd(const CellSearchId searchId);

/*J 音楽コンテンツ再生用の選択コンテキストの取得 */
int cellSearchGetMusicSelectionContext(const CellSearchId searchId,
									   const CellSearchContentId *contentId,
									   CellSearchRepeatMode repeatMode,
									   CellSearchContextOption option,
									   CellMusicSelectionContext *outContext);

/*J 音楽コンテンツを1曲だけ再生する選択コンテキストの取得 */
int cellSearchGetMusicSelectionContextOfSingleTrack(const CellSearchContentId *contentId,
													CellMusicSelectionContext *outContext);

/*J Videoに含まれるシーンを検索 */
int cellSearchStartSceneSearchInVideo(const CellSearchContentId *videoId,
									  CellSearchSceneSearchType searchType,
									  CellSearchSortOrder sortOrder,
									  CellSearchId *outSearchId);

/*J ゲームタイトルやタグに一致するシーンを検索 */
int cellSearchStartSceneSearch(CellSearchSceneSearchType searchType,
							   const char *gameTitle,
							   const char **tags,
							   uint32_t tagNum,
							   CellSearchSortKey sortKey,
							   CellSearchSortOrder sortOrder,
							   CellSearchId *outSearchId);

/*J Video/SceneのDeveloperDataを取得 */
int cellSearchGetContentInfoDeveloperData(const CellSearchContentId *contentId,
										  char developerData[CELL_SEARCH_DEVELOPERDATA_LEN_MAX]);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif /* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif /*_CELL_SYSUTIL_SEARCH_H_*/
