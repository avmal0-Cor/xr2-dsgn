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
// Version: 4.0.2 (2008/01/20)

#ifndef WM4TRIFAN_H
#define WM4TRIFAN_H

#include "Wm4GraphicsLIB.h"
#include "Wm4Triangles.h"

namespace Wm4
{

class WM4_GRAPHICS_ITEM TriFan : public Triangles
{
    WM4_DECLARE_RTTI;
    WM4_DECLARE_NAME_ID;
    WM4_DECLARE_STREAM;

public:
    // Construction and destruction.
    TriFan (VertexBuffer* pkVBuffer,
        bool bUpdateModelNormals = true,
        int iTangentSpaceType = GU_TANGENT_SPACE_NONE);

    TriFan (VertexBuffer* pkVBuffer, IndexBuffer* pkIBuffer,
        bool bUpdateModelNormals = true,
        int iTangentSpaceType = GU_TANGENT_SPACE_NONE);

    virtual ~TriFan ();

    // Interpretation of the index buffer data.
    virtual int GetTriangleQuantity () const;
    virtual bool GetTriangle (int i, int& riV0, int& riV1, int& riV2) const;

protected:
    TriFan ();
};

WM4_REGISTER_STREAM(TriFan);
typedef Pointer<TriFan> TriFanPtr;
#include "Wm4TriFan.inl"

}

#endif
