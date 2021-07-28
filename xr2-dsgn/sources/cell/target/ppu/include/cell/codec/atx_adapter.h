/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _INCLUDE_ATX_ADAPTER_H
#define _INCLUDE_ATX_ADAPTER_H

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define ATRACX_EXTRA_CONFIG_DATA_SIZE (4)

#define CELL_ADEC_ATRACX_WORD_SZ_16BIT (0x02)
#define CELL_ADEC_ATRACX_WORD_SZ_24BIT (0x03)
#define CELL_ADEC_ATRACX_WORD_SZ_32BIT (0x04)
#define CELL_ADEC_ATRACX_WORD_SZ_FLOAT (0x84)

#define CELL_ADEC_ATRACX_ATS_HDR_NOTINC (0)
#define CELL_ADEC_ATRACX_ATS_HDR_INC    (1)

typedef struct {
    int32_t sampling_freq; /* Sampling frequency */
    int32_t ch_config_idx; /* Channel config index */
    int32_t nch_out;       /* Output channels */
    int32_t nbytes;        /* size of a raw_data_frame */
    uint8_t extra_config_data[ATRACX_EXTRA_CONFIG_DATA_SIZE]; /* downmix coefficients */
    int32_t bw_pcm;        /* Bit length and type of output PCM
                              0x02 short int,16bit
                              0x03 int,24bit
                              0x04 int,32bit
                              0x84 float,32bit
                           */
    uint8_t  downmix_flag; /* 0:downmix off
                              1:downmix on
                           */
    uint8_t  au_includes_ats_hdr_flg;
/*
  au_includes_ats_hdr_flg
  
  CELL_ADEC_ATRACX_ATS_HDR_NOTINC: AU not includes ATS header
  CELL_ADEC_ATRACX_ATS_HDR_INC   : AU includes ATS header
*/
} CellAdecParamAtracX;


/* ATRAC-X BSI */
typedef struct {
	uint32_t samplingFreq;       /* Sampling frequency [Hz] */
	uint32_t channelConfigIndex; /* channel configuration index */
	uint32_t nbytes;             /* the number of bytes in a single raw_data_frame() */
}CellAdecAtracXInfo;


/*******************************************************************************
	Error Code for error callback
*******************************************************************************/
#define CELL_ADEC_ERROR_ATX_OFFSET (0x80612200U)

#define	CELL_ADEC_ERROR_ATX_NONE	                   CELL_ERROR_CAST(0x80612200)	        /* no errors */
#define	CELL_ADEC_ERROR_ATX_OK	                       CELL_ERROR_CAST(0x80612200)

/*----- Adapter level error -----*/
#define CELL_ADEC_ERROR_ATX_BUSY                       CELL_ERROR_CAST(0x80612264)
#define CELL_ADEC_ERROR_ATX_EMPTY                      CELL_ERROR_CAST(0x80612265)
#define CELL_ADEC_ERROR_ATX_ATSHDR                     CELL_ERROR_CAST(0x80612266)

/*----- Core level error -----*/
/* non fatal errors */
#define	CELL_ADEC_ERROR_ATX_NON_FATAL                  CELL_ERROR_CAST(0x80612281)	/* undefined error */
#define	CELL_ADEC_ERROR_ATX_NOT_IMPLE                  CELL_ERROR_CAST(0x80612282)	/* not implemented */
#define	CELL_ADEC_ERROR_ATX_PACK_CE_OVERFLOW           CELL_ERROR_CAST(0x80612283)	/* pack_channel_block_at5() in pack_at5.c */
#define CELL_ADEC_ERROR_ATX_ILLEGAL_NPROCQUS           CELL_ERROR_CAST(0x80612284)	/* unpack_sheader() in pack_at5_sheader.c */

/* fatal errors(block level) */
#define CELL_ADEC_ERROR_ATX_FATAL                      CELL_ERROR_CAST(0x8061228c)	/* undefined error */
#define	CELL_ADEC_ERROR_ATX_ENC_OVERFLOW               CELL_ERROR_CAST(0x8061228d)	/* at5enc_proc() in at5enc.c */
#define CELL_ADEC_ERROR_ATX_PACK_CE_UNDERFLOW          CELL_ERROR_CAST(0x8061228e)	/* pack_channel_block_at5() in pack_at5.c */

/* to check syntax */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_IDCT                CELL_ERROR_CAST(0x8061228f)	/* unpack_idct() in pack_at5_codetbl.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_GAINADJ             CELL_ERROR_CAST(0x80612290)	/* unpack_gainadj() in pack_at5_gainadj.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_IDSF                CELL_ERROR_CAST(0x80612291)	/* unpack_idsf() in pack_at5_sfactor.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_SPECTRA             CELL_ERROR_CAST(0x80612292)	/* unpack_spectra() in pack_at5_spectra.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_IDWL	               CELL_ERROR_CAST(0x80612293)	/* unpack_idwl() in pack_at5_idwl.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_GHWAVE              CELL_ERROR_CAST(0x80612294)	/* unpack_ghwave() in pack_at5_ghwave.c */
#define	CELL_ADEC_ERROR_ATX_SYNTAX_SHEADER             CELL_ERROR_CAST(0x80612295)	/* unpack_sheader() in pack_at5_sheader.c */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDWL_A              CELL_ERROR_CAST(0x80612296)	/* idwl[][]{0-7} range error	*/
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDWL_B              CELL_ERROR_CAST(0x80612297)	/* idwl restriction error (wlc_encqu<=divqu<nprocqus,ich=0) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDWL_C              CELL_ERROR_CAST(0x80612298)	/* idwl restriction error (0<=wlc_divqu<=wlc_encqu,ich=0) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDWL_D              CELL_ERROR_CAST(0x80612299)	/* idwl restriction error (wlc_encqu<divqu<=nprocqus,ich=1) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDWL_E              CELL_ERROR_CAST(0x8061229a)	/* idwl restriction error (wlc_encqu<=nprocqus)	*/
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDSF_A              CELL_ERROR_CAST(0x8061229b)	/* idsf[][]{0-63} range error */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDSF_B              CELL_ERROR_CAST(0x8061229c)	/* sfc_nbitlen{0-6} range error	*/
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDSF_C              CELL_ERROR_CAST(0x8061229d)	/* idsf restriction error (0<=sfc_divqu<=nrealqus) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDSF_D              CELL_ERROR_CAST(0x8061229e)	/* idsf restriction error (0<=sfc_sg_divqu<=nrealqus) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_IDCT_A              CELL_ERROR_CAST(0x8061229f)	/* idct restriction error (0<=ctc_divqu<=nrealqus) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_GC_NGC              CELL_ERROR_CAST(0x806122a0)	/* ngc[][]{0-7} range error */
#define CELL_ADEC_ERROR_ATX_SYNTAX_GC_IDLEV_A          CELL_ERROR_CAST(0x806122a1)	/* idlev[][][]{0-15} range error */
#define CELL_ADEC_ERROR_ATX_SYNTAX_GC_IDLOC_A          CELL_ERROR_CAST(0x806122a2)	/* idloc[][][]{0-31} range error */
#define CELL_ADEC_ERROR_ATX_SYNTAX_GC_IDLEV_B          CELL_ERROR_CAST(0x806122a3)	/* gain_control_data restriction error (idlev[][][]) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_GC_IDLOC_B          CELL_ERROR_CAST(0x806122a4)	/* gain_control_data restriction error (idloc[][][]) */
#define CELL_ADEC_ERROR_ATX_SYNTAX_SN_NWVS             CELL_ERROR_CAST(0x806122a5)	/* sinusoid_data restriction error	*/

/* fatal errors(handle level) */
#define CELL_ADEC_ERROR_ATX_FATAL_HANDLE               CELL_ERROR_CAST(0x806122aa)	/* undefined error at handle level */
/* parameter assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_SAMPLING_FREQ       CELL_ERROR_CAST(0x806122ab)	/* sampling freqeuency assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_CH_CONFIG_INDEX     CELL_ERROR_CAST(0x806122ac)	/* channel_config_index assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_NBYTES              CELL_ERROR_CAST(0x806122ad)	/* a number of bytes(nbytes) assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_BLOCK_NUM           CELL_ERROR_CAST(0x806122ae)	/* a number of blocks(block_num) assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_BLOCK_ID            CELL_ERROR_CAST(0x806122af)	/* block_id assertion error */
#define CELL_ADEC_ERROR_ATX_ASSERT_CHANNELS            CELL_ERROR_CAST(0x806122b0)	/* a number of channels per block assertion error */

#define CELL_ADEC_ERROR_ATX_UNINIT_BLOCK_SPECIFIED     CELL_ERROR_CAST(0x806122b1)	/* uninitialized block specified */

#define CELL_ADEC_ERROR_ATX_POSCFG_PRESENT             CELL_ERROR_CAST(0x806122b2) 	/* "position_config_data" presents(currently fatal error) */
#define CELL_ADEC_ERROR_ATX_BUFFER_OVERFLOW            CELL_ERROR_CAST(0x806122b3)	/* buffer overflow */
#define CELL_ADEC_ERROR_ATX_ILL_BLK_TYPE_ID            CELL_ERROR_CAST(0x806122b4)	/* illegal "block_type_id" detected */
#define	CELL_ADEC_ERROR_ATX_UNPACK_CHANNEL_BLK_FAILED  CELL_ERROR_CAST(0x806122b5)	/* unpack_channel_block() failed */
#define CELL_ADEC_ERROR_ATX_ILL_BLK_ID_USED_1          CELL_ERROR_CAST(0x806122b6)	/* illegal "block_id" used (more audio_channel_block) */
#define CELL_ADEC_ERROR_ATX_ILL_BLK_ID_USED_2          CELL_ERROR_CAST(0x806122b7)	/* illegal "block_id" used (less audio_channel_block) */

/* error in parameter assertion */
#define	CELL_ADEC_ERROR_ATX_ILLEGAL_ENC_SETTING        CELL_ERROR_CAST(0x806122b8)	/* illegal encoder setting(init_atrac5_encode()) */
#define	CELL_ADEC_ERROR_ATX_ILLEGAL_DEC_SETTING        CELL_ERROR_CAST(0x806122b9)	/* illegal decoder setting(init_atrac5_decode()) */

#define CELL_ADEC_ERROR_ATX_ASSERT_NSAMPLES            CELL_ERROR_CAST(0x806122ba)	/* a number of input PCM samples assertion error */
                                                                                        /* (for atx_encode() and atx_encode_alt()) */
/* ATS */
#define	CELL_ADEC_ERROR_ATX_ILL_SYNCWORD               CELL_ERROR_CAST(0x806122bb)	/* illegal syncword used	*/
#define	CELL_ADEC_ERROR_ATX_ILL_SAMPLING_FREQ          CELL_ERROR_CAST(0x806122bc)	/* illegal sampling_frequeny_index @ config_data()	*/
#define	CELL_ADEC_ERROR_ATX_ILL_CH_CONFIG_INDEX        CELL_ERROR_CAST(0x806122bd)	/* illegal channel_config_index	@ config_data()	*/

#define CELL_ADEC_ERROR_ATX_RAW_DATA_FRAME_SIZE_OVER   CELL_ERROR_CAST(0x806122be)	/* size of raw_data_frame > frame_length	*/
#define CELL_ADEC_ERROR_ATX_SYNTAX_ENHANCE_LENGTH_OVER CELL_ERROR_CAST(0x806122bf)	/* enhance_length {0,2046} range error	*/

#define CELL_ADEC_ERROR_ATX_SPU_INTERNAL_FAIL          CELL_ERROR_CAST(0x806122c8)	/* spu-processing failed */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _INCLUDE_ATX_ADAPTER_H */
