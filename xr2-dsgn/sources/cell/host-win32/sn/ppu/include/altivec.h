/* PowerPC AltiVec include file.

   Copyright (c) 2008 SN Systems Ltd.
   support@snsys.com

   This file is provided for compatibility with GCC.  
   */

#ifndef __SNC_ALTIVEC_H__
#define __SNC_ALTIVEC_H__ 1

#ifndef __SNC__
  #error "This is an SNC specific file, it should not be included in builds with other compilers"
#endif

#ifndef __SNC_INLINE
  #define __SNC_INLINE static inline __attribute__( ( always_inline ) )
#endif

#if __SN_VER__ <= 25002
  #ifdef __cplusplus
    /* Intrinsics to extract and insert values into vectors */
    /* Optimisations may be available to eliminate LHS dependencies. For example, vec_perm( x, y, vec_lvsl( constant ) ) -> vec_sld( x, y, n  ) */
    /* These intrinsics may migrate to compiler internals */

    /* Implemented in the compiler as of 250.3 */

    __SNC_INLINE unsigned char vec_extract( vector unsigned char a, unsigned element ) { union { vector unsigned char v; unsigned char s[ 16 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element, (unsigned char*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE signed char vec_extract( vector signed char a, unsigned element ) { union { vector signed char v; signed char s[ 16 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element, (signed char*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE unsigned short vec_extract( vector unsigned short a, unsigned element ) { union { vector unsigned short v; unsigned short s[ 8 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element*2, (unsigned short*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE signed short vec_extract( vector signed short a, unsigned element ) { union { vector signed short v; signed short s[ 8 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element*2, (signed short*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE unsigned int vec_extract( vector unsigned int a, unsigned element ) { union { vector unsigned int v; unsigned int s[ 4 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element*4, (unsigned int*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE signed int vec_extract( vector signed int a, unsigned element ) { union { vector signed int v; signed int s[ 4 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element*4, (signed int*)0 ) ); return u.s[ 0 ]; }
    __SNC_INLINE float vec_extract( vector float a, unsigned element ) { union { vector float v; float s[ 4 ]; } u; u.v = vec_perm( a, a, vec_lvsl( (int)element*4, (float*)0 ) ); return u.s[ 0 ]; }

    __SNC_INLINE vector unsigned char vec_insert( unsigned char a, vector unsigned char b, unsigned element ) { union { vector unsigned char v; unsigned char s[ 16 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element, (unsigned char*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element, (unsigned char*)0 ) ); }
    __SNC_INLINE vector signed char vec_insert( signed char a, vector signed char b, signed element ) { union { vector signed char v; signed char s[ 16 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element, (signed char*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element, (signed char*)0 ) ); }
    __SNC_INLINE vector unsigned short vec_insert( unsigned short a, vector unsigned short b, unsigned element ) { union { vector unsigned short v; unsigned short s[ 8 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element*2, (unsigned short*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element*2, (unsigned short*)0 ) ); }
    __SNC_INLINE vector signed short vec_insert( signed short a, vector signed short b, signed element ) { union { vector signed short v; signed short s[ 8 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element*2, (signed short*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element*2, (signed short*)0 ) ); }
    __SNC_INLINE vector unsigned int vec_insert( unsigned int a, vector unsigned int b, unsigned element ) { union { vector unsigned int v; unsigned int s[ 4 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element*4, (unsigned int*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element*4, (unsigned int*)0 ) ); }
    __SNC_INLINE vector signed int vec_insert( signed int a, vector signed int b, signed element ) { union { vector signed int v; signed int s[ 4 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element*4, (signed int*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element*4, (signed int*)0 ) ); }
    __SNC_INLINE vector float vec_insert( float a, vector float b, signed element ) { union { vector float v; float s[ 4 ]; } u; u.v = vec_perm( b, b, vec_lvsl( (int)element*4, (float*)0 ) ); u.s[ 0 ] = a; return vec_perm( u.v, u.v, vec_lvsr( (int)element*4, (float*)0 ) ); }

    __SNC_INLINE vector unsigned char vec_promote( unsigned char a, unsigned element ) { return (vector unsigned char)a; }
    __SNC_INLINE vector signed char vec_promote( signed char a, unsigned element ) { return (vector signed char)a; }
    __SNC_INLINE vector unsigned short vec_promote( unsigned short a, unsigned element ) { return (vector unsigned short)a; }
    __SNC_INLINE vector signed short vec_promote( signed short a, unsigned element ) { return (vector signed short)a; }
    __SNC_INLINE vector unsigned int vec_promote( unsigned int a, unsigned element ) { return (vector unsigned int)a; }
    __SNC_INLINE vector signed int vec_promote( signed int a, unsigned element ) { return (vector signed int)a; }
    __SNC_INLINE vector float vec_promote( float a, unsigned element ) { return (vector float)a; }

    __SNC_INLINE vector unsigned char vec_splats( unsigned char a ) { return (vector unsigned char)a; }
    __SNC_INLINE vector signed char vec_splats( signed char a ) { return (vector signed char)a; }
    __SNC_INLINE vector unsigned short vec_splats( unsigned short a ) { return (vector unsigned short)a; }
    __SNC_INLINE vector signed short vec_splats( signed short a ) { return (vector signed short)a; }
    __SNC_INLINE vector unsigned int vec_splats( unsigned int a ) { return (vector unsigned int)a; }
    __SNC_INLINE vector signed int vec_splats( signed int a ) { return (vector signed int)a; }
    __SNC_INLINE vector float vec_splats( float a ) { return (vector float)a; }
  #endif
#endif

#endif
