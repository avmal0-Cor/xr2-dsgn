/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#define  _TAND2_CC0 -0.00020844956382258822L
#define  _TAND2_CC1  0.02334489464693293380L
#define  _TAND2_CC2 -0.46161689768996201755L
#define  _TAND2_SC0 -0.00000748373924372997L
#define  _TAND2_SC1  0.00280592875035233052L
#define  _TAND2_SC2 -0.12828356435663158978L
#define  _TAND2_KC1  (13176794.0L / 8388608.0L)
#define  _TAND2_KC2  7.5497899548918821691639751442098584e-8L

//
// Computes the tangent of the given angles by first reducing the
// range to [-pi/4..pi/4] and performing the appropriate sin/cos ratio
//
_FUNC_DEF(vec_double2, tand2, (vec_double2 x))
{
    vec_double2 xl,x2,x3,res;
    vec_double2 nan = (vec_double2)spu_splats(0x7ff8000000000000ull);
    vec_uchar16 copyEven = __extension__ (vec_uchar16)(vec_uint4){ 0x00010203, 0x00010203, 0x08090a0b, 0x08090a0b };

    // Range reduction using : xl = angle * TwoOverPi;
    //  
    xl = spu_mul(x, spu_splats((double)0.63661977236758134307553505349005744L));


    // Find the quadrant the angle falls in
    // using:  q = (int) (ceil(abs(xl))*sign(xl))
    //
    xl = spu_add(xl,spu_sel(spu_splats((double)0.5L),xl,spu_splats(0x8000000000000000ull)));
    vec_float4 xf = spu_roundtf(xl);
    vec_int4 q = spu_convts(xf,0);
    q = spu_shuffle(q,q,copyEven);

     
    // Remainder in range [-pi/4..pi/4]
    //
    vec_float4 qf = spu_convtf(q,0);
    vec_double2 qd = spu_extend(qf);
    vec_double2 p1 = spu_nmsub(qd,spu_splats((double)_TAND2_KC1),x);
    xl = spu_nmsub(qd,spu_splats((double)_TAND2_KC2),p1);
    
    // Compute x^2 and x^3
    //
    x2 = spu_mul(xl,xl);
    x3 = spu_mul(x2,xl);
    

    // Compute both the sin and cos of the angles
    // using a polynomial expression:
    //   cx = 1.0f + x2 * ((C0 * x2 + C1) * x2 + C2), and
    //   sx = x + x3 * ((S0 * x2 + S1) * x2 + S2)
    //
    vec_double2 ct1 = spu_madd(spu_splats((double)_TAND2_CC0),x2,spu_splats((double)_TAND2_CC1));
    vec_double2 st1 = spu_madd(spu_splats((double)_TAND2_SC0),x2,spu_splats((double)_TAND2_SC1));

    vec_double2 ct2 = spu_madd(ct1,x2,spu_splats((double)_TAND2_CC2));
    vec_double2 st2 = spu_madd(st1,x2,spu_splats((double)_TAND2_SC2));
    
    vec_double2 cx = spu_madd(ct2,x2,spu_splats((double)1.0L));
    vec_double2 sx = spu_madd(st2,x3,xl);

    
    // Compute both cx/sx and sx/cx
    //
    vec_double2 cxosx = divd2(cx,sx);
    vec_double2 sxocx = divd2(sx,cx);

    vec_double2 ncxosx = (vec_double2)spu_xor(spu_splats(0x8000000000000000ull),(vec_ullong2)cxosx);

    // For odd numbered quadrants return -cx/sx , otherwise return
    // sx/cx
    //
    vec_ullong2 mask = (vec_ullong2)spu_cmpeq(spu_and(q,(int)0x1),spu_splats((int)0));
    res = spu_sel(ncxosx,sxocx,mask);

    // If input = +/-Inf return NAN
    //
    res = spu_sel(res,nan,cmpinfd2(x));

    // If input =0 or denorm return input
    //
    res = spu_sel(res,x,cmpzerodenormd2(x));

    return res;
}
