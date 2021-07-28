/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
/* ymath.h internal header */
#ifndef _YMATH
#define _YMATH
#include <yvals.h>
_C_STD_BEGIN
_C_LIB_DECL

		/* MACROS FOR _FPP_TYPE */
#define _FPP_NONE	0	/* software emulation of FPP */
#define _FPP_X86	1	/* Intel Pentium */
#define _FPP_SPARC	2	/* Sun SPARC */
#define _FPP_MIPS	3	/* SGI MIPS */
#define _FPP_S390	4	/* IBM S/390 */
#define _FPP_PPC	5	/* Motorola PowerPC */
#define _FPP_HPPA	6	/* Hewlett-Packard PA-RISC */
#define _FPP_ALPHA	7	/* Compaq Alpha */
#define _FPP_ARM	8	/* ARM ARM */
#define _FPP_M68K	9	/* Motorola 68xxx */
#define _FPP_SH4	10	/* Hitachi SH4 */
#define _FPP_IA64	11	/* Intel IA64 */
#define _FPP_WCE	12	/* EDG Windows CE */
#define _FPP_SPU	13	/* STI SPU */

		/* MACROS FOR _Dtest RETURN (0 => ZERO) */
#define _DENORM		(-2)	/* C9X only */
#define _FINITE		(-1)
#define _INFCODE	1
#define _NANCODE	2
#define _SMAXCODE       3 	/* max value for SPU Exnteded Precision float. */

		/* MACROS FOR _Feraise ARGUMENT */

  #define _FE_DIVBYZERO	0x10000
  #define _FE_OVERFLOW	0x20
  #define _FE_UNDERFLOW	0x10
  #define _FE_INEXACT	0x08
  #define _FE_INVALID	0x04
  #define _FE_NC_NAN    0x02
  #define _FE_NC_DENORM 0x01
#define _FE_ALL_HW_EXCEPT	(FE_INEXACT \
	| _FE_INVALID | _FE_OVERFLOW | _FE_UNDERFLOW \
        | _FE_NC_NAN | _FE_NC_DENORM)

		/* TYPE DEFINITIONS */
typedef union
	{	/* pun float types as integer array */
	unsigned short _Word[8];
	float _Float;
	double _Double;
	long double _Long_double;
	} _Dconst;

		/* ERROR REPORTING */
void _Feraise(int);

		/* double DECLARATIONS */
double _Cosh(double, double);
short _Dtest(double *);
short _Exp(double *, double, short);
double _Log(double, int);
double _Sin(double, unsigned int);
double _Sinh(double, double);
extern /* const */ _Dconst _Denorm, _Hugeval, _Inf,
	_Nan, _Snan;

		/* float DECLARATIONS */
float _FCosh(float, float);
short _FDtest(float *);
short _FExp(float *, float, short);
float _FSinh(float, float);
extern /* const */ _Dconst _FMax, _FInf, _FNan;

		/* long double DECLARATIONS */
long double _LCosh(long double, long double);
short _LDtest(long double *);
short _LExp(long double *, long double, short);
long double _LLog(long double, int);
long double _LSin(long double, unsigned int);
long double _LSinh(long double, long double);
extern /* const */ _Dconst _LDenorm, _LInf, _LNan, _LSnan;

 #if defined(__SUNPRO_CC)	/* compiler test */
float fmodf(float, float);
long double fmodl(long double, long double);
 #endif /* defined(__SUNPRO_CC) */

 #if defined(__BORLANDC__)	/* compiler test */
float fmodf(float, float);
float logf(float);
 #endif /* defined(__BORLANDC__) */

_END_C_LIB_DECL
_C_STD_END
#endif /* _YMATH */

/*
 * Copyright (c) 1992-2003 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.02:0216 */
