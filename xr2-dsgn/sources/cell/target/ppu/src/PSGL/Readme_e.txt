[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                  Copyright (C) 2009 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
======================================================================
This package contains the PlayStation(R)3 Graphics Library source code
and prebuilt binaries.

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
  |---Readme_*.txt
  +---target
        +---ppu
        |   +---src
        |   |   +---PSGL (source directory)
        |   |       +---jetstream
        |   |       |   |---Makefile (top level makefile)
        |   |       |   +---PSGL.sln (top level Visual Studio 2003 solution)
        |   |       +---cgc
        |   |       +---jsbuild (build output directory)
        |   +---lib
        |   |   +---PSGL
        |   |       +---RSX (prebuilt libraries)
        |   |            |---debug
        |   |            |---gcmhud
        |   |            |---opt
        |   |            +---ultra-opt
        |   +---include (SDK public headers)
        +---spu
            +---lib
            |   +---PSGL
            |       +---RSX (prebuilt libraries)
            |            |---debug
            |            |---gcmhud
            |            |---opt
            |            +---ultra-opt
            +---include (SDK public headers)

----------------------------------------------------------------------
How to build
----------------------------------------------------------------------
To build the PSGL libraries, you would need to have the PlayStation(R)3
SDK installed and set up with necessary environment variables.

* Visual Studio Build
You will also need to have ProDG with Visaul Studio Integration installed
to build PSGL with Visual Studio 2003 or 2005.
Please refer to the restriction above that you will also need to apply
online updates available for it in order to build properly.  

The to build all PSGL Libraries with Visual C++ 2003 or 2005 see 
the projects found here: 
cell/target/ppu/src/PSGL/jetstream/PSGL-2003.sln
cell/target/ppu/src/PSGL/jetstream/PSGL-2005.sln.

You can select each build flavors:
  Debug, GcmHud, Opt, Ultra Opt

The final libraries will be built under the folder
  cell/target/ppu/src/PSGL/jsbuild/target
layed out just as in the SDK $CELL_SDK/target folder.
If you replace the libraries with the ones that you have just built,
it will allow you to use your own build of PSGL in the SDK environment.

* MSYS or Linux Build
  cell/target/ppu/src/PSGL/jetstream/Makefile
Is the top level Makefile for building PSGL under MSys or Linux.
The following environment variables should be set in addition
  export OS_VERSION=X_Y_Z
  export JS_OSDIR=$CELL_SDK
  export JS_RASTERIZER=JSGCM
Where "X_Y_Z" stands for the SDK version, such as "1_8_0".
Four different flavors of PSGL libraries can be built with this package.
Build targets can be selected by setting up additional environment
variables JS_DEBUG, JS_NO_ERROR_CHECK, and JS_RUNTIME.
debug version:
  export JS_DEBUG=1
  export JS_NO_ERROR_CHECK=0
  export JS_RUNTIME=
opt version:
  export JS_DEBUG=0
  export JS_NO_ERROR_CHECK=0
  export JS_RUNTIME=
ultra-opt version:
  export JS_DEBUG=0
  export JS_NO_ERROR_CHECK=1
  export JS_RUNTIME=
gcmhud version:
  export JS_DEBUG=0
  export JS_NO_ERROR_CHECK=0
  export JS_RUNTIME=gcmhud
With in a concole window, change directory to
  cell/target/ppu/src/PSGL/jetstream
With the above environment variables set up, typing
  make
will generate the libraries and object files under the folder
  cell/target/ppu/src/PSGL/jsbuild
to the follwoing folders for the each build flavor
  cell-JSGCM-debugrsx-X_Y_Z (debug)
  cell-JSGCM-opt-gcmhudrsx-X_Y_Z (gcmhud)
  cell_JSGCM-opt-nocheckrsx-X_Y_Z (ultra-opt)
  cell-JSGCM-optrsx-X_Y_Z (opt)
Where "X_Y_Z" stands for the SDK version, such as "1_8_0".
The libraries can be found under the lib folder inside them.
Please copy libraries you built to this location inside the SDK release package
  $CELL_SDK/target/ppu/lib/PSGL/RSX
under their respective folders if you would like to replace the binaries.
This will allow you to use your own build of PSGL in the SDK environment.

* GCM HUD Build
PSGL HUD build (hud) is now replaced with GCM HUD (gcmhud),
it contains the hooks to enable use of GCM HUD,
but does not include the HUD library.
The latest version of HUD library can be downloaded from PS3 DevNet at:
  https://ps3.scedev.net/projects/rsx_tools
This package incorporates the latest package GCM HUD library for your convenience.
Please check back at the URL above for the latest updates for the GCM HUD.
As specified in the "GCM HUD Guide" document, the linker options need to be changed
in order to build and executable using the GCM HUD version of PSGL with GCM HUD.
To link with the GCM HUD, the following flags are required:
  -lgcm_hud -lgcm_pm -lsysmodule_stub -lusbd_stub -lio_stub -lm
  -lstdc++ -lsysutil_stub -lfs_stub -lsheap_stub
Do not link with libgcm_cmd.a and libgcm_sys_stub.a.
Further details for using GCM HUD is decribed in the document mentioned above.
If you have replaced the SDK PSGL libraries with your own, you can set
  export CELL_PSGL_VERSION=gcmhud
for building with the GCM HUD version of PSGL.

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
- Microsoft Visual Studio .NET 2003 Service Pack 1 (KB918007) is
  required to build the Visual Studio solutions and projects.

<About the Operation Guarantee>
- For the operation guarantee of the runtime libraries, the following
  two conditions apply:
  - The PSGL library that is provided in binary (*.a) format is used
    as it is
  - The libgcm API is not directly called and is not mixed with the PSGL
    API

  A binary that is created by building the source code, whether with
  or without changes, is also excluded from the operation guarantee of
  the runtime libraries (it is the same when the libgcm functions are
  directly called from a title application and is mixed with the PSGL
  API by understanding the contents of the source).

  Therefore, please note that it will be handled like an original
  runtime library created by a licensee.

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this software conform to
the contract concluded between your company and our company (Sony
Computer Entertainment Inc).

----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
"PlayStation" is a registered trademark of Sony Computer Entertainment Inc.
All other product and company names mentioned herein, with or without
the registered trademark symbol (R) or trademark symbol (TM), are
generally registered trademarks and/or trademarks of their respective
owners.

