[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 180.006
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes new features and bug fixes of PSGL 
in addition to SDK180.002 package (PS3_SDK-180_002.zip).

Please use this package by expanding it in an environment in which 
the packages related to SDK180.002 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_PSGL_*.txt
|---license
|    +---others
|         +--Aladdin_Enterprises.txt
+---target
     |---ppu
     |    |---include
     |    |    |---GLES
     |    |    |    |---gl.h
     |    |    |    +---glext.h
     |    |    +---PSGL
     |    |         |---deprecated.h
     |    |         |---export.h
     |    |         |---psgl.h
     |    |         |---psglu.h
     |    |         +---report.h
     |    |---lib
     |    |    +---PSGL
     |    |         +---RSX
     |    |              |---debug
     |    |              |    |---libPSGL.a
     |    |              |    |---libPSGLFX.a
     |    |              |    |---libPSGLU.a
     |    |              |    +---libPSGLcgc.a
     |    |              |---gcmhud
     |    |              |    |---libPSGL.a
     |    |              |    |---libPSGLFX.a
     |    |              |    |---libPSGLU.a
     |    |              |    +---libPSGLcgc.a
     |    |              |---opt
     |    |              |    |---libPSGL.a
     |    |              |    |---libPSGLFX.a
     |    |              |    |---libPSGLU.a
     |    |              |    +---libPSGLcgc.a
     |    |              +---ultra-opt
     |    |                   |---libPSGL.a
     |    |                   |---libPSGLFX.a
     |    |                   |---libPSGLU.a
     |    |                   +---libPSGLcgc.a
     |    +---src
     |         +---PSGL
     +---spu
          |---include
          |    +---PSGL
          |         +---spu_psgl.h
          +---lib
               +---PSGL
                    +---RSX
                         |---debug
                         |    +---libspuPSGL.a
                         |---gcmhud
                         |    +---libspuPSGL.a
                         |---opt
                         |    +---libspuPSGL.a
                         +---ultra-opt
                              +---libspuPSGL.a

----------------------------------------------------------------------
Changes
----------------------------------------------------------------------
<New Features>
- psglRescAdjustAspectRatio() that is the wrapper function of 
  cellRescAdjustAspectRatio() has been added.
- GL_MAX_COLOR_ATTACHMENTS_OES can be now specified to "pname",
  the first argument of glGetIntegerv().

<Specification change>
- The following functions in Shader.cpp have been moved :
    void _jsCreatePushBuffer( _CGprogram *program )
    void _jsSetDefaultValuesVP( _CGprogram *program )
    void _jsSetDefaultValuesFP( _CGprogram *program )
    void _jsPrepareFPSharedPushBuffer( _CGprogram *program, jsFifoData * &jsGcmCurrent 

  (Before being moved) src/PSGL/jetstream/src/Raster/JSGCM/Shader.cpp
  (After being moved) src/PSGL/jetstream/src/Raster/JSGCM/GCMShader.cpp

<Bug Fixes>
- The following problem has been fixed.

  - When a new rendering target was created, cellGcmSetAntialiasingControl() 
    was not called causing the default antialias setting set by 
    psglCreateDeviceAuto()/psglCreateDeviceExtended() to be inherited. 

- The following problem has been fixed.

  - When the PSGL RESC feature is enabled, the dedicated memory was allocated 
    internally, however, the allocated buffer was not released even though 
    psglDestroyDevice() was executed. 

- The following problem has been fixed.

  - When GL_BGRA was specified to "internalformat", the argument of 
    glTexImage2D() or the like, an error GL_INVALID_ENUM was returned and 
    it was unable to set it correctly. 

- The following problem has been fixed.

  - When glGenerateMipMapsOES() was executed for a texture that has smaller 
    widths than the height, RSX(TM) could not handle the processing correctly 
    and an error log was output to the tty. 

- The following problem has been fixed.

  - When cgGetParameterResource() was executed for an element in the array 
    which was unused within the shader, CG_C was returned incorrectly 
    instead of CG_UNDEFINED.

- The following problem has been fixed.

  - Wen cgCreateProgram() was executed for a fragment shader that has a 
    common parameter, the fragment shader could not be read correctly.

- The following problem has been fixed.

  - When glTextureReferenceSCE() is called with insufficient memory to create 
    a texture, the memory allocation fails. However, GL_OUT_OF_MEMORY is not 
    correctly returned as an error. The texture is created when glFlush() is 
    called, however an assert may occur due to this problem. 

- The following problem has been fixed. 

  - glCompressedTexImage2D() is not correctly handled with the DXT texture 
    that is not a multiplier of 2. The texture information managed by 
    the PSGL internally is updated by glFlush() or the like, however 
    the RSX(TM) error log may be output to the tty depending on the timing. 

- The following problem has been fixed. 

  - When PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE is specified to 
    "enable" of the PSGLdeviceParameters structure and 
    psglCreateDeviceExtended() is called, it may hang up within psglSwap().

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
None

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

----------------------------------------------------------------------
Note on License
----------------------------------------------------------------------
When using the following library, the copyright and license notices are
required.  However, the opt/ultra-opt version (not the debug version), 
which is used by a title application, does not include this software; 
therefore, it is unnecessary to have the notice if the application 
only uses the opt/ultra-opt version of this library.

libPSGL (debug version)
  Uses the MD5 Message-Digest Algorithm of Aladdin Enterprises available
  at http://sourceforge.net/project/showfiles.php?group_id=42360.
  
  Please refer to cell/license/others/Aladdin_Enterprises.txt.
