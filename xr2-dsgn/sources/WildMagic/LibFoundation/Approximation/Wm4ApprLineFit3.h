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

#ifndef WM4APPRLINEFIT3_H
#define WM4APPRLINEFIT3_H

#include "Wm4FoundationLIB.h"
#include "Wm4Line3.h"

namespace Wm4
{

// Least-squares fit of a line to (x,y,z) data by using distance measurements
// orthogonal to the proposed line.
template <class Real> WM4_FOUNDATION_ITEM
Line3<Real> OrthogonalLineFit3 (int iQuantity, const Vector3<Real>* akPoint);

}

#endif
