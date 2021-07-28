/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _cg_sce_h_
#define _cg_sce_h_

#define cgSetParameter1f cgGLSetParameter1f
#define cgSetParameter2f cgGLSetParameter2f
#define cgSetParameter3f cgGLSetParameter3f
#define cgSetParameter4f cgGLSetParameter4f
#define cgSetParameter1d cgGLSetParameter1d
#define cgSetParameter2d cgGLSetParameter2d
#define cgSetParameter3d cgGLSetParameter3d
#define cgSetParameter4d cgGLSetParameter4d
#define cgSetParameter1fv cgGLSetParameter1fv
#define cgSetParameter2fv cgGLSetParameter2fv
#define cgSetParameter3fv cgGLSetParameter3fv
#define cgSetParameter4fv cgGLSetParameter4fv
#define cgSetParameter1dv cgGLSetParameter1dv
#define cgSetParameter2dv cgGLSetParameter2dv
#define cgSetParameter3dv cgGLSetParameter3dv
#define cgSetParameter4dv cgGLSetParameter4dv
#define cgSetMatrixParameterdr cgGLSetMatrixParameterdr
#define cgSetMatrixParameterfr cgGLSetMatrixParameterfr
#define cgSetMatrixParameterdc cgGLSetMatrixParameterdc
#define cgSetMatrixParameterfc cgGLSetMatrixParameterfc

#include <Cg/NV/cg.h>

#ifdef __cplusplus
extern "C" {
#endif

CGFX_API void psglFXInit();
CGFX_API void psglFXExit();

#ifdef __cplusplus
}
#endif

#endif
