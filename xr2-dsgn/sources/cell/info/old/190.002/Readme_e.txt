[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 190.002
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(TM) (Reference Tool). 

The following component is not included. 
  - Performance Analyzer Tools  
    (This package will be separated as PS3_PASuite-190_002.zip and
     PS3_SPUsim-190_002.zip. These packages will be released soon.)

The following are included in the info directory. 
Please make sure to read them.

  - Software Setup Guide
  - Important_Changes: SDK190 new features,chages,notes
  - Notice: known bugs, and limitations
  - Bugfix: bug fix
  - Reference System Supplement
  - PLAYSTATION(R)3 Debugging Station Software Setup Guide
  - Reference Tool Operating Instructions
  - RefToolHW_operating_instructions/DECR/ :
       Operating Instructions of Reference Tool DECR
  - text/ : The text files of Important_Changes, Notice and Bugfix

When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 
From SDK190 onwards, the sample programs under cell/samples has been 
consolidated with the sample data package (PS3_SDK_SampleData-XXX_XXX.zip) 
and will be provided in a separate package (PS3_SDK_Sample-190_002.zip).

     PS3_CP_Update-190_002.zip         Reference Tool CP update file
     PS3_SDK_Sample-190_002.zip        SDK Samples and Sample Data
     PS3_Toolchain-Win_402_190_002.zip   gcc 402/gdb Windows
     PS3_Toolchain-Linux_402_190_002.zip gcc 402/gdb Linux
     PS3_Toolchain-Win_411_190_002.zip   gcc 411/gdb Windows
     PS3_Toolchain-Linux_411_190_002.zip gcc 411/gdb Linux
     PS3_SDKDoc-190_01-Latest-*.zip    Documents
     PS3_Generator-150.zip             Disc Image Generator
     PS3_Checker-150.zip               Disc Image Checker
     PS3_Eclipse_Plugins-180_002.zip   Eclipse Plugins
     PS3_at3tool-2000.zip              at3tool
     PS3_PSSG-120.zip                  PSSG
     PS3_PSSGArtTools-120.zip          PSSGArt Tools
     PS3_COLLADA-Pipeline-190_002.zip  COLLADA Pipeline
     MultiStream_Tools-136.zip         MultiStream Tools
     SEdit-1200.zip                    SEdit
     VAGconverter-1.0.zip              VAG Converter
     SCREAM_Tools-101.zip              SCREAM Tools
     PAMF_Tools-250-*.zip              PAMF Tools
     PAMF_Tools-Command_Line-250.zip   PAMF Tools(Command Line) 

"Cell" is the code name for an advanced microprocessor being developed 
jointly by IBM Corporation, Sony Corporation, 
Sony Computer Entertainment Inc. and Toshiba Corporation.

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
|    |---RefToolHW_operating_instructions
|    +---text
|---host-common
|---host-linux              linux version of utility
|---host-win32              windows version of utility
+---target
     |---updater            Flash Memory update file
     |---images             executable image files (system files)
     |---tool               libprof.sprx
     |---common
     |    +---include
     |---ppu
     |    |---include
     |    +---lib
     +---spu
          |---include
          +---lib


----------------------------------------------------------------------
SDK190 Environment Setting
----------------------------------------------------------------------
Once the package is expanded, the software for the communication 
processor must be updated first. 
Please refer to "Reference Tool Software Setup Guide" for the detailed 
updating procedure, and please note the following points when updating. 

- If you are using the environment of SDK100 or before, please write 
  the Flash Memory with the ebootrom included in SDK100 first.

- Please note that the operation of when updated directly 
  from the environment of SDK100 or before to SDK190 is not guaranteed. 

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

