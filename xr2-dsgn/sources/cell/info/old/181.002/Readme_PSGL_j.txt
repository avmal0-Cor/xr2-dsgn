[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 181.002
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、SDK181.001パッケージ (PS3_SDK-181_001.zip) に、
PSGL の新規機能と不具合修正を含めたものです。

SDK181.001の関連パッケージを展開した環境で、本パッケージを展開して
ご使用ください。

----------------------------------------------------------------------
パッケージ構成
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
変更点
----------------------------------------------------------------------
<新規機能>
- cellRescAdjustAspectRatio()のラッパ関数であるpsglRescAdjustAspectRatio()を
  追加しました。
- glGetIntegerv()の第一引数pnameにGL_MAX_COLOR_ATTACHMENTS_OESを
  指定出来るようにしました。

<仕様変更>
- Shader.cpp中の以下の関数を移動しました。
    void _jsCreatePushBuffer( _CGprogram *program )
    void _jsSetDefaultValuesVP( _CGprogram *program )
    void _jsSetDefaultValuesFP( _CGprogram *program )
    void _jsPrepareFPSharedPushBuffer( _CGprogram *program, jsFifoData * &jsGcmCurrent )

  (移動前) src/PSGL/jetstream/src/Raster/JSGCM/Shader.cpp
  (移動後) src/PSGL/jetstream/src/Raster/JSGCM/GCMShader.cpp

<不具合修正>
- 以下の不具合を修正しました。

  - 新しくレンダーターゲットを作成した際にcellGcmSetAntialiasingControl()を
    呼んでいませんでした。
    その為、psglCreateDeviceAuto()/psglCreateDeviceExtended()で設定した
    デフォルトのアンチエイリアスの設定を引き継いでしまっていました。

- 以下の不具合を修正しました。

  - PSGLのRESC機能を有効にした場合は内部で専用のメモリを確保していますが、
    psglDestroyDevice()を実行しても確保したバッファを解放していません。

- 以下の不具合を修正しました。

  - glTexImage2D()等の引数internalformatにGL_BGRAを指定するとGL_INVALID_ENUM
    エラーが返ってきて、正しく設定する事ができません。

- 以下の不具合を修正しました。

  - 高さよりも幅の値が小さいテクスチャに対してglGenerateMipMapsOES()を実行
    した際にRSX(R)が正しく処理出来ずにttyへエラーログが出力される事があり
    ます。

- 以下の不具合を修正しました。

  - シェーダ内の未使用の配列の要素に対してcgGetParameterResource()を実行した
    場合はCG_UNDEFINEDを返すべきですが、CG_Cを返していました。

- 以下の不具合を修正しました。

  - 共有パラメータが存在するフラグメントシェーダに対してcgCreateProgram()を
    実行した場合に正しくフラグメントシェーダが読み込めない事があります。

- 以下の不具合を修正しました。

  - メモリが不足している状況でglTextureReferenceSCE()を呼び、テクスチャを
    作成するとメモリの割り当てに失敗しますが、その際に正しくGL_OUT_OF_MEMORY
    エラーを返していません。
    テクスチャの作成はglFlush()のタイミングで行われますが、上記の不具合に
    よりassertを起こしたりする事があります。

- 以下の不具合を修正しました。

  - ２の乗数で無いDXTテクスチャに対してglCompressedTexImage2D()が正しくハンド
    リング出来ていません。
    なお、PSGL内部で管理しているテクスチャ情報はglFlush()等によって更新されま
    すが、そのタイミングにてttyへRSX(R)のエラーログが出力される事があります。

- 以下の不具合を修正しました。

  - PSGLdeviceParameters構造体のenableに
    PSGL_DEVICE_PARAMETERS_RESC_PAL_TEMPORAL_MODE を指定して
    psglCreateDeviceExtended()を呼び出すとpsglSwap()の内部でハングアップ
    する事があります。

----------------------------------------------------------------------
制限事項
----------------------------------------------------------------------
なし

----------------------------------------------------------------------
使用許諾・制限
----------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

----------------------------------------------------------------------
商標に関する注意書き
----------------------------------------------------------------------
"PLAYSTATION"は、株式会社ソニー・コンピュータエンタテインメント
の登録商標です。
パッケージ内に記載されている会社名、製品名は一般に各社の商標または
登録商標です。
なお、本文中に(R)、(TM)マークは明記していない場合があります。

----------------------------------------------------------------------
ソフトウェアのライセンスについて
----------------------------------------------------------------------
以下のライブラリを使用した場合、著作権、ライセンスの記述が必要ですが
libPSGL(debug version)のみに含まれていますので、ゲームアプリケーション
で使用するバージョン（opt/ultra-opt version)では記載が必要ありません。

libPSGL(debug version)
  http://sourceforge.net/project/showfiles.php?group_id=42360
  から取得したAladdin EnterprisesのMD5 Message-Digest Algorithmを
  使用しています。
  cell/license/others/Aladdin_Enterprises.txtを参照してください。
