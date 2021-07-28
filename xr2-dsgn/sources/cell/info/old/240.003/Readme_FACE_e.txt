[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 240.003
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package contains files relating to the libface library.

libface is a library that analyzes grayscale images to detect 
human faces in the images. libface provides the following functions.
* Detects face(s) in a grayscale image
* Detects the eyes, nose, and mouth in a detected face
* Decides whether or not a detected face is smiling

Please refer to the documents contained in this package for more details.

Please use this package by expanding it in an environment in which 
the packages related to SDK240.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
|---SDK_doc
|---sample_data
|    +---face
|---samples
|    +---sdk
|         +---face
+---target
     |---common
     |    +---include
     |         +---cell
     |              |---face\
     |              +---face.h
     |---ppu
     |    +---lib
     |         +---libface.a
     +---spu
          +---lib
               +---libface.a

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
