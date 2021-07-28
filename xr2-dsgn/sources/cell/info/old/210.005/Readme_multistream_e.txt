[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 210.005
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
Bugfixes and an additional feature for the MultiStream of the SDK210.001
package (PS3_SDK-210_001.zip) have been added to this package. 

Please use this package by expanding it in an environment in which 
the packages related to SDK210.001 are expanded. 

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
<New feature>
- The function of cellMSStreamGetMP3FrameInfo() has been added. 
  cellMSStreamGetMP3FrameInfo() reference is as follow.

  #include <cell/mstream.h>
  int cellMSStreamGetMP3FrameInfo(
                                    int streamNum,
                                    int *numFrames,
                                    int *totalSizeOfFrames
                                      );

  Calling Conditions
  ==================
  - Can be called from an interrupt handler. Multithread safe.

  Arguments
  =========
  streamNum: MP3 stream channel number to retrieve information from.
  numFrames: Returns the number of MP3 frames played.
  totalSizeOfFrames: Returns the cummulative size in bytes of the frames played.

  Return Values
  =============
  Value  :  Description
   0     :  Success
  -1     :  Error.

  * If -1 is returned, use cellMSSystemGetLastError() to get a more meaningful
    error message. 

  Description
  ===========
  - Used to get the number of MP3 frames played on a stream since the last 
    cellMSStreamPlay() command. This information can be used to work out 
    the correct playback time of the MP3 stream.

<Bug fixes>
- (B#47418)
  Reverb DSP could cause the stack buffer to overflow resulting 
  very rarely (approximately once in a week) in a crash.
  This problem has been fixed.

- (B#47530)
  Subbus paths could incorrectly output to the wrong channel 
  in TD only mode.(Atrac libs only) 
  This problem has been fixed.

- (B#47535)
  Time domain only envelope resolution too small. 
  This problem has been fixed.

- (B#47536)
  The cellMSStreamGetMP3FramePos() returned invalid value if polled before 
  next SPU cycle. This problem has been fixed.

- (B#47538)
  MP3 decoding started too soon on spu side. This could cause MP3 playback
  glitches. This problem has been fixed.

----------------------------------------------------------------------
Known bugs, Limitations and Cautions of libmstream
----------------------------------------------------------------------
(Added to Release 210.001)
- The *.pic file can be found under cell/samples/sdk/sound/multistream/
  dspeffectsm, however, use the PPU object file(*.ppu.o), that is located 
  under cell/target/spu/lib/pic/multistream, in the game application. 

(Added to Release 210.001)
- cellMSSystemConfigureSysUtil() is to be deleted in the future. 
  Please migrate to cellMSSystemConfigureSysUtilEx(), that has been added 
  to SDK200.002, in the game application. 

- In Time Domain only mode, ADPCM data at low sample rates (< 24khz) 
  can cause noise.

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

