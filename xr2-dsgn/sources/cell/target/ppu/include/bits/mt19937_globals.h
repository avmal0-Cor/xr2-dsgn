/*
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 */
#ifndef _MT19937_GLOBALS_H_
#define _MT19937_GLOBALS_H_

// These are global values and typedefs needed in the MT19937 random number generator
// implementation and in the definitions of it's global state vector

/* Period parameters */  
#define _PERIOD_N 624

typedef struct 
{
  // Current batch of MT19937 generated numbers
  unsigned int State[_PERIOD_N]; 
  // Next number in the state vector to be returned to the caller
  int Next_Number;
} __MT19937_State_t;

#endif
