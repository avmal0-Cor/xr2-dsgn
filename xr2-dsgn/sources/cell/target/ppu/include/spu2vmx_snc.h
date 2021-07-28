/*  SCE CONFIDENTIAL                          */
/*  PlayStation(R)3 Programmer Tool Runtime Library */
/*  Copyright (C) 2007-2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                        */

#ifndef _SPU2VMX_SNC_H_
#define _SPU2VMX_SNC_H_	1

#ifndef __SPU__

/* see SPU C/C++ Language Extensions, Version 2.1 */

#include <si2vmx_snc.h>

/* 2.3. Constant Formation Intrinsics */

__SNC_INLINE vec_uchar16 spu_splats(  unsigned char a ) { return (vec_uchar16)a; }
__SNC_INLINE vec_char16 spu_splats(  signed char a ) { return (vec_char16)a; }
__SNC_INLINE vec_ushort8 spu_splats(  unsigned short a ) { return (vec_ushort8)a; }
__SNC_INLINE vec_short8 spu_splats(  signed short a ) { return (vec_short8)a; }
__SNC_INLINE vec_uint4 spu_splats(  unsigned int a ) { return (vec_uint4)a; }
__SNC_INLINE vec_int4 spu_splats(  signed int a ) { return (vec_int4)a; }
__SNC_INLINE vec_float4 spu_splats(  float a ) { return (vec_float4)a; }
__SNC_INLINE vec_ullong2 spu_splats(  unsigned long long a ) { return (vec_ullong2)(vec_uint4)( (a >> 32ull), (a & (1ll<<32)-1 ), (a >> 32ull), (a & (1ll<<32)-1 ) ); }
__SNC_INLINE vec_llong2 spu_splats(  signed long long a ) { return (vec_llong2)(vec_uint4)( (a >> 32ull), (a & (1ll<<32)-1 ), (a >> 32), (a & (1ll<<32)-1 ) ); }
__SNC_INLINE vec_double2 spu_splats(  double a ) { unsigned long long t = *(unsigned long long*)&a; return (vec_double2)(vec_uint4)( (t >> 32ull), ( t & (1ll<<32)-1 ), (t >> 32ull), (t & (1ll<<32)-1 ) ); }
 
/* 2.4. Conversion Intrinsics  */

__SNC_INLINE vec_float4 spu_convtf( vec_uint4 a, int b ) { return (vec_float4)si_cuflt( (qword)a, b ); }
__SNC_INLINE vec_float4 spu_convtf( vec_int4 a, int b ) { return (vec_float4)si_csflt( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_convts( vec_float4 a, int b ) { return (vec_int4)si_cflts( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_convtu( vec_float4 a, int b ) { return (vec_uint4)si_cfltu( (qword)a, b ); }

__SNC_INLINE vec_short8 spu_extend( vec_char16 a ) { return (vec_short8)si_xsbh( (qword)a ); }
__SNC_INLINE vec_int4 spu_extend( vec_short8 a ) { return (vec_int4)si_xshw( (qword)a ); }
__SNC_INLINE vec_llong2 spu_extend( vec_int4 a ) { return (vec_llong2)si_xswd( (qword)a ); }
__SNC_INLINE vec_double2 spu_extend( vec_float4 a ) { return (vec_double2)si_fesd( (qword)a ); }

__SNC_INLINE vec_float4 spu_roundtf( vec_double2 a ) { return (vec_float4)si_frds( (qword)a ); }

/* 2.5. Arithmetic Intrinsics  */

__SNC_INLINE vec_uint4 spu_add( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_a( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_add( vec_int4 a, vec_int4 b ) { return (vec_int4)si_a( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_add( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_ah( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_add( vec_short8 a, vec_short8 b ) { return (vec_short8)si_ah( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_add( vec_float4 a, vec_float4 b ) { return (vec_float4)si_fa( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_add( vec_double2 a, vec_double2 b ) { return (vec_double2)si_dfa( (qword)a, (qword)b ); }

__SNC_INLINE vec_uint4 spu_add( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_ai( (qword)a, ( int )b ); }
__SNC_INLINE vec_int4 spu_add( vec_int4 a, int b ) { return (vec_int4)si_ai( (qword)a, b ); }
__SNC_INLINE vec_ushort8 spu_add( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_ahi( (qword)a, (short)b ); }
__SNC_INLINE vec_short8 spu_add( vec_short8 a, short b ) { return (vec_short8)si_ahi( (qword)a, b ); }

__SNC_INLINE vec_uint4 spu_addx( vec_uint4 a, vec_uint4 b, vec_uint4 c ) { return (vec_uint4)si_addx( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_int4 spu_addx( vec_int4 a, vec_int4 b, vec_int4 c ) { return (vec_int4)si_addx( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_uint4 spu_genb( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_bg( (qword)b, (qword)a ); }
__SNC_INLINE vec_int4 spu_genb( vec_int4 a, vec_int4 b ) { return (vec_int4)si_bg( (qword)b, (qword)a ); }
__SNC_INLINE vec_uint4 spu_genbx( vec_uint4 a, vec_uint4 b, vec_uint4 c ) { return (vec_uint4)si_bgx( (qword)b, (qword)a, (qword)c ); }
__SNC_INLINE vec_int4 spu_genbx( vec_int4 a, vec_int4 b, vec_int4 c ) { return (vec_int4)si_bgx( (qword)b, (qword)a, (qword)c ); }
__SNC_INLINE vec_uint4 spu_genc( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_cg( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_genc( vec_int4 a, vec_int4 b ) { return (vec_int4)si_cg( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_gencx( vec_uint4 a, vec_uint4 b, vec_uint4 c ) { return (vec_uint4)si_cgx( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_int4 spu_gencx( vec_int4 a, vec_int4 b, vec_int4 c ) { return (vec_int4)si_cgx( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_int4 spu_madd( vec_short8 a, vec_short8 b, vec_int4 c ) { return (vec_int4)si_mpya( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_float4 spu_madd( vec_float4 a, vec_float4 b, vec_float4 c ) { return (vec_float4)si_fma( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_double2 spu_madd( vec_double2 a, vec_double2 b, vec_double2 c ) { return (vec_double2)si_fma( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_int4 spu_mhhadd( vec_short8 a, vec_short8 b, vec_int4 c ) { return (vec_int4)si_mpyhha( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_uint4 spu_mhhadd( vec_ushort8 a, vec_ushort8 b, vec_uint4 c ) { return (vec_uint4)si_mpyhhau( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_float4 spu_msub( vec_float4 a, vec_float4 b, vec_float4 c ) { return (vec_float4)si_fms( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_double2 spu_msub( vec_double2 a, vec_double2 b, vec_double2 c ) { return (vec_double2)si_fms( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_float4 spu_mul( vec_float4 a, vec_float4 b ) { return (vec_float4)si_fm( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_mul( vec_double2 a, vec_double2 b ) { return (vec_double2)si_dfm( (qword)a, (qword)b ); }

__SNC_INLINE vec_int4 spu_mulh( vec_short8 a, vec_short8 b ) { return (vec_int4)si_mpyh( (qword)a, (qword)b ); }

/* GCC uses spu_mulhh for spu_mule */
__SNC_INLINE vec_int4 spu_mule( vec_short8 a, vec_short8 b ) { return (vec_int4)si_mpyhh( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_mule( vec_ushort8 a, vec_ushort8 b ) { return (vec_int4)si_mpyhhu( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_mulhh( vec_short8 a, vec_short8 b ) { return (vec_int4)si_mpyhh( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_mulhh( vec_ushort8 a, vec_ushort8 b ) { return (vec_int4)si_mpyhhu( (qword)a, (qword)b ); }

__SNC_INLINE vec_int4 spu_mulo( vec_short8 a, vec_short8 b ) { return (vec_int4)si_mpy( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_mulo( vec_ushort8 a, vec_ushort8 b ) { return (vec_uint4)si_mpyu( (qword)a, (qword)b ); }

__SNC_INLINE vec_int4 spu_mulo( vec_short8 a, short b ) { return (vec_int4)si_mpyi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_mulo( vec_ushort8 a, unsigned short b ) { return (vec_uint4)si_mpyui( (qword)a, b ); }

__SNC_INLINE vec_int4 spu_mulsr( vec_short8 a, vec_short8 b ) { return (vec_int4)si_mpys( (qword)a, (qword)b ); }

__SNC_INLINE vec_double2 spu_nmadd( vec_double2 a, vec_double2 b, vec_double2 c ) { return (vec_double2)si_dfnma( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_float4 spu_nmsub( vec_float4 a, vec_float4 b, vec_float4 c ) { return (vec_float4)si_fnms( (qword)a, (qword)b, (qword)c ); }
__SNC_INLINE vec_double2 spu_nmsub( vec_double2 a, vec_double2 b, vec_double2 c ) { return (vec_double2)si_dfnms( (qword)a, (qword)b, (qword)c ); }

__SNC_INLINE vec_float4 spu_re( vec_float4 a ) { return (vec_float4)si_fi( (qword)a, si_frest( (qword)a ) ); }
__SNC_INLINE vec_float4 spu_rsqrte( vec_float4 a ) { return (vec_float4)si_fi( (qword)a, si_frsqest( (qword)a ) ); }

__SNC_INLINE vec_ushort8 spu_sub( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_sfh( (qword)b, (qword)a ); }
__SNC_INLINE vec_short8 spu_sub( vec_short8 a, vec_short8 b ) { return (vec_short8)si_sfh( (qword)b, (qword)a ); }
__SNC_INLINE vec_uint4 spu_sub( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_sf( (qword)b, (qword)a ); }
__SNC_INLINE vec_int4 spu_sub( vec_int4 a, vec_int4 b ) { return (vec_int4)si_sf( (qword)b, (qword)a ); }
__SNC_INLINE vec_float4 spu_sub( vec_float4 a, vec_float4 b ) { return (vec_float4)si_fs( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_sub( vec_double2 a, vec_double2 b ) { return (vec_double2)si_dfs( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_sub(  unsigned int a, vec_uint4 b ) { return (vec_uint4)si_sfi( (qword)b, ( int )a ); }
__SNC_INLINE vec_int4 spu_sub(  signed int a, vec_int4 b ) { return (vec_int4)si_sfi( (qword)b, ( int )a ); }
__SNC_INLINE vec_ushort8 spu_sub(  unsigned short a, vec_ushort8 b ) { return (vec_ushort8)si_sfhi( (qword)b, ( short )a ); }
__SNC_INLINE vec_short8 spu_sub(  signed short a, vec_short8 b ) { return (vec_short8)si_sfhi( (qword)b, ( short )a ); }

__SNC_INLINE vec_uint4 spu_subx( vec_uint4 a, vec_uint4 b, vec_uint4 c ) { return (vec_uint4)si_sfx( (qword)b, (qword)a, (qword)c ); }
__SNC_INLINE vec_int4 spu_subx( vec_int4 a, vec_int4 b, vec_int4 c ) { return (vec_int4)si_sfx( (qword)b, (qword)a, (qword)c ); }

/* 2.6. Byte Operation Intrinsics  */

__SNC_INLINE vec_uchar16 spu_absd( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_absdb( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_avg( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_avgb( (qword)a, (qword)b ); }

__SNC_INLINE vec_ushort8 spu_sumb( vec_uchar16 a, vec_uchar16 b ) { return (vec_ushort8)si_sumb( (qword)a, (qword)b ); }  

/* 2.7. Compare, Branch and Halt Intrinsics  */

__SNC_INLINE void spu_bisled( void (*func)() ) { si_bisled( func ); }
__SNC_INLINE void spu_bisled_d( void (*func)() ) { si_bisledd( func ); }
__SNC_INLINE void spu_bisled_e( void (*func)() ) { si_bislede( func ); }

__SNC_INLINE vec_uint4 spu_cmpabseq( vec_float4 a, vec_float4 b ) { return (vec_uint4)si_fcmeq( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpabsgt( vec_float4 a, vec_float4 b ) { return (vec_uint4)si_fcmgt( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_cmpeq( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_ceqb( (qword)a, (qword)b ); }
__SNC_INLINE vec_uchar16 spu_cmpeq( vec_char16 a, vec_char16 b ) { return (vec_uchar16)si_ceqb( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_cmpeq( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_ceqh( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_cmpeq( vec_short8 a, vec_short8 b ) { return (vec_ushort8)si_ceqh( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpeq( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_ceq( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpeq( vec_int4 a, vec_int4 b ) { return (vec_uint4)si_ceq( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpeq( vec_float4 a, vec_float4 b ) { return (vec_uint4)si_fceq( (qword)a, (qword)b ); }
__SNC_INLINE vec_uchar16 spu_cmpeq( vec_uchar16 a, unsigned char b ) { return (vec_uchar16)si_ceqbi( (qword)a, ( signed char )b ); }
__SNC_INLINE vec_uchar16 spu_cmpeq( vec_char16 a, signed char b ) { return (vec_uchar16)si_ceqbi( (qword)a, b ); }
__SNC_INLINE vec_ushort8 spu_cmpeq( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_ceqhi( (qword)a, ( signed short )b ); }
__SNC_INLINE vec_ushort8 spu_cmpeq( vec_short8 a, signed short b ) { return (vec_ushort8)si_ceqhi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_cmpeq( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_ceqi( (qword)a, ( signed int )b ); }
__SNC_INLINE vec_uint4 spu_cmpeq( vec_int4 a, signed int b ) { return (vec_uint4)si_ceqi( (qword)a, b ); }

__SNC_INLINE vec_uchar16 spu_cmpgt( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_clgtb( (qword)a, (qword)b ); }
__SNC_INLINE vec_uchar16 spu_cmpgt( vec_char16 a, vec_char16 b ) { return (vec_uchar16)si_cgtb( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_cmpgt( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_clgth( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_cmpgt( vec_short8 a, vec_short8 b ) { return (vec_ushort8)si_cgth( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpgt( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_clgt( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpgt( vec_int4 a, vec_int4 b ) { return (vec_uint4)si_cgt( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_cmpgt( vec_float4 a, vec_float4 b ) { return (vec_uint4)si_fcgt( (qword)a, (qword)b ); }
__SNC_INLINE vec_uchar16 spu_cmpgt( vec_uchar16 a, unsigned char b ) { return (vec_uchar16)si_clgtbi( (qword)a, b ); }
__SNC_INLINE vec_uchar16 spu_cmpgt( vec_char16 a, signed char b ) { return (vec_uchar16)si_cgtbi( (qword)a, b ); }
__SNC_INLINE vec_ushort8 spu_cmpgt( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_clgthi( (qword)a, b ); }
__SNC_INLINE vec_ushort8 spu_cmpgt( vec_short8 a, signed short b ) { return (vec_ushort8)si_cgthi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_cmpgt( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_clgti( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_cmpgt( vec_int4 a, signed int b ) { return (vec_uint4)si_cgti( (qword)a, b ); }

__SNC_INLINE void spu_hcmpeq( unsigned int a, unsigned int b ) { si_heq( si_from_uint( a ), si_from_uint( b ) ); }
__SNC_INLINE void spu_hcmpeq( signed int a, signed int b ) { si_heq( si_from_int( a ), si_from_int( b ) ); }
__SNC_INLINE void spu_hcmpgt( unsigned int a, unsigned int b ) { si_hgt( si_from_uint( a ), si_from_uint( b ) ); }
__SNC_INLINE void spu_hcmpgt( signed int a, signed int b ) { si_hgt( si_from_int( a ), si_from_int( b ) ); }

/* 2.8. Bits and Mask Intrinsics */

__SNC_INLINE vec_uchar16 spu_cntb( vec_uchar16 a ) { return (vec_uchar16)si_cntb( (qword)a ); }
__SNC_INLINE vec_uchar16 spu_cntb( vec_char16 a ) { return (vec_uchar16)si_cntb( (qword)a ); }

__SNC_INLINE vec_uint4 spu_cntlz( vec_uint4 a ) { return (vec_uint4)si_clz( (qword)a ); }
__SNC_INLINE vec_uint4 spu_cntlz( vec_int4 a ) { return (vec_uint4)si_clz( (qword)a ); }
__SNC_INLINE vec_uint4 spu_cntlz( vec_float4 a ) { return (vec_uint4)si_clz( (qword)a ); }

__SNC_INLINE vec_uint4 spu_gather( vec_uchar16 a ) { return (vec_uint4)si_gbb( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_char16 a ) { return (vec_uint4)si_gbb( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_ushort8 a ) { return (vec_uint4)si_gbh( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_short8 a ) { return (vec_uint4)si_gbh( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_uint4 a ) { return (vec_uint4)si_gb( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_int4 a ) { return (vec_uint4)si_gb( (qword)a ); }
__SNC_INLINE vec_uint4 spu_gather( vec_float4 a ) { return (vec_uint4)si_gb( (qword)a ); }

__SNC_INLINE vec_uchar16 spu_maskb( unsigned short value ) { return __builtin_constant_p( value ) ? si_fsmbi( value ) : si_fsmb( si_from_ushort( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskb( signed short value ) { return __builtin_constant_p( value ) ? si_fsmbi( value ) : si_fsmb( si_from_short( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskb( unsigned int value ) { return __builtin_constant_p( value ) ? si_fsmbi( value ) : si_fsmb( si_from_uint( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskb( signed int value ) { return __builtin_constant_p( value ) ? si_fsmbi( value ) : si_fsmb( si_from_int( value ) ); }

__SNC_INLINE vec_uchar16 spu_maskh( unsigned short value ) { return si_fsmh( si_from_ushort( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskh( signed short value ) { return si_fsmh( si_from_short( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskh( unsigned int value ) { return si_fsmh( si_from_uint( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskh( signed int value ) { return si_fsmh( si_from_int( value ) ); }

__SNC_INLINE vec_uchar16 spu_maskw( unsigned short value ) { return si_fsm( si_from_ushort( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskw( signed short value ) { return si_fsm( si_from_short( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskw( unsigned int value ) { return si_fsm( si_from_uint( value ) ); }
__SNC_INLINE vec_uchar16 spu_maskw( signed int value ) { return si_fsm( si_from_int( value ) ); }

__SNC_INLINE vec_uchar16 spu_sel( vec_uchar16 a, vec_uchar16 b, vec_uchar16 pattern ) { return (vec_uchar16)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_char16 spu_sel( vec_char16 a, vec_char16 b, vec_uchar16 pattern ) { return (vec_char16)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_ushort8 spu_sel( vec_ushort8 a, vec_ushort8 b, vec_ushort8 pattern ) { return (vec_ushort8)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_short8 spu_sel( vec_short8 a, vec_short8 b, vec_ushort8 pattern ) { return (vec_short8)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_uint4 spu_sel( vec_uint4 a, vec_uint4 b, vec_uint4 pattern ) { return (vec_uint4)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_int4 spu_sel( vec_int4 a, vec_int4 b, vec_uint4 pattern ) { return (vec_int4)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_float4 spu_sel( vec_float4 a, vec_float4 b, vec_uint4 pattern ) { return (vec_float4)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_ullong2 spu_sel( vec_ullong2 a, vec_ullong2 b, vec_ullong2 pattern ) { return (vec_ullong2)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_llong2 spu_sel( vec_llong2 a, vec_llong2 b, vec_ullong2 pattern ) { return (vec_llong2)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_double2 spu_sel( vec_double2 a, vec_double2 b, vec_ullong2 pattern ) { return (vec_double2)si_selb( (qword)a, (qword)b, (qword)( pattern ) ); }

__SNC_INLINE vec_uchar16 spu_shuffle( vec_uchar16 a, vec_uchar16 b, vec_uchar16 pattern ) { return (vec_uchar16)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_char16 spu_shuffle( vec_char16 a, vec_char16 b, vec_uchar16 pattern ) { return (vec_char16)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_ushort8 spu_shuffle( vec_ushort8 a, vec_ushort8 b, vec_uchar16 pattern ) { return (vec_ushort8)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_short8 spu_shuffle( vec_short8 a, vec_short8 b, vec_uchar16 pattern ) { return (vec_short8)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_uint4 spu_shuffle( vec_uint4 a, vec_uint4 b, vec_uchar16 pattern ) { return (vec_uint4)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_int4 spu_shuffle( vec_int4 a, vec_int4 b, vec_uchar16 pattern ) { return (vec_int4)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_float4 spu_shuffle( vec_float4 a, vec_float4 b, vec_uchar16 pattern ) { return (vec_float4)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_ullong2 spu_shuffle( vec_ullong2 a, vec_ullong2 b, vec_uchar16 pattern ) { return (vec_ullong2)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_llong2 spu_shuffle( vec_llong2 a, vec_llong2 b, vec_uchar16 pattern ) { return (vec_llong2)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }
__SNC_INLINE vec_double2 spu_shuffle( vec_double2 a, vec_double2 b, vec_uchar16 pattern ) { return (vec_double2)si_shufb( (qword)a, (qword)b, (qword)( pattern ) ); }

__SNC_INLINE vec_uchar16 spu_and( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_and( vec_char16 a, vec_char16 b ) { return (vec_char16)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_and( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_and( vec_short8 a, vec_short8 b ) { return (vec_short8)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_and( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_and( vec_int4 a, vec_int4 b ) { return (vec_int4)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_and( vec_float4 a, vec_float4 b ) { return (vec_float4)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_and( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_and( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_and( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_and( vec_double2 a, vec_double2 b ) { return (vec_double2)si_and( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_and( vec_uchar16 a, unsigned char b ) { return (vec_uchar16)si_andbi( (qword)a, ( signed char )b ); }
__SNC_INLINE vec_char16 spu_and( vec_char16 a, signed char b ) { return (vec_char16)si_andbi( (qword)a, b ); }
__SNC_INLINE vec_ushort8 spu_and( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_andhi( (qword)a, ( signed short )b ); }
__SNC_INLINE vec_short8 spu_and( vec_short8 a, signed short b ) { return (vec_short8)si_andhi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_and( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_andi( (qword)a, ( signed int )b ); }
__SNC_INLINE vec_int4 spu_and( vec_int4 a, signed int b ) { return (vec_int4)si_andi( (qword)a, b ); }

__SNC_INLINE vec_uchar16 spu_andc( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_andc( vec_char16 a, vec_char16 b ) { return (vec_char16)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_andc( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_andc( vec_short8 a, vec_short8 b ) { return (vec_short8)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_andc( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_andc( vec_int4 a, vec_int4 b ) { return (vec_int4)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_andc( vec_float4 a, vec_float4 b ) { return (vec_float4)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_andc( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_andc( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_andc( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_andc( vec_double2 a, vec_double2 b ) { return (vec_double2)si_andc( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_eqv( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_eqv( vec_char16 a, vec_char16 b ) { return (vec_char16)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_eqv( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_eqv( vec_short8 a, vec_short8 b ) { return (vec_short8)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_eqv( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_eqv( vec_int4 a, vec_int4 b ) { return (vec_int4)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_eqv( vec_float4 a, vec_float4 b ) { return (vec_float4)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_eqv( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_eqv( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_eqv( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_eqv( vec_double2 a, vec_double2 b ) { return (vec_double2)si_eqv( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_nand( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_nand( vec_char16 a, vec_char16 b ) { return (vec_char16)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_nand( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_nand( vec_short8 a, vec_short8 b ) { return (vec_short8)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_nand( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_nand( vec_int4 a, vec_int4 b ) { return (vec_int4)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_nand( vec_float4 a, vec_float4 b ) { return (vec_float4)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_nand( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_nand( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_nand( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_nand( vec_double2 a, vec_double2 b ) { return (vec_double2)si_nand( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_nor( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_nor( vec_char16 a, vec_char16 b ) { return (vec_char16)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_nor( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_nor( vec_short8 a, vec_short8 b ) { return (vec_short8)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_nor( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_nor( vec_int4 a, vec_int4 b ) { return (vec_int4)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_nor( vec_float4 a, vec_float4 b ) { return (vec_float4)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_nor( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_nor( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_nor( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_nor( vec_double2 a, vec_double2 b ) { return (vec_double2)si_nor( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_or( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_or( vec_char16 a, vec_char16 b ) { return (vec_char16)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_or( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_or( vec_short8 a, vec_short8 b ) { return (vec_short8)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_or( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_or( vec_int4 a, vec_int4 b ) { return (vec_int4)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_or( vec_float4 a, vec_float4 b ) { return (vec_float4)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_or( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_or( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_or( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_or( vec_double2 a, vec_double2 b ) { return (vec_double2)si_or( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_or( vec_uchar16 a, unsigned char b ) { return (vec_uchar16)si_orbi( (qword)a, b ); }
__SNC_INLINE vec_char16 spu_or( vec_char16 a, signed char b ) { return (vec_char16)si_orbi( (qword)a, ( unsigned char )b ); }
__SNC_INLINE vec_ushort8 spu_or( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_orhi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_or( vec_short8 a, signed short b ) { return (vec_short8)si_orhi( (qword)a, ( unsigned short )b ); }
__SNC_INLINE vec_uint4 spu_or( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_ori( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_or( vec_int4 a, signed int b ) { return (vec_int4)si_ori( (qword)a, ( unsigned int )b ); }

__SNC_INLINE vec_uchar16 spu_orc( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_orc( vec_char16 a, vec_char16 b ) { return (vec_char16)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_orc( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_orc( vec_short8 a, vec_short8 b ) { return (vec_short8)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_orc( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_orc( vec_int4 a, vec_int4 b ) { return (vec_int4)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_orc( vec_float4 a, vec_float4 b ) { return (vec_float4)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_orc( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_orc( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_orc( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_orc( vec_double2 a, vec_double2 b ) { return (vec_double2)si_orc( (qword)a, (qword)b ); }

__SNC_INLINE vec_uint4 spu_orx( vec_uint4 a ) { return (vec_uint4)si_orx( (qword)a ); }
__SNC_INLINE vec_int4 spu_orx( vec_int4 a ) { return (vec_int4)si_orx( (qword)a ); }

__SNC_INLINE vec_uchar16 spu_xor( vec_uchar16 a, vec_uchar16 b ) { return (vec_uchar16)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_char16 spu_xor( vec_char16 a, vec_char16 b ) { return (vec_char16)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_xor( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_xor( vec_short8 a, vec_short8 b ) { return (vec_short8)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_xor( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_xor( vec_int4 a, vec_int4 b ) { return (vec_int4)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_float4 spu_xor( vec_float4 a, vec_float4 b ) { return (vec_float4)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_ullong2 spu_xor( vec_ullong2 a, vec_ullong2 b ) { return (vec_ullong2)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_llong2 spu_xor( vec_llong2 a, vec_llong2 b ) { return (vec_llong2)si_xor( (qword)a, (qword)b ); }
__SNC_INLINE vec_double2 spu_xor( vec_double2 a, vec_double2 b ) { return (vec_double2)si_xor( (qword)a, (qword)b ); }

__SNC_INLINE vec_uchar16 spu_xor( vec_uchar16 a, unsigned char b ) { return (vec_uchar16)si_xorbi( (qword)a, b ); }
__SNC_INLINE vec_char16 spu_xor( vec_char16 a, signed char b ) { return (vec_char16)si_xorbi( (qword)a, (unsigned char)b ); }
__SNC_INLINE vec_ushort8 spu_xor( vec_ushort8 a, unsigned short b ) { return (vec_ushort8)si_xorhi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_xor( vec_short8 a, signed short b ) { return (vec_short8)si_xorhi( (qword)a, (unsigned short)b ); }
__SNC_INLINE vec_uint4 spu_xor( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_xori( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_xor( vec_int4 a, signed int b ) { return (vec_int4)si_xori( (qword)a, (unsigned int)b ); }

/* 2.10. Shift and Rotate Intrinsics */

/* element-wise rotate left by Bits */
__SNC_INLINE vec_ushort8 spu_rl( vec_ushort8 a, vec_short8 b ) { return (vec_ushort8)si_roth( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_rl( vec_short8 a, vec_short8 b ) { return (vec_short8)si_roth( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_rl( vec_uint4 a, vec_int4 b ) { return (vec_uint4)si_rot( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_rl( vec_int4 a, vec_int4 b ) { return (vec_int4)si_rot( (qword)a, (qword)b ); }

__SNC_INLINE vec_ushort8 spu_rl( vec_ushort8 a, int b ) { return (vec_ushort8)si_rothi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_rl( vec_short8 a, int b ) { return (vec_short8)si_rothi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_rl( vec_uint4 a, int b ) { return (vec_uint4)si_roti( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_rl( vec_int4 a, int b ) { return (vec_int4)si_roti( (qword)a, b ); }

/* element-wise shift right (negative count) */
__SNC_INLINE vec_ushort8 spu_rlmask( vec_ushort8 a, vec_short8 b ) { return (vec_ushort8)si_rothm( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_rlmask( vec_short8 a, vec_short8 b ) { return (vec_short8)si_rothm( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_rlmask( vec_uint4 a, vec_int4 b ) { return (vec_uint4)si_rotm( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_rlmask( vec_int4 a, vec_int4 b ) { return (vec_int4)si_rotm( (qword)a, (qword)b ); }

__SNC_INLINE vec_ushort8 spu_rlmask( vec_ushort8 a, int b ) { return (vec_ushort8)si_rothmi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_rlmask( vec_short8 a, int b ) { return (vec_short8)si_rothmi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_rlmask( vec_uint4 a, int b ) { return (vec_uint4)si_rotmi( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_rlmask( vec_int4 a, int b ) { return (vec_int4)si_rotmi( (qword)a, b ); }

/* element-wise arithmetic shift right (negative count) */
__SNC_INLINE vec_short8 spu_rlmaska( vec_short8 a, vec_short8 b ) { return (vec_short8)si_rotmah( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_rlmaska( vec_ushort8 a, vec_short8 b ) { return (vec_ushort8)si_rotmah( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_rlmaska( vec_int4 a, vec_int4 b ) { return (vec_int4)si_rotma( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_rlmaska( vec_uint4 a, vec_int4 b ) { return (vec_uint4)si_rotma( (qword)a, (qword)b ); }

__SNC_INLINE vec_ushort8 spu_rlmaska( vec_ushort8 a, int b ) { return (vec_ushort8)si_rotmahi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_rlmaska( vec_short8 a, int b ) { return (vec_short8)si_rotmahi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_rlmaska( vec_uint4 a, int b ) { return (vec_uint4)si_rotmai( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_rlmaska( vec_int4 a, int b ) { return (vec_int4)si_rotmai( (qword)a, b ); }

/* shift whole qword right 0-7 bits (negative count) */
__SNC_INLINE vec_uchar16 spu_rlmaskqw( vec_uchar16 a, int count ) { return (vec_uchar16)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_char16 spu_rlmaskqw( vec_char16 a, int count ) { return (vec_char16)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ushort8 spu_rlmaskqw( vec_ushort8 a, int count ) { return (vec_ushort8)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_short8 spu_rlmaskqw( vec_short8 a, int count ) { return (vec_short8)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_uint4 spu_rlmaskqw( vec_uint4 a, int count ) { return (vec_uint4)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_int4 spu_rlmaskqw( vec_int4 a, int count ) { return (vec_int4)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_float4 spu_rlmaskqw( vec_float4 a, int count ) { return (vec_float4)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ullong2 spu_rlmaskqw( vec_ullong2 a, int count ) { return (vec_ullong2)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_llong2 spu_rlmaskqw( vec_llong2 a, int count ) { return (vec_llong2)si_rotqmbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_double2 spu_rlmaskqw( vec_double2 a, int count ) { return (vec_double2)si_rotqmbi( (qword)a, si_from_int( count ) ); }

/* shift whole qword right 0-15 bytes (negative count) */
__SNC_INLINE vec_uchar16 spu_rlmaskqwbyte( vec_uchar16 a, int count ) { return (vec_uchar16)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_char16 spu_rlmaskqwbyte( vec_char16 a, int count ) { return (vec_char16)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ushort8 spu_rlmaskqwbyte( vec_ushort8 a, int count ) { return (vec_ushort8)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_short8 spu_rlmaskqwbyte( vec_short8 a, int count ) { return (vec_short8)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_uint4 spu_rlmaskqwbyte( vec_uint4 a, int count ) { return (vec_uint4)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_int4 spu_rlmaskqwbyte( vec_int4 a, int count ) { return (vec_int4)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_float4 spu_rlmaskqwbyte( vec_float4 a, int count ) { return (vec_float4)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ullong2 spu_rlmaskqwbyte( vec_ullong2 a, int count ) { return (vec_ullong2)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_llong2 spu_rlmaskqwbyte( vec_llong2 a, int count ) { return (vec_llong2)si_rotqmby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_double2 spu_rlmaskqwbyte( vec_double2 a, int count ) { return (vec_double2)si_rotqmby( (qword)a, si_from_int( count ) ); }

/* shift whole qword right 0-128 bits (negative count = 0 (mod 8) ) */
__SNC_INLINE vec_uchar16 spu_rlqwbytebc( vec_uchar16 a, int count ) { return (vec_uchar16)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_char16 spu_rlqwbytebc( vec_char16 a, int count ) { return (vec_char16)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ushort8 spu_rlqwbytebc( vec_ushort8 a, int count ) { return (vec_ushort8)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_short8 spu_rlqwbytebc( vec_short8 a, int count ) { return (vec_short8)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_uint4 spu_rlqwbytebc( vec_uint4 a, int count ) { return (vec_uint4)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_int4 spu_rlqwbytebc( vec_int4 a, int count ) { return (vec_int4)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_float4 spu_rlqwbytebc( vec_float4 a, int count ) { return (vec_float4)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ullong2 spu_rlqwbytebc( vec_ullong2 a, int count ) { return (vec_ullong2)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_llong2 spu_rlqwbytebc( vec_llong2 a, int count ) { return (vec_llong2)si_rotqbybi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_double2 spu_rlqwbytebc( vec_double2 a, int count ) { return (vec_double2)si_rotqbybi( (qword)a, si_from_int( count ) ); }

/* shift whole qword right 0-128 bits (negative count = 0 (mod 8) ) */
__SNC_INLINE vec_uchar16 spu_rlqwbyte( vec_uchar16 a, int count ) { return (vec_uchar16)si_rotqby( (qword)a, si_from_int( count ) ); }  
__SNC_INLINE vec_char16 spu_rlqwbyte( vec_char16 a, int count ) { return (vec_char16)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ushort8 spu_rlqwbyte( vec_ushort8 a, int count ) { return (vec_ushort8)si_rotqby( (qword)a, si_from_int( count ) ); }  
__SNC_INLINE vec_short8 spu_rlqwbyte( vec_short8 a, int count ) { return (vec_short8)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_uint4 spu_rlqwbyte( vec_uint4 a, int count ) { return (vec_uint4)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_int4 spu_rlqwbyte( vec_int4 a, int count ) { return (vec_int4)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_float4 spu_rlqwbyte( vec_float4 a, int count ) { return (vec_float4)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ullong2 spu_rlqwbyte( vec_ullong2 a, int count ) { return (vec_ullong2)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_llong2 spu_rlqwbyte( vec_llong2 a, int count ) { return (vec_llong2)si_rotqby( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_double2 spu_rlqwbyte( vec_double2 a, int count ) { return (vec_double2)si_rotqby( (qword)a, si_from_int( count ) ); }

/* rotate whole qword left 0-127 bits */
__SNC_INLINE vec_uchar16 spu_rlqw( vec_uchar16 a, int count ) { return (vec_uchar16)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_char16 spu_rlqw( vec_char16 a, int count ) { return (vec_char16)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ushort8 spu_rlqw( vec_ushort8 a, int count ) { return (vec_ushort8)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_short8 spu_rlqw( vec_short8 a, int count ) { return (vec_short8)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_uint4 spu_rlqw( vec_uint4 a, int count ) { return (vec_uint4)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_int4 spu_rlqw( vec_int4 a, int count ) { return (vec_int4)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_float4 spu_rlqw( vec_float4 a, int count ) { return (vec_float4)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_ullong2 spu_rlqw( vec_ullong2 a, int count ) { return (vec_ullong2)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_llong2 spu_rlqw( vec_llong2 a, int count ) { return (vec_llong2)si_rotqbi( (qword)a, si_from_int( count ) ); }
__SNC_INLINE vec_double2 spu_rlqw( vec_double2 a, int count ) { return (vec_double2)si_rotqbi( (qword)a, si_from_int( count ) ); }

/* element-wise shift left by bits */
__SNC_INLINE vec_ushort8 spu_sl( vec_ushort8 a, vec_ushort8 b ) { return (vec_ushort8)si_shlh( (qword)a, (qword)b ); }
__SNC_INLINE vec_short8 spu_sl( vec_short8 a, vec_ushort8 b ) { return (vec_short8)si_shlh( (qword)a, (qword)b ); }
__SNC_INLINE vec_uint4 spu_sl( vec_uint4 a, vec_uint4 b ) { return (vec_uint4)si_shl( (qword)a, (qword)b ); }
__SNC_INLINE vec_int4 spu_sl( vec_int4 a, vec_uint4 b ) { return (vec_int4)si_shl( (qword)a, (qword)b ); }
__SNC_INLINE vec_ushort8 spu_sl( vec_ushort8 a, unsigned int b ) { return (vec_ushort8)si_shlhi( (qword)a, b ); }
__SNC_INLINE vec_short8 spu_sl( vec_short8 a, unsigned int b ) { return (vec_short8)si_shlhi( (qword)a, b ); }
__SNC_INLINE vec_uint4 spu_sl( vec_uint4 a, unsigned int b ) { return (vec_uint4)si_shli( (qword)a, b ); }
__SNC_INLINE vec_int4 spu_sl( vec_int4 a, unsigned int b ) { return (vec_int4)si_shli( (qword)a, b ); }

/*  shift whole quadword left by bits */
__SNC_INLINE vec_uchar16 spu_slqw( vec_uchar16 a, unsigned int count ) { return (vec_uchar16)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_char16 spu_slqw( vec_char16 a, unsigned int count ) { return (vec_char16)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ushort8 spu_slqw( vec_ushort8 a, unsigned int count ) { return (vec_ushort8)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_short8 spu_slqw( vec_short8 a, unsigned int count ) { return (vec_short8)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_uint4 spu_slqw( vec_uint4 a, unsigned int count ) { return (vec_uint4)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_int4 spu_slqw( vec_int4 a, unsigned int count ) { return (vec_int4)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_float4 spu_slqw( vec_float4 a, unsigned int count ) { return (vec_float4)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ullong2 spu_slqw( vec_ullong2 a, unsigned int count ) { return (vec_ullong2)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_llong2 spu_slqw( vec_llong2 a, unsigned int count ) { return (vec_llong2)si_shlqbi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_double2 spu_slqw( vec_double2 a, unsigned int count ) { return (vec_double2)si_shlqbi( (qword)a, si_from_uint( count ) ); }

/*  shift whole quadword left by 0-16 bytes */
__SNC_INLINE vec_uchar16 spu_slqwbyte( vec_uchar16 a, unsigned int count ) { return (vec_uchar16)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_char16 spu_slqwbyte( vec_char16 a, unsigned int count ) { return (vec_char16)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ushort8 spu_slqwbyte( vec_ushort8 a, unsigned int count ) { return (vec_ushort8)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_short8 spu_slqwbyte( vec_short8 a, unsigned int count ) { return (vec_short8)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_uint4 spu_slqwbyte( vec_uint4 a, unsigned int count ) { return (vec_uint4)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_int4 spu_slqwbyte( vec_int4 a, unsigned int count ) { return (vec_int4)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_float4 spu_slqwbyte( vec_float4 a, unsigned int count ) { return (vec_float4)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ullong2 spu_slqwbyte( vec_ullong2 a, unsigned int count ) { return (vec_ullong2)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_llong2 spu_slqwbyte( vec_llong2 a, unsigned int count ) { return (vec_llong2)si_shlqby( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_double2 spu_slqwbyte( vec_double2 a, unsigned int count ) { return (vec_double2)si_shlqby( (qword)a, si_from_uint( count ) ); }

/*  shift whole quadword left by 0-128 bits ( count = 0 (mod 8) ) */
__SNC_INLINE vec_uchar16 spu_slqwbytebc( vec_uchar16 a, unsigned int count ) { return (vec_uchar16)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_char16 spu_slqwbytebc( vec_char16 a, unsigned int count ) { return (vec_char16)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ushort8 spu_slqwbytebc( vec_ushort8 a, unsigned int count ) { return (vec_ushort8)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_short8 spu_slqwbytebc( vec_short8 a, unsigned int count ) { return (vec_short8)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_uint4 spu_slqwbytebc( vec_uint4 a, unsigned int count ) { return (vec_uint4)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_int4 spu_slqwbytebc( vec_int4 a, unsigned int count ) { return (vec_int4)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_float4 spu_slqwbytebc( vec_float4 a, unsigned int count ) { return (vec_float4)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_ullong2 spu_slqwbytebc( vec_ullong2 a, unsigned int count ) { return (vec_ullong2)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_llong2 spu_slqwbytebc( vec_llong2 a, unsigned int count ) { return (vec_llong2)si_shlqbybi( (qword)a, si_from_uint( count ) ); }
__SNC_INLINE vec_double2 spu_slqwbytebc( vec_double2 a, unsigned int count ) { return (vec_double2)si_shlqbybi( (qword)a, si_from_uint( count ) ); }

/* 2.11. Control Intrinsics  */

__SNC_INLINE void spu_idisable() { SPU_UNSUPPORTED_ACTION; }
__SNC_INLINE void spu_ienable() { SPU_UNSUPPORTED_ACTION; }

__SNC_INLINE vec_uint4 spu_mffpscr() { return (vec_uint4)si_fscrrd(); }
__SNC_INLINE unsigned int spu_mfspr( int reg ) { return si_to_uint( si_mfspr( reg ) ); }
__SNC_INLINE void spu_mtspr( int reg, unsigned int a ) { si_mtspr( reg, si_from_uint( a ) ); }
__SNC_INLINE void spu_mtfpscr( vec_uint4 a ) { si_fscrwr( (qword)a ); }
__SNC_INLINE void spu_dsync() { si_dsync(); }
__SNC_INLINE void spu_stop( unsigned int type ) { si_stop(); }
__SNC_INLINE void spu_sync() { si_sync(); }
__SNC_INLINE void spu_sync_c() { si_syncc(); }

__SNC_INLINE void spu_mfcdma32( volatile void *ls, unsigned int ea, unsigned int size, unsigned int tagid, unsigned int cmd ) {}
__SNC_INLINE void spu_mfcdma64( volatile void *ls, unsigned int eahi, unsigned int ealow, unsigned int size, unsigned int tagid, unsigned int cmd ) {}
__SNC_INLINE unsigned int spu_mfcstat( unsigned int type ) { return ~0; }

__SNC_INLINE unsigned int spu_readch( unsigned int channel ) { return si_to_uint( si_rdch( channel ) ); }
__SNC_INLINE vec_uint4 spu_readchqw( unsigned int channel ) { return (vec_uint4)si_rdch( channel ); }
__SNC_INLINE unsigned int spu_readchcnt( unsigned int channel ) { return si_to_uint( si_rchcnt( channel ) ); }
__SNC_INLINE void spu_writech( unsigned int channel, unsigned int a )	{ si_wrch( channel, si_from_uint( a ) ); }
__SNC_INLINE void spu_writech( unsigned int channel, signed int a )	{ si_wrch( channel, si_from_int( a ) ); }

__SNC_INLINE void spu_writechqw( unsigned int channel, vec_uchar16 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_char16 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_ushort8 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_short8 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_uint4 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_int4 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_float4 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_ullong2 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_llong2 a ) { return si_wrch( channel, (qword)a ); }
__SNC_INLINE void spu_writechqw( unsigned int channel, vec_double2 a ) { return si_wrch( channel, (qword)a ); }

/* 2.13. Scalar Intrinsics  */

__SNC_INLINE unsigned char spu_extract( vec_uchar16 a, int element ) { return ((unsigned char*)&a)[ element & 15 ]; }
__SNC_INLINE signed char spu_extract( vec_char16 a, int element ) { return ((signed char*)&a)[ element & 15 ]; }
__SNC_INLINE unsigned short spu_extract( vec_ushort8 a, int element ) { return ((unsigned short*)&a)[ element & 7 ]; }
__SNC_INLINE signed short spu_extract( vec_short8 a, int element ) { return ((signed short*)&a)[ element & 7 ]; }
__SNC_INLINE unsigned int spu_extract( vec_uint4 a, int element ) { return ((unsigned int*)&a)[ element & 3 ]; }
__SNC_INLINE signed int spu_extract( vec_int4 a, int element ) { return ((signed int*)&a)[ element & 3 ]; }
__SNC_INLINE unsigned long long spu_extract( vec_ullong2 a, int element ) { return ((unsigned long long*)&a)[ element & 1 ]; }
__SNC_INLINE signed long long spu_extract( vec_llong2 a, int element ) { return ((signed long long*)&a)[ element & 1 ]; }
__SNC_INLINE float spu_extract( vec_float4 a, int element ) { return ((float*)&a)[ element & 3 ]; }
__SNC_INLINE double spu_extract( vec_double2 a, int element ) { return ((double*)&a)[ element & 1 ]; }

__SNC_INLINE vec_uchar16 spu_insert( unsigned char value, vec_uchar16 a, int element ) { ((unsigned char*)&a)[ element & 15 ] = value; return a; }
__SNC_INLINE vec_char16 spu_insert( signed char value, vec_char16 a, int element ) { ((signed char*)&a)[ element & 15 ] = value; return a; }
__SNC_INLINE vec_ushort8 spu_insert( unsigned short value, vec_ushort8 a, int element ) { ((unsigned short*)&a)[ element & 7 ] = value; return a; }
__SNC_INLINE vec_short8 spu_insert( signed short value, vec_short8 a, int element ) { ((signed short*)&a)[ element & 7 ] = value; return a; }
__SNC_INLINE vec_uint4 spu_insert( unsigned int value, vec_uint4 a, int element ) { ((unsigned int*)&a)[ element & 3 ] = value; return a; }
__SNC_INLINE vec_int4 spu_insert( signed int value, vec_int4 a, int element ) { ((signed int*)&a)[ element & 3 ] = value; return a; }
__SNC_INLINE vec_ullong2 spu_insert( unsigned long long value, vec_ullong2 a, int element ) { ((unsigned long long*)&a)[ element & 1 ] = value; return a; }
__SNC_INLINE vec_llong2 spu_insert( signed long long value, vec_llong2 a, int element ) { ((signed long long*)&a)[ element & 1 ] = value; return a; }
__SNC_INLINE vec_float4 spu_insert( float value, vec_float4 a, int element ) { ((float*)&a)[ element & 3 ] = value; return a; }
__SNC_INLINE vec_double2 spu_insert( double value, vec_double2 a, int element ) { ((double*)&a)[ element & 1 ] = value; return a; }

__SNC_INLINE vec_uchar16 spu_promote(  unsigned char a, int element ) { vec_uchar16 b = (vec_uchar16)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_char16 spu_promote(  signed char a, int element ) { vec_char16 b = (vec_char16)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_ushort8 spu_promote(  unsigned short a, int element ) { vec_ushort8 b = (vec_ushort8)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_short8 spu_promote(  signed short a, int element ) { vec_short8 b = (vec_short8)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_uint4 spu_promote(  unsigned int a, int element ) { vec_uint4 b = (vec_uint4)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_int4 spu_promote(  signed int a, int element ) { vec_int4 b = (vec_int4)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_float4 spu_promote(  float a, int element ) { vec_float4 b = (vec_float4)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_ullong2 spu_promote(  unsigned long long a, int element ) { vec_ullong2 b = (vec_ullong2)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_llong2 spu_promote(  signed long long a, int element ) { vec_llong2 b = (vec_llong2)0; return spu_insert( a, b, element ); }
__SNC_INLINE vec_double2 spu_promote(  double a, int element ) { vec_double2 b = (vec_double2)0; return spu_insert( a, b, element ); }

#endif /* !__SPU__ */
#endif /* !_SPU2VMX_SNC_H_ */

