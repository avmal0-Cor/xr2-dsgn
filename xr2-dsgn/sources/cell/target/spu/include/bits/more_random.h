/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _MORE_RANDOM_H_
#define _MORE_RANDOM_H_
#ifndef _YVALS
#include <yvals.h>
#endif
#include <spu_intrinsics.h>

_C_STD_BEGIN
_C_LIB_DECL

// Prototypes for the SCEA random number functions

// These return single numbers in various ranges
_INLINE_DEF int                 get_state_MT(void **state);     /* Get address and size of state vector */
_INLINE_DEF void                init_MT(unsigned int s);        /* Initialize with a single seed value */
_INLINE_DEF void                init_by_array_MT(unsigned int init_key[], int key_length);  /* Initialize using an array */
_INLINE_DEF unsigned int        rand_int32_MT(void);            /* Generate 32 bit random int (most efficient) */
_INLINE_DEF unsigned int        rand_int31_MT(void);            /* Generate 31 bit random int */
_INLINE_DEF float               rand_real1_MT(void);            /* Generate a random number on [0,1]-real-interval */
_INLINE_DEF float               rand_real2_MT(void);            /* Generate a random number on [0,1)-real-interval */
_INLINE_DEF float               rand_real3_MT(void);            /* Generate a random number on (0,1)-real-interval */
// Vector versions return the next 4 numbers in the sequence as a vector
_INLINE_DEF vector unsigned int rand_int32_MT_i4(void);         /* Generate 32 bit random int (most efficient) */
_INLINE_DEF vector unsigned int rand_int31_MT_i4(void);         /* Generate 31 bit random int */
_INLINE_DEF vector float        rand_real1_MT_f4(void);         /* Generate a random number on [0,1]-real-interval */
_INLINE_DEF vector float        rand_real2_MT_f4(void);         /* Generate a random number on [0,1)-real-interval */
_INLINE_DEF vector float        rand_real3_MT_f4(void);         /* Generate a random number on (0,1)-real-interval */

_END_C_LIB_DECL

#ifndef _NO_INLINE_DEFINITIONS
/* Here we include the inline definitions of all the implementations. */
#include <bits/spec.h>
#include <bits/mt19937.h>
#endif

_C_STD_END

#endif // _MORE_RANDOM_H_

#if defined(_STD_USING)

using _CSTD get_state_MT;
using _CSTD init_MT;
using _CSTD init_by_array_MT;
using _CSTD rand_int32_MT;
using _CSTD rand_int31_MT;
using _CSTD rand_real1_MT;
using _CSTD rand_real2_MT;
using _CSTD rand_real3_MT;
using _CSTD rand_int32_MT_i4;
using _CSTD rand_int31_MT_i4;
using _CSTD rand_real1_MT_f4;
using _CSTD rand_real2_MT_f4;
using _CSTD rand_real3_MT_f4;

#endif /* defined (_STD_USING) */

