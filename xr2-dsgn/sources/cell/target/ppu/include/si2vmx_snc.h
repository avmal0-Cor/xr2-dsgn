/*  SCE CONFIDENTIAL                          */
/*  PlayStation(R)3 Programmer Tool Runtime Library */
/*  Copyright (C) 2007-2008 Sony Computer Entertainment Inc.   */
/*  All Rights Reserved.                        */

#ifndef _SI2VMX_SNC_H_
#define _SI2VMX_SNC_H_	1

#ifndef __SPU__

#ifndef __VEC_TYPES_SNC_H__
#include <vec_types_snc.h>
#endif

#ifndef SPU_HALT_ACTION
  #define SPU_HALT_ACTION __builtin_trap()
#endif

#ifndef SPU_STOP_ACTION
  #define SPU_STOP_ACTION __builtin_trap()
#endif

#ifndef SPU_UNSUPPORTED_ACTION
  #define SPU_UNSUPPORTED_ACTION __builtin_trap()
#endif

#undef __SNC_INLINE
#define __SNC_INLINE static inline __attribute__( ( always_inline ) )

__SNC_INLINE qword si_a( qword a, qword b ) { return (qword)vec_add( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_absdb( qword a, qword b ) { return (qword)vec_sel( vec_sub( (vec_uchar16)a, (vec_uchar16)b ), vec_sub( (vec_uchar16)b, (vec_uchar16)a ), vec_cmpgt( (vec_uchar16)b, (vec_uchar16)a ) ); }
__SNC_INLINE qword si_addx( qword a, qword b, qword c ) { return (qword)vec_add( vec_add( (vec_int4)a, (vec_int4)b ), vec_and( (vec_int4)c, (vec_int4)1 ) ); }
__SNC_INLINE qword si_ah( qword a, qword b ) { return (qword)vec_add( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_ahi( qword a, short b ) { return (qword)vec_add( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_ai( qword a, int b ) { return (qword)vec_add( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_and( qword a, qword b ) { return (qword)vec_and( a, b ); }
__SNC_INLINE qword si_andbi( qword a, signed char b ) { return (qword)vec_and( (vec_char16)a, (vec_char16)b ); }
__SNC_INLINE qword si_andc( qword a, qword b ) { return (qword)vec_andc( a, b ); }
__SNC_INLINE qword si_andhi( qword a, signed short b ) { return (qword)vec_and( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_andi( qword a, signed int b )  { return (qword)vec_and( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_avgb( qword a, qword b ) { return (qword)vec_avg( (vec_uchar16)a, (vec_uchar16)b ); }
__SNC_INLINE qword si_bg( qword a, qword b ) { return (qword)vec_subc( (vec_uint4)b, (vec_uint4)a ); }
__SNC_INLINE qword si_bgx( qword a, qword b, qword c ) { vec_uint4 bgta = (vec_uint4)vec_cmpgt( (vec_uint4)b, (vec_uint4)a ); return (qword)vec_and( vec_or( bgta, vec_and( vec_cmpeq( (vec_uint4)a, (vec_uint4)b ), (vec_uint4)c ) ), (vec_uint4)1 ); }
__SNC_INLINE qword si_cg( qword a, qword b ) { return (qword)vec_addc( (vec_uint4)a, (vec_uint4)b ); }
__SNC_INLINE qword si_cgx( qword a, qword b, qword c ) { vec_uint4 cy = vec_and( (vec_uint4)a, (vec_uint4)1 ); return (qword)vec_or( vec_addc( (vec_uint4)a, cy ), vec_addc( vec_add( (vec_uint4)a, cy ), (vec_uint4)b ) ); }

__SNC_INLINE qword si_cbd( qword a, int imm ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( lvsr, (vec_uchar16)3, vec_cmpeq( lvsl, vec_and( vec_splat( vec_add( (vec_uchar16)a, (vec_uchar16)(imm&15) ), 3 ), (qword)15 ) ) ); }
__SNC_INLINE qword si_cbx( qword a, qword b ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( lvsr, (vec_uchar16)3, vec_cmpeq( lvsl, vec_and( vec_splat( vec_add( (vec_uchar16)a, (vec_uchar16)b ), 3 ), (qword)15 ) ) ); }
__SNC_INLINE qword si_chd( qword a, int imm ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( (vec_ushort8)lvsr, vec_splat( (vec_ushort8)lvsl, 1 ), (vec_ushort8)vec_cmpeq( vec_and( (vec_ushort8)lvsl, (vec_ushort8)14 ), vec_and( vec_splat( vec_add( (vec_ushort8)a, (vec_ushort8)(imm&15) ), 1 ), (vec_ushort8)14 ) ) ); }
__SNC_INLINE qword si_chx( qword a, qword b ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( (vec_ushort8)lvsr, vec_splat( (vec_ushort8)lvsl, 1 ), (vec_ushort8)vec_cmpeq( vec_and( (vec_ushort8)lvsl, (vec_ushort8)14 ), vec_and( vec_splat( vec_add( (vec_ushort8)a, (vec_ushort8)b ), 1 ), (vec_ushort8)14 ) ) ); }
__SNC_INLINE qword si_cwd( qword a, int imm ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( (vec_uint4)lvsr, vec_splat( (vec_uint4)lvsl, 0 ), (vec_uint4)vec_cmpeq( vec_and( (vec_uint4)lvsl, (vec_uint4)12 ), vec_and( vec_splat( vec_add( (vec_uint4)a, (vec_uint4)(imm&15) ), 0 ), (vec_uint4)12 ) ) ); }
__SNC_INLINE qword si_cwx( qword a, qword b ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( (vec_uint4)lvsr, vec_splat( (vec_uint4)lvsl, 0 ), (vec_uint4)vec_cmpeq( vec_and( (vec_uint4)lvsl, (vec_uint4)12 ), vec_and( vec_splat( vec_add( (vec_uint4)a, (vec_uint4)b ), 0 ), (vec_uint4)12 ) ) ); }
__SNC_INLINE qword si_cdd( qword a, int imm ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( vec_and( (vec_uchar16)lvsl, (vec_uchar16)7 ), lvsr, vec_sra( vec_sl( vec_xor( (vec_uchar16)lvsl, (vec_uchar16)vec_splat( vec_add( (vec_uchar16)a, (vec_uchar16)(imm&15) ), 0 ) ), (vec_uchar16)-4 ), (vec_uchar16)7 ) ); }
__SNC_INLINE qword si_cdx( qword a, qword b ) { qword lvsr = vec_lvsr( 0, (int*)0 ), lvsl = vec_lvsl( 0, (int*)0 ); return (qword)vec_sel( vec_and( (vec_uchar16)lvsl, (vec_uchar16)7 ), lvsr, vec_sra( vec_sl( vec_xor( (vec_uchar16)lvsl, (vec_uchar16)vec_splat( vec_add( (vec_uchar16)a, (vec_uchar16)b ), 0 ) ), (vec_uchar16)-4 ), (vec_uchar16)7 ) ); }

__SNC_INLINE qword si_ceq( qword a, qword b ) { return (qword)vec_cmpeq( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_ceqb( qword a, qword b ) { return (qword)vec_cmpeq( (vec_char16)a, (vec_char16)b ); }
__SNC_INLINE qword si_ceqbi( qword a, signed char b ) { return (qword)vec_cmpeq( (vec_char16)a, (vec_char16)b ); }
__SNC_INLINE qword si_ceqh( qword a, qword b ) { return (qword)vec_cmpeq( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_ceqhi( qword a, signed short b ) { return (qword)vec_cmpeq( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_ceqi( qword a, signed int b ) { return (qword)vec_cmpeq( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_cflts( qword a, int b ) { return (qword)vec_cts( (vector float)a, b ); }
__SNC_INLINE qword si_cfltu( qword a, int b ) { return (qword)vec_ctu( (vector float)a, b ); }

__SNC_INLINE qword si_cgt( qword a, qword b ) { return (qword)vec_cmpgt( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_cgtb( qword a, qword b ) { return (qword)vec_cmpgt( (vec_char16)a, (vec_char16)b ); }
__SNC_INLINE qword si_cgtbi( qword a, signed char b ) { return (qword)vec_cmpgt( (vec_char16)a, (vec_char16)b ); }
__SNC_INLINE qword si_cgth( qword a, qword b ) { return (qword)vec_cmpgt( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_cgthi( qword a, signed short b ) { return (qword)vec_cmpgt( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_cgti( qword a, signed int b ) { return (qword)vec_cmpgt( (vec_int4)a, (vec_int4)b ); }
__SNC_INLINE qword si_clgt( qword a, qword b ) { return (qword)vec_cmpgt( (vec_uint4)a, (vec_uint4)b ); }
__SNC_INLINE qword si_clgtb( qword a, qword b ) { return (qword)vec_cmpgt( (vec_uchar16)a, (vec_uchar16)b ); }

__SNC_INLINE qword si_clgtbi( qword a, unsigned char b ) { return (qword)vec_cmpgt( (vec_uchar16)a, (vec_uchar16)b ); }
__SNC_INLINE qword si_clgth( qword a, qword b ) { return (qword)vec_cmpgt( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_clgthi( qword a, unsigned short b ) { return (qword)vec_cmpgt( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_clgti( qword a, unsigned int b ) { return (qword)vec_cmpgt( (vec_uint4)a, (vec_uint4)b ); }

__SNC_INLINE qword si_clz( qword a ) { vec_uchar16 add = (vec_uchar16)(vec_uint4)( 0x00081018 ); vec_uchar16 table = (vec_uchar16)( 32, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 ); vec_uint4 bits8 = (vec_uint4)vec_add( add, vec_min( vec_add( vec_perm( table, table, (vec_uchar16)a ), (vec_uchar16)4 ), vec_perm( table, table, vec_sr( (vec_uchar16)a, (vec_uchar16)4 ) ) ) ); vec_ushort8 bits16 = (vec_ushort8)vec_min( (vec_uchar16)bits8, (vec_uchar16)vec_sra( (vec_short8)bits8, (vec_ushort8)8 ) ); return (qword)vec_min( (vec_uchar16)bits16, (vec_uchar16)vec_sra( (vec_int4)bits16, (vec_uint4)-16 ) ); }
__SNC_INLINE qword si_cntb( qword a ) { vec_uchar16 table = (vec_uchar16)( 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 ); return (qword)vec_add( vec_perm( table, table, (vec_uchar16)a ), vec_perm( table, table, (vec_uchar16)vec_sr( (vec_uint4)a, (vec_uint4)4 ) ) ); }

__SNC_INLINE qword si_csflt( qword a, int b ) { return (qword)vec_ctf( (vec_int4)a, b ); }
__SNC_INLINE qword si_cuflt( qword a, int b ) { return (qword)vec_ctf( (vec_uint4)a, b ); }

__SNC_INLINE qword si_dfa( qword a, qword b ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1]; qword result; ((double*)(void*)&result)[0] = a0 + b0; ((double*)(void*)&result)[1] = a1 + b1; return result; }
__SNC_INLINE qword si_dfm( qword a, qword b ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1]; qword result; ((double*)(void*)&result)[0] = a0 * b0; ((double*)(void*)&result)[1] = a1 * b1; return result; }
__SNC_INLINE qword si_dfma( qword a, qword b, qword c ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], c0 = ((double*)(void*)&c)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1], c1 = ((double*)(void*)&c)[1]; qword result; ((double*)(void*)&result)[0] = a0 * b0 + c0; ((double*)(void*)&result)[1] = a1 * b1 + c1; return result; }
__SNC_INLINE qword si_dfms( qword a, qword b, qword c ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], c0 = ((double*)(void*)&c)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1], c1 = ((double*)(void*)&c)[1]; qword result; ((double*)(void*)&result)[0] = a0 * b0 - c0; ((double*)(void*)&result)[1] = a1 * b1 - c1; return result; }
__SNC_INLINE qword si_dfnma( qword a, qword b, qword c ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], c0 = ((double*)(void*)&c)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1], c1 = ((double*)(void*)&c)[1]; qword result; ((double*)(void*)&result)[0] = -(a0 * b0 + c0); ((double*)(void*)&result)[1] = -(a1 * b1 + c1); return result; }
__SNC_INLINE qword si_dfnms( qword a, qword b, qword c ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], c0 = ((double*)(void*)&c)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1], c1 = ((double*)(void*)&c)[1]; qword result; ((double*)(void*)&result)[0] = c0 - a0 * b0; ((double*)(void*)&result)[1] = c1 - a1 * b1; return result; }
__SNC_INLINE qword si_dfs( qword a, qword b ) { double a0 = ((double*)(void*)&a)[0], b0 = ((double*)(void*)&b)[0], a1 = ((double*)(void*)&a)[1], b1 = ((double*)(void*)&b)[1]; qword result; ((double*)(void*)&result)[0] = a0 - b0; ((double*)(void*)&result)[1] = a1 - b1; return result; }

__SNC_INLINE qword si_eqv( qword a, qword b ) { qword tmp = vec_xor( a, b ); return vec_nor( tmp, tmp ); }

__SNC_INLINE qword si_fa( qword a, qword b ) { return (qword)vec_add( (vec_float4)a, (vec_float4)b ); }

__SNC_INLINE qword si_fceq( qword a, qword b ) { return (qword)vec_cmpeq( (vec_float4)a, (vec_float4)b ); }
__SNC_INLINE qword si_fcgt( qword a, qword b ) { return (qword)vec_cmpgt( (vec_float4)a, (vec_float4)b ); }
__SNC_INLINE qword si_fcmeq( qword a, qword b ) { return (qword)vec_cmpeq( vec_sl( (vec_uint4)a, (vec_uint4)1 ), vec_sl( (vec_uint4)b, (vec_uint4)1 ) ); }
__SNC_INLINE qword si_fcmgt( qword a, qword b ) { return (qword)vec_cmpgt( vec_sl( (vec_uint4)a, (vec_uint4)1 ), vec_sl( (vec_uint4)b, (vec_uint4)1 ) ); }

__SNC_INLINE qword si_fesd( qword a ) { double result[ 2 ]; result[ 0 ] = ((float*)(void*)&a)[ 0 ]; result[ 1 ] = ((float*)(void*)&a)[ 2 ]; return *(qword*)(void*)result; }
__SNC_INLINE qword si_fi( qword a, qword b ) { return b; }

__SNC_INLINE qword si_fm( qword a, qword b ) { return (qword)vec_madd( (vec_float4)a, (vec_float4)b, (vec_float4)0 ); }
__SNC_INLINE qword si_fma( qword a, qword b, qword c ) { return (qword)vec_madd( (vec_float4)a, (vec_float4)b, (vec_float4)c ); }
__SNC_INLINE qword si_fms( qword a, qword b, qword c ) { return (qword)vec_madd( (vec_float4)a, (vec_float4)b, (vec_float4)vec_xor( (vec_int4)c, (vec_int4)0x80000000 ) ); }
__SNC_INLINE qword si_fnms( qword a, qword b, qword c ) { return (qword)vec_nmsub( (vec_float4)a, (vec_float4)b, (vec_float4)c ); }

__SNC_INLINE qword si_frds( qword a ) { float result[ 4 ]; result[ 0 ] = ((double*)(void*)&a)[ 0 ]; result[ 2 ] = ((double*)(void*)&a)[ 1 ]; result[ 1 ] = result[ 3 ] = 0; return *(qword*)(void*)result; }
__SNC_INLINE qword si_frest( qword a ) { return (qword)vec_re( (vec_float4)a ); }
__SNC_INLINE qword si_frsqest( qword a ) { return (qword)vec_rsqrte( (vec_float4)a ); }

__SNC_INLINE qword si_fs( qword a, qword b ) { return (qword)vec_sub( (vec_float4)a, (vec_float4)b ); }
__SNC_INLINE qword si_fscrrd() { return (qword)0; }
__SNC_INLINE qword si_fsm( qword a ) { vec_int4 mask = (vec_int4)( 8, 4, 2, 1 ); return (qword)vec_cmpeq( mask, vec_and( vec_splat( (vec_int4)a, 0 ), mask ) ); }
__SNC_INLINE qword si_fsmb( qword a ) { vec_uchar16 input = vec_pack( (vec_ushort8)vec_splat( (vec_uchar16)a, 2 ), (vec_ushort8)vec_splat( (vec_uchar16)a, 3 ) ); vec_uchar16 mask = vec_sr( (vec_uchar16)0x80, (vec_uchar16)vec_lvsl( 0, (int*)0 ) ); return (qword)vec_cmpeq( mask, vec_and( input, mask ) ); }
__SNC_INLINE qword si_fsmbi( const unsigned short a ) { return (qword)(vec_uchar16)( ( a & 0x8000 ) ? 255 : 0, ( a & 0x4000 ) ? 255 : 0, ( a & 0x2000 ) ? 255 : 0, ( a & 0x1000 ) ? 255 : 0, ( a & 0x800 ) ? 255 : 0, ( a & 0x400 ) ? 255 : 0, ( a & 0x200 ) ? 255 : 0, ( a & 0x100 ) ? 255 : 0, ( a & 0x80 ) ? 255 : 0, ( a & 0x40 ) ? 255 : 0, ( a & 0x20 ) ? 255 : 0, ( a & 0x10 ) ? 255 : 0, ( a & 8 ) ? 255 : 0, ( a & 4 ) ? 255 : 0, ( a & 2 ) ? 255 : 0, ( a & 1 ) ? 255 : 0 ); }
__SNC_INLINE qword si_fsmh( qword a ) { vec_ushort8 mask = (vec_ushort8)( 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 ); return (qword)vec_cmpeq( mask, vec_and( vec_splat( (vec_ushort8)a, 1 ), mask ) ); }
__SNC_INLINE qword si_gb( qword a ) { vec_uint4 shifts = vec_sub( (vec_uint4)3, vec_sr( (vec_uint4)vec_lvsl( 0, (int*)0 ), (vec_uint4)2 ) ), bits = vec_sl( vec_and( (vec_uint4)a, (vec_uint4)1 ), shifts ); return (qword)vec_sld( vec_sums( (vec_int4)bits, (vec_int4)0 ), (vec_int4)0, 12 ); }
__SNC_INLINE qword si_gbh( qword a ) { vec_ushort8 shifts = vec_sub( (vec_ushort8)7, vec_sr( (vec_ushort8)vec_lvsl( 0, (int*)0 ), (vec_ushort8)1 ) ), bits = vec_sl( vec_and( (vec_ushort8)a, (vec_ushort8)1 ), shifts ); vec_uint4 sum4s = vec_sum4s( (vec_uchar16)bits, (vec_uint4)0 ); return (qword)vec_sld( vec_sums( (vec_int4)sum4s, (vec_int4)0 ), (vec_int4)0, 12 ); }
__SNC_INLINE qword si_gbb( qword a ) { vec_uchar16 shifts = vec_sub( (vec_uchar16)15, (vec_uchar16)vec_lvsl( 0, (int*)0 ) ), bits = vec_sl( vec_and( (vec_uchar16)a, (vec_uchar16)1 ), shifts ); vec_uint4 sum4s = vec_sl( vec_sum4s( bits, (vec_uint4)0 ), vec_and( (vec_uint4)shifts, (vec_uint4)8 ) ); return (qword)vec_sld( vec_sums( (vec_int4)sum4s, (vec_int4)0 ), (vec_int4)0, 12 ); }

__SNC_INLINE qword si_il( signed short imm ) { return (qword)(vec_int4)imm; }
__SNC_INLINE qword si_ila( unsigned int imm ) { return (qword)(vec_uint4)imm; }
__SNC_INLINE qword si_ilh( signed short imm ) { return (qword)(vec_short8)imm; }
__SNC_INLINE qword si_ilhu( signed short imm ) { return (qword)(vec_uint4)(imm<<16); }
__SNC_INLINE qword si_iohl( qword a, unsigned short imm ) { return (qword)vec_or( a, (qword)(vec_uint4)imm ); }
__SNC_INLINE qword si_lqa( unsigned int imm ) { return *(qword*)imm; }
__SNC_INLINE qword si_lqd( qword a, unsigned int imm ) { return *(qword*)( *(unsigned*)(void*)&a + imm ); }
__SNC_INLINE qword si_lqr( unsigned int imm ) { return *(qword*)imm; }
__SNC_INLINE qword si_lqx( qword a, qword b ) { return *(qword*)( *(unsigned*)(void*)&a + *(unsigned*)(void*)&b ); }
__SNC_INLINE qword si_mfspr( int x ) { return (qword)0; }

__SNC_INLINE qword si_mpy( qword a, qword b ) { return (qword)vec_mulo( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_mpyu( qword a, qword b ) { return (qword)vec_mulo( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_mpya( qword a, qword b, qword c ) { return (qword)vec_add( vec_mulo( (vec_short8)a, (vec_short8)b ), (vec_int4)c ); }
__SNC_INLINE qword si_mpyh( qword a, qword b ) { return (qword)vec_sl( vec_mule( (vec_ushort8)a, vec_sld( (vec_ushort8)b, (vec_ushort8)b, 2 ) ), (vec_uint4)-16 ); }
__SNC_INLINE qword si_mpyhh( qword a, qword b ) { return (qword)vec_mule( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_mpyhhu( qword a, qword b ) { return (qword)vec_mule( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_mpyhha( qword a, qword b, qword c ) { return (qword)vec_add( vec_mule( (vec_short8)a, (vec_short8)b ), (vec_int4)c ); }
__SNC_INLINE qword si_mpyhhau( qword a, qword b, qword c ) { return (qword)vec_add( vec_mule( (vec_ushort8)a, (vec_ushort8)b ), (vec_uint4)c ); }
__SNC_INLINE qword si_mpyi( qword a, short b ) { return (qword)vec_mulo( (vec_short8)a, (vec_short8)b ); }
__SNC_INLINE qword si_mpyui( qword a, unsigned short b ) { return (qword)vec_mulo( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_mpys( qword a, qword b ) { return (qword)vec_sra( vec_mulo( (vec_short8)a, (vec_short8)b ), (vec_uint4)-16 ); }

__SNC_INLINE qword si_nand( qword a, qword b ) { a = vec_and( a, b ); return (qword)vec_nor( a, a ); }
__SNC_INLINE qword si_nor( qword a, qword b ) { return (qword)vec_nor( a, b ); }

__SNC_INLINE qword si_or( qword a, qword b ) { return (qword)vec_or( a, b ); }
__SNC_INLINE qword si_orbi( qword a, unsigned char b ) { return (qword)vec_or( (vec_uchar16)a, (vec_uchar16)b ); }
__SNC_INLINE qword si_orc( qword a, qword b ) { return (qword)vec_or( a, vec_nor( b, b ) ); }
__SNC_INLINE qword si_orhi( qword a, unsigned short b ) { return (qword)vec_or( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_ori( qword a, unsigned int b ) { return (qword)vec_or( (vec_uint4)a, (vec_uint4)b ); }

__SNC_INLINE qword si_orx( qword a )  { vec_uint4 x = vec_or( vec_mergel( (vec_uint4)a, (vec_uint4)0 ), vec_mergeh( (vec_uint4)a, (vec_uint4)0 ) ); return (qword)vec_or( vec_mergel( x, (vec_uint4)0 ), vec_mergeh( x, (vec_uint4)0 ) ); }

__SNC_INLINE qword si_rchcnt( int channel ) { return (qword)0; }
__SNC_INLINE qword si_rdch( int channel ) { return (qword)0; }

__SNC_INLINE qword si_rot( qword a, qword b ) { return (qword)vec_rl( (vec_uint4)a, (vec_uint4)b ); }
__SNC_INLINE qword si_rotm( qword a, qword b ) { vec_uint4 minusb = vec_and( vec_sub( (vec_uint4)0, (vec_uint4)b ), (vec_uint4)63 ); return (qword)vec_sel( vec_sr( (vec_uint4)a, (vec_uint4)minusb ), (vec_uint4)0, vec_cmpgt( (vec_uint4)minusb, (vec_uint4)31 ) ); }
__SNC_INLINE qword si_rotma( qword a, qword b ) { vec_uint4 minusb = vec_and( vec_sub( (vec_uint4)0, (vec_uint4)b ), (vec_uint4)63 ); return (qword)vec_sra( (vec_uint4)a, vec_min( (vec_uint4)minusb, (vec_uint4)31 ) ); }

__SNC_INLINE qword si_roth( qword a, qword b ) { return (qword)vec_rl( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_rothm( qword a, qword b ) { vec_ushort8 minusb = vec_and( vec_sub( (vec_ushort8)0, (vec_ushort8)b ), (vec_ushort8)31 ); return (qword)vec_sel( vec_sr( (vec_ushort8)a, (vec_ushort8)minusb ), (vec_ushort8)0, vec_cmpgt( (vec_ushort8)minusb, (vec_ushort8)15 ) ); }
__SNC_INLINE qword si_rotmah( qword a, qword b ) { vec_ushort8 minusb = vec_and( vec_sub( (vec_ushort8)0, (vec_ushort8)b ), (vec_ushort8)31 ); return (qword)vec_sra( (vec_short8)a, vec_min( (vec_ushort8)minusb, (vec_ushort8)15 ) ); }

__SNC_INLINE qword si_roti( qword a, int b ) { return (qword)vec_rl( (vec_uint4)a, (vec_uint4)(b&31) ); }
__SNC_INLINE qword si_rotmi( qword a, int b ) { return (-b&63) >= 32 ? (qword)0 : (qword)vec_sr( (vec_int4)a, (vec_uint4)(-b&63) ); }
__SNC_INLINE qword si_rotmai( qword a, int b ) { return (-b&63) >= 32 ? (qword)vec_sra( (vec_int4)a, (vec_uint4)31 ) : (qword)vec_sra( (vec_int4)a, (vec_uint4)(-b&63) ); }

__SNC_INLINE qword si_rothi( qword a, int b ) { return (qword)vec_rl( (vec_ushort8)a, (vec_ushort8)(b&15)); }
__SNC_INLINE qword si_rothmi( qword a, int b ) { return (-b&31) >= 16 ? (qword)0 : (qword)vec_sr( (vec_ushort8)a, (vec_ushort8)(-b&31) ); }
__SNC_INLINE qword si_rotmahi( qword a, int b ) { return (qword)vec_sra( (vec_short8)a, (vec_ushort8)((-b&31) >= 16 ? 15 : (-b&31) ) ); }

__SNC_INLINE qword si_rotqbi( qword a, qword count ) { vec_uchar16 sh = vec_splat( (vec_uchar16)count, 3 ), h = vec_sll( (vec_uchar16)a, sh ), l = vec_sll( vec_sld( (vec_uchar16)a, (vec_uchar16)a, 8 ), sh ); return (qword)vec_or( h, vec_sld( l, l, 8 ) ); }
__SNC_INLINE qword si_rotqmbi( qword a, qword count ) { return (qword)vec_srl( (vec_uchar16)a, vec_sub( (vec_uchar16)0, vec_splat( (vec_uchar16)count, 3 ) ) ); }
__SNC_INLINE qword si_rotqby( qword a, qword count ) { vec_uchar16 sh = vec_sl( vec_splat( (vec_uchar16)count, 3 ), (vec_uchar16)3 ); return (qword)vec_or( vec_slo( (vec_uchar16)a, sh ), vec_sro( (vec_uchar16)a, vec_sub( (vec_uchar16)0, sh ) ) ); }
__SNC_INLINE qword si_rotqbybi( qword a, qword count ) { vec_uchar16 sh = vec_splat( (vec_uchar16)count, 3 ); return (qword)vec_or( vec_slo( (vec_uchar16)a, sh ), vec_sro( (vec_uchar16)a, vec_sub( (vec_uchar16)7, sh ) ) ); }
__SNC_INLINE qword si_rotqmby( qword a, qword count ) { vec_uchar16 sh = vec_sl( vec_sub( (vec_uchar16)0, vec_splat( (vec_uchar16)count, 3 ) ), (vec_uchar16)3 ); return (qword)vec_sel( vec_sro( (vec_uchar16)a, sh ), (vec_uchar16)0, vec_sra( sh, (vec_uchar16)7 ) ); }
__SNC_INLINE qword si_rotqmbybi( qword a, qword count ) { vec_uchar16 sh = vec_sub( (vec_uchar16)0, vec_splat( (vec_uchar16)count, 3 ) ); return (qword)vec_sel( vec_sro( (vec_uchar16)a, sh ), (vec_uchar16)0, vec_sra( sh, (vec_uchar16)7 ) ); }

__SNC_INLINE qword si_rotqbii( qword a, int b ) { vec_uchar16 sh = (vec_uchar16)(b&7), h = vec_sll( (vec_uchar16)a, sh ), l = vec_sll( vec_sld( (vec_uchar16)a, (vec_uchar16)a, 8 ), sh ); return (qword)vec_or( h, vec_sld( l, l, 8 ) ); }
__SNC_INLINE qword si_rotqbyi( qword a, int b ) { return (qword)vec_sld( a, a, (b&15) ); }
__SNC_INLINE qword si_rotqmbii( qword a, int b ) { return (qword)vec_srl( (vec_uchar16)a, (vec_uchar16)(-b&15) ); }
__SNC_INLINE qword si_rotqmbyi( qword a, int b )  { return (-b&31)==0 ? (qword)a : (-b&31)>15 ? (qword)0 : (qword)vec_sld( (vec_uchar16)0, (vec_uchar16)a, (-b&15) ); }

__SNC_INLINE qword si_selb( qword a, qword b, qword c ) { return vec_sel( a, b, c ); }

__SNC_INLINE qword si_sf( qword a, qword b ) { return (qword)vec_sub( (vec_int4)b, (vec_int4)a ); }
__SNC_INLINE qword si_sfh( qword a, qword b ) { return (qword)vec_sub( (vec_short8)b, (vec_short8)a ); }
__SNC_INLINE qword si_sfhi( qword a, short b ) { return (qword)vec_sub( (vec_short8)b, (vec_short8)a ); }
__SNC_INLINE qword si_sfi( qword a, int b ) { return (qword)vec_sub( (vec_int4)b, (vec_int4)a ); }
__SNC_INLINE qword si_sfx( qword a, qword b, qword c ) { return (qword)vec_sub( vec_sub( (vec_int4)b, (vec_int4)a ), vec_nor( (vec_int4)c, (vec_int4)-2 ) ); }

__SNC_INLINE qword si_shl( qword a, qword b ) { return (qword)vec_andc( vec_sl( (vec_uint4)a, (vec_uint4)b ), vec_sra( vec_sl( (vec_uint4)b, (vec_uint4)(~5) ), (vec_uint4)-1 ) ); }
__SNC_INLINE qword si_shlh( qword a, qword b ) { return (qword)vec_andc( vec_sl( (vec_ushort8)a, (vec_ushort8)b ), vec_sra( vec_sl( (vec_ushort8)b, (vec_ushort8)(~4) ), (vec_ushort8)-1 ) ); }
__SNC_INLINE qword si_shlqbi( qword a, qword count ) { return (qword)vec_sll( (vec_uchar16)a, vec_splat( (vec_uchar16)count, 3 ) ); }
__SNC_INLINE qword si_shlqby( qword a, qword count ) { vec_uchar16 sh = vec_sl( vec_splat( (vec_uchar16)count, 3 ), (vec_uchar16)3 ); return (qword)vec_andc( vec_slo( (vec_uchar16)a, sh ), vec_sra( sh, (vec_uchar16)-1 ) ); }
__SNC_INLINE qword si_shlqbybi( qword a, qword count ) { vec_uchar16 sh = vec_splat( (vec_uchar16)count, 3 ); return (qword)vec_andc( vec_slo( (vec_uchar16)a, sh ), vec_sra( sh, (vec_uchar16)-1 ) ); }

__SNC_INLINE qword si_shli( qword a, unsigned int b ) { return (b&32) ? (qword)0 : (qword)vec_sl( (vec_uint4)a, (vec_uint4)(b&31) ); }
__SNC_INLINE qword si_shlhi( qword a, unsigned int b ) { return (b&16) ? (qword)0 : (qword)vec_sl( (vec_ushort8)a, (vec_ushort8)(b&15) ); }
__SNC_INLINE qword si_shlqbii( qword a, unsigned int b ) { return (qword)vec_sll( (vec_uchar16)a, (vec_uchar16)(b&7) ); }
__SNC_INLINE qword si_shlqbyi( qword a, unsigned int b ) { return (b&16) ? (qword)0 : (qword)vec_slo( (vec_uchar16)a, (vec_uchar16)((b&15)*8) ); }

__SNC_INLINE qword si_shufb( qword a, qword b, qword pattern ) { qword table = (qword)( 0, 0, 0, 0, 0, 0, 255, 128 ); qword perm = vec_perm( a, b, pattern ); qword fancy = vec_perm( table, table, vec_sr( pattern, (qword)5 ) ); return (qword)vec_sel( perm, fancy, vec_sra( pattern, (qword)7 ) ); }

__SNC_INLINE qword si_sumb( qword a, qword b ) { vec_ushort8 ha, hb; vec_uint4 suma = vec_sum4s( a, (vec_uint4)0 ), sumb = vec_sum4s( b, (vec_uint4)0 ); ha = vec_pack( suma, suma ); hb = vec_pack( sumb, sumb ); return (qword)vec_mergel( hb, ha ); }
__SNC_INLINE qword si_xor( qword a, qword b ) { return vec_xor( a, b ); }
__SNC_INLINE qword si_xorbi( qword a, unsigned char b ) { return (qword)vec_xor( (vec_uchar16)a, (vec_uchar16)b ); }
__SNC_INLINE qword si_xorhi( qword a, unsigned short b ) { return (qword)vec_xor( (vec_ushort8)a, (vec_ushort8)b ); }
__SNC_INLINE qword si_xori( qword a, unsigned int b ) { return (qword)vec_xor( (vec_uint4)a, (vec_uint4)b ); }
__SNC_INLINE qword si_xsbh( qword a ) { return (qword)vec_unpackl( vec_pack( (vec_short8)a, (vec_short8)a ) ); }
__SNC_INLINE qword si_xshw( qword a ) { return (qword)vec_unpackl( vec_pack( (vec_int4)a, (vec_int4)a ) ); }
__SNC_INLINE qword si_xswd( qword a ) { return vec_sel( (qword)vec_sra( vec_sld( (vec_uint4)a, (vec_uint4)a, 4 ), (vec_uint4)-1 ), (qword)a, (qword)vec_mergel( (vec_uint4)0, (vec_uint4)-1 ) ); }

__SNC_INLINE double si_to_double( qword q ) { return *(double*)(void*)&q; }
__SNC_INLINE float si_to_float( qword q ) { return *(float*)(void*)&q; }
__SNC_INLINE int si_to_int( qword q ) { return *(int*)(void*)&q; }
__SNC_INLINE long long si_to_llong( qword q ) { return *(long long*)(void*)&q; }
__SNC_INLINE short si_to_short( qword q ) { return *(short*)(void*)&q; }
__SNC_INLINE signed char si_to_char( qword q ) { return *(signed char*)(void*)&q; }
__SNC_INLINE unsigned char si_to_uchar( qword q ) { return *(unsigned char*)(void*)&q; }
__SNC_INLINE unsigned int si_to_uint( qword q ) { return *(unsigned int*)(void*)&q; }
__SNC_INLINE unsigned long long si_to_ullong( qword q ) { return *(unsigned long long*)(void*)&q; }
__SNC_INLINE unsigned short si_to_ushort( qword q ) { return *(unsigned short*)(void*)&q; }
__SNC_INLINE void * si_to_ptr( qword q ) { return *(void**)(void*)&q; }

__SNC_INLINE qword si_from_char( signed char value ) { union { qword q; char x[ 16 ]; } qx; qx.x[ 3 ] = value; return qx.q; }
__SNC_INLINE qword si_from_uchar( unsigned char value ) { union { qword q; unsigned char x[ 16 ]; } qx; qx.x[ 3 ] = value; return qx.q; }

__SNC_INLINE qword si_from_short( short value ) { union { qword q; short x[ 8 ]; } qx; qx.x[ 1 ] = value; return qx.q; }
__SNC_INLINE qword si_from_ushort( unsigned short value ) { union { qword q; unsigned short x[ 8 ]; } qx; qx.x[ 1 ] = value; return qx.q; }

__SNC_INLINE qword si_from_int( int value ) { union { qword q; int x[ 4 ]; } qx; qx.x[ 0 ] = value; return qx.q; }
__SNC_INLINE qword si_from_uint( unsigned int value ) { union { qword q; unsigned int x[ 4 ]; } qx; qx.x[ 0 ] = value; return qx.q; }
__SNC_INLINE qword si_from_float( float value ) { union { qword q; float x[ 4 ]; } qx; qx.x[ 0 ] = value; return qx.q; }
__SNC_INLINE qword si_from_ptr( void *value ) { union { qword q; void *x[ 4 ]; } qx; qx.x[ 0 ] = value; return qx.q; }

__SNC_INLINE qword si_from_llong( long long value ) { union { qword q; long long x[ 2 ]; } qx; qx.x[ 0 ] = value; return qx.q; }
__SNC_INLINE qword si_from_ullong( unsigned long long value ) { union { qword q; unsigned long long x[ 2 ]; } qx; qx.x[ 0 ] = value; return qx.q; }
__SNC_INLINE qword si_from_double( double value ) { union { qword q; double x[ 2 ]; } qx; qx.x[ 0 ] = value; return qx.q; }

__SNC_INLINE void si_bisled( void (*func)() ) { SPU_UNSUPPORTED_ACTION; }
__SNC_INLINE void si_bisledd( void (*func)() ) { SPU_UNSUPPORTED_ACTION; }
__SNC_INLINE void si_bislede( void (*func)() ) { SPU_UNSUPPORTED_ACTION; }
__SNC_INLINE void si_dsync() {}
__SNC_INLINE void si_fscrwr( qword a ) { }
__SNC_INLINE void si_mtspr( int reg, qword a ) {}

__SNC_INLINE void si_heq( qword a, qword b ) { if( vec_any_eq( vec_splat( (vec_int4)a, 0 ), vec_splat( (vec_int4)b, 0 ) ) ) { SPU_HALT_ACTION; } }
__SNC_INLINE void si_heqi( qword a, unsigned int b ) { if( vec_any_eq( vec_splat( (vec_int4)a, 0 ), (vec_int4)b ) ) { SPU_HALT_ACTION; } }
__SNC_INLINE void si_hgt( qword a, qword b ) { if( vec_any_gt( vec_splat( (vec_int4)a, 0 ), vec_splat( (vec_int4)b, 0 ) ) ) { SPU_HALT_ACTION; } }
__SNC_INLINE void si_hgti( qword a, signed int b ) { if( vec_any_gt( vec_splat( (vec_int4)a, 0 ), (vec_int4)b ) ) { SPU_HALT_ACTION; } }
__SNC_INLINE void si_hlgt( qword a, qword b ) { if( vec_any_gt( vec_splat( (vec_uint4)a, 0 ), vec_splat( (vec_uint4)b, 0 ) ) ) { SPU_HALT_ACTION; } }
__SNC_INLINE void si_hlgti( qword a, unsigned int b ) { if( vec_any_gt( vec_splat( (vec_uint4)a, 0 ), (vec_uint4)b ) ) { SPU_HALT_ACTION; } }

__SNC_INLINE void si_stqa( qword a, unsigned int imm ) { *(qword*)imm = a; }
__SNC_INLINE void si_stqd( qword a, qword b, unsigned int imm ) { *(qword*)( *(unsigned*)(void*)&b + imm ) = a; }
__SNC_INLINE void si_stqr( qword a, unsigned int imm ) { *(qword*)imm = a; }
__SNC_INLINE void si_stqx( qword a, qword b, qword c ) { *(qword*)( *(unsigned*)(void*)&b + *(unsigned*)(void*)&c ) = a; }

__SNC_INLINE void si_stop() { SPU_STOP_ACTION; }
__SNC_INLINE void si_stopd( qword a, qword b, qword c ) { SPU_STOP_ACTION; }

__SNC_INLINE void si_sync() {}
__SNC_INLINE void si_syncc() {}
__SNC_INLINE void si_wrch( int channel, qword a ) {}

#endif /* !__SPU__ */
#endif /* !_SI2VMX_SNC_H_ */

