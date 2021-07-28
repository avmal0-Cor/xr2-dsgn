/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/





#ifndef __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__
#define __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__

/**
 *  @addtogroup SHEAP SHEAP
 */
/**
 *  @addtogroup PU PU
 *  @ingroup SHEAP 
 */
/**
 *  @addtogroup CellKeySheapSemaphore
 *  @ingroup PU 
 */
/*@{*/

#include <stdint.h>
#include <cell/sync.h>
#include <cell/sheap/key_sheap.h>
#include <cell/atomic.h>
#include <cell/sheap/error.h>
#include <spu_mfcio.h>
#include <cell/sheap/sheap_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int 
cellKeySheapSemaphoreNew(CellKeySheapSemaphore*, uint64_t ksheap, CellSheapKey key,
						 int count);

void
cellKeySheapSemaphoreDelete(CellKeySheapSemaphore* obj);

#ifdef __cplusplus
}
#endif


#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline int
cellKeySheapSemaphoreTryP(CellKeySheapSemaphore* obj)
{	
	char work_area[256];
	volatile uint32_t* p = (volatile uint32_t*)(((uintptr_t)work_area + 127) & ~127);
	volatile int *int_p = (volatile int*)p;

	int old;

	do {
		mfc_getllar(p, obj->ea, 0, 0);
		mfc_read_atomic_status();

		if(*int_p <= 0){
			mfc_putllc(p, obj->ea, 0, 0);
			mfc_read_atomic_status();
			return CELL_SYNC_ERROR_BUSY;
		}
		old = *p;
		*int_p  =  old - 1;

		spu_dsync();
		mfc_putllc(p, obj->ea, 0, 0);
	} while (__builtin_expect(mfc_read_atomic_status(), 0));

	return CELL_OK;

}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapSemaphoreP(CellKeySheapSemaphore* obj)
{
	char work_area[256];
	volatile uint32_t* p = (volatile uint32_t*)(((uintptr_t)work_area + 127) & ~127);
	volatile int *int_p = (volatile int*)p;

	int old;

	do {
		
		do {
			mfc_getllar(p, obj->ea, 0, 0);
			mfc_read_atomic_status();
		}while(__builtin_expect(*int_p <= 0,0));

		old = *p;
		*int_p  =  old - 1;

		spu_dsync();
		mfc_putllc(p, obj->ea, 0, 0);
	} while (__builtin_expect(mfc_read_atomic_status(), 0));

	return;
}

#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
static inline void
cellKeySheapSemaphoreV(CellKeySheapSemaphore* obj)
{
	char work_area[256];
	uint32_t* p = (uint32_t*)(((uintptr_t)work_area + 127)& ~127);

	cellAtomicIncr32(p,obj->ea);
	return;
}




#endif /* __CELL_SHEAP_KEY_SHEAP_SEMAPHORE_H__ */
