/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _TT800_H
#define _TT800_H

/* The algorithm is taken from the following: */
/* C version of TT800 random number generator developed */
/* by M. Matsumoto, email: matumoto@math.keio.ac.jp */
/* See: ACM Transactions on Modelling and Computer Simulation, */
/* Vol. 4, No. 3, 1994, pages 254-266. */
// This version supports getting and saving of the state vector
// which is encapsulated in a global structure.  Also added functions to
// seed the generator from an integer or array. And to produce output numbers
// scaled into various ranges.

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

// Core random number generator, generates numbers in batches and returns them one at a time.
_FUNC_DEF(unsigned int, rand_int32_TT800,(void))
{
  unsigned int y;
  static const unsigned int mag01[2]={0x0, 0x8ebfd028 } ; // This is a magic number, do not change

  // Do we need to generate a new batch of numbers?
  if (__TT800.Next_Number==_N_TT800)
  { 
    // generate _N_TT800 words at one time 
    int kk;
    for (kk=0;kk<_N_TT800-_M_TT800;kk++)
    {
      __TT800.State[kk] = __TT800.State[kk+_M_TT800] ^ (__TT800.State[kk] >> 1) ^ mag01[__TT800.State[kk] % 2];
    }
    for (; kk<_N_TT800;kk++)
    {
      __TT800.State[kk] = __TT800.State[kk+(_M_TT800-_N_TT800)] ^ (__TT800.State[kk] >> 1) ^ mag01[__TT800.State[kk] % 2];
    }
    __TT800.Next_Number=0;
  }
  // Get the next available number from the array
  y = __TT800.State[__TT800.Next_Number];
  // Temper the number using some more magic numbers from Matsumoto's algorithm
  y ^= (y << 7) & 0x2b5b2500; 
  y ^= (y << 15) & 0xdb8b0000;
  y &= 0xffffffff; // Only required if word size != 32 
  y ^= (y >> 16); 
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
  unsigned int dr = rand_int32_TT800();
    return (((float)dr) + 0.5f)*(1.0f/4294967296.0f); 
    /* divided by 2^32 */
}

#endif /* _TT800_H */
