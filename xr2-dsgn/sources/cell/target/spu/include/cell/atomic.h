/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_ATOMIC_H__
#define __CELL_ATOMIC_H__ 1

#include <spu_mfcio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t cellAtomicAdd32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t cellAtomicAdd64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t cellAtomicAnd32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t cellAtomicAnd64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t cellAtomicCompareAndSwap32(uint32_t *ls, uint64_t ea, uint32_t compare, uint32_t value);
uint64_t cellAtomicCompareAndSwap64(uint64_t *ls, uint64_t ea, uint64_t compare, uint64_t value);
uint32_t cellAtomicDecr32(uint32_t *ls, uint64_t ea);
uint64_t cellAtomicDecr64(uint64_t *ls, uint64_t ea);
uint32_t cellAtomicIncr32(uint32_t *ls, uint64_t ea);
uint64_t cellAtomicIncr64(uint64_t *ls, uint64_t ea);
uint32_t cellAtomicNop32(uint32_t *ls, uint64_t ea);
uint64_t cellAtomicNop64(uint64_t *ls, uint64_t ea);
uint32_t cellAtomicOr32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t cellAtomicOr64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t cellAtomicStore32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t cellAtomicStore64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t cellAtomicSub32(uint32_t *ls, uint64_t ea, uint32_t value);
uint64_t cellAtomicSub64(uint64_t *ls, uint64_t ea, uint64_t value);
uint32_t cellAtomicTestAndDecr32(uint32_t *ls, uint64_t ea);
uint64_t cellAtomicTestAndDecr64(uint64_t *ls, uint64_t ea);

__attribute__((always_inline))
static inline
uint32_t cellAtomicLockLine32(uint32_t *ls, uint64_t ea)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 2;

	spu_hcmpeq(((uintptr_t)ls & 0x7f) == 0, 0);
	ea &= ~0x7f;
	mfc_getllar(ls, ea, 0, 0);
	mfc_read_atomic_status();
	spu_dsync();
	return ls[i];
}

__attribute__((always_inline))
static inline
uint64_t cellAtomicLockLine64(uint64_t *ls, uint64_t ea)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 3;

	spu_hcmpeq(((uintptr_t)ls & 0x7f) == 0, 0);
	ea &= ~0x7f;
	mfc_getllar(ls, ea, 0, 0);
	mfc_read_atomic_status();
	spu_dsync();
	return ls[i];
}

__attribute__((always_inline))
static inline
int cellAtomicStoreConditional32(uint32_t *ls, uint64_t ea, uint32_t value)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 2;

	spu_hcmpeq(((uintptr_t)ls & 0x7f) == 0, 0);
	ls[i] = value;
	ea &= ~0x7f;
	spu_dsync();
	mfc_putllc(ls, ea, 0, 0);
	return mfc_read_atomic_status();
}

__attribute__((always_inline))
static inline
int cellAtomicStoreConditional64(uint64_t *ls, uint64_t ea, uint64_t value)
{
	unsigned int i = ((uint32_t)ea & 0x7f) >> 3;

	spu_hcmpeq(((uintptr_t)ls & 0x7f) == 0, 0);
	ls[i] = value;
	ea &= ~0x7f;
	spu_dsync();
	mfc_putllc(ls, ea, 0, 0);
	return mfc_read_atomic_status();
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_ATOMIC_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
