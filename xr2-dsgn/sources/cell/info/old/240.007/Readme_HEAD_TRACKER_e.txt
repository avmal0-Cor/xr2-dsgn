[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 240.007
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package contains files relating to the libhead_tracker library.

libhead_tracker is an image recognition library that tracks the contour 
of a person's head in video input from PLAYSTATION(R)Eye. 
Because the library tracks contours, tracking is possible even when 
the target's face is hidden, or the target turns sideways or faces backwards.

Please refer to the documents contained in this package for more details.

Please use this package by expanding it in an environment in which 
the packages related to SDK240.001 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell\
|---SDK_doc
|---samples
|    +---sdk
|         +---vision
|              +---head_tracker
+---target
     |---common
     |    +---include
     |         +---cell
     |              |---head_tracker\
     |              +---head_tracker.h
     |---ppu
     |    +---lib
     |         +---libhead_tracker.a
     +---spu
          +---lib
               +---libhead_tracker.a

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
