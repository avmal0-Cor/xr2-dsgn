/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef	 __CELL_VDEC_DIVX_H__
#define	 __CELL_VDEC_DIVX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <stdbool.h>

/**
 * profile and level.
 */
enum {
	CELL_VDEC_DIVX_QMOBILE                   = 10,
	CELL_VDEC_DIVX_MOBILE                    = 11,
	CELL_VDEC_DIVX_HOME_THEATER              = 12,
	CELL_VDEC_DIVX_HD_720                    = 13,
	CELL_VDEC_DIVX_HD_1080                   = 14,
};

typedef struct CellVdecDivxSpecificInfo {

	size_t   thisSize;

	uint16_t maxDecodedFrameWidth;

	uint16_t maxDecodedFrameHeight;

} CellVdecDivxSpecificInfo;

typedef struct CellVdecDivxSpecificInfo2 {

	size_t   thisSize;

	uint16_t maxDecodedFrameWidth;

	uint16_t maxDecodedFrameHeight;

	uint16_t numberOfDecodedFrameBuffer;

} CellVdecDivxSpecificInfo2;

typedef struct CellVdecDivxInfo {

	uint8_t  pictureType;                        /**< 2 bit code which identifies whether a
												  *   picture is an intra-coded picture (I),
												  *   predictive-coded picture (P),
												  *   bidirectionally predictive-coded
												  *   picture (B).
												  */
	uint16_t horizontalSize;                     /**< 13 bit code representing the width of
												  *   the displayable part of the luminance
												  *   component in pixel units.
												  */
	uint16_t verticalSize;                       /**< 13 bit code representing the height of
												  *   the displayable integer part of the
												  *   luminance component in pixel units.
												  */
	uint8_t  pixelAspectRatio;                   /**< 4 bit code which defines the value of
												  *   pixel aspect ratio.
												  */
	uint8_t  parWidth;                           /**< 8 bit code which indicates the
												  *   horizontal size of pixel aspect ratio.
												  */
	uint8_t  parHeight;                          /**< 8 bit code which indicates the
												  *   vertical size of pixel aspect ratio.
												  */
	bool     colourDescription;                  /**< A flag which if set to 'true' indicates
												  *   the presence of colourPrimaries,
												  *   transferCharacteristics and
												  *   matrixCoefficients in the bitstream.
												  */
	uint8_t  colourPrimaries;                    /**< 8-bit integer describes the chromaticity
												  *   coordinates of the source primaries.
												  */
	uint8_t  transferCharacteristics;            /**< 8-bit integer describes the opto-
												  *   electronic transfer characteristic of
												  *   the source picture.
												  */
	uint8_t  matrixCoefficients;                 /**< 8-bit integer describes the matrix
												  *   coefficients used in deriving luminance
												  *   and chrominance signals from the green,
												  *   blue, and red primaries.
												  */
	uint8_t  pictureStruct;

	uint16_t frameRateCode;

} CellVdecDivxInfo;


/**
 * values of frameRateCode
 */
enum {
	CELL_VDEC_DIVX_FRC_UNDEFINED             = 0x00, /**< unknown, or not defined. */
	CELL_VDEC_DIVX_FRC_24000DIV1001          = 0x01, /**< 23.976fps (24000/1001 fps） */
	CELL_VDEC_DIVX_FRC_24                    = 0x02, /**< 24 fps */
	CELL_VDEC_DIVX_FRC_25                    = 0x03, /**< 25 fps */
	CELL_VDEC_DIVX_FRC_30000DIV1001          = 0x04, /**< 29.97 fps (30000/1001 fps） */
	CELL_VDEC_DIVX_FRC_30                    = 0x05, /**< 30 fps */
	CELL_VDEC_DIVX_FRC_50                    = 0x06, /**< 50 fps */
	CELL_VDEC_DIVX_FRC_60000DIV1001          = 0x07, /**< 59.94 fps (60000/1001 fps） */
	CELL_VDEC_DIVX_FRC_60                    = 0x08, /**< 60 fps */
};


/**
 * values of pixelAspectRatio
 */
enum {
	CELL_VDEC_DIVX_ARI_PAR_1_1               = 0x1, /**< 1:1 (Square) */
	CELL_VDEC_DIVX_ARI_PAR_12_11             = 0x2, /**< 12:11 (625-type for 4:3 picture) */
	CELL_VDEC_DIVX_ARI_PAR_10_11             = 0x3, /**< 10:11 (525-type for 4:3 picture) */
	CELL_VDEC_DIVX_ARI_PAR_16_11             = 0x4, /**< 16:11 (625-type stretched for 16:9 picture) */
	CELL_VDEC_DIVX_ARI_PAR_40_33             = 0x5, /**< 40:33 (525-type stretched for 16:9 picture) */
	CELL_VDEC_DIVX_ARI_PAR_EXTENDED_PAR      = 0xF, /**< extended PAR */
};


/**
 * values of pictureType
 */
enum {
	CELL_VDEC_DIVX_VCT_I                     = 0x0, /**< intra-coded (I) */
	CELL_VDEC_DIVX_VCT_P                     = 0x1, /**< predictive-coded (P) */
	CELL_VDEC_DIVX_VCT_B                     = 0x2, /**< bidirectionally-predictive-coded (B) */
};


/**
 * values of pictureStruct
 */
enum {
	CELL_VDEC_DIVX_PSTR_FRAME                = 0x0, /**< frame */
	CELL_VDEC_DIVX_PSTR_TOP_BTM              = 0x1, /**< top field, bottom field, in that order */
	CELL_VDEC_DIVX_PSTR_BTM_TOP              = 0x2, /**< bottom field, top field, in that order */
};


/**
 * values of colourPrimaries
 */
enum {
	CELL_VDEC_DIVX_CP_ITU_R_BT_709           = 0x01, /**< recommendation ITU-R BT.709. */
	CELL_VDEC_DIVX_CP_UNSPECIFIED            = 0x02, /**< unspecified video image characteristics. */
	CELL_VDEC_DIVX_CP_ITU_R_BT_470_SYS_M     = 0x04, /**< recommendation ITU-R BT.470-6 System M. */
	CELL_VDEC_DIVX_CP_ITU_R_BT_470_SYS_BG    = 0x05, /**< recommendation ITU-R BT.470-6 System B, G. */
	CELL_VDEC_DIVX_CP_SMPTE_170_M            = 0x06, /**< SMPTE 170M. */
	CELL_VDEC_DIVX_CP_SMPTE_240_M            = 0x07, /**< SMPTE 240M (1987). */
	CELL_VDEC_DIVX_CP_GENERIC_FILM           = 0x08, /**< generic film. */
};


/**
 * values of transferCharacteristics
 */
enum {
	CELL_VDEC_DIVX_TC_ITU_R_BT_709           = 0x01, /**< recommendation ITU-R BT.709. */
	CELL_VDEC_DIVX_TC_UNSPECIFIED            = 0x02, /**< unspecified video image characteristics. */
	CELL_VDEC_DIVX_TC_ITU_R_BT_470_SYS_M     = 0x04, /**< recommendation ITU-R BT.470-6 System M. */
	CELL_VDEC_DIVX_TC_ITU_R_BT_470_SYS_BG    = 0x05, /**< recommendation ITU-R BT.470-6 System B, G */
	CELL_VDEC_DIVX_TC_SMPTE_170_M            = 0x06, /**< SMPTE 170M. */
	CELL_VDEC_DIVX_TC_SMPTE_240_M            = 0x07, /**< SMPTE 240M (1987). */
	CELL_VDEC_DIVX_TC_LINEAR                 = 0x08, /**< linear transfer characteristics */
	CELL_VDEC_DIVX_TC_LOG_100_1              = 0x09, /**< logarithmic transfer characteristic.(100:1) */
	CELL_VDEC_DIVX_TC_LOG_316_1              = 0x0a, /**< logarithmic transfer characteristic.(316:1) */
};


/**
 * values of matrixCoefficients
 */
enum {
	CELL_VDEC_DIVX_MXC_ITU_R_BT_709          = 0x01, /**< recommendation ITU-R BT.709. */
	CELL_VDEC_DIVX_MXC_UNSPECIFIED           = 0x02, /**< unspecified video image characteristics. */
	CELL_VDEC_DIVX_MXC_FCC                   = 0x04, /**< FCC. */
	CELL_VDEC_DIVX_MXC_ITU_R_BT_470_SYS_BG   = 0x05, /**< recommendation ITU-R BT.470-6 System B, G */
	CELL_VDEC_DIVX_MXC_SMPTE_170_M           = 0x06, /**< SMPTE 170M. */
	CELL_VDEC_DIVX_MXC_SMPTE_240_M           = 0x07, /**< SMPTE 240M (1987). */
	CELL_VDEC_DIVX_MXC_YCGCO                 = 0x08, /**< YCgCo. */
};


#ifdef __cplusplus
}
#endif


#endif /* __CELL_VDEC_DIVX_H__ */
