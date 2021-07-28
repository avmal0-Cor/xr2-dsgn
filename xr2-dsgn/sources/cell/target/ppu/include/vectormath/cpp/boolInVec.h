/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _BOOLINVEC_H
#define _BOOLINVEC_H

#include <math.h>
#include <altivec.h>

namespace Vectormath {

class floatInVec;

//--------------------------------------------------------------------------------------------------
// boolInVec class
//

class boolInVec
{
private:
    vec_uint4 mData;

public:
    // Default constructor; does no initialization
    //
    inline boolInVec( ) { };

    // Construct from a value converted from float
    //
    inline boolInVec(floatInVec vec);

    // Explicit cast from bool
    //
    explicit inline boolInVec(bool scalar);

    // Construct from 0 word slot of vec_uint4
    //
    explicit inline boolInVec(vec_uint4 vec);

    // Explicit cast to bool
    //
    inline bool getAsBool() const;

#ifndef _VECTORMATH_NO_SCALAR_CAST
    // Implicit cast to bool
    //
    inline operator bool() const;
#endif

    // Get vector data; a bool value is in 0 word slot, other slots are undefined
    //
    inline vec_uint4 get128() const;

    // Boolean negation operator
    //
    inline const boolInVec operator ! () const;

    // Assignment operator
    //
    inline boolInVec& operator = (boolInVec vec);

    // Boolean and assignment operator
    //
    inline boolInVec& operator &= (boolInVec vec);

    // Boolean exclusive or assignment operator
    //
    inline boolInVec& operator ^= (boolInVec vec);

    // Boolean or assignment operator
    //
    inline boolInVec& operator |= (boolInVec vec);

};

// Equal operator
//
inline const boolInVec operator == (boolInVec vec0, boolInVec vec1);

// Not equal operator
//
inline const boolInVec operator != (boolInVec vec0, boolInVec vec1);

// And operator
//
inline const boolInVec operator & (boolInVec vec0, boolInVec vec1);

// Exclusive or operator
//
inline const boolInVec operator ^ (boolInVec vec0, boolInVec vec1);

// Or operator
//
inline const boolInVec operator | (boolInVec vec0, boolInVec vec1);

// Conditionally select between two values
//
inline const boolInVec select(boolInVec vec0, boolInVec vec1, boolInVec select_vec1);


} // namespace Vectormath


//--------------------------------------------------------------------------------------------------
// boolInVec implementation
//

#include "floatInVec.h"

namespace Vectormath {

inline
boolInVec::boolInVec(floatInVec vec)
{
    *this = (vec != floatInVec(0.0f));
}

inline
boolInVec::boolInVec(bool scalar)
{
    const unsigned int mask = -(int)scalar;
    mData = vec_splats(mask);
}

inline
boolInVec::boolInVec(vec_uint4 vec)
{
    mData = vec;
}

inline
bool
boolInVec::getAsBool() const
{
    return vec_all_gt(mData, (vec_uint4)(0));
}

inline
vec_uint4
boolInVec::get128() const
{
    return mData;
}

#ifndef _VECTORMATH_NO_SCALAR_CAST
inline
boolInVec::operator bool() const
{
    return getAsBool();
}
#endif

inline
const boolInVec
boolInVec::operator ! () const
{
    return boolInVec(vec_nor(mData, mData));
}

inline
boolInVec&
boolInVec::operator = (boolInVec vec)
{
    mData = vec.mData;
    return *this;
}

inline
boolInVec&
boolInVec::operator &= (boolInVec vec)
{
    *this = *this & vec;
    return *this;
}

inline
boolInVec&
boolInVec::operator ^= (boolInVec vec)
{
    *this = *this ^ vec;
    return *this;
}

inline
boolInVec&
boolInVec::operator |= (boolInVec vec)
{
    *this = *this | vec;
    return *this;
}

inline
const boolInVec
operator == (boolInVec vec0, boolInVec vec1)
{
    return boolInVec((vec_uint4)vec_cmpeq(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator != (boolInVec vec0, boolInVec vec1)
{
    return !(vec0 == vec1);
}

inline
const boolInVec
operator & (boolInVec vec0, boolInVec vec1)
{
    return boolInVec(vec_and(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator | (boolInVec vec0, boolInVec vec1)
{
    return boolInVec(vec_or(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
operator ^ (boolInVec vec0, boolInVec vec1)
{
    return boolInVec(vec_xor(vec0.get128(), vec1.get128()));
}

inline
const boolInVec
select(boolInVec vec0, boolInVec vec1, boolInVec select_vec1)
{
    return boolInVec(vec_sel(vec0.get128(), vec1.get128(), select_vec1.get128()));
}

} // namespace Vectormath

#endif // boolInVec_h
