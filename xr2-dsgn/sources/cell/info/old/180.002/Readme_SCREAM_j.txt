[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 180.002
                   Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved
======================================================================
このパッケージには、SCREAM ランタイムライブラリが含まれています。

SCREAM ランタイムライブラリは、以下に示す高度な機能を提供します。
- サウンドのスクリプティング
- プラグインのサポート
- 優先度ベースのボイス管理

----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
cell/
|---Readme_*.txt
|---SDK_doc
|     +---jp/en
|          +---pdf
|               +---sound_SCREAM
|                      |---SCREAM-Overview_*.pdf
|                      +---SCREAM-Reference_*.pdf
|---sample_data
|---samples
|    +---sound
|         +---SCREAM
+---target
     +---ppu
          |---include
          |    +---scream.h
          +---lib
               +---libscream.a

----------------------------------------------------------------------
180.002 の変更点
----------------------------------------------------------------------
[新規追加]
- MultiStream側の処理と同期をとる為にCellScreamInit構造体にmsFlagsメンバを
  追加しました。

- cellScreamStartSoundSystem()実行時にCellScreamInit構造体のmsFlagsが不正
  だった場合にCELL_SCREAM_ERROR_BAD_FLAGS を返すようにしました。
  また、この変更に伴い、上記のマクロCELL_SCREAM_ERROR_BAD_FLAGSをscream.hに
  追加しました。

- cellScreamMSSync()をCellScreamInit構造体のmsFlagsメンバの状態に依存したタイ
  ミング同期に変更しました。

[サンプルの変更]
- 以下のサンプルプログラムにてlibmstreamSPURS.aからlibmstreamSPURSMP3.aをリンク
  するように修正しました。
  cell/samples/sound/SCREAM/scream_routing
  cell/samples/sound/SCREAM/simple_scream

[SCREAM 概要]
｢3 初期化、アップデート、終了｣章
- ｢MultiStreamとSCREAMの初期化｣節: MultiStreamのタイムドメインモードに
  ついての記述を追加
- ｢SCREAM/MultiStreamのヒント｣節: SCREAMのタイミングが不正確である場合に
  ついての記述を追加

[SCREAMリファレンス]
｢構造体｣章
- ｢CellScreamInit｣節: msFlagsメンバおよび解説を追加

｢システム設定関数｣章
- ｢cellScreamStartSoundSystem｣節: 返り値CELL_SCREAM_ERROR_BAD_FLAGSを追加
- ｢cellScreamMSSync｣節: SCREAMとMultiStreamのタイミングについての解説を追加


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

