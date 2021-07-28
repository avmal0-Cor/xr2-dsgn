[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime libgcm Library 132.003
                    Copyright(C) 2006 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================
This package is based on libgcm and libgcm_pm of SDK 132.002 and 
includes the following changes. 

If you are going to use the newest SDK132 environment, please expand 
PS3_SDK-132_002.zip before expanding this package. 

---------------------------------------------------------------------
Contents of This Package
---------------------------------------------------------------------
cell/
|---Readme_libgcm*.txt
|---SDK_doc                 Document files
|---target
|    |---ppu
|    |    |---include
|    |    +---lib
|    +---spu
|         |---include
|         +---lib
+---samples                 Sample directory

---------------------------------------------------------------------
Changes from SDK132.002
---------------------------------------------------------------------
libgcm
======
<New Additions>
- The inline function of cellGcmSetTransferLocation() has been added. 

- The command generation functions of libgcm are provided as 
  the SPU static library and inline functions. For details, 
  please refer to "libgcm Reference". 

  According to this, the following files have been added: 
    target/spu/lib/libgcm_spu.a
    target/spu/include/cell/gcm_spu.h
    target/spu/include/cell/gcm/*.h

- The following sample has been added: 
  samples/gcm/Dice-spu
  This sample demonstrates how to use more than one command buffer 
  that is generated on SPU. 

<Specification Changes>
- Since the SPU version library has been provided, declarations and 
  definitions that are common between PPU and SPU have been separated 
  from the conventional header files and moved to the following 
  header files that have been newly added: 
    gcm_enum.h: Enumeration definition file
    gcm_error.h: Error definition file
    gcm_struct.h: Structure definition file
    gcm_commnad_c.h: C function mode definition file
    gcm_command_cpp_explicit.h: C++ function mode definition file
                                (explicit context data)
    gcm_command_cpp_implicit.h: C++ function mode definition file
                                (implicit context data)
    gcm_implementation_sub.h: Inline function definition file 
                              for common use between PPU and SPU
    gcm_global_implementation_sub.h: External function definition file 
                                     for common use between PPU and SPU
    gcm_prototypes_sub.h: Prototype declaration file for common use 
                          between PPU and SPU

  Similarly, declarations and definitions that are used only with PPU 
  have been separated from the conventional header files and moved to 
  the following header files that have been newly added: 
     gcm_implementation_ppu.h: Inline function definition file 
                               to be used only with PPU
     gcm_global_implementation_ppu.h: External function definition file 
                                      to be used with PPU
     gcm_prototypes_ppu.h: Prototype declaration file to be used 
                           only with PPU

<Bugs Fixed>
- The code that was performing pass-by-value, from a floating point 
  value to an unsigned integer value, with a pointer type has been 
  changed so that pass-by-value is performed with a union type. 
  According to this, CELL_GCM_ENDIAN_SWAP_F() and CELL_GCM_SWAP_HALFS_F() 
  have been deleted from gcm_macros.h, and a change has been made 
  so as to use the union type CellGcmCast that has been added to 
  gcm_struct.h. 

  The following is the list of functions and macros that have been 
  modified so that pass-by-value and memory store are performed with 
  CellGcmCast: 

    [Functions]
    cellGcmSetDepthBounds()
    cellGcmSetPointSize()
    cellGcmSetPolygonOffset()
    cellGcmSetPolygonStipplePattern()
    cellGcmSetVertexData4f()
    cellGcmSetVertexProgramParameterBlock()
    cellGcmSetVertexProgramConstants()
    cellGcmSetFogParams(),

    [Macros]
    CELL_GCM_SET_POLYGON_STIPPLE_PATTERN()
    CELL_GCM_SET_VIEWPORT_OFFSET_AND_SCALE()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_16U_XYZ0()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_16U_XYZW()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_32U()


- The enumeration constant CELL_GCM_FUNC_FUNC_ADD_SIGNED has been 
  modified to CELL_GCM_FUNC_ADD_SIGNED. 

- With samples/gcm/pmZcull, it has been modified so as to change 
  the display size according to the display resolution. 

libgcm_pm
=========
<New Additions>
- Measurement target events to be set with cellGcmSetPerfMonEvent() 
  have been added: 
     CELL_GCM_PM_GCLK_SETUP2RASTER_ACTIVE
     CELL_GCM_PM_GCLK_SETUP2RASTER_IDLE
     CELL_GCM_PM_GCLK_SETUP2RASTER_STALLING
     CELL_GCM_PM_GCLK_SETUP2RASTER_STARVING
     CELL_GCM_PM_GCLK_INSTRUCTION_ISSUED_VPE0
     CELL_GCM_PM_GCLK_CRASTER_OUTPUT_TILE_COUNT
     CELL_GCM_PM_GCLK_CRASTER_SEARCH_MODE
     CELL_GCM_PM_GCLK_IDX_VTX_CACHE_HIT
     CELL_GCM_PM_GCLK_SETUP_POINTS
     CELL_GCM_PM_GCLK_SETUP_LINES
     CELL_GCM_PM_GCLK_VPC_CULL_WOULD_CLIP
     CELL_GCM_PM_GCLK_IDX_FETCH_FBI

     CELL_GCM_PM_RCLK_CROP_COMP_WRITE
     CELL_GCM_PM_RCLK_ZROP_COMP_READ_COMP_WRITE
     CELL_GCM_PM_RCLK_ZROP_NO_READ_COMP_WRITE
     CELL_GCM_PM_RCLK_ZROP_UNCOMP_READ_COMP_WRITE
     CELL_GCM_PM_RCLK_ZROP_TOTAL_WRITTEN
     CELL_GCM_PM_RCLK_CROP_SUBPACKET_COUNT

     CELL_GCM_PM_SCLK_SQD_ENDSEG_RASTER_TIMEOUT
     CELL_GCM_PM_SCLK_TEXTURE_ANISO_COUNT_01
     CELL_GCM_PM_SCLK_TEXTURE_ANISO_COUNT_02
     CELL_GCM_PM_SCLK_TEXTURE_ANISO_COUNT_04
     CELL_GCM_PM_SCLK_TEXTURE_ANISO_COUNT_08
     CELL_GCM_PM_SCLK_TEXTURE_ANISO_COUNT_16
     CELL_GCM_PM_SCLK_TEXTURE_QUAD_COUNT
     CELL_GCM_PM_SCLK_SHADER_ALL_QUADS
     CELL_GCM_PM_SCLK_SHADER_FIRST_PASS_QUADS
     CELL_GCM_PM_SCLK_SIF_FETCH_TEXTURE_PASS_COUNT
     CELL_GCM_PM_SCLK_SIF_FETCH_NO_TEXTURE_PASS_COUNT
     CELL_GCM_PM_SCLK_FINERASTERBUNDLES
     CELL_GCM_PM_SCLK_L2_TEXTURE_CACHE_SET3_MISS_COUNT
     CELL_GCM_PM_SCLK_L2_TEXTURE_CACHE_SET3_REQUEST_COUNT
     CELL_GCM_PM_SCLK_L2_TEXTURE_CACHE_SET4_MISS_COUNT
     CELL_GCM_PM_SCLK_L2_TEXTURE_CACHE_SET4_REQUEST_COUNT

     CELL_GCM_PM_SCLK_PREROP_VALID_PIXEL
     CELL_GCM_PM_MCLK_FB_IDLE
     CELL_GCM_PM_MCLK_FB_RW_IDLE
     CELL_GCM_PM_MCLK_FB_STALLED
     CELL_GCM_PM_MCLK_FB_COMPRESSED_WRITE
     CELL_GCM_PM_MCLK_FB_NORMAL_WRITE
     CELL_GCM_PM_MCLK_FB_COMPRESSED_READ
     CELL_GCM_PM_MCLK_FB_NORMAL_READ
     CELL_GCM_PM_MCLK_FB_CROP_READ
     CELL_GCM_PM_MCLK_FB_CROP_WRITE
     CELL_GCM_PM_MCLK_FB_TEXTURE_READ
     CELL_GCM_PM_MCLK_FB_ZROP_READ

---------------------------------------------------------------------
Permission and Restrictions on Use
---------------------------------------------------------------------
The permission and restrictions on using this package conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc).  

---------------------------------------------------------------------
Note on Trademarks
---------------------------------------------------------------------
"PLAYSTATION" is a registered trademark of Sony Computer Entertainment
Inc. 

All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally trademarks and/or registered trademarks of their respective 
owners. 

---------------------------------------------------------------------
Copyrights
---------------------------------------------------------------------
The copyright of this software belongs to Sony Computer Entertainment
Inc. 
