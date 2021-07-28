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

#ifndef WM4SOFTUTILITYMCR_H
#define WM4SOFTUTILITYMCR_H

// TO DO.  Reimplement to manipulate float interpreted as an integer.

#define WM4_FLOAT_TO_INT(fFloat)\
    (int)(fFloat)

#define WM4_FLOAT_TO_UINT(fFloat)\
    (unsigned int)(fFloat)

#define WM4_UNIT_FLOAT_TO_INT(fFloat,iMaxValue)\
    (int)(fFloat*(float)iMaxValue)

#define WM4_UNIT_FLOAT_TO_UINT(fFloat,uiMaxValue)\
    (unsigned int)(fFloat*(float)uiMaxValue)

#endif
