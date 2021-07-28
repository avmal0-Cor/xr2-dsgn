/* tgmath.h standard header */
#ifndef _TGMATH
#define _TGMATH
#include <complex.h>
#include <math.h>

#ifdef __cplusplus
_C_STD_BEGIN
		// TEMPLATE CLASS _Rc_type (ADDITIONS TO <math.h>)
template<> struct _Rc_type<_Fcomplex>
	{	// determine if type is real or complex
	typedef char _Type;
	};

template<> struct _Rc_type<_Dcomplex>
	{	// determine if type is real or complex
	typedef char _Type;
	};

template<> struct _Rc_type<_Lcomplex>
	{	// determine if type is real or complex
	typedef char _Type;
	};

		// TEMPLATE CLASS _Rc_widened (ADDITIONS TO <math.h>)
template<> struct _Rc_widened<float, char>
	{	// determine real/complex
	typedef char _Type;
	};

template<> struct _Rc_widened<char, float>
	{	// determine real/complex
	typedef char _Type;
	};

template<> struct _Rc_widened<char, char>
	{	// determine real/complex
	typedef char _Type;
	};

		// TEMPLATE CLASS _Real_type (ADDITIONS TO <math.h>)
template<> struct _Real_type<_Fcomplex>
	{	// determine equivalent real type
	typedef float _Type;
	};

template<> struct _Real_type<_Dcomplex>
	{	// determine equivalent real type
	typedef double _Type;
	};

template<> struct _Real_type<_Lcomplex>
	{	// determine equivalent real type
	typedef long double _Type;
	};

		// TEMPLATE CLASS _Combined_type (ADDITIONS TO <math.h>)
template<> struct _Combined_type<char, float>
	{	// determine combined type
	typedef _Fcomplex _Type;
	};

template<> struct _Combined_type<char, double>
	{	// determine combined type
	typedef _Dcomplex _Type;
	};

template<> struct _Combined_type<char, long double>
	{	// determine combined type
	typedef _Lcomplex _Type;
	};

		// MACROS (ONE ARGUMENT)
 #define _TGEN_COMTYP(Carg)	typename _Combined_type<Carg, \
	typename _Real_type<_T1>::_Type>::_Type

 #define _TGEN1(fn, Carg, Ret) template<class _T1> inline \
	Ret fn(_T1 _Left) \
	{	/* overload fn(x) returning Ret, real/complex is Carg */ \
	typedef typename _Combined_type<Carg, \
		typename _Real_type<_T1>::_Type>::_Type \
		_Tw; \
	return (_CSTD fn((_Tw)_Left)); \
	}

 #define _TGEN_C(fn)	_TGEN1(fn, char, \
	_TGEN_COMTYP(char))

 #define _TGEN_C0(fn)	_TGEN1(fn, char, \
	typename _Real_type<_T1>::_Type)

 #define _TGEN_R(fn)	_TGEN1(fn, float, \
	typename _Real_type<_T1>::_Type)

 #define _TGEN_RI(fn, Ret)	_TGEN1(fn, float, Ret)

 #define _TGEN_RC(fn)	_TGEN1(fn, typename _Rc_type<_T1>::_Type, \
	_TGEN_COMTYP(typename _Rc_type<_T1>::_Type))

 #define _TGEN_RC0(fn)	_TGEN1(fn, typename _Rc_type<_T1>::_Type, \
	typename _Real_type<_T1>::_Type)

 #define _TGEN_RX(fn, Xarg) template<class _T1> inline \
	typename _Real_type<_T1>::_Type \
		fn(_T1 _Left, Xarg _Xtra) \
	{	/* overload fn(x, z) returning real, with Xarg z */ \
	typedef typename _Real_type<_T1>::_Type \
		_Tw; \
	return (_CSTD fn((_Tw)_Left, _Xtra)); \
	}

		// MACROS (TWO ARGUMENTS)
 #define _TGEN2(fn, Carg1, Carg2) template<class _T1, class _T2> inline \
	typename _Combined_type< \
		typename _Rc_widened<Carg1, Carg2>::_Type, \
		typename _Real_widened<typename _Real_type<_T1>::_Type, \
			typename _Real_type<_T2>::_Type>::_Type>::_Type \
		fn(_T1 _Left, _T2 _Right) \
	{	/* overload fn(x, y) returning Ret, real/complex is Carg1/Carg2 */ \
	typedef typename _Combined_type< \
		typename _Rc_widened<Carg1, Carg2>::_Type, \
		typename _Real_widened<typename _Real_type<_T1>::_Type, \
			typename _Real_type<_T2>::_Type>::_Type>::_Type \
		_Tw; \
	return (_CSTD fn((_Tw)_Left, (_Tw)_Right)); \
	}

 #define _TGEN_RC2(fn)	_TGEN2(fn, typename _Rc_type<_T1>::_Type, \
	typename _Rc_type<_T2>::_Type)

 #define _TGEN_R2(fn)	_TGEN2(fn, float, float)

 #define _TGEN_R2X(fn, Xarg) template<class _T1, class _T2> inline \
	typename _Real_widened<typename _Real_type<_T1>::_Type, \
		typename _Real_type<_T2>::_Type>::_Type \
		fn(_T1 _Left, _T2 _Right, Xarg _Xtra) \
	{	/* overload fn(x, y, z) returning real, with Xarg z */ \
	typedef typename _Real_widened<typename _Real_type<_T1>::_Type, \
			typename _Real_type<_T2>::_Type>::_Type \
		_Tw; \
	return (_CSTD fn((_Tw)_Left, (_Tw)_Right, _Xtra)); \
	}

		// MACROS (THREE ARGUMENTS)
 #define _TGEN_R3(fn) template<class _T1, class _T2, class _T3> inline \
	typename _Real_widened< \
		typename _Real_widened< \
			typename _Real_type<_T1>::_Type, \
			typename _Real_type<_T2>::_Type>::_Type, \
		typename _Real_type<_T3>::_Type>::_Type \
		fn(_T1 _Left, _T2 _Right, _T3 _Xtra) \
	{	/* overload fn(x, y, z) returning real */ \
	typedef typename _Real_widened< \
		typename _Real_widened< \
			typename _Real_type<_T1>::_Type, \
			typename _Real_type<_T2>::_Type>::_Type, \
		typename _Real_type<_T3>::_Type>::_Type \
		_Tw; \
	return (_CSTD fn((_Tw)_Left, (_Tw)_Right, (_Tw)_Xtra)); \
	}

		// GENERIC TEMPLATES
	_TGEN_RC(acos)
	_TGEN_RC(acosh)
	_TGEN_RC(asin)
	_TGEN_RC(asinh)
	_TGEN_RC(atan)
	_TGEN_RC(atanh)
	_TGEN_R2(atan2)
	_TGEN_C0(carg)
	_TGEN_R(cbrt)
	_TGEN_R(ceil)
	_TGEN_C0(cimag)
	_TGEN_C(conj)
	_TGEN_R2(copysign)
	_TGEN_RC(cos)
	_TGEN_RC(cosh)
	_TGEN_C(cproj)
	_TGEN_C0(creal)
	_TGEN_R(erf)
	_TGEN_R(erfc)
	_TGEN_RC(exp)
	_TGEN_R(exp2)
	_TGEN_R(expm1)
	_TGEN_RC0(fabs)
	_TGEN_R2(fdim)
	_TGEN_R(floor)
	_TGEN_R3(fma)
	_TGEN_R2(fmax)
	_TGEN_R2(fmin)
	_TGEN_R2(fmod)
	_TGEN_RX(frexp, int *)
	_TGEN_R2(hypot)
	_TGEN_RI(ilogb, int)
	_TGEN_RX(ldexp, int)
	_TGEN_R(lgamma)
	_TGEN_RI(llrint, _Longlong)
	_TGEN_RI(llround, _Longlong)
	_TGEN_RC(log)
	_TGEN_R(log10)
	_TGEN_R(log1p)
	_TGEN_R(log2)
	_TGEN_R(logb)
	_TGEN_RI(lrint, long)
	_TGEN_RI(lround, long)
	_TGEN_R(nearbyint)
	_TGEN_R2(nextafter)
	_TGEN_RX(nexttoward, long double)
	_TGEN_RC2(pow)
	_TGEN_R2(remainder)
	_TGEN_R2X(remquo, int *)
	_TGEN_R(rint)
	_TGEN_R(round)
	_TGEN_RX(scalbln, long)
	_TGEN_RX(scalbn, int)
	_TGEN_RC(sin)
	_TGEN_RC(sinh)
	_TGEN_RC(sqrt)
	_TGEN_RC(tan)
	_TGEN_RC(tanh)
	_TGEN_R(tgamma)
	_TGEN_R(trunc)
_C_STD_END

 #if defined(_STD_USING)
using _CSTD acos; using _CSTD acosh; using _CSTD asin;
using _CSTD asinh; using _CSTD atan; using _CSTD atanh;
using _CSTD atan2; using _CSTD carg; using _CSTD cbrt;
using _CSTD ceil; using _CSTD cimag; using _CSTD conj;
using _CSTD copysign; using _CSTD cos; using _CSTD cosh;
using _CSTD cproj; using _CSTD creal; using _CSTD erf;
using _CSTD erfc; using _CSTD exp; using _CSTD exp2;
using _CSTD expm1; using _CSTD fabs; using _CSTD fdim;
using _CSTD floor; using _CSTD fma; using _CSTD fmax;
using _CSTD fmin; using _CSTD fmod; using _CSTD frexp;
using _CSTD hypot; using _CSTD ilogb; using _CSTD ldexp;
using _CSTD lgamma; using _CSTD llrint; using _CSTD llround;
using _CSTD log; using _CSTD log10; using _CSTD log1p;
using _CSTD log2; using _CSTD logb; using _CSTD lrint;
using _CSTD lround; using _CSTD nearbyint; using _CSTD nextafter;
using _CSTD nexttoward; using _CSTD pow; using _CSTD remainder;
using _CSTD remquo; using _CSTD rint; using _CSTD round;
using _CSTD scalbln; using _CSTD scalbn; using _CSTD sin;
using _CSTD sinh; using _CSTD sqrt; using _CSTD tan;
using _CSTD tanh; using _CSTD tgamma; using _CSTD trunc;
 #endif /* defined(_STD_USING) */

#else /* __cplusplus */

 #if __EDG__
  #define _CARGR(x, fd, ff, fl) \
	__generic(x,,, fd, ff, fl,,,)(x)
  #define _CARGRI(return_type, x, fd, ff, fl) \
	__generic(x,,, fd, ff, fl,,,)(x)
  #define _CARGC(x, cfd, cff, cfl) \
	__generic(x,,, ,,, cfd, cff, cfl)(x)
  #define _CARGCX(x, cfd, cff, cfl) \
	__generic(x,,, ,,, cfd, cff, cfl)(x)
  #define _CARGRC(x, fd, ff, fl, cfd, cff, cfl) \
	__generic(x,,, fd, ff, fl, cfd, cff, cfl)(x)
  #define _CARGRCX(x, fd, ff, fl, cfd, cff, cfl) \
	__generic(x,,, fd, ff, fl, cfd, cff, cfl)(x)
  #define _CARG2R(x, y, fd, ff, fl) \
	__generic(x, y,, fd, ff, fl,,,)(x, y)
  #define _CARG2RX(x, y, fd, ff, fl) \
	__generic(x,,, fd, ff, fl,,,)(x, y)
  #define _CARG2RC(x, y, fd, ff, fl, cfd, cff, cfl) \
	__generic(x, y,, fd, ff, fl, cfd, cff, cfl)(x, y)
  #define _CARG3R(x, y, z, fd, ff, fl) \
	__generic(x, y, z, fd, ff, fl,,,)(x, y, z)
  #define _CARG3RX(x, y, z, fd, ff, fl) \
	__generic(x, y,, fd, ff, fl,,,)(x, y, z)

 #elif 2 <= __GNUC__ || defined(__APPLE__) && 3 <= __GNUC__
  #define _FLT_TYPE(expression)	_FLT_OR_DBL(__typeof__ (expression))
  #define _FLT_OR_DBL(mytype)	__typeof__ (*(0 \
	? (__typeof__ (0 ? (mytype *)0 : (void *)((mytype)0.5 == 0)))0 \
	: (__typeof__ (0 ? (double *)0 : (void *)((mytype)0.5 != 0)))0))

  #define _CARGR(x, fd, ff, fl) \
	(__extension__({ \
	_FLT_TYPE(x) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		_Ans = fd(x); \
	else if (sizeof (_Ans) == sizeof (float)) \
	_Ans = ff(x); \
	else \
		_Ans = fl(x); \
	_Ans; \
	}))
  #define _CARGRI(return_type, x, fd, ff, fl) \
	(__extension__({ \
	return_type _Ans; \
	_FLT_TYPE(x) _Type; \
	if (sizeof (_Type) == sizeof (double)) \
		_Ans = fd(x); \
	else if (sizeof (_Type) == sizeof (float)) \
		_Ans = ff(x); \
	else \
		_Ans = fl(x); \
	_Ans; \
	}))
  #define _CARGC(x, cfd, cff, cfl) \
	(__extension__({ \
	_FLT_TYPE(x) _Ans; \
	if (sizeof (__real__ (_Ans)) == sizeof (double)) \
		_Ans = cfd(x); \
	else if (sizeof (__real__ (_Ans)) == sizeof (float)) \
		_Ans = cff(x); \
	else \
		_Ans = cfl(x); \
	_Ans; \
	}))
  #define _CARGCX(x, cfd, cff, cfl) \
	(__extension__({ \
	_FLT_TYPE(__real__ (x)) _Ans; \
	if (sizeof (__real__ (_Ans)) == sizeof (double)) \
		_Ans = cfd(x); \
	else if (sizeof (__real__ (_Ans)) == sizeof (float)) \
		_Ans = cff(x); \
	else \
		_Ans = cfl(x); \
	_Ans; \
	}))
  #define _CARGRC(x, fd, ff, fl, cfd, cff, cfl) \
	(__extension__({ \
	_FLT_TYPE(x) _Ans; \
	if (sizeof (__real__ (_Ans)) == sizeof (double)) \
		if (sizeof (_Ans) == sizeof (double)) \
			_Ans = fd(x); \
		else \
			_Ans = cfd(x); \
	else if (sizeof (__real__ (_Ans)) == sizeof (float)) \
		if (sizeof (_Ans) == sizeof (float)) \
			_Ans = ff(x); \
		else \
			_Ans = cff(x); \
	else \
		if (sizeof (_Ans) == sizeof (long double)) \
			_Ans = fl(x); \
		else \
			_Ans = cfl(x); \
	_Ans; \
	}))
  #define _CARGRCX(x, fd, ff, fl, cfd, cff, cfl) \
	(__extension__({ \
	_FLT_TYPE(__real (x)) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		if (sizeof (x) == sizeof (double)) \
			_Ans = fd(x); \
		else \
			_Ans = cfd(x); \
	else if (sizeof (_Ans) == sizeof (float)) \
		if (sizeof (x) == sizeof (float)) \
			_Ans = ff(x); \
		else \
			_Ans = cff(x); \
	else \
		if (sizeof (x) == sizeof (long double)) \
			_Ans = fl(x); \
		else \
			_Ans = cfl(x); \
	_Ans; \
	}))
  #define _CARG2R(x, y, fd, ff, fl) \
	(__extension__({ \
	_FLT_TYPE((x) + (y)) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		_Ans = fd(x, y); \
	else if (sizeof (_Ans) == sizeof (float)) \
		_Ans = ff(x, y); \
	else \
		_Ans = fl(x, y); \
	_Ans; \
	}))
  #define _CARG2RX(x, y, fd, ff, fl) \
	(__extension__({ \
	_FLT_TYPE(x) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		_Ans = fd(x, y); \
	else if (sizeof (_Ans) == sizeof (float)) \
		_Ans = ff(x, y); \
	else \
		_Ans = fl(x, y); \
	_Ans; \
	}))
  #define _CARG2RC(x, y, fd, ff, fl, cfd, cff, cfl) \
	(__extension__({ \
	_FLT_TYPE((x) + (y)) _Ans; \
	if (sizeof (__real__ (_Ans)) == sizeof (double)) \
		if (sizeof (_Ans) == sizeof (double)) \
			_Ans = fd(x, y); \
		else \
			_Ans = cfd(x, y); \
	else if (sizeof (__real__ (_Ans)) == sizeof (float)) \
		if (sizeof (_Ans) == sizeof (float)) \
			_Ans = ff(x, y); \
		else \
			_Ans = cff(x, y); \
	else \
		if (sizeof (_Ans) == sizeof (long double)) \
			_Ans = fl(x, y); \
		else \
			_Ans = cfl(x, y); \
	_Ans; \
	}))
  #define _CARG3R(x, y, z, fd, ff, fl) \
	(__extension__({ \
	_FLT_TYPE((x) + (y) + (z)) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		_Ans = fd(x, y, z); \
	else if (sizeof (_Ans) == sizeof (float)) \
		_Ans = ff(x, y, z); \
	else \
		_Ans = fl(x, y, z); \
	_Ans; \
	}))
  #define _CARG3RX(x, y, z, fd, ff, fl) \
	(__extension__({ \
	_FLT_TYPE((x) + (y)) _Ans; \
	if (sizeof (_Ans) == sizeof (double)) \
		_Ans = fd(x, y, z); \
	else if (sizeof (_Ans) == sizeof (float)) \
		_Ans = ff(x, y, z); \
	else \
		_Ans = fl(x, y, z); \
	_Ans; \
	}))

 #else /* compiler type */
  #error type-generic math functions not supported
 #endif /* compiler type */

#undef acos
#define acos(x) \
	_CARGRC(x, acos, acosf, acosl, cacos, cacosf, cacosl)
#undef acosh
#define acosh(x) \
	_CARGRC(x, acosh, acoshf, acoshl, cacosh, cacoshf, cacoshl)
#undef asin
#define asin(x) \
	_CARGRC(x, asin, asinf, asinl, casin, casinf, casinl)
#undef asinh
#define asinh(x) \
	_CARGRC(x, asinh, asinhf, asinhl, casinh, casinhf, casinhl)
#undef atan
#define atan(x) \
	_CARGRC(x, atan, atanf, atanl, catan, catanf, catanl)
#undef atanh
#define atanh(x) \
	_CARGRC(x, atanh, atanhf, atanhl, catanh, catanhf, catanhl)
#undef atan2
#define atan2(y, x) \
	_CARG2R(y, x, atan2, atan2f, atan2l)
#undef carg
#define carg(x) \
	_CARGCX(x, carg, cargf, cargl)
#undef cbrt
#define cbrt(x) \
	_CARGR(x, cbrt, cbrtf, cbrtl)
#undef ceil
#define ceil(x) \
	_CARGR(x, ceil, ceilf, ceill)
#undef cimag
#define cimag(x) \
	_CARGCX(x, cimag, cimagf, cimagl)
#undef conj
#define conj(x) \
	_CARGC(x, conj, conjf, conjl)
#undef copysign
#define copysign(x, y) \
	_CARG2R(x, y, copysign, copysignf, copysignl)
#undef cos
#define cos(x) \
	_CARGRC(x, cos, cosf, cosl, ccos, ccosf, ccosl)
#undef cosh
#define cosh(x) \
	_CARGRC(x, cosh, coshf, coshl, ccosh, ccoshf, ccoshl)
#undef cproj
#define cproj(x) \
	_CARGC(x, cproj, cprojf, cprojl)
#undef creal
#define creal(x) \
	_CARGCX(x, creal, crealf, creall)
#undef erf
#define erf(x) \
	_CARGR(x, erf, erff, erfl)
#undef erfc
#define erfc(x) \
	_CARGR(x, erfc, erfcf, erfcl)
#undef exp
#define exp(x) \
	_CARGRC(x, exp, expf, expl, cexp, cexpf, cexpl)
#undef exp2
#define exp2(x) \
	_CARGR(x, exp2, exp2f, exp2l)
#undef expm1
#define expm1(x) \
	_CARGR(x, expm1, expm1f, expm1l)
#undef fabs
#define fabs(x) \
	_CARGRCX(x, fabs, fabsf, fabsl, cabs, cabsf, cabsl)
#undef fdim
#define fdim(x, y) \
	_CARG2R(x, y, fdim, fdimf, fdiml)
#undef floor
#define floor(x) \
	_CARGR(x, floor, floorf, floorl)
#undef fma
#define fma(x, y, z) \
	_CARG3R(x, y, z, fma, fmaf, fmal)
#undef fmax
#define fmax(x, y) \
	_CARG2R(x, y, fmax, fmaxf, fmaxl)
#undef fmin
#define fmin(x, y) \
	_CARG2R(x, y, fmin, fminf, fminl)
#undef fmod
#define fmod(x, y) \
	_CARG2R(x, y, fmod, fmodf, fmodl)
#undef frexp
#define frexp(x, y) \
	_CARG2RX(x, y, frexp, frexpf, frexpl)
#undef hypot
#define hypot(x, y) \
	_CARG2R(x, y, hypot, hypotf, hypotl)
#undef ilogb
#define ilogb(x) \
	_CARGRI(int, x, ilogb, ilogbf, ilogbl)
#undef ldexp
#define ldexp(x, y) \
	_CARG2RX(x, y, ldexp, ldexpf, ldexpl)
#undef lgamma
#define lgamma(x) \
	_CARGR(x, lgamma, lgammaf, lgammal)
#undef llrint
#define llrint(x) \
	_CARGRI(_Longlong, x, llrint, llrintf, llrintl)
#undef llround
#define llround(x) \
	_CARGRI(_Longlong, x, llround, llroundf, llroundl)
#undef log
#define log(x) \
	_CARGRC(x, log, logf, logl, clog, clogf, clogl)
#undef log1p
#define log1p(x) \
	_CARGR(x, log1p, log1pf, log1pl)
#undef log10
#define log10(x) \
	_CARGR(x, log10, log10f, log10l)
#undef log2
#define log2(x) \
	_CARGR(x, log2, log2f, log2l)
#undef logb
#define logb(x) \
	_CARGR(x, logb, logbf, logbl)
#undef lrint
#define lrint(x) \
	_CARGRI(long, x, lrint, lrintf, lrintl)
#undef lround
#define lround(x) \
	_CARGRI(long, x, lround, lroundf, lroundl)
#undef nearbyint
#define nearbyint(x) \
	_CARGR(x, nearbyint, nearbyintf, nearbyintl)
#undef nextafter
#define nextafter(x, y) \
	_CARG2R(x, y, nextafter, nextafterf, nextafterl)
#undef nexttoward
#define nexttoward(x, y) \
	_CARG2RX(x, y, nexttoward, nexttowardf, nexttowardl)
#undef pow
#define pow(x, y) \
	_CARG2RC(x, y, pow, powf, powl, cpow, cpowf, cpowl)
#undef remainder
#define remainder(x, y) \
	_CARG2R(x, y, remainder, remainderf, remainderl)
#undef remquo
#define remquo(x, y, z) \
	_CARG3RX(x, y, z, remquo, remquof, remquol)
#undef rint
#define rint(x) \
	_CARGR(x, rint, rintf, rintl)
#undef round
#define round(x) \
	_CARGR(x, round, roundf, roundl)
#undef scalbln
#define scalbln(x, y) \
	_CARG2RX(x, y, scalbln, scalblnf, scalblnl)
#undef scalbn
#define scalbn(x, y) \
	_CARG2RX(x, y, scalbn, scalbnf, scalbnl)
#undef sin
#define sin(x) \
	_CARGRC(x, sin, sinf, sinl, csin, csinf, csinl)
#undef sinh
#define sinh(x) \
	_CARGRC(x, sinh, sinhf, sinhl, csinh, csinhf, csinhl)
#undef sqrt
#define sqrt(x) \
	_CARGRC(x, sqrt, sqrtf, sqrtl, csqrt, csqrtf, csqrtl)
#undef tan
#define tan(x) \
	_CARGRC(x, tan, tanf, tanl, ctan, ctanf, ctanl)
#undef tanh
#define tanh(x) \
	_CARGRC(x, tanh, tanhf, tanhl, ctanh, ctanhf, ctanhl)
#undef tgamma
#define tgamma(x) \
	_CARGR(x, tgamma, tgammaf, tgammal)
#undef trunc
#define trunc(x) \
	_CARGR(x, trunc, truncf, truncl)
#endif /* __cplusplus */

#endif /* _TGMATH */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
