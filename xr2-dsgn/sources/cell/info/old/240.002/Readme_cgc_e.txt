[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 240.002
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
The bug in the cgc of the SDK240.001 package (PS3_SDK-240_001.zip)
has been fixed in this package.

Please use this package by expanding it in an environment in which 
the packages related to SDK240.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
|---Readme_cgc_*.txt
|---host-win32
|    +---Cg
|         |---bin
|         |    |---libcgc.dll
|         |    +---sce-cgc.exe
|         +---lib
|              |---libcgc.a
|              |---libcgc.lib
|              |---libcgc_static.lib
|              +---libcgc_static_2005.lib
+---target
     +---ppu
          +---lib
               +---libcgc.a

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
<New Feature>
- A library for Visual C++ 2005(libcgc_static_2005.lib) has been added.

<Bugfix>
- An invalid register was allocated to a vertex program.
  This problem has been fixed.
  (The behavior of the vertex program when an invalid register is allocated 
   is not defined.)

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
None

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this software conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc). 

----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
"PLAYSTATION" is a registered trademark of Sony Computer Entertainment Inc.
All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

