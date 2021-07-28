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
// Version: 4.0.0 (2006/06/28)

#ifndef WM4CAPSULE3_H
#define WM4CAPSULE3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Segment3.h"

namespace Wm4
{

template <class Real>
class Capsule3
{
public:
    // construction
    Capsule3 ();  // uninitialized
    Capsule3 (const Segment3<Real>& rkSegment, Real fRadius);

    Segment3<Real> Segment;
    Real Radius;
};

#include "Wm4Capsule3.inl"

typedef Capsule3<float> Capsule3f;
typedef Capsule3<double> Capsule3d;

}

#endif
