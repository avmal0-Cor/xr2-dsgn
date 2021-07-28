/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _FLOATINVEC_MATH_H
#define _FLOATINVEC_MATH_H

#include <floatInVec.h>

namespace Vectormath {

/// @name floatInVec Math Extensions ("floatInVec_math.h" is required)
/// @{

// Compute the arc cosine of x
//
inline floatInVec acosf(floatInVec x);

// Compute the arc sine of x
//
inline floatInVec asinf(floatInVec x);

// Compute the arc tangent of x
//
inline floatInVec atanf(floatInVec x);

// Compute the arc tangent of y/x
//
inline floatInVec atan2f(floatInVec y, floatInVec x);

// Compute the cube root of x
//
inline floatInVec cbrtf(floatInVec x);

// Compute the smallest integral value greater than or equal to x
//
inline floatInVec ceilf(floatInVec x);

// Compute the value with the magnitude of x and the sign of y
//
inline floatInVec copysignf(floatInVec x, floatInVec y);

// Compute the cosine of x
//
inline floatInVec cosf(floatInVec x);

// Compute the quotient of x/y
//
inline floatInVec divf(floatInVec x, floatInVec y);

// Compute the base-2 exponential of x
//
inline floatInVec exp2f(floatInVec x);

// Compute the natural exponential of x
//
inline floatInVec expf(floatInVec x);

// Compute expf(x)-1
//
inline floatInVec expm1f(floatInVec x);

// Compute the absolute value of x
//
inline floatInVec fabsf(floatInVec x);

// Compute the difference if positive, 0 otherwise
//
inline floatInVec fdimf(floatInVec x, floatInVec y);

// Compute the largest integral value less than or equal to x
//
inline floatInVec floorf(floatInVec x);

// Compute (x*y)+z, rounded as one ternary operation
//
inline floatInVec fmaf(floatInVec x, floatInVec y, floatInVec z);

// Compute the maximum value of two values
//
inline floatInVec fmaxf(floatInVec x, floatInVec y);

// Compute the minimum value of two values
//
inline floatInVec fminf(floatInVec x, floatInVec y);

// Compute the remainder of x/y
//
inline floatInVec fmodf(floatInVec x, floatInVec y);

// Compute the Euclidean distance
//
inline floatInVec hypotf(floatInVec x, floatInVec y);

// Compute the base-10 logarithm of x
//
inline floatInVec log10f(floatInVec x);

// Compute logf(x+1)
//
inline floatInVec log1pf(floatInVec x);

// Compute the base-2 logarithm of x
//
inline floatInVec log2f(floatInVec x);

// Compute the exponent of x
//
inline floatInVec logbf(floatInVec x);

// Compute the natural logarithm of x
//
inline floatInVec logf(floatInVec x);

// Compute the integral and fractional parts of x
//
inline floatInVec modff(floatInVec x, floatInVec* i);

// Compute the negation of x
//
inline floatInVec negatef(floatInVec x);

// Compute x raised to the power of y
//
inline floatInVec powf(floatInVec x, floatInVec y);

// Compute the reciprocal of x
//
inline floatInVec recipf(floatInVec x);

// Compute the reciprocal of the square root of x
//
inline floatInVec rsqrtf(floatInVec x);

// Compute the remainder of x/y
//
inline floatInVec remainderf(floatInVec x, floatInVec y);

// Compute the sine and cosine of x
//
inline void sincosf(floatInVec x, floatInVec* s, floatInVec* c);

// Compute the sine of x
//
inline floatInVec sinf(floatInVec x);

// Compute the square root of x
//
inline floatInVec sqrtf(floatInVec x);

// Compute the tangent of x
//
inline floatInVec tanf(floatInVec x);

// Compute the integral value nearest to but no larger in magnitude than x
//
inline floatInVec truncf(floatInVec x);


/// @}

} // namespace Vectormath

#include <floatInVec_math_impl.h>

#endif // _FLOATINVEC_MATH_H
