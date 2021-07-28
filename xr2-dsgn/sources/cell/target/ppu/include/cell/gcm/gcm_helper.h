/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_HELPER_H__
#define __CELL_GCM_HELPER_H__

#ifdef __SNC__
#pragma diag_suppress=bad_linkage_of_ref_within_inline_function
#endif

static inline float cellGcmSwap16Float32(const float f)
{
    union SwapF32_16
    {
        uint32_t ui;
        float f;
    } v;
    v.f = f;
    v.ui = (v.ui>>16) | (v.ui<<16);
    return v.f;
}

//	32bit signed fixed-point	[1:11:20]
static inline int32_t cellGcmGetFixedSint32(const float f)
{
	return (int32_t)(f * 1048576.0f);	// f * 2^20
}

//	16bit unsigned fixed-point	[0:12:4]
static inline uint16_t cellGcmGetFixedUint16(const float f)
{
	return (uint16_t)(f * 16.0f);		// f * 2^4
}

//	uint32 type memory copy
static inline void *cellGcmMemoryCopyUint32(void *dst, const void *src, size_t n)
{
	uint32_t *d = (uint32_t *)dst;
	const uint32_t *s = (uint32_t *)src;

	while(n > 0){
		*d = CELL_GCM_ENDIAN_SWAP(*s);
		d++;
		s++;
		n-=4;
	}

	return dst;
}

static inline uint32_t cellGcmAlign(uint32_t alignment, uint32_t value)
{
	return (alignment==0? value: (value==0? 0: (((uint32_t)((value - 1) / alignment) + 1) * alignment)));
}

#endif	/* __CELL_GCM_HELPER_H__ */
