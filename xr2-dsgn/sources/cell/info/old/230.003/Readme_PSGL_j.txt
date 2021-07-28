[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 PSGL Library Source Code 230.003
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
======================================================================
このパッケージには、PSGLライブラリのソースコードと再構築したバイナリが
含まれています。

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
  |---Readme_*.txt
  |---SDK_doc
  +---target
        +---ppu
        |   +---src
        |   |   +---PSGL (ソースコードディレクトリ)
        |   |       +---jetstream
        |   |       |   |---Makefile (makefile)
        |   |       |   +---PSGL.sln (Visual Studio 2003の
        |   |       |                 ソリューションファイル)
        |   |       +---cgc
        |   |       +---jsbuild (ビルド時のバイナリ出力用ディレクトリ)
        |   +---lib
        |   |   +---PSGL
        |   |       +---RSX (再構築版のバイナリ)
        |   |            |---debug
        |   |            |---gcmhud
        |   |            |---opt
        |   |            +---ultra-opt
        |   +---include (SDK公開ヘッダ)
        +---spu
            +---lib
            |   +---PSGL
            |       +---RSX (再構築版のバイナリ)
            |            |---debug
            |            |---gcmhud
            |            |---opt
            |            +---ultra-opt
            +---include (SDK公開ヘッダ)

----------------------------------------------------------------------
ビルド方法
----------------------------------------------------------------------
このパッケージを使ってPSGLライブラリの再構築を行う場合には、
別パッケージで提供している、PLAYSTATION(R)3 Programmer Tool Runtime
Library SDK 関連パッケージの展開や必要な環境変数の設定を行う
必要があります。

* Visual Studio 2003 および 2005 での再構築方法
  Visual Studio 2003 および 2005 を使用してPSGLライブラリの再構築する場合は、
  Visaul StudioインテグレーションをインストールしたProDG for
  PLAYSTATION(R)3も必要となります。別パッケージで提供している
  最新版のProDG for PLAYSTATION(R)3パッケージをご使用ください。
  また、File notesで制限事項などが記載されていますので参照してください。

  PSGLライブラリ全てを再構築するVisual Studio 2003 および 2005 のソリューション
  ファイルは以下の2つのファイルです。
  cell/target/ppu/src/PSGL/jetstream/PSGL-2003.sln
  cell/target/ppu/src/PSGL/jetstream/PSGL-2005.sln
  これらのファイルを使用してPSGLのバージョン(Debug, GcmHud, Opt, Ultra Opt)を
  指定しライブラリを再構築することが出来ます。

  再構築したPSGLのライブラリは、
  cell/target/ppu/src/PSGL/jsbuild/target 以下にSDKパッケージのcell/target以下と
  同じ構成で生成されます。

  生成されたライブラリをSDKで使用する場合は、
  cell/target/ppu/src/PSGL/jsbuild/target 以下を、SDKパッケージの
  cell/target以下にコピーしてください。

* MSYS や Linuxでの再構築方法
  cell/target/ppu/src/PSGL/jetstream/Makefileは、PSGLライブラリ全てを
  再構築するメイクファイルとなります。

  始めに、以下の環境変数が既に設定されていることを確認してください。
    export OS_VERSION=X_Y_Z
    export JS_OSDIR=$CELL_SDK
    export JS_RASTERIZER=JSGCM
   ただし、「X_Y_Z」はSDKパッケージのリリースバージョンとなります。
   例) SDK180.002の場合、「X_Y_Z」は「1_8_0」。

  次に、JS_DEBUG, JS_NO_ERROR_CHECK, JS_RUNTIMEの環境変数を追加します。
  各環境変数の値は、再構築するPSGLライブラリのバージョンによって
  異なりますので、再構築するバージョンに合わせて環境変数の値を設定
  してください。

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

  環境変数の設定したら、以下の場所でビルドを行います。
  > cd  cell/target/ppu/src/PSGL/jetstream
  > make

  PSGLのライブラリは、cell/target/ppu/src/PSGL/jsbuildに
  以下のような各バージョン毎ディレクトリのlib以下に生成されます。
    debugバージョン：
      cell/target/ppu/src/PSGL/jsbuild/cell-JSGCM-debugrsx-X_Y_Z/
    gcmhudバージョン：(現在、対応していません。)
      cell/target/ppu/src/PSGL/jsbuild/cell-JSGCM-opt-gcmhudrsx-X_Y_Z/
    ultra-optバージョン：
      cell/target/ppu/src/PSGL/jsbuild/cell_JSGCM-opt-nocheckrsx-X_Y_Z/
    optバージョン：
      cell/target/ppu/src/PSGL/jsbuild/cell-JSGCM-optrsx-X_Y_Z/
    ただし、「X_Y_Z」はSDKパッケージのリリースバージョンとなります。
    例) SDK180.003の場合、「X_Y_Z」は「1_8_0」。

  生成されたライブラリをSDKで使用する場合は、以下の場所にコピーしてください。
    $CELL_SDK/target/ppu/lib/PSGL/RSX/(debug, gcmhud, opt, ultra-opt)

* GCM HUD の再構築方法
  今までのPSGLHUD(hud)は、GCMHUD(gcmhud)で置き換えられています。
  その中にはlibgcm_hud.aを有効にする為のフック関数が含まれており、
  今までのPSGLHUDは含んでおりません。

  最新版のlibgcm_hud.aはPS3 Devnetの以下のURLよりダウンロードすることが
  出来ますのでご確認ください。
  (便宜上、最新のlibgcm_hud.aは別パッケージにて提供しています。）
    https://ps3.scedev.net/projects/rsx_tools

  libgcm_hud.aとPSGLのGCMHUDバージョンを使ってビルド及び実行する場合は
  「GCM HUD ガイド」のドキュメントに記載されているように、リンカオプションを
  以下のように変更する必要があります。(libgcm_cmd.aとlibgcm_sys_stub.aは
  リンクしないで下さい)。

    -lgcm_hud -lgcm_pm -lsysmodule_stub -lusbd_stub -lio_stub -lm -lstdc++ \
    -lsysutil_stub -lfs_stub -lsheap_stub

  GCMHUDの詳しい使い方についても「GCM HUD ガイド」のドキュメント
  を参照してください。

  SDKに含まれているPSGLHUDから今回のパッケージに含まれているGCMHUD
  バージョンに切り替える場合は、以下のように環境変数(CELL_PSGL_VERSION)を
  設定してからゲームアプリケーションをビルドして下さい。
    export CELL_PSGL_VERSION=gcmhud

----------------------------------------------------------------------
変更点
----------------------------------------------------------------------
本パッケージにはSDK230.001のPSGLに対しての変更点が含まれます。
変更点の詳細に関しては本パッケージに含まれる
"PlayStation(R) Graphics Library プログラミングガイド" および
"PlayStation(R) Graphics Library レファレンス" をご参照ください。

主な変更点は静的コマンドバッファAPIに関するものです。
ユーザが作成したコマンドバッファを使用した際に
PSGLがストールしうる状況をユーザがハンドルできるように
psglSetStallCallBack() APIが追加されました。
またpsglCallCommandBuffer()の返り値の型が変更され、
ユーザがコマンドバッファへの呼び出しを行った後の
FIFO 内での現在位置を示すvoid *ポインタを返すようになりました。
この機能に関しての説明は「静的コマンドバッファ」節の例62にあります。
この機能のサンプルとして StaticCommandBuffer_02 も更新されています。

glReadPixels() で上から下へスキャンされる形式のイメージが
取得できるように変更されました。
height引数に負の値を使用することにより指定可能です。


各ドキュメントの、前回リリースからの主な変更点を以下に列記します。
（明確化のための細かな文章校正がなされている箇所などは省略させて
いただきます。）

PlayStation(R) Graphics Library プログラミングガイド
------------------------------------------------------
｢12 PLAYSTATION(R)3に固有の機能｣章
- ｢静的コマンドバッファ｣節に、
  ｢例62：StaticCommandBuffer2コールバックストール｣を追加


PlayStation(R) Graphics Library リファレンス
------------------------------------------------------
｢PSGL API｣章
- ｢psglGetDeviceDimensions｣節において、冒頭の一行説明を
  ｢デバイスのサイズを取得する｣ に訂正 

｢OpenGL APIおよびSCE拡張API｣章
- ｢glReadPixels section｣節において、負数を引数heightの値に指定できる旨を
  以下のように反映：
  - 引数heightの説明にその旨を追加 
  - エラーGL_INVALID_VALUEの説明から、heightが負の値である際にエラーが返る
    旨を削除
  - 解説にその旨を追加  

- ｢psglCallCommandBuffer｣節において、定義の記述を
    void psglCallCommandBuffer(void *commandBuffer);
  から
    void *psglCallCommandBuffer(void *commandBuffer);
  に変更し、返り値の記述を追加

- ｢psglGetCommandRecordCurrent｣節に、返り値の記述を追加

- ｢psglSetStallCallBack｣節を追加
 
｢Cgランタイムライブラリ｣章
- ｢cgCompileProgram｣節において、返り値の記述を｢なし｣に訂正

｢PSGL SPUライブラリ｣章
- 各節の定義の記述において、#include行の記述を｢PSGL/spu_psgl.h｣に訂正

----------------------------------------------------------------------
制限事項
----------------------------------------------------------------------
- Visual Studioのソリューションファイル及びプロジェクトファイルを
  使用してビルドを行う為には、Microsoft Visual Studio .NET 2003 Service
  Pack 1 (KB918007)が必要です。

<動作保証について>
- ランタイムライブラリとしての動作保証につきましては以下の２点が条件となります。
  - *.a形式で提供しているバイナリのPSGLライブラリをそのまま使用している
  - libgcmのAPIを直接呼び出してPSGLのAPIと混在していない

  なお、変更の有無に関わらずソースコードからビルドして作成したバイナリについても
  ランタイムライブラリとしての動作保証から外れる形になります(ソースの内容を把握し
  た上でゲームアプリケーションからlibgcmの関数を直接呼び出し、PSGLのAPIと混在する
  場合も同様です)。

  つまり、各ライセンシ様にて作成された独自のランタイムライブラリとしての扱いと同
  等になりますので、この点を御理解の上で御利用下さい。

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

