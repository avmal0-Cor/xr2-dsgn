[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 100.002
                    Copyright(C) 2006 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(R) (Reference Tool). 
It does not operate on the CEB series. 

In this package, the following components are not included. 
  - PSSG
  - COLLADA_DOM

The following are included in the info directory. 
Please make sure to read them.

  - Software Setup Guide
  - Important_Changes: SDK100 new features,chages,notes
  - Notice: known bugs, and limitations
  - Bugfix: bug fix
  - PLAYSTATION(R)3 FAQ   (*)
  - Reference System Supplement
  - Reference Tool Operating Instructions
  - RefToolHW_operating_instructions/  
      DEH/    Operating Instructions of Reference Tool DEH
              (English and Japanese instructions are in each files)
      DECR/   Operating Instructions of Reference Tool DECR

(*) The same document as SDK 096 is used since no change is made.

When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 
In SDK100, the packages other than the Documents package are not updated. 
As for the Reference Tool CP update file package and the Toolchain 
packages, please use the packages released in SDK094. 
As for the Performance Analyzer Tools package, please use the package 
released in SDK093. 

     PS3_CP_Update-094_002.zip         Reference Tool CP update file
     PS3_SDK_SampleData-100_002.zip    Sample Data 
     PS3_Toolchain-Win_094_002.zip     gcc/gdb Windows
     PS3_Toolchain-Linux_094_002.zip   gcc/gdb Linux
     PS3_SDKDoc-100_01-Latest-*.zip    Documents
     PS3_SDKDoc-100_01-(patch_from_096_01)-*.zip  Patch Document
     PS3_Generator-070.zip             Disc Image Generator
     PS3_Eclipse_Plugins-094_002.zip   Eclipse Plugins
     PS3_PA_093_002.zip                Performance Analyzer Tools 
     PS3_at3tool-0800.zip              at3tool
     PAMF_Tools-100.zip                PAMF Tools
     MultiStream_Tools-133.zip         MultiStream Tools
     SEdit-1200.zip                    SEdit
     VAGconverter-1.0.zip              VAG Converter

"Cell" is the code name for an advanced microprocessor being developed 
jointly by IBM Corporation, Sony Corporation, 
Sony Computer Entertainment Inc. and Toshiba Corporation.

Please note that the specifications are preliminary and subject to change 
without prior notice.  

----------------------------------------------------------------------
Environment for Operation Check
----------------------------------------------------------------------
Linux Environment
   Red Hat Professional Workstation (Linux 2.4.21)
   Red Hat Enterprise Linux 4 WS (Linux 2.6.9)
   SUSE Linux Professional 9.2 (Linux 2.6.8)

Windows Environment
   Windows XP Professional Service Pack 2

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_*.txt
|---version-SDK             Release version file
|---license                 Trademarks and Licenses file
|    |---license_*.txt
|    +---others
|---info
|    +---RefToolHW_operating_instructions
|---host-common
|---host-linux              linux version of utility
|---host-win32              windows version of utility
|---target
|    |---bootrom            Flash Memory image file(ebootrom)
|    |---images             executable image files (system files)
|    |---common
|    |    +---include
|    |---ppu
|    |    |---include
|    |    +---lib
|    +---spu
|         |---include
|         +---lib
+---samples

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this package conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc).  

----------------------------------------------------------------------
Trademarks and Licenses
----------------------------------------------------------------------
For the trademarks and licenses of the software included in 
the Reference Tool and this package, please refer to the files within 
the license folder. 

