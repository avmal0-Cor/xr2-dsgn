/************************************************************************
  SCE CONFIDENTIAL
  PlayStation(R)3 Programmer Tool Runtime Library 310.001

  gcm_method_data.h

  This header file is provided to assist in writing GCM Method Macros.

  Notice:
  This file is not included by <cell/gcm.h>

  Copyright (C) 2008 Sony Computer Entertainment Inc. All Rights Reserved.
*************************************************************************/

#ifndef __CELL_GCM_METHOD_DATA_H__
#define __CELL_GCM_METHOD_DATA_H__

/*---------------------------------------------------------------------------------------------------
  About CELL_GCM_ENDIAN_SWAP

  The target System is a big endian environment.
  However, some users build their Command Buffers in a little endian environment, like Windows or Linux on an x86 CPU.
  So the command buffer which is created in a little endian environment must have an endian swap applied to it.
  To ensure compatibility, GCM Method Macro uses the CELL_GCM_ENDIAN_SWAP macro which is provided by libgcm.
  In a big endian environment, CELL_GCM_ENDIAN_SWAP doesn't have any effect, but in a little endian environment it swaps 32bit data.

----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------
  Method Address
  The following declarations are base addresses which are usable in GCM Method Macros.

  Notice:
     Internally within GCM the term "TRANSFORM" is used to mean Vertex Shader.
     So "TRANSFORM_PROGRAM" means Vertex Shader Program.
     "NV4097" is also a term used internally, but is not used in GCM Method Macros.
     Calling "ADDRESS" is inconvenient, so we use "OFFSET" instead.
  --------------------------------------------------------------------------------------------------*/

#define CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM_LOAD         CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM_LOAD
#define CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM_START        CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM_START
#define CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM              CELL_GCM_NV4097_SET_TRANSFORM_PROGRAM
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TIMEOUT              CELL_GCM_NV4097_SET_TRANSFORM_TIMEOUT
#define CELL_GCM_METHOD_ADDRESS_VERTEX_DATA_ARRAY_OFFSET    CELL_GCM_NV4097_SET_VERTEX_DATA_ARRAY_OFFSET
#define CELL_GCM_METHOD_ADDRESS_VERTEX_DATA_ARRAY_FORMAT    CELL_GCM_NV4097_SET_VERTEX_DATA_ARRAY_FORMAT
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_OFFSET       CELL_GCM_NV4097_SET_VERTEX_TEXTURE_OFFSET
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_ADDRESS      CELL_GCM_NV4097_SET_VERTEX_TEXTURE_ADDRESS
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_CONTROL0     CELL_GCM_NV4097_SET_VERTEX_TEXTURE_CONTROL0
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_FILTER       CELL_GCM_NV4097_SET_VERTEX_TEXTURE_FILTER
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_CONTROL3     CELL_GCM_NV4097_SET_VERTEX_TEXTURE_CONTROL3
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_IMAGE_RECT   CELL_GCM_NV4097_SET_VERTEX_TEXTURE_IMAGE_RECT
#define CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_BORDER_COLOR CELL_GCM_NV4097_SET_VERTEX_TEXTURE_BORDER_COLOR
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_OFFSET              CELL_GCM_NV4097_SET_TEXTURE_OFFSET
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_ADDRESS             CELL_GCM_NV4097_SET_TEXTURE_ADDRESS
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_FILTER              CELL_GCM_NV4097_SET_TEXTURE_FILTER
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL0            CELL_GCM_NV4097_SET_TEXTURE_CONTROL0
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL1            CELL_GCM_NV4097_SET_TEXTURE_CONTROL1
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_IMAGE_RECT          CELL_GCM_NV4097_SET_TEXTURE_IMAGE_RECT
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_BORDER_COLOR        CELL_GCM_NV4097_SET_TEXTURE_BORDER_COLOR
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL2            CELL_GCM_NV4097_SET_TEXTURE_CONTROL2
#define CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL3            CELL_GCM_NV4097_SET_TEXTURE_CONTROL3
#define CELL_GCM_METHOD_ADDRESS_INVALIDATE_VERTEX_FILE      CELL_GCM_NV4097_INVALIDATE_VERTEX_FILE
#define CELL_GCM_METHOD_ADDRESS_INDEX_ARRAY_OFFSET          CELL_GCM_NV4097_SET_INDEX_ARRAY_ADDRESS
#define CELL_GCM_METHOD_ADDRESS_BEGIN_END                   CELL_GCM_NV4097_SET_BEGIN_END
#define CELL_GCM_METHOD_ADDRESS_DRAW_INDEX_ARRAY            CELL_GCM_NV4097_DRAW_INDEX_ARRAY
#define CELL_GCM_METHOD_ADDRESS_VERTEX_CONSTANT_LOAD        CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD

/*----------------------------------------------------------------------------------------------------
  Method Header

  The following declarations are provided as a reference implementation of Method Header creation.

  About CELL_GCM_METHOD and CELL_GCM_METHOD_NI

  CELL_GCM_METHOD and CELL_GCM_METHOD_NI are both declared in libgcm.

  CELL_GCM_METHOD creates method headers.
  The first paremeter is the method offset, which indicates the method address from which data write should start from.
  The second parameter is the method count which represents the number of data items to be written.
  By specifying method count appropriately, multiple data items can be set consecutively, starting from the method address
  indicated by the method offset.
  In other words, if the method count is 4, the first data item will be transferred to the method address pointed to
  by the method offset, the second data item will be transferred to the method address indicated by method offset + 0x04,
  the third data item to method offset + (0x04*2), and the fourth to method offset + (0x04*3).

     BitField:
     [0:2]   = 3b'000
     [3:13]  = Method Count
     [14:18] = 5b'00000
     [19:31] = Method Offset

  CELL_GCM_METHOD_NI creates a special method header which does NOT increment the method address.
  The first paremeter is the method offset which indicates the method address to which the data is written.
  The second parameter is the method count which represents the number of data items to be written.
  With this header, all subsequent method data is just pushed to the same address. (Note: NI stands for "Non Increment".)
  In other words, if method count is 4, all four data items will be transferred to the method address pointed to
  by the method offset.
  CELL_GCM_METHOD_NI is just provided for method address CELL_GCM_METHOD_ADDRESS_DRAW_INDEX_ARRAY only.
  because method data must be written to this same address continuously.
  Please don't use it for other method addresses. Using with other method addresses has not been verified.

     BitField:
     [0:2]   = 3b'010
     [3:13]  = Method Count
     [14:18] = 5b'00000
     [19:31] = Method Offset

  (Note: the binary value description used above is based on Verilog notation.
  For example, 3b'010 indicates a 3 bit value where [0:0] is 0, [1:1] is 1 and [2:2] is 0.)

  For further information, please see the "GCM Method Macro Overview" document.

  <Common Parameter>
     num:
        Number of subsequent Method Data items.
  --------------------------------------------------------------------------------------------------*/

/*
   --- Method Nop ---
   This is provided for Command buffer alignment.

   Note:
      The Command works like a Method Header which has method count 0.
*/

#define CELL_GCM_METHOD_NOP (0x00000000)

/*
   --- Method Header for Invalidate Vertex File ---
   
   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetDrawIndexArray
*/

#define CELL_GCM_METHOD_HEADER_INVALIDATE_VERTEX_FILE(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_INVALIDATE_VERTEX_FILE,num)

/*
   --- Method Header for Index Array Offset ---
   
   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetDrawIndexArray
*/

#define CELL_GCM_METHOD_HEADER_INDEX_ARRAY_OFFSET(num)	\
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_INDEX_ARRAY_OFFSET, num)

/*
   --- Method Header for Draw Index Array ---
   
   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetDrawIndexArray

   <Note:>
      Two versions of this macro are provided:
      CELL_GCM_METHOD_HEADER_DRAW_INDEX_ARRAY uses CELL_GCM_METHOD.
      CELL_GCM_METHOD_HEADER_DRAW_INDEX_ARRAY_NI uses CELL_GCM_METHOD_NI.
      (CELL_GCM_METHOD_HEADER_DRAW_INDEX_ARRAY_NI does not increment method address.)
*/

#define CELL_GCM_METHOD_HEADER_DRAW_INDEX_ARRAY(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_DRAW_INDEX_ARRAY, num)

#define CELL_GCM_METHOD_HEADER_DRAW_INDEX_ARRAY_NI(num) \
  CELL_GCM_METHOD_NI(CELL_GCM_METHOD_ADDRESS_DRAW_INDEX_ARRAY, num)

/*
   --- Method Header for Begin End ---
   
   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetDrawIndexArray
*/

#define CELL_GCM_METHOD_HEADER_BEGIN_END(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_BEGIN_END, num)

/*
   --- Method Header for Vertex Data Array format ---
   
   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexDataArrayFromat
   
   <Parameters:>
      index: The slot number of the vertex attribute. (0,1,2...15)
      num: 1,2,3 ... 16
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_DATA_ARRAY_FORMAT(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_DATA_ARRAY_FORMAT + (index) * 0x04, num)

/*
   --- Method Header for Vertex Data Array offset ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexDataArrayOffset

   <Parameters:>
      index: The slot number of the vertex attribute. (0,1,2...15)
      num: 1,2,3 ... 16
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_DATA_ARRAY_OFFSET(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_DATA_ARRAY_OFFSET + (index) * 0x04, num)

/*
   --- Method Header for Texture Offset ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1 or 8 x n [ n = 1 .. 16]

   <Note:>
      If you just need to change Texture Offset, num should be set to 1.
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_OFFSET(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_OFFSET + (index) * 0x20, num)

/*
   --- Method Header for Texture Address ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTextureAddress

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_ADDRESS(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_ADDRESS + (index) * 0x20, num)

/*
   --- Method Header for Texture Filter ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTextureFilter

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_FILTER(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_FILTER + (index) * 0x20, num)

/*
   --- Method Header for Texture Image Rect ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_IMAGE_RECT(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_IMAGE_RECT + (index) * 0x20, num)

/*
   --- Method Header for Texture Border Color ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTextureBorderColor

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_BORDER_COLOR(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_BORDER_COLOR + (index) * 0x20, num)

/*
   --- Method Header for Texture Control ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTextureControl

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_CONTROL0(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL0 + (index) * 0x20, num)

/*
   --- Method Header for Texture Control1 ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_CONTROL1(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL1 + (index) * 0x20, num)


/*
   --- Method Header for Texture Control2 ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTextureOptimization

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1,2,3...16
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_CONTROL2(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL2 + (index) * 0x04, num)

/*
   --- Method Header for Texture Control3 ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2...15)
      num: 1,2,3...16

   <Note:>
      This Method must also be used when replacing cellGcmSetTexture.
*/

#define CELL_GCM_METHOD_HEADER_TEXTURE_CONTROL3(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_TEXTURE_CONTROL3 + (index) * 0x04, num)

/*
   --- Method Header for Vertex Texture Offset ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTexture

   <Parameters:>
      index: the index of the vertex texture sampler. (0,1,2,3)
      num: 1 or 8 x n [ n = 1,2,3,4]

   <Note:>
      If you just need to change Vertex Texture Offset, num is set 1.
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_OFFSET(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_OFFSET + (index) * 0x20, num)

/*
   --- Method Header for Vertex Texture Address ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTextureAddress

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_ADDRESS(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_ADDRESS + (index) * 0x20, num)

/*
   --- Method Header for Vertex Texture Control ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTextureControl

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_CONTROL0(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_CONTROL0 + (index) * 0x20, num)

/*
   --- Method Header for Vertex Texture Filter ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTextureFilter

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_FILTER(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_FILTER + (index) * 0x20, num)

/*
   --- Method Header for Vertex Texture Border Color ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTextureBorderColor

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_BORDER_COLOR(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_BORDER_COLOR + (index) * 0x20, num)

/*
   --- Method Header for Vertex Texture Control3 ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_CONTROL3(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_CONTROL3 + (index) * 0x20, num)

/*
   --- Method Header for Vertex Image Rect ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexTexture

   <Parameters:>
      index: the index of the texture sampler. (0,1,2,3)
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TEXTURE_IMAGE_RECT(index, num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TEXTURE_IMAGE_RECT + (index) * 0x20, num)

/*
   --- Method Header for Vertex Program ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexProgram
      cellGcmSetVertexProgramLoad
      cellGcmSetVertexProgramLoadSlot

   <Parameters:>
      num: 4,8,12,16,20,24,28,32

   <Note:>
      Each ucode instruction is 128bits (i.e. 4 words).
      The transfer must be in 4-word units.
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_PROGRAM(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM, num)

/*
   --- Method Header for Vertex Program Load ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexProgram
      cellGcmSetVertexProgramLoad
      cellGcmSetVertexProgramLoadSlot

   <Parameters:>
      num:
         1 Set VertexProgramLoad only.
         2 Set VertexProgramLoad and VertexProgramLoadStart.
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_PROGRAM_LOAD(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM_LOAD, num)

/*
   --- Method Header for Vertex Program Start ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexProgramStartSlot
      cellGcmSetVertexProgram
      cellGcmSetVertexProgramLoad
      cellGcmSetVertexProgramLoadSlot

   <Parameters:>
      num: 1 Set VertexProgramLoadStart only.
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_PROGRAM_START(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_PROGRAM_START, num)


/*
   --- Method Header for Vertex Program Timeout ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexProgram
      cellGcmSetVertexProgramLoad
      cellGcmSetVertexProgramLoadSlot

   <Parameters:>
      num: 1
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_TIMEOUT(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_TIMEOUT, num)

/*
   --- Method Header for Vertex Program Constant Load ---

   <Libgcm function(s) corresponding to this macro:>
      cellGcmSetVertexProgram
      cellGcmSetVertexProgramLoad
      cellGcmSetVertexProgramLoadSlot

   <Parameters:>
      num: 1 - 33

   <Note:>
      When pushing a constant value, always start from this address.
*/

#define CELL_GCM_METHOD_HEADER_VERTEX_CONSTANT_LOAD(num) \
  CELL_GCM_METHOD(CELL_GCM_METHOD_ADDRESS_VERTEX_CONSTANT_LOAD, num)


/*----------------------------------------------------------------------------------------------------
  Method Data

  The following declarations are provided as reference implementations
  for creating Method Data for each method address.
  --------------------------------------------------------------------------------------------------*/

/*
   --- Method Data for Vertex Data Array Format ---

   <Valid Method Address:>
      0x1740 + 0x04*n
      (where n is the slot number of the vertex attribute, as in 0, 1, 2 ... 15)

   <Parameters:>
      frequency:
         Frequency of the vertex streams in the array (0 - 65535)

      stride:
         Offset bytes between consecutive vertices (0 - 255)

      size:
         Number of coordinates per vertex (0 - 4)

      type:
         The following values can be set to Type.
         0x1 CELL_GCM_VERTEX_S1
         0x2 CELL_GCM_VERTEX_F
         0x3 CELL_GCM_VERTEX_SF
         0x4 CELL_GCM_VERTEX_UB,
         0x5 CELL_GCM_VERTEX_S32K
         0x6 CELL_GCM_VERTEX_CMP
         0x7 CELL_GCM_VERTEX_UB256

   <Note:>
      When type == CELL_GCM_VERTEX_UB256, size must be 4.
*/


#define CELL_GCM_METHOD_DATA_VERTEX_DATA_ARRAY_FORMAT(frequency, stride, size, type) \
  CELL_GCM_ENDIAN_SWAP(((frequency) << 16) | ((stride) << 8) | ((size) << 4) | (type))

/*
   --- Method Data for Vertex Data Array Format ---

   <Valid Method Address:>
      0x1680 + 0x04*n
      (where n is the slot number of the vertex attribute as in, 0, 1, 2 ... 15)

   <Parameters:>
      location:
         Specify Memory location of memory.
         0x0 CELL_GCM_LOCATION_LOCAL Local memory
         0x1 CELL_GCM_LOCATION_MAIN  Main memory

      offset:
         For Offset, specify the location where the vertex attribute data is placed.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_DATA_ARRAY_OFFSET(location, offset)	\
  CELL_GCM_ENDIAN_SWAP(((location) << 31) | (offset))

/*
   --- Method Data for Texture Offset ---

   <Valid Method Address:>
      0x1a00 + n*0x20
      (where n is the index of the texture sampler, as in 0, 1, 2 ... 15)

   <Parameters:>
      offset:
         For Offset, specify the location where the actual texture data is placed.
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_OFFSET(offset)	\
  CELL_GCM_ENDIAN_SWAP(offset)

/*
   --- Method Data for Texture Border Format ---

   <Valid Method Address:>
      0x1a04 + n*0x20
      (where n is the index of the texture sampler, as in 0, 1, 2 ... 15)

   <Parameters:>
      location:
         Indicate whether the texture is on local memory or main memory.
         Value  Macro Description
         0x0    CELL_GCM_LOCATION_LOCAL Local memory
         0x1    CELL_GCM_LOCATION_MAIN  Main memory

      cubemap:
         Enable or disable cube mapping.
         Value  Macro             Description
         0x0    CELL_GCM_FALSE    Disable CubeMap
         0x1    CELL_GCM_TRUE     Enable  CubeMap

      dimension:
         Specify texture dimension.
         Value  Macro                          Description
         0x1    CELL_GCM_TEXTURE_DIMENSION_1   1D texture
         0x2    CELL_GCM_TEXTURE_DIMENSION_2   2D texture
         0x3    CELL_GCM_TEXTURE_DIMENSION_3   3D texture

      format:
         Set format as follows:
         format = (1) color format | (2) swizzle flag | (3) normalization flag
         About the details of format, please refer to the GCM Method Macros Overview.

      mipmap:
         For Mipmap, specify the number of levels the mipmap texture has.
         The value range is 1 - 13.

      border:
         Specify border type.
         Value  Macro                             Description
         0x00   CELL_GCM_TEXTURE_BORDER_TEXTURE   Border texture
         0x01   CELL_GCM_TEXTURE_BORDER_COLOR     Border color
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_BORDER_FORMAT(location, cubemap, dimension, format, mipmap, border) \
  CELL_GCM_ENDIAN_SWAP( ((location) + 1) | ((cubemap) << 2) | ((border) << 3) | ((dimension) << 4) | ((format) << 8) | ((mipmap) << 16) )


/*
   --- Method Data for Texture Format ---

   <Valid Method Address:>
      0x1a04 + n*0x20
      (where n is the index of the texture sampler, as in 0, 1, 2 ... 15)

   <Parameters:>
      location:
         Indicate whether the texture is on local memory or main memory.
         Value  Macro Description
         0x0    CELL_GCM_LOCATION_LOCAL Local memory
         0x1    CELL_GCM_LOCATION_MAIN  Main memory

      cubemap:
         Enable or disable cube mapping.
         Value  Macro             Description
         0x0    CELL_GCM_FALSE    Disable CubeMap
         0x1    CELL_GCM_TRUE     Enable  CubeMap

      dimension:
         Specify texture dimension.
         Value  Macro                          Description
         0x1    CELL_GCM_TEXTURE_DIMENSION_1   1D texture
         0x2    CELL_GCM_TEXTURE_DIMENSION_2   2D texture
         0x3    CELL_GCM_TEXTURE_DIMENSION_3   3D texture

      format:
         Set format as follows:
         format = (1) color format | (2) swizzle flag | (3) normalization flag
         About the details of format, please refer to the GCM Method Macros Overview.

      mipmap:
         For Mipmap, specify the number of levels the mipmap texture has.
         The value range is 1 - 13.
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_FORMAT(location, cubemap, dimension, format, mipmap) \
	CELL_GCM_METHOD_DATA_TEXTURE_BORDER_FORMAT( (location) , (cubemap) , (dimension) , (format) , (mipmap) , (0x01) )

/*
   --- Method Data for Texture Address ---

   <Valid Method Address:>
      0x1a08 + n*0x20
      (where n is the index of the texture sampler, as in 0, 1, 2 ... 15)

   <Parameters:>
      wrapr wrapt wraps:
         For Wraps, Wrapt, and Wrapr, specify how to adjust the provided S, T and R coordinates
         respectively when they exceed the size of the texture. The following values can be specified:

         Value Macro Description
         0x1   CELL_GCM_TEXTURE_WRAP
         Use the remainder from the division by the texture size as the coordinate value.
         Results in the texture being used in a repeat pattern.

         0x2 CELL_GCM_TEXTURE_MIRROR
         Use the coordinate value with the texture size as the turning point.
         Results in mirror images of the texture in a repeat pattern.

         0x3 CELL_GCM_TEXTURE_CLAMP_TO_EDGE
         Use the coordinate value clamped to the range of [1/2N, 1 - 1/2N].
         (N is the size of the texture in the direction of the clamping.)

         0x4 CELL_GCM_TEXTURE_BORDER
         Read out the texel value at the texture border
         0x5 CELL_GCM_TEXTURE_CLAMP
         Use the coordinate value clamped to the range of [0, 1]

      UnsignedRemap:
         For UnsignedRemap, specify the method of normalizing the read texel values (-128 to 127, or 0 to 255).
         The following values can be specified.
         Value  Macro                                  Description
         0x0    CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL Remap read value to the range of [0.0, 1.0]
         0x1    CELL_GCM_TEXTURE_UNSIGNED_REMAP_BIASED Remap read value to the range of [-1.0, 1.0]

      zfunc:
         For Zfunc, specify the testing method to be used on the depth texture when applying a shadow map.
         Value Macro                               Description
         0x0   CELL_GCM_TEXTURE_ZFUNC_NEVER        Never pass
         0x1   CELL_GCM_TEXTURE_ZFUNC_LESS         Pass if the texel value is less than the R coordinate value
         0x2   CELL_GCM_TEXTURE_ZFUNC_EQUAL        Pass if the texel value is equal to the R coordinate value
         0x3   CELL_GCM_TEXTURE_ZFUNC_LEQUAL       Pass if the texel value is less than or equal to the R coordinate value
         0x4   CELL_GCM_TEXTURE_ZFUNC_GREATER      Pass if the texel value is greater than the R coordinate value
         0x5   CELL_GCM_TEXTURE_ZFUNC_NOTEQUAL     Pass if the texel value is not equal to the R coordinate value
         0x6   CELL_GCM_TEXTURE_ZFUNC_GEQUAL       Pass if the texel value is greater than or equal to the R coordinate value
         0x7   CELL_GCM_TEXTURE_ZFUNC_ALWAYS       Always pass

      gamma:
         For Gamma, specify whether or not to apply sRGB reverse conversion (gamma reverse correction)
         when reading out the texture data.
         The value consists of 4 bits.
         The correspondance between the bit fields and the RGBA components is shown below:
            [0:0] Alpha ignored
            [1:1] Blue
            [2:2] Green
            [3:3] Red
         0x0 no conversion
         0x1 convert

      anisoBias:
         For AnisoBias, specify the bias to be applied to the anisotropic filter.
         The value range is 0 to 15.
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_ADDRESS( wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma, anisoBias) \
  CELL_GCM_ENDIAN_SWAP((wraps) | ((anisoBias) << 4) | ((wrapt) << 8) | ((unsignedRemap) << 12) | ((wrapr) << 16) | ((gamma) << 20) | ((zfunc) << 28))

/*
   --- Method Data for Texture Control0 ---

   <Valid Method Address:>
      0x1a0c + n*0x20 (where n is the index of the texture sampler, as in 0, 1, 2 ... 15)

   <Parameters:>
      enable:
         Value Macro Description
         0x0 CELL_GCM_FALSE Disable the texture sampler
         0x1 CELL_GCM_TRUE Enable the texture sampler

      minlod:
         Minimum LOD value of the texture reduction filter (0.0 - 12.0)
         12-bit unsigned fixed-point value [integer part 4 bits: decimal part 8 bits]

      maxlod:
         Maximum LOD value of the texture reduction filter (0.0 - 12.0)
         12-bit unsigned fixed-point value [integer part 4 bits: decimal part 8 bits]

      maxAniso:
         Maximum value for the sample level of the anisotropic filter

         Value Macro                         Description
         0x0   CELL_GCM_TEXTURE_MAX_ANISO_1  x1 sample
         0x1   CELL_GCM_TEXTURE_MAX_ANISO_2  x2 sample
         0x2   CELL_GCM_TEXTURE_MAX_ANISO_4  x4 sample
         0x3   CELL_GCM_TEXTURE_MAX_ANISO_6  x6 sample
         0x4   CELL_GCM_TEXTURE_MAX_ANISO_8  x8 sample
         0x5   CELL_GCM_TEXTURE_MAX_ANISO_10 x10 sample
         0x6   CELL_GCM_TEXTURE_MAX_ANISO_12 x12 sample
         0x7   CELL_GCM_TEXTURE_MAX_ANISO_16 x16 sample
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_CONTROL0(enable, minlod, maxlod, maxaniso)	\
  CELL_GCM_ENDIAN_SWAP(((maxaniso) << 4) | ((maxlod) << 7) | ((minlod) << 19) | ((enable) << 31))

/*
   --- Method Data for Texture Control1 ---

   <Valid Method Address:>
      0x1a10 + n*0x20
      (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>
      RemapOrder:
         Specify the order in which to map the two input elements (X and Y) to the four vectors (X, Y, Z and W) of the Cg program.

         Value Macro                             Description
         0x0   CELL_GCM_TEXTURE_REMAP_ORDER_XYXY Map as XYXY
         0x1   CELL_GCM_TEXTURE_REMAP_ORDER_XXXY Map as XXXY

      OutB, OutG, OutR, OutA:
         These specify the color output elements. Use the following values for each component:

         Value Macro                         Description
         0x0   CELL_GCM_TEXTURE_REMAP_ZERO   Output 0 to the color value
         0x1   CELL_GCM_TEXTURE_REMAP_ONE    Output 1 to the color value
         0x2   CELL_GCM_TEXTURE_REMAP_REMAP  Output the value specified for remap to the color value

      InB, InG, InR, InA:
         These specify the color input elements. Use the following values for each component.

         Value Macro                         Description
         0x0   CELL_GCM_TEXTURE_REMAP_FROM_A Select the alpha element of the color for the input value
         0x1   CELL_GCM_TEXTURE_REMAP_FROM_R Select the red element of the color for the input value
         0x2   CELL_GCM_TEXTURE_REMAP_FROM_G Select the green element of the color for the input value
         0x3   CELL_GCM_TEXTURE_REMAP_FROM_B Select the blue element of the color for the input value

   <Note:>
      The remap order takes on meaning if the texture is in one of the formats below. It will be ignored in all other formats.
      - CELL_GCM_TEXTURE_COMPRESSED_HILO8
      - CELL_GCM_TEXTURE_COMPRESSED_HILO_S8
      - CELL_GCM_TEXTURE_Y16_X16
      - CELL_GCM_TEXTURE_Y16_X16_FLOAT
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_CONTROL1(RemapOrder, OutB, OutG, OutR, OutA, InB, InG, InR, InA) \
  CELL_GCM_ENDIAN_SWAP(((RemapOrder) << 16) | \
		       ((OutB) << 14) | ((OutG) << 12) |  ((OutR) << 10) |  ((OutA) << 8) | \
		       ((InB) << 6) | ((InG) << 4) |  ((InR) << 2) | (InA))


/*
   --- Method Data for Texture Filter ---

   <Valid Method Address:>
      0x1a14 + n*0x20
      (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>

      bias:
         For bias, specify the bias value of the mipmap level to be applied when reducing the texture
         as a 13-bit signed fixed-point value [signed 1 bit : integer part 4 bits : decimal part 8 bits].
         Negative values are represented in 2's complement format.

         If the integer part is e and the decimal part is m, the actual value to be set in MipMapLodbias will be as follows.
         For a positive value: e + m/256
         For a negative value: e + m/256 - 16
         The range of values that can be specified for MipMapLodbias is -16.0 (set value 0x1000) to 15.996 (set value 0x0fff).

      min:
         For min, set the type of filter to use for reducing the texture.
         The following values can be specified.

         Value Macro
         0x1   CELL_GCM_TEXTURE_NEAREST
         Read the values of the texel closest to the texel coordinates and use them as the texture values.

         0x2 CELL_GCM_TEXTURE_LINEAR
         Read the values of the four texels closest to the texel coordinates
         and use the weighted average of these values as the texture values.
         Depending on the wrap mode, use the value of the texture border as well.

         0x3 CELL_GCM_TEXTURE_NEAREST_NEAREST
         Select the mipmap level closest to the texel coordinates and from a texture of that level,
         read the values of the texel closest to the texel coordinates and use them as the texture values.

         0x4 CELL_GCM_TEXTURE_LINEAR_NEAREST
         Select the mipmap level closest to the texel coordinates and from a texture of that level,
         read the values of the four texels closest to the texel coordinates
         and use the weighted average of these values as the texture values

         0x5 CELL_GCM_TEXTURE_NEAREST_LINEAR
         Select the two mipmap levels closest to the texel coordinates and from a texture of each level,
         read the values of the texel closest to the texel coordinates,
         and use the weighted averages of these values as the texture values
      
         0x6 CELL_GCM_TEXTURE_LINEAR_LINEAR
         Select the two mipmap levels closest to the texel coordinates and from a texture of each level,
         read the values of the four texels closest to the texel coordinates,
         take the weighted averages of these four values,
         and then use the weighted average of the values of the two levels as the texture values
      
         0x7 CELL_GCM_TEXTURE_CONVOLUTION_MIN
         Calculate the weighted average of 2x3 or 3x3 texels that is close to the texel coordinate.
         The calculation expression for the weighted average is specified with Conv.

      mag:
         For mag, specify the type of filter to use for enlarging the texture.

         Value Macro
         0x1   CELL_GCM_TEXTURE_NEAREST
         Read the values of the texel closest to the texel coordinates and use them as the texture values

         0x2   CELL_GCM_TEXTURE_LINEAR
         Read the values of the four texels closest to the texel coordinates
         and use the weighted average of these values as the texture values.
         Depending on the wrap mode,use the value of the texture border as well.

         0x4 CELL_GCM_TEXTURE_CONVOLUTION_MAG
         Calculate the weighted average of 2x3 or 3x3 texels that is close to the texel coordinate.
         The calculation expression for the weighted average is specified with Conv.
   
      conv:
         For conv, specify the calculation to be applied when using the convolution filter.
         The following values can be specified.

         Value Macro
         0x1   CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX
         Multiply the following to the 2x3 texel close to the texel coordinate
         [1 4 1] [1 0 1] * 1/8

         0x2   CELL_GCM_TEXTURE_CONVOLUTION_GAUSSIAN
         Multiply the following to the 3x3 texel close to the texel coordinate
         [1 2 1][2 4 2][1 2 1] * 1/16

         0x3   CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX_ALT
         Multiply the following to the 2x3 texel close to the texel coordinate
         [1 0 1] [1 4 1] * 1/8
         When not using the convolution filter, specify CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX.

   <Note:>
      Note that when the convolution filter is being used, mipmap cannot be used at the same time.
      The convolution filter is only enabled for a texture with a mipmap level of 0.
      Also, specify 0 to MaxAniso of TextureControl0.
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_FILTER(bias, min, mag, conv)	\
  CELL_GCM_ENDIAN_SWAP((bias) | ((conv) << 13) | ((min) << 16) | ((mag) << 24))

/*
   --- Method Data for Texture Filter Signed ---

   <Note:>
      This is an extension of CELL_GCM_METHOD_DATA_TEXTURE_FILTER(bias, min, mag, conv)

   <Parameters:>
      as rs gs bs:
         Each field specifies whether or not the corresponding color channel contains a sign bit.
         as=>A rs=>R gs=>G bs=>B

         Value Macro            Description
         0x0   CELL_GCM_FALSE   Disable bit
         0x1   CELL_GCM_TRUE    Enable bit
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_FILTER_SIGNED(bias, min, mag, conv, as, rs, gs, bs) \
  CELL_GCM_ENDIAN_SWAP((bias) | ((conv) << 13) | ((min) << 16) | ((mag) << 24) \
		       | ((as) << 28) | ((rs) << 29) | ((gs) << 30) | ((bs) << 31) )

/*
   --- Method Data for Texture Image Rect ---

   <Valid Method Address:>
      0x1a18 + n*0x20F
      (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>
      width:
         Texture width (1 - 4096)

      height:
         Texture height (1 - 4096)
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_IMAGE_RECT(height,width)	\
  CELL_GCM_ENDIAN_SWAP((height) | ((width) << 16))


/*
   --- Method Data for Texture Border Color ---

   <Valid Method Address:>
      0x1a1c + n*0x20
      (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>
      color:
         Specify the texture's border color value in A8R8G8B8 format.
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_BORDER_COLOR(color)	\
  CELL_GCM_ENDIAN_SWAP(color)

/*
   --- Method Data for Texture Control2 ---

   <Valid Method Address:>
      0x0b00 + n*0x04 (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>
      slope:
         The slope value to be used for optimizing mipmaps.
         It is represented as a 5bit unsigned floating point value, with zero bias, of the form e2m3 (i.e. with a 2bit exponent and 3bit mantissa).
         The bias is zero so it's evaluated as (1 + m/8) * 2^e, and therefore the slope can take any of the following values (corresponding
         to the raw values 0x0 to 0x1f, in that order):
           1.0, 1.125, 1.25, 1.375, 1.5,  1.625, 1.75, 1.875,
           2.0, 2.25,  2.5,  2.75,  3.0,  3.25,  3.5,  3.75,
           4.0, 4.5,   5.0,  5.5,   6.0,  6.5,   7.0,  7.5,
           8.0, 9.0,  10.0, 11.0,  12.0, 13.0,  14.0, 15.0.
         (Example: if slope = 0x8, then this represents the value 2.0)
         Initial value is 0x8.

      iso:
         Isotropic filter LOD calculation optimization flag.
         For iso specify the method for calculating the LOD with an isotropic filter.
         The isotropic filter is only enabled when the maxAniso argument of cellGcmSetTextureControl()
         is CELL_GCM_TEXTURE_MAX_ANISO_1.
         Specify one of the following values.

         Value Macro Description
         0x0   CELL_GCM_TEXTURE_ISO_LOW
         Calculate the LOD value from the partial differential value of the texture coordinate for 2 axes.
         The precision of the LOD value will decrease and calculation burden will increase.

         0x1   CELL_GCM_TEXTURE_ISO_HIGH
         Calculate the LOD value from the partial differential value of the texture coordinate for 4 axes.
         The precision of the LOD value will increase and calculation burden will decrease.
         Initial value is CELL_GCM_TEXTURE_ISO_HIGH

      aniso:
         Anisotropic filter LOD calculation optimization flag
         For aniso, specify the method for calculating the LOD with an anisotropic filter.
         The anisotropic filter is enabled when the maxAniso argument of cellGcmSetTextureControl()
         is CELL_GCM_TEXTURE_MAX_ANISO_2 or higher (other than CELL_GCM_TEXTURE_MAX_ANISO_1).
         Specify one of the following values.

         Value Macro Description
         0x0   CELL_GCM_TEXTURE_ANISO_LOW
         Calculate the LOD value from the difference vectors of the texture coordinate for 2 axes.
         The precision of the LOD value will decrease and calculation burden will decrease.

         0x1   CELL_GCM_TEXTURE_ANISO_HIGH
         Calculate the LOD value from the difference voectors of the texture coordinate for 4 axes.
         The precision of the LOD value will increase and calculation burden will increase.
         Initial value is CELL_GCM_TEXTURE_ANISO_HIGH
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_CONTROL2(slope, iso, aniso) \
  CELL_GCM_ENDIAN_SWAP((slope) | ((iso)<<6) | ((aniso)<<7) | (0x2d<<8))

/*
   --- Method Data for Texture Control3 ---

   <Valid Method Address:>
      0x1840 + n*0x04 (where n is the index of the texture sampler, as in, 0, 1, 2 ... 15)

   <Parameters:>
      pitch:
         Specify the pitch size of the texture.
         This value is the product of the texture width multiplied by the number of bytes per texel.
         Specify 0 when using the swizzle texture format or the DXTC compression format.

      depth:
         Texture depth (1 - 512)
*/

#define CELL_GCM_METHOD_DATA_TEXTURE_CONTROL3(pitch,depth)	\
  CELL_GCM_ENDIAN_SWAP((pitch) | ((depth) << 20))


/*
   --- Method Data for Vertex Texture Offset ---
   
   <Valid Method Address:>
      0x0900 + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      offset:
         For Offset, specify where the actual texture data is placed. Specify the offset value converted using cellGcmAddressToOffset() or equivalent.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_OFFSET(offset)	\
  CELL_GCM_ENDIAN_SWAP(offset)

/*
   --- Method Data for Vertex Texture Format ---

   <Valid Method Address:>
      0x0904 + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      location:
         Indicate whether the texture is on local memory or main memory.
         Value Macro                   Description
         0x0   CELL_GCM_LOCATION_LOCAL Local memory
         0x1   CELL_GCM_LOCATION_MAIN  Main memory

      dimension:
         Value Macro                          Description
         0x1   CELL_GCM_TEXTURE_DIMENSION_1   1D texture
         0x2   CELL_GCM_TEXTURE_DIMENSION_2   2D texture

      format:
         The following two formats can be used for vertex textures.
         - 0xbb
         CELL_GCM_TEXTURE_W32_Z32_Y32_X32_FLOAT | CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR
         Linear format, normalized texture coordinate, four 32-bit float values
         - 0xbc
         CELL_GCM_TEXTURE_X32_FLOAT | CELL_GCM_TEXTURE_LN | CELL_GCM_TEXTURE_NR
         Linear format, normalized texture coordinate, one 32-bit float value

      mipmap:
         For Mipmap, specify the number of levels the mipmap texture has.
         The value range is 1 - 13.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_FORMAT(location,dimension,format,mipmap) \
  CELL_GCM_ENDIAN_SWAP(((location) + 1) | ((dimension) << 4) | ((format) << 8) | ((mipmap) << 16))

/*
   --- Method Data for Vertex Texture Address ---

   <Valid Method Address:>
      0x0908 + n*0x20
      (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      wrapt [20:23], wraps [28:31]:
         For Wraps and Wrapt, specify the method of adjusting the given S and T coordinates respectively, when they exceed the size of the texture.
         Set one of the following values to each of Wraps and Wrapt.

         Value   Macro
         0x1     CELL_GCM_TEXTURE_WRAP
         Use the remainder from the division by the texture size, as the coordinate value.
         Results in the texture being used in a repeat pattern.

         0x2     CELL_GCM_TEXTURE_MIRROR
         Use the coordinate with the texture size as the turning point.
         Results in mirror images of the texture in a repeat pattern.

         0x3     CELL_GCM_TEXTURE_CLAMP_TO_EDGE
         Use the coordinate value clamped to the range of [1/2N, 1 - 1/2N].
         (N is the size of the texture in the direction of clamping.)

         0x4     CELL_GCM_TEXTURE_BORDER
         Read out the texel value at the texture border

         0x5     CELL_GCM_TEXTURE_CLAMP
         Use coordinate value clamped to the range of [0, 1]
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_ADDRESS(wraps,wrapt)	\
  CELL_GCM_ENDIAN_SWAP((wraps) | ((wrapt) << 8))

/*
   --- Method Data for Texture Control0 ---

   <Valid Method Address:>
      0x090c + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      enable:
         Enable/Disable the texture sampler.
         Value Macro            Description
         0x0   CELL_GCM_FALSE   Disable
         0x1   CELL_GCM_TRUE    Enable

      minLod:
         Minimum LOD value of the texture reduction filter (0.0 - 12.0)
         12-bit unsigned fixed-point value [integer part 4 bits: decimal part 8 bits]
   
      maxLod:
         Maximum LOD value of the texture reduction filter (0.0 - 12.0)
         12-bit unsigned fixed-point value [integer part 4 bits: decimal part 8 bits]

*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_CONTROL0(enable, minLod, maxLod) \
  CELL_GCM_ENDIAN_SWAP(((maxLod) << 7) | ((minLod) << 19) | ((enable) << 31))

/*
   --- Method Data for Texture Control3 ---

   <Valid Method Address:>
      0x0910 + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      Pitch:
         Specify the pitch size of the texture.
         This value is the product of the texture width multiplied by the number of bytes per texel.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_CONTROL3(pitch)	\
  CELL_GCM_ENDIAN_SWAP(pitch)

/*
   --- Method Data for Vertex Texture Filter ---

   <Valid Method Address:>
      0x0914 + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      bias:
         For bias, specify the bias value for the mipmap level to be applied when reducing the texture.
         The format is a 13-bit signed fixed-point value [signed 1 bit: integer part 4 bits: decimal part 8 bits].
         Negative values are represented in 2's complement format.

         If the integer part is e and the decimal part is m, the actual value to be set in bias will be as follows.

         For a positive value: e + m/256
         For a negative value: e + m/256 - 16

         The range of values that be specified for bias is -16.0 (set value 0x1000) to 15.996 (set value 0x0fff).
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_FILTER(bias)	\
  CELL_GCM_ENDIAN_SWAP(bias)

/*
   --- Method Data for Image Rect ---

   <Valid Method Address:>
      0x0918 + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      width:
         Texture width (1 - 4096)

      height:
         Texture height (1 - 4096)
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_IMAGE_RECT(height,width)	\
  CELL_GCM_ENDIAN_SWAP((height) | ((width) << 16))

/*
   --- Method Data for Border Color ---

   <Valid Method Address:>
      0x091c + n*0x20 (where n is the index of the vertex texture sampler, as in, 0, 1, 2, or 3)

   <Parameters:>
      color:
         Specify the texture's border color value in the A8R8G8B8 format.
         This value will be converted and used as a float value.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TEXTURE_BORDER_COLOR(color)	\
  CELL_GCM_ENDIAN_SWAP(color)

/*
   --- Method Data for Vertex Program Load ---

   <Valid Method Address:>
      0x1e9c

   <Parameters:>
      loadslot:
         Specify the number of the first instruction slot to which the vertex shader instructions will be loaded.
         An instruction slot in the range of 0 to 511 can be specified.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_PROGRAM_LOAD(loadslot)	\
  CELL_GCM_ENDIAN_SWAP(loadslot)

/*
  --- Method Data for Vertex Program ---

  <Valid Method Address:>
      0x0b80 + n*0x04 (where n corresponds to the transfer destination word)

  <Parameters:>
      raw:
         ucode to be transferred (Range: ucode which created in sce-cgc)

  <Note:>
     (1) This macro receives Vertex Shader's ucode. The transfer must be in 4-word units.
     (2) ucode produced by sce-cgc doesn't need CELL_GCM_ENDIAN_SWAP, so even in little endian environments, such as
         Windows or Linux on x86 CPU, the Vertex Shader's ucode doesn't need ENDIAN_SWAP applied to it.
     (3) Set Instruction slot must be in the range of 0 to 511.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_PROGRAM(raw) \
  (raw)

/*
   --- Method Data for Vertex Program Start ---

   <Valid Method Address:>
      0x1ea0

   <Parameters:>
      address:
         Specify the number of the vertex shader instruction slot from where the execution of the vertex shader program
         is to be started.
         For StartAddress, specify the slot number which contains the first vertex shader instruction.
         An instruction slot in the range of 0 to 511 can be specified.

         This address is equivalent to cellGcmSetVertexProgramStartSlot().
*/

#define CELL_GCM_METHOD_DATA_VERTEX_PROGRAM_START(start)	\
  CELL_GCM_ENDIAN_SWAP(start)

/*
   --- Method Data for Vertex Timeout ---

   <Valid Method Address:>
      0x1ef8

   <Parameters:>
      Count:
         For count, always specify 0xFFFF. Operation has not been verified on libgcm for any other value.

      RegisterCount:
         Set the number of registers for the vertex program to be used.
         When the number of registers is less than 32, the value 32 (0x20) will be used, when greater than 32, the value 48 (0x30)
         will be used.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_TIMEOUT(Count, RegisterCount) \
  CELL_GCM_ENDIAN_SWAP(((RegisterCount) << 16) | (Count))

#define CELL_GCM_METHOD_DATA_VERTEX_PROGRAM_TIMEOUT(Count, RegisterCount) \
 CELL_GCM_METHOD_DATA_VERTEX_TIMEOUT(Count, RegisterCount)
// CELL_GCM_METHOD_DATA_VERTEX_PROGRAM_TIMEOUT is naming bug in SDK210 Beta.
// CELL_GCM_METHOD_DATA_VERTEX_TIMEOUT should be used.


/*
   --- Method Data for Invalidate Vertex File ---

   This method flushes the on-chip post-transform vertex cache.

   <Valid Method Address:>
      0x1714

   <Parameters:>
      (no parameters)

   <Note:>
      Method Data must be "0"
      If set to any other value, an unexpected operation or an error may occur.
      This Method is related to a hardware bug. Please refer to the Gcm Method Macro Overview document.
*/

#define CELL_GCM_METHOD_DATA_INVALIDATE_VERTEX_FILE (0x00000000)

/*
   --- Method Data for Index Array offset ---

   <Valid Method Address:>
      0x181c

   <Parameters:>
      offset:
         For Offset, specify where the actual index data is placed.
         Specify the offset value converted using cellGcmAddressToOffset() or equivalent.

   <Note:>
      (1) offset must be two-byte aligned.
      But, generally, it is highly recommended to align data on their natural boundaries for best performance.

      (2)
      This Method is related to a hardware bug. Please refer to the Gcm Method Macro Overview document.
*/

#define CELL_GCM_METHOD_DATA_INDEX_ARRAY_OFFSET(offset) \
  CELL_GCM_ENDIAN_SWAP(offset)

/*
   --- Method Data for Index Array Format ---

   <Valid Method Address:>
      0x1820

   <Parameters:>
      type:
         For type, specify the type of array element specified in indices. Use one of the following values.
         Value Macro                               Description
         0x0   CELL_GCM_DRAW_INDEX_ARRAY_TYPE_32   Use a 4-byte index array
         0x1   CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16   Use a 2-byte index array

      location:
         For location, specify whether the index array data is on main memory or local memory. Use one of the following values.
         Value Macro                    Description
         0x0   CELL_GCM_LOCATION_LOCAL  Index array data is on local memory
         0x1   CELL_GCM_LOCATION_MAIN   Index array data is on main memory

   This Method is related to a hardware bug. Please refer to the Gcm Method Macro Overview document.
*/

#define CELL_GCM_METHOD_DATA_INDEX_ARRAY_FORMAT(location,type)	\
  CELL_GCM_ENDIAN_SWAP((location) | ((type) << 4))

/*
   --- Method Data for Begin End ---

   <Valid Method Address:>
      0x1808

   <Parameters:>
      mode:
         For mode, specify the rendering mode for building primitives.
         Use one of the following values:

         Value  Macro                           Description
         0x0                                    End of Rendering
         0x1  CELL_GCM_PRIMITIVE_POINTS         Render POINTS primitive
         0x2  CELL_GCM_PRIMITIVE_LINE_STRIP     Render LINE_STRIP primitive
         0x3  CELL_GCM_PRIMITIVE_LINE_LOOP      Render LINE_LOOP primitive
         0x4  CELL_GCM_PRIMITIVE_LINES          Render LINES primitive
         0x5  CELL_GCM_PRIMITIVE_TRIANGLES      Render TRIANGLES primitive
         0x6  CELL_GCM_PRIMITIVE_TRIANGLE_STRIP Render TRIANGLE_STRIP primitive
         0x7  CELL_GCM_PRIMITIVE_TRIANGLE_FAN   Render TRIANGLE_FAN primitive
         0x8  CELL_GCM_PRIMITIVE_QUADS          Render QUADS primitive
         0x9  CELL_GCM_PRIMITIVE_QUAD_STRIP     Render QUAD_STRIP primitive
         0xa  CELL_GCM_PRIMITIVE_POLYGON        Render POLYGON primitive

         Note that 0x0 is used to indicate the end of a primitive list specified by DrawIndexArray.

*/

#define CELL_GCM_METHOD_DATA_BEGIN_END(mode) \
  CELL_GCM_ENDIAN_SWAP(mode)

/*
   --- Method Data for Draw Index Array ---

   <Valid Method Address:>
      0x1824

   <Parameters:>
      count:
         Number of elements to render. [0,255]

      first:
         starting offset of the indices to be fetched.

   <Note:>
      The indices (in the index array) themselves cannot be larger than 20b
      as this is the largest index reference that IDX can handle.
*/

#define CELL_GCM_METHOD_DATA_DRAW_INDEX_ARRAY(first, count) \
  CELL_GCM_ENDIAN_SWAP((first) | ((count) << 24))

/*
   --- Method Data for Vertex Constant Load ---

   <Valid Method Address:>
      0x1efc

   <Parameters:>
      loadstart:
         Starting index of constant registers (0 - 467)

   <Note:>
      The maximum number of vertex program constants is 468,
      even though an error is only thrown when greater than 543
*/

#define CELL_GCM_METHOD_DATA_VERTEX_CONSTANT_LOAD(loadstart) \
  CELL_GCM_ENDIAN_SWAP(loadstart)

/*
   --- Method Data for Vertex Constant ---

   <Valid Method Address:>
      0x1f00 - 0x1f7c

   <Parameters:>
      value:
         Value to be transfered.

   <Note:>
      The maximum number of vertex program constants is 468,
      but the FE does not check this limitation.
*/

#define CELL_GCM_METHOD_DATA_VERTEX_CONSTANT(value) \
  CELL_GCM_ENDIAN_SWAP(value)

#endif /* __CELL_GCM_METHOD_DATA_H__ */
