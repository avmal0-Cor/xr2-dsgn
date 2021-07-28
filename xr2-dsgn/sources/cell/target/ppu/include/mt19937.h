/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _MT19937_H__
#define _MT19937_H__
#ifndef _YVALS
#include <yvals.h>
#endif

_C_STD_BEGIN
_C_LIB_DECL

// Prototypes for the SCEA random number functions

// These return single numbers in various ranges
int                 get_state_MT(void **state);     /* Get address and size of state vector */
void                init_MT(unsigned int s);        /* Initialize with a single seed value */
void                init_by_array_MT(unsigned int init_key[], int key_length);  /* Initialize using an array */
unsigned int        rand_int32_MT(void);            /* Generate 32 bit random int (most efficient) */
unsigned int        rand_int31_MT(void);            /* Generate 31 bit random int */
float               rand_real1_MT(void);            /* Generate a random number on [0,1]-real-interval */
float               rand_real2_MT(void);            /* Generate a random number on [0,1)-real-interval */
float               rand_real3_MT(void);            /* Generate a random number on (0,1)-real-interval */

#ifndef _NO_INLINE_DEFINITIONS
/* Here we include the inline definitions of all the implementations. */
#include <bits/spec.h>
#include <bits/mt19937_impl.h>
#endif

_END_C_LIB_DECL

_C_STD_END
#endif // _MT19937_H__

#if defined(_STD_USING)

using _CSTD get_state_MT;
using _CSTD init_MT;
using _CSTD init_by_array_MT;
using _CSTD rand_int32_MT;
using _CSTD rand_int31_MT;
using _CSTD rand_real1_MT;
using _CSTD rand_real2_MT;
using _CSTD rand_real3_MT;
#endif /* defined (_STD_USING) */

