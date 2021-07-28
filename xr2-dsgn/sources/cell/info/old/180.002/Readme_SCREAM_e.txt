[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 180.002
                   Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved
======================================================================
This package includes the SCREAM Runtime Library.

The SCREAM Runtime Library offers the following advanced features:
- Sound scripting
- Plug-in support
- Priority-based voice management

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_*.txt
|---SDK_doc
|     +---jp/en
|          +---pdf
|               +---sound_SCREAM
|                      |---SCREAM-Overview_*.pdf
|                      +---SCREAM-Reference_*.pdf
|---sample_data
|---samples
|    +---sound
|         +---SCREAM
+---target
     +---ppu
          |---include
          |    +---scream.h
          +---lib
               +---libscream.a

----------------------------------------------------------------------
Changes in Release 180.002
----------------------------------------------------------------------
[New Addition]
- In order to synchronize with the MultiStream processing, 
  msFlags member of CellScreamInit structure has been added.

- When cellScreamStartSoundSystem() is executed and msFlags 
  of CellScreamInit structure is invalid, it now returns 
  CELL_SCREAM_ERROR_BAD_FLAGS.
  Accordingly to this change, scream.h has been added to the above 
  CELL_SCREAM_ERROR_BAD_FLAGS macro.
  
- cellScreamMSSync() has been changed so that the timing synchronization 
  depends on the status of msFlags member of CellScreamInit structure.

[Sample change]
- In the following sample programs, libmstreamSPURSMP3.a is now 
  linked from libmstreamSPURS.a.
  cell/samples/sound/SCREAM/scream_routing
  cell/samples/sound/SCREAM/simple_scream

[SCREAM Overview]
Chapter 3: Initializing, Updating and Closing
- Initializing MultiStream and SCREAM section: Added information 
  regarding time-domain mode of MultiStream
- SCREAM/MultiStream Tips section: Added information regarding incorrect 
  timing in SCREAM

[SCREAM Reference]
Structures
- CellScreamInit section: Added msFlags member and its description

System Setup Functions
- cellScreamStartSoundSystem section: Added return value 
  CELL_SCREAM_ERROR_BAD_FLAGS
- cellScreamMSSync section: Added description regarding timing between 
  SCREAM and MultiStream

---------------------------------------------------------------------
Permission and Restrictions on Use
---------------------------------------------------------------------
The permission, restrictions, etc. on using this software conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc). 

---------------------------------------------------------------------
Note on Trademarks
---------------------------------------------------------------------
"PLAYSTATION" is either registered trademarks or trademarks of 
Sony Computer Entertainment Inc.

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally trademarks and/or registered trademarks of their respective 
owners. 

---------------------------------------------------------------------
Copyrights
---------------------------------------------------------------------
The copyright of this software belongs to Sony Computer Entertainment
Inc. 
