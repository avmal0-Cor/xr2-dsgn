/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _TT800_H
#define _TT800_H

/* The algorithm is taken from the following: */
/* A C-program for TT800 : July 8th 1996 Version */
/* by M. Matsumoto, email: matumoto@math.keio.ac.jp */
/* genrand() generate one pseudorandom number with double precision */
/* which is uniformly distributed on [0,1]-interval */
/* for each call.  One may choose any initial 25 seeds */
/* except all zeros. */

/* See: ACM Transactions on Modelling and Computer Simulation, */
/* Vol. 4, No. 3, 1994, pages 254-266. */

#include <bits/tt800_globals.h>

// Returns the address and size (in bytes) of the TT800 state vector.
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
  extern __TT800_State_t __TT800;
  _END_C_LIB_DECL

_FUNC_DEF(int, get_state_TT800,(void **state))
{
  *state = (void *)&__TT800;
  return(sizeof(__TT800));
}

/* initializes TT800 a seed (Needs to be checked) */
// !!!GAC THIS INITIALIZER MAY NOT BE STATISTICALLY SOUND, it didn't come with the generator
_FUNC_DEF(void, init_TT800,(unsigned int s))
{
  int i;
  unsigned int *p = (unsigned int *)__TT800.State;
  for(i=0; i < _N_TT800; i++)
  {
    *(p++) ^= s;
  }
  __TT800.Next_Number = 0;
}

/* initializes TT800 an array (Needs to be checked) */
// !!!GAC THIS INITIALIZER MAY NOT BE STATISTICALLY SOUND, it didn't come with the generator
_FUNC_DEF(void, init_by_array_TT800,(unsigned int init_key[], int key_length))
{
  int i,j;
  unsigned int *p = (unsigned int *)__TT800.State;
  for(i=0,j=0; i < _N_TT800; i++, j++)
  {
    if(j > key_length) j=0;
    *(p++) ^= init_key[j];
  }
  __TT800.Next_Number = 0;
}

_FUNC_DEF(unsigned int, rand_int32_TT800,(void))
{

  unsigned int y;     // Must be 32 bits
  if (__TT800.Next_Number==_N_TT800)
  { /* generate N words at one time */

    const vector unsigned char shuffle1 = {0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b};
    const vector unsigned int magic_vector = {0x8ebfd028, 0x8ebfd028, 0x8ebfd028, 0x8ebfd028};
    const vector unsigned int zero = {0,0,0,0};
    vector unsigned int *input = &__TT800.State[1];
    vector unsigned int *output = __TT800.State;
    vector unsigned int magic, shifted, source;

    // Does this:
    // x[0] = x[7]  ^ (x[0] >> 1) ^ mag01[x[0] & 1];
    // x[1] = x[8]  ^ (x[1] >> 1) ^ mag01[x[1] & 1];
    // x[2] = x[9]  ^ (x[2] >> 1) ^ mag01[x[2] & 1];
    // x[3] = x[10] ^ (x[3] >> 1) ^ mag01[x[3] & 1];


    source  = spu_shuffle(*input, *(input+1), shuffle1);
    shifted = spu_rlmask(*output,-1);
    magic   = spu_gather(*output);
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // x[4] = x[11]  ^ (x[4] >> 1) ^ mag01[x[4] & 1];
    // x[5] = x[12]  ^ (x[5] >> 1) ^ mag01[x[5] & 1];
    // x[6] = x[13]  ^ (x[6] >> 1) ^ mag01[x[6] & 1];
    // x[7] = x[14]  ^ (x[7] >> 1) ^ mag01[x[7] & 1];

    input++;
    output++;
    source  = spu_shuffle(*(input), *(input+1), shuffle1);
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // x[8] = x[15]  ^ (x[8] >> 1) ^ mag01[x[8] & 1];
    // x[9] = x[16]  ^ (x[9] >> 1) ^ mag01[x[9] & 1];
    // x[10] = x[17] ^ (x[10] >> 1) ^ mag01[x[10] & 1];
    // x[11] = x[18] ^ (x[11] >> 1) ^ mag01[x[11] & 1];

    input++;
    output++;
    source  = spu_shuffle(*(input), *(input+1), shuffle1);
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // x[12] = x[19]  ^ (x[12] >> 1) ^ mag01[x[12] & 1];
    // x[13] = x[20]  ^ (x[13] >> 1) ^ mag01[x[13] & 1];
    // x[14] = x[21]  ^ (x[14] >> 1) ^ mag01[x[14] & 1];
    // x[15] = x[22] ^ (x[15] >> 1) ^ mag01[x[15] & 1];

    input++;
    output++;
    source  = spu_shuffle(*(input), *(input+1), shuffle1);
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // x[16] = x[23]  ^ (x[16] >> 1) ^ mag01[x[16] & 1];
    // x[17] = x[24]  ^ (x[17] >> 1) ^ mag01[x[17] & 1];
    // x[18] = x[0]   ^ (x[18] >> 1) ^ mag01[x[18] & 1];
    // x[19] = x[1]   ^ (x[19] >> 1) ^ mag01[x[19] & 1];

    vector unsigned char shuffle2 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

    input++;
    output++;
    source  = spu_shuffle(*(input), *(input+1), shuffle1);  // gets x[23] and x[24]
    input   = ((vector unsigned int *)__TT800.State);                     // reset input to 0
    source  = spu_shuffle(source,*input,shuffle2);          // put x[0] and x[1] in last 2 slots
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // x[20] = x[2] ^ (x[20] >> 1) ^ mag01[x[20] & 1];
    // x[21] = x[3] ^ (x[21] >> 1) ^ mag01[x[21] & 1];
    // x[22] = x[4] ^ (x[22] >> 1) ^ mag01[x[22] & 1];
    // x[23] = x[5] ^ (x[23] >> 1) ^ mag01[x[23] & 1];

    vector unsigned char shuffle3 = {0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

    output++;
    source  = spu_shuffle(*(input), *(input+1), shuffle3);
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_xor(spu_xor(source,shifted), magic);

    // This one only does slot 6
    // x[24] = x[6] ^ (x[24] >> 1) ^ mag01[x[24] & 1];

    vector unsigned int prefmask = {0xffffffff,0,0,0};

    input++;
    output++;
    source  = spu_rlqwbyte(*input,8);                  // Align slot 6 with preferred position
    shifted = spu_rlmask(*(output),-1);
    magic   = spu_gather(*(output));
//!!!GAC This cast should not be necessary (spu_maskw breaks c++ if it isn't here)
    magic   = spu_maskw((unsigned int)si_to_uint((qword)magic));
    magic   = spu_sel(zero,magic_vector,magic);
    *output = spu_sel(*output, spu_xor(spu_xor(source,shifted), magic), prefmask); // spu_sel added so we only write back preferred position

    __TT800.Next_Number=0;
  }
  y = si_to_uint((qword)spu_rlqwbyte(__TT800.State[__TT800.Next_Number>>2], (__TT800.Next_Number&3)*4));
  y ^= (y << 7) & 0x2b5b2500; /* s and b, magic vectors */
  y ^= (y << 15) & 0xdb8b0000; /* t and c, magic vectors */

//  y &= 0xffffffff; /* you may delete this line if word size = 32 */
/* 
   the following line was added by Makoto Matsumoto in the 1996 version
   to improve lower bit's corellation.
   Delete this line to o use the code published in 1994.
*/
  y ^= (y >> 16); /* added to the 1994 version */
  __TT800.Next_Number++;
  return( y );
}
/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(unsigned int, rand_int31_TT800, (void))
{
    return (long)(rand_int32_TT800()>>1);
}

/* generates a random number on [0,1]-real-interval */
_FUNC_DEF(float, rand_real1_TT800, (void))
{
    return rand_int32_TT800()*(1.0f/4294967295.0f); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
_FUNC_DEF(float, rand_real2_TT800, (void))
{
    return rand_int32_TT800()*(1.0f/4294967296.0f); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
_FUNC_DEF(float, rand_real3_TT800, (void))
{
    return (((float)rand_int32_TT800()) + 0.5f)*(1.0f/4294967296.0f); 
    /* divided by 2^32 */
}
// These are fat and inefficient for now, they will be replaced in the next release

/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(vector unsigned int, rand_int32_TT800_i4, (void))
{
  vector unsigned int temp;
  temp = spu_promote(rand_int32_TT800(), 0);
  temp = spu_insert(rand_int32_TT800(), temp, 1);
  temp = spu_insert(rand_int32_TT800(), temp, 2);
  temp = spu_insert(rand_int32_TT800(), temp, 3);
  return (temp);
}

/* generates a random number on [0,0x7fffffff]-interval */
_FUNC_DEF(vector unsigned int, rand_int31_TT800_i4, (void))
{
  vector unsigned int temp;
  temp = spu_promote(rand_int31_TT800(), 0);
  temp = spu_insert(rand_int31_TT800(), temp, 1);
  temp = spu_insert(rand_int31_TT800(), temp, 2);
  temp = spu_insert(rand_int31_TT800(), temp, 3);
  return (temp);
}

/* generates a random number on [0,1]-real-interval */
_FUNC_DEF(vector float, rand_real1_TT800_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real1_TT800(), 0);
  temp = spu_insert(rand_real1_TT800(), temp, 1);
  temp = spu_insert(rand_real1_TT800(), temp, 2);
  temp = spu_insert(rand_real1_TT800(), temp, 3);
  return (temp); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
_FUNC_DEF(vector float, rand_real2_TT800_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real2_TT800(), 0);
  temp = spu_insert(rand_real2_TT800(), temp, 1);
  temp = spu_insert(rand_real2_TT800(), temp, 2);
  temp = spu_insert(rand_real2_TT800(), temp, 3);
  return (temp); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
_FUNC_DEF(vector float, rand_real3_TT800_f4, (void))
{
  vector float temp;
  temp = spu_promote(rand_real3_TT800(), 0);
  temp = spu_insert(rand_real3_TT800(), temp, 1);
  temp = spu_insert(rand_real3_TT800(), temp, 2);
  temp = spu_insert(rand_real3_TT800(), temp, 3);
  return (temp); 
    /* divided by 2^32 */
}

#endif /* _TT800_H */
