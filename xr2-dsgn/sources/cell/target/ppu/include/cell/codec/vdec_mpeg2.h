/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef	 __CELL_VDEC_MPEG2_H__
#define	 __CELL_VDEC_MPEG2_H__

/* //////// for C++...... //////// */
#ifdef __cplusplus
extern "C" {
#endif
/* //////// for C++...end //////// */

#include <types.h>
#include <stdbool.h>




/**
 * values of "level" in main profile
 */
enum {
	CELL_VDEC_MPEG2_MP_LL,	/**< Main Profile, Low Level */
	CELL_VDEC_MPEG2_MP_ML,	/**< Main Profile, Main Level */
	CELL_VDEC_MPEG2_MP_H14,	/**< Main Profile, High-1440 Level */
	CELL_VDEC_MPEG2_MP_HL,	/**< Main Profile, High Level */
};

typedef struct CellVdecMpeg2SpecificInfo {

	size_t   thisSize;

	uint16_t maxDecodedFrameWidth;

	uint16_t maxDecodedFrameHeight;

} CellVdecMpeg2SpecificInfo;

typedef struct CellVdecMpeg2Info {

	uint16_t horizontal_size;						/**< horizontal size of luminance
													 */
	uint16_t vertical_size;							/**< vertical size of luminance
													 */
	uint8_t  aspect_ratio_information;				/**< aspect ratio information.
													 */
	uint8_t  frame_rate_code;						/**< frame rate.
													 */
	bool     progressive_sequence;					/**< progressive sequence.
													 */
	bool     low_delay;								/**< low delay.
													 */
	uint8_t  video_format;							/**< video format.
													 */
	bool     colour_description;					/**< color description.
													 */
	uint8_t  colour_primaries;						/**< color primaries.
													 */
	uint8_t  transfer_characteristics;				/**< transfer characteristics.
													 */
	uint8_t  matrix_coefficients;					/**< matrix coefficients.
													 */
	uint16_t temporal_reference[2];					/**< temporal reference.
													 */
	uint8_t  picture_coding_type[2];				/**< picture coding_type.
													 */
	uint8_t  picture_structure[2];					/**< picture struct.
													 */
	bool     top_field_first;						/**< top_field first.
													 */
	bool     repeat_first_field;					/**< repeat_first field.
													 */
	bool     progressive_frame;						/**< progressive frame.
													 */
	uint32_t time_code;								/**< time from gop top
													 */
	bool     closed_gop;							/**< closed gop flag
													 * 0 : It's not closed GOP  / 1 : It's closed GOP
													 */
	bool     broken_link;							/**< broken link flag
													 * 0 : Not broken	1 : broken
													 */
	uint16_t vbv_delay[2];							/**< vbv buffer delay
													 */
	uint16_t display_horizontal_size;				/**< valid horizontal size on display
													 */
	uint16_t display_vertical_size;					/**< valid vertical size on display
													 */
	uint8_t  number_of_frame_centre_offsets[2];		/**< number of frame centre offsets
													 */
	uint16_t frame_centre_horizontal_offset[2][3];	/**< frame centre horizontal offset
													 */
	uint16_t frame_centre_vertical_offset[2][3];	/**< frame centre vertical offset
													 */
	uint32_t headerPresentFlags;					/**< header presentation flags
													 */
	uint32_t headerRetentionFlags;					/**< header retention flags
													 */
	bool     mpeg1Flag;								/**< mpeg1 flag
													 * 0 : It's not mpeg1  / 1 : It's mpeg1
													 * When following extensions doesn't exist, stream is mpeg1.
													 *   sequence_extension()
													 *   picture_coding_extension()
													 *   picture_display_extension()
													 */
	uint8_t  ccDataLength[2];						/**< closed caption data length.
													 */
	uint8_t  ccData[2][128];						/**< closed caption data.
													 */
	uint64_t reserved[2];							/**< reserved.
													 */
} CellVdecMpeg2Info;




/**
 * values of "headerPresentFlags" & "headerRetentionFlags"
 */
enum {

	CELL_VDEC_MPEG2_FLG_SEQ_HDR       = 0x00000001,	/**< Contain sequence_header() in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_SEQ_EXT       = 0x00000002,	/**< Contain sequence_extension() in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_SEQ_DSP_EXT   = 0x00000004,	/**< Contain sequence_display_extension() in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_SEQ_USR_DAT   = 0x00000008,	/**< Contain sequence level user data in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_SEQ_END       = 0x00000010,	/**< Contain seuqnce_end_code in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_GOP_HDR       = 0x00000020,	/**< Contain group_of_picture_header() in decoded AU.
													 */
	CELL_VDEC_MPEG2_FLG_GOP_USR_DAT   = 0x00000040,	/**< Contain GOP level user data decoded AU.
													 */


	CELL_VDEC_MPEG2_FLG_PIC_HDR_1     = 0x00000100,	/**< Contained picture_header() in decoded AU.
													 * It's first field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_EXT_1     = 0x00000200,	/**< Contained picture_coding_extension() in decoded AU.
													 * It's first field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_DSP_EXT_1 = 0x00000400,	/**< Contained picture_display_extension() in decoded AU.
													 * It's first field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_USR_DAT_1 = 0x00000800,	/**< Contained pic level user data decoded AU.
													 * It's first field in case of field structure
													 */


	CELL_VDEC_MPEG2_FLG_PIC_HDR_2     = 0x00001000,	/**< Contained picture_header() in decoded AU.
													 * It's second field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_EXT_2     = 0x00002000,	/**< Contained picture_coding_extension() in decoded AU.
													 * It's second field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_DSP_EXT_2 = 0x00004000,	/**< Contained picture_display_extension() in decoded AU.
													 * It's second field in case of field structure
													 */
	CELL_VDEC_MPEG2_FLG_PIC_USR_DAT_2 = 0x00008000,	/**< Contained pic level user data decoded AU.
													 * It's second field in case of field structure
													 */


};


/**
 * values of acpect ratio
 */

/* MPEG2 */
enum {
	CELL_VDEC_MPEG2_ARI_SAR_1_1                 = 0x01,
	CELL_VDEC_MPEG2_ARI_DAR_4_3                 = 0x02,
	CELL_VDEC_MPEG2_ARI_DAR_16_9                = 0x03,
	CELL_VDEC_MPEG2_ARI_DAR_2P21_1              = 0x04,
};

/* MPEG1 */
enum {
	CELL_VDEC_MPEG1_ARI_SAR_1P0                 = 0x01,
	CELL_VDEC_MPEG1_ARI_SAR_0P6735              = 0x02,
	CELL_VDEC_MPEG1_ARI_SAR_0P7031              = 0x03,
	CELL_VDEC_MPEG1_ARI_SAR_0P7615              = 0x04,
	CELL_VDEC_MPEG1_ARI_SAR_0P8055              = 0x05,
	CELL_VDEC_MPEG1_ARI_SAR_0P8437              = 0x06,
	CELL_VDEC_MPEG1_ARI_SAR_0P8935              = 0x07,
	CELL_VDEC_MPEG1_ARI_SAR_0P9157              = 0x08,
	CELL_VDEC_MPEG1_ARI_SAR_0P9815              = 0x09,
	CELL_VDEC_MPEG1_ARI_SAR_1P0255              = 0x0a,
	CELL_VDEC_MPEG1_ARI_SAR_1P0695              = 0x0b,
	CELL_VDEC_MPEG1_ARI_SAR_1P0950              = 0x0c,
	CELL_VDEC_MPEG1_ARI_SAR_1P1575              = 0x0d,
	CELL_VDEC_MPEG1_ARI_SAR_1P2015              = 0x0e,
};



/**
 * values of frame rate
 */
enum {
	CELL_VDEC_MPEG2_FRC_FORBIDDEN               = 0x00,
	CELL_VDEC_MPEG2_FRC_24000DIV1001            = 0x01,
	CELL_VDEC_MPEG2_FRC_24                      = 0x02,
	CELL_VDEC_MPEG2_FRC_25                      = 0x03,
	CELL_VDEC_MPEG2_FRC_30000DIV1001            = 0x04,
	CELL_VDEC_MPEG2_FRC_30                      = 0x05,
	CELL_VDEC_MPEG2_FRC_50                      = 0x06,
	CELL_VDEC_MPEG2_FRC_60000DIV1001            = 0x07,
	CELL_VDEC_MPEG2_FRC_60                      = 0x08,
};

/**
 * values of video format
 */
enum {
	CELL_VDEC_MPEG2_VF_COMPONENT                = 0x00,
	CELL_VDEC_MPEG2_VF_PAL                      = 0x01,
	CELL_VDEC_MPEG2_VF_NTSC                     = 0x02,
	CELL_VDEC_MPEG2_VF_SECAM                    = 0x03,
	CELL_VDEC_MPEG2_VF_MAC                      = 0x04,
	CELL_VDEC_MPEG2_VF_UNSPECIFIED              = 0x05,
};

/**
 * values of colour primaries
 */
enum {
	CELL_VDEC_MPEG2_CP_FORBIDDEN                = 0x00,
	CELL_VDEC_MPEG2_CP_ITU_R_BT_709             = 0x01,
	CELL_VDEC_MPEG2_CP_UNSPECIFIED              = 0x02,
	CELL_VDEC_MPEG2_CP_ITU_R_BT_470_2_SYS_M     = 0x04,
	CELL_VDEC_MPEG2_CP_ITU_R_BT_470_2_SYS_BG    = 0x05,
	CELL_VDEC_MPEG2_CP_SMPTE_170_M              = 0x06,
	CELL_VDEC_MPEG2_CP_SMPTE_240_M              = 0x07,
};

/**
 * values of transfer characteristics
 */
enum {
	CELL_VDEC_MPEG2_TC_FORBIDDEN                = 0x00,
	CELL_VDEC_MPEG2_TC_ITU_R_BT_709             = 0x01,
	CELL_VDEC_MPEG2_TC_UNSPECIFIED              = 0x02,
	CELL_VDEC_MPEG2_TC_ITU_R_BT_470_2_SYS_M     = 0x04,
	CELL_VDEC_MPEG2_TC_ITU_R_BT_470_2_SYS_BG    = 0x05,
	CELL_VDEC_MPEG2_TC_SMPTE_170_M              = 0x06,
	CELL_VDEC_MPEG2_TC_SMPTE_240_M              = 0x07,
	CELL_VDEC_MPEG2_TC_LINEAR                   = 0x08,
	CELL_VDEC_MPEG2_TC_LOG_100_1                = 0x09,
	CELL_VDEC_MPEG2_TC_LOG_316_1                = 0x0a,
};

/**
 * values of matrix coefficients
 */
enum {
	CELL_VDEC_MPEG2_MXC_FORBIDDEN               = 0x00,
	CELL_VDEC_MPEG2_MXC_ITU_R_BT_709            = 0x01,
	CELL_VDEC_MPEG2_MXC_UNSPECIFIED             = 0x02,
	CELL_VDEC_MPEG2_MXC_FCC                     = 0x04,
	CELL_VDEC_MPEG2_MXC_ITU_R_BT_470_2_SYS_BG   = 0x05,
	CELL_VDEC_MPEG2_MXC_SMPTE_170_M             = 0x06,
	CELL_VDEC_MPEG2_MXC_SMPTE_240_M             = 0x07,
};

/**
 * values of picture coding type
 */
enum {
	CELL_VDEC_MPEG2_PCT_FORBIDDEN               = 0x00,
	CELL_VDEC_MPEG2_PCT_I                       = 0x01,
	CELL_VDEC_MPEG2_PCT_P                       = 0x02,
	CELL_VDEC_MPEG2_PCT_B                       = 0x03,
	CELL_VDEC_MPEG2_PCT_D                       = 0x04,
};


/**
 * values of picture structure
 */
enum {
	CELL_VDEC_MPEG2_PSTR_TOP_FIELD              = 0x01,
	CELL_VDEC_MPEG2_PSTR_BOTTOM_FIELD           = 0x02,
	CELL_VDEC_MPEG2_PSTR_FRAME                  = 0x03,
};


/* //////// for C++...... //////// */
#ifdef __cplusplus
}
#endif
/* //////// for C++...end //////// */

#endif /* __CELL_VDEC_MPEG2_H__ */
