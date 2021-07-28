/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#define _TAN_KC1  1.57079625129f
#define _TAN_KC2  7.54978995489e-8f

//
// Computes the tangent of all four slots of x by using a polynomial approximation.
//
_FUNC_DEF(vec_float4, tanf4, (vec_float4 x))
{
    vec_float4 xl,x2,x3,res;
    vec_int4   q;

    // Range reduction using : xl = angle * TwoOverPi;
    //  
    xl = spu_mul(x, spu_splats(0.63661977236f));

    // Find the quadrant the angle falls in
    // using:  q = (int) (ceil(abs(x))*sign(x))
    //
    xl = spu_add(xl,spu_sel(spu_splats(0.5f),xl,spu_splats(0x80000000)));
    q = spu_convts(xl,0);

     
    // Remainder in range [-pi/4..pi/4]
    //
    vec_float4 qf = spu_convtf(q,0);
    vec_float4 p1 = spu_nmsub(qf,spu_splats(_TAN_KC1),x);
    xl  = spu_nmsub(qf,spu_splats(_TAN_KC2),p1);
    
    // Compute x^2 and x^3
    //
    x2 = spu_mul(xl,xl);
    x3 = spu_mul(x2,xl);
    

    // Compute both the sin and cos of the angles
    // using a polynomial expression:
    //   cx = 1.0f + x2 * (C0 * x2 + C1), and
    //   sx = xl + x3 * S0
    //
    vec_float4 ct2 = spu_madd(spu_splats( 0.0097099364f),x2,spu_splats(-0.4291161787f));
    
    vec_float4 cx = spu_madd(ct2,x2,spu_splats(1.0f));
    vec_float4 sx = spu_madd(spu_splats(-0.0957822992f),x3,xl);

    
    // Compute both cx/sx and sx/cx
    //
    vec_float4 cxosx = divf4(cx,sx);
    vec_float4 sxocx = divf4(sx,cx);

    vec_float4 ncxosx = (vec_float4)spu_xor(spu_splats(0x80000000),(vec_uint4)cxosx);

    // For odd numbered quadrants return -cx/sx , otherwise return
    // sx/cx
    //
    vec_uchar16 mask = (vec_uchar16)spu_cmpeq(spu_and(q,(int)0x1),spu_splats((int)0));
    res = spu_sel(ncxosx,sxocx,mask);

    return res;
}
