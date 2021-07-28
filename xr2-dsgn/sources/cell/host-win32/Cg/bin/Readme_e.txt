[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                    Copyright(C) 2006 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================

sce-cgc.exe
   This is the compiler used to build vertex shaders and fragment shaders.

   Use as follows:
   <TypeB>
     sce-cgc -profile sce_vp_TypeB -o <file>.vpo <file>.cg
     sce-cgc -profile sce_fp_TypeB -o <file>.fpo <file>.cg
   <rsx>
     sce-cgc -profile sce_vp_rsx -o <file>.vpo <file>.cg
     sce-cgc -profile sce_fp_rsx -o <file>.fpo <file>.cg

psgl_shader_builder.exe
   This tool is used to build fixed function fragment shaders.
   Please refer to graphics/PSGL-Programming_Guide_*.pdf in
   the document package.
