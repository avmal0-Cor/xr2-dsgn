/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/******************************************************************
*	filename:a_dec_base.h
*	function:
*	auther:Koichi Chiba
*	history: 2005.01.31 v0.1.0.0 New
*			 2005.03.11 v0.1.0.1 "#ifdef  SCE_AUD_DEC_BUILD_LIB" added
*			 2005.03.26 v0.1.0.2 Modify all
*******************************************************************/

#ifndef	 __CELL_ADEC_BASE_H__
#define	 __CELL_ADEC_BASE_H__

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include <cell/error.h>

#define CELL_DEC_CELL_LV2 1
#define CELL_DEC_DBG_ON 0

/*
	CELL_ERROR_FACILITY_CODEC			 0x061
	liba_dec		: 0x8061_0001 - 0x8061_00ff
*/

#define CELL_ADEC_ERROR_FATAL	CELL_ERROR_CAST(0x80610001)
#define CELL_ADEC_ERROR_SEQ		CELL_ERROR_CAST(0x80610002)
#define CELL_ADEC_ERROR_ARG		CELL_ERROR_CAST(0x80610003)
#define CELL_ADEC_ERROR_BUSY	CELL_ERROR_CAST(0x80610004)
#define CELL_ADEC_ERROR_EMPTY	CELL_ERROR_CAST(0x80610005)
	
#define CELL_ADEC_MEM_ALIGN 128

#include <sys/ppu_thread.h>
#include <sys/synchronization.h>
#include <sys/timer.h>  /* for usleep() */
#include <sys/memory.h>

/* PTS Invalid value */
#define CELL_ADEC_PTS_INVALID		CELL_CODEC_PTS_INVALID

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* __CELL_ADEC_BASE_H__ */
