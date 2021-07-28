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
// Version: 4.0.1 (2007/05/06)

#ifndef WM4DISTVECTOR2BOX2_H
#define WM4DISTVECTOR2BOX2_H

#include "Wm4FoundationLIB.h"
#include "Wm4Distance.h"
#include "Wm4Box2.h"

namespace Wm4
{

template <class Real>
class WM4_FOUNDATION_ITEM DistVector2Box2
    : public Distance<Real,Vector2<Real> >
{
public:
    DistVector2Box2 (const Vector2<Real>& rkVector, const Box2<Real>& rkBox);

    // object access
    const Vector2<Real>& GetVector () const;
    const Box2<Real>& GetBox () const;

    // static distance queries
    virtual Real Get ();
    virtual Real GetSquared ();

    // function calculations for dynamic distance queries
    virtual Real Get (Real fT, const Vector2<Real>& rkVelocity0,
        const Vector2<Real>& rkVelocity1);
    virtual Real GetSquared (Real fT, const Vector2<Real>& rkVelocity0,
        const Vector2<Real>& rkVelocity1);

private:
    using Distance<Real,Vector2<Real> >::m_kClosestPoint0;
    using Distance<Real,Vector2<Real> >::m_kClosestPoint1;

    const Vector2<Real>* m_pkVector;
    const Box2<Real>* m_pkBox;
};

typedef DistVector2Box2<float> DistVector2Box2f;
typedef DistVector2Box2<double> DistVector2Box2d;

}

#endif
