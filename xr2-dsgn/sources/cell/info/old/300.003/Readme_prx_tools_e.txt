[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 300.003
                    Copyright(C) 2009 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================
The bug in the ppu-lv2-prx-fixup.exe of the SDK300.001 package
(PS3_SDK-300_001.zip) has been fixed in this package.

Please use this package by expanding it in an environment in which 
the packages related to SDK300.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
+---host-win32
     +---bin
          |---binary2elf.exe
          |---ppu-lv2-prx-exportpickup.exe
          |---ppu-lv2-prx-fixup.exe
          |---ppu-lv2-prx-info.exe
          |---ppu-lv2-prx-libgen.exe
          +---ppu-lv2-prx-strip.exe

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
<Bug fix>
- When it was attempted to create a PRX that contains a compiled object 
  by specifying -mno-toc option, the following error was displayed. 
  This problem has been fixed. 

    Panic !! There is no NOP after BL. It's can't call other PRX.

  See also the following Technical Notes.

  - Limitations of Using the -mno-toc Option with PRX
    (Technical Notes ID:200907-14  https://ps3.scedev.net/technotes/view/941)

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this software conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc). 

----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
"PlayStation" is a registered trademark of Sony Computer Entertainment Inc.

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
