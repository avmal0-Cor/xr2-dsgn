[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 320.001
                    Copyright(C) 2010 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes the software required for developing application 
software with the hardware with RSX(TM) (Reference Tool). 

To use the software, extract this package in an environment where SDK310 
packages are extracted.

The following are included in the cell\info and the cell\SDK_doc directories. 
Please make sure to read them.

  [Documents under cell\info\]
   - Important_Changes: SDK320 new features,chages,notes
   - Notice: known bugs, and limitations
   - Bugfix: bug fix

  [Documents under cell\info\old\XXX]  (XXX:SDK release version)
   - The following files in each SDKXXX release version, in SDK310 and before.
      - Readme, Important_Changes, Notice_SDK, Bugfix_SDK, 
        Changes_Toolchain, Notice_Toolchain

  [Documents under cell\SDK_doc\jp(en)\pdf\development_basic]
   - Reference System Supplement 
   - Reference Tool Software Setup Guide 
   - Reference Tool DECR-1400 Software Setup Guide 
   - Debugging Station Software Setup Guide
   - Reference Tool Instruction Manual Appendix
   - Reference Tool(DECR-1000) Instruction Manual
   - Reference Tool(DECR-1400) Instruction Manual
   - Debugging Station(DECHJ00J/DECHJ00A) Instruction Manual
   - Debugging Station(DECHA00J/DECHA00A) Instruction Manual
   - Debugging Station(DECH-2000J/DECH-2000A) Instruction Manual

When using the software included in the SDK package, please make sure 
to download the following packages that are provided in separate packages. 

  For Programmers
  ===============
     PS3_CP_Update-240_001.zip                  Reference Tool CP update file
     PS3_SDK-310_001.zip                        SDK Libraries
     PS3_SDK_Samples-310_001.zip                SDK Samples and Sample Data
     PS3_SDK_Samples-320_001.zip                SDK Samples and Sample Data
     PS3_SDK_CGTUTORIAL-240_001.zip             CgTutorial Samples 
     PS3_Toolchain_411-Win_310_001.zip          gcc 411/gdb Windows version
     PS3DECRUpdateData.320.001.zip              Update data for Reference Tool
     PS3_SDKDoc-320_01-Latest-*.zip             Documents
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins

     edge_1_1_1.zip                             PlayStation(R)Edge
     PS3_PhyreEngine-260.zip                    PhyreEngine(TM)
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  For Artists
  ===========
  - Graphics:
      PS3_PhyreEngineArtTools-260.zip           PhyreEngine(TM)Art Tools
      PS3_GPAD-SDK310.2_8.5_Installer.zip       GPAD
      FX_Composer2-242.zip                      FX Composer2.4.2

   - Sound:
      Sulpha-10.zip                     Sulpha MultiStream Debugger
      PS3_VAGPreviewer-1100.zip         VAG Previewer 
      PS3_celptool-110.zip              celptool
      PS3_at3tool-3000.zip              at3tool
      MultiStream_Tools-141.zip         MultiStream Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      VAGconverter-1.0.zip              VAG Converter
      SEdit-1200.zip                    SEdit

  - Codec:
      PAMF_Tools-510-*.zip              PAMF Tools
      PAMF_Tools-Command_Line-510.zip   PAMF Tools(Command Line) 

  Creating Disc
  =============
     PS3_Generator_Tools-271.zip        Disc Image Generator Tools
     PS3_Checker-271.zip                Disc Image Checker


"Cell" is the code name for an advanced microprocessor being developed 
jointly by IBM Corporation, Sony Corporation, 
Sony Computer Entertainment Inc. and Toshiba Corporation.

----------------------------------------------------------------------
Environment for Operation Check
----------------------------------------------------------------------
Windows Environment
   Windows XP Professional Service Pack 2

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
|---Readme_*.txt
|---version-SDK             Release version file
|---license                 Trademarks and Licenses file
|    |---license_*.txt
|    +---others
|---info
|    |---*.txt
|    +---old\XXX
|---SDK_doc\jp(en)\pdf\
|    +---development_basic
|---host-common
|---host-win32              windows version of utility
+---target
     |---updater            Flash Memory update file (directory only)
     |---images             executable image files (system files)
     |---common
     |    +---include
     |---ppu
     |    |---include
     |    +---lib
     +---spu
          |---include
          +---lib


----------------------------------------------------------------------
SDK320 Environment Setting
----------------------------------------------------------------------
Once the package is expanded, the software for the communication 
processor must be updated first. 
Please refer to "Reference Tool Software Setup Guide" for the detailed 
updating procedure, and please note the following points when updating. 

- If you are using the environment of SDK100 or before, please write 
  the Flash Memory with the ebootrom included in SDK100 first.

- Please note that the operation of when updated directly 
  from the environment of SDK100 or before to SDK320 is not guaranteed. 

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

