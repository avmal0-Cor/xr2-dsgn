[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 230.004
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
Bugfix for the MultiStream of the SDK230.001 package (PS3_SDK-230_001.zip)
has been added to this package. 

Please use this package by expanding it in an environment in which 
the packages related to SDK230.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_multistream_*.txt
+---target
     +---ppu
          |---include
          |    +---cell
          |         +---mstream.h
          +---lib
               |---libmstreamSPURSAT3.a
               |---libmstreamSPURSMP3.a
               |---libmstreamThreadAT3.a
               +---libmstreamThreadMP3.a

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
<Bug fix>
- When playing back ATRAC(TM) streams, if the time domain is used 
  in the multiple subbuses, a distortion occurs in the audio output.
  This problem has been fixed. 
  Note that this problem does not occur with MP3 playback.

----------------------------------------------------------------------
Known bugs, Limitations and Cautions of libmstream
----------------------------------------------------------------------
- The *.pic file can be found under cell/samples/sdk/sound/multistream/
  dspeffectsm, however, use the PPU object file(*.ppu.o), that is located 
  under cell/target/spu/lib/pic/multistream, in the game application. 

- cellMSSystemConfigureSysUtil() is to be deleted in the future. 
  Please migrate to cellMSSystemConfigureSysUtilEx(), that has been added 
  to SDK200.002, in the game application. 

- In Time Domain only mode, ADPCM data at low sample rates (< 24khz) 
  can cause noise.

(Added to Release 220.002)
- When samples are compiled individually using the project file that is 
  contained in each sample, it is required to build dependencies among 
  the following samples.
  Please refer to readme_*.txt of each sample for more details.
  Note that when the whole samples are compiled using the project files 
  (samples.sln/samples-2005.sln) or the makefile contained immediately under 
  samples/sdk/sound/multistream, the dependencies can be ignored. 

  dsp_sample
  eq_sample
  rain_sample
  simple_created_td
  simple_created_fd

(Added to Release 220.002)
- When each sample is compiled using the SNC compiler, the sample data may not 
  be loaded correctly because the environment variable CELL_SDK is used in 
  the sample data path. 

  When the CELL_SDK is defined with a path other than "/usr/local/cell/", 
  it is required to alter the SAMPLE_CELL_SDK path in the sample source 
  according to the environment.

  This problem does not occur when the GCC compiler is used.

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
"ATRAC" is a trademark of Sony Corporation.

"DTS" is a registered trademark of DTS, Inc.

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

