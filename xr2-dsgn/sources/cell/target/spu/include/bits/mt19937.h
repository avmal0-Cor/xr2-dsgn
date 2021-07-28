/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#include <bits/mt19937_globals.h>

#if 0
/* This are only used in comments now. */
#define _PERIOD_N 624
#define _PERIOD_M 397
#define _MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define _UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define _LOWER_MASK 0x7fffffffUL /* least significant r bits */
#endif

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

  _C_LIB_DECL
  extern  __MT19937_State_t __MT19937;
  _END_C_LIB_DECL

_FUNC_DEF(int, get_state_MT,(void **state))
{

  *state = (void *)&__MT19937;
  return(sizeof(__MT19937));
}

/* initializes __MT19937.State[_PERIOD_N] with a seed */
_FUNC_DEF(void, init_MT,(unsigned int s))
{

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

  unsigned int y;

  if (__MT19937.Next_Number >= _PERIOD_N)
  { /* generate _PERIOD_N words at one time */
    if (__MT19937.Next_Number == _PERIOD_N+1)   /* if init_rand() has not been called, */
      init_MT(5489UL); /* a default initial seed is used */

    const vector unsigned int upper_mask = {0x80000000,0x80000000,0x80000000,0x80000000};
    const vector unsigned int lower_mask = {0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff};
    const vector unsigned int matrix_a   = {0x9908b0df,0x9908b0df,0x9908b0df,0x9908b0df};
    const vector unsigned char shuffle1  = {0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13}; // offset by 1 word
    const vector unsigned char shuffle2  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x11, 0x12, 0x13}; // 3 ints from first qword, 1 int from second
    const vector unsigned int zero       = {0,0,0,0};
    vector unsigned int a,b,*output,yy,shifted_y,magic;
    int words;

    // code below does this
    //for (kk=0;kk<_PERIOD_N-_PERIOD_M;kk++)       // loop kk < 227   56 qwords + 3 words
    //{
      // y = (__MT19937.State[kk]&_UPPER_MASK)|(__MT19937.State[kk+1]&_LOWER_MASK);
      // __MT19937.State[kk] = __MT19937.State[kk+_PERIOD_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    //}
      
    for(output = (vector unsigned int*)__MT19937.State, words=56; words != 0; words--)
    {
      a = spu_shuffle(*output, *(output+1), shuffle1);  // unaligned load of kk +1 int
      yy = spu_or(spu_and(*output,upper_mask),spu_and(a, lower_mask));
      shifted_y = spu_rlmask(yy,-1);
      magic     = spu_gather(yy);
// !!!GAC This cast should not be necessary (spu_maskw breaks C++ if it isn't)
      magic     = spu_maskw((unsigned int)si_to_uint((qword)magic));
      magic     = spu_sel(zero,matrix_a,magic);
      b = spu_shuffle(*(output+99), *(output+100), shuffle1);  // kk + 397 words      
      *output = spu_xor(spu_xor(b,shifted_y),magic);
      output++;
    }
    
    // Code below does this...
    // y = (__MT19937.State[224]&_UPPER_MASK)|(__MT19937.State[224+1]&_LOWER_MASK);
    // __MT19937.State[224] = __MT19937.State[224+_PERIOD_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[225]&_UPPER_MASK)|(__MT19937.State[225+1]&_LOWER_MASK);
    // __MT19937.State[225] = __MT19937.State[225+_PERIOD_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[226]&_UPPER_MASK)|(__MT19937.State[226+1]&_LOWER_MASK);
    // __MT19937.State[226] = __MT19937.State[226+_PERIOD_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[227]&_UPPER_MASK)|(__MT19937.State[227+1]&_LOWER_MASK);
    // __MT19937.State[227] = __MT19937.State[0] ^ (y >> 1) ^ mag01[y & 0x1UL];
    
    a = spu_shuffle(*output, *(output+1), shuffle1);  // unaligned load of kk +1 int
    yy = spu_or(spu_and(*output,upper_mask),spu_and(a, lower_mask));
    shifted_y = spu_rlmask(yy,-1);
    magic     = spu_gather(yy);
// !!!GAC This cast should not be necessary (spu_maskw breaks C++ if it isn't)
    magic     = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic     = spu_sel(zero,matrix_a,magic);
    b = spu_shuffle(*(output+99), *(output+100), shuffle1);
    b = spu_shuffle(b,*(output-56),shuffle2); 
    *output = spu_xor(spu_xor(b,shifted_y),magic);
    output++;
    
    // Code below does this...
    //for (kk=228;kk<_PERIOD_N-1;kk++)          // loop kk < 623
    //{
    //  y = (__MT19937.State[kk]&_UPPER_MASK)|(__MT19937.State[kk+1]&_LOWER_MASK);
    //  __MT19937.State[kk] = __MT19937.State[kk+(_PERIOD_M-_PERIOD_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    //}
    
    for( words=98; words != 0; words--)
    {
      a = spu_shuffle(*output, *(output+1), shuffle1);  // unaligned load of kk +1 int
      yy = spu_or(spu_and(*output,upper_mask),spu_and(a, lower_mask));
      shifted_y = spu_rlmask(yy,-1);
      magic     = spu_gather(yy);
// !!!GAC This cast should not be necessary (spu_maskw breaks C++ if it isn't)
      magic     = spu_maskw((unsigned int)si_to_uint((qword)magic));
      magic     = spu_sel(zero,matrix_a,magic);
      b = spu_shuffle(*(output-57), *(output-56), shuffle1);  // kk + 397 words      
      *output = spu_xor(spu_xor(b,shifted_y),magic);
      output++;
    }
    
    // Code below does this...
    // y = (__MT19937.State[620]&_UPPER_MASK)|(__MT19937.State[620+1]&_LOWER_MASK);
    // __MT19937.State[620] = __MT19937.State[620+(_PERIOD_M-_PERIOD_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[621]&_UPPER_MASK)|(__MT19937.State[621+1]&_LOWER_MASK);
    // __MT19937.State[621] = __MT19937.State[621+(_PERIOD_M-_PERIOD_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[622]&_UPPER_MASK)|(__MT19937.State[622+1]&_LOWER_MASK);
    // __MT19937.State[622] = __MT19937.State[622+(_PERIOD_M-_PERIOD_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    // y = (__MT19937.State[623]&_UPPER_MASK)|(__MT19937.State[0]&_LOWER_MASK);
    // __MT19937.State[623] = __MT19937.State[396] ^ (y >> 1) ^ mag01[y & 0x1UL];
    
    a = spu_shuffle(*output, *(output+1), shuffle1);  // unaligned load of kk +1 int
    a = spu_shuffle(a, *(output-155), shuffle2);      // fold in byte 0
    yy = spu_or(spu_and(*output,upper_mask),spu_and(a, lower_mask));
    shifted_y = spu_rlmask(yy,-1);
    magic     = spu_gather(yy);
// !!!GAC This cast should not be necessary (spu_maskw breaks C++ if it isn't)
    magic     = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic     = spu_sel(zero,matrix_a,magic);
    b = spu_shuffle(*(output-57), *(output-56), shuffle1);
    b = spu_shuffle(b,*(output-56),shuffle2); 
    *output = spu_xor(spu_xor(b,shifted_y),magic);
    
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

// These are fat and inefficient for now, they will be replaced in the next release

/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(vector unsigned int, rand_int32_MT_i4, (void))
{
  vector unsigned int temp;
  temp = spu_promote(rand_int32_MT(), 0);
  temp = spu_insert(rand_int32_MT(), temp, 1);
  temp = spu_insert(rand_int32_MT(), temp, 2);
  temp = spu_insert(rand_int32_MT(), temp, 3);
  return (temp);
}

/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(vector unsigned int, rand_int31_MT_i4, (void))
{
  vector unsigned int temp;
  temp = spu_promote(rand_int31_MT(), 0);
  temp = spu_insert(rand_int31_MT(), temp, 1);
  temp = spu_insert(rand_int31_MT(), temp, 2);
  temp = spu_insert(rand_int31_MT(), temp, 3);
  return (temp);
}

/* generates a random number on [0,1]-real-interval */
_FUNC_DEF(vector float, rand_real1_MT_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real1_MT(), 0);
  temp = spu_insert(rand_real1_MT(), temp, 1);
  temp = spu_insert(rand_real1_MT(), temp, 2);
  temp = spu_insert(rand_real1_MT(), temp, 3);
  return (temp); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
_FUNC_DEF(vector float, rand_real2_MT_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real2_MT(), 0);
  temp = spu_insert(rand_real2_MT(), temp, 1);
  temp = spu_insert(rand_real2_MT(), temp, 2);
  temp = spu_insert(rand_real2_MT(), temp, 3);
  return (temp); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
_FUNC_DEF(vector float, rand_real3_MT_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real3_MT(), 0);
  temp = spu_insert(rand_real3_MT(), temp, 1);
  temp = spu_insert(rand_real3_MT(), temp, 2);
  temp = spu_insert(rand_real3_MT(), temp, 3);
  return (temp); 
    /* divided by 2^32 */
}


