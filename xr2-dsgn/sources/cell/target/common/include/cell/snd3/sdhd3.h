/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _CELL_SDHD3_H
#define	_CELL_SDHD3_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#include <sys/types.h>

/*
--------------------------------------------------------------
|CellHD3CommonAttr                                           |
|: CellHD3ProgramAttr_offset ------------+                   |
|: CellHD3ToneAttr_offset -------------+ |                   |
|: CellHD3VagAttr_offset ------------+ | |                   |
+                                    | | |                   |
|CellHD3ProgramAttr -----------------|-|-+                   |
|: CellHD3programParamCtx[127]       | |                     |
|------------------------------------|-|---------------------+
|CellHD3ToneAttr --------------------|-+                     |
|: CellHD3ToneParamCtx[]             |                       |
+------------------------------------|-----------------------+
|CellHD3VagAttr ---------------------+                       |
|: CellHD3VagParamCtx[]                                      |
+------------------------------------------------------------+
*/

/* Common Attribute */
typedef struct {
	int8_t     id[4];                     /* Identification ID           */
	uint32_t   attrSize;                  /* Attribute size (56)         */
	uint32_t   version;                   /* Version                     */
	uint32_t   reserve1;                  /* (Reserved area)             */
	uint32_t   CellHD3ProgramAttr_offset; /* Offset to program attribute */
	uint32_t   CellHD3ToneAttr_offset;    /* Offset to tone attribute    */
	uint32_t   CellHD3VagAttr_offset;     /* Offset to VAG attribute     */
	uint32_t   reserve2[9];               /* (Arbitrary data)            */
} CellHD3CommonAttr;

#define	NO_ATTR_OFFSET			(0xffffffff)

/* VAG Parameter */
typedef struct {
	uint32_t   offset;   /* Offset of waveform data from the beginning of the BD3 */
	uint32_t   fs;       /* Sampling frequency */
	uint32_t   size;     /* Waveform size      */
	uint32_t   reserve;  /* (Reserved area)    */
} CellHD3VagParamCtx;

/* VAG Attribute */
typedef struct {
	uint8_t     ID[4];           /* 'P3VA'                       */
	uint32_t    attrSize;        /* Attribute size               */
	uint32_t    vagParam_size;   /* VAG parameter size           */
	uint32_t    reserve1;        /* (Reserved area)              */
	uint32_t    index_low;       /* VAG parameter starting index */
	uint32_t    index_high;      /* VAG parameter ending index   */
	uint32_t    reserve2[2];     /* (Reserved area)              */
	CellHD3VagParamCtx     vagParam[0];
	/* vagParam is an array having the corresponding number of elements
	   from the starting index to the ending index. Each element
	   is a VAG parameter (CellHD3VagParamCtx structure defined above).
	   If there are unused indices among the indices from the starting index
	   to the ending index, the elements corresponding to those unused
	   indices are padded with 0xffffffff.                                    */
} CellHD3VagAttr;

/* Tone Parameter */
#define	MAX_PRIORITY    (127)
#define	MAX_GROUP       (127)
#define	MAX_NOTE        (127)
#define	MAX_VOLUME      (4096)
#define	MIN_VOLUME      (0)
#define	MAX_DETUNE      (99)
#define	MIN_DETUNE      (-99)
#define	MAX_PANPOT      (127)
#define	MAX_AR          (0x7f)
#define	MAX_AR_MODE     (1)
#define	MAX_DR          (0xf)
#define	MAX_SL          (0xf)
#define	MAX_SR          (0x7f)
#define	MAL_SR_MODE     (3)
#define	MAX_RR          (0x1f)
#define	MAX_RR_MODE     (1)
#define	MAX_BENDRANGE   (24)

#define	TONE_NOISE      (0xffffffff)

typedef struct{
	uint32_t   CellHD3VagParamCtxIndex; /* Index of VAG parameter used by */
                                        /* this tone parameter            */
                                        /* (for a noise sound source,     */
                                        /* this is 0xffffffff)            */
	uint32_t   priority;                /* Priority                  */
	uint32_t   group;                   /* Group                     */
	uint32_t   reserve1;                /* (Reserved area)           */
	uint32_t   note_low;                /* Lowest sound producing note number  */
	uint32_t   note_high;               /* Highest sound producing note number */
	uint32_t   reserve2[2];             /* (Reserved area)           */
	int32_t    volume;                  /* Volume                    */
	uint32_t   panpot;                  /* Panpot                    */
	int32_t    effectSwitch;            /* EffectSwitch              */
	uint32_t   reserve3;                /* (Reserved area)           */
	uint32_t   centerNote;              /* Note number when sampling */
	int32_t    centerDetune;            /* Detune when sampling      */
	int32_t    detune;                  /* Detune                    */
	uint32_t   reserve4;                /* (Reserved area)           */
	uint32_t   bendRange_lower;         /* Bend range                */
	uint32_t   bendRange_upper;         /* Bend range+               */
	uint32_t   reserve5;                /* (Reserved area)           */
	uint32_t   reserve6;                /* (Reserved area)           */
	uint32_t   adsr1;                   /* ADSR1 (envelope)          */
	uint32_t   adsr2;                   /* ADSR1 (envelope)          */
	uint32_t   panpotEx;                /* FR panpot                 */
	uint32_t   reserve7;                /* (Reserved area)           */
} CellHD3ToneParamCtx;


/* Tone Attribute */
typedef struct{
	uint8_t    ID[4];           /* 'P3TN'                   */
	uint32_t   attrSize;        /* Attribute size           */
	uint32_t   toneParam_size;  /* Tone parameter size      */
	uint32_t   reserve1;        /* (Reserved area)          */
	uint32_t   index_low;       /* toneParam starting index */
	uint32_t   index_high;      /* toneParam ending index   */
	uint32_t   reserve2[2];     /* (Reserved area)          */
	CellHD3ToneParamCtx   toneParam[0];
	/* toneParam is an array having the corresponding number of elements
	   from the starting index to the ending index. Each element is
	   a tone parameter (CellHD3ToneParamCtx structure defined above).
	   If there are unused indices among the indices from the starting
	   index to the ending index, the elements corresponding to those unused
	   indices are padded with 0xffffffff.*/
} CellHD3ToneAttr;

/* Program Parameter */
typedef struct{
	uint32_t   CellHD3ToneParamCtx_count;   /* Number of tone parameters that are 
	                                           used by this program (n)              */
	uint32_t   reserve[3];                  /* (Reserved area)                       */
	uint32_t   CellHD3ToneParamCtxIndex[0];
	/* toneParamCtx_count is the number of tone parameters that are used by the program.
	   The number of tone parameter indices that are used, which follow this entry,
	   is equal to the value specified for this count.                                  */
} CellHD3programParamCtx;

/* Program Attribute */
#define MAX_PROGRAM_NUM   (127)
#define PROGRAM_NOT_USE   (0xffffffff)

typedef struct{
	uint8_t     ID[4];                                    /* 'P3PG'          */
	uint32_t    attrSize;                                 /* Attribute size  */
	uint32_t    reserve1[6];                              /* (Reserved area) */
	uint32_t    programParam_offset[MAX_PROGRAM_NUM + 1]; /* Offset to program parameter of program number 0 
                                                             (number of bytes from beginning of HD3)         */
    /* The program attribute is data for associating a MIDI program number with a tone parameter.
       It is a structure consisting of a header, a table of offsets to program parameters, and 
       program parameters of indefinite size. */
} CellHD3ProgramAttr;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif	/* defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus) */

#endif	/* _CELL_SDHD3_H */
