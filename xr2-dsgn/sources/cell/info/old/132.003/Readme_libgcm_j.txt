[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime libgcm Library 132.003
                    Copyright(C) 2006 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、SDK 132.002のlibgcm, libgcm_pmを基に、以下の変更点を
含めたパッケージです。

最新のSDK132環境をご使用する場合は、PS3_SDK-132_002.zipを展開後に
本パッケージを展開してください。

---------------------------------------------------------------------
パッケージ構成
---------------------------------------------------------------------
cell/
|---Readme_libgcm*.txt
|---SDK_doc                 ドキュメントファイル
|---target
|    |---ppu
|    |    |---include
|    |    +---lib
|    +---spu
|         |---include
|         +---lib
+---samples                 サンプルディレクトリ

---------------------------------------------------------------------
SDK132.002からの変更点
---------------------------------------------------------------------
libgcm
======
<新規追加>
- cellGcmSetTransferLocation()のインライン関数を追加しました。

- libgcmのコマンド生成関数部をSPUのスタティックライブラリおよびインライン
  関数として提供します。詳細は、「libgcm リファレンス」を参照ください。

  これに伴い、以下のファイルを追加しました。
    target/spu/lib/libgcm_spu.a
    target/spu/include/cell/gcm_spu.h
    target/spu/include/cell/gcm/*.h

- 以下のサンプルを追加しました。
  samples/gcm/Dice-spu
  SPU上で生成される複数のコマンドバッファの使用方法を示すサンプル

<仕様変更>
- SPU版のライブラリ提供に伴い、PPUとSPUで共通の宣言及び定義を今までの
  ヘッダファイルから分離し、新設した以下のヘッダファイルに移動しました。
    gcm_enum.h : 列挙型定義ファイル
    gcm_error.h : エラー定義ファイル
    gcm_struct.h : 構造体定義ファイル
    gcm_commnad_c.h : C関数モード定義ファイル
    gcm_command_cpp_explicit.h : C++関数モード定義ファイル
                                （明示的コンテキストデータ）
    gcm_command_cpp_implicit.h : C++関数モード定義ファイル
                                （暗黙的コンテキストデータ）
    gcm_implementation_sub.h : PPU,SPUで共通に使用する
                               インライン関数定義ファイル
    gcm_global_implementation_sub.h : PPU,SPUで共通に使用する
                                      外部関数定義ファイル
    gcm_prototypes_sub.h : PPU,SPUで共通に使用するプロトタイプ宣言ファイル

  同様にして、PPUでのみ使用する宣言や定義を今までのヘッダファイルから分離し、
  新設した以下のヘッダファイルに移動しました。
     gcm_implementation_ppu.h : PPUでのみ使用する
                                インライン関数定義ファイル
     gcm_global_implementation_ppu.h : PPUで使用する外部関数定義ファイル
     gcm_prototypes_ppu.h : PPUでのみ使用するプロトタイプ宣言ファイル

<不具合修正>
- ポインタ型による浮動小数値から符号なし整数値への値渡しを行っていたコードを
  共用体を使って値渡しをするように修正を行いました。
  これにより、gcm_macros.h からCELL_GCM_ENDIAN_SWAP_F(), CELL_GCM_SWAP_
  HALFS_F()を削除し、gcm_struct.h に追加した共用体 CellGcmCastを使用する
  ようにしました。

  CellGcmCastによる値の渡しおよびメモリへの格納を行うよう修正した関数および
  マクロ一覧を以下に示します。

    [関数]
    cellGcmSetDepthBounds()
    cellGcmSetPointSize()
    cellGcmSetPolygonOffset()
    cellGcmSetPolygonStipplePattern()
    cellGcmSetVertexData4f()
    cellGcmSetVertexProgramParameterBlock()
    cellGcmSetVertexProgramConstants()
    cellGcmSetFogParams(),

    [マクロ]
    CELL_GCM_SET_POLYGON_STIPPLE_PATTERN()
    CELL_GCM_SET_VIEWPORT_OFFSET_AND_SCALE()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_16U_XYZ0()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_16U_XYZW()
    CELL_GCM_SET_TRANSFORM_CONSTANT_LOAD_AND_LOAD_32U()


- 列挙定数 CELL_GCM_FUNC_FUNC_ADD_SIGNED を CELL_GCM_FUNC_ADD_SIGNED に
  修正しました。

- samples/gcm/pmZcullにおいて、表示解像度に合せて表示サイズを変更するように
  修正しました。

libgcm_pm
=========
<新規追加>
- cellGcmSetPerfMonEvent()で設定する計測対象のイベントを追加しました。
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
使用許諾・制限
---------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

---------------------------------------------------------------------
商標に関する注意書き
---------------------------------------------------------------------
"PLAYSTATION"は株式会社ソニー・コンピュータエンタテインメントの
登録商標です。

パッケージ内に記載されている会社名、製品名は一般に各社の商標または
登録商標です。
なお、(R)、(TM)マークは明記していない場合があります。

---------------------------------------------------------------------
著作権について
---------------------------------------------------------------------
本ソフトウェアの著作権は、株式会社ソニー･コンピュータエンタテインメント
に帰属しています。

