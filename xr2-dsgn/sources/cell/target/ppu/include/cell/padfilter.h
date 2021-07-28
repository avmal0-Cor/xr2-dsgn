/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/*
	IIR Filter (integer version, C bind)
*/
#ifndef __CELL_PADFILTER_IIRFILTERINT_H__
#define __CELL_PADFILTER_IIRFILTERINT_H__

#include <cell/error.h>
#include <stdint.h>
#include <types.h>

/*
  libpadfilter error code: 0x80121400 - 0x80121401
*/

#define CELL_PADFILTER_OK                          CELL_OK
#define CELL_PADFILTER_ERROR_INVALID_PARAMETER     CELL_ERROR_CAST(0x80121401)

/* cutoff */
enum {
    CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_050 = 0,	/* fc=0.50*f-nyquist */
    CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_020 = 1,	/* fc=0.20*f-nyquist */
    CELL_PADFILTER_IIR_CUTOFF_2ND_LPF_BT_010 = 2,	/* fc=0.10*f-nyquist */
};

/* IIR Filter Second Order Section */
typedef struct CellPadFilterIIRSos{
	/* coefficient */
	int32_t u[3];
	int32_t a1;
	int32_t a2;
	int32_t b0;
	int32_t b1;
	int32_t b2;
} CellPadFilterIIRSos;

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

int32_t cellPadFilterIIRInit(CellPadFilterIIRSos *pSos, int32_t cutoff);
uint32_t cellPadFilterIIRFilter(CellPadFilterIIRSos *pSos, uint32_t filterIn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_PADFILTER_IIRFILTERINT_H__ */
