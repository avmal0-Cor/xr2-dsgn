/*****************************************************************
 * SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (c) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 *
 * R & D Division, Sony Computer Entertainment Inc.
 *****************************************************************/

/** 
 * \error_code used by "MicIn" interface in <MIOS> and <LibMic>
 */
#ifndef __CELLMIC_ERROR_CODE__H
#define __CELLMIC_ERROR_CODE__H

#include <cell/error.h>

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
////////////////////////////////////////////////////////
/*  error code
 *	CELL_ERROR_FACILITY_MICCAM			0x014
 *	"MicIn" interface error code:	    0x8014_0101 - 0x8014_01ff
 *	"MicDSP" interface error code:	    0x8014_0200 - 0x8014_02ff
 */
#define CELL_MICIN_MAKE_ERROR(status) \
  CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_MICCAM, status)

#define CELL_MICIN_ERROR_ALREADY_INIT       CELL_MICIN_MAKE_ERROR(0x101) /* aready init */
#define CELL_MICIN_ERROR_SYSTEM             CELL_MICIN_MAKE_ERROR(0x102) /* error in device */
#define CELL_MICIN_ERROR_NOT_INIT           CELL_MICIN_MAKE_ERROR(0x103) /* not init */
#define CELL_MICIN_ERROR_PARAM              CELL_MICIN_MAKE_ERROR(0x104) /* param error */
#define CELL_MICIN_ERROR_PORT_FULL          CELL_MICIN_MAKE_ERROR(0x105) /* device port is full */
#define CELL_MICIN_ERROR_ALREADY_OPEN       CELL_MICIN_MAKE_ERROR(0x106) /* device is already opened */
#define CELL_MICIN_ERROR_NOT_OPEN           CELL_MICIN_MAKE_ERROR(0x107) /* device is already closed */
#define CELL_MICIN_ERROR_NOT_RUN            CELL_MICIN_MAKE_ERROR(0x108) /* device is not running */
#define CELL_MICIN_ERROR_TRANS_EVENT        CELL_MICIN_MAKE_ERROR(0x109) /* device trans event error */
#define CELL_MICIN_ERROR_OPEN               CELL_MICIN_MAKE_ERROR(0x10a) /* error in open */
#define CELL_MICIN_ERROR_SHAREDMEMORY       CELL_MICIN_MAKE_ERROR(0x10b) /* error in shared memory */
#define CELL_MICIN_ERROR_MUTEX              CELL_MICIN_MAKE_ERROR(0x10c) /* error in mutex */
#define CELL_MICIN_ERROR_EVENT_QUEUE        CELL_MICIN_MAKE_ERROR(0x10d) /* error in event queue */
#define CELL_MICIN_ERROR_SYSTEM_NOT_FOUND   CELL_MICIN_MAKE_ERROR(0x10e) /* device not found */
#define CELL_MICIN_ERROR_FATAL              CELL_MICIN_MAKE_ERROR(0x10f) /* generic error */
#define CELL_MICIN_ERROR_SYSTEM_NOT_SUPPORT CELL_MICIN_MAKE_ERROR(0x110) /* device is not supported */

#define CELL_MICIN_ERROR_DEVICE             CELL_MICIN_ERROR_SYSTEM
#define CELL_MICIN_ERROR_DEVICE_NOT_FOUND   CELL_MICIN_ERROR_SYSTEM_NOT_FOUND
#define CELL_MICIN_ERROR_DEVICE_NOT_SUPPORT CELL_MICIN_ERROR_SYSTEM_NOT_SUPPORT


/*
 * error code for microphone-signal-processing-library: 0x8014_0200 - 0x8014_02ff 
 */
#define CELL_MICIN_ERROR_DSP                CELL_MICIN_MAKE_ERROR(0x200)  /* generic error                        */
#define CELL_MICIN_ERROR_DSP_ASSERT         CELL_MICIN_MAKE_ERROR(0x201)  /* assertion failure                    */
#define CELL_MICIN_ERROR_DSP_PATH           CELL_MICIN_MAKE_ERROR(0x202)  /* Path is wrong in file system         */
#define CELL_MICIN_ERROR_DSP_FILE           CELL_MICIN_MAKE_ERROR(0x203)  /* File is currupped with               */      
#define CELL_MICIN_ERROR_DSP_PARAM          CELL_MICIN_MAKE_ERROR(0x204)  /* called function arg/param is bad     */
#define CELL_MICIN_ERROR_DSP_MEMALLOC       CELL_MICIN_MAKE_ERROR(0x205)  /* insufficient memory                  */
#define CELL_MICIN_ERROR_DSP_POINTER        CELL_MICIN_MAKE_ERROR(0x206)  /* Bad pointer to invalid memory addr   */
#define CELL_MICIN_ERROR_DSP_FUNC           CELL_MICIN_MAKE_ERROR(0x207)  /* unsupported function                 */ 
#define CELL_MICIN_ERROR_DSP_MEM            CELL_MICIN_MAKE_ERROR(0x208)  /* bad memory region                    */
#define CELL_MICIN_ERROR_DSP_ALIGN16        CELL_MICIN_MAKE_ERROR(0x209)  /* bad memory alignment in 16 bytes     */
#define CELL_MICIN_ERROR_DSP_ALIGN128       CELL_MICIN_MAKE_ERROR(0x20a)  /* bad memory alignment in 128 bytes    */
#define CELL_MICIN_ERROR_DSP_EAALIGN128     CELL_MICIN_MAKE_ERROR(0x20b)  /* bad effective address(EA) alignment in 128 bytes         */
#define CELL_MICIN_ERROR_DSP_LIB_HANDLER    CELL_MICIN_MAKE_ERROR(0x216)  /* invalid dsp library handler, mostly memory curruption    */
#define CELL_MICIN_ERROR_DSP_LIB_INPARAM    CELL_MICIN_MAKE_ERROR(0x217)  /* unrecognized input parameter to dsp library API          */
#define CELL_MICIN_ERROR_DSP_LIB_NOSPU      CELL_MICIN_MAKE_ERROR(0x218)  /* failure to communicate with SPU side dsp library         */
#define CELL_MICIN_ERROR_DSP_LIB_SAMPRATE   CELL_MICIN_MAKE_ERROR(0x219)  /* this sampling rate is not supported by dsp library       */

////////////////////////////////////////////////////////
#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

/* end of error code definition */

#endif /* __CELLMIC_ERROR_CODE__H */

