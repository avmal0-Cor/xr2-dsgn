/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef	 __CELL_VDEC_H__
#define	 __CELL_VDEC_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <cell/error.h>
#include <cell/spurs.h>
#include <cell/codec/types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * error code defines
 */

/* wrong or invalid parameter */
#define CELL_VDEC_ERROR_ARG   CELL_ERROR_CAST(0x80610101)

/* wrong or invalid calling sequence */
#define CELL_VDEC_ERROR_SEQ   CELL_ERROR_CAST(0x80610102)

/* there is no space for input data */
#define CELL_VDEC_ERROR_BUSY  CELL_ERROR_CAST(0x80610103)

/* there is no data for output */
#define CELL_VDEC_ERROR_EMPTY CELL_ERROR_CAST(0x80610104)

/* the access unit was disregarded due to error */
#define CELL_VDEC_ERROR_AU    CELL_ERROR_CAST(0x80610105)

/* the picture is incomplete condition */
#define CELL_VDEC_ERROR_PIC   CELL_ERROR_CAST(0x80610106)

/* fatal error! you must close the library immediately */
#define CELL_VDEC_ERROR_FATAL CELL_ERROR_CAST(0x80610180)


/**
 * other defines
 */

/* PTS value indicate invalid */
#define CELL_VDEC_PTS_INVALID CELL_CODEC_PTS_INVALID

/* DTS value indicate invalid */
#define CELL_VDEC_DTS_INVALID CELL_CODEC_DTS_INVALID


/**
 * available CODEC types for using library.
 */
typedef enum {

	CELL_VDEC_CODEC_TYPE_MPEG2 = 0x00000000,

	CELL_VDEC_CODEC_TYPE_AVC   = 0x00000001,

	CELL_VDEC_CODEC_TYPE_DIVX  = 0x00000005,

	CELL_VDEC_CODEC_TYPE_MAX                       /**< always invalid, don't use.
													*/
} CellVdecCodecType;


/**
 * data type for callback message code
 */
typedef enum {

	CELL_VDEC_MSG_TYPE_AUDONE,                     /**< decode finish callback.
													*/
	CELL_VDEC_MSG_TYPE_PICOUT,                     /**< decoded picture available callback.
													*/
	CELL_VDEC_MSG_TYPE_SEQDONE,                    /**< end sequence done callback
													*/
	CELL_VDEC_MSG_TYPE_ERROR,                      /**< fatal error occured!
													*/
} CellVdecMsgType;


/**
 * data type for decoder operation mode
 */
typedef enum {

	CELL_VDEC_DEC_MODE_NORMAL,                     /**< normal decode mode
													*/
	CELL_VDEC_DEC_MODE_B_SKIP,                     /**< B picture skip mode
													*/
} CellVdecDecodeMode;


/**
 * data type for output picture format type
 */
typedef enum {

	CELL_VDEC_PICFMT_ARGB32_ILV,

	CELL_VDEC_PICFMT_RGBA32_ILV,

	CELL_VDEC_PICFMT_UYVY422_ILV,

	CELL_VDEC_PICFMT_YUV420_PLANAR,

} CellVdecPicFormatType;

/**
 * data type for output color matrix coefficient.
 */
typedef enum {

	CELL_VDEC_COLOR_MATRIX_TYPE_BT601,

	CELL_VDEC_COLOR_MATRIX_TYPE_BT709,

} CellVdecColorMatrixType;

/**
 * data type for output picture format type
 */
typedef enum {

	CELL_VDEC_PICITEM_ATTR_NORMAL,

	CELL_VDEC_PICITEM_ATTR_SKIPPED,

} CellVdecPicAttr;


/**
 * data type for universal frame rate code.
 */
typedef enum {

	CELL_VDEC_FRC_24000DIV1001          = 0x80,    /**< 23.976 fps (24000/1001 fps)
													*/
	CELL_VDEC_FRC_24                    = 0x81,    /**< 24 fps
													*/
	CELL_VDEC_FRC_25                    = 0x82,    /**< 25 fps
													*/
	CELL_VDEC_FRC_30000DIV1001          = 0x83,    /**< 29.97 fps (30000/1001 fps)
													*/
	CELL_VDEC_FRC_30                    = 0x84,    /**< 30 fps
													*/
	CELL_VDEC_FRC_50                    = 0x85,    /**< 50 fps
													*/
	CELL_VDEC_FRC_60000DIV1001          = 0x86,    /**< 59.94 fps (60000/1001 fps)
													*/
	CELL_VDEC_FRC_60                    = 0x87,    /**< 60 fps
													*/
} CellVdecFrameRate;


/**
 * type of libvdec handle.
 */
typedef void* CellVdecHandle;


/**
 * data type for codec type information
 */
typedef struct CellVdecType {

	CellVdecCodecType        codecType;            /**< type of codec
													*/
	uint32_t                 profileLevel;         /**< profile and level
													*/
} CellVdecType;

/**
 * extended data type for codec type information
 */
typedef struct CellVdecTypeEx {

	CellVdecCodecType        codecType;            /**< type of codec
													*/
	uint32_t                 profileLevel;         /**< profile and level
													*/
	void                    *codecSpecificInfo;

} CellVdecTypeEx;

/**
 * data type for library attributes infomation
 */
typedef struct CellVdecAttr {

	size_t                   memSize;              /**< memory size library required
													*/
	uint8_t                  cmdDepth;             /**< depth of the command queue
													*/
	uint32_t                 decoderVerUpper;      /**< reserved
													*/
	uint32_t                 decoderVerLower;      /**< reserved
													*/
} CellVdecAttr;


/**
 * data type for configurable information
 */
typedef struct CellVdecResource {

	void                    *memAddr;

	size_t                   memSize;

	int32_t                  ppuThreadPriority;

	size_t                   ppuThreadStackSize;

	int32_t                  spuThreadPriority;

	uint32_t                 numOfSpus;

} CellVdecResource;

/**
 * data type for SPURS information
 */
typedef struct CellVdecResourceSpurs {

	CellSpurs               *spursAddr;

	uint8_t                  tasksetPriority[8];

	uint32_t                 tasksetMaxContention;

} CellVdecResourceSpurs;

/**
 * extended data type for configurable information
 */
typedef struct CellVdecResourceEx {

	void                    *memAddr;

	size_t                   memSize;

	int32_t                  ppuThreadPriority;

	size_t                   ppuThreadStackSize;

	int32_t                  spuThreadPriority;

	uint32_t                 numOfSpus;

	CellVdecResourceSpurs   *spursResource;

} CellVdecResourceEx;

/**
 * data type for presentation or decode time stamp
 */
typedef CellCodecTimeStamp CellVdecTimeStamp;

/**
 * data type for access unit information
 */
typedef struct CellVdecAuInfo {

	void                    *startAddr;            /**< start address of an access unit
													*/
	size_t                   size;                 /**< size of an access unit
													*/
	CellCodecTimeStamp       pts;                  /**< PTS value.
													*/
	CellCodecTimeStamp       dts;                  /**< DTS value.
													*/
	uint64_t                 userData;             /**< data for user defined
													*/
	uint64_t                 codecSpecificData;    /**< reserved.
													*/
} CellVdecAuInfo;


/**
 * data type for output picture information
 */
typedef struct CellVdecPicItem {

	CellVdecCodecType        codecType;            /**< codec type of CellVdecPicItem.
													*/
	void                    *startAddr;            /**< start address of decoded picture buffer
													*/
	size_t                   size;                 /**< size of decoded picture buffer
													*/
	uint8_t                  auNum;                /**< number of access units
													*/
	CellCodecTimeStamp       auPts[2];             /**< presentation time stamp (PTS).
													*/
	CellCodecTimeStamp       auDts[2];             /**< decoding time stamp (DTS).
													*/
	uint64_t                 auUserData[2];        /**< UserData from AuInfo struct.
													*/
	int32_t                  status;               /**< picture output status
													*/
	CellVdecPicAttr          attr;                 /**< supplemental info.
													*/
	void                    *picInfo;              /**< extra data of codec specific info
													*/
} CellVdecPicItem;


/**
 * data type for output picture format
 */
typedef struct CellVdecPicFormat {

	CellVdecPicFormatType    formatType;

	CellVdecColorMatrixType  colorMatrixType;

	uint8_t                  alpha;

} CellVdecPicFormat;


/**
 * types of user defined callback function
 */
typedef uint32_t
(*CellVdecCbMsg)(

	CellVdecHandle           handle,

	CellVdecMsgType          msgType,

	int32_t                  msgData,

	void                    *cbArg );


/**
 * data type for callback function information
 */
typedef struct CellVdecCb {

	CellVdecCbMsg            cbFunc;               /**< pointer for callback function
													*/
	void                    *cbArg;                /**< user argument of callback function
													*/
} CellVdecCb;


/**
 * libvdec application programming interfaces
 */
int32_t
cellVdecQueryAttr(
	const CellVdecType      *type,
	CellVdecAttr            *attr );

int32_t
cellVdecQueryAttrEx(
	const CellVdecTypeEx    *type,
	CellVdecAttr            *attr );

int32_t
cellVdecOpen(
	const CellVdecType      *type,
	const CellVdecResource  *resource,
	const CellVdecCb        *callback,
	CellVdecHandle          *handle );

int32_t
cellVdecOpenEx(
	const CellVdecTypeEx     *type,
	const CellVdecResourceEx *resource,
	const CellVdecCb         *callback,
	CellVdecHandle           *handle );

int32_t
cellVdecClose(
	CellVdecHandle           handle );


int32_t
cellVdecStartSeq(
	CellVdecHandle           handle );


int32_t
cellVdecEndSeq(
	CellVdecHandle           handle );


int32_t
cellVdecDecodeAu(
	CellVdecHandle           handle,
	CellVdecDecodeMode       mode,
	const CellVdecAuInfo    *auInfo );


int32_t
cellVdecGetPicture(
	CellVdecHandle           handle,
	const CellVdecPicFormat *format,
	void                    *outBuff );


int32_t
cellVdecGetPicItem(
	CellVdecHandle           handle,
	const CellVdecPicItem  **picItem );

int32_t
cellVdecSetFrameRate(
	CellVdecHandle           handle,
	CellVdecFrameRate        frameRate );

#ifdef __cplusplus
}
#endif

#endif /* __CELL_VDEC_H__ */
