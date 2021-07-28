[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
sce-cgc.exe
   頂点シェーダ・フラグメントシェーダをビルドする際に用いられる
   コンパイラです。

   <使用方法>
    TypeB :
     sce-cgc -profile sce_vp_TypeB -o <file>.vpo <file>.cg
     sce-cgc -profile sce_fp_TypeB -o <file>.fpo <file>.cg

    rsx :
     sce-cgc -profile sce_vp_rsx -o <file>.vpo <file>.cg
     sce-cgc -profile sce_fp_rsx -o <file>.fpo <file>.cg

psgl_shader_builder.exe
   固定関数フラグメントシェーダをビルドする際に用いられるツールです。
   使用方法については、ドキュメントパッケージの
   graphics/PSGL-Programming_Guide_*.pdfを参照してください。
