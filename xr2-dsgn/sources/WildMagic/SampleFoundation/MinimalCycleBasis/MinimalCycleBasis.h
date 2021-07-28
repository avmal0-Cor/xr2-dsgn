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

#ifndef MINIMALCYCLEBASIS_H
#define MINIMALCYCLEBASIS_H

#include "Wm4WindowApplication2.h"
#include "Wm4PlanarGraph.h"
#include <vector>
using namespace Wm4;

class MinimalCycleBasis : public WindowApplication2
{
    WM4_DECLARE_INITIALIZE;

public:
    MinimalCycleBasis ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnDisplay ();

protected:
    typedef PlanarGraph<IVector2> Graph;
    Graph m_kGraph;
    std::vector<Graph::Primitive*> m_kPrimitives;
    Color* m_akColor;
};

WM4_REGISTER_INITIALIZE(MinimalCycleBasis);

#endif
