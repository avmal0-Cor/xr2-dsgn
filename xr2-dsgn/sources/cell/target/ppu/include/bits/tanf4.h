/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

#define _TAN_KC1  1.57079625129f
#define _TAN_KC2  7.54978995489e-8f

//
// Computes the tangent of all four slots of x by using a polynomia approximation. 
//
_FUNC_DEF(vec_float4, tanf4, (vec_float4 x))
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
    // using:  q = (int) (ceil(abs(x))*sign(x))
    //
    xl = vec_add(xl,vec_sel((vec_float4)(0.5f),xl,smask));
    q = vec_cts(xl,0);

     
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
    //   cx = 1.0f + x2 * (C0 * x2 + C1), and
    //   sx = xl + x3 * S0
    //
    vec_float4 ct2 = vec_madd((vec_float4)( 0.0097099364f),xl2,(vec_float4)(-0.4291161787f));
    
    vec_float4 cx = vec_madd(ct2,xl2,(vec_float4)(1.0f));
    vec_float4 sx = vec_madd((vec_float4)(-0.0957822992f),xl3,xl);

    
    // Compute both cx/sx and sx/cx
    //
    vec_float4 cxosx = divf4(cx,sx);
    vec_float4 sxocx = divf4(sx,cx);

    vec_float4 ncxosx = (vec_float4)vec_xor(smask,(vec_uint4)cxosx);

    // For odd numbered quadrants return -cx/sx , otherwise return
    // sx/cx
    //
    vec_uint4 mask = (vec_uint4)vec_cmpeq(vec_and(q,(vec_int4)(0x1)),(vec_int4)vzero);
    res = vec_sel(ncxosx,sxocx,mask);

    return res;
}
