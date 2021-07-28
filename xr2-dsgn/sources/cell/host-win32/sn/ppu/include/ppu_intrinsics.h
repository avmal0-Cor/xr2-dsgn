/*  SCE CONFIDENTIAL                          */
/*  PlayStation(R)3 Programmer Tool Runtime Library */
/*  Copyright (C) 2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                        */
#ifndef __PPU_INTRINSICS_SNC_H__
#define __PPU_INTRINSICS_SNC_H__

#include <stdbool.h>

#if !defined(__PPU__) || !defined(__SNC__)
#   error SNC version of ppu_intrinsics.h included on wrong platform/compiler
#endif

#ifndef __SNC_INLINE
#   define __SNC_INLINE static inline __attribute__((always_inline))
#endif

/*
 * __INTRINSICS_CHECKING can be used to enable run-time parameter bounds checking for 
 * some of the intrinsics below. 
 */
#ifndef __ASSERT
#   ifdef __INTRINSICS_CHECKING
#       include <assert.h>
#       define __ASSERT(x)  assert (x)
#   else
#       define __ASSERT(x)
#   endif
#endif

/*
 * Some intrinsics currently implemented in the compiler will be moved to this file in future.
 * Please always include ppu_intrinsics.h when using intrinsics.  This will guarantee compatibility
 * with future releases.
 *
 * For upwards compatibility please avoid using the internal __builtin_* intrinsics directly.
 */

/*
 * JSRE form intrinsics. Beware... parameter order may be unexpected!
 */

/* specify the address and direction of a read data stream. Will continue to load cache blocks starting at address */
__SNC_INLINE void __dcbt_TH1000 (void * __address, bool __direction, bool __unlimited, int __id)
{
    __builtin_dcbt3 ((unsigned int) __address & -128u, 
                     ((unsigned int) __direction & 1) * 64 + ((unsigned int) __unlimited & 1) * 32 + ((unsigned int) __id & 15), 
                     8);
}


/* control a stream started by __dcbt_TH1000. start and stop the stream, specify count and other flags. */
__SNC_INLINE void __dcbt_TH1010 ( bool __g0,
                                  int __stop, 
                                  int __unit_count, 
                                  bool __transient, 
                                  bool __unlimited, 
                                  int __id
                                )
{
    __builtin_dcbt3 ((unsigned int) __g0 << 31 | ( (unsigned int) __stop & 3) << 29 | ( (unsigned int) __unit_count & 1023) << 7, 
                     ((unsigned int) __transient & 1) * 64 + ((unsigned int) __unlimited & 1) * 32 + ((unsigned int) __id & 15), 
                     8);
}

/* read the time base. Skip values with zero lower 32 bits. */
/* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_intrinsics.h when using it for future compatibility. */
/* __SNC_INLINE long long __mftb () { return __builtin_mftb (); } */

/* Invalidate a L2 instruction cache block */
__SNC_INLINE void __icbi (void * __ptr) { __builtin_icbi (__ptr, 0); }

/* Invalidate a L2 data cache block */
__SNC_INLINE void __dcbi (void * __ptr) { __builtin_dcbi (__ptr, 0); }

/* Flush a L2 data cache block */
__SNC_INLINE void __dcbf (void * __ptr) { __builtin_dcbf (__ptr, 0); }

/* Zero a L2 data cache block. */
__SNC_INLINE void __dcbz (void * __ptr) { __builtin_dcbz (__ptr, 0); }

/* Write out a L2 data cache block */
__SNC_INLINE void __dcbst (void * __ptr) { __builtin_dcbst (__ptr, 0); }

/* Read a L2 data cache block ready for a store. */
__SNC_INLINE void __dcbtst (void * __ptr) { __builtin_dcbtst (__ptr, 0); }

/* Read a L2 data cache block, non-streamed form. */
/* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_intrinsics.h when using it for future compatibility. */
/*__SNC_INLINE void __dcbt (void * __ptr) { __builtin_dcbt (__ptr, 0); } */


/* JSRE form memory operations. Beware... parameter order may be unexpected! */
__SNC_INLINE unsigned int       __lwarx (volatile void * __base) { return __builtin_lwarx (__base, 0); }
__SNC_INLINE unsigned long long __ldarx (volatile void * __base) { return __builtin_ldarx (__base, 0); }
__SNC_INLINE bool __stwcx (volatile void * __base, unsigned int       __value) { return __builtin_stwcx (__value, __base, 0); }
__SNC_INLINE bool __stdcx (volatile void * __base, unsigned long long __value) { return __builtin_stdcx (__value, __base, 0); }

__SNC_INLINE unsigned short     __lhbrx (void * __base) { return __builtin_lhbrx (__base, 0); }
__SNC_INLINE unsigned int       __lwbrx (void * __base) { return __builtin_lwbrx (__base, 0); }
__SNC_INLINE unsigned long long __ldbrx (void * __base) { return __builtin_ldbrx (__base, 0); }

__SNC_INLINE void __sthbrx (void * __base, unsigned short     __value) { __builtin_sthbrx (__value, __base, 0); }
__SNC_INLINE void __stwbrx (void * __base, unsigned int       __value) { __builtin_stwbrx (__value, __base, 0); }
__SNC_INLINE void __stdbrx (void * __base, unsigned long long __value) { __builtin_stdbrx (__value, __base, 0); }

/* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_intrinsics.h when using it for future compatibility. */
/*__SNC_INLINE unsigned long long __cntlzd (long long __a) { long long result = __builtin_clzll (__a); return result; } */

/* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_intrinsics.h when using it for future compatibility. */
/*__SNC_INLINE long long __cntlzw (long long __a) { long long result = __builtin_clz (__a); return result; } */

__SNC_INLINE void __sync ()   { __builtin_sync ();   }
__SNC_INLINE void __isync ()  { __builtin_isync ();  }
__SNC_INLINE void __lwsync () { __builtin_lwsync (); }
__SNC_INLINE void __eieio ()  { __builtin_eieio ();  }

/**
 * \brief Convert double to double-word
 *
 * Converts a double value to a double-word integer. This function takes the 
 * current rounding mode into account.
 *
 * \param __a  The value to be converted to a double-word integer.
 * \result  The result of converting 'a' to a double-word integer.
 */
__SNC_INLINE long long __fctid (double __a)
{
    union { long long __out; double __in; } __convert; 
    __convert.__in = __builtin_fctid (__a);
    return __convert.__out;
}

/**
 * \brief Convert double to word with round towards zero
 *
 * Converts a double value to a word. This function always rounds towards zero.
 *
 * \param __a  The value to be converted to a word
 * \result  The result of converting 'a' to a word
 */
__SNC_INLINE int __fctiw (double __a)
{
    union { long long __out; double __in; } __convert;
    __convert.__in = __builtin_fctiw (__a);
    return __convert.__out;
}

/**
 * \brief Convert double to double-word with round towards zero
 * 
 * Converts a double value to a word. This function always rounds towards zero.
 * 
 * \param __a  The value to be converted to a double-word integer
 * \result  The result of converting 'a' to a double-word integer
 */
__SNC_INLINE long long __fctidz (double __a)
{
    union { long long __out; double __in; } __convert;
    __convert.__in = __builtin_fctidz (__a); 
    return __convert.__out;
}

/**
 * \brief Convert double to word with round towards zero
 * 
 * Converts a double to word. This function always rounds towards zero.
 *
 * \param __a  The value to be converted to a word
 * \result  The result of converting 'a' to a word
 */
__SNC_INLINE int __fctiwz (double __a)
{
    union { long long __out; double __in; } __convert;
    __convert.__in = __builtin_fctiwz (__a);
    return __convert.__out;
}

/**
 * \brief Convert double-word to double
 *
 * Converts a double-word value to double-precision floating point.
 *
 * \param __a The value to be converted to floating point
 * \result  The result of converting 'a' to floating point
 */
__SNC_INLINE double __fcfid (long long __a)
{
    union { long long __in; double __out; } __convert; 
    __convert.__in = __a; 
    return __builtin_fcfid (__convert.__out);
}

__SNC_INLINE double __mffs () { return __builtin_mffs (); }
__SNC_INLINE void __mtfsf  (int __mask, double __value) { __builtin_mtfsf  (__mask, __value); }
__SNC_INLINE void __mtfsfi (int __bits, int    __field) { __builtin_mtfsfi (__bits, __field); }

__SNC_INLINE void __mtfsb0 (int __bit) { __builtin_mtfsb0 (__bit); }
__SNC_INLINE void __mtfsb1 (int __bit) { __builtin_mtfsb1 (__bit); }

__SNC_INLINE double __setflm (double __a)
{
    double __tmp = __builtin_mffs ();
    __builtin_mtfsf (255, __a); 
    return __tmp;
}

/*
 * Utility functions. These are support functions for the following intrinsics and are not designed
 * to be used directly.
 */
__SNC_INLINE unsigned long long __mask64 (unsigned int __mb, unsigned int __me) { __ASSERT (__mb <= __me && __me < 64U); return (unsigned long long) -1 << 63U - (__me - __mb) >> __mb; }
__SNC_INLINE unsigned int       __mask32 (unsigned int __mb, unsigned int __me) { __ASSERT (__mb <= __me && __me < 32U); return (unsigned int)       -1 << 31U - (__me - __mb) >> __mb; }

__SNC_INLINE unsigned long long __rotate64 (unsigned long long __a, unsigned int __sh) { __ASSERT (__sh <= 64U); return (__a << __sh) | (__a >> (64U - __sh)); }
__SNC_INLINE unsigned int       __rotate32 (unsigned int       __a, unsigned int __sh) { __ASSERT (__sh <= 32U); return (__a << __sh) | (__a >> (32U - __sh)); }


/******/

/**
 * \brief Rotate left double-word then clear left
 *
 * The value in the argument 'a' is rotated leftwards by the number of bits specified by the 
 * argument 'b'. A mask  is generated having 1-bits from bit 'mb' through bit 63, and 0-bits 
 * elsewhere. The rotated data is ANDed with the generated mask and returned.
 *
 * \param __a   A value that is to be shifted and masked
 * \param __sh  The number of bits by which the value 'a' is shifted
 * \param __me  The mask is generated with 1-bits from bit 0 to the bit given by this parameter
 * \result      The result of shifting and masking the value 'a'
 */
__SNC_INLINE unsigned long long __rldcl (unsigned long long __a, unsigned long long __sh, unsigned int __mb)
{
    unsigned long long __mask = __mask64 (__mb, 63);
    return __rotate64 (__a, (unsigned int) __sh) & __mask;
}

/**
 * \brief Rotate left double-word then clear right
 *
 * The value in the argument 'a' is rotated leftwards by the number of bits specified by the 
 * argument 'sh'. A mask is generated having 1-bits from bit 0 though bit 'me' and 0-bits 
 * elsewhere. The rotated data ANDed with the generated mask is then returned.
 *
 * \param __a   A value that is to be shifted and masked
 * \param __sh  The number of bits by which the value 'a' is shifted
 * \param __me  The mask is generated with 1-bits from bit 0 to the bit given by this parameter
 * \result      The result of shifting and masking the value 'a'
 */
__SNC_INLINE unsigned long long __rldcr (unsigned long long __a, unsigned long long __sh, unsigned int __me)
{
    unsigned long long __mask = __mask64 (0, __me);
    return __rotate64 (__a, (unsigned int) __sh) & __mask;
}

/**
 * \brief Rotate left double-word immediate then clear
 *
 * The value in the argument 'a' is rotated leftwards by the number of bits specified by the 
 * argument 'sh'. A mask is generated having 1-bits from bit 'mb' through bit '63 - sh' and
 * 0-bits elsewhere. The rotated data is ANDed with the generated mask and returned.
 *
 * \param __a
 * \param __sh
 * \param __mb  
 * \result
 */
/* Table 178 */
__SNC_INLINE unsigned long long __rldic (unsigned long long __a, unsigned int __sh, unsigned int __mb) 
{ 
    unsigned long long __mask = __mask64 (__mb, 63 - __sh);
    return __rotate64 (__a, __sh) & __mask;
}

/**
 * \brief Rotate left double-word immediate then clear left
 *
 * The value in the argument 'a' is rotated leftwards by the number of bits specified by the 
 * argument 'sh'. A mask is generated having 1-bits from bit 'mb' through bit 63 and 0-bits 
 * elsewhere. The rotated data is ANDed with the generated mask and returned.
 *
 * \param __a
 * \param __sh
 * \param __mb
 * \result
 */
/* Table 179 */
__SNC_INLINE unsigned long long __rldicl (unsigned long long __a, unsigned int __sh, unsigned int __mb)
{
    unsigned long long __mask = __mask64 (__mb, 63);
    return __rotate64 (__a, __sh) & __mask;
}

/**
 * \brief Rotate Left double-word immediate then clear right
 *
 * The value in the argument 'a' is rotated leftwards by the number of bits specified by the 
 * argument 'sh'. A mask is generated having 1-bits from bit 0 though bit 'me' and 0-bits 
 * elsewhere. The rotated data is ANDed with the generated mask and returned.
 *
 * \param a
 * \param sh
 * \param me
 * \result
 */
/* Table 180 */
__SNC_INLINE unsigned long long __rldicr (unsigned long long __a, unsigned int __sh, unsigned int __me)
{
    unsigned long long __mask = __mask64 (0, __me);
    return __rotate64 (__a, __sh) & __mask;
}

/**
 * \brief Rotate left double-word immediate then mask insert
 * 
 * A mask is generated with 1-bits from bit 'mb' through bit '63 - sh', and 0-bits elsewhere. 
 * The value in 'a' is ANDed with the complement of this mask, zeroing out just the bits 
 * inside the range 'mb' through '63 - sh'. The argument 'b' is rotated left by 'sh' bits and 
 * ANDs the result with the mask, zeroing out all bits outside the range 'mb' through '63 - sh'. 
 * The two masked values are combined together with inclusive OR and returned.
 * 
 * \param __a
 * \param __b
 * \param __sh
 * \param __mb
 * \result
 */
/* Table 181 */
__SNC_INLINE unsigned long long __rldimi (unsigned long long __a, unsigned long long __b, unsigned int __sh, unsigned int __mb)
{
    unsigned long long __mask = __mask64 (__mb, 63 - __sh);
    return (__rotate64 (__b, __sh) & __mask) | (__a & ~__mask);
}

/**
 * \brief Rotate left word immediate then mask insert
 *
 * A mask is generated with 1-bits from bit 'mb' through bit 'me', and 0-bits elsewhere. The 
 * value in 'a' is ANDed with the complement of this mask, zeroing out just the bits inside 
 * the range 'mb' through 'me'. The argument 'b' is rotated left by 'sh' bits and ANDs the 
 * result with the mask, zeroing out all bits outside the range 'mb' through 'me'. The two 
 * masked values are combined together with inclusive OR and returned.
 *
 * \param __a
 * \param __b   
 * \param __sh  The number of bits by which 'b' is rotated
 * \param __mb  Used, along with 'me', to generate the mask value: this is the starting point of
 *              the sequence of 1-bits
 * \param __me  Used, along with 'mb', to generate the mask value: this is the ending point of 
 *              the sequence of 1-bits
 * \result 
 */
/* Table 182 */
__SNC_INLINE unsigned int __rlwimi (unsigned int __a, unsigned int __b, unsigned int __sh, unsigned int __mb, unsigned int __me)
{
    unsigned int __mask = __mask32 (__mb, __me);
    return (__rotate32 (__b, __sh) & __mask) | (__a & ~__mask); 
}

/**
 * \brief Rotate left word then AND with mask
 * 
 * A mask is generated with 1-bits from 'mb' through bit 'me', and 0-bits elsewhere. The value
 * in a is rotated left by sh bits, then ANDed with this mask and returned.
 * 
 * \param __a
 * \param __sh
 * \param __mb  Used, along with 'me', to generate the mask value: this is the starting point of
 *              the sequence of 1-bits
 * \param __me  Used, along with 'mb', to generate the mask value: this is the ending point of 
 *              the sequence of 1-bits
 * \result
 */
/* Table 183 */
__SNC_INLINE unsigned int __rlwinm (unsigned int __a, unsigned int __sh, unsigned int __mb, unsigned int __me)
{
    unsigned int __mask = __mask32 (__mb, __me);
    return __rotate32 (__a, __sh) & __mask;
}

/**
 * \brief Rotate left word then AND with mask
 * 
 * The argument 'a' is rotated left by the argument 'b'. A mask is generated having 1-bits 
 * from bit 'mb' through bit 'me', and 0-bits elsewhere. The rotated data ANDed with the 
 * generated mask is returned.
 * 
 * \param __a
 * \param __sh
 * \param __mb  Used, along with 'me', to generate the mask value: this is the starting point of
 *              the sequence of 1-bits
 * \param __me  Used, along with 'mb', to generate the mask value: this is the ending point of 
 *              the sequence of 1-bits
 * \result
 */
/* Table 184 */
__SNC_INLINE unsigned int __rlwnm (unsigned int __a, unsigned int __sh, unsigned int __mb, unsigned int __me)
{
    unsigned int __mask = __mask32 (__mb, __me);
    return __rotate32 (__a, __sh) & __mask;
}

/* note: to test for infinity, compare with 3.40282347e38f instead */
__SNC_INLINE double __fmul  (double __a, double __b) { double __result = __a * __b; return __result; }
__SNC_INLINE double __fmuls (double __a, double __b) { float  __result = __a * __b; return __result; }

/* it would be more efficient to use the ternary operator a <= 0 ? b : c */
__SNC_INLINE float __fsels (float __a, float __b, float __c) { return __builtin_fsels (__a, __b, __c); }

/* Floating Reciprocal Square Root Estimate Single */
__SNC_INLINE float __frsqrtes (float __a) { return __builtin_frsqrtes(__a); }

/* Floating Reciprocal Square Root Estimate */
__SNC_INLINE double __frsqrte (double __a) { return __builtin_frsqrte (__a); }

#endif /* __PPU_INTRINSICS_SNC_H__ */

/* eof ppu_intrinsics.h */
