/*  SCE CONFIDENTIAL                          */
/*  PlayStation(R)3 Programmer Tool Runtime Library */
/*  Copyright (C) 2007-2009 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                        */

#ifndef __PPU_ALTIVEC_INTERNALS_H__
#define __PPU_ALTIVEC_INTERNALS_H__

#ifdef __SNC__

#ifndef __SNC_INLINE
  #define __SNC_INLINE static inline __attribute__((always_inline))
#endif

/*
 * This file contains SNC compatible definitions of the builtins used in the GCC 
 * implementation of the Altivec PIM specification.  These are provided to aid 
 * porting GCC code which uses them.
 * 
 * NOTE: These GCC internal intrinsics are not officially supported on GCC or SNC
 *       and are likely to change in the future.  Please use the officially
 *       supported vec_* Altivec intrinsics as documented in the Altivec PIM.
 *
 */

typedef vector unsigned char __V4SI;

__SNC_INLINE vector signed int __builtin_altivec_vaddcuw( vector signed int a, vector signed int b ) { return (vector signed int)vec_addc( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector float __builtin_altivec_vaddfp( vector float a, vector float b ) { return (vector float)vec_add( a, (vector float)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vaddsbs( vector signed char a, vector signed char b ) { return (vector signed char)vec_adds( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vaddshs( vector signed short a, vector signed short b ) { return (vector signed short)vec_adds( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vaddsws( vector signed int a, vector signed int b ) { return (vector signed int)vec_adds( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vaddubm( vector signed char a, vector signed char b ) { return (vector signed char)vec_add( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vaddubs( vector signed char a, vector signed char b ) { return (vector signed char)vec_adds( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vadduhm( vector signed short a, vector signed short b ) { return (vector signed short)vec_add( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vadduhs( vector signed short a, vector signed short b ) { return (vector signed short)vec_adds( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vadduwm( vector signed int a, vector signed int b ) { return (vector signed int)vec_add( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vadduws( vector signed int a, vector signed int b ) { return (vector signed int)vec_adds( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vand( vector signed int a, vector signed int b ) { return (vector signed int)vec_and( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vandc( vector signed int a, vector signed int b ) { return (vector signed int)vec_andc( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vavgsb( vector signed char a, vector signed char b ) { return (vector signed char)vec_avg( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vavgsh( vector signed short a, vector signed short b ) { return (vector signed short)vec_avg( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vavgsw( vector signed int a, vector signed int b ) { return (vector signed int)vec_avg( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vavgub( vector signed char a, vector signed char b ) { return (vector signed char)vec_avg( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vavguh( vector signed short a, vector signed short b ) { return (vector signed short)vec_avg( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vavguw( vector signed int a, vector signed int b ) { return (vector signed int)vec_avg( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector float __builtin_altivec_vcfsx( vector signed int a, const int b ) { return (vector float)vec_ctf( (vector signed int)a, b ); }
__SNC_INLINE vector float __builtin_altivec_vcfux( vector signed int a, const int b ) { return (vector float)vec_ctf( (vector unsigned int)a, b ); }
__SNC_INLINE vector signed int __builtin_altivec_vcmpbfp( vector float a, vector float b ) { return (vector signed int)vec_cmpb( a, (vector float)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vcmpeqfp( vector float a, vector float b ) { return (vector signed int)vec_cmpeq( a, (vector float)b ); }
__SNC_INLINE vector bool char __builtin_altivec_vcmpequb( vector signed char a, vector signed char b ) { return (vector bool char)vec_cmpeq( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector bool short __builtin_altivec_vcmpequh( vector signed short a, vector signed short b ) { return (vector bool short)vec_cmpeq( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector bool int __builtin_altivec_vcmpequw( vector signed int a, vector signed int b ) { return (vector bool int)vec_cmpeq( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vcmpgefp( vector float a, vector float b ) { return (vector signed int)vec_cmpge( a, (vector float)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vcmpgtfp( vector float a, vector float b ) { return (vector signed int)vec_cmpgt( a, (vector float)b ); }
__SNC_INLINE vector bool char __builtin_altivec_vcmpgtsb( vector signed char a, vector signed char b ) { return (vector bool char)vec_cmpgt( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector bool short __builtin_altivec_vcmpgtsh( vector signed short a, vector signed short b ) { return (vector bool short)vec_cmpgt( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector bool int __builtin_altivec_vcmpgtsw( vector signed int a, vector signed int b ) { return (vector bool int)vec_cmpgt( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector bool char __builtin_altivec_vcmpgtub( vector signed char a, vector signed char b ) { return (vector bool char)vec_cmpgt( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector bool short __builtin_altivec_vcmpgtuh( vector signed short a, vector signed short b ) { return (vector bool short)vec_cmpgt( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector bool int __builtin_altivec_vcmpgtuw( vector signed int a, vector signed int b ) { return (vector bool int)vec_cmpgt( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vctsxs( vector float a, unsigned char b ) { return (vector signed int)vec_cts( a, b ); }
__SNC_INLINE vector signed int __builtin_altivec_vctuxs( vector float a, unsigned char b ) { return (vector signed int)vec_ctu( a, b ); }
__SNC_INLINE vector float __builtin_altivec_vexptefp( vector float a ) { return (vector float)vec_expte( a ); }
__SNC_INLINE vector float __builtin_altivec_vlogefp( vector float a ) { return (vector float)vec_loge( a ); }
__SNC_INLINE vector float __builtin_altivec_vmaddfp( vector float a, vector float b, vector float c ) { return (vector float)vec_madd( (vector float)a, (vector float)b, (vector float)c ); }
__SNC_INLINE vector float __builtin_altivec_vmaxfp( vector float a, vector float b ) { return (vector float)vec_max( (vector float)a, (vector float)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vmaxsb( vector signed char a, vector signed char b ) { return (vector signed char)vec_max( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmaxsh( vector signed short a, vector signed short b ) { return (vector signed short)vec_max( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmaxsw( vector signed int a, vector signed int b ) { return (vector signed int)vec_max( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vmaxub( vector signed char a, vector signed char b ) { return (vector signed char)vec_max( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmaxuh( vector signed short a, vector signed short b ) { return (vector signed short)vec_max( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmaxuw( vector signed int a, vector signed int b ) { return (vector signed int)vec_max( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmhaddshs( vector signed short a, vector signed short b, vector signed short c ) { return (vector signed short)vec_madds( (vector signed short)a, (vector signed short)b, (vector signed short)c ); }
__SNC_INLINE vector signed short __builtin_altivec_vmhraddshs( vector signed short a, vector signed short b, vector signed short c ) { return (vector signed short)vec_mradds( (vector signed short)a, (vector signed short)b, (vector signed short)c ); }
__SNC_INLINE vector float __builtin_altivec_vminfp( vector float a, vector float b ) { return (vector float)vec_min( (vector float)a, (vector float)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vminsb( vector signed char a, vector signed char b ) { return (vector signed char)vec_min( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vminsh( vector signed short a, vector signed short b ) { return (vector signed short)vec_min( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vminsw( vector signed int a, vector signed int b ) { return (vector signed int)vec_min( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vminub( vector signed char a, vector signed char b ) { return (vector signed char)vec_min( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vminuh( vector signed short a, vector signed short b ) { return (vector signed short)vec_min( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vminuw( vector signed int a, vector signed int b ) { return (vector signed int)vec_min( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmladduhm( vector signed short a, vector signed short b, vector signed short c ) { return (vector signed short)vec_mladd( (vector unsigned short)a, (vector unsigned short)b, (vector unsigned short)c ); }
__SNC_INLINE vector signed char __builtin_altivec_vmrghb( vector signed char a, vector signed char b ) { return (vector signed char)vec_mergeh( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmrghh( vector signed short a, vector signed short b ) { return (vector signed short)vec_mergeh( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmrghw( vector signed int a, vector signed int b ) { return (vector signed int)vec_mergeh( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vmrglb( vector signed char a, vector signed char b ) { return (vector signed char)vec_mergel( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmrglh( vector signed short a, vector signed short b ) { return (vector signed short)vec_mergel( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmrglw( vector signed int a, vector signed int b ) { return (vector signed int)vec_mergel( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsummbm( vector signed char a, vector signed char b, vector signed int c ) { return (vector signed int)vec_msum( (vector signed char)a, (vector unsigned char)b, (vector signed int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsumshm( vector signed short a, vector signed short b, vector signed int c ) { return (vector signed int)vec_msum( (vector signed short)a, (vector signed short)b, (vector signed int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsumshs( vector signed short a, vector signed short b, vector signed int c ) { return (vector signed int)vec_msums( (vector signed short)a, (vector signed short)b, (vector signed int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsumubm( vector signed char a, vector signed char b, vector signed int c ) { return (vector signed int)vec_msum( (vector unsigned char)a, (vector unsigned char)b, (vector unsigned int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsumuhm( vector signed short a, vector signed short b, vector signed int c ) { return (vector signed int)vec_msum( (vector unsigned short)a, (vector unsigned short)b, (vector unsigned int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vmsumuhs( vector signed short a, vector signed short b, vector signed int c ) { return (vector signed int)vec_msums( (vector unsigned short)a, (vector unsigned short)b, (vector unsigned int)c ); }
__SNC_INLINE vector signed short __builtin_altivec_vmulesb( vector signed char a, vector signed char b ) { return (vector signed short)vec_mule( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmulesh( vector signed short a, vector signed short b ) { return (vector signed int)vec_mule( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmuleub( vector signed char a, vector signed char b ) { return (vector signed short)vec_mule( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmuleuh( vector signed short a, vector signed short b ) { return (vector signed int)vec_mule( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmulosb( vector signed char a, vector signed char b ) { return (vector signed short)vec_mulo( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmulosh( vector signed short a, vector signed short b ) { return (vector signed int)vec_mulo( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vmuloub( vector signed char a, vector signed char b ) { return (vector signed short)vec_mulo( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vmulouh( vector signed short a, vector signed short b ) { return (vector signed int)vec_mulo( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector float __builtin_altivec_vnmsubfp( vector float a, vector float b, vector float c ) { return (vector float)vec_nmsub( (vector float)a, (vector float)b, (vector float)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vnor( vector signed int a, vector signed int b ) { return (vector signed int)vec_nor( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vor( vector signed int a, vector signed int b ) { return (vector signed int)vec_or( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vperm_4si( vector signed int a, vector signed int b, vector signed char c ) { return (vector signed int)vec_perm( (vector signed int)a, (vector signed int)b, (vector unsigned char)c ); }
__SNC_INLINE vector pixel __builtin_altivec_vpkpx( vector signed int a, vector signed int b ) { return (vector pixel)vec_packpx( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vpkshss( vector signed short a, vector signed short b ) { return (vector signed char)vec_packs( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vpkshus( vector signed short a, vector signed short b ) { return (vector signed char)vec_packsu( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vpkswss( vector signed int a, vector signed int b ) { return (vector signed short)vec_packs( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vpkswus( vector signed int a, vector signed int b ) { return (vector signed short)vec_packsu( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vpkuhum( vector signed short a, vector signed short b ) { return (vector signed char)vec_pack( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vpkuhus( vector signed short a, vector signed short b ) { return (vector signed char)vec_packsu( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vpkuwum( vector signed int a, vector signed int b ) { return (vector signed short)vec_pack( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vpkuwus( vector signed int a, vector signed int b ) { return (vector signed short)vec_packsu( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector float __builtin_altivec_vrefp( vector float a ) { return (vector float)vec_re( (vector float)a ); }
__SNC_INLINE vector float __builtin_altivec_vrfim( vector float a ) { return (vector float)vec_floor( (vector float)a ); }
__SNC_INLINE vector float __builtin_altivec_vrfin( vector float a ) { return (vector float)vec_round( (vector float)a ); }
__SNC_INLINE vector float __builtin_altivec_vrfip( vector float a ) { return (vector float)vec_ceil( (vector float)a ); }
__SNC_INLINE vector float __builtin_altivec_vrfiz( vector float a ) { return (vector float)vec_trunc( (vector float)a ); }
__SNC_INLINE vector signed char __builtin_altivec_vrlb( vector signed char a, vector signed char b ) { return (vector signed char)vec_rl( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vrlh( vector signed short a, vector signed short b ) { return (vector signed short)vec_rl( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vrlw( vector signed int a, vector signed int b ) { return (vector signed int)vec_rl( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector float __builtin_altivec_vrsqrtefp( vector float a ) { return (vector float)vec_rsqrte( (vector float)a ); }
__SNC_INLINE vector signed int __builtin_altivec_vsel_4si( vector signed int a, vector signed int b, vector signed int c ) { return (vector signed int)vec_sel( (vector unsigned int)a, (vector unsigned int)b, (vector unsigned int)c ); }
__SNC_INLINE vector signed int __builtin_altivec_vsl( vector signed int a, vector signed int b ) { return (vector signed int)vec_sll( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vslb( vector signed char a, vector signed char b ) { return (vector signed char)vec_sl( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsldoi_4si( vector signed int a, vector signed int b, unsigned char c ) { return (vector signed int)vec_sld( (vector unsigned int)a, (vector unsigned int)b, c ); }
__SNC_INLINE vector signed short __builtin_altivec_vslh( vector signed short a, vector signed short b ) { return (vector signed short)vec_sl( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vslo( vector signed int a, vector signed int b ) { return (vector signed int)vec_slo( (vector unsigned int)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vslw( vector signed int a, vector signed int b ) { return (vector signed int)vec_sl( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vspltb( vector signed char a, unsigned char b ) { return (vector signed char)vec_splat( (vector unsigned char)a, b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsplth( vector signed short a, unsigned char b ) { return (vector signed short)vec_splat( (vector unsigned short)a, b ); }
__SNC_INLINE vector signed char __builtin_altivec_vspltisb( signed char a ) { return (vector signed char)vec_splat_s8( a ); }
__SNC_INLINE vector signed short __builtin_altivec_vspltish( signed char a ) { return (vector signed short)vec_splat_s16( a ); }
__SNC_INLINE vector signed int __builtin_altivec_vspltisw( signed char a ) { return (vector signed int)vec_splat_s32( a ); }
__SNC_INLINE vector signed int __builtin_altivec_vspltw( vector signed int a, unsigned char b ) { return (vector signed int)vec_splat( (vector unsigned int)a, b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsr( vector signed int a, vector signed int b ) { return (vector signed int)vec_srl( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vsrab( vector signed char a, vector signed char b ) { return (vector signed char)vec_sra( (vector signed char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsrah( vector signed short a, vector signed short b ) { return (vector signed short)vec_sra( (vector signed short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsraw( vector signed int a, vector signed int b ) { return (vector signed int)vec_sra( (vector signed int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vsrb( vector signed char a, vector signed char b ) { return (vector signed char)vec_sr( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsrh( vector signed short a, vector signed short b ) { return (vector signed short)vec_sr( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsro( vector signed int a, vector signed int b ) { return (vector signed int)vec_sro( (vector unsigned int)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsrw( vector signed int a, vector signed int b ) { return (vector signed int)vec_sr( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsubcuw( vector signed int a, vector signed int b ) { return (vector signed int)vec_subc( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector float __builtin_altivec_vsubfp( vector float a, vector float b ) { return (vector float)vec_sub( (vector float)a, (vector float)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vsubsbs( vector signed char a, vector signed char b ) { return (vector signed char)vec_subs( (vector signed char)a, (vector signed char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsubshs( vector signed short a, vector signed short b ) { return (vector signed short)vec_subs( (vector signed short)a, (vector signed short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsubsws( vector signed int a, vector signed int b ) { return (vector signed int)vec_subs( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vsububm( vector signed char a, vector signed char b ) { return (vector signed char)vec_sub( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed char __builtin_altivec_vsububs( vector signed char a, vector signed char b ) { return (vector signed char)vec_subs( (vector unsigned char)a, (vector unsigned char)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsubuhm( vector signed short a, vector signed short b ) { return (vector signed short)vec_sub( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed short __builtin_altivec_vsubuhs( vector signed short a, vector signed short b ) { return (vector signed short)vec_subs( (vector unsigned short)a, (vector unsigned short)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsubuwm( vector signed int a, vector signed int b ) { return (vector signed int)vec_sub( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsubuws( vector signed int a, vector signed int b ) { return (vector signed int)vec_subs( (vector unsigned int)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsum2sws( vector signed int a, vector signed int b ) { return (vector signed int)vec_sum2s( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsum4sbs( vector signed char a, vector signed int b ) { return (vector signed int)vec_sum4s( (vector signed char)a, (vector signed int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsum4shs( vector signed short a, vector signed int b ) { return (vector signed int)vec_sum4s( (vector signed short)a, (vector signed int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsum4ubs( vector signed char a, vector signed int b ) { return (vector signed int)vec_sum4s( (vector unsigned char)a, (vector unsigned int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vsumsws( vector signed int a, vector signed int b ) { return (vector signed int)vec_sums( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed int __builtin_altivec_vupkhpx( vector signed short a ) { return (vector signed int)vec_unpackh( (vector pixel)a ); }
__SNC_INLINE vector signed short __builtin_altivec_vupkhsb( vector signed char a ) { return (vector signed short)vec_unpackh( (vector signed char)a ); }
__SNC_INLINE vector signed int __builtin_altivec_vupkhsh( vector signed short a ) { return (vector signed int)vec_unpackh( (vector signed short)a ); }
__SNC_INLINE vector signed int __builtin_altivec_vupklpx( vector signed short a ) { return (vector signed int)vec_unpackl( (vector pixel)a ); }
__SNC_INLINE vector signed short __builtin_altivec_vupklsb( vector signed char a ) { return (vector signed short)vec_unpackl( (vector signed char)a ); }
__SNC_INLINE vector signed int __builtin_altivec_vupklsh( vector signed short a ) { return (vector signed int)vec_unpackl( (vector signed short)a ); }
__SNC_INLINE vector signed int __builtin_altivec_vxor( vector signed int a, vector signed int b ) { return (vector signed int)vec_xor( (vector signed int)a, (vector signed int)b ); }
__SNC_INLINE vector signed char __builtin_altivec_lvebx( long long offset, void *p ) { return (vector signed char)vec_lde( offset, (unsigned char *)p ); }
__SNC_INLINE vector signed short __builtin_altivec_lvehx( long long offset, void *p ) { return (vector signed short)vec_lde( offset, (unsigned short *)p ); }
__SNC_INLINE vector signed int __builtin_altivec_lvewx( long long offset, void *p ) { return (vector signed int)vec_lde( offset, (unsigned int *)p ); }
__SNC_INLINE vector float __builtin_altivec_lvlx( long long offset, void *p ) { return (vector float)vec_lvlx( offset, (float *)p ); }
__SNC_INLINE vector float __builtin_altivec_lvlxl( long long offset, void *p ) { return (vector float)vec_lvlxl( offset, (float *)p ); }
__SNC_INLINE vector float __builtin_altivec_lvrx( long long offset, void *p ) { return (vector float)vec_lvrx( offset, (float *)p ); }
__SNC_INLINE vector float __builtin_altivec_lvrxl( long long offset, void *p ) { return (vector float)vec_lvrxl( offset, (float *)p ); }
__SNC_INLINE vector signed char __builtin_altivec_lvsl( long long offset, void *p ) { return (vector signed char)vec_lvsl( offset, (unsigned char *)p ); }
__SNC_INLINE vector signed char __builtin_altivec_lvsr( long long offset, void *p ) { return (vector signed char)vec_lvsr( offset, (unsigned char *)p ); }
__SNC_INLINE vector signed char __builtin_altivec_lvx( long long offset, void *p ) { return (vector signed char)*(vector unsigned char*)( (unsigned char *)p + offset ); }
__SNC_INLINE vector signed char __builtin_altivec_lvxl( long long offset, void *p ) { return (vector signed char)vec_ldl( offset, (unsigned char *)p ); }
__SNC_INLINE void __builtin_altivec_stvebx( vector signed char a, long long offset, void *p ) { vec_ste( (vector signed char)a, offset, (signed char*)p ); }
__SNC_INLINE void __builtin_altivec_stvehx( vector signed short a, long long offset, void *p ) { vec_ste( (vector signed short)a, offset, (signed short*)p ); }
__SNC_INLINE void __builtin_altivec_stvewx( vector signed int a, long long offset, void *p ) { vec_ste( (vector signed int)a, offset, (signed int*)p ); }
__SNC_INLINE void __builtin_altivec_stvlx( vector signed char a, long long offset, void *p ) { vec_stvlx( a, offset, (vector signed char*)p ); }
__SNC_INLINE void __builtin_altivec_stvlxl( vector signed char a, long long offset, void *p ) { vec_stvlxl( a, offset, (vector signed char*)p ); }
__SNC_INLINE void __builtin_altivec_stvrx( vector signed char a, long long offset, void *p ) { vec_stvrx( a, offset, (vector signed char*)p ); }
__SNC_INLINE void __builtin_altivec_stvrxl( vector signed char a, long long offset, void *p ) { vec_stvrxl( a, offset, (vector signed char*)p ); }
__SNC_INLINE void __builtin_altivec_stvx( vector signed int a, long long offset, void *p ) { vector signed int *addr = (vector signed int *)( (signed char *)p + offset ); *addr = a; }
__SNC_INLINE void __builtin_altivec_stvxl( vector signed int a, long long offset, void *p ) { vec_stl( a, offset, (vector signed int*)p ); }

#define __CR6_EQ     0
#define __CR6_EQ_REV 1
#define __CR6_LT     2
#define __CR6_LT_REV 3

#endif /* __SNC__ */

#endif /* __PPU_ALTIVEC_INTERNALS_H__ */

/* eof ppu_altivec_internals.h */
