[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 180.002
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(TM) (Reference Tool). 

The following component is not included. 
  - COLLADA_DOM
  - Performance Analyzer Tools  (This will be released soon.)
  - PAMF Tools  (This will be released soon.)

The following are included in the info directory. 
Please make sure to read them.

  - Software Setup Guide
  - Important_Changes: SDK180 new features,chages,notes
  - Notice: known bugs, and limitations
  - Bugfix: bug fix
  - Reference System Supplement
  - PLAYSTATION(R)3 Debugging Station Software Setup Guide
  - Reference Tool Operating Instructions   (*)
  - RefToolHW_operating_instructions/  
      DEH/    Operating Instructions of Reference Tool DEH
              (English and Japanese instructions are in each files)
      DECR/   Operating Instructions of Reference Tool DECR
  - text/ : The text files of Important_Changes, Notice and Bugfix

(*) The same document as SDK 170 is used since no change is made.

When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 

     PS3_CP_Update-150_004.zip         Reference Tool CP update file
     PS3_SDK_SampleData-180_002.zip    Sample Data 
     PS3_Toolchain-Win_180_002.zip     gcc/gdb Windows
     PS3_Toolchain-Linux_180_002.zip   gcc/gdb Linux
     PS3_SDKDoc-180_01-Latest-*.zip    Documents
     PS3_SDK_SCREAM-180_002.zip        SCREAM Library
     PS3_Generator-120.zip             Disc Image Generator
     PS3_Checker-120.zip               Disc Image Checker
     PS3_Eclipse_Plugins-180_002.zip   Eclipse Plugins
     PS3_at3tool-2000.zip              at3tool
     PS3_PSSG-110.zip                  PSSG
     PS3_PSSGArtTools-110.zip          PSSGArt Tools
     MultiStream_Tools-136.zip         MultiStream Tools
     SEdit-1200.zip                    SEdit
     VAGconverter-1.0.zip              VAG Converter
     SCREAM_Tools-101.zip              SCREAM Tools

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
|---target
|    |---updater            Flash Memory update file
|    |---images             executable image files (system files)
|    |---tool               libprof.sprx
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
SDK180 Environment Setting
----------------------------------------------------------------------
Once the package is expanded, the software for the communication 
processor must be updated first. 
Please refer to "Reference Tool Software Setup Guide" for the detailed 
updating procedure, and please note the following points when updating. 

- If you are using the environment of SDK100 or before, please write 
  the Flash Memory with the ebootrom included in SDK100 first.

- Please note that the operation of when updated directly 
  from the environment of SDK100 or before to SDK180 is not guaranteed. 

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

