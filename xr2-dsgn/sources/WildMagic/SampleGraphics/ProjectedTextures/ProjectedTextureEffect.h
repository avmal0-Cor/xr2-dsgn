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

#ifndef PROJECTEDTEXTUREEFFECT_H
#define PROJECTEDTEXTUREEFFECT_H

#include "Wm4ShaderEffect.h"
#include "Wm4Camera.h"
#include "Wm4Light.h"

namespace Wm4
{

class ProjectedTextureEffect : public ShaderEffect
{
    WM4_DECLARE_RTTI;
    WM4_DECLARE_NAME_ID;
    WM4_DECLARE_STREAM;

public:
    ProjectedTextureEffect (Camera* pkProjector, const char* acProjectorImage,
        Light* pkLight);
    virtual ~ProjectedTextureEffect ();

    virtual void SetGlobalState (int iPass, Renderer* pkRenderer,
        bool bPrimaryEffect);
    virtual void RestoreGlobalState (int iPass, Renderer* pkRenderer,
        bool bPrimaryEffect);

protected:
    // streaming
    ProjectedTextureEffect ();

    CameraPtr m_spkProjector;
    LightPtr m_spkLight;
};

WM4_REGISTER_STREAM(ProjectedTextureEffect);
typedef Pointer<ProjectedTextureEffect> ProjectedTextureEffectPtr;

}

#endif
