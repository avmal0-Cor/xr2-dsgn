// Wild Magic Source Code
// David Eberly
// http://www.geometrictools.com
// Copyright (c) 1998-2009
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// either of the locations:
//     http://www.gnu.org/copyleft/lgpl.html
//     http://www.geometrictools.com/License/WildMagicLicense.pdf
//
// Version: 4.8.0 (2009/04/10)

#include "Wm4FoundationPCH.h"
#include "Wm4IntrRay2Ray2.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
IntrRay2Ray2<Real>::IntrRay2Ray2 (const Ray2<Real>& rkRay0,
    const Ray2<Real>& rkRay1)
    :
    m_pkRay0(&rkRay0),
    m_pkRay1(&rkRay1)
{
}
//----------------------------------------------------------------------------
template <class Real>
const Ray2<Real>& IntrRay2Ray2<Real>::GetRay0 () const
{
    return *m_pkRay0;
}
//----------------------------------------------------------------------------
template <class Real>
const Ray2<Real>& IntrRay2Ray2<Real>::GetRay1 () const
{
    return *m_pkRay1;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay2Ray2<Real>::Test ()
{
    Vector2<Real> kDiff;
    Real afParameter[2];
    m_iIntersectionType = Classify(afParameter,&kDiff,0);

    if (m_iIntersectionType == IT_POINT)
    {
        // Test whether the line-line intersection is on the rays.
        if (afParameter[0] >= (Real)0 && afParameter[1] >= (Real)0)
        {
            m_iQuantity = 1;
        }
        else
        {
            m_iQuantity = 0;
            m_iIntersectionType = IT_EMPTY;
        }
    }
    else if (m_iIntersectionType == IT_RAY)
    {
        m_iQuantity = INT_MAX;
    }
    else
    {
        m_iQuantity = 0;
    }

    return m_iIntersectionType != IT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real>
bool IntrRay2Ray2<Real>::Find ()
{
    Vector2<Real> kDiff;
    Real afParameter[2];
    m_iIntersectionType = Classify(afParameter,&kDiff,0);

    if (m_iIntersectionType == IT_POINT)
    {
        // Test whether the line-line intersection is on the rays.
        if (afParameter[0] >= (Real)0 && afParameter[1] >= (Real)0)
        {
            m_iQuantity = 1;
            m_kPoint = m_pkRay0->Origin + afParameter[0]*m_pkRay0->Direction;
        }
        else
        {
            m_iQuantity = 0;
            m_iIntersectionType = IT_EMPTY;
        }
    }
    else if (m_iIntersectionType == IT_RAY)
    {
        m_iQuantity = INT_MAX;
    }
    else
    {
        m_iQuantity = 0;
    }

    return m_iIntersectionType != IT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real>
int IntrRay2Ray2<Real>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class Real>
const Vector2<Real>& IntrRay2Ray2<Real>::GetPoint () const
{
    return m_kPoint;
}
//----------------------------------------------------------------------------
template <class Real>
int IntrRay2Ray2<Real>::Classify (Real* afS, Vector2<Real>* pkDiff,
    Vector2<Real>* pkDiffN)
{
    // The intersection of two lines is a solution to P0+s0*D0 = P1+s1*D1.
    // Rewrite this as s0*D0 - s1*D1 = P1 - P0 = Q.  If D0.Dot(Perp(D1)) = 0,
    // the lines are parallel.  Additionally, if Q.Dot(Perp(D1)) = 0, the
    // lines are the same.  If D0.Dot(Perp(D1)) is not zero, then
    //   s0 = Q.Dot(Perp(D1))/D0.Dot(Perp(D1))
    // produces the point of intersection.  Also,
    //   s1 = Q.Dot(Perp(D0))/D0.Dot(Perp(D1))

    Vector2<Real> kDiff = m_pkRay1->Origin - m_pkRay0->Origin;
    if (pkDiff)
    {
        *pkDiff = kDiff;
    }

    Real fD0DotPerpD1 = m_pkRay0->Direction.DotPerp(m_pkRay1->Direction);
    if (Math<Real>::FAbs(fD0DotPerpD1) > Math<Real>::ZERO_TOLERANCE)
    {
        // lines intersect in a single point
        if (afS)
        {
            Real fInvD0DotPerpD1 = ((Real)1.0)/fD0DotPerpD1;
            Real fDiffDotPerpD0 = kDiff.DotPerp(m_pkRay0->Direction);
            Real fDiffDotPerpD1 = kDiff.DotPerp(m_pkRay1->Direction);
            afS[0] = fDiffDotPerpD1*fInvD0DotPerpD1;
            afS[1] = fDiffDotPerpD0*fInvD0DotPerpD1;
        }
        return IT_POINT;
    }

    // lines are parallel
    kDiff.Normalize();
    if (pkDiffN)
    {
        *pkDiffN = kDiff;
    }

    Real fDiffNDotPerpD1 = kDiff.DotPerp(m_pkRay1->Direction);
    if (Math<Real>::FAbs(fDiffNDotPerpD1) <= Math<Real>::ZERO_TOLERANCE)
    {
        // lines are colinear
        return IT_RAY;
    }

    // lines are parallel, but distinct
    return IT_EMPTY;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class IntrRay2Ray2<float>;

template WM4_FOUNDATION_ITEM
class IntrRay2Ray2<double>;
//----------------------------------------------------------------------------
}
