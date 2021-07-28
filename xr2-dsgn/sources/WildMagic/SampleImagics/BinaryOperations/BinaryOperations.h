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
// Version: 4.0.1 (2006/10/18)

#ifndef BINARYOPERATIONS_H
#define BINARYOPERATIONS_H

#include "Wm4ConsoleApplication.h"
#include "Wm4Imagics.h"
using namespace Wm4;

class BinaryOperations : public ConsoleApplication
{
    WM4_DECLARE_INITIALIZE;

public:
    virtual int Main (int iQuantity, char** apcArgument);

protected:
    void GetBoundaries (const Binary2D& rkImage);
    void GetComponents (const Binary2D& rkImage);
    void GetL1Distance (const Binary2D& rkImage);
    void GetL2Distance (const Binary2D& rkImage);
    void GetSkeleton (const Binary2D& rkImage);
};

WM4_REGISTER_INITIALIZE(BinaryOperations);

#endif
