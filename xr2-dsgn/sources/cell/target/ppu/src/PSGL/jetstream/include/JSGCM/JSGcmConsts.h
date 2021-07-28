/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * Contains JSGCM related constants, including vertex attribute constants.
 */

#ifndef _H_JSGCMCONSTS_H_
#define _H_JSGCMCONSTS_H_

#include <cell/gcm/gcm_macros.h>

// max surface/scissor/viewport dimension
#define JSGCM_MAX_RT_DIMENSION                      (CELL_GCM_MAX_RT_DIMENSION)

// a few texture consts
#define JSGCM_MAX_SHADER_TEXCOORD_COUNT             (CELL_GCM_MAX_SHADER_TEXCOORD_COUNT)
#define JSGCM_MAX_TEXIMAGE_COUNT                    (CELL_GCM_MAX_TEXIMAGE_COUNT)
#define JSGCM_MAX_LOD_COUNT                         (CELL_GCM_MAX_LOD_COUNT)
#define JSGCM_MAX_TEX_DIMENSION                     (CELL_GCM_MAX_TEX_DIMENSION)

// max attrib count
#define JSGCM_ATTRIB_COUNT                          16

// Names for each of the vertex attributes
#define JSGCM_ATTRIB_POSITION                       0
#define JSGCM_ATTRIB_VERTEX_WEIGHT                  1
#define JSGCM_ATTRIB_NORMAL                         2
#define JSGCM_ATTRIB_COLOR                          3
#define JSGCM_ATTRIB_SECONDARY_C OLOR               4
#define JSGCM_ATTRIB_FOG_COORD                      5
#define JSGCM_ATTRIB_PSIZE                          6
#define JSGCM_ATTRIB_UNUSED1                        7
#define JSGCM_ATTRIB_TEXCOORD0                      8
#define JSGCM_ATTRIB_TEXCOORD1                      9
#define JSGCM_ATTRIB_TEXCOORD2                      10
#define JSGCM_ATTRIB_TEXCOORD3                      11
#define JSGCM_ATTRIB_TEXCOORD4                      12
#define JSGCM_ATTRIB_TEXCOORD5                      13
#define JSGCM_ATTRIB_TEXCOORD6                      14
#define JSGCM_ATTRIB_TEXCOORD7                      15

// Names for the vertex output components:
#define JSGCM_ATTRIB_OUTPUT_HPOS                    0
#define JSGCM_ATTRIB_OUTPUT_COL0                    1
#define JSGCM_ATTRIB_OUTPUT_COL1                    2
#define JSGCM_ATTRIB_OUTPUT_BFC0                    3
#define JSGCM_ATTRIB_OUTPUT_BFC1                    4
#define JSGCM_ATTRIB_OUTPUT_FOGC                    5
#define JSGCM_ATTRIB_OUTPUT_PSIZ                    6
#define JSGCM_ATTRIB_OUTPUT_TEX0                    7
#define JSGCM_ATTRIB_OUTPUT_TEX1                    8
#define JSGCM_ATTRIB_OUTPUT_TEX2                    9
#define JSGCM_ATTRIB_OUTPUT_TEX3                    10
#define JSGCM_ATTRIB_OUTPUT_TEX4                    11
#define JSGCM_ATTRIB_OUTPUT_TEX5                    12
#define JSGCM_ATTRIB_OUTPUT_TEX6                    13
#define JSGCM_ATTRIB_OUTPUT_TEX7                    14

// viewport adjusting
#define JSGCM_SUBPIXEL_ADJUST                       (0.5/(1<<12))
#define JSGCM_VIEWPORT_EPSILON                      0.0f

// max vertex program constant slots
#define JSGCM_VTXPRG_MAX_CONST                      (CELL_GCM_VTXPRG_MAX_CONST)
#define JSGCM_VTXPRG_MAX_INST                       (CELL_GCM_VTXPRG_MAX_INST)
// ------------------------------------------------------------------

#endif
