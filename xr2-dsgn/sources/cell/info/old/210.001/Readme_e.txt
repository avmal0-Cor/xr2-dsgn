[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 210.001
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(TM) (Reference Tool). 

//////////////////////////////////////////////////////////////////////
(Updated as of 2008/01/07)
- PS3ToolUpdater.self(/usr/local/cell/target/images/) that is contained 
  in the SDK package has been updated. 
  Changes are as shown below:
  - A problem that the system software of the Debugging Station was unable 
    to be updated correctly has been fixed. 
  - It has been changed to delete the registry when downgrading 
    the system software 2.10 to a version.
    This is a workaround for the problem described in the following technote:
    https://ps3.scedev.net/technotes/view/664
  (Warning)
      Using the new PS3ToolUpdater.self to downgrade the 
      Debugging Station system software from Version 2.10 
      will delete the registry and initialize network settings.
      For this reason, the debugger will temporarily become unusable
      when the Debugging Station is rebooted after the downgrade
      (a blank screen will be displayed).
      After downgrading from 2.10, always press the POWER switch
      on the front panel for at least 5 seconds and boot from the 
      system software.
      When the system software is up, configure network settings
      from the system software in order to use the debugger as
      before.
      Because the Reference Tool uses DEV LAN (the development 
      network), the debugger will continue to be usable.
      However, like the Debugging Station, its LAN settings will 
      be initialized, so it will be necessary to configure network 
      settings again from the system software.

(Updated as of 2008/01/07)
- Changes and restrictions for cgc have been added to the following 
  documents.
  info/Important_Changes_*.pdf
  info/Notice_SDK_*.pdf
  info/Bugfix_SDK_*.pdf
  info/text/Important_Changes_*.txt
  info/text/Notice_SDK_*.txt
  info/text/Bugfix_SDK_*.txt
//////////////////////////////////////////////////////////////////////

The following are included in the cell/info and the cell/SDK_doc directories. 
Please make sure to read them.

  [Documents under cell/info/]
   - Important_Changes: SDK210 new features,chages,notes
   - Notice: known bugs, and limitations
   - Bugfix: bug fix

  [Documents under cell/info/text/]
   - The text files of Important_Changes, Notice and Bugfix

  [Documents under cell/SDK_doc/jp(en)/pdf/development_basic]
   - Software Setup Guide 
   - Reference System Supplement
   - PLAYSTATION(R)3 Debugging Station Software Setup Guide
   - Reference Tool Instruction Manual Appendix
   - Reference Tool Instruction Manual  (*)

  (*) Contents have not been changed from the SDK200 documents.

When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 

  For Programmers
  ===============
     PS3_CP_Update-210_001.zip                  Reference Tool CP update file
     PS3_SDK_Samples-210_001.zip                SDK Samples and Sample Data
     PS3_Toolchain-Win(/Linux)_402_210_001.zip  gcc 402/gdb Windows and Linux version
     PS3_Toolchain-Win(/Linux)_411_210_001.zip  gcc 411/gdb Windows and Linux version
     PS3DECRUpdateData.210.001.zip              Update data for Reference Tool
     PS3_SDKDoc-210_01-Latest-*.zip             Documents
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins
     PS3_PASuite-200_002.zip                    Performance analyzer tools

     edge_0_3_4.zip                             PLAYSTATION(R)Edge
     PS3_PSSG-130.zip                           PSSG
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  For Artists
  ===========
  - Graphics:
      PS3_PSSGArtTools-130.zip          PSSGArt Tools
      FX_Composer2-24.zip               FX Composer2.4

   - Sound:
      MultiStream_Tools-138.zip         MultiStream Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      SEdit-1200.zip                    SEdit
      VAGconverter-1.0.zip              VAG Converter
      PS3_at3tool-2000.zip              at3tool

  - Codec:
      PAMF_Tools-310-*.zip              PAMF Tools
      PAMF_Tools-Command_Line-310.zip   PAMF Tools(Command Line) 

  Creating Disc
  =============
      PS3_Generator-175.zip              Disc Image Generator
      PS3_Checker-175.zip                Disc Image Checker

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
SDK210 Environment Setting
----------------------------------------------------------------------
Once the package is expanded, the software for the communication 
processor must be updated first. 
Please refer to "Reference Tool Software Setup Guide" for the detailed 
updating procedure, and please note the following points when updating. 

- If you are using the environment of SDK100 or before, please write 
  the Flash Memory with the ebootrom included in SDK100 first.

- Please note that the operation of when updated directly 
  from the environment of SDK100 or before to SDK210 is not guaranteed. 

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

