[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 154.004
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================
This package includes the following utilities and documents that are 
necessary for booting a game application from the built-in HDD of 
the Reference Tool. 

  - HDD Boot Game Utility
  - NPDRM Package Creation Tool

In order to boot a game application from the built-in HDD, please use 
the NPDRM package creation tool to create an appropriate NPDRM package. 
The created NPDRM package can be installed from the menu 
"Game -> Install Package File" of the system software. 

Please refer to "HDD Boot Game Utility Overview" for the procedure of 
developing an HDD boot game application and "NPDRM Package Requirements" 
for the details of the method of encrypting a game application and 
the method of installing it to the built-in HDD. 

Please use this package by expanding it in the environment of 
PLAYSTATION(R)3 Programmer Tool Runtime Library SDK 154.001. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_*.txt
|---version-SDK      SDK release version file
|---host-linux(host-win32)
|    +---bin         Utilities
|---target
|    |---common
|    |    +---include
|    +---ppu
|         +---include
|---samples          Samples
|    |---sysutil
|    +---network
+---SDK_doc/jp(en)   PDF documents
     +---pdf
          |---NP
          |    |---NP_DRM-Overview_*.pdf
          |    |---NP_DRM-Package_Requirements_*.pdf
          |    |---NP_DRM-Reference_*.pdf
          |    |---NP_DRM_Error_Code-Reference_*.pdf
          |    +---NP_ID_*.pdf
          +---sysutil
               |---Hdd_Game-Overview_*.pdf
               +---Hdd_Game-Reference_*.pdf


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

