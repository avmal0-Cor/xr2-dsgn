[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
======================================================================
This directory contains the source code of the resolution conversion 
library (libresc).

----------------------------------------------------------------------
Directory Structure of the libresc Source
----------------------------------------------------------------------
resc/
  |---readme_*.txt
  |---libresc.sln                  (Solution for Visual Studio 2005)
  |---src                          (Source directory)
  |    |---libresc.vcproj          (Project for Visual Studio 2005)
  |    +---Makefile
  |---include                      (libresc.h)
  +---lib                          (The built library by Makefile will 
                                    be placed)

----------------------------------------------------------------------
How to Build
----------------------------------------------------------------------
- Windows (VisualStudio 2005)
  Open libresc.sln and build it.
  There are 2 types of configurations such as "PS3 Release" and 
  "PS3 Debug".

- Linux / Windows (msys)
  Perform the following command in the src directory:
  $ make 
  The debug build can be obtained as follows:
  $ make CELL_RESC_DEBUG=1

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
<About the Operation Guarantee>
- For the operation guarantee of the runtime libraries, the following
  condition apply.
  A binary that is created by building the source code, whether with
  or without changes, is also excluded from the operation guarantee of
  the runtime libraries.
  Therefore, please note that it will be handled like an original
  runtime library created by a licensee.

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this software conform to
the contract concluded between your company and our company (Sony
Computer Entertainment Inc).

----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
"PLAYSTATION" is a registered trademark of Sony Computer Entertainment 
Inc.
All other product and company names mentioned herein, with or without
the registered trademark symbol (R) or trademark symbol (TM), are
generally registered trademarks and/or trademarks of their respective
owners.
