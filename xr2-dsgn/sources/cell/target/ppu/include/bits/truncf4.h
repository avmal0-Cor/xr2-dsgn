/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
// truncf4 - for each of four float slots, round towards zero to integer value.

_FUNC_DEF(
   vec_float4,
   truncf4,
   ( vec_float4 x ))
{
   return vec_trunc( x );
}

