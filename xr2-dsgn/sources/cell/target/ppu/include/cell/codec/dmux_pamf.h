/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __DMUX_PAMF_H__
#define __DMUX_PAMF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Specific Informations for PAMF Stream
 */
typedef struct {
    size_t    thisSize;
    bool      programEndCodeCb;
} CellDmuxPamfSpecificInfo;


/*
 * MPEG2 Video Macros for Levels
 */
typedef enum {
	CELL_DMUX_PAMF_M2V_MP_LL = 0,
	CELL_DMUX_PAMF_M2V_MP_ML,
	CELL_DMUX_PAMF_M2V_MP_H14,
	CELL_DMUX_PAMF_M2V_MP_HL
} CellDmuxPamfM2vLevel;

/**
 * AVC Macros for Levels
 */
typedef enum {
	CELL_DMUX_PAMF_AVC_LEVEL_2P1 = 21,
	CELL_DMUX_PAMF_AVC_LEVEL_3P0 = 30,
	CELL_DMUX_PAMF_AVC_LEVEL_3P1 = 31,
	CELL_DMUX_PAMF_AVC_LEVEL_3P2 = 32,
	CELL_DMUX_PAMF_AVC_LEVEL_4P1 = 41,
	CELL_DMUX_PAMF_AVC_LEVEL_4P2 = 42,
} CellDmuxPamfAvcLevel;


/*
 * Specific Informations for MPEG2 Video Access Unit
 */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfAuSpecificInfoM2v;

/* Specific Informations for AVC Access Unit */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfAuSpecificInfoAvc;


/*
 * Specific Informations for LPCM Access Unit
 */
typedef struct {
	uint8_t  channelAssignmentInfo;
	uint8_t  samplingFreqInfo;
	uint8_t  bitsPerSample;
} CellDmuxPamfAuSpecificInfoLpcm;

/* Specific Informations for AC3 Acceess Unit */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfAuSpecificInfoAc3;

/* Specific Informations for Atrac3plus Acceess Unit */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfAuSpecificInfoAtrac3plus;

/* Specific Informations for UserData Acceess Unit */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfAuSpecificInfoUserData;


/*
 * Specific Informations for MPEG2 Video Elementary Stream
 */
typedef struct {
	uint32_t profileLevel;
} CellDmuxPamfEsSpecificInfoM2v;

/* Specific Informations for AVC  Elementary Stream */
typedef struct {
	uint32_t level;
} CellDmuxPamfEsSpecificInfoAvc;


/*
 * Specific Informations for LPCM Elementary Stream
 */
typedef struct {
	uint32_t samplingFreq;
	uint32_t numOfChannels;
	uint32_t bitsPerSample;
} CellDmuxPamfEsSpecificInfoLpcm;

/* Specific Informations for AC3 Elementary Stream */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfEsSpecificInfoAc3;

/* Specific Informations for Atrac3plus Elementary Stream */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfEsSpecificInfoAtrac3plus;

/* Specific Informations for UserData Elementary Stream */
typedef struct {
	uint32_t reserved1;
} CellDmuxPamfEsSpecificInfoUserData;


/*
 * Sampling Rate Macros for Audio Codec
 */
typedef enum {
	CELL_DMUX_PAMF_FS_48K  = 48000
} CellDmuxPamfSamplingFrequency;

/*
 * Bits Per Samples Macros for Audio Codec
 */
typedef enum {
	CELL_DMUX_PAMF_BITS_PER_SAMPLE_16 = 16,
	CELL_DMUX_PAMF_BITS_PER_SAMPLE_24 = 24
} CellDmuxPamfBitsPerSample;

/*
 * LPCM Macros for Channel Assignment info
 */
typedef enum {
	CELL_DMUX_PAMF_LPCM_CH_M1 = 1,
	CELL_DMUX_PAMF_LPCM_CH_LR = 3,
	CELL_DMUX_PAMF_LPCM_CH_LRCLSRSLFE = 9,
	CELL_DMUX_PAMF_LPCM_CH_LRCLSCS1CS2RSLFE = 11
} CellDmuxPamfLpcmChannelAssignmentInfo;

/*
 * LPCM Macros for Sampling Frequency
 */
typedef enum {
	CELL_DMUX_PAMF_LPCM_FS_48K  = 1
} CellDmuxPamfLpcmFs;

/*
 * LPCM Macros for Bits Per Sample
 */
typedef enum {
	CELL_DMUX_PAMF_LPCM_BITS_PER_SAMPLE_16 = 1,
	CELL_DMUX_PAMF_LPCM_BITS_PER_SAMPLE_24 = 3
} CellDmuxPamfLpcmBitsPerSamples;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __DMUX_PAMF_H__ */
