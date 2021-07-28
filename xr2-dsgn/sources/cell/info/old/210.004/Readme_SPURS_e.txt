[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 210.004
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
The bug in the libspurs of the SDK210.001 package 
(PS3_SDK-210_001.zip) has been fixed in this package.

Please use this package by expanding it in an environment in which 
the packages related to SDK210.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_SPURS_*.txt
+---target
     +---spu
          +---lib
               |---libspurs.a
               +---pic
                    +---libspurs.a

----------------------------------------------------------------------
Chages
----------------------------------------------------------------------
<libspurs job>
<Bug fix>
- The following problem has been fixed. 

  - When cellSpursJobInitialize() is executed in a SPURS job, 
    up to 16 bytes of the LS area after _end in the SPURS job may be 
    corrupted.

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

