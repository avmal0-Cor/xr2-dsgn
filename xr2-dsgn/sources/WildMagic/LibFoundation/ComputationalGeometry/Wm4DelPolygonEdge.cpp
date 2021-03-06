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

#include "Wm4FoundationPCH.h"
#include "Wm4DelPolygonEdge.h"

namespace Wm4
{
//----------------------------------------------------------------------------
template <class Real>
DelPolygonEdge<Real>::DelPolygonEdge (int iV0, int iV1, int iNullIndex,
    DelTriangle<Real>* pkTri)
    :
    VEManifoldMesh::Edge(iV0,iV1)
{
    NullIndex = iNullIndex;
    Tri = pkTri;
}
//----------------------------------------------------------------------------
template <class Real>
VEManifoldMesh::EPtr DelPolygonEdge<Real>::ECreator (int iV0, int iV1)
{
    return WM4_NEW DelPolygonEdge<Real>(iV0,iV1,0,0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template WM4_FOUNDATION_ITEM
class DelPolygonEdge<float>;

template WM4_FOUNDATION_ITEM
class DelPolygonEdge<double>;
//----------------------------------------------------------------------------
}
