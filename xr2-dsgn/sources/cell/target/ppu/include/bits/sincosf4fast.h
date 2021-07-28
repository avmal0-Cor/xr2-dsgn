/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#include "sincos_c.h"

//
//     A faster implementation of sincosf4.  Returns accurate
//     (21+ bits of the mantissa) results only for inputs in the range [-pi/4 to pi/4].  While
//     no error is reported, results are unpredictable for inputs outside this range.
//
_FUNC_DEF(void, sincosf4fast, (vec_float4 x, vec_float4* s, vec_float4* c))
{
    vec_float4 xl2,xl3;
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
 
    // Compute x^2 and x^3
    //
    xl2 = vec_madd(x,x,(vec_float4)vzero);
    xl3 = vec_madd(xl2,x,(vec_float4)vzero);
    

    // Compute both the sin and cos of the angles
    // using a polynomial expression:
    //   cx = 1.0f + xl2 * ((C0 * xl2 + C1) * xl2 + C2), and
    //   sx = x + xl3 * ((S0 * xl2 + S1) * xl2 + S2)
    //
    vec_float4 ct1 = vec_madd((vec_float4)(_SINCOS_CC0),xl2,(vec_float4)(_SINCOS_CC1));
    vec_float4 st1 = vec_madd((vec_float4)(_SINCOS_SC0),xl2,(vec_float4)(_SINCOS_SC1));

    vec_float4 ct2 = vec_madd(ct1,xl2,(vec_float4)(_SINCOS_CC2));
    vec_float4 st2 = vec_madd(st1,xl2,(vec_float4)(_SINCOS_SC2));
    
    *c = vec_madd(ct2,xl2,(vec_float4)(1.0f));
    *s = vec_madd(st2,xl3,x);

}

