/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_VPOST_H__
#define __CELL_VPOST_H__


/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       Header Files                                                         */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <types.h>

#include <cell/spurs.h>
#include <cell/error.h>



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       for C++                                                              */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/* for C++ */
#ifdef __cplusplus
extern "C" {
#endif



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       error code                                                           */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/
/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       vpost library  (PPM)                                                 */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/*****  for cellVpostQueryAttr()  *****/
#define CELL_VPOST_ERROR_Q_ARG_CFG_NULL    CELL_ERROR_CAST( 0x80610410 )
#define CELL_VPOST_ERROR_Q_ARG_CFG_INVALID CELL_ERROR_CAST( 0x80610411 )
#define CELL_VPOST_ERROR_Q_ARG_ATTR_NULL   CELL_ERROR_CAST( 0x80610412 )


/*****  for cellVpostOpen()  *****/
#define CELL_VPOST_ERROR_O_ARG_CFG_NULL         CELL_ERROR_CAST( 0x80610440 )
#define CELL_VPOST_ERROR_O_ARG_CFG_INVALID      CELL_ERROR_CAST( 0x80610441 )
#define CELL_VPOST_ERROR_O_ARG_RSRC_NULL        CELL_ERROR_CAST( 0x80610442 )
#define CELL_VPOST_ERROR_O_ARG_RSRC_INVALID     CELL_ERROR_CAST( 0x80610443 )
#define CELL_VPOST_ERROR_O_ARG_HDL_NULL         CELL_ERROR_CAST( 0x80610444 )

#define CELL_VPOST_ERROR_O_FATAL_QUERY_FAIL     CELL_ERROR_CAST( 0x80610460 )
#define CELL_VPOST_ERROR_O_FATAL_CREATEMON_FAIL CELL_ERROR_CAST( 0x80610461 )
#define CELL_VPOST_ERROR_O_FATAL_INITSPURS_FAIL CELL_ERROR_CAST( 0x80610462 )


/*****  for cellVpostClose()  *****/
#define CELL_VPOST_ERROR_C_ARG_HDL_NULL          CELL_ERROR_CAST( 0x80610470 )
#define CELL_VPOST_ERROR_C_ARG_HDL_INVALID       CELL_ERROR_CAST( 0x80610471 )

#define CELL_VPOST_ERROR_C_FATAL_LOCKMON_FAIL    CELL_ERROR_CAST( 0x80610490 )
#define CELL_VPOST_ERROR_C_FATAL_UNLOCKMON_FAIL  CELL_ERROR_CAST( 0x80610491 )
#define CELL_VPOST_ERROR_C_FATAL_DESTROYMON_FAIL CELL_ERROR_CAST( 0x80610492 )
#define CELL_VPOST_ERROR_C_FATAL_FINSPURS_FAIL   CELL_ERROR_CAST( 0x80610463 )


/*****  for cellVpostExec()  *****/
#define CELL_VPOST_ERROR_E_ARG_HDL_NULL           CELL_ERROR_CAST( 0x806104a0 )
#define CELL_VPOST_ERROR_E_ARG_HDL_INVALID        CELL_ERROR_CAST( 0x806104a1 )
#define CELL_VPOST_ERROR_E_ARG_INPICBUF_NULL      CELL_ERROR_CAST( 0x806104a2 )
#define CELL_VPOST_ERROR_E_ARG_INPICBUF_INVALID   CELL_ERROR_CAST( 0x806104a3 )
#define CELL_VPOST_ERROR_E_ARG_CTRL_NULL          CELL_ERROR_CAST( 0x806104a4 )
#define CELL_VPOST_ERROR_E_ARG_CTRL_INVALID       CELL_ERROR_CAST( 0x806104a5 )
#define CELL_VPOST_ERROR_E_ARG_OUTPICBUF_NULL     CELL_ERROR_CAST( 0x806104a6 )
#define CELL_VPOST_ERROR_E_ARG_OUTPICBUF_INVALID  CELL_ERROR_CAST( 0x806104a7 )
#define CELL_VPOST_ERROR_E_ARG_PICINFO_NULL       CELL_ERROR_CAST( 0x806104a8 )

#define CELL_VPOST_ERROR_E_FATAL_LOCKMON_FAIL     CELL_ERROR_CAST( 0x806104c0 )
#define CELL_VPOST_ERROR_E_FATAL_UNLOCKMON_FAIL   CELL_ERROR_CAST( 0x806104c1 )



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       Entry Module                                                         */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/*****  for cellVpostEntQueryAttr()  *****/
#define CELL_VPOST_ENT_ERROR_Q_ARG_ATTR_NULL CELL_ERROR_CAST( 0x80618110 )


/*****  for cellVpostEntOpen()  *****/
#define CELL_VPOST_ENT_ERROR_O_ARG_RSRC_NULL        CELL_ERROR_CAST( 0x80618140 )
#define CELL_VPOST_ENT_ERROR_O_ARG_RSRC_INVALID     CELL_ERROR_CAST( 0x80618141 )
#define CELL_VPOST_ENT_ERROR_O_ARG_HDL_NULL         CELL_ERROR_CAST( 0x80618142 )

#define CELL_VPOST_ENT_ERROR_O_FATAL_QUERY_FAIL     CELL_ERROR_CAST( 0x80618160 )
#define CELL_VPOST_ENT_ERROR_O_FATAL_CSPUCORE_FAIL  CELL_ERROR_CAST( 0x80618161 )


/*****  for cellVpostEntClose()  *****/
#define CELL_VPOST_ENT_ERROR_C_ARG_HDL_NULL          CELL_ERROR_CAST( 0x80618170 )
#define CELL_VPOST_ENT_ERROR_C_ARG_HDL_INVALID       CELL_ERROR_CAST( 0x80618171 )

#define CELL_VPOST_ENT_ERROR_C_FATAL_SNDCMD_FAIL     CELL_ERROR_CAST( 0x80618190 )
#define CELL_VPOST_ENT_ERROR_C_FATAL_RCVRES_FAIL     CELL_ERROR_CAST( 0x80618191 )
#define CELL_VPOST_ENT_ERROR_C_FATAL_DSPUCORE_FAIL   CELL_ERROR_CAST( 0x80618192 )


/*****  for cellVpostEntExec()  *****/
#define CELL_VPOST_ENT_ERROR_E_ARG_HDL_NULL           CELL_ERROR_CAST( 0x806181a0 )
#define CELL_VPOST_ENT_ERROR_E_ARG_HDL_INVALID        CELL_ERROR_CAST( 0x806181a1 )
#define CELL_VPOST_ENT_ERROR_E_ARG_INPICBUF_NULL      CELL_ERROR_CAST( 0x806181a2 )
#define CELL_VPOST_ENT_ERROR_E_ARG_INPICBUF_INVALID   CELL_ERROR_CAST( 0x806181a3 )
#define CELL_VPOST_ENT_ERROR_E_ARG_INPICINFO_NULL     CELL_ERROR_CAST( 0x806181a4 )
#define CELL_VPOST_ENT_ERROR_E_ARG_INPICINFO_INVALID  CELL_ERROR_CAST( 0x806181a5 )
#define CELL_VPOST_ENT_ERROR_E_ARG_CTRL_NULL          CELL_ERROR_CAST( 0x806181a6 )
#define CELL_VPOST_ENT_ERROR_E_ARG_CTRL_INVALID       CELL_ERROR_CAST( 0x806181a7 )
#define CELL_VPOST_ENT_ERROR_E_ARG_COMB_INVALID       CELL_ERROR_CAST( 0x806181a8 )
#define CELL_VPOST_ENT_ERROR_E_ARG_OUTPICBUF_NULL     CELL_ERROR_CAST( 0x806181a9 )
#define CELL_VPOST_ENT_ERROR_E_ARG_OUTPICBUF_INVALID  CELL_ERROR_CAST( 0x806181aa )
#define CELL_VPOST_ENT_ERROR_E_ARG_OUTPICINFO_NULL    CELL_ERROR_CAST( 0x806181ab )

#define CELL_VPOST_ENT_ERROR_E_FATAL_SNDCMD_FAIL      CELL_ERROR_CAST( 0x806181c0 )
#define CELL_VPOST_ENT_ERROR_E_FATAL_RCVRES_FAIL      CELL_ERROR_CAST( 0x806181c1 )
#define CELL_VPOST_ENT_ERROR_E_FATAL_SPUCORE_FAIL     CELL_ERROR_CAST( 0x806181c2 )



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       IPC                                                                  */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/*****  for cellVpostIpcQueryAttr()  *****/
#define CELL_VPOST_IPC_ERROR_Q_ARG_ATTR_NULL CELL_ERROR_CAST( 0x80618210 )


/*****  for cellVpostIpcOpen()  *****/
#define CELL_VPOST_IPC_ERROR_O_ARG_RSRC_NULL        CELL_ERROR_CAST( 0x80618240 )
#define CELL_VPOST_IPC_ERROR_O_ARG_RSRC_INVALID     CELL_ERROR_CAST( 0x80618241 )
#define CELL_VPOST_IPC_ERROR_O_ARG_HDL_NULL         CELL_ERROR_CAST( 0x80618242 )

#define CELL_VPOST_IPC_ERROR_O_FATAL_QUERY_FAIL     CELL_ERROR_CAST( 0x80618260 )
#define CELL_VPOST_IPC_ERROR_O_FATAL_CSPUCORE_FAIL  CELL_ERROR_CAST( 0x80618261 )


/*****  for cellVpostIpcClose()  *****/
#define CELL_VPOST_IPC_ERROR_C_ARG_HDL_NULL          CELL_ERROR_CAST( 0x80618270 )
#define CELL_VPOST_IPC_ERROR_C_ARG_HDL_INVALID       CELL_ERROR_CAST( 0x80618271 )

#define CELL_VPOST_IPC_ERROR_C_FATAL_SNDCMD_FAIL     CELL_ERROR_CAST( 0x80618290 )
#define CELL_VPOST_IPC_ERROR_C_FATAL_RCVRES_FAIL     CELL_ERROR_CAST( 0x80618291 )
#define CELL_VPOST_IPC_ERROR_C_FATAL_DSPUCORE_FAIL   CELL_ERROR_CAST( 0x80618292 )


/*****  for cellVpostIpcExec()  *****/
#define CELL_VPOST_IPC_ERROR_E_ARG_HDL_NULL           CELL_ERROR_CAST( 0x806182a0 )
#define CELL_VPOST_IPC_ERROR_E_ARG_HDL_INVALID        CELL_ERROR_CAST( 0x806182a1 )
#define CELL_VPOST_IPC_ERROR_E_ARG_INPICBUF_NULL      CELL_ERROR_CAST( 0x806182a2 )
#define CELL_VPOST_IPC_ERROR_E_ARG_INPICBUF_INVALID   CELL_ERROR_CAST( 0x806182a3 )
#define CELL_VPOST_IPC_ERROR_E_ARG_INPICINFO_NULL     CELL_ERROR_CAST( 0x806182a4 )
#define CELL_VPOST_IPC_ERROR_E_ARG_INPICINFO_INVALID  CELL_ERROR_CAST( 0x806182a5 )
#define CELL_VPOST_IPC_ERROR_E_ARG_CTRL_NULL          CELL_ERROR_CAST( 0x806182a6 )
#define CELL_VPOST_IPC_ERROR_E_ARG_CTRL_INVALID       CELL_ERROR_CAST( 0x806182a7 )
#define CELL_VPOST_IPC_ERROR_E_ARG_COMB_INVALID       CELL_ERROR_CAST( 0x806182a8 )
#define CELL_VPOST_IPC_ERROR_E_ARG_OUTPICBUF_NULL     CELL_ERROR_CAST( 0x806182a9 )
#define CELL_VPOST_IPC_ERROR_E_ARG_OUTPICBUF_INVALID  CELL_ERROR_CAST( 0x806182aa )
#define CELL_VPOST_IPC_ERROR_E_ARG_OUTPICINFO_NULL    CELL_ERROR_CAST( 0x806182ab )

#define CELL_VPOST_IPC_ERROR_E_FATAL_SNDCMD_FAIL      CELL_ERROR_CAST( 0x806182c0 )
#define CELL_VPOST_IPC_ERROR_E_FATAL_RCVRES_FAIL      CELL_ERROR_CAST( 0x806182c1 )
#define CELL_VPOST_IPC_ERROR_E_FATAL_SPUCORE_FAIL     CELL_ERROR_CAST( 0x806182c2 )



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       video scaler                                                         */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/*****  for cellVpostVscQueryAttr()  *****/
#define CELL_VPOST_VSC_ERROR_Q_ARG_ATTR_NULL CELL_ERROR_CAST( 0x80618310 )


/*****  for cellVpostVscOpen()  *****/
#define CELL_VPOST_VSC_ERROR_O_ARG_RSRC_NULL        CELL_ERROR_CAST( 0x80618340 )
#define CELL_VPOST_VSC_ERROR_O_ARG_RSRC_INVALID     CELL_ERROR_CAST( 0x80618341 )
#define CELL_VPOST_VSC_ERROR_O_ARG_HDL_NULL         CELL_ERROR_CAST( 0x80618342 )

#define CELL_VPOST_VSC_ERROR_O_FATAL_QUERY_FAIL     CELL_ERROR_CAST( 0x80618360 )
#define CELL_VPOST_VSC_ERROR_O_FATAL_CSPUCORE_FAIL  CELL_ERROR_CAST( 0x80618361 )


/*****  for cellVpostVscClose()  *****/
#define CELL_VPOST_VSC_ERROR_C_ARG_HDL_NULL          CELL_ERROR_CAST( 0x80618370 )
#define CELL_VPOST_VSC_ERROR_C_ARG_HDL_INVALID       CELL_ERROR_CAST( 0x80618371 )

#define CELL_VPOST_VSC_ERROR_C_FATAL_SNDCMD_FAIL     CELL_ERROR_CAST( 0x80618390 )
#define CELL_VPOST_VSC_ERROR_C_FATAL_RCVRES_FAIL     CELL_ERROR_CAST( 0x80618391 )
#define CELL_VPOST_VSC_ERROR_C_FATAL_DSPUCORE_FAIL   CELL_ERROR_CAST( 0x80618392 )


/*****  for cellVpostVscExec()  *****/
#define CELL_VPOST_VSC_ERROR_E_ARG_HDL_NULL           CELL_ERROR_CAST( 0x806183a0 )
#define CELL_VPOST_VSC_ERROR_E_ARG_HDL_INVALID        CELL_ERROR_CAST( 0x806183a1 )
#define CELL_VPOST_VSC_ERROR_E_ARG_INPICBUF_NULL      CELL_ERROR_CAST( 0x806183a2 )
#define CELL_VPOST_VSC_ERROR_E_ARG_INPICBUF_INVALID   CELL_ERROR_CAST( 0x806183a3 )
#define CELL_VPOST_VSC_ERROR_E_ARG_INPICINFO_NULL     CELL_ERROR_CAST( 0x806183a4 )
#define CELL_VPOST_VSC_ERROR_E_ARG_INPICINFO_INVALID  CELL_ERROR_CAST( 0x806183a5 )
#define CELL_VPOST_VSC_ERROR_E_ARG_CTRL_NULL          CELL_ERROR_CAST( 0x806183a6 )
#define CELL_VPOST_VSC_ERROR_E_ARG_CTRL_INVALID       CELL_ERROR_CAST( 0x806183a7 )
#define CELL_VPOST_VSC_ERROR_E_ARG_COMB_INVALID       CELL_ERROR_CAST( 0x806183a8 )
#define CELL_VPOST_VSC_ERROR_E_ARG_OUTPICBUF_NULL     CELL_ERROR_CAST( 0x806183a9 )
#define CELL_VPOST_VSC_ERROR_E_ARG_OUTPICBUF_INVALID  CELL_ERROR_CAST( 0x806183aa )
#define CELL_VPOST_VSC_ERROR_E_ARG_OUTPICINFO_NULL    CELL_ERROR_CAST( 0x806183ab )

#define CELL_VPOST_VSC_ERROR_E_FATAL_SNDCMD_FAIL      CELL_ERROR_CAST( 0x806183c0 )
#define CELL_VPOST_VSC_ERROR_E_FATAL_RCVRES_FAIL      CELL_ERROR_CAST( 0x806183c1 )
#define CELL_VPOST_VSC_ERROR_E_FATAL_SPUCORE_FAIL     CELL_ERROR_CAST( 0x806183c2 )



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       CSC                                                                  */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/*****  for cellVpostCscQueryAttr()  *****/
#define CELL_VPOST_CSC_ERROR_Q_ARG_ATTR_NULL CELL_ERROR_CAST( 0x80618410 )


/*****  for cellVpostCscOpen()  *****/
#define CELL_VPOST_CSC_ERROR_O_ARG_RSRC_NULL        CELL_ERROR_CAST( 0x80618440 )
#define CELL_VPOST_CSC_ERROR_O_ARG_RSRC_INVALID     CELL_ERROR_CAST( 0x80618441 )
#define CELL_VPOST_CSC_ERROR_O_ARG_HDL_NULL         CELL_ERROR_CAST( 0x80618442 )

#define CELL_VPOST_CSC_ERROR_O_FATAL_QUERY_FAIL     CELL_ERROR_CAST( 0x80618460 )
#define CELL_VPOST_CSC_ERROR_O_FATAL_CSPUCORE_FAIL  CELL_ERROR_CAST( 0x80618461 )


/*****  for cellVpostCscClose()  *****/
#define CELL_VPOST_CSC_ERROR_C_ARG_HDL_NULL          CELL_ERROR_CAST( 0x80618470 )
#define CELL_VPOST_CSC_ERROR_C_ARG_HDL_INVALID       CELL_ERROR_CAST( 0x80618471 )

#define CELL_VPOST_CSC_ERROR_C_FATAL_SNDCMD_FAIL     CELL_ERROR_CAST( 0x80618490 )
#define CELL_VPOST_CSC_ERROR_C_FATAL_RCVRES_FAIL     CELL_ERROR_CAST( 0x80618491 )
#define CELL_VPOST_CSC_ERROR_C_FATAL_DSPUCORE_FAIL   CELL_ERROR_CAST( 0x80618492 )


/*****  for cellVpostCscExec()  *****/
#define CELL_VPOST_CSC_ERROR_E_ARG_HDL_NULL           CELL_ERROR_CAST( 0x806184a0 )
#define CELL_VPOST_CSC_ERROR_E_ARG_HDL_INVALID        CELL_ERROR_CAST( 0x806184a1 )
#define CELL_VPOST_CSC_ERROR_E_ARG_INPICBUF_NULL      CELL_ERROR_CAST( 0x806184a2 )
#define CELL_VPOST_CSC_ERROR_E_ARG_INPICBUF_INVALID   CELL_ERROR_CAST( 0x806184a3 )
#define CELL_VPOST_CSC_ERROR_E_ARG_INPICINFO_NULL     CELL_ERROR_CAST( 0x806184a4 )
#define CELL_VPOST_CSC_ERROR_E_ARG_INPICINFO_INVALID  CELL_ERROR_CAST( 0x806184a5 )
#define CELL_VPOST_CSC_ERROR_E_ARG_CTRL_NULL          CELL_ERROR_CAST( 0x806184a6 )
#define CELL_VPOST_CSC_ERROR_E_ARG_CTRL_INVALID       CELL_ERROR_CAST( 0x806184a7 )
#define CELL_VPOST_CSC_ERROR_E_ARG_COMB_INVALID       CELL_ERROR_CAST( 0x806184a8 )
#define CELL_VPOST_CSC_ERROR_E_ARG_OUTPICBUF_NULL     CELL_ERROR_CAST( 0x806184a9 )
#define CELL_VPOST_CSC_ERROR_E_ARG_OUTPICBUF_INVALID  CELL_ERROR_CAST( 0x806184aa )
#define CELL_VPOST_CSC_ERROR_E_ARG_OUTPICINFO_NULL    CELL_ERROR_CAST( 0x806184ab )

#define CELL_VPOST_CSC_ERROR_E_FATAL_SNDCMD_FAIL      CELL_ERROR_CAST( 0x806184c0 )
#define CELL_VPOST_CSC_ERROR_E_FATAL_RCVRES_FAIL      CELL_ERROR_CAST( 0x806184c1 )
#define CELL_VPOST_CSC_ERROR_E_FATAL_SPUCORE_FAIL     CELL_ERROR_CAST( 0x806184c2 )



/********|*********|*********|*********|*********|*********|*********|*********/

/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       Data-Types                                                           */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/* handle of post processing for video */
typedef void* CellVpostHandle;


/* picture depth */
typedef enum
{
	CELL_VPOST_PIC_DEPTH_8,  /* 8 bit per 1 component of pixel */
}CellVpostPictureDepth;


/* input picture format */
typedef enum
{
	CELL_VPOST_PIC_FMT_IN_YUV420_PLANAR,  /* YUV, 4:2:0, planar format */
}CellVpostPictureFormatIn;


/* output picture format */
typedef enum
{
	CELL_VPOST_PIC_FMT_OUT_RGBA_ILV,       /* RGBA interleaved          */
	CELL_VPOST_PIC_FMT_OUT_YUV420_PLANAR,  /* YUV, 4:2:0, planar format */
}CellVpostPictureFormatOut;


/* picture sturcure */
typedef enum
{
	/* frame ( progressive ) */
	CELL_VPOST_PIC_STRUCT_PFRM,

	/* interlaced-frame ( both top field and bottom field ) */
	CELL_VPOST_PIC_STRUCT_IFRM,

	/* interlaced-frame ( top field ) */
	CELL_VPOST_PIC_STRUCT_ITOP,

	/* interlaced-frame ( bottom field ) */
	CELL_VPOST_PIC_STRUCT_IBTM,

}CellVpostPictureStructure;


/* execution type of post-processing */
typedef enum
{
	/* P->P, frame->frame */
	CELL_VPOST_EXEC_TYPE_PFRM_PFRM,

	/* P->I, frame(top)->interlaced-frame(top) */
	CELL_VPOST_EXEC_TYPE_PTOP_ITOP,

	/* P->I, frame(bottom)->interlaced-frame(bottom) */
	CELL_VPOST_EXEC_TYPE_PBTM_IBTM,

	/* I->P, interlaced-frame(top)->frame */
	CELL_VPOST_EXEC_TYPE_ITOP_PFRM,

	/* I->P, interlaced-frame(bottom)->frame */
	CELL_VPOST_EXEC_TYPE_IBTM_PFRM,

	/* I->I, interlaced-frame(both fields)->interlaced-frame(both fields) */
	CELL_VPOST_EXEC_TYPE_IFRM_IFRM,

	/* I->I, interlaced-frame(top)->interlaced-frame(top) */
	CELL_VPOST_EXEC_TYPE_ITOP_ITOP,

	/* I->I, interlaced-frame(bottom)->interlaced-frame(bottom) */
	CELL_VPOST_EXEC_TYPE_IBTM_IBTM,
}CellVpostExecType;


/* type of chroma position */
typedef enum
{
	CELL_VPOST_CHROMA_POS_TYPE_A,  /* MPEG1           */
	CELL_VPOST_CHROMA_POS_TYPE_B,  /* MPEG2, AVC, VC1 */
}CellVpostChromaPositionType;


/* type of scan method */
typedef enum 
{
	CELL_VPOST_SCAN_TYPE_P,  /* progressive */
	CELL_VPOST_SCAN_TYPE_I,  /* interlace   */
}CellVpostScanType;


/* range of Quantization Level */
typedef enum
{
	CELL_VPOST_QUANT_RANGE_FULL,       /* full range      */
	CELL_VPOST_QUANT_RANGE_BROADCAST,  /* broadcast range */
}CellVpostQuantRange;


/* matrix for color space conversion */
typedef enum
{
	CELL_VPOST_COLOR_MATRIX_BT601,  /* ITU-R Rec. BT.601 */
	CELL_VPOST_COLOR_MATRIX_BT709,  /* ITU-R Rec. BT.709 */
}CellVpostColorMatrix;


/* type of video scaler algorithm */
typedef enum
{
	CELL_VPOST_SCALER_TYPE_BILINEAR,      /* bilinear              */
	CELL_VPOST_SCALER_TYPE_LINEAR_SHARP,  /* linear with sharpness */
	CELL_VPOST_SCALER_TYPE_2X4TAP,        /* 2x4-tap filtering     */
	CELL_VPOST_SCALER_TYPE_8X4TAP,        /* 8x4-tap filtering     */
}CellVpostScalerType;


/* type of IPC( interlace to progressive conversion ) algorithm */
typedef enum
{
	CELL_VPOST_IPC_TYPE_DOUBLING,  /* line doubling          */
	CELL_VPOST_IPC_TYPE_LINEAR,    /* line averaging         */
	CELL_VPOST_IPC_TYPE_MAVG,      /* matched line averaging */
}CellVpostIpcType;


/* paramerters for configuration of the post-processing modules for video */
typedef struct
{
	/* for input picture */
	uint32_t                 inMaxWidth;    /* maximum width   */
	uint32_t                 inMaxHeight;   /* maximum height  */
	CellVpostPictureDepth    inDepth;       /* depth           */
	CellVpostPictureFormatIn inPicFmt;      /* picture format  */

	/* for output picture */ 
	uint32_t                  outMaxWidth;  /* maximum width   */
	uint32_t                  outMaxHeight; /* maximum height  */
	CellVpostPictureDepth     outDepth;     /* depth           */
	CellVpostPictureFormatOut outPicFmt;    /* picture format  */

	uint32_t reserved1;
	uint32_t reserved2;
}CellVpostCfgParam;


/* attributes of the post-processing modules for video */
typedef struct
{
	size_t   memSize;       /* size of memory to be used in library           */
	uint8_t  delay;         /* delay                                          */
	uint32_t vpostVerUpper; /* version of the post-processing modules (upper) */
	uint32_t vpostVerLower; /* version of the post-processing modules (lower) */
}CellVpostAttr;


/* system resources for the post-processing modules for video */
typedef struct
{
	void     *memAddr;            /* start address of memory to be allocated to lib. */
	size_t    memSize;            /* size of memory to be allocated to lib.          */
	int32_t   ppuThreadPriority;  /* priority of PPU thread to be started in lib.    */
	size_t    ppuThreadStackSize; /* stack size of PPU thread started in lib.        */
	int32_t   spuThreadPriority;  /* priority of SPU thread started in lib.          */
	uint32_t  numOfSpus;          /* number of SPUs to be used in lib.               */
}CellVpostResource;


/* system resources for the post-processing modules for video */
typedef struct
{
	void      *memAddr;           /* start address of memory to be allocated to lib. */
	size_t     memSize;           /* size of memory to be allocated to lib.          */
	CellSpurs *spurs;             /* pointer to SPURS instance                       */
	uint8_t    priority[ 8 ];     /* priority of SPURS task                          */
	uint32_t   maxContention;
}CellVpostResourceEx;


/* window information */
typedef struct
{
	uint32_t x;       /* horizontal start position */
	uint32_t y;       /* vertival start position   */
	uint32_t width;   /* width of window           */
	uint32_t height;  /* height of window          */
}CellVpostWindow;


/* parameters for controlling the post-processing modules for video */
typedef struct
{
	CellVpostExecType execType;  /* execution tyep of post-processing */

	/* for algorithm */
	CellVpostScalerType scalerType;  /* type of video scaler */
	CellVpostIpcType    ipcType;     /* type of IPC          */

	/* for input picture */
	uint32_t inWidth;                            /* width                    */
	uint32_t inHeight;                           /* height                   */
	CellVpostChromaPositionType inChromaPosType; /* chroma position          */
	CellVpostQuantRange         inQuantRange;    /* range of quantization    */
	CellVpostColorMatrix        inColorMatrix;   /* color matrix             */
	CellVpostWindow             inWindow;        /* window information (cut) */

	/* for output picture */
	uint32_t        outWidth;     /* width                      */
	uint32_t        outHeight;    /* height                     */
	CellVpostWindow outWindow;    /* window information (paste) */
	uint8_t         outAlpha;     /* alpha                      */

	/* for user data */
	uint64_t userData;

	uint32_t reserved1;
	uint32_t reserved2;
}CellVpostCtrlParam;


/* picture information */
typedef struct
{
	/* for input picture */
	uint32_t inWidth;                             /* width                 */
	uint32_t inHeight;                            /* height                */
	CellVpostPictureDepth       inDepth;          /* depth                 */
	CellVpostScanType           inScanType;       /* type of scan method   */
	CellVpostPictureFormatIn    inPicFmt;         /* picture format        */
	CellVpostChromaPositionType inChromaPosType;  /* chroma position       */
	CellVpostPictureStructure   inPicStruct;      /* picture structure     */
	CellVpostQuantRange         inQuantRange;     /* range of quantization */
	CellVpostColorMatrix        inColorMatrix;    /* color matrix          */

	/* for output(converted) picture */
	uint32_t outWidth;                            /* width                 */
	uint32_t outHeight;                           /* height                */
	CellVpostPictureDepth       outDepth;         /* depth                 */
	CellVpostScanType           outScanType;      /* type of scan method   */
	CellVpostPictureFormatOut   outPicFmt;        /* picture format        */
	CellVpostChromaPositionType outChromaPosType; /* chroma position       */
	CellVpostPictureStructure   outPicStruct;     /* picture structure     */
	CellVpostQuantRange         outQuantRange;    /* range of quantization */
	CellVpostColorMatrix        outColorMatrix;   /* color matrix          */

	/* for user data */
	uint64_t userData;

	uint32_t reserved1;
	uint32_t reserved2;
}CellVpostPictureInfo;



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       API functions                                                        */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/


/* query the attributes of the post-processing modules for video */
int32_t cellVpostQueryAttr(
	const CellVpostCfgParam *cfgParam,
	      CellVpostAttr     *attr
);


/* open the post-processing modules for video */
int32_t cellVpostOpen(
	const CellVpostCfgParam *cfgParam,
	const CellVpostResource *resource,
	      CellVpostHandle   *handle
);


/* open the post-processing modules for video */
int32_t cellVpostOpenEx(
	const CellVpostCfgParam   *cfgParam,
	const CellVpostResourceEx *resource,
	      CellVpostHandle     *handle
);


/* close the post-processing modules for video */
int32_t cellVpostClose(
	CellVpostHandle handle
);


/* execute the post-processing modules for video */
int32_t cellVpostExec(
	      CellVpostHandle       handle,
	const void                 *inPicBuff,
	const CellVpostCtrlParam   *ctrlParam,
	      void                 *outPicBuff,
	      CellVpostPictureInfo *picInfo
);



/********|*********|*********|*********|*********|*********|*********|*********/
/*                                                                            */
/*       for C++                                                              */
/*                                                                            */
/********|*********|*********|*********|*********|*********|*********|*********/

/* for C++ */
#ifdef __cplusplus
}
#endif



/********|*********|*********|*********|*********|*********|*********|*********/


#endif  /* __CELL_VPOST_H__*/
