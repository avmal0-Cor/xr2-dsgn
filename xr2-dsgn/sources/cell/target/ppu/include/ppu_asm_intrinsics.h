/*  SCE CONFIDENTIAL                          */
/*  PlayStation(R)3 Programmer Tool Runtime Library */
/*  Copyright (C) 2007-2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                        */

#ifndef _PPU_ASM_INTRINSICS_H
#define _PPU_ASM_INTRINSICS_H

/*
 * asm compatibility header to aid translation of GCC inline assembler to portable C/C++
 *
 * This header file maps many ppu instructions to the equivalent portable C code.
 * It is intended as a porting aid and guide.
 *
 * With optimisation, non-microcoded instructions should map 1:1 with each function.
 * for example, an __addic / __subfe pair should produce something similar
 * in the output. The __rlwinm function should map to a single rlwinm instruction and the
 * optimiser will reduce it further if possible.
 *
 * But above all, this header is intended to act as a guide to generating portable C.
 *
 */

#ifdef __SNC__
  /* non GNU C versions */

  #undef __ALWAYS_INLINE
  #define __ALWAYS_INLINE static inline __attribute__( ( always_inline ) )

  /*
   * xer-based instructions
   */

  #ifdef __SUPPORT_XER_INSTRUCTIONS__
    static __thread unsigned char __ca;
    __ALWAYS_INLINE long long __addc( long long a, long long b ) { unsigned long long result = a + b; __ca = (unsigned long long)a > (unsigned long long)~b; return result; }
    __ALWAYS_INLINE long long __adde( long long a, long long b ) { unsigned long long result = a + b + __ca; __ca = (unsigned long long)a+__ca > ~(unsigned long long)b | ( a == -1 ) & __ca; return result; }
    __ALWAYS_INLINE long long __addic( long long a, short b ) { unsigned long long result = a + b; __ca = (unsigned long long)a > (unsigned long long)~b; return result; }
    __ALWAYS_INLINE long long __addme( long long a ) { unsigned long long result = a - 1 + __ca; __ca = (unsigned long long)a != 0 | __ca; return result; }
    __ALWAYS_INLINE long long __addze( long long a ) { unsigned long long result = a + __ca; __ca = (unsigned long long)a > ~(unsigned long long)__ca; return result; }
    __ALWAYS_INLINE long long __subfc( long long a, long long b ) { unsigned long long result = b - a; __ca = (unsigned long long)a <= (unsigned long long)b; return result; }
    __ALWAYS_INLINE long long __subfe( long long a, long long b ) { unsigned long long result = ~a + b + __ca; __ca = (unsigned long long)~a+__ca > ~(unsigned long long)b | ( a == 0 ) & __ca; return result; }
    __ALWAYS_INLINE long long __subfme( long long a ) { unsigned long long result = ~a - 1 + __ca; __ca = (unsigned long long)a < (unsigned long long)__ca - 1; return result; }
    __ALWAYS_INLINE long long __subfze( long long a ) { unsigned long long result = ~a + __ca; __ca = ( a == 0 ) & __ca; return result; }
    __ALWAYS_INLINE long long __subfic( long long a, const short b ) { unsigned long long result = b - a; __ca = (unsigned long long)a <= (unsigned long long)b; return result; }
    __ALWAYS_INLINE long long __srad( long long a, long long b ) { unsigned long long result = a >> b; __ca = a < 0 & ( result << b ) != a; return result; }
    __ALWAYS_INLINE long long __sradi( long long a, unsigned char b ) { unsigned long long result = a >> b; __ca = a < 0 & ( result << b ) != a; return result; }
    __ALWAYS_INLINE long long __sraw( long long a, long long b ) { int result = (int)a >> b; __ca = a < 0 & ( result << b ) != (int)a; return result; }
    __ALWAYS_INLINE long long __srawi( long long a, unsigned char b ) { int result = (int)a >> b; __ca = a < 0 & ( result << b ) != (int)a; return result; }
  #endif

  /*
   * arithmetic instructions
   */

  __ALWAYS_INLINE long long __add( long long a, long long b ) { unsigned long long result = a + b; return result; }
  __ALWAYS_INLINE long long __addi( long long a, short b ) { unsigned long long result = a + b; return result; }
  __ALWAYS_INLINE long long __addis( long long a, short b ) { unsigned long long result = a + ( b << 16 ); return result; }
  __ALWAYS_INLINE long long __subf( long long a, long long b ) { unsigned long long result = b - a; return result; }
  __ALWAYS_INLINE long long __neg( long long a ) { unsigned long long result = -a; return result; }
  __ALWAYS_INLINE long long __divd( long long a, long long b ) { unsigned long long result = a / b; return result; }
  __ALWAYS_INLINE long long __divdu( unsigned long long a, unsigned long long b ) { unsigned long long result = a / b; return result; }
  __ALWAYS_INLINE long long __divw( long long a, long long b ) { unsigned long long result = (unsigned)( (signed)a / (signed)b ); return result; }
  __ALWAYS_INLINE long long __divwu( long long a, long long b ) { unsigned long long result = (unsigned)a / (unsigned)b; return result; }
  __ALWAYS_INLINE long long __mulhd( long long a, long long b ) { unsigned long long result = __builtin_mulhd( a, b ); return result; }
  __ALWAYS_INLINE long long __mulhdu( long long a, long long b ) { unsigned long long result = __builtin_mulhdu( a, b ); return result; }
  __ALWAYS_INLINE long long __mulhw( long long a, long long b ) { unsigned long long result = __builtin_mulhw( a, b ); return result; }
  __ALWAYS_INLINE long long __mulhwu( long long a, long long b ) { unsigned long long result = __builtin_mulhwu( a, b ); return result; }
  __ALWAYS_INLINE long long __mulld( long long a, long long b ) { unsigned long long result = a * b; return result; }
  __ALWAYS_INLINE long long __mulli( long long a, short b ) { unsigned long long result = a * b; return result; }
  __ALWAYS_INLINE long long __mullw( long long a, long long b ) { unsigned long long result = (long long )(int)a * (int)b; return result; }
  __ALWAYS_INLINE long long __extsb( long long a ) { unsigned long long result = (signed char)a; return result; }
  __ALWAYS_INLINE long long __extsh( long long a ) { unsigned long long result = (signed short)a; return result; }
  __ALWAYS_INLINE long long __extsw( long long a ) { unsigned long long result = (signed int)a; return result; }

  /*
   * logical instructions
   */

  __ALWAYS_INLINE long long __and( long long a, long long b ) { unsigned long long result = a & b; return result; }
  __ALWAYS_INLINE long long __andc( long long a, long long b ) { unsigned long long result = a & ~b; return result; }
  __ALWAYS_INLINE long long __eqv( long long a, long long b ) { unsigned long long result = ~( a ^ b ); return result; }
  __ALWAYS_INLINE long long __nand( long long a, long long b ) { unsigned long long result = ~( a & b ); return result; }
  __ALWAYS_INLINE long long __nor( long long a, long long b ) { unsigned long long result = ~( a | b ); return result; }
  __ALWAYS_INLINE long long __or( long long a, long long b ) { unsigned long long result = a | b; return result; }
  __ALWAYS_INLINE long long __orc( long long a, long long b ) { unsigned long long result = a | ~b; return result; }
  __ALWAYS_INLINE long long __ori( long long a, unsigned short b ) { unsigned long long result = a | (unsigned long long)b; return result; }
  __ALWAYS_INLINE long long __oris( long long a, unsigned short b ) { unsigned long long result = a | ( (unsigned long long)b << 16 ); return result; }
  __ALWAYS_INLINE long long __xor( long long a, long long b ) { unsigned long long result = a ^ b; return result; }
  __ALWAYS_INLINE long long __xori( long long a, const unsigned short b ) { unsigned long long result = a ^ (unsigned long long)b; return result; }
  __ALWAYS_INLINE long long __xoris( long long a, const unsigned short b ) { unsigned long long result = a ^ ( (unsigned long long)b << 16 ); return result; }

  /*
   * floating point instructions
   */

  __ALWAYS_INLINE double __fadd( double a, double b ) { double result = a + b; return result; }
  __ALWAYS_INLINE double __fadds( double a, double b ) { float result = a + b; return result; }
  __ALWAYS_INLINE double __fdiv( double a, double b ) { double result = a / b; return result; }
  __ALWAYS_INLINE double __fdivs( double a, double b ) { float result = a / b; return result; }
  __ALWAYS_INLINE double __fmadd( double a, double b, double c ) { double result = a * b + c; return result; }
  __ALWAYS_INLINE double __fmadds( double a, double b, double c ) { float result = a * b + c; return result; }
  __ALWAYS_INLINE double __fmr( double b ) { return b; }
  __ALWAYS_INLINE double __fmsubs( double a, double b, double c ) { float result = a * b - c; return result; }
  __ALWAYS_INLINE double __fmsub( double a, double b, double c ) { double result = a * b - c; return result; }

  /* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_asm_intrinsics.h when using it for future compatibility. */
  /*__ALWAYS_INLINE double __fabs( double a ) { double result = __builtin_fabs( a ); return result; } */

  /* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_asm_intrinsics.h when using it for future compatibility. */
  /*__ALWAYS_INLINE float __fabsf( float a ) { double result = __builtin_fabsf( a ); return result; } */

  __ALWAYS_INLINE double __fnabs( double a ) { double result = __builtin_fnabs( a ); return result; }
  __ALWAYS_INLINE double __fnabsf( double a ) { double result = __builtin_fnabsf( a ); return result; }
  __ALWAYS_INLINE double __fneg( double a ) { double result = -a; return result; }
  __ALWAYS_INLINE double __fnmadd( double a, double b, double c ) { double result = -( c + a * b ); return result; }
  __ALWAYS_INLINE double __fnmadds( double a, double b, double c ) { float result = -( c + a * b ); return result; }
  __ALWAYS_INLINE double __fnmsub( double a, double b, double c ) { double result = c - a * b; return result; }
  __ALWAYS_INLINE double __fnmsubs( double a, double b, double c ) { float result = c - a * b; return result; }
  __ALWAYS_INLINE float __fres( float a ) { double result = __builtin_fre( a ); return result; }
  __ALWAYS_INLINE double __fsqrt( double a ) { double result = __builtin_fsqrt( a ); return result; }

  /* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_asm_intrinsics.h when using it for future compatibility. */
  /*__ALWAYS_INLINE float __fsqrts( float a ) { double result = __builtin_fsqrt( a ); return result; } */

  __ALWAYS_INLINE double __frsp( double a ) { return (float)a; }

  /* Note: This intrinsic is being moved from the compiler to this header file, always include ppu_asm_intrinsics.h when using it for future compatibility. */
  /*__ALWAYS_INLINE double __fsel( double a, double b, double c ) { return __builtin_fsel( a, b, c ); } */

  __ALWAYS_INLINE double __fsub( double a, double b ) { double result = a - b; return result; }
  __ALWAYS_INLINE double __fsubs( double a, double b ) { float result = a - b; return result; }
  __ALWAYS_INLINE long long __lbz( const short offset, void *p ) { unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lbzx( void *p, long long offset ) { unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __ld( const short offset, void *p ) { unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __ldx( void *p, long long offset ) { unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE double __lfd( const short offset, void *p ) { double *addr = (double *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE double __lfdx( void *p, long long offset ) { double *addr = (double *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE double __lfs( const short offset, void *p ) { float *addr = (float *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE double __lfsx( void *p, long long offset ) { float *addr = (float *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lha( const short offset, void *p ) { short *addr = (short *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lhax( void *p, long long offset ) { short *addr = (short *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lhz( const short offset, void *p ) { unsigned short *addr = (unsigned short *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lhzx( void *p, long long offset ) { unsigned short *addr = (unsigned short *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lwa( const short offset, void *p ) { int *addr = (int *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lwax( void *p, long long offset ) { int *addr = (int *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lwz( const short offset, void *p ) { unsigned *addr = (unsigned *)( (unsigned char *)p + offset ); return *addr; }
  __ALWAYS_INLINE long long __lwzx( void *p, long long offset ) { unsigned *addr = (unsigned *)( (unsigned char *)p + offset ); return *addr; }


  /*
   * shifts:
   */

  __ALWAYS_INLINE long long __sld( long long a, long long b ) { unsigned long long result = a << b; return result; }
  __ALWAYS_INLINE long long __sldi( long long a, unsigned char b ) { unsigned long long result = a << b; return result; }
  __ALWAYS_INLINE long long __slw( long long a, long long b ) { unsigned result = (unsigned)a << (unsigned)b; return result; }
  __ALWAYS_INLINE long long __slwi( long long a, unsigned char b ) { unsigned result = (unsigned)a << (unsigned)b; return result; }
  __ALWAYS_INLINE long long __srd( long long a, long long b ) { unsigned long long result = (unsigned long long)a >> b; return result; }
  __ALWAYS_INLINE long long __srdi( long long a, long long b ) { unsigned long long result = (unsigned long long)a >> b; return result; }
  __ALWAYS_INLINE long long __srw( long long a, long long b ) { unsigned result = (unsigned)a >> b; return result; }
  __ALWAYS_INLINE long long __srwi( long long a, unsigned char b ) { unsigned result = (unsigned)a >> b; return result; }

  /*
   * stores:
   */

  /*
   * stores are converted to offset, indexed or update forms depending on context.
   *
   * store -> load -> store tripples of different types are re-interpreted
   *   eg. stfiwx or float to int conversion via stack-based unions
   */

  __ALWAYS_INLINE void __stb( long long a, const short offset, void *p ) { unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stbx( long long a, void *p, long long offset ) { unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __std( long long a, const short offset, void *p ) { unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stdx( long long a, void *p, long long offset ) { unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stfd( double a, const short offset, void *p ) { double *addr = (double *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stfdx( double a, void *p, long long offset ) { double *addr = (double *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stfs( double a, const short offset, void *p ) { float *addr = (float *)( (unsigned char *)p + offset ); *addr = (float)a; }
  __ALWAYS_INLINE void __stfsx( double a, void *p, long long offset ) { float *addr = (float *)( (unsigned char *)p + offset ); *addr = (float)a; }
  __ALWAYS_INLINE void __sth( long long a, const short offset, void *p ) { short *addr = (short *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __sthx( long long a, void *p, long long offset ) { short *addr = (short *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stw( long long a, const short offset, void *p ) { unsigned int *addr = (unsigned int *)( (unsigned char *)p + offset ); *addr = a; }
  __ALWAYS_INLINE void __stwx( long long a, void *p, long long offset ) { unsigned int *addr = (unsigned int *)( (unsigned char *)p + offset ); *addr = a; }

  /* store the bottom 32 bits of a fpu register after fctiw/fctiwz */
  __ALWAYS_INLINE void __stfiwx( double a, void *p, long long offset ) { __builtin_stfiwx( a, p, offset ); }


  /* load/store update instruction functional equivalents */
  #define __lbzu( offset, p ) __extension__ ({ unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __ldu( offset, p ) __extension__ ({ unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lfdu( offset, p ) __extension__ ({ double *addr = (double *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lfsu( offset, p ) __extension__ ({ float *addr = (float *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lhau( offset, p ) __extension__ ({ short *addr = (short *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lhzu( offset, p ) __extension__ ({ unsigned short *addr = (unsigned short *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lwau( offset, p ) __extension__ ({ int *addr = (int *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lwzu( offset, p ) __extension__ ({ unsigned *addr = (unsigned *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __stbu( a, offset, p ) __extension__ ({ unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stdu( a, offset, p ) __extension__ ({ unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stfdu( a, offset, p ) __extension__ ({ double *addr = (double *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stfsu( a, offset, p ) __extension__ ({ float *addr = (float *)( (unsigned char *)p + offset ); *addr = (float)a; p = (void*)addr; })
  #define __sthu( a, offset, p ) __extension__ ({ short *addr = (short *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stwu( a, offset, p ) __extension__ ({ unsigned int *addr = (unsigned int *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })

  #define __lbzux( p, offset ) __extension__ ({ unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __ldux( p, offset ) __extension__ ({ unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lfdux( p, offset ) __extension__ ({ double *addr = (double *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lfsux( p, offset ) __extension__ ({ float *addr = (float *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lhaux( p, offset ) __extension__ ({ short *addr = (short *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lhzux( p, offset ) __extension__ ({ unsigned short *addr = (unsigned short *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lwaux( p, offset ) __extension__ ({ int *addr = (int *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __lwzux( p, offset ) __extension__ ({ unsigned *addr = (unsigned *)( (unsigned char *)p + offset ); p = (void*)addr; *addr; })
  #define __stbux( a, p, offset ) __extension__ ({ unsigned char *addr = (unsigned char *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stdux( a, p, offset ) __extension__ ({ unsigned long long *addr = (unsigned long long *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stfdux( a, p, offset ) __extension__ ({ double *addr = (double *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stfsux( a, p, offset ) __extension__ ({ float *addr = (float *)( (unsigned char *)p + offset ); *addr = (float)a; p = (void*)addr; })
  #define __sthux( a, p, offset ) __extension__ ({ short *addr = (short *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })
  #define __stwux( a, p, offset ) __extension__ ({ unsigned int *addr = (unsigned int *)( (unsigned char *)p + offset ); *addr = a; p = (void*)addr; })

#else /* __GNUC__ */
  /* GNU C versions */

  /*
   * xer-based instructions
   */

  #ifdef __SUPPORT_XER_INSTRUCTIONS__
    #define __addc( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "addc %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __adde( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "adde %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __addic( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "addic %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "I"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __addme( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "addme %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) : "xer" ); __ppu_asm_tmp; })
    #define __addze( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "addze %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) : "xer" ); __ppu_asm_tmp; })
    #define __subfc( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "subfc %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ): "xer"  ); __ppu_asm_tmp; })
    #define __subfe( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "subfe %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ): "xer"  ); __ppu_asm_tmp; })
    #define __subfme( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "subfme %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) : "xer" ); __ppu_asm_tmp; })
    #define __subfze( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "subfze %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) : "xer" ); __ppu_asm_tmp; })
    #define __subfic( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "subfic %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "I"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __srad( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "srad %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __sradi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "sradi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __sraw( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "sraw %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) : "xer" ); __ppu_asm_tmp; })
    #define __srawi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__ volatile (  "srawi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) : "xer" ); __ppu_asm_tmp; })
  #endif

  /*
   * arithmetic instructions
   */

  #define __add( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "add %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __addi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "addi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "b"( a ), "I"( b ) ); __ppu_asm_tmp; })
  #define __addis( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "addis %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "b"( a ), "I"( b ) ); __ppu_asm_tmp; })

  #define __subf( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "subf %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })

  #define __neg( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "neg %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) ); __ppu_asm_tmp; })

  #define __extsb( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "extsb %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) ); __ppu_asm_tmp; })
  #define __extsh( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "extsh %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) ); __ppu_asm_tmp; })
  #define __extsw( a ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "extsw %0, %1" : "=r"( __ppu_asm_tmp ) : "r"( a ) ); __ppu_asm_tmp; })

  #define __divd( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "divd %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __divdu( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "divdu %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __divw( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "divw %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __divwu( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "divwu %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })

  #define __mulld( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "mulld %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __mulli( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "mulli %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "i"( (int)b ) ); __ppu_asm_tmp; })
  #define __mullw( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "mullw %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })

  /*
   * logical instructions
   */

  #define __and( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "and %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __andc( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "andc %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __eqv( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "eqv %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __or( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "or %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __orc( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "orc %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __ori( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "ori %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "i"( (unsigned)b ) ); __ppu_asm_tmp; })
  #define __oris( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "oris %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "i"( (unsigned)b ) ); __ppu_asm_tmp; })
  #define __nand( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "nand %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __nor( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "nor %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __xor( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "xor %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __xori( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "xori %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "i"( (unsigned)b ) ); __ppu_asm_tmp; })
  #define __xoris( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "xoris %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "i"( (unsigned)b ) ); __ppu_asm_tmp; })

  /*
   * floating point instructions
   */

  #define __fadd( a, b ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fadd %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })
  #define __fadds( a, b ) __extension__ ({ float __ppu_asm_tmp; __asm__(  "fadds %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })
  #define __fdiv( a, b ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fdiv %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })
  #define __fdivs( a, b ) __extension__ ({ float __ppu_asm_tmp; __asm__(  "fdivs %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })
  #define __fmr( a ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fmr %0, %1" : "=f"( __ppu_asm_tmp ) : "f"( a ) ); __ppu_asm_tmp; })
  #define __fneg( a ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fneg %0, %1" : "=f"( __ppu_asm_tmp ) : "f"( a ) ); __ppu_asm_tmp; })
  #define __fsub( a, b ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fsub %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })
  #define __fsubs( a, b ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "fsubs %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "f"( a ), "f"( b ) ); __ppu_asm_tmp; })

  /* Load instructions.  */

  #define __lbz( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lbz %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lbzu( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lbzu %0, %2( %1 )" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lbzux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lbzux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lbzx( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lbzx %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __ld( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "ld %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __ldu( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "ldu %0, %2( %1 )" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __ldux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "ldux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __ldx( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "ldx %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lfd( offset, p ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfd %0, %1( %2 )" : "=f"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lfdu( offset, p ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfdu %0, %2( %1 )" : "=f"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lfdux( p, offset ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfdux %0, %1, %2" : "=f"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lfdx( p, offset ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfdx %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lfs( offset, p ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfs %0, %1( %2 )" : "=f"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lfsu( offset, p ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfsu %0, %2( %1 )" : "=f"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lfsux( p, offset ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfsux %0, %1, %2" : "=f"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lfsx( p, offset ) __extension__ ({ double __ppu_asm_tmp; __asm__(  "lfsx %0, %1, %2" : "=f"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lhz( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhz %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lhzu( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhzu %0, %2( %1 )" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lhzux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhzux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lhzx( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhzx %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lwz( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwz %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lwzu( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwzu %0, %2( %1 )" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lwzux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwzux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lwzx( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwzx %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })

  /* Microcoded load instructions. */

  #define __lha( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lha %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lhau( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhau %0, %2( %1 )" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "I"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lhaux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhaux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lhax( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lhax %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lwa( offset, p ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwa %0, %1( %2 )" : "=r"( __ppu_asm_tmp ) : "I"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })
  #define __lwaux( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwaux %0, %1, %2" : "=r"( __ppu_asm_tmp ), "+b"( p ) : "r"( offset ) : "memory" ); __ppu_asm_tmp; })
  #define __lwax( p, offset ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "lwax %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( offset ), "b"( p ) : "memory" ); __ppu_asm_tmp; })



  /*
   * shift instructions
   */

  #define __sld( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "sld %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __sldi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "sldi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) ); __ppu_asm_tmp; })
  #define __slw( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "slw %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __slwi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "slwi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) ); __ppu_asm_tmp; })
  #define __srd( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "srd %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __srdi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "srdi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) ); __ppu_asm_tmp; })
  #define __srw( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "srw %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "r"( b ) ); __ppu_asm_tmp; })
  #define __srwi( a, b ) __extension__ ({ unsigned long long __ppu_asm_tmp; __asm__(  "srwi %0, %1, %2" : "=r"( __ppu_asm_tmp ) : "r"( a ), "K"( b ) ); __ppu_asm_tmp; })

  /*
   * store instructions
   */

  #define __stb( a, offset, p ) __extension__ ({ __asm__(  "stb %0, %1( %2 )" :  : "r"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __stbu( a, offset, p ) __extension__ ({ __asm__(  "stbu %1, %2( %0 )" : "+b"( p ) : "r"( a ), "I"( offset ) : "memory" ); })
  #define __stbux( a, p, offset ) __extension__ ({ __asm__(  "stbux %1, %0, %2" : "+b"( p ) : "r"( a ), "r"( offset ) : "memory" ); })
  #define __stbx( a, p, offset ) __extension__ ({ __asm__(  "stbx %0, %1, %2" :  : "r"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __std( a, offset, p ) __extension__ ({ __asm__(  "std %0, %1( %2 )" : : "r"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __stdu( a, offset, p ) __extension__ ({ __asm__(  "stdu %1, %2( %0 )" : "+b"( p ) : "r"(a), "I"( offset ) : "memory" ); })
  #define __stdux( a, p, offset ) __extension__ ({ __asm__(  "stdux %1, %0, %2" : "+b"( p ) : "r"( a ), "r"( offset ) : "memory" ); })
  #define __stdx( a, p, offset ) __extension__ ({ __asm__(  "stdx %0, %1, %2" : : "r"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __stfd( a, offset, p ) __extension__ ({ __asm__(  "stfd %0, %1( %2 )" : : "f"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __stfdu( a, offset, p ) __extension__ ({ __asm__(  "stfdu %1, %2( %0 )" : "+b"( p ) : "f"( a ), "I"( offset ) : "memory" ); })
  #define __stfdux( a, p, offset ) __extension__ ({ __asm__(  "stfdux %1, %0, %2" : "+b"( p ) : "f"( a ), "r"( offset ) : "memory" ); })
  #define __stfdx( a, p, offset ) __extension__ ({ __asm__(  "stfdx %0, %1, %2" : : "f"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __stfs( a, offset, p ) __extension__ ({ __asm__(  "stfs %0, %1( %2 )" : : "f"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __stfsu( a, offset, p ) __extension__ ({ __asm__(  "stfsu %1, %2( %0 )" : "+b"( p ) : "f"( a ), "I"( offset ) : "memory" ); })
  #define __stfsux( a, p, offset ) __extension__ ({ __asm__(  "stfsux %1, %0, %2" : "+b"( p ) : "f"( a ), "r"( offset ) : "memory" ); })
  #define __stfsx( a, p, offset ) __extension__ ({ __asm__(  "stfsx %0, %1, %2" : : "f"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __sth( a, offset, p ) __extension__ ({ __asm__(  "sth %0, %1( %2 )" : : "r"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __sthu( a, offset, p ) __extension__ ({ __asm__(  "sthu %1, %2( %0 )" : "+b"( p ) : "r"(a), "I"( offset ) : "memory" ); })
  #define __sthux( a, p, offset ) __extension__ ({ __asm__(  "sthux %1, %0, %2" : "+b"( p ) : "r"( a ), "r"( offset ) : "memory" ); })
  #define __sthx( a, p, offset ) __extension__ ({ __asm__(  "sthx %0, %1, %2" : : "r"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __stw( a, offset, p ) __extension__ ({ __asm__(  "stw %0, %1( %2 )" : : "r"( a ), "I"( offset ), "b"( p ) : "memory" ); })
  #define __stwu( a, offset, p ) __extension__ ({ __asm__(  "stwu %1, %2( %0 )" : "+b"( p ) : "r"( a ), "I"( offset ) : "memory" ); })
  #define __stwux( a, p, offset ) __extension__ ({ __asm__(  "stwux %1, %0, %2" : "+b"( p ) : "r"( a ), "r"( offset ) : "memory" ); })
  #define __stwx( a, p, offset ) __extension__ ({ __asm__(  "stwx %0, %1, %2" : : "r"( a ), "b"( p ), "r"( offset ) : "memory" ); })
  #define __stfiwx( a, p, offset ) __extension__ ({ __asm__(  "stfiwx %0, %1, %2" : : "f"( a ), "b"( p ), "r"( offset ) : "memory" ); })

#endif

#endif /* _PPU_ASM_INTRINSICS_H */

