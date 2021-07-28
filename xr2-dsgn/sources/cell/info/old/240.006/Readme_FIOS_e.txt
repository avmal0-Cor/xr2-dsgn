[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 240.006
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package contains files relating to the FIOS(File I/O Scheduler).

FIOS is a library that improves the file access speed of games by 
providing a scheduling feature of file I/O requests, a data compression 
feature and a data cache feature.
Please refer to the documents contained in this package for more details.

Please use this package by expanding it in an environment in which 
the packages related to SDK240.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
|---SDK_doc
|---license
|    |---license_*.txt
|    +---others
|---host-win32
|    +---bin
|         +---psarc.exe
|---samples
|    +---sdk
|         +---fios
|              +---simple
+---target
     +---ppu
          |---include
          |    +---cell
          |         |---fios\
          |         +---fios.h
          +---lib
               +---libfios.a

----------------------------------------------------------------------
Environment Requirements
----------------------------------------------------------------------
- To use the file compression feature of FIOS, download and install 
  PLAYSTATION(R) Edge release 0.4.1 from the following URL:

  https://ps3.scedev.net/projects/psedge

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
- The compression algorism LZMA is described in 
  PSARC-Users_Guide_j(e).pdf, however, only Zlib is supported by the 
  current psarc program.

- It is described in the libfios reference that 
  op::getRequestedSize() and op::getFulfilledSize() return the same 
  value when the processing is completed, however, it may return a 
  different value.

- When scheduler::readDirectory() or scheduler::readDirectorySync() is 
  executed for a directory, and if the directory is deleted and then a 
  new directory is created with the same name as the deleted directory, 
  scheduler::readDirectory() and scheduler::readDirectorySync() may not 
  work correctly with the newly created directory.

- op::waitMultipleTimeout() does not timeout as specified. Please use 
  op::waitMultipleDeadline() instead.

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
All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

For the trademarks and licenses of the software included in 
the Reference Tool and this package, please refer to the files within 
the license folder. 
