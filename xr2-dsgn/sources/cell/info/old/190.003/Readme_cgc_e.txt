[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 190.003
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes new features and bug fixes of cgc 
in addition to SDK190.002 package (PS3_SDK-190_002.zip).

Please use this package by expanding it in an environment in which 
the packages related to SDK190.002 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_cgc*.txt
|---host-linux
|    +---Cg
|         |---bin
|         |    +---sce-cgc
|         +---lib
|              +---libcgc.a
|---host-win32
|    +---Cg
|         |---bin
|         |    |---libcgc.dll
|         |    +---sce-cgc.exe
|         +---lib
|              |---libcgc.a
|              |---libcgc.lib
|              +---libcgc_static.lib
+---target
     +---ppu
          +---lib
               +---libcgc.a

----------------------------------------------------------------------
Chages
----------------------------------------------------------------------
<Bugfixs>
- The following bug has been fixed: 
  When multiple texture units are specified using the -texformat option,
  only the last texture unit is enabled. 

- The following problem has been fixed: 
  The attribute of unused may not be correctly set to the unused parameter
  within the shader program. Accordingly, the unused parameter is not
  correctly deleted using sce-cgcstrip.

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

