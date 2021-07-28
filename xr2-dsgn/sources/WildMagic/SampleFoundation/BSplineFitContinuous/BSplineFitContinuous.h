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

#ifndef BSPLINEFITCONTINUOUS_H
#define BSPLINEFITCONTINUOUS_H

#include "Wm4WindowApplication3.h"
using namespace Wm4;

class BSplineFitContinuous : public WindowApplication3
{
    WM4_DECLARE_INITIALIZE;

public:
    BSplineFitContinuous ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();

protected:
    Polyline* OriginalPolyline (int iCtrlQuantity, Vector3d* akCtrl);
    Polyline* ReducedPolyline (int iCtrlQuantity, Vector3d* akCtrl,
        double dFraction);

    NodePtr m_spkScene, m_spkTrnNode;
    Culler m_kCuller;
    int m_iDegree;
};

WM4_REGISTER_INITIALIZE(BSplineFitContinuous);

#endif
