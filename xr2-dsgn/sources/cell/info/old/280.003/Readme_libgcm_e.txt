[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 280.003
                    Copyright(C) 2009 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================
The bug in the libgcm_cmd.a and libgcm_spu.a of the SDK280.001 package
(PS3_SDK-280_001.zip) has been fixed in this package.

Please use this package by expanding it in an environment in which 
the packages related to SDK280.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
+---target
     +---ppu
     |    +---lib
     |         +---libgcm_cmd.a
     +---spu
          +---lib
               +---pic
               |    +---libgcm_spu.a
               +---libgcm_spu.a

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
<Bug fix>
- The bug that is described in the following technote has been fixed.

  - Bug regarding the Branch Evaluation Processing of the SPU Version 
    cellGcmCgStripBranchesFromFragmentUCode()
    (Technical Notes ID:200907-09  https://ps3.scedev.net/technotes/view/936)

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
