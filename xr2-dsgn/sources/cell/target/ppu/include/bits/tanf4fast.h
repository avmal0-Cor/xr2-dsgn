/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

// A faster implementation of tanf4. Returns accurate (21+bits of the mantissa) results only
// for inputs in the range [-pi/4 .. pi/4].  While no error is reported, results
// are unpredictable for inputs outside this range.  
//
_FUNC_DEF(vec_float4, tanf4fast, (vec_float4 x))
{
    vec_float4 x2,x3;
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));

    // Compute x^2 and x^3
    //
    x2 = vec_madd(x,x,(vec_float4)vzero);
    x3 = vec_madd(x2,x,(vec_float4)vzero);
    

    // Compute both the sin and cos of the angles
    // using a polynomial expression:
    //   cx = 1.0f + x2 * (C0 * x2 + C1), and
    //   sx = xl + x3 * S0
    //
    vec_float4 ct2 = vec_madd((vec_float4)( 0.0097099364f),x2,(vec_float4)(-0.4291161787f));
    
    vec_float4 cx = vec_madd(ct2,x2,(vec_float4)(1.0f));
    vec_float4 sx = vec_madd((vec_float4)(-0.0957822992f),x3,x);

    vec_float4 res = divf4(sx,cx);
    return res;
}
