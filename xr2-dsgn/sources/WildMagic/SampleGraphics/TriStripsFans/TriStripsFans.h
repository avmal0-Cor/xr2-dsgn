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
// Version: 4.0.1 (2006/08/13)

#ifndef TRISTRIPSFANS_H
#define TRISTRIPSFANS_H

#include "Wm4WindowApplication3.h"
using namespace Wm4;

class TriStripsFans : public WindowApplication3
{
    WM4_DECLARE_INITIALIZE;

public:
    TriStripsFans ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();
    virtual bool OnKeyDown (unsigned char ucKey, int iX, int iY);

protected:
    void CreateScene ();

    NodePtr m_spkScene;
    WireframeStatePtr m_spkWireframe;
    TriStripPtr m_spkStrip;
    TriMeshPtr m_spkStripMesh;
    TriFanPtr m_spkFan;
    TriMeshPtr m_spkFanMesh;
    Triangles* m_pkActive;
    Culler m_kCuller;
};

WM4_REGISTER_INITIALIZE(TriStripsFans);

#endif
