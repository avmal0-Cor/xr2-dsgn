/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef __SINCOS_C2__
#define __SINCOS_C2__

//
// Common constants used to evaluate sind2/cosd2/tand2
//
#define _SINCOS_CC0D  0.00000000206374484196L
#define _SINCOS_CC1D -0.00000027555365134677L
#define _SINCOS_CC2D  0.00002480157946764225L
#define _SINCOS_CC3D -0.00138888888730525966L
#define _SINCOS_CC4D  0.04166666666651986722L
#define _SINCOS_CC5D -0.49999999999999547304L

#define _SINCOS_SC0D  0.00000000015893606014L
#define _SINCOS_SC1D -0.00000002505069049138L
#define _SINCOS_SC2D  0.00000275573131527032L
#define _SINCOS_SC3D -0.00019841269827816117L
#define _SINCOS_SC4D  0.00833333333331908278L
#define _SINCOS_SC5D -0.16666666666666612594L

#define _SINCOS_KC1D  (13176794.0L / 8388608.0L)
#define _SINCOS_KC2D  7.5497899548918821691639751442098584e-8L


//
// Common constants used to evaluate sinf4/cosf4/tanf4
//
#define _SINCOS_CC0  -0.0013602249f
#define _SINCOS_CC1   0.0416566950f
#define _SINCOS_CC2  -0.4999990225f
#define _SINCOS_SC0  -0.0001950727f
#define _SINCOS_SC1   0.0083320758f
#define _SINCOS_SC2  -0.1666665247f

#define _SINCOS_KC1  1.57079625129f
#define _SINCOS_KC2  7.54978995489e-8f

//
// Common constants used to evaluate sinf4est/cosf4est
//
#define _SINCOS_R1 -0.1666665668f
#define _SINCOS_R2  0.8333025139e-2f
#define _SINCOS_R3 -0.1980741872e-3f
#define _SINCOS_R4  0.2601903036e-5f

#define _SINCOS_C1  (201.0f/64.0f)
#define _SINCOS_C2  9.67653589793e-4f

/** _M_PI is internally used **/
#ifndef _M_PI
#define _M_PI 3.141592653589793f
#endif /* _M_PI */

#endif 
