/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _FLOATINVEC_MATH_IMPL_H
#define _FLOATINVEC_MATH_IMPL_H

namespace Vectormath {

#ifndef _VECTORMATH_ENABLE_FASTMATH

inline
floatInVec
acosf(floatInVec x)
{
    return floatInVec(acosf4(x.get128()));
}

inline
floatInVec
asinf(floatInVec x)
{
    return floatInVec(asinf4(x.get128()));
}

inline
floatInVec
atanf(floatInVec x)
{
    return floatInVec(atanf4(x.get128()));
}

inline
floatInVec
atan2f(floatInVec y, floatInVec x)
{
    return floatInVec(atan2f4(y.get128(), x.get128()));
}

inline
floatInVec
cbrtf(floatInVec x)
{
    return floatInVec(cbrtf4(x.get128()));
}

inline
floatInVec
ceilf(floatInVec x)
{
    return floatInVec(ceilf4(x.get128()));
}

inline
floatInVec
copysignf(floatInVec x, floatInVec y)
{
    return floatInVec(copysignf4(x.get128(), y.get128()));
}

inline
floatInVec
cosf(floatInVec x)
{
    return floatInVec(cosf4(x.get128()));
}

inline
floatInVec
divf(floatInVec x, floatInVec y)
{
    return floatInVec(divf4(x.get128(), y.get128()));
}

inline
floatInVec
exp2f(floatInVec x)
{
    return floatInVec(exp2f4(x.get128()));
}

inline
floatInVec
expf(floatInVec x)
{
    return floatInVec(expf4(x.get128()));
}

inline
floatInVec
expm1f(floatInVec x)
{
    return floatInVec(expm1f4(x.get128()));
}

inline
floatInVec
fabsf(floatInVec x)
{
    return floatInVec(fabsf4(x.get128()));
}

inline
floatInVec
fdimf(floatInVec x, floatInVec y)
{
    return floatInVec(fdimf4(x.get128(), y.get128()));
}

inline
floatInVec
floorf(floatInVec x)
{
    return floatInVec(floorf4(x.get128()));
}

inline
floatInVec
fmaf(floatInVec x, floatInVec y, floatInVec z)
{
    return floatInVec(fmaf4(x.get128(), y.get128(), z.get128()));
}

inline
floatInVec
fmaxf(floatInVec x, floatInVec y)
{
    return floatInVec(fmaxf4(x.get128(), y.get128()));
}

inline
floatInVec
fminf(floatInVec x, floatInVec y)
{
    return floatInVec(fminf4(x.get128(), y.get128()));
}

inline
floatInVec
fmodf(floatInVec x, floatInVec y)
{
    return floatInVec(fmodf4(x.get128(), y.get128()));
}

inline
floatInVec
hypotf(floatInVec x, floatInVec y)
{
    return floatInVec(hypotf4(x.get128(), y.get128()));
}

inline
floatInVec
log10f(floatInVec x)
{
    return floatInVec(log10f4(x.get128()));
}

inline
floatInVec
log1pf(floatInVec x)
{
    return floatInVec(log1pf4(x.get128()));
}

inline
floatInVec
log2f(floatInVec x)
{
    return floatInVec(log2f4(x.get128()));
}

inline
floatInVec
logbf(floatInVec x)
{
    return floatInVec(logbf4(x.get128()));
}

inline
floatInVec
logf(floatInVec x)
{
    return floatInVec(logf4(x.get128()));
}

inline
floatInVec
modff(floatInVec x, floatInVec* i)
{
    return floatInVec(modff4(x.get128(), (vec_float4*)i));
}

inline
floatInVec
negatef(floatInVec x)
{
    return floatInVec(negatef4(x.get128()));
}

inline
floatInVec
powf(floatInVec x, floatInVec y)
{
    return floatInVec(powf4(x.get128(), y.get128()));
}

inline
floatInVec
recipf(floatInVec x)
{
    return floatInVec(recipf4(x.get128()));
}

inline
floatInVec
rsqrtf(floatInVec x)
{
    return floatInVec(rsqrtf4(x.get128()));
}

inline
floatInVec
remainderf(floatInVec x, floatInVec y)
{
    return floatInVec(remainderf4(x.get128(), y.get128()));
}

inline
void
sincosf(floatInVec x, floatInVec* s, floatInVec* c)
{
    sincosf4(x.get128(), (vec_float4*)s, (vec_float4*)c);
}

inline
floatInVec
sinf(floatInVec x)
{
    return floatInVec(sinf4(x.get128()));
}

inline
floatInVec
sqrtf(floatInVec x)
{
    return floatInVec(sqrtf4(x.get128()));
}

inline
floatInVec
tanf(floatInVec x)
{
    return floatInVec(tanf4(x.get128()));
}

inline
floatInVec
truncf(floatInVec x)
{
    return floatInVec(truncf4(x.get128()));
}

#else // _VECTORMATH_ENABLE_FASTMATH

inline
floatInVec
acosf(floatInVec x)
{
    return floatInVec(acosf4fast(x.get128()));
}

inline
floatInVec
asinf(floatInVec x)
{
    return floatInVec(asinf4fast(x.get128()));
}

inline
floatInVec
atanf(floatInVec x)
{
    return floatInVec(atanf4fast(x.get128()));
}

inline
floatInVec
atan2f(floatInVec y, floatInVec x)
{
    return floatInVec(atan2f4fast(y.get128(), x.get128()));
}

inline
floatInVec
cbrtf(floatInVec x)
{
    return floatInVec(cbrtf4fast(x.get128()));
}

inline
floatInVec
ceilf(floatInVec x)
{
    return floatInVec(ceilf4(x.get128()));
}

inline
floatInVec
copysignf(floatInVec x, floatInVec y)
{
    return floatInVec(copysignf4(x.get128(), y.get128()));
}

inline
floatInVec
cosf(floatInVec x)
{
    return floatInVec(cosf4fast(x.get128()));
}

inline
floatInVec
divf(floatInVec x, floatInVec y)
{
    return floatInVec(divf4fast(x.get128(), y.get128()));
}

inline
floatInVec
exp2f(floatInVec x)
{
    return floatInVec(exp2f4fast(x.get128()));
}

inline
floatInVec
expf(floatInVec x)
{
    return floatInVec(expf4fast(x.get128()));
}

inline
floatInVec
expm1f(floatInVec x)
{
    return floatInVec(expm1f4fast(x.get128()));
}

inline
floatInVec
fabsf(floatInVec x)
{
    return floatInVec(fabsf4(x.get128()));
}

inline
floatInVec
fdimf(floatInVec x, floatInVec y)
{
    return floatInVec(fdimf4(x.get128(), y.get128()));
}

inline
floatInVec
floorf(floatInVec x)
{
    return floatInVec(floorf4(x.get128()));
}

inline
floatInVec
fmaf(floatInVec x, floatInVec y, floatInVec z)
{
    return floatInVec(fmaf4(x.get128(), y.get128(), z.get128()));
}

inline
floatInVec
fmaxf(floatInVec x, floatInVec y)
{
    return floatInVec(fmaxf4(x.get128(), y.get128()));
}

inline
floatInVec
fminf(floatInVec x, floatInVec y)
{
    return floatInVec(fminf4(x.get128(), y.get128()));
}

inline
floatInVec
fmodf(floatInVec x, floatInVec y)
{
    return floatInVec(fmodf4(x.get128(), y.get128()));
}

inline
floatInVec
hypotf(floatInVec x, floatInVec y)
{
    return floatInVec(hypotf4(x.get128(), y.get128()));
}

inline
floatInVec
log10f(floatInVec x)
{
    return floatInVec(log10f4fast(x.get128()));
}

inline
floatInVec
log1pf(floatInVec x)
{
    return floatInVec(log1pf4fast(x.get128()));
}

inline
floatInVec
log2f(floatInVec x)
{
    return floatInVec(log2f4fast(x.get128()));
}

inline
floatInVec
logbf(floatInVec x)
{
    return floatInVec(logbf4(x.get128()));
}

inline
floatInVec
logf(floatInVec x)
{
    return floatInVec(logf4fast(x.get128()));
}

inline
floatInVec
modff(floatInVec x, floatInVec* i)
{
    return floatInVec(modff4(x.get128(), (vec_float4*)i));
}

inline
floatInVec
negatef(floatInVec x)
{
    return floatInVec(negatef4(x.get128()));
}

inline
floatInVec
powf(floatInVec x, floatInVec y)
{
    return floatInVec(powf4fast(x.get128(), y.get128()));
}

inline
floatInVec
recipf(floatInVec x)
{
    return floatInVec(recipf4fast(x.get128()));
}

inline
floatInVec
rsqrtf(floatInVec x)
{
    return floatInVec(rsqrtf4fast(x.get128()));
}

inline
floatInVec
remainderf(floatInVec x, floatInVec y)
{
    return floatInVec(remainderf4(x.get128(), y.get128()));
}

inline
void
sincosf(floatInVec x, floatInVec* s, floatInVec* c)
{
    sincosf4fast(x.get128(), (vec_float4*)s, (vec_float4*)c);
}

inline
floatInVec
sinf(floatInVec x)
{
    return floatInVec(sinf4fast(x.get128()));
}

inline
floatInVec
sqrtf(floatInVec x)
{
    return floatInVec(sqrtf4fast(x.get128()));
}

inline
floatInVec
tanf(floatInVec x)
{
    return floatInVec(tanf4fast(x.get128()));
}

inline
floatInVec
truncf(floatInVec x)
{
    return floatInVec(truncf4(x.get128()));
}

#endif // _VECTORMATH_ENABLE_FASTMATH

#ifndef _FASTMATH_H
// PLEASE DO NOT CALL THE FOLLOWING FUNCTIONS DIRECTLY!

inline
floatInVec
f_acosf(floatInVec x)
{
    return acosf(x);
}

inline
floatInVec
f_asinf(floatInVec x)
{
    return asinf(x);
}

inline
floatInVec
f_atanf(floatInVec x)
{
    return atanf(x);
}

inline
floatInVec
f_atan2f(floatInVec y, floatInVec x)
{
    return atan2f(y, x);
}

inline
floatInVec
f_cbrtf(floatInVec x)
{
    return cbrtf(x);
}

inline
floatInVec
f_ceilf(floatInVec x)
{
    return ceilf(x);
}

inline
floatInVec
f_copysignf(floatInVec x, floatInVec y)
{
    return copysignf(x, y);
}

inline
floatInVec
f_cosf(floatInVec x)
{
    return cosf(x);
}

inline
floatInVec
f_exp2f(floatInVec x)
{
    return exp2f(x);
}

inline
floatInVec
f_expf(floatInVec x)
{
    return expf(x);
}

inline
floatInVec
f_expm1f(floatInVec x)
{
    return expm1f(x);
}

inline
floatInVec
__builtin_fabsf(floatInVec x)
{
    return fabsf(x);
}

inline
floatInVec
f_fdimf(floatInVec x, floatInVec y)
{
    return fdimf(x, y);
}

inline
floatInVec
f_floorf(floatInVec x)
{
    return floorf(x);
}

inline
floatInVec
f_fmaf(floatInVec x, floatInVec y, floatInVec z)
{
    return fmaf(x, y, z);
}

inline
floatInVec
f_fmaxf(floatInVec x, floatInVec y)
{
    return fmaxf(x, y);
}

inline
floatInVec
f_fminf(floatInVec x, floatInVec y)
{
    return fminf(x, y);
}

inline
floatInVec
f_fmodf(floatInVec x, floatInVec y)
{
    return fmodf(x, y);
}

inline
floatInVec
f_hypotf(floatInVec x, floatInVec y)
{
    return hypotf(x, y);
}

inline
floatInVec
f_log10f(floatInVec x)
{
    return log10f(x);
}

inline
floatInVec
f_log1pf(floatInVec x)
{
    return log1pf(x);
}

inline
floatInVec
f_log2f(floatInVec x)
{
    return log2f(x);
}

inline
floatInVec
f_logbf(floatInVec x)
{
    return logbf(x);
}

inline
floatInVec
f_logf(floatInVec x)
{
    return logf(x);
}

inline
floatInVec
f_modff(floatInVec x, floatInVec* i)
{
    return modff(x, i);
}

inline
floatInVec
f_powf(floatInVec x, floatInVec y)
{
    return powf(x, y);
}

inline
floatInVec
f_remainderf(floatInVec x, floatInVec y)
{
    return remainderf(x, y);
}

inline
void
f_sincosf(floatInVec x, floatInVec* s, floatInVec* c)
{
    sincosf(x, s, c);
}

inline
floatInVec
f_sinf(floatInVec x)
{
    return sinf(x);
}

inline
floatInVec
f_sqrtf(floatInVec x)
{
    return sqrtf(x);
}

inline
floatInVec
f_tanf(floatInVec x)
{
    return tanf(x);
}

inline
floatInVec
f_truncf(floatInVec x)
{
    return truncf(x);
}

#endif // _FASTMATH_H

} // namespace Vectormath

#endif // _FLOATINVEC_MATH_IMPL_H
