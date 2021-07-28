/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_SIMD_SCALARS_H
#define _SCE_SIMD_SCALARS_H

namespace simd {

typedef unsigned int          uint;
typedef unsigned short      ushort;
typedef unsigned char        uchar;
typedef signed char          schar;

};
                 
#include <math.h>

namespace simd {

inline
vec_float4
_vec_insert( float val, vec_float4 vec, int slot )
{
   union {
      vec_float4 v;
      float s[4];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%4] = val;
   return tmp.v;
}

inline
float 
_vec_extract( vec_float4 vec, int slot )
{
   union {
      vec_float4 v;
      float s[4];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%4];
}

inline
vec_uint4
_vec_insert( uint val, vec_uint4 vec, int slot )
{
   union {
      vec_uint4 v;
      uint s[4];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%4] = val;
   return tmp.v;
}

inline
uint 
_vec_extract( vec_uint4 vec, int slot )
{
   union {
      vec_uint4 v;
      uint s[4];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%4];
}

inline
vec_int4
_vec_insert( int val, vec_int4 vec, int slot )
{
   union {
      vec_int4 v;
      int s[4];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%4] = val;
   return tmp.v;
}

inline
int 
_vec_extract( vec_int4 vec, int slot )
{
   union {
      vec_int4 v;
      int s[4];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%4];
}

inline
vec_ushort8
_vec_insert( ushort val, vec_ushort8 vec, int slot )
{
   union {
      vec_ushort8 v;
      ushort s[8];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%8] = val;
   return tmp.v;
}

inline
ushort 
_vec_extract( vec_ushort8 vec, int slot )
{
   union {
      vec_ushort8 v;
      ushort s[8];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%8];
}

inline
vec_short8
_vec_insert( short val, vec_short8 vec, int slot )
{
   union {
      vec_short8 v;
      short s[8];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%8] = val;
   return tmp.v;
}

inline
short 
_vec_extract( vec_short8 vec, int slot )
{
   union {
      vec_short8 v;
      short s[8];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%8];
}

inline
vec_uchar16
_vec_insert( uchar val, vec_uchar16 vec, int slot )
{
   union {
      vec_uchar16 v;
      uchar s[16];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%16] = val;
   return tmp.v;
}

inline
uchar 
_vec_extract( vec_uchar16 vec, int slot )
{
   union {
      vec_uchar16 v;
      uchar s[16];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%16];
}

inline
vec_char16
_vec_insert( schar val, vec_char16 vec, int slot )
{
   union {
      vec_char16 v;
      schar s[16];
   } tmp;
   
   tmp.v = vec;
   tmp.s[slot%16] = val;
   return tmp.v;
}

inline
schar 
_vec_extract( vec_char16 vec, int slot )
{
   union {
      vec_char16 v;
      schar s[16];
   } tmp;
   
   tmp.v = vec;
   return tmp.s[slot%16];
}

};

#endif
