/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __FIBER_PPU_H__
#define __FIBER_PPU_H__	1

/* The C Standard Library */
#include <stdint.h>

/* SPU C/C++ Language Extension */
#include <spu_intrinsics.h>

/* fiber types */
#include <cell/fiber/error.h>
#include <cell/fiber/ppu_fiber_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellFiberPpuSendSignal(uint32_t eaFiber, unsigned int* numWorker);
int cellFiberPpuGetScheduler(uint32_t eaFiber, uint32_t* pEaScheduler);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* FIBER_PPU_H */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
