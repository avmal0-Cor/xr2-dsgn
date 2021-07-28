/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOB_GUARD_H__
#define __CELL_SPURS_JOB_GUARD_H__

#define CELL_SPURS_JOB_GUARD_ALIGN		128
#define CELL_SPURS_JOB_GUARD_SIZE		128

#ifdef DONT_USE_DEPRECATED_STRUCTURE

typedef struct CellSpursJobGuard {
	unsigned char skip[CELL_SPURS_JOB_GUARD_SIZE];
} __attribute__((aligned(CELL_SPURS_JOB_GUARD_ALIGN))) CellSpursJobGuard;

#else /* #ifdef DONT_USE_DEPRECATED_STRUCTURE */

#include <stdint.h>

// 128 bytes
typedef struct CellSpursJobGuard
{
	/// Job Guard is passed when count reaches zero.
	uint32_t count __attribute__((deprecated));

	uint32_t originalCount __attribute__((deprecated));

	uint64_t eaJobChain __attribute__((deprecated));

	uint8_t un1[3] __attribute__((deprecated));
	uint8_t readyCount __attribute__((deprecated));
	uint32_t un2[3] __attribute__((deprecated));

	uint8_t un3[3] __attribute__((deprecated));
	uint8_t autoReset __attribute__((deprecated));
	uint32_t un4[3] __attribute__((deprecated));

	char unused[CELL_SPURS_JOB_GUARD_SIZE - 16 - 16 - 16] __attribute__((deprecated));
} __attribute__((__aligned__(CELL_SPURS_JOB_GUARD_ALIGN))) CellSpursJobGuard;

#endif /* #ifdef DONT_USE_DEPRECATED_STRUCTURE */

#endif /* __CELL_SPURS_JOB_GUARD_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
