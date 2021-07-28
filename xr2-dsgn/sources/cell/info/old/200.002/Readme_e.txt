[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 200.002
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(TM) (Reference Tool). 

From SDK 200 onwards, the update data for the Reference Tool 
(PS3UPDAT.PUP.XXX.XXX) that was provided under 
cell/target/updater/ref-tool has been deleted from the SDK package. 
Instead, it is provided in a separate package 
(PS3DECRUpdateData.200.002.zip).

Also, the location of the documents that are contained in the SDK 192 
package under cell/info have been changed. Please refer to the documents 
in the following structure:

  [Documents under cell/info/]
   - Important_Changes: SDK200 new features,chages,notes
   - Notice: known bugs, and limitations
   - Bugfix: bug fix

  [Documents under cell/info/text/]
   - The text files of Important_Changes, Notice and Bugfix

  [Documents under cell/SDK_doc/jp(en)/pdf/development_basic]
   - Software Setup Guide 
   - Reference System Supplement
   - PLAYSTATION(R)3 Debugging Station Software Setup Guide
   - Reference Tool Instruction Manual Appendix  (*)
   - Reference Tool Instruction Manual  (*)

(*) Contents have not been changed from the SDK192 documents, however, 
    the file names have been changed as follows:

    Old :Reference_Tool_Operating_Instructions_Appendix_*.pdf
    New :Reference_Tool-Instruction_Manual_Appendix_*.pdf

    Old :Instruction_manual_*.pdf
    New :Reference_Tool-Instruction_Manual_*.pdf


When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 

  For Programmers
  ===============
     PS3_CP_Update-190_002.zip                  Reference Tool CP update file
     PS3_SDK_Sample-200_002.zip                 SDK Samples and Sample Data
     PS3_Toolchain-Win(/Linux)_402_200_002.zip  gcc 402/gdb Windows and Linux version
     PS3_Toolchain-Win(/Linux)_411_200_002.zip  gcc 411/gdb Windows and Linux version
     PS3DECRUpdateData.200.002.zip              Update data for Reference Tool
     PS3_SDKDoc-200_01-Latest-*.zip             Documents
     PS3_SDK_libpad_for_Windows-1000.zip        The libpad for Windows package
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins
     PS3_PASuite-200_002.zip                    Performance analyzer tools

     edge-030.zip                               PLAYSTATION(R)Edge
     PS3_PSSG-121.zip                           PSSG
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  For Artists
  ===========
  - Graphics:
      PS3_PSSGArtTools-121.zip          PSSGArt Tools
      FX_Composer2-20.zip               FX Composer2
      FX_Composer2-24beta4.zip          FX Composer2.4 beta4 

   - Sound:
      MultiStream_Tools-136.zip         MultiStream Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      SEdit-1200.zip                    SEdit
      VAGconverter-1.0.zip              VAG Converter
      PS3_at3tool-2000.zip              at3tool

  - Codec:
      PAMF_Tools-250-*.zip              PAMF Tools
      PAMF_Tools-Command_Line-250.zip   PAMF Tools(Command Line) 

  Creating Disc
  =============
      PS3_Generator-165.zip              Disc Image Generator
      PS3_Checker-165.zip                Disc Image Checker

The following component is not included in the above lists.

  For Programmers
  ===============
    PS3_SPUsim-XXX_XXX.zip              SPU Simulator

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
|    |---*.pdf
|    +---text/*.txt
|---SDK_doc/jp(en)/pdf/
|    +---development_basic
|---host-common
|---host-linux              linux version of utility
|---host-win32              windows version of utility
+---target
     |---updater            Flash Memory update file (directory only)
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
SDK200 Environment Setting
----------------------------------------------------------------------
Once the package is expanded, the software for the communication 
processor must be updated first. 
Please refer to "Reference Tool Software Setup Guide" for the detailed 
updating procedure, and please note the following points when updating. 

- If you are using the environment of SDK100 or before, please write 
  the Flash Memory with the ebootrom included in SDK100 first.

- Please note that the operation of when updated directly 
  from the environment of SDK100 or before to SDK200 is not guaranteed. 

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

