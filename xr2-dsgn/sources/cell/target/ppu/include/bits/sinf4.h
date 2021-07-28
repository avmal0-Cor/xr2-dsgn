/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#include "sincos_c.h"

//
//     Computes the sine of each of the four slots
//     by using a polynomial approximation.
//
_FUNC_DEF(vec_float4, sinf4, (vec_float4 x))
{
    vec_float4 xl,xl2,xl3,res;
    vec_int4   q;
    vec_float4 vzero = (vec_float4)vec_xor((vec_uint4)(0), (vec_uint4)(0));
    vec_uint4 smask = (vec_uint4)(vec_int4)(-1);
    smask = vec_vslw(smask, smask);

    // Range reduction using : xl = angle * TwoOverPi;
    //  
    xl = vec_madd(x, (vec_float4)(0.63661977236f),(vec_float4)vzero);

    // Find the quadrant the angle falls in
    // using:  q = (int) (ceil(abs(xl))*sign(xl))
    //
    xl = vec_add(xl,vec_sel((vec_float4)(0.5f),xl,smask));
    q = vec_cts(xl,0);

     
    // Compute an offset based on the quadrant that the angle falls in
    // 
    vec_int4 offset = vec_and(q,(vec_int4)((int)0x3));

    // Remainder in range [-pi/4..pi/4]
    //
    vec_float4 qf = vec_ctf(q,0);
    vec_float4 p1 = vec_nmsub(qf,(vec_float4)(_SINCOS_KC1),x);
    xl  = vec_nmsub(qf,(vec_float4)(_SINCOS_KC2),p1);
    
    // Compute x^2 and x^3
    //
    xl2 = vec_madd(xl,xl,(vec_float4)vzero);
    xl3 = vec_madd(xl2,xl,(vec_float4)vzero);
    

    // Compute both the sin and cos of the angles
    // using a polynomial expression:
    //   cx = 1.0f + xl2 * ((C0 * xl2 + C1) * xl2 + C2), and
    //   sx = xl + xl3 * ((S0 * xl2 + S1) * xl2 + S2)
    //
    vec_float4 ct1 = vec_madd((vec_float4)(_SINCOS_CC0),xl2,(vec_float4)(_SINCOS_CC1));
    vec_float4 st1 = vec_madd((vec_float4)(_SINCOS_SC0),xl2,(vec_float4)(_SINCOS_SC1));

    vec_float4 ct2 = vec_madd(ct1,xl2,(vec_float4)(_SINCOS_CC2));
    vec_float4 st2 = vec_madd(st1,xl2,(vec_float4)(_SINCOS_SC2));
    
    vec_float4 cx = vec_madd(ct2,xl2,(vec_float4)(1.0f));
    vec_float4 sx = vec_madd(st2,xl3,xl);

    // Use the cosine when the offset is odd and the sin
    // when the offset is even
    //
    vec_uint4 mask1 = (vec_uint4)vec_cmpeq(vec_and(offset,
                                                   (vec_int4)(0x1)),
                                           (vec_int4)vzero);
    res = vec_sel(cx,sx,mask1);

    // Flip the sign of the result when (offset mod 4) = 1 or 2
    //
    vec_uint4 mask2 = (vec_uint4)vec_cmpeq(vec_and(offset,(vec_int4)(0x2)),(vec_int4)vzero);
    res = vec_sel((vec_float4)vec_xor(smask,(vec_uint4)res),res,mask2);

    return res;

}

