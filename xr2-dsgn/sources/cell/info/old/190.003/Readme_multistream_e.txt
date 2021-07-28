[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 190.003
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes new features and bug fixes of MultiStream library 
in addition to SDK190.002 package (PS3_SDK-190_002.zip).

Please use this package by expanding it in an environment in which 
the packages related to SDK190.002 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_multistream_*.txt
|---samples
|    +---sdk
|         +---sound
|              +---multistream
|                   +---dspeffects
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
<Bugfixs>
- When calling cellMSDSPLoadFromMemory, if the CELL_MS_ALLOC_DSPHANDLE 
  parameter is passed to the argument handle, libmstream may take a long
  time to complete the allocation of the DSP handle. 
  This problem has been fixed.

- If the DSP effect is processed on a sub bus, the read/write areas of
  the DSP effect may be corrupted. This problem has been fixed.

- If streams are played using surround sound mode as well as non-surround
  sound mode by setting the pitch using cellMSStreamSetPitch, the streams
  are not played with the correct pitch. This problem has been fixed.

- Playback in Time domain only mode fails. This problem has been fixed.

- Playing streams in standard frequency domain mode causes audio to stutter.
  This problem has been fixed.

- When playing back very small samples multiple times, they cannot 
  be loop-played correctly. This problem has been fixed.
 
- cellMP3Init has been fixed so that it does not use memalign inside.

- When the PPU processing is delayed behind the libaudio processing, 
  it now outputs a warning message to TTY.

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
- In Time Domain only mode, ADPCM data at low sample rates (< 24khz) 
  can cause noise

- When the sound output is set based on the system utility information 
  using cellMSSystemConfigureSysUtil(), it is not set correctly. 
  Check the available audio mode using cellAudioOutGetSoundAvailability(), 
  then change the sound output using cellAudioOutConfigure().
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

