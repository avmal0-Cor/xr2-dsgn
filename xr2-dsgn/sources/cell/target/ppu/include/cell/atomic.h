/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_ATOMIC_H__
#define __CELL_ATOMIC_H__ 1

#include <stdint.h>
#include <stdbool.h>
#include <ppu_intrinsics.h>

#define cellAtomicLockLine32(ea)			__lwarx(ea)
#define cellAtomicStoreConditional32(ea,v)	(0 == __stwcx(ea,v))

static inline
uint32_t cellAtomicAdd32(uint32_t *ea, uint32_t value)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old + value));
	return old;
}

static inline
uint32_t cellAtomicSub32(uint32_t *ea, uint32_t value)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old - value));
	return old;
}

static inline
uint32_t cellAtomicAnd32(uint32_t *ea, uint32_t value)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old & value));
	return old;
}

static inline
uint32_t cellAtomicOr32(uint32_t *ea, uint32_t value)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old | value));
	return old;
}

static inline
uint32_t cellAtomicStore32(uint32_t *ea, uint32_t value)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, value));
	return old;
}

static inline
uint32_t cellAtomicIncr32(uint32_t *ea)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old + 1));
	return old;
}

static inline
uint32_t cellAtomicDecr32(uint32_t *ea)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old - 1));
	return old;
}

static inline
uint32_t cellAtomicNop32(uint32_t *ea)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
	} while (0 == __stwcx(ea, old));
	return old;
}

static inline
uint32_t cellAtomicTestAndDecr32(uint32_t *ea)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
		if (old == 0) {
			break;
		}
	} while (0 == __stwcx(ea, old - 1));
	return old;
}

static inline
uint32_t cellAtomicCompareAndSwap32(uint32_t *ea, uint32_t compare, uint32_t swap)
{
	uint32_t old;
	do {
		old = __lwarx(ea);
		if (old != compare) {
			break;
		}
	} while (0 == __stwcx(ea, swap));
	return old;
}

#ifdef _ARCH_PPC64

#define cellAtomicLockLine64(ea)			__ldarx(ea)
#define cellAtomicStoreConditional64(ea,v)	(0 == __stdcx(ea,v))

static inline
uint64_t cellAtomicAdd64(uint64_t *ea, uint64_t value)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old + value));
	return old;
}

static inline
uint64_t cellAtomicSub64(uint64_t *ea, uint64_t value)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old - value));
	return old;
}

static inline
uint64_t cellAtomicAnd64(uint64_t *ea, uint64_t value)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old & value));
	return old;
}

static inline
uint64_t cellAtomicOr64(uint64_t *ea, uint64_t value)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old | value));
	return old;
}

static inline
uint64_t cellAtomicStore64(uint64_t *ea, uint64_t value)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, value));
	return old;
}

static inline
uint64_t cellAtomicIncr64(uint64_t *ea)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old + 1));
	return old;
}

static inline
uint64_t cellAtomicDecr64(uint64_t *ea)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old - 1));
	return old;
}

static inline
uint64_t cellAtomicNop64(uint64_t *ea)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
	} while (0 == __stdcx(ea, old));
	return old;
}

static inline
uint64_t cellAtomicTestAndDecr64(uint64_t *ea)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
		if (old == 0) {
			break;
		}
	} while (0 == __stdcx(ea, old - 1));
	return old;
}

static inline
uint64_t cellAtomicCompareAndSwap64(uint64_t *ea, uint64_t compare, uint64_t swap)
{
	uint64_t old;
	do {
		old = __ldarx(ea);
		if (old != compare) {
			break;
		}
	} while (0 == __stdcx(ea, swap));
	return old;
}

#endif // _ARCH_PPC64

#endif /* __CELL_ATOMIC_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
