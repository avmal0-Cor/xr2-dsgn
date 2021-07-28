/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef	 __CELL_VDEC_AVC_H__
#define	 __CELL_VDEC_AVC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <stdbool.h>

/* value of max cc data length. */
#define CELL_VDEC_AVC_CCD_MAX 128

/**
 * values of mpeg4-avc "level"
 */
enum {
	CELL_VDEC_AVC_LEVEL_1P0                   = 10,
	CELL_VDEC_AVC_LEVEL_1P1                   = 11,
	CELL_VDEC_AVC_LEVEL_1P2                   = 12,
	CELL_VDEC_AVC_LEVEL_1P3                   = 13,
	CELL_VDEC_AVC_LEVEL_2P0                   = 20,
	CELL_VDEC_AVC_LEVEL_2P1                   = 21,
	CELL_VDEC_AVC_LEVEL_2P2                   = 22,
	CELL_VDEC_AVC_LEVEL_3P0                   = 30,
	CELL_VDEC_AVC_LEVEL_3P1                   = 31,
	CELL_VDEC_AVC_LEVEL_3P2                   = 32,
	CELL_VDEC_AVC_LEVEL_4P0                   = 40,
	CELL_VDEC_AVC_LEVEL_4P1                   = 41,
	CELL_VDEC_AVC_LEVEL_4P2                   = 42,
};

typedef struct CellVdecAvcSpecificInfo {

	size_t   thisSize;

	uint16_t maxDecodedFrameWidth;

	uint16_t maxDecodedFrameHeight;

	bool     disableDeblockingFilter;

	uint8_t  numberOfDecodedFrameBuffer;

} CellVdecAvcSpecificInfo;

typedef struct CellVdecAvcInfo {

	uint16_t horizontalSize;
	uint16_t verticalSize;
	uint8_t  pictureType[2];
	bool     idrPictureFlag;

	uint8_t  aspect_ratio_idc;                     /**< aspect_ratio_idc specifies
													*   the value of the sample aspect
													*   ratio of the luma samples.
													*/
	uint16_t sar_height;                           /**< sar_width indicates the horizontal
													*   size of the sample aspect ratio
													*   (in arbitrary units).
													*/
	uint16_t sar_width;                            /**< sar_height indicates the vertical
													*   size of the sample aspect ratio
													*   (in the same arbitrary units as
													*   sar_width).
													*/
	uint8_t  pic_struct;                           /**< pic_struct indicates whether a
													*   picture should be displayed as
													*   a frame or one or more fields.
													*/
	int16_t  picOrderCount[2];

	bool     vui_parameters_present_flag;          /**< vui_parameters_present_flag equal
													*   to 1 specifies that the
													*   vui_parameters( ) syntax structure
													*   is present next in the bitstream.
													*/
	bool     frame_mbs_only_flag;                  /**< frame_mbs_only_flag equal to 0
													*   specifies that coded pictures of
													*   the coded video sequence may either
													*   be coded fields or coded frames.
													*/
	bool     video_signal_type_present_flag;
	uint8_t  video_format;                         /**< video_format indicates the
													*   representation of the pictures,
													*   before being coded in accordance
													*   with this Recommendation |
													*   International Standard.
													*/
	bool     video_full_range_flag;                /**< video_full_range_flag indicates the
													*   black level and range of the luma
													*   and chroma signals.
													*/
	bool     colour_description_present_flag;      /**< colour_description_present_flag equal
													*   to 1 specifies that colour_primaries,
													*   transfer_characteristics and
													*   matrix_coefficients are present.
													*/
	uint8_t  colour_primaries;                     /**< colour_primaries indicates the
													*   chromaticity coordinates of the source
													*   primaries.
													*/
	uint8_t  transfer_characteristics;             /**< transfer_characteristics indicates the
													*   opto-electronic transfer characteristic
													*   of the source picture.
													*/
	uint8_t  matrix_coefficients;                  /**< matrix_coefficients describes the matrix
													*   coefficients used in deriving luma and
													*   chroma signals from the green, blue, and
													*   red primaries.
													*/
	bool     timing_info_present_flag;             /**< timing_info_present_flag equal to 1
													*   specifies that num_units_in_tick,
													*   time_scale and fixed_frame_rate_flag
													*   are present in the bitstream.
													*/
	uint8_t  frameRateCode;
	bool     fixed_frame_rate_flag;                /**< fixed_frame_rate_flag equal to 1 indicates
													*   that the temporal distance between the HRD
													*   output times of any two consecutive pictures
													*   in output order is constrained as follows.
													*/
	bool     low_delay_hrd_flag;                   /**< low_delay_hrd_flag specifies the HRD
													*   operational mode.
													*/
	bool     entropy_coding_mode_flag;             /**< entropy_coding_mode_flag selects the
													*   entropy decoding method to be applied
													*   for the syntax elements.
													*/
	uint16_t nalUnitPresentFlags;

	uint8_t  ccDataLength[2];                      /**< closed caption data length.
													 */
	uint8_t  ccData[2][CELL_VDEC_AVC_CCD_MAX];     /**< closed caption data.
													 */
	uint64_t reserved[2];                          /**< reserved.
													 */
} CellVdecAvcInfo;


/**
 * values of "video_format"
 */
enum {
	CELL_VDEC_AVC_VF_COMPONENT                = 0x00,
	CELL_VDEC_AVC_VF_PAL                      = 0x01,
	CELL_VDEC_AVC_VF_NTSC                     = 0x02,
	CELL_VDEC_AVC_VF_SECAM                    = 0x03,
	CELL_VDEC_AVC_VF_MAC                      = 0x04,
	CELL_VDEC_AVC_VF_UNSPECIFIED              = 0x05,
};


/**
 * values of "colour_primaries"
 */
enum {
	// 0x00...reserved
	CELL_VDEC_AVC_CP_ITU_R_BT_709_5           = 0x01,
	CELL_VDEC_AVC_CP_UNSPECIFIED              = 0x02,
	// 0x03...reserved
	CELL_VDEC_AVC_CP_ITU_R_BT_470_6_SYS_M     = 0x04,
	CELL_VDEC_AVC_CP_ITU_R_BT_470_6_SYS_BG    = 0x05,
	CELL_VDEC_AVC_CP_SMPTE_170_M              = 0x06,
	CELL_VDEC_AVC_CP_SMPTE_240_M              = 0x07,
	CELL_VDEC_AVC_CP_GENERIC_FILM             = 0x08,
};


/**
 * values of "transfer_characteristics"
 */
enum {
	// 0x00...reserved
	CELL_VDEC_AVC_TC_ITU_R_BT_709_5           = 0x01,
	CELL_VDEC_AVC_TC_UNSPECIFIED              = 0x02,
	// 0x03...reserved
	CELL_VDEC_AVC_TC_ITU_R_BT_470_6_SYS_M     = 0x04,
	CELL_VDEC_AVC_TC_ITU_R_BT_470_6_SYS_BG    = 0x05,
	CELL_VDEC_AVC_TC_SMPTE_170_M              = 0x06,
	CELL_VDEC_AVC_TC_SMPTE_240_M              = 0x07,
	CELL_VDEC_AVC_TC_LINEAR                   = 0x08,
	CELL_VDEC_AVC_TC_LOG_100_1                = 0x09,
	CELL_VDEC_AVC_TC_LOG_316_1                = 0x0a,
};


/**
 * values of "matrix_coefficients"
 */
enum {
	CELL_VDEC_AVC_MXC_GBR                     = 0x00,
	CELL_VDEC_AVC_MXC_ITU_R_BT_709_5          = 0x01,
	CELL_VDEC_AVC_MXC_UNSPECIFIED             = 0x02,
	// 0x03...reserved
	CELL_VDEC_AVC_MXC_FCC                     = 0x04,
	CELL_VDEC_AVC_MXC_ITU_R_BT_470_6_SYS_BG   = 0x05,
	CELL_VDEC_AVC_MXC_SMPTE_170_M             = 0x06,
	CELL_VDEC_AVC_MXC_SMPTE_240_M             = 0x07,
	CELL_VDEC_AVC_MXC_YCGCO                   = 0x08,
};



/**
 * values of "FrameRateCode"
 */
enum {
	CELL_VDEC_AVC_FRC_24000DIV1001            = 0x00,
	CELL_VDEC_AVC_FRC_24                      = 0x01,
	CELL_VDEC_AVC_FRC_25                      = 0x02,
	CELL_VDEC_AVC_FRC_30000DIV1001            = 0x03,
	CELL_VDEC_AVC_FRC_30                      = 0x04,
	CELL_VDEC_AVC_FRC_50                      = 0x05,
	CELL_VDEC_AVC_FRC_60000DIV1001            = 0x06,
	CELL_VDEC_AVC_FRC_60                      = 0x07,
};



/**
 * values of "NulUnitPresentFlags"
 */
enum {
	CELL_VDEC_AVC_FLG_SPS                     = 0x0001,
	CELL_VDEC_AVC_FLG_PPS                     = 0x0002,
	CELL_VDEC_AVC_FLG_AUD                     = 0x0004,
	CELL_VDEC_AVC_FLG_EO_SEQ                  = 0x0008,
	CELL_VDEC_AVC_FLG_EO_STREAM               = 0x0100,
	CELL_VDEC_AVC_FLG_FILLER_DATA             = 0x0200,
	CELL_VDEC_AVC_FLG_PIC_TIMING_SEI          = 0x0400,
	CELL_VDEC_AVC_FLG_BUFF_PERIOD_SEI         = 0x0800,
	CELL_VDEC_AVC_FLG_USER_DATA_UNREG_SEI     = 0x1000,
};


/**
 * values of "aspect_ratio_idc"
 */
enum {
	CELL_VDEC_AVC_ARI_SAR_UNSPECIFIED         = 0x00,
	CELL_VDEC_AVC_ARI_SAR_1_1                 = 0x01,
	CELL_VDEC_AVC_ARI_SAR_12_11               = 0x02,
	CELL_VDEC_AVC_ARI_SAR_10_11               = 0x03,
	CELL_VDEC_AVC_ARI_SAR_16_11               = 0x04,
	CELL_VDEC_AVC_ARI_SAR_40_33               = 0x05,
	CELL_VDEC_AVC_ARI_SAR_24_11               = 0x06,
	CELL_VDEC_AVC_ARI_SAR_20_11               = 0x07,
	CELL_VDEC_AVC_ARI_SAR_32_11               = 0x08,
	CELL_VDEC_AVC_ARI_SAR_80_33               = 0x09,
	CELL_VDEC_AVC_ARI_SAR_18_11               = 0x0a,
	CELL_VDEC_AVC_ARI_SAR_15_11               = 0x0b,
	CELL_VDEC_AVC_ARI_SAR_64_33               = 0x0c,
	CELL_VDEC_AVC_ARI_SAR_160_99              = 0x0d,
	CELL_VDEC_AVC_ARI_SAR_4_3                 = 0x0e,
	CELL_VDEC_AVC_ARI_SAR_3_2                 = 0x0f,
	CELL_VDEC_AVC_ARI_SAR_2_1                 = 0x10,
	CELL_VDEC_AVC_ARI_SAR_EXTENDED_SAR        = 0xff,
};


/**
 * values of "PictureType"
 */
enum {
	CELL_VDEC_AVC_PCT_I                       = 0x00, /**< I-type */
	CELL_VDEC_AVC_PCT_P                       = 0x01, /**< P-type */
	CELL_VDEC_AVC_PCT_B                       = 0x02, /**< B-type */
	CELL_VDEC_AVC_PCT_UNKNOWN                 = 0x03, /**< unknown */
};


/**
 * values of "pic_struct"
 */
enum {
	CELL_VDEC_AVC_PSTR_FRAME                  = 0x00, /**< frame */
	CELL_VDEC_AVC_PSTR_FIELD_TOP              = 0x01, /**< top field */
	CELL_VDEC_AVC_PSTR_FIELD_BTM              = 0x02, /**< bottom field */
	CELL_VDEC_AVC_PSTR_FIELD_TOP_BTM          = 0x03, /**< top field, bottom field, in that order */
	CELL_VDEC_AVC_PSTR_FIELD_BTM_TOP          = 0x04, /**< bottom field, top field, in that order */
	CELL_VDEC_AVC_PSTR_FIELD_TOP_BTM_TOP      = 0x05, /**< top field, bottom field, top field repeated in that order */
	CELL_VDEC_AVC_PSTR_FIELD_BTM_TOP_BTM      = 0x06, /**< bottom field, top field, bottom field repeated in that order */
	CELL_VDEC_AVC_PSTR_FRAME_DOUBLING         = 0x07, /**< frame doubling */
	CELL_VDEC_AVC_PSTR_FRAME_TRIPLING         = 0x08, /**< frame tripling */
};

#ifdef __cplusplus
}
#endif

#endif /* __CELL_VDEC_AVC_H__ */
