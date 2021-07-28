/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _FLOATINVEC_H
#define _FLOATINVEC_H

#include <math.h>
#include <spu_intrinsics.h>

namespace Vectormath {

class boolInVec;

//--------------------------------------------------------------------------------------------------
// floatInVec class
//

// A class representing a scalar float value contained in a vector register
class floatInVec
{
private:
    vec_float4 mData;

public:
    // Default constructor; does no initialization
    //
    inline floatInVec( ) { };

    // Construct from a value converted from bool
    //
    inline floatInVec(boolInVec vec);

    // Construct from a slot of vec_float4
    //
    inline floatInVec(vec_float4 vec, int slot);

    // Construct from 0 word slot of vec_float4
    //
    explicit inline floatInVec(vec_float4 vec);

    // Explicit cast from float
    //
    explicit inline floatInVec(float scalar);

    // Explicit cast to float
    //
    inline float getAsFloat() const;

#ifndef _VECTORMATH_NO_SCALAR_CAST
    // Implicit cast to float
    //
    inline operator float() const;
#endif

    // Get vector data; a float value is in 0 word slot, other slots are undefined
    //
    inline vec_float4 get128() const;

    // Post increment (add 1.0f)
    //
    inline const floatInVec operator ++ (int);

    // Post decrement (subtract 1.0f)
    //
    inline const floatInVec operator -- (int);

    // Pre increment (add 1.0f)
    //
    inline floatInVec& operator ++ ();

    // Pre decrement (subtract 1.0f)
    //
    inline floatInVec& operator -- ();

    // Negation operator
    //
    inline const floatInVec operator - () const;

    // Assignment operator
    //
    inline floatInVec& operator = (floatInVec vec);

    // Multiplication assignment operator
    //
    inline floatInVec& operator *= (floatInVec vec);

    // Division assignment operator
    //
    inline floatInVec& operator /= (floatInVec vec);

    // Addition assignment operator
    //
    inline floatInVec& operator += (floatInVec vec);

    // Subtraction assignment operator
    //
    inline floatInVec& operator -= (floatInVec vec);

};

// Multiplication operator
//
inline const floatInVec operator * (floatInVec vec0, floatInVec vec1);

// Division operator
//
inline const floatInVec operator / (floatInVec vec0, floatInVec vec1);

// Addition operator
//
inline const floatInVec operator + (floatInVec vec0, floatInVec vec1);

// Subtraction operator
//
inline const floatInVec operator - (floatInVec vec0, floatInVec vec1);

// Less than operator
//
inline const boolInVec operator < (floatInVec vec0, floatInVec vec1);

// Less than or equal operator
//
inline const boolInVec operator <= (floatInVec vec0, floatInVec vec1);

// Greater than operator
//
inline const boolInVec operator > (floatInVec vec0, floatInVec vec1);

// Greater than or equal operator
//
inline const boolInVec operator >= (floatInVec vec0, floatInVec vec1);

// Equal operator
//
inline const boolInVec operator == (floatInVec vec0, floatInVec vec1);

// Not equal operator
//
inline const boolInVec operator != (floatInVec vec0, floatInVec vec1);

// Conditionally select between two values
//
inline const floatInVec select(floatInVec vec0, floatInVec vec1, boolInVec select_vec1);


} // namespace Vectormath


//--------------------------------------------------------------------------------------------------
// floatInVec implementation
//

#include "boolInVec.h"

namespace Vectormath {

inline
floatInVec::floatInVec(vec_float4 vec)
{
    mData = vec;
}

inline
floatInVec::floatInVec(boolInVec vec)
{
    mData = spu_sel(spu_splats(0.0f), spu_splats(1.0f), vec.get128());
}

inline
floatInVec::floatInVec(vec_float4 vec, int slot)
{
    mData = spu_promote(spu_extract(vec, slot), 0);
}

inline
floatInVec::floatInVec(float scalar)
{
    mData = spu_promote(scalar, 0);
}

inline
float
floatInVec::getAsFloat() const
{
    return spu_extract(mData,0);
}

#ifndef _VECTORMATH_NO_SCALAR_CAST
inline
floatInVec::operator float() const
{
    return getAsFloat();
}
#endif

inline
vec_float4
floatInVec::get128() const
{
    return mData;
}

inline
const floatInVec
floatInVec::operator ++ (int)
{
    vec_float4 olddata = mData;
    operator ++();
    return floatInVec(olddata);
}

inline
const floatInVec
floatInVec::operator -- (int)
{
    vec_float4 olddata = mData;
    operator --();
    return floatInVec(olddata);
}

inline
floatInVec&
floatInVec::operator ++ ()
{
    *this += floatInVec((vec_float4)(1.0f));
    return *this;
}

inline
floatInVec&
floatInVec::operator -- ()
{
    *this -= floatInVec((vec_float4)(1.0f));
    return *this;
}

inline
const floatInVec
floatInVec::operator - () const
{
    return floatInVec((vec_float4)spu_xor((vec_uint4)mData, spu_splats(0x80000000)));
}

inline
floatInVec&
floatInVec::operator = (floatInVec vec)
{
    mData = vec.mData;
    return *this;
}

inline
floatInVec&
floatInVec::operator *= (floatInVec vec)
{
    *this = *this * vec;
    return *this;
}

inline
floatInVec&
floatInVec::operator /= (floatInVec vec)
{
    *this = *this / vec;
    return *this;
}

inline
floatInVec&
floatInVec::operator += (floatInVec vec)
{
    *this = *this + vec;
    return *this;
}

inline
floatInVec&
floatInVec::operator -= (floatInVec vec)
{
    *this = *this - vec;
    return *this;
}

inline
const floatInVec
operator * (floatInVec vec0, floatInVec vec1)
{
    return floatInVec(spu_mul(vec0.get128(), vec1.get128()));
}

inline
const floatInVec
operator / (floatInVec num, floatInVec den)
{
    return floatInVec(divf4(num.get128(), den.get128()));
}

inline
const floatInVec
operator + (floatInVec vec0, floatInVec vec1)
{
    return floatInVec(spu_add(vec0.get128(), vec1.get128()));
}

inline
const floatInVec
operator - (floatInVec vec0, floatInVec vec1)
{
    return floatInVec(spu_sub(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator < (floatInVec vec0, floatInVec vec1)
{
    return boolInVec(spu_cmpgt(vec1.get128(), vec0.get128()));
}

inline
const boolInVec
operator <= (floatInVec vec0, floatInVec vec1)
{
    return !(vec0 > vec1);
}

inline
const boolInVec
operator > (floatInVec vec0, floatInVec vec1)
{
    return boolInVec(spu_cmpgt(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator >= (floatInVec vec0, floatInVec vec1)
{
    return !(vec0 < vec1);
}

inline
const boolInVec
operator == (floatInVec vec0, floatInVec vec1)
{
    return boolInVec(spu_cmpeq(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator != (floatInVec vec0, floatInVec vec1)
{
    return !(vec0 == vec1);
}

inline
const floatInVec
select(floatInVec vec0, floatInVec vec1, boolInVec select_vec1)
{
    return floatInVec(spu_sel(vec0.get128(), vec1.get128(), select_vec1.get128()));
}

} // namespace Vectormath

#endif // floatInVec_h
