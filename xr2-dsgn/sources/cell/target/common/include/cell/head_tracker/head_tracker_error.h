/* 
   SCE CONFIDENTIAL
   PlayStation(R)3 Programmer Tool Runtime Library 310.001
   Copyright (C) 2008 Sony Computer Entertainment Inc.
   All Rights Reserved.
*/

#ifndef __CELL_HEAD_TRACKER_ERROR_H__
#define __CELL_HEAD_TRACKER_ERROR_H__ 1

#include <cell/error.h>

/* error code
 * CELL_ERROR_FACILITY_MICCAM	0x014
 * libhead_tracker : 0x8014_0a01 - 0x8014_0aff
 */

#define CELL_HEAD_TRACKER_OK                            CELL_OK

// 引数の値が無効
#define	CELL_HEAD_TRACKER_ERROR_INVAL                   CELL_ERROR_CAST(0x80140a01)

// 引数アドレスのアライメントが不正
#define	CELL_HEAD_TRACKER_ERROR_ALIGN                   CELL_ERROR_CAST(0x80140a02)

// この操作に必要なメモリを割り当てられない
#define	CELL_HEAD_TRACKER_ERROR_NOMEM                   CELL_ERROR_CAST(0x80140a03)

// キャリブレーションに失敗した
#define	CELL_HEAD_TRACKER_ERROR_CALIB_FAIL              CELL_ERROR_CAST(0x80140a04)

// キャリブレーションが十分行われていない
#define	CELL_HEAD_TRACKER_ERROR_CALIB_INCOMPLETE        CELL_ERROR_CAST(0x80140a05)

#endif /* __CELL_HEAD_TRACKER_ERROR_H__ */
