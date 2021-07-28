/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
/* fenv.h standard header -- simple version */
#ifndef _FENV
#define _FENV
#ifndef _YMATH
 #include <ymath.h>
#endif /* _YMATH */

_C_STD_BEGIN
_C_LIB_DECL

typedef unsigned int fexcept_t;
typedef unsigned int fenv_t;

#define FE_DOWNWARD	0x03
#define FE_TONEAREST	0x00
#define FE_TOWARDZERO	0x01
#define FE_UPWARD	0x02

#define _FE_EXMASK_OFF	22
#define _FE_RND_OFF	0

		/* MACROS */
#define _FE_INVALID_S   0x00000400 /** for set the invalid exception */
#define _FE_INVALID_C   0x01F80700 /** for clear the invalid exception */

#define FE_DIVBYZERO	_FE_DIVBYZERO
#define FE_INEXACT	_FE_INEXACT
#define FE_INVALID	_FE_INVALID
#define FE_OVERFLOW	_FE_OVERFLOW
#define FE_UNDERFLOW	_FE_UNDERFLOW
#define FE_ALL_EXCEPT	(FE_DIVBYZERO | FE_INEXACT \
	| FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

#define _FE_RND_MASK	0x03u

#define FE_DFL_ENV	(&_CSTD _Fenv0)

		/* FUNCTION DECLARATIONS */
int feclearexcept(int);
int fegetexceptflag(fexcept_t *, int);
int feraiseexcept(int);
int fesetexceptflag(const fexcept_t *, int);
int fetestexcept(int);
int fegetround(void);
int fesetround(int);
int fegetenv(fenv_t *);
int feholdexcept(fenv_t *);
int fesetenv(const fenv_t *);
int feupdateenv(const fenv_t *);
fexcept_t fegettrapenable(void);
int fesettrapenable(fexcept_t);

double _Force_raise(int except);

		/* OBJECT DECLARATIONS */
extern /* const */ fenv_t _Fenv0;
_END_C_LIB_DECL
_C_STD_END
#endif /* _FENV */

#ifdef _STD_USING
using _CSTD fenv_t; using _CSTD fexcept_t;
using _CSTD feclearexcept; using _CSTD fegetexceptflag;
using _CSTD feraiseexcept; using _CSTD fesetexceptflag;
using _CSTD fetestexcept; using _CSTD fegetround;
using _CSTD fesetround; using _CSTD fegetenv;
using _CSTD feholdexcept; using _CSTD fesetenv;
using _CSTD feupdateenv;
using _CSTD fegettrapenable; using _CSTD fesettrapenable;

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
