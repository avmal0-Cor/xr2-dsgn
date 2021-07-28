/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _VECTORMATH_VECIDX_AOS_H
#define _VECTORMATH_VECIDX_AOS_H

#include "floatInVec.h"

namespace Vectormath {
namespace Aos {

//-----------------------------------------------------------------------------
// VecIdx
// Used in setting elements of Vector3, Vector4, Point3, or Quat with the
// subscripting operator.
//

class VecIdx
{
private:
    vec_float4 &ref __attribute__ ((aligned(16)));
    int i __attribute__ ((aligned(16)));
public:
    inline VecIdx( vec_float4& vec, int idx ): ref(vec) { i = idx; }

    // implicitly casts to float unless _VECTORMATH_NO_SCALAR_CAST defined
    // in which case, implicitly casts to floatInVec, and one must call
    // getAsFloat to convert to float.
    //
    inline float getAsFloat() const;

#ifdef _VECTORMATH_NO_SCALAR_CAST
    inline operator floatInVec() const;
#else
    inline operator float() const;
#endif

    inline float operator =( float scalar );
    inline floatInVec operator =( floatInVec scalar );
    inline floatInVec operator =( const VecIdx& scalar );
    inline floatInVec operator *=( float scalar );
    inline floatInVec operator *=( floatInVec scalar );
    inline floatInVec operator /=( float scalar );
    inline floatInVec operator /=( floatInVec scalar );
    inline floatInVec operator +=( float scalar );
    inline floatInVec operator +=( floatInVec scalar );
    inline floatInVec operator -=( float scalar );
    inline floatInVec operator -=( floatInVec scalar );
};

} // namespace Aos
} // namespace Vectormath

#endif
