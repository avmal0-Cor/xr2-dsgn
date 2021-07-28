[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 200.007
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
The bug in the MultiStream of the SDK200.002 package 
(PS3_SDK-200_002.zip) has been fixed in this package.

Please use this package by expanding it in an environment in which 
the packages related to SDK200.002 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_multistream_*.txt
+---target
     +---ppu
          +---lib
               |---libmstreamSPURSAT3.a
               |---libmstreamSPURSMP3.a
               |---libmstreamThreadAT3.a
               +---libmstreamThreadMP3.a

----------------------------------------------------------------------
Chages
----------------------------------------------------------------------
<Bug fixes>
- (B#44827)
  When multiple DSP effects are used by linking libmstreamSPURSAT3.a, 
  it may crash on the SPU.
  This problem has been fixed so that the restriction on the number of 
  DSP effects is eased.

- The following bug has been fixed.

  - (B#43798)
    When cellMSSystemGetNeededMemorySize() is executed by linking 
    libmstream*AT3.a, an invalid value(memory size) is returned.

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

"DTS" is a registered trademark of DTS, Inc.

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

