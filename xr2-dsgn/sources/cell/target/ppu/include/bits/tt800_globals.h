/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef _TT800_GLOBALS_H_
#define _TT800_GLOBALS_H_

#include <yvals.h>

// These are global values and typedefs needed in the tt800 random number generator
// implementation and in the definitions of it's global state vector

#define _N_TT800 25
#define _N_TT800_QWORDS ((25/4)+1)
#define _M_TT800 7

typedef struct 
{
  // Current batch of TT800 generated numbers
  unsigned int State[_N_TT800]; 
  // Next number in the state vector to be returned to the caller
  int Next_Number;
} __TT800_State_t;

_C_LIB_DECL
extern __TT800_State_t __TT800;
_END_C_LIB_DECL

#endif
