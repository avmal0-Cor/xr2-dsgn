/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* Nonblocking Signal notification
 *  Cosecutive Siganal-Notifications are possible using this module
 *  (1) Send signal data to destination SPU Local Storage that can keep multiple outstanding Signals
 *  (2) Send source unit mask Signal-Notification w/Fence by OR-mode
 */

#ifndef __CELL_DAISY_NBSNR_H__
#define __CELL_DAISY_NBSNR_H__

#include <stdint.h>
#include <stdbool.h>

/* Make sure both signal1 and signal2 is ***OR MODE*** */

#define CELL_DAISY_NBSNR_PARAM_SIZE 256

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * initialize nonblocksnr
	 * @param paramEa parameter buffer EA *** 256bytes ***
	 * @param mySpuNum spunum of myself
	 * @param groupNum spu thread count which participate in nonblocksnr
	 */
	void cellDaisyNbsnrInitialize(uint64_t paramEa, int mySpuNum, int groupNum);

	/*
	 * return spunum of myself
	 * @retval spunum of myself
	 */
	int cellDaisyNbsnrGetMySpunum(void) ;

	/*
	 * send signal data followed by signal notification
	 * @param data signal data
	 * @param ea destination signal notification effective address
	 * @param tag DMA tag used for signal data transfer and signal notification
	 * @param tid tid used for signal data transfer and signal notification
	 * @param rid rid used for signal data transfer and signal notification
	 */
	void cellDaisyNbsnrSndsigf(uint32_t data, uint64_t ea, int tag, int tid, int rid);
  
	/*
	 * return Nonblock Signal1 value if it exists
	 * @retval signal data
	 */
	uint32_t cellDaisyNbsnrReadSignal1IfExists(void);

	/*
	 * return Nonblock Signal2 value if it exists
	 * @retval signal data
	 */
	uint32_t cellDaisyNbsnrReadSignal2IfExists(void);

#ifdef __cplusplus
};
#endif

#endif /*__CELL_DAYSY_NBSNR_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
