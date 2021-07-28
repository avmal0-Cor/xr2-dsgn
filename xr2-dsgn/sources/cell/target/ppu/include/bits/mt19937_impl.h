/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
/*
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 */
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_rand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/


#include <bits/mt19937_globals.h>

//#define _PERIOD_N 624
#define _PERIOD_M 397
#define _MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define _UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define _LOWER_MASK 0x7fffffffUL /* least significant r bits */

// Returns the address and size (in bytes) of the MT19937 state vector.
// This lets you to save and restore the state of the random number generator
// when code is swapped on or off of the SPU.  This allows you to unload an SPU applet,
// load it back up later and have it continue from where it left off in the
// random number sequence, rather than having it start from the beginning.
// This is a better, faster and more statistically correct than reseeding the random
// number generator each time the SPU applet is reloaded.
// 
// To save the state, call this function to get the address and size of the state
// vector then memcopy or DMA that data to some safe location.  To restore it, do 
// do the same thing but copy the data into the address provided.
//
// DO NOT use this to seed the generator as you could put in values would cause the
// generator output to be non-random or degenerate to zero.  Always use the init_
// functions to seed the generator, they protect against bad seed values.
//
// The internal format of this data may change so avoid putting it in long-term
// storage or making assumptions about it's size/format till the library is finalized.
//

_FUNC_DEF(int, get_state_MT,(void **state))
{
  extern __MT19937_State_t __MT19937;

  *state = (void *)&__MT19937;
  return(sizeof(__MT19937));
}

/* initializes __MT19937.State[_PERIOD_N] with a seed */
_FUNC_DEF(void, init_MT,(unsigned int s))
{
  extern __MT19937_State_t __MT19937;

    __MT19937.State[0]= s & 0xffffffffUL;
    for (__MT19937.Next_Number=1; __MT19937.Next_Number<_PERIOD_N; __MT19937.Next_Number++) {
        __MT19937.State[__MT19937.Next_Number] = 
	    (1812433253UL * (__MT19937.State[__MT19937.Next_Number-1] ^ (__MT19937.State[__MT19937.Next_Number-1] >> 30)) + __MT19937.Next_Number); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array __MT19937.State[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        __MT19937.State[__MT19937.Next_Number] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
_FUNC_DEF(void, init_by_array_MT,(unsigned int init_key[], int key_length))
{
  extern __MT19937_State_t __MT19937;
    int i, j, k;
    init_MT(19650218UL);
    i=1; j=0;
    k = (_PERIOD_N>key_length ? _PERIOD_N : key_length);
    for (; k; k--) {
        __MT19937.State[i] = (__MT19937.State[i] ^ ((__MT19937.State[i-1] ^ (__MT19937.State[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        __MT19937.State[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=_PERIOD_N) { __MT19937.State[0] = __MT19937.State[_PERIOD_N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=_PERIOD_N-1; k; k--) {
        __MT19937.State[i] = (__MT19937.State[i] ^ ((__MT19937.State[i-1] ^ (__MT19937.State[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        __MT19937.State[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=_PERIOD_N) { __MT19937.State[0] = __MT19937.State[_PERIOD_N-1]; i=1; }
    }

    __MT19937.State[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
_FUNC_DEF(unsigned int, rand_int32_MT,(void))
{
  extern __MT19937_State_t __MT19937;
  unsigned long y;
  static unsigned long mag01[2]={0x0UL, _MATRIX_A};
  /* mag01[x] = x * _MATRIX_A  for x=0,1 */

  if (__MT19937.Next_Number >= _PERIOD_N)
  { /* generate N words at one time */
    int kk;

    if (__MT19937.Next_Number == _PERIOD_N+1)   /* if init_genrand() has not been called, */
      init_MT(5489UL); /* a default initial seed is used */

    for (kk=0;kk<_PERIOD_N-_PERIOD_M;kk++)
    {
      y = (__MT19937.State[kk]&_UPPER_MASK)|(__MT19937.State[kk+1]&_LOWER_MASK);
      __MT19937.State[kk] = __MT19937.State[kk+_PERIOD_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (;kk<_PERIOD_N-1;kk++)
    {
      y = (__MT19937.State[kk]&_UPPER_MASK)|(__MT19937.State[kk+1]&_LOWER_MASK);
      __MT19937.State[kk] = __MT19937.State[kk+(_PERIOD_M-_PERIOD_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (__MT19937.State[_PERIOD_N-1]&_UPPER_MASK)|(__MT19937.State[0]&_LOWER_MASK);
    __MT19937.State[_PERIOD_N-1] = __MT19937.State[_PERIOD_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    __MT19937.Next_Number = 0;
  }

  y = __MT19937.State[__MT19937.Next_Number++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}
/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(unsigned int, rand_int31_MT, (void))
{
    return (long)(rand_int32_MT()>>1);
}

/* generates a random number on [0,1]-real-interval */
_FUNC_DEF(float, rand_real1_MT, (void))
{
    return rand_int32_MT()*(1.0f/4294967295.0f); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
_FUNC_DEF(float, rand_real2_MT, (void))
{
    return rand_int32_MT()*(1.0f/4294967296.0f); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
_FUNC_DEF(float, rand_real3_MT, (void))
{
    return (((float)rand_int32_MT()) + 0.5f)*(1.0f/4294967296.0f); 
    /* divided by 2^32 */
}
#if 0
/* generates a random number on [0,1) with 53-bit resolution*/
double rand_res53_MT(void) 
{ 
    unsigned long a=rand_int32_MT()>>5, b=rand_int32_MT()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
}
#endif
/* These real versions are due to Isaku Wada, 2002/01/09 added */

