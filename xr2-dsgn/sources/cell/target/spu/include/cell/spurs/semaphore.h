/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_SEMAPHORE_H__
#define __CELL_SPURS_SEMAPHORE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/semaphore_types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int _cellSpursSemaphoreInitialize(uint64_t ea, int total, unsigned isIWL);
int cellSpursSemaphoreP(uint64_t ea);
int cellSpursSemaphoreV(uint64_t ea);
int cellSpursSemaphoreGetTasksetAddress(uint64_t ea, uint64_t *pEaTaskset);

#ifdef __cplusplus
} /* extern "C" */
#endif

#define cellSpursSemaphoreInitialize(ea,total) \
		_cellSpursSemaphoreInitialize((ea),(total),0)
#define cellSpursSemaphoreInitializeIWL(ea,total) \
		_cellSpursSemaphoreInitialize((ea),(total),1)

#endif /* __CELL_SPURS_SEMAPHORE_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
